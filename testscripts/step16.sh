#!/bin/sh
# Step16.
echo "Now check count of interrupts."
echo "1. The count should be one greater than seen in step14."
echo "2. Now run 'dmesg' to see the ISR activity for the InProgress bit."
cat /proc/interrupts | grep expdrv
dmesg | tail -6
