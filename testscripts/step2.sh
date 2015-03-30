#!/bin/sh
# Step2.
echo "Write a known fill pattern to hardware (Card DIMM) memory[20...1000]."
echo "Then read back that memory image for later comparison."
./scriptWriteMem -h 0x20 0x1000 pattern2.fill
./scriptReadMem  -h 0x20 0x1000 step2ref
