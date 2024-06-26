# PeppyMeter-4-Linux-desktop
PeppyMeter VU-Meter for Linux desktop users listening to music

![Screenshot1](https://github.com/spagoc/PeppyMeter-4-Linux-desktop/assets/1367579/bdacbabb-ee81-4f31-989d-5a7a43f034db)

As a Linux user, I would always appreciate having a visually appealing VU-meter displayed on my desktop while I'm listening to music. This repository contains my endeavor to integrate the PeppyMeter VU-Meter with the modern PipeWire sound server. 

PeppyMeter is coded in Python, so initially, I attempted to accomplish everything within Python. However, it seems that capturing the PipeWire stream in Python is currently not feasible (at least, based on my understanding of https://github.com/pablodz/pipewire_python/tree/main). Consequently, I opted to develop a C program instead.

## INSTALL
1. Install PeppyMeter (https://github.com/project-owner/PeppyMeter):
   download the .zip and decompress it in your home dir.
2. Change PeppyMeter config.txt with the one that is in this repo.
3. Peppymeter needs python pygame library. Install it directly (or use a separated Python virtual env):  
   ####  (Ubuntu)
   ```bash
     sudo apt install python-is-python3 python3-pygame 
   ```        
   ####  (Fedora)     
   ```bash
     pip install pygame  
   ```
4. Download the binary file "pwstream4peppymet" that is in the "Releases" section, it feeds PeppyMeter named pipe with a PipeWire stream.     
   Or compile pwstream4peppymet.c:
     ####  (Ubuntu)
     ```bash  
     sudo apt install libpipewire-0.3-dev
     ```

     ```bash     
     gcc -Wall pwstream4peppymet.c -o pwstream4peppymet $(pkg-config --cflags --libs libpipewire-0.3) -lm
     ```
6. Put the binary file "pwstream4peppymet" in the ~/PeppyMeter-master dir and make it executable.
7. Put peppymeter-start.sh in the ~/Desktop dir or where you prefer (if you used Python virtual env adapt the script shell). Make it executable.

## RUN
* Right-click on peppymeter-start.sh and select "Run as a program" (see first screenshot).
* To stop the program just close the VU-meter window.

## EXTRA
* You can run qpwgraph or helvum and connect the desired PipeWire node to the PeppyMeter node in the audio graph (see qpwgraph in the second screenshot). That's can be useful for example if you have more active apps producing sounds and you want to VU-meter only one of them etc.
* Instead if you would like to have PeppyMeter connected to your mic input try to change "volume.max = 10.0" in your config.txt.
* To see all the VU-meters present in PeppyMeter change "blue" in "random" in your config.txt (I prefer the blu one because it reminds me the McIntosh hi-end amps).

## DISTROS
Tested on Ubuntu 23.10, Ubuntu 24.04 LTS, Fedora 40.  

## References and thanks:
* https://github.com/project-owner/PeppyMeter
* https://github.com/project-owner/PeppyMeter.doc
* https://pipewire.org
* https://docs.pipewire.org/audio-capture_8c-example.html, Copyright © 2022 Wim Taymans
* https://gitlab.freedesktop.org/rncbc/qpwgraph

  
![qpwgraph](https://github.com/spagoc/PeppyMeter-4-Linux-desktop/assets/1367579/fad59bef-24b3-44ec-a5dd-9a1e48fcce73)
