#!/bin/sh
# Step8.
echo "Write a DCB into hardware (card DIMM) memory and execute it."
echo "This DCB command should initiate a DMA whose direction is "
echo "   DMA source      = from hardware (card DIMM) memory[100...1FF]"
echo "   DMA destination = into system (device driver) memory[100...1FF]."
./scriptWriteMem -h 0x00 0x20 step8.dcb
./scriptExecute
