#!/bin/sh
# Step10. 
echo "Compare system (device driver) mid-memory[100...1FF] against step7's"
echo " hardware (card DIMM) original memory image.  "
#
echo "Since step8 DMA was from card->device driver block,"
echo "   DIFF should show the two as Equal."
./scriptReadMem -s 0x100 0x100 step10
diff step10 step7ref
