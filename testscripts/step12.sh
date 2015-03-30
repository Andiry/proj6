#!/bin/sh
# Step12.
echo "Save hardware (card DIMM) memory image at offset [0x800..0x900]"
echo " for later comparison."
./scriptReadMem -h 0x800 0x100 step12ref
