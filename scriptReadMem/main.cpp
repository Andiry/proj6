//
//  application to write a diskfile after reading one of:
//        - config space of a PCIe Explorer card.
//        - card memory hardware on a PCIe Explorer card.
//        - system memory buffer reserved by the Explorer card device driver.
//
//  memory block within Card memory or System memory can be of two formats:
//        - DCB block, 32 bytes long.
//        - Memory fill pattern or test pattern.
//
//  blocks starting at offset==0x00 are always interpreted as DCB format.
//
//  NOTE:  Since:
//           - PCI-Config base address of hardware
//           - and mapping address between application and device-driver buffer
//         can change with each reboot and each execution of this application, the DCB
//         structure stored into diskfiles cannot hope to contain accurate runtime
//         values for these (re)mapped locations. Therefore, stored DCB files will contain
//         offsets into cardmem/devicedriver buffer areas, rather than absolute 'physical' addresses.
//
//         Further, any pointer containing a 'physical' address is not that at all. The kernel
//         memory manager dereferences these pointers (see mm kernel code) into the actual
//         hardware address anyway. A true physical pointer is no longer possible nor supported
//         with a running kernel.
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

#define SYS_MEM_SIZE 64*1024

// globals
int gfd;
void *gSysAddr;
void *gCtrlAddrBase;
void *gCardAddr;

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



// Display contents of user-space struct...
void ShowHWInfo()
{
  cout << "Vendor ID = " << card1.VendorID << "\n";  // DWord 0 LSB.
  cout << "Device ID = " << card1.DeviceID << "\n";  // DWord 1 MSB.

  cout << "Command = " << card1.Command  << "\n";    // Dword 01 LSB.
  cout << "Status = " <<  card1.Status << "\n";      // DWord 01 MSB.

  cout << "Revision ID = " << card1.RevisionID << "\n";  // DWord 02 LSB.
  cout << "Class Code = " << card1.ClassCode << "\n";     // DWord 02 MSB.

  cout << "Cache Line Size = " <<       card1.CacheLineSize << "\n";  // DWord 03 Byte0.
  cout <<  "Master Latency Timer = " << card1.MasterLatencyTimer << "\n";  // DWord 03 Byte1.
  cout << "Header Type = " <<           card1.HeaderType << "\n"; // DWord 03 Byte2.
  cout <<  "BIST = " <<                 card1.BIST       << "\n"; // DWord 03 Byte3.

  cout <<  "BAR0 = " << card1.BAR0 << "\n";                  // DWord 04.
  cout <<  "BAR1 = " << card1.BAR1 << "\n";                  // DWord 05.
  cout <<  "BAR2 = " << card1.BAR2 << "\n";                  // DWord 06.
  cout <<  "BAR3 = " << card1.BAR3 << "\n";                  // DWord 07.
  cout <<  "BAR4 = " << card1.BAR4 << "\n";                  // DWord 08.
  cout <<  "BAR5 = " << card1.BAR5 << "\n";                  // DWord 09.
  cout <<  "Cardbus CIS Pointer = " << card1.CardbusCISPointer << "\n";   // DWord 10 d.

  cout <<  "Subsystem Vendor ID = " << card1.SubsystemVendorID << "\n";   // DWord 11 Byte0-1.
  cout <<  "Subsystem ID = "        << card1.SubsystemID  << "\n";        // DWord 11 Byte2-3.

  cout <<  "Expansion ROM Base Address = " << card1.ExpansionROMBaseAddress << "\n"; // DWord 12. 

  cout <<  "Capabilities Pointer = " << card1.CapabilitiesPointer << "\n";  // DWord 13 Byte0.
}


// -------------------------
unsigned long fetch_dcb_dword(dcb_t dcb, int byteOffset)
{
  unsigned long result=0;
  
  for (int i=byteOffset; i<byteOffset+4; i++) {
      result += ((unsigned long)(dcb.byte[i]) << (i-byteOffset)*8);
  }
  return(result);
}

// -------------------------
dcb_t store_dcb_dword(dcb_t dcb, int byteOffset, unsigned int wrtval)
{
  for (int i=byteOffset; i<byteOffset+4; i++) {
      dcb.byte[i] = 0xFF & (wrtval >> ((i-byteOffset)*8));
      // printf("wrtval-> %04x -> DCB[%x] \n", dcb.byte[i], i);
  }
  return(dcb); 	
}


// -------------------------
// DCB qword[0] holds Status and Command fields.
// DCB qword[1] holds 'System Address' (device driver DMA buffer) field.
// DCB qword[2] holds 'Internal Address' (card onboard DIMM physical address) field.
// DCB qword[3] holds Iterations and Size fields.
//
dcb_t ConvertFromPhysicalAddrToOffset(dcb_t dcb)
{
  // 1. fetch actual values from HW or devdriver buffer memory.
  // 2. compute offset = runtime value - runtime base addr of memblock.  	
  // 3. then put Offset values back into dcb struct, ready for outfile.
  unsigned int ms_dword;
  unsigned int ls_dword;
  unsigned long mask;

  // Explorer Rev1.0 firmware places most-significant dword at lowest address.
  unsigned long phySysAddr = 0;
  phySysAddr += fetch_dcb_dword(dcb, 8) << 32;   // "system address" in spec.
  
  // followed by least-significant dword.   	
  phySysAddr += fetch_dcb_dword(dcb, 12);

  unsigned long phySysBase = (unsigned long) gSysAddr;
  unsigned long phySysOffset = phySysAddr - phySysBase;

  if (phySysAddr < phySysBase) {
         cout << "Error: Bad DCB[SysAddr] system address (devdriver buffer) loaded into hardware." << endl;
         cout << "       Correct hardware settings and retry this run." << endl;
  }

  cout << "SysBase=" << phySysBase << endl;  // debug displays.
  cout << "SysAddr=" << phySysAddr << endl;
  cout << "SysOffset=" << phySysOffset << endl;

  ms_dword = phySysOffset >> 32;
  dcb = store_dcb_dword(dcb, 8, ms_dword);

  // followed by least-significant dword. 
  mask = 0xFFFFFFFF;  // avoids main.cpp:192: warning: left shift count >= width of type
  ls_dword = phySysOffset & mask;
  dcb = store_dcb_dword(dcb, 12, ls_dword);         // system (devdriver) sddress into DCB qword[1].

  // System (device driver DMA buffer) offset conversion done.
  // ------------------------------------------------------------------
  // Now for the HW offset configured into card BAR hardware. 

  // Explorer Rev1.0 firmware places most-significant dword at lowest address.
  unsigned long phyCardAddr = 0;
  phyCardAddr += fetch_dcb_dword(dcb, 16) << 32;   // "internal address" in spec.

  // followed by least-significant dword.   	
  phyCardAddr += fetch_dcb_dword(dcb, 20);

  unsigned long phyCardBase = card1.BAR2 + ((unsigned long)card1.BAR3 << 32); // from HW.
  phyCardBase &= ~0x7F;  // mask off PCIe Base Register attribute bits.
  unsigned long phyCardOffset = phyCardAddr - phyCardBase;

  if (phyCardAddr < phyCardBase) {
         cout << "Error: Bad DCB[CardAddr] internal address loaded into hardware." << endl;
         cout << "       Correct hardware settings and retry this run." << endl;
  }
  
  cout << "CardBase=" << phyCardBase << endl;  // debug displays.
  cout << "CardAddr=" << phyCardAddr << endl;
  cout << "CardOffset=" << phyCardOffset << endl;

  ms_dword = phyCardOffset >> 32;
  dcb = store_dcb_dword(dcb, 16, ms_dword);

  // followed by least-significant dword. 
  mask = 0xFFFFFFFF;  // avoids main.cpp:192: warning: left shift count >= width of type
  ls_dword = phyCardOffset & mask;
  dcb = store_dcb_dword(dcb, 20, ls_dword);         // HW address into DCB qword[2].

  // Internal Address value (card DIMM address) is now converted to Offset for DCB-file use. 
  
  return(dcb);   
}



// debug display.
void ShowDCB(dcb_t dcb)
{
  for ( unsigned int i=0; i<32; ++i) {    
        if (i%8 == 0) {
        	cout << "qword" << i/8 << endl;
        }
        if (i%4 == 0) {
        	cout << " dword" << i/4 << "=" << dcb.qword[i/8].dword[i/4] << endl;
        }
        if (i%2 == 0) {
        	cout << "  word" << i/2 << "=" << dcb.qword[i/8].word[i/2] << endl;
        }

  	    int j = dcb.byte[i];
        cout << "    byte " << i << "=" << j << endl;
  }
}





// show card DIMM memory contents
void ShowSysMem(size_t offset, size_t blksize)
{
  unsigned int byteval;
  unsigned char rd_buf[SYS_MEM_SIZE];

  read(gfd, rd_buf, SYS_MEM_SIZE);
  for ( unsigned int i=offset; i<offset+blksize; ++i) {
      byteval = (rd_buf[i] & 0xFF);
      cout << i << "=" << byteval << " " << endl;   // demo display
  }	
}

// write card's DIMM to outfile. 
void ReadFromSysMem(size_t offset, size_t blksize, char *outfn)
{
  unsigned char rd_buf[SYS_MEM_SIZE];
  dcb_t dcb;

  if (offset+blksize > SYS_MEM_SIZE) {
  	cout << "Error: System blocksize exceeded, limited to " << SYS_MEM_SIZE << endl;
  	return;
  }
  
  read(gfd, rd_buf, SYS_MEM_SIZE);  // fetch everything from hardware here...
  
  FILE *fp = fopen(outfn, "w");
  if (offset == 0x00) 
  {
    for ( unsigned int i=offset; i<32; i++) {
        dcb.byte[i] = (int)(rd_buf[i] & 0xFF);   // pour rd_buf into common dcb struct.
    }
    dcb = ConvertFromPhysicalAddrToOffset(dcb);
    // ShowDCB(dcb);
    for ( unsigned int i=0; i<32; i++) {    
        fprintf(fp, "0x%02X \n", dcb.byte[i]);
    }  	
  } else {
    for ( unsigned int i=offset; i<(offset+blksize); i++) {
      fprintf(fp, "0x%02X \n", (int)(rd_buf[i] & 0xFF));
    }
  }	
  fclose(fp);
}


// show the card DIMM buffer
void ShowCardMem(size_t offset, size_t blksize)
{
  char *ptr = (char *)gCardAddr; // base addr of card onboard DIMM memblock
  
  for ( unsigned int i=offset; i<(offset+blksize); ++i) {
      cout << (int)(ptr[i] & 0xFF) << " " << endl;   // demo display
  }	
}

// Write out contents of card DIMM block
void ReadFromCardMem(size_t offset, size_t blksize, char *outfn)
{
  char *ptr = (char *)gCardAddr; // base addr of card device memory
  FILE *fp = fopen(outfn, "w");
  
  if (offset == 0x00) 
  {
  	dcb_t dcb;
    for ( unsigned int i=0; i<32; ++i) {    
        dcb.byte[i] = (int)(ptr[i] & 0xFF);
    }
    dcb = ConvertFromPhysicalAddrToOffset(dcb);
    // ShowDCB(dcb);
    for ( unsigned int i=0; i<32; ++i) {    
        fprintf(fp, "0x%02X \n", dcb.byte[i]);
    }  	
  } else {
    for ( unsigned int i=offset; i<(offset+blksize); ++i) {
        fprintf(fp, "0x%02X \n", (int)(ptr[i] & 0xFF));
    }
  }	
  fclose(fp);
}


int main(int argc, char *argv[])
{
  int res = 0;
  gfd = -1;
  gSysAddr = NULL;
  gCardAddr = NULL;
  gCtrlAddrBase = NULL;
  char outfn[80];
  
  // system("tail -30 /var/log/messages | grep DMABufAddr > startup.ini");

  if (argc < 5) {
    cout << "usage: scriptReadMem -[c|h|r|s] 0xOffset 0xSize outfile" << endl;
    cout << "       where: " << endl;
    cout << "          Offset = base address in memory block to Read." << endl;
    cout << "                   Offset value=0x00 causes data to be interpreted as a DCB." << endl;
    cout << "          Size   = number of bytes to read beyond Offset address." << endl;
    cout << "          c = card Config space" << endl;
    cout << "          h = hardware, Explorer card DIMM memory" << endl;
    cout << "          r = Register File, Explorer card Register I/O bank" << endl;
    cout << "          s = system memory, device driver Kernelmem DMA buffer" << endl;
    return(-1);
  } 
  // else get on with it. 
  char cmdOption = argv[1][0];
  if (cmdOption == '-') { cmdOption = argv[1][1]; }
  char *endtoken;  
  size_t optionOffset = strtol(argv[2], &endtoken, 16);  
  size_t optionBlkSize = strtol(argv[3], &endtoken, 16);
  strcpy(outfn, argv[4]);  
  
  // We want all I/O in hexadecimal string format, no base10.
  cout.setf(ios::hex, ios::basefield);
  cout.setf(ios::showbase);
  
  // find device driver.
  gfd = open("/dev/explorer", O_RDWR);
  if (gfd == -1)
  {
    printf("Couldn't open /dev/explorer\n");
    return 0;
  }

  // get in touch with the card's device driver.
  ioctl(gfd, EXP_IOCTALLOCDMABUF, 4);	
  ioctl(gfd, EXP_IOCGDMABUFPA, &gSysAddr);

  // Map Device memory for hookup to device driver.
  // first page for control block; next 256MB for DIMM memory block 
  gCtrlAddrBase = mmap(NULL, 4096 + 256*1024*1024, PROT_READ|PROT_WRITE, MAP_SHARED, gfd, (off_t)0x00000000);

  gCardAddr = (void *)((unsigned long)gCtrlAddrBase + 0x1000);	

  cout <<  "ctrladdrbase (card)   = " << gCtrlAddrBase << endl;
  cout <<  "expaddr (card)   = " << gCardAddr << endl;

  // load up the PCI Config given to the card this bootup cycle.
  LoadHWInfo(gfd);

  switch (cmdOption) 
  {
  	case 'c':
  	     LoadHWInfo(gfd);
         ShowHWInfo();
         cout <<  "expaddr (card)   = " << gCardAddr << endl;
         cout <<  "sysaddr (driver) = " << gSysAddr << endl;
         break;
    case 'h':    // card DIMM HW
         // ShowCardMem(optionOffset, optionBlkSize);
         ReadFromCardMem(optionOffset, optionBlkSize, outfn);
         break;
    case 'r':   // card controller I/O register bank HW
      // ReadFromRegisterFile(optionOffset, optionBlkSize, outfn);
         break;
    case 's':   // device driver bufferspace
         // ShowSysMem(optionOffset, optionBlkSize);
         ReadFromSysMem(optionOffset, optionBlkSize, outfn);
         break;
    default:
         cout <<  "expaddr (card)   = " << gCardAddr << endl;
         cout <<  "sysaddr (driver) = " << gSysAddr << endl;
         cout <<  "Bad option " << cmdOption << " specification, run ignored, nothing done." << endl;
         break; 
  }    
  
  // Unmap Device memory
  munmap(gCtrlAddrBase, 4096 + 256*1024*1024);

  close(gfd);

  return res;
}

