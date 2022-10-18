//=======================================================================================================================
/**  
@author 
	jjumbo
@since 
	2007. 02 .23

@remarks
	목적.
		1.간단한 프로토콜에 대해서 클라이언트가 서버 없이도, 실시간으로 패킷을 주고 받게 한다.
	사용법.
		1. CMakeVirtualPacket::Init(카테고리, 프로토콜)에 테스트 하고자 하는 프로토콜을 등록한다.
		2. 게임 채팅상에서 @recv 프로토콜 , @send 프로토콜 을 입력 함으로써 사용 가능하다.

주의 
*/
//=======================================================================================================================
#pragma once
#ifdef MAKE_REALTIME_SEND_PACKET
#include <map.h>
#include <singleton.h>

#define MakeVirtualPacket(byCategory , byProtocol) CMakeVirtualPacket::insertMap(#byProtocol , byProtocol , byCategory)

class CMakeVirtualPacket :public util::Singleton<CMakeVirtualPacket>
{
public:
	CMakeVirtualPacket(void);
	~CMakeVirtualPacket(void);
	void Init();
	void insertMap(TCHAR * szProtocol , BYTE byProtocol , BYTE byCategory );
	void SendPacket(TCHAR * szProtocol);
	void RecvPacket(TCHAR * szProtocol);

private:
	std::map<util::_tstring , MSG_BASE> m_VirtualPacketMap;
	std::map<util::_tstring ,MSG_BASE>::iterator m_Iterator;
};

#endif //MAKE_REALTIME_SEND_PACKET
