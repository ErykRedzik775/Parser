#include "tsCommon.h"
#include "tsTransportStream.h"
#include <iostream>


int main(int argc, char *argv[ ], char *envp[ ])
{
  const int size = 188;
  //weeterg
  // TODO - open file
  xTS_PacketHeader    TS_PacketHeader;
  xTS_AdaptationField TS_AdaptationField;
  xTS_Packet TS_Packet;
  uint8_t* bufor = new uint8_t[size];
  int counter=0;
  FILE * pFile = fopen( "../example_new.ts", "rb" );
  if( pFile == NULL) 
  {
    std::cout << "Error";
    return 0;
  }
  //else std::cout << "Git";

  int32_t TS_PacketId = 0;
  while( !feof( pFile ) )
  {
    // TODO - read from file
    fread( bufor, sizeof( uint8_t ), size, pFile );

    TS_PacketHeader.Reset();
\
    printf("%010d ", TS_PacketId);
    TS_Packet.ParsePacket(bufor);
    printf("\n");

    TS_PacketId++;
    counter++;
    if(counter==30) return 0;
  }
  delete [] bufor;
  return 0;
}