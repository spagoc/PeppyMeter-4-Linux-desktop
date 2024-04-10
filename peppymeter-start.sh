#!/bin/sh
PIPE_NAME="/tmp/peppymeterfifo"

# Check if the named pipe already exists
if [[ ! -p $PIPE_NAME ]]; then
    # Create the named pipe
    mkfifo $PIPE_NAME
    
alias activate=". ../pyenvs/pygame/bin/activate"
cd
cd PeppyMeter-master/
activate	
python peppymeter.py &
cd 
./pi/pwstream4peppymet

