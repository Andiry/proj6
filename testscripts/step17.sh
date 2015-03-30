#!/bin/sh
# Step17.
echo "Compare system (device driver) memory against the step12 "
echo "  original hardware (card DIMM) memory image."
#
echo "Since Step13/Step15 DMA xfer was from oncard memory to System memory,"
echo "   DIFF should show that System memory now matches Step12 oncard memory."
./scriptReadMem -s 0x800 0x100 step17
diff step17 step12ref
