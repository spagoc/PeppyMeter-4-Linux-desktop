// feed peppymeter named pipe with a pipewire stream
// $ gcc -Wall pwstream4peppymet.c -o pwstream4peppymet $(pkg-config --cflags --libs libpipewire-0.3) -lm
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <signal.h> 
#include <spa/param/audio/format-utils.h> 
#include <pipewire/pipewire.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
 
#define NAMED_PIPE_PATH "/tmp/peppymeterfifo" //in futuro leggere file di config di peppymeter

#define SLEEP_DURATION 15000 // 15 milliseconds (0.015 second), was //33000 // 33 milliseconds (0.033 second)
    
// globals
struct data {
        struct pw_main_loop *loop;
        struct pw_stream *stream; 
        struct spa_audio_info format;
        unsigned move:1;
};
int fd; 

/* our data processing function is in general:
 *
 *  struct pw_buffer *b;
 *  b = pw_stream_dequeue_buffer(stream);
 *
 *  .. consume stuff in the buffer ...
 *
 *  pw_stream_queue_buffer(stream, b);
 */
 
void convert_flot_to_two_bytes(float float_value, unsigned char *byte1, unsigned char *byte2) {
    /* convert a float parameter (from 0.0 e 1.0) to two bytes (from 0 e 2^16 - 1) */
    float_value = (float_value < 0.0f) ? 0.0f : float_value;
    float_value = (float_value > 1.0f) ? 1.0f : float_value;

    unsigned int integer_value = (unsigned int)(float_value * 65535.0f);

    *byte1 = (unsigned char)(integer_value >> 8);   
    *byte2 = (unsigned char)(integer_value & 0xFF); 
}

static void on_process(void *userdata)
{
        struct data *data = userdata;
        struct pw_buffer *b;
        struct spa_buffer *buf;
        float *samples, max;
        uint32_t c, n, n_channels, n_samples, peak;
 
        if ((b = pw_stream_dequeue_buffer(data->stream)) == NULL) {
                pw_log_warn("out of buffers: %m");
                return;
        }
 
        buf = b->buffer;
        if ((samples = buf->datas[0].data) == NULL)
                return;
 
        n_channels = data->format.info.raw.channels;
        n_samples = buf->datas[0].chunk->size / sizeof(float);
 
        /* move cursor up */
        if (data->move)
                fprintf(stdout, "%c[%dA", 0x1b, n_channels + 1);
        fprintf(stdout, "captured %d samples\n", n_samples / n_channels);

        unsigned char byte1 = 0x00; // Esempio di byte 1
        unsigned char byte2 = 0x45; // Esempio di byte 2
        for (c = 0; c < data->format.info.raw.channels; c++) {
                max = 0.0f;
                for (n = c; n < n_samples; n += n_channels)
                        max = fmaxf(max, fabsf(samples[n]));
 
                peak = SPA_CLAMP(max * 30, 0, 39);
 
                fprintf(stdout, "channel %d: |%*s%*s| peak:%f\n",
                                c, peak+1, "*", 40 - peak, "", max);
                convert_flot_to_two_bytes(max, &byte2, &byte1); //-0.0015
                //fprintf(stdout, "Float value:%f Byte 1:%d Byte 2: %d\n", max, byte1, byte2);

                // Write data to the named pipe
                if (write(fd, &byte1, sizeof(unsigned char)) != sizeof(unsigned char) ||
                    write(fd, &byte2, sizeof(unsigned char)) != sizeof(unsigned char)) {
                        perror("Failed to write to named pipe");
                        close(fd);
                        exit(EXIT_FAILURE);
                }
                //printf("Data written to named pipe successfully\n");

        } //end external for

        data->move = true;
        fflush(stdout); 
        pw_stream_queue_buffer(data->stream, b);

        usleep(SLEEP_DURATION); // not always
} // end on_process()
 
/* Be notified when the stream param changes. We're only looking at the
 * format changes.
 */
static void
on_stream_param_changed(void *_data, uint32_t id, const struct spa_pod *param)
{
        struct data *data = _data;
 
        /* NULL means to clear the format */
        if (param == NULL || id != SPA_PARAM_Format)
                return;
 
        if (spa_format_parse(param, &data->format.media_type, &data->format.media_subtype) < 0)
                return;
 
        /* only accept raw audio */
        if (data->format.media_type != SPA_MEDIA_TYPE_audio ||
            data->format.media_subtype != SPA_MEDIA_SUBTYPE_raw)
                return;
 
        /* call a helper function to parse the format for us. */
        spa_format_audio_raw_parse(param, &data->format.info.raw);
 
        fprintf(stdout, "capturing rate:%d channels:%d\n",
                        data->format.info.raw.rate, data->format.info.raw.channels);
 
}
 
static const struct pw_stream_events stream_events = {
        PW_VERSION_STREAM_EVENTS,
        .param_changed = on_stream_param_changed,
        .process = on_process,
};
 
static void do_quit(void *userdata, int signal_number) {
        struct data *data = userdata;
        pw_main_loop_quit(data->loop);
}

int main(int argc, char *argv[])
{
        struct data data = { 0, };
        const struct spa_pod *params[1];
        uint8_t buffer[1024];
        struct pw_properties *props;
        struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));
 
        pw_init(&argc, &argv);
 
        /* make a main loop. If you already have another main loop, you can add
         * the fd of this pipewire mainloop to it. */
        data.loop = pw_main_loop_new(NULL);
 
        pw_loop_add_signal(pw_main_loop_get_loop(data.loop), SIGINT, do_quit, &data);
        pw_loop_add_signal(pw_main_loop_get_loop(data.loop), SIGTERM, do_quit, &data);

        /* Create a simple stream, the simple stream manages the core and remote
         * objects for you if you don't need to deal with them.
         *
         * If you plan to autoconnect your stream, you need to provide at least
         * media, category and role properties.
         *
         * Pass your events and a user_data pointer as the last arguments. This
         * will inform you about the stream state. The most important event
         * you need to listen to is the process event where you need to produce
         * the data.
         */
        props = pw_properties_new(PW_KEY_MEDIA_TYPE, "Audio",
                        PW_KEY_CONFIG_NAME, "client-rt.conf",
                        PW_KEY_MEDIA_CATEGORY, "Capture",
                        PW_KEY_MEDIA_ROLE, "Music",
                        NULL);
        if (argc > 1)
                /* Set stream target if given on command line */
                pw_properties_set(props, PW_KEY_TARGET_OBJECT, argv[1]);
 
        /* capture from the sink monitor ports */
        pw_properties_set(props, PW_KEY_STREAM_CAPTURE_SINK, "true");
        
        // Create or open the named pipe for writing
        fd = open(NAMED_PIPE_PATH, O_WRONLY); 
        if (fd == -1) {
            perror("Failed to open named pipe");
            exit(EXIT_FAILURE);
        }
        data.stream = pw_stream_new_simple(
                        pw_main_loop_get_loop(data.loop),
                        "PeppyMeter",
                        props,
                        &stream_events,
                        &data);
 
        /* Make one parameter with the supported formats. The SPA_PARAM_EnumFormat
         * id means that this is a format enumeration (of 1 value).
         * We leave the channels and rate empty to accept the native graph
         * rate and channels. */
        params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat,
                        &SPA_AUDIO_INFO_RAW_INIT(
                                .format = SPA_AUDIO_FORMAT_F32));
 
        /* Now connect this stream. We ask that our process function is
         * called in a realtime thread. */
        pw_stream_connect(data.stream,
                          PW_DIRECTION_INPUT,
                          PW_ID_ANY,
                          PW_STREAM_FLAG_AUTOCONNECT |
                          PW_STREAM_FLAG_MAP_BUFFERS |
                          PW_STREAM_FLAG_RT_PROCESS,
                          params, 1);
 
        /* and wait while we let things run */
        pw_main_loop_run(data.loop);
 
        pw_stream_destroy(data.stream);
        pw_main_loop_destroy(data.loop);
        pw_deinit();

        // Close the named pipe
        //close(fd);
        return 0;
}


