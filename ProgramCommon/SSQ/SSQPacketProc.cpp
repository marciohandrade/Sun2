#include "stdafx.h"

#include "SSQInfoStruct.h"
#include "SSQPacketProc.h"

namespace nsSSQ
{

};

// (WAVERIX) (090719) (NOTE) �Ƹ� Ŭ���̾�Ʈ������ ������� �ʴ´�. ���� ����

//	Client Packet Serializer
WORD
WarPacketAssembler::Msg_Size = 0;

BYTE
WarPacketAssembler::Msg_Stream[4096];

