#!/bin/sh
PIPE_NAME="/tmp/peppymeterfifo"

# Check if the named pipe already exists
if [ ! -p $PIPE_NAME ]; then
    # Create the named pipe
    mkfifo $PIPE_NAME
fi  

# Activate python virtual env named pygame
alias activate=". ../pyenvs/pygame/bin/activate"
cd
cd PeppyMeter-master/
activate	

# Run peppymeter in the virtual env
python peppymeter.py &
cd 
./pi/pwstream4peppymet
