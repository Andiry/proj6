#!/bin/sh
# Step1.
# See file 'readme.txt' for an overview of this demo sequence.
#
echo "Write a known fill pattern to "system" (device driver) memory[20...1000]."
./scriptWriteMem -s 0x20 0x1000 pattern1.fill
