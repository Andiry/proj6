//
//  program to initiate execution of a PCIe Explorer DCB block.
//  use this after loading a command into the card with scriptWriteMem.
//
//  For now, will always assume DCB is stored at 0x00 offset.
//  Later on, might add an argv for nonzero DCB locations.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include "../common.hpp"
#include "../dcb.h"
#include "../module/common.h"
#include <iostream>
#include <sstream>
#include <fstream>

int gfd;
void *gSysAddr;
void *gDevAddr;
void *gCtrlAddr;
void *gCtrlAddrBase;

CardMem card1;

using namespace std;

// Load Explorer card's config info from device driver into user-space struct...
void LoadHWInfo(int m_fd)
{
  unsigned int datum;
   
  datum = 0x0;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.VendorID = (datum & 0xFFFF);  // DWord 0 LSB.
  card1.DeviceID = (datum >> 16);     // DWord 0 MSB.

  datum = 0x4;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.Command = (datum & 0xFFFF);   // DWord 01 LSB.
  card1.Status = (datum >> 16);       // DWord 01 MSB.

  datum = 0x8;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.RevisionID = (datum & 0xFF);   // DWord 02 LSB.
  card1.ClassCode = (datum >> 8);      // DWord 02 MSB.

  datum = 0xC;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.CacheLineSize = ((datum & 0x000000FF) >> 0);       // DWord 03 Byte0.
  card1.MasterLatencyTimer = ((datum & 0x0000FF00) >> 8);  // DWord 03 Byte1.
  card1.HeaderType = ((datum & 0x00FF0000) >> 16);         // DWord 03 Byte2.
  card1.BIST = ((datum & 0xFF000000) >> 24);               // DWord 03 Byte3.

  datum = 0x10;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.BAR0 = datum;     // DWord 04.
  
  datum = 0x14;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.BAR1 = datum;     // DWord 05.

  datum = 0x18;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.BAR2 = datum;     // DWord 06.

  datum = 0x1C;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.BAR3 = datum;     // DWord 07.
 
  datum = 0x20;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.BAR4 = datum;     // DWord 08.

  datum = 0x24;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.BAR5 = datum;    // DWord 09.

  datum = 0x28;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.CardbusCISPointer = datum; // DWord 10.

  datum = 0x2C;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.SubsystemVendorID = (datum & 0x0000FFFF);   // DWord 11 Byte0-1.
  card1.SubsystemID = ((datum & 0xFFFF0000) >> 16); // DWord 11 Byte2-3.
 
  datum = 0x30;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.ExpansionROMBaseAddress = datum;  // DWord 12.

  datum = 0x34;
  ioctl(m_fd, EXP_IOCXCFGDATA, &datum);
  card1.CapabilitiesPointer = (datum & 0x000000FF);  // DWord 13 Byte0.
}


void ShowCardCtrl(size_t offset, size_t blksize)
{
  
  char *ptr = (char *)gCtrlAddr; // base addr card register I/O block

  for ( unsigned int i=offset; i<(offset+blksize); ++i) {
      cout << (int)(ptr[i] & 0xFF) << " " << endl;   // display
  }
}


// Write filedata out to card ctrl I/O register
// Use DWord access.
void WriteToCardCtrl(size_t byteoffset, size_t blksize, char *inpfn)
{
  unsigned int *addr = (unsigned int*)gCtrlAddr;
  unsigned int filebyte;
  int err;

  FILE *fp = fopen(inpfn, "r");

  for ( unsigned int i=byteoffset; i<(byteoffset+blksize); i+=4) {
      unsigned int Kdword = 0;
      for (int j=0; j<4  && !feof(fp); ++j) {
          err = fscanf(fp, "%X", &filebyte); // LSB first...
          Kdword |= (filebyte & 0xFF) << (j*8);
      }
      // cout << (unsigned long)&(addr[i/4]) << "=" << Kdword << endl;
      addr[i/4] = Kdword; // array of dword.
  }
  
  fclose(fp);
}



int main(int argc, char *argv[])
{
  int res = 0;
  gfd = -1;
  gSysAddr = NULL;
  gDevAddr = NULL;

  char inpfn[80];

  // Add argv[] for the day when nonzero DCB offsets may be specified.
  unsigned long phyCardOffset = 0;

  cout.setf(ios::hex, ios::basefield);
  cout.setf(ios::showbase);
  
  gfd = open("/dev/explorer", O_RDWR);
  if (gfd == -1)
  {
    printf("Couldn't open /dev/explorer\n");
    return( -1);
  }

  // only mmap control I/O block 
  gCtrlAddrBase = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, gfd, (off_t)0x00000000);

  // control I/O block start at 0x0C00
  gCtrlAddr = (void *)((unsigned long)gCtrlAddrBase + 0x0C00);

  // load up the PCI Config given to the card this bootup cycle.
  LoadHWInfo(gfd);

  if (argc <= 1)   // use ioctl mechanism to access card register I/O block
  {
    // set up base address into card's DIMM memory, where DCB will be found.
    // usually should be at 0xC0000000 as assigned at PCI enumeration time.
    unsigned long phyCardBase = card1.BAR2 + ((unsigned long)card1.BAR3 << 32); // from HW.
    phyCardBase &= ~0x7F;  // mask off PCIe Base Register attribute bits.
    unsigned long phyCardAddr = phyCardBase + phyCardOffset;

    cout <<  "Sending Execute, DCB offset=" << phyCardAddr << endl;
    ioctl(gfd, EXP_IOCTSTARTDMA, phyCardAddr);
    cout <<  "gCtrlAddr:" << gCtrlAddr << endl;


  } else {       // use mmap mechanism to access card register I/O block
    char *endtoken;
    size_t optionOffset = strtol(argv[1], &endtoken, 16);
    size_t optionBlkSize = strtol(argv[2], &endtoken, 16);
    strcpy(inpfn, argv[3]);

    ShowCardCtrl(optionOffset, optionBlkSize);
    WriteToCardCtrl(optionOffset, optionBlkSize, inpfn);
    ShowCardCtrl(optionOffset, optionBlkSize);
  }

  close(gfd);
  
  munmap(gCtrlAddrBase, 4096);

  return res;
}

