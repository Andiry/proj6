#!/bin/sh
# Step3.
echo "Read back piece of system (device driver) low memory[100...200] for later comparison."
./scriptReadMem -s 0x100 0x100 step3ref
