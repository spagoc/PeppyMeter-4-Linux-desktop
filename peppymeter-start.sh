#!/bin/sh
alias activate=". ../pyenvs/pygame/bin/activate"
cd
cd PeppyMeter-master/
activate	
python peppymeter.py &
cd 
./pi/pwstream4peppymet

