#include "stdafx.h"

#include "SSQInfoStruct.h"
#include "SSQPacketProc.h"

namespace nsSSQ
{

};

// (WAVERIX) (090719) (NOTE) 아마 클라이언트에서는 사용하지 않는다. 제거 예정

//	Client Packet Serializer
WORD
WarPacketAssembler::Msg_Size = 0;

BYTE
WarPacketAssembler::Msg_Stream[4096];

