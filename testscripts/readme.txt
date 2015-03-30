#
# Demo Overview.
#
# Before running the GUI or the testscripts, 
#    it is necessary to load the device driver.
#
# Simply type 'sudo ./loadmodule' to get the device driver running.
#
# Running the GUI or the testscripts must be done as root.
# 'sudo ./step1.sh' and so forth, one step at a time.
#
# Step1.
# Write a known fill pattern to "system" (device driver) memory.
#    ./scriptWriteMem -s 0x20 0x1000 pattern1.fill
#
# Step2.
# Write a known fill pattern to hardware (Card DIMM) memory.
# Then read back that memory image for later comparison.
#    ./scriptWriteMem -h 0x20 0x1000 pattern2.fill
#    ./scriptReadMem  -h 0x20 0x1000 step2ref
#
# Step3.
# Read back a small part of system (device driver) low memory,
# for later comparison.
#    ./scriptReadMem -s 0x100 0x100 step3ref
#
# Step4. (step4-ioctl.sh or step4-mmap.sh)
# Write a DCB into hardware (card DIMM) memory, then execute it.
# This DCB command causes a DMA transfer from system (device driver) memory
# to hardware (oncard DIMM) memory.
# step4-ioctl.sh use ioctl mechanism to access card register I/O block,
# while step4-mmap.sh use mmap mechanism to access card register I/O block,
#    ./scriptWriteMem -h 0x00 0x20 step4.dcb
#    ./scriptExecute
#
# Step5.
# Now that step4 command execution is done,
# read hardware (card DIMM) memory and compare it to step2's image.
# DIFF comparison should fail if DMA command executed OK. 
#    ./scriptReadMem -h 0x20 0x1000 step5
#    diff step2ref step5
#
# Step6.
# Now compare hardware (card DIMM) memory against step3 image.
# This image should have been transferred to card memory,
#  and DIFF should compare the two as Equal.
#    ./scriptReadMem -h 0x100 0x100 step6
#    diff step3ref step6
#
# Step7.
# Fill both card and system memory with known patterns,
# and save hardware (card DIMM)memory image for later comparison.
#    ./scriptWriteMem -s 0x20 0x1000 pattern1.fill
#    ./scriptWriteMem -h 0x20 0x1000 pattern2.fill
#    ./scriptReadMem -h 0x100 0x100 step7ref
#
# Step8.
# Write a DCB into hardware (card DIMM) memory and execute it.
# This DCB command should initiate a DMA whose direction is
# DMA source      = from hardware (card DIMM) memory
# DMA destination = into system (device driver) memory.  
#    ./scriptWriteMem -h 0x00 0x20 step8.dcb
#    ./scriptExecute
#
# Step9.
# Now that step8 command has executed, read the system (device driver)
# destination memory for comparison against its original fill pattern
# in step7. DIFF comparison should show a change in System memory.
#    ./scriptReadMem -s 0x20 0x1000 step9
#    diff step9 pattern1.fill
#
# Step10. 
# Compare system (device driver) mid-memory against step7's
# hardware (card DIMM) original memory image.
#
# Since step8 DMA was from card->device driver block,
#   DIFF should compare the two as Equal.
#    ./scriptReadMem -s 0x100 0x100 step10
#    diff step10 step7ref
#
# Step11.
# Fill hardware (card DIMM) memory with a known fill pattern.
#    ./scriptWriteMem -h 0x20 0x1000 pattern3.fill
#
# Step12.
# Save hardware (card DIMM) memory image at offset [0x800..0x900]
#  for later comparison.
#    ./scriptReadMem -h 0x800 0x100 step12ref
#
# Step13.
# Write a DCB into hardware (card DIMM) memory, but do not execute it.
# This DCB command will request an INTx at the end of the DMA transfer.
# DMA transfer will be from oncard memory to System memory.
#    ./scriptWriteMem -h 0x00 0x20 step13.dcb
#
# Step14.
# Before execution in step15, get the latest count of INTS for our DMA device.
# After Step15 execution, we should see this count rise by one.
#    cat /proc/interrupts | grep expdrv
#
# Step15.
# Now execute the step13 DCB, which performs a 
# DMA transfer followed by an interrupt request.
#    ./scriptExecute
#
# Step16.
# Now check count of interrupts.
# The count should be one greater than seen in step14.
# Also read the system syslog using dmesg, which will show the ISR
#  activity necesary to see and cancel the InProgress bit.
#    cat /proc/interrupts | grep expdrv
#
# Step17.
# Compare system (device driver) memory against the step12 
# original hardware (card DIMM) memory image.
#
# Since Step13/Step15 DMA xfer was from oncard memory to System memory,
# DIFF should show that System memory now matches Step12 oncard memory.
#    ./scriptReadMem -s 0x800 0x100 step17
#    diff step17 step12ref
#
# Step18.
# Compare system (device driver) memory against the original Step7 image.
# Step8 and Step15 DMA transfers were both in the same direction,
#   oncard DIMM->system memory. 
#
# DIFF comparison should Fail, showing that system memory has been
# altered since Step7.
#     ./scriptReadMem -s 0x20 0x1000 step18
#     diff step18 pattern1.fill
#
# Step 19.
# DMA operation has completed, which should set the InProgress bit.
# Read this register block, then compare it against the "Reset" value.
# Diff comparison should Fail, indicating that this bit was set in Step15.
#     ./scriptReadMem -r 0x04 0x01 step19
#     diff step19 NotInProgress.fill
#
# Step 20.
# Now that the InProgress bit has been examined, it's time to Reset it as an ISR would do.
# This bit will remain reset until the end of the next DMA transfer.
# After resetting it with a Write, read back its value, 
#   comparing it against a known 'Reset' fill pattern.
#
# DIFF comparison should pass, showing no difference between the two Reset images.
#     ./scriptWriteMem -r 0x04 0x01 NotInProgress.fill
#     ./scriptReadMem -r 0x04 0x01 step20
#     echo "Comparing reset-pattern files."
#     diff step20 NotInProgress.fill
