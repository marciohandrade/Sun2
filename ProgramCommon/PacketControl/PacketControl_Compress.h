#pragma once
#ifndef PROGRAMCOMMON_PACKETCONTROL_COMPRESS_H
#define PROGRAMCOMMON_PACKETCONTROL_COMPRESS_H

#include <PacketStruct.h>
#include "./PacketControl.h"
//==================================================================================================
//
namespace nsPacket {
;

class CompressUtil
{
    static const INT    COMPRESS_LEVEL;
    static const DWORD  BUFFER_SIZE     = 65536;//PacketControl::SIZEofSTREAM;
    static const WORD   NEED_COMPRESS_TOTAL_SZ = 150 + sizeof(MSG_BASE);
public:
    enum eTRANS_STATE
    {
        DEFAULT_TRANS            = 0,
        PACKET_COMPRESS_TRANS    ,
        PACKET_DECOMPRESS_TRANS    ,
    };

    static PacketInfo MakeStream(const BOOLEAN isPack, BYTE* OUT pOutPtr,
                                 MSG_BASE* const pMsg, const WORD wSize);
private:
    static void* __cdecl memalloc_handler(unsigned int len, const int compress_trans);
    static void __cdecl memfree_handler(void* ptr, const int compress_trans);

    static eTRANS_STATE ms_COMPRESS_TRANS;
    //
    friend class Environment;
};

//==================================================================================================
//

}; // end of namespace

#endif //PROGRAMCOMMON_PACKETCONTROL_COMPRESS_H
