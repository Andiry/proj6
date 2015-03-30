#!/bin/sh
# Step4.
echo "Write a DCB into hardware (card DIMM) memory, then execute it."
echo "This DCB command causes a DMA transfer"
echo " -from system (device driver) memory[100...1FF]"
echo " -to hardware (oncard DIMM) memory[100...1FF]."
./scriptWriteMem -h 0x00 0x20 step4.dcb
./scriptExecute
#./scriptExecute 0x00 0x08 step4.ctl
