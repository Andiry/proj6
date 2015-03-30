#!/bin/sh
# Step14.
echo "Before execution in step15, get the latest count of INTS for our DMA device."
echo "After Step15 execution, we should see this count rise by one."
cat /proc/interrupts | grep expdrv
