#!/bin/sh
# Step7.
echo "Fill both card and system memory with known patterns,"
echo "  and save hardware (card DIMM)memory[20...1000] image for later comparison."
./scriptWriteMem -s 0x20 0x1000 pattern1.fill
./scriptWriteMem -h 0x20 0x1000 pattern2.fill
./scriptReadMem -h 0x100 0x100 step7ref
