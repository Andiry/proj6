#!/bin/sh
# Step11.
echo "Fill hardware (card DIMM) memory with a known fill pattern."
./scriptWriteMem -h 0x20 0x1000 pattern3.fill
