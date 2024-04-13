# PeppyMeter-4-Linux-desktop
PeppyMeter VU-Meter for Linux desktop users listening to music
![Screenshot1](https://github.com/spagoc/PeppyMeter-4-Linux-desktop/assets/1367579/bdacbabb-ee81-4f31-989d-5a7a43f034db)

As a Linux user, I always appreciate having a visually appealing VU-meter displayed on my desktop while I'm listening to music. This repository contains my endeavor to integrate the PeppyMeter VU-Meter with the modern PipeWire sound server. I conducted this project using Ubuntu Desktop 2023.10.

PeppyMeter is coded in Python, so initially, I attempted to accomplish everything within Python. However, it seems that capturing the PipeWire stream in Python is currently not feasible (at least, based on my understanding from https://github.com/pablodz/pipewire_python/tree/main). Consequently, I opted to develop a C program instead."

## INSTALL
* Install and configure PeppyMeter:
  download and decompress source code master branch of PeppyMeter (repo in references below)
  change PeppyMeter config.txt with the one that is in this repo
* Peppymeter needs two python libraries, put them in your main env or or in a separated virtual env:
    ### $ pip install pygame requests  
* Compile peppymeter-start.c, that feeds PeppyMeter named pipe with a PipeWire stream:
    ### $ gcc -Wall pwstream4peppymet.c -o pwstream4peppymet $(pkg-config --cflags --libs libpipewire-0.3) -lm
  To avoid compilation you can use binary for Ubuntu that is in "Release".

## RUN
* Adapt, make executable and run the shell script: 
    ### peppymeter-start.sh. 
* You can also put it on the Ubuntu desktop and run it with a right-click and select "Run as a program" (see first screenshot).
* To stop the program just close the VU-meter window.

## EXTRA
* You can run qpwgraph or helvum and connect the desired PipeWire node to the PeppyMeter node (see qpwgraph in the second screenshot). That's can be useful for example if you have more active apps producing sounds and you want to VU-meter only one of them etc.
* Instead if you would like to have PeppyMeter connected to your mic input try to change "volume.max = 10.0" in your config.txt.
* To see all the VU-meters present in PeppyMeter change "blue" in "random" in your config.txt.

Enjoy!

## References and thanks:
* https://github.com/project-owner/PeppyMeter
* https://github.com/project-owner/PeppyMeter.doc
* https://pipewire.org
* https://docs.pipewire.org/audio-capture_8c-example.html, Copyright © 2022 Wim Taymans
* https://gitlab.freedesktop.org/rncbc/qpwgraph

  
![qpwgraph](https://github.com/spagoc/PeppyMeter-4-Linux-desktop/assets/1367579/fad59bef-24b3-44ec-a5dd-9a1e48fcce73)
