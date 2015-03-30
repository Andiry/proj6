#ifndef READMEM_HPP_
#define READMEM_HPP_
// 
//
//

class CardMem
{
public:
   CardMem() {}
   ~CardMem() {}
   
  int VendorID; // DWord 0 LSB.
  int DeviceID;    // DWord 1 MSB.
  int Command;     // Dword 01 LSB.
  int Status;      // DWord 01 MSB.
  int RevisionID;  // DWord 02 LSB.
  int ClassCode;   // DWord 02 MSB.
  int CacheLineSize;        // DWord 03 Byte0.
  int MasterLatencyTimer;   // DWord 03 Byte1.
  int HeaderType;           // DWord 03 Byte2.
  int BIST;                 // DWord 03 Byte3.
  int BAR0;  // DWord 04.
  int BAR1;  // DWord 05.
  int BAR2;  // DWord 06.
  int BAR3;  // DWord 07.
  int BAR4;  // DWord 08.
  int BAR5;  // DWord 09.
  int CardbusCISPointer;         // DWord 10 d.
  int SubsystemVendorID;         // DWord 11 Byte0-1.
  int SubsystemID;               // DWord 11 Byte2-3.
  int ExpansionROMBaseAddress;   // DWord 12. 
  int CapabilitiesPointer;       // DWord 13 Byte0.
};
  
#endif /*READMEM_HPP_*/



