#!/bin/sh
# Step18.
echo "Compare system (device driver) memory against the original Step7 image."
echo "Step8 and Step15 DMA transfers were both in the same direction,"
echo "    oncard DIMM->system memory. "
#
echo "DIFF comparison should Fail, showing that system memory "
echo "  has been altered since Step7."
./scriptReadMem -s 0x20 0x1000 step18
diff step18 pattern1.fill
