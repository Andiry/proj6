#!/bin/sh
# Step9.
echo "Now that step8 command has executed, read the system (device driver)"
echo "  destination memory for comparison against its original fill pattern in step7."
echo "DIFF comparison should show a change in System memory[20...1000]."
./scriptReadMem -s 0x20 0x1000 step9
diff step9 pattern1.fill
