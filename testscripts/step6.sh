#!/bin/sh
# Step6.
echo "Now compare hardware (card DIMM) memory against step3 image."
echo "This image should have been transferred to card memory[100...1FF],"
echo " and DIFF should compare the two as Equal."
./scriptReadMem -h 0x100 0x100 step6
diff step3ref step6
