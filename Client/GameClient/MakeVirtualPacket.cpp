#include "SunClientPrecompiledHeader.h"

#ifdef MAKE_REALTIME_SEND_PACKET
#include "makevirtualpacket.h"
#include <SunAuth/AuthProtocol.h>
#include "GlobalFunc.h"
#include "Application.h"

CMakeVirtualPacket::CMakeVirtualPacket(void)
{
	Init();
}

CMakeVirtualPacket::~CMakeVirtualPacket(void)
{
}

void CMakeVirtualPacket::Init()
{	
	MakeVirtualPacket(CW_CHAT , CW_NOTICE_BRD);						// GM °øÁö
	MakeVirtualPacket(CATEGORY_AUTH , CF_AUTH_ID_PASSWORD_CHECK_SYN);				//
	MakeVirtualPacket(CATEGORY_AUTH , FC_AUTH_ID_PASSWORD_CHECK_ACK);				//
	MakeVirtualPacket(CATEGORY_AUTH , CF_AUTH_SERVER_LIST_SYN);			// 
#ifdef __CN_269_MATRIX_CARD_AUTH
	MakeVirtualPacket(CATEGORY_AUTH , FC_AUTH_MATRIX_CARD_SYN);
	MakeVirtualPacket(CATEGORY_AUTH, CF_AUTH_MATRIX_CARD_ACK);
#endif // __CN_269_MATRIX_CARD_AUTH

	MakeVirtualPacket(CG_GM, CG_GM_PICKUP_ACK);
	MakeVirtualPacket(CG_ZONE, CG_ZONE_PORTAL_FIELD_MOVE_ACK);

	MakeVirtualPacket(CG_PARTY, CG_PARTY_RETURN_G2C_LEADER_QUERY_CMD);
	MakeVirtualPacket(CG_PARTY, CG_PARTY_RETURN_G2C_MEMBER_QUERY_CMD);
	MakeVirtualPacket(CG_PARTY, CG_PARTY_RETURN_G2C_MEMBER_REMOVE_CMD);

	MakeVirtualPacket(CG_ITEM, CG_ITEM_VALIDATION_OF_REIHALT_ACK);
	MakeVirtualPacket(CG_ITEM, CG_ITEM_VALIDATION_OF_REIHALT_NAK);

	MakeVirtualPacket(CG_QUEST, CG_QUEST_CHANGEofCLASS_CMD);

	MakeVirtualPacket(CG_ITEM, CG_ITEM_PICK_MONEY_ACK);
	MakeVirtualPacket(CG_ITEM,CG_ITEM_REPURCHASE_ACK);
	MakeVirtualPacket(CG_BATTLE, CG_BATTLE_CHARGE_ETHER_BULLET_BRD);
	MakeVirtualPacket(CG_BATTLE,CG_BATTLE_REMOVE_ETHER_BULLET_BRD);
	MakeVirtualPacket(CG_ITEM,CG_ITEM_EQUIP_ETHER_DEVICE_ACK);

}

void CMakeVirtualPacket::insertMap(TCHAR * szProtocol , BYTE byProtocol , BYTE byCategory )
{
	MSG_BASE base;
	base.m_byCategory = byCategory;
	base.m_byProtocol = byProtocol;
	m_VirtualPacketMap.insert(make_pair(szProtocol , base) );
}

void CMakeVirtualPacket::SendPacket(TCHAR * szProtocol)
{
	m_Iterator = m_VirtualPacketMap.find(szProtocol);

	if( m_Iterator != m_VirtualPacketMap.end() )
	{
		MSG_BASE SendPacket = m_Iterator->second;

		eCONNECTION_INDEX eConnectIndex = CI_GAMESERVERIDX;
		if( szProtocol[1] == 'L')
		{
			eConnectIndex = CI_AUTHSERVERIDX;
		}
		else if ( szProtocol[1] =='W')
		{
			eConnectIndex = CI_CHATSERVERIDX;
		}
		else 
		{
			eConnectIndex = CI_GAMESERVERIDX;
		}

		GlobalFunc::SendPacket(eConnectIndex , &SendPacket , sizeof(SendPacket) );
	}
}

void CMakeVirtualPacket::RecvPacket(TCHAR * szProtocol)
{
	m_Iterator = m_VirtualPacketMap.find(szProtocol);

	if( m_Iterator != m_VirtualPacketMap.end() )
	{
		MSG_BASE SendPacket = m_Iterator->second;

		eCONNECTION_INDEX eConnectIndex = CI_GAMESERVERIDX;
		if( szProtocol[1] == 'L')
		{
			eConnectIndex = CI_AUTHSERVERIDX;
		}
		else if ( szProtocol[1] =='W')
		{
			eConnectIndex = CI_CHATSERVERIDX;
		}
		else 
		{
			eConnectIndex = CI_GAMESERVERIDX;
		}

		NetworkRecv(eConnectIndex ,(BYTE*) &SendPacket  , sizeof(SendPacket));
	}
	
}
#endif //MAKE_REALTIME_SEND_PACKET