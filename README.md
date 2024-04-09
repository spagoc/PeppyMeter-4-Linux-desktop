# PeppyMeter-4-Linux-desktop
PeppyMeter VU-Meter for Linux desktop users listening music
![Screenshot1](https://github.com/spagoc/PeppyMeter-4-Linux-desktop/assets/1367579/bdacbabb-ee81-4f31-989d-5a7a43f034db)

As a Linux user I would always like to have a beautiful VU-meter on the desktop while I listen to music.
This repo contains my effort to tie PeppyMeter VU-Meter to the modern PipeWire sound server. I used Ubuntu Desktop 2023.10.
PeppyMeter is written in Python so I tried to do all the work in Python but, at he moment, appears not yet possible to capture the PipeWire stream in Python (at least for my understanding of https://github.com/pablodz/pipewire_python/tree/main). So I decided to write a C program.

## INSTALL
* Install and configure PeppyMeter:
  download and decompress source code master branch of PeppyMeter (repo in references below)
  change PeppyMeter config.txt with the one that is in this repo
  create and configure the named pipe:
    ### $ mkfifo /tmp/peppymeterfifo
* Peppymeter needs two libraries, put them in your main env or or in a separated virtual env:
    ### $ pip install pygame requests  
* Compile peppymeter-start.c, that feeds PeppyMeter named pipe with a PipeWire stream:
    ### $ gcc -Wall pwstream4peppymet.c -o pwstream4peppymet $(pkg-config --cflags --libs libpipewire-0.3) -lm

## RUN
* Adapt, make executable and run the shell script: 
    ### peppymeter-start.sh. 
* You can also put it on the Ubuntu desktop and run it with a right-click and select "Run as a program" (see first screenshot).
* To stop the program just close the VU-meter window.

## EXTRA
* You can run qpwgraph or helvum and connect the desired PipeWire node to the PeppyMeter node. That's can be useful for example if you have more active apps producing sounds and you want to VU-meter only one of them.
* If you like to have PeppyMeter connected to your mic input try to change "volume.max = 10.0" in your config.txt.
* To see all the VU-meters present in PeppyMeter change "blue" in "random" in your config.txt.

Enjoy!

## References and thanks:
* https://github.com/project-owner/PeppyMeter
* https://github.com/project-owner/PeppyMeter.doc
* https://pipewire.org
* https://docs.pipewire.org/audio-capture_8c-example.html, Copyright © 2022 Wim Taymans
* https://gitlab.freedesktop.org/rncbc/qpwgraph

  
![qpwgraph](https://github.com/spagoc/PeppyMeter-4-Linux-desktop/assets/1367579/fad59bef-24b3-44ec-a5dd-9a1e48fcce73)
