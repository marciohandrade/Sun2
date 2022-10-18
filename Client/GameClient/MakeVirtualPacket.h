//=======================================================================================================================
/**  
@author 
	jjumbo
@since 
	2007. 02 .23

@remarks
	����.
		1.������ �������ݿ� ���ؼ� Ŭ���̾�Ʈ�� ���� ���̵�, �ǽð����� ��Ŷ�� �ְ� �ް� �Ѵ�.
	����.
		1. CMakeVirtualPacket::Init(ī�װ�, ��������)�� �׽�Ʈ �ϰ��� �ϴ� ���������� ����Ѵ�.
		2. ���� ä�û󿡼� @recv �������� , @send �������� �� �Է� �����ν� ��� �����ϴ�.

���� 
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
