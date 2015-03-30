#ifndef COMMON_HPP_
#define COMMON_HPP_

// 
//
//


class CardMem
{
public:
   CardMem() {}
   ~CardMem() {}
   
  unsigned int VendorID; // DWord 0 LSB.
  unsigned int DeviceID;    // DWord 1 MSB.
  unsigned int Command;     // Dword 01 LSB.
  unsigned int Status;      // DWord 01 MSB.
  unsigned int RevisionID;  // DWord 02 LSB.
  unsigned int ClassCode;   // DWord 02 MSB.
  unsigned int CacheLineSize;        // DWord 03 Byte0.
  unsigned int MasterLatencyTimer;   // DWord 03 Byte1.
  unsigned int HeaderType;           // DWord 03 Byte2.
  unsigned int BIST;                 // DWord 03 Byte3.
  unsigned int BAR0;  // DWord 04.
  unsigned int BAR1;  // DWord 05.
  unsigned int BAR2;  // DWord 06.
  unsigned int BAR3;  // DWord 07.
  unsigned int BAR4;  // DWord 08.
  unsigned int BAR5;  // DWord 09.
  unsigned int CardbusCISPointer;         // DWord 10 d.
  unsigned int SubsystemVendorID;         // DWord 11 Byte0-1.
  unsigned int SubsystemID;               // DWord 11 Byte2-3.
  unsigned int ExpansionROMBaseAddress;   // DWord 12. 
  unsigned int CapabilitiesPointer;       // DWord 13 Byte0.
};

#endif /*COMMON_HPP_*/
