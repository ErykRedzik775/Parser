#include "tsTransportStream.h"


//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================

void xTS_PacketHeader::Reset()
{

}

int32_t xTS_PacketHeader::Parse(const uint8_t* Input)
{   
   // uint32_t Tmp = ((uint32_t)Input);
    //Tmp = xSwapBytes32(Tmp);
    //uint32_t maskSB = 0b111111110000000000000000000000;
    unsigned int mask = 0x80;

    SB = Input[0];

    E = bool(Input[1] & mask);
    mask >>= 1;

    S = bool(Input[1] & mask);
    mask >>= 1;
    if( S == 1 ) StartOfPES = true;
    else StartOfPES = false;

    TP = bool(Input[1] & mask);
    mask -= 1;

    PID = Input[1] & mask;
    mask = 0xFF;
    PID <<= 8;
    PID |= Input[2] & mask;
    
    mask = 0xC0;
    TSC = Input[3] & mask;
    TSC >>= 6;

    mask >>=2;
    AFC = Input[3] & mask;
    AFC >>= 4;
    

    mask = 0x0F;
    CC = Input[3] & mask;
return 0;
}

void xTS_PacketHeader::Print() const
{   
    printf("TF: \tSB=%d\tE=%d\tS=%d\tTP=%d\tPID=%d\tTSC=%d\tAFC=%d\tCC=%d", SB, E, S, TP, PID, TSC, AFC, CC);
}
//=============================================================================================================================================================================
//ADAPTATION FIELD //
int32_t xTS_AdaptationField::ParseAF(const uint8_t* Input){
    unsigned int mask = 0x80;
    AFL = Input[4] & mask;
    DC = Input[5] & mask;
    
    mask >>= 1;
    RA = Input[5] & mask;
    
    mask >>=1;
    SP = Input[5] & mask;

    mask >>=1;
    PR = Input[5] & mask;

    mask >>=1;
    OR = Input[5] & mask;

    mask >>=1;
    SPF = Input[5] & mask;

    mask >>=1;
    TP = Input[5] & mask;

    mask >>=1;
    EX = Input[5] & mask;

    return 0;
}

void xTS_AdaptationField::PrintAF() const
{
    printf("\tAF: \tAFL=%d\tDC=%d\tRA=%d\tSP=%d\tPR=%d\tOR=%d\tSPF=%d\tTP=%d\tEX=%d", AFL, DC, RA, SP, PR, OR, SPF, TP, EX);
}
//========================
//================
void xTS_Packet::PrintPacket(){
    packtet_header.Print();
}
//==================
int32_t xTS_Packet::ParsePacket(const uint8_t* Input)
{
    packtet_header.Parse(Input);
    packtet_header.Print();

    if(packtet_header.AFC == 3 || packtet_header.AFC == 2){
        packter_af.ParseAF(Input);
        //packter_af.PrintAF();
    }
    packet.ParsePES(Input);
    getLengthInfo();
    if(  packtet_header.StartOfPES == true && packtet_header.PID == 136){
        printf("\tStarted:");
        packet.PrintPES();
    }
    if(packtet_header.StartOfPES == false && packtet_header.PID == 136 && packtet_header.AFC != 3)
        printf("\tContinue Total Length=%d", packet.PES_total_length);
    if(packtet_header.StartOfPES == false && packtet_header.AFC == 3 && packtet_header.PID == 136){
        printf("\tFinished Total Length=%d", packet.PES_total_length);
    }
    return 0;
}

int32_t xTS_PES::ParsePES(const uint8_t* Input){
    PSC = Input[6];
    PSC <<= 8;
    PSC += Input[7];
    PSC <<= 8;
    PSC += Input[8];

    SID = Input[9];

    PES_PACKET_LENGTH = Input[10];
    PES_PACKET_LENGTH <<= 8;
    PES_PACKET_LENGTH += Input[11];

    return 0;
}
void xTS_PES::PrintPES() const {
    printf(" PSCP=%d SID=%d L=%d",PSC, SID, PES_PACKET_LENGTH);
}

//==========================
void xTS_Packet::getLengthInfo(){
    if(packtet_header.AFC == 1 && packtet_header.PID == 136){
        packet.PES_total_length = 0;
    }
    if(packtet_header.AFC < 2  && packtet_header.PID == 136){
        packet.PES_total_length += 184;
    }
    else if(packtet_header.PID == 136)
        packet.PES_total_length += 183 - packter_af.AFL;
}