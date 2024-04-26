#!/bin/sh
PIPE_NAME="/tmp/peppymeterfifo"

# Check if the named pipe already exists
if [ ! -p $PIPE_NAME ]; then
    # Create the named pipe
    mkfifo $PIPE_NAME
fi  

# If you use python virtual env (eg. named pygame) activate it
#alias activate=". ../pyenvs/pygame/bin/activate"
#cd
#cd PeppyMeter-master/
#activate	

# Run peppymeter in the virtual env
cd ~/PeppyMeter-master
python peppymeter.py &
./pwstream4peppymet
