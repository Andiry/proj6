#!/usr/bin/perl
#
# parse a memory block fetched via scriptReadMem.
# Assume that a DCB starts a offset 0x00 in the file.
#   Within that DCB, assume all src/dest values are Offsets
#   above card/system memory block base addresses.
#
#   This is required since these base addresses are not
#   known until runtime, and change with each execution.
#

use strict;

my @ByteArray=[[0]];
my @DWord;



#------

my $wordcount=0;
my $bytecount=0;

while (<>) {
  my $i = hex;  # implied $_
  $ByteArray[$wordcount][$bytecount] = $i;
  $bytecount++;
  if ($bytecount > 3) {$bytecount=0; $wordcount++; }
}
print "------------------\n";

# array is filled from infile.
# now put bytes into dwords, dump it.
for ($wordcount=0; $ByteArray[$wordcount]; $wordcount++) {

         my $j = $ByteArray[$wordcount][0];
         $j += $ByteArray[$wordcount][1] << 8;
         $j += $ByteArray[$wordcount][2] << 16;
         $j += $ByteArray[$wordcount][3] << 24;

         $DWord[$wordcount] = $j;

         my $str = sprintf("DWord %x = %08x \n", $wordcount, $j );
         print $str;
      }

# ... or else parse the DCB by field...
print "------------------\n";
printf("Status  =0x%08x \n" , $DWord[0]);
printf("Command =0x%08x \n" , $DWord[1]);
printf("SystemAddr=%08x_%08x \n", $DWord[2], $DWord[3]); # 64-bit
printf("CardAddr  =%08x_%08x \n", $DWord[4], $DWord[5]); # 64-bit
printf("Iterations=0x%08x \n", $DWord[6]);
printf("Size      =0x%08x \n", $DWord[7]);
