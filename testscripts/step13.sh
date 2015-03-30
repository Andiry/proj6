#!/bin/sh
# Step13.
echo "Write a DCB into hardware (card DIMM) memory, but do not execute it."
#
echo "This DCB command will request an INTx at the end of the DMA transfer."
echo "DMA will be from oncard memory[800...8FF] to System memory[800...8FF]."
./scriptWriteMem -h 0x00 0x20 step13.dcb
