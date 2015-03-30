#!/bin/sh
# Step5.
echo "Now that step4 command execution is done,"
echo "read hardware (card DIMM) memory[20...1000] and compare it to step2's image."
echo "DIFF comparison should fail (show diffs) if DMA command executed OK."
./scriptReadMem -h 0x20 0x1000 step5
diff step2ref step5
