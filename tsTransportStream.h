#pragma once
#include "tsCommon.h"
#include <string>

/*
MPEG-TS packet:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |                             Header                            | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   4 |                  Adaptation field + Payload                   | `
`     |                                                               | `
` 184 |                                                               | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `


MPEG-TS packet header:
`        3                   2                   1                   0  `
`      1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0  `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `
`   0 |       SB      |E|S|T|           PID           |TSC|AFC|   CC  | `
`     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ `

Sync byte                    (SB ) :  8 bits
Transport error indicator    (E  ) :  1 bit
Payload unit start indicator (S  ) :  1 bit
Transport priority           (T  ) :  1 bit
Packet Identifier            (PID) : 13 bits
Transport scrambling control (TSC) :  2 bits
Adaptation field control     (AFC) :  2 bits
Continuity counter           (CC ) :  4 bits
*/


//=============================================================================================================================================================================

class xTS
{
public:
  static constexpr uint32_t TS_PacketLength = 188;
  static constexpr uint32_t TS_HeaderLength = 4;

  static constexpr uint32_t PES_HeaderLength = 6;

  static constexpr uint32_t BaseClockFrequency_Hz         =    90000; //Hz
  static constexpr uint32_t ExtendedClockFrequency_Hz     = 27000000; //Hz
  static constexpr uint32_t BaseClockFrequency_kHz        =       90; //kHz
  static constexpr uint32_t ExtendedClockFrequency_kHz    =    27000; //kHz
  static constexpr uint32_t BaseToExtendedClockMultiplier =      300;
};

//=============================================================================================================================================================================

class xTS_PacketHeader
{
public:
  enum class ePID : uint16_t
  {
    PAT  = 0x0000,
    CAT  = 0x0001,
    TSDT = 0x0002,
    IPMT = 0x0003,
    NIT  = 0x0010, //DVB specific PID
    SDT  = 0x0011, //DVB specific PID
    NuLL = 0x1FFF,
  };
  
protected:
  //TODO - header fields
  uint32_t SB; // 8 bitów
  uint32_t E; // 1 bit
  uint32_t S; // 1bit
  uint32_t TP; // 1bit
  uint32_t PID; // 13 bitów
  uint32_t TSC; // 2bity
  uint32_t AFC; // 2 bity
  uint32_t CC=0; //4 bity 
  bool StartOfPES = false; 

public:
  void     Reset();
  int32_t  Parse(const uint8_t* Input);
  void     Print() const;
  friend class xTS_AdaptationField;
  friend class xTS_Packet;

public:
  //TODO - direct acces to header values

public:
  //TODO
  //bool     hasAdaptationField() const { /*TODO*/ }
  //bool     hasPayload        () const { /*TODO*/ }
};

//=============================================================================================================================================================================

class xTS_AdaptationField{
  protected:
    uint32_t AFL;
    uint32_t DC;
    uint32_t RA;
    uint32_t SP;
    uint32_t PR;
    uint32_t OR;
    uint32_t SPF;
    uint32_t TP;
    uint32_t EX;   
  public:
    void PrintAF()const ;
    int32_t ParseAF(const uint8_t* Input); 
    friend class xTS_PacketHeader;
    friend class xTS_Packet;


};
class xTS_PESASSE{
protected:
  uint32_t PTS_DTS_I;
  uint32_t ESCR_F;
  uint32_t ES_R_F;
  uint32_t DSM_TMF;
  uint32_t ACIF;
  uint32_t CRCF;
  uint32_t EF;
  uint32_t PDF;
  uint32_t PHFF;
  uint32_t PPSCF;
  uint32_t PBF;
  uint32_t PEF_2;
  uint32_t headerLemgth = 14;
  uint32_t payload = 2880;
public:
  int32_t checkPESHeader(const uint8_t* Input);
  //void getLengthInfo();
  friend class xTS_PES;

};
class xTS_PES{
  protected:
  uint32_t PSC;
  uint32_t SID;
  uint32_t PES_PACKET_LENGTH;
  uint32_t PES_total_length;

  public:
    int32_t ParsePES(const uint8_t* Input);
    void PrintPES() const;
    //void getLengthInfo();
    friend class xTS_Packet;
};

class xTS_Packet{
  protected:
    xTS_PacketHeader packtet_header;
    xTS_AdaptationField packter_af;
    xTS_PES packet;
    xTS_PESASSE packet_pes_header;

  public:
    void getLengthInfo();
    void PrintPacket() ;
    int32_t ParsePacket(const uint8_t* Input); 
    friend class xTS_PacketHeader;
    friend class xTS_AdaptationField;
    friend class xTS_PES;
};