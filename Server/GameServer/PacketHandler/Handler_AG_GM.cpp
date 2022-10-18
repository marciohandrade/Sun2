#include "StdAfx.h"
#include <PacketStruct_AG.h>
#include "Player.h"
#include <ServerSession.h>
#include "Handler_AG_GM.h"
#include "GameZone.h"
#include "GameGuildManager.h"
#include "GameGuild.h"
#include "GameServerEx.h"
#include "ItemManager.h"

Handler_AG_GM::Handler_AG_GM()
{
}

Handler_AG_GM::~Handler_AG_GM()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_AG_GM::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_AG_GM::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(AG_GM,  AG_GM_TELEPORT_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_GM,  AG_GM_CHAR_INFO_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_GM,  AG_GM_TRACKING_REQUEST_INFO_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_AG_GM_IMPL( AG_GM_TELEPORT_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_TELEPORT_CMD* pRecvMsg = (MSG_AG_GM_TELEPORT_CMD*)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) 
        return;

    GameField* pField = pPlayer->GetField();
    if( !pField ) 
        return;

	pField->TeleportObject( pPlayer, &pRecvMsg->m_vDestPos );
}

//캐릭터의 정보
Handler_AG_GM_IMPL( AG_GM_CHAR_INFO_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_CHAR_INFO_SYN* pRecvMsg = (MSG_AG_GM_CHAR_INFO_SYN*)pMsg;

	Player *pFindChar = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pFindChar ) return;

	MSG_AG_GM_CHAR_INFO_ACK sendMsg;
	sendMsg.m_dwGMUserKey = pRecvMsg->m_dwGMUserKey;
	sendMsg.m_byCategory = AG_GM;
	sendMsg.m_byProtocol = AG_GM_CHAR_INFO_ACK;
    CHECK_ID_LENGTH_BUFFER(sendMsg.m_pszAccountID);
	_tcsncpy(sendMsg.m_pszAccountID, pFindChar->GetUserID(), _countof(sendMsg.m_pszAccountID));
    sendMsg.m_pszAccountID[_countof(sendMsg.m_pszAccountID) - 1] = '\0';

	sendMsg.m_eCharType = pFindChar->GetCharType();
	sendMsg.m_wLevel = pFindChar->GetLevel();
	sendMsg.m_Money = pFindChar->GetMoney();
	pFindChar->GetPos( & sendMsg.m_vCurPos );

	GameZone *pZone = pFindChar->GetGameZonePtr();
	if( pZone )
	{
		sendMsg.m_RoomKey = pZone->GetKey();
		sendMsg.m_MapCode = pZone->GetMapCode();
	}
	else
	{
		sendMsg.m_RoomKey = 0;
		sendMsg.m_MapCode = 0;
	}

	// 길드이름.
	GUILDGUID GuildGuid = pFindChar->GetCharInfo()->m_GuildGuid;
	if( 0 == GuildGuid )
		ZeroMemory( sendMsg.m_szGuildName, MAX_GUILDNAME_LENGTH );
	else
	{
		GameGuild * pGuild = g_GameGuildManager.FindGuild( GuildGuid );
		if( pGuild )
			_tcsncpy( sendMsg.m_szGuildName, pGuild->GetGuildName(), MAX_GUILDNAME_LENGTH );
	}

	pFindChar->SendPacket( &sendMsg, sizeof(sendMsg) );

    static_cast<PlayerAttributes&>(pFindChar->GetAttr()).Display();
}

Handler_AG_GM_IMPL( AG_GM_TRACKING_REQUEST_INFO_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_TRACKING_REQUEST_INFO_SYN* pRecvMsg = (MSG_AG_GM_TRACKING_REQUEST_INFO_SYN*)pMsg;
    if (FlowControl::FCAssert(sizeof(*pRecvMsg) == wSize) == false) {
        return;
    };
	Player *pFindChar = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pFindChar )	return;

	GameZone *pZone = pFindChar->GetGameZonePtr();
	if( !pZone )		return;

	GameField *pField = pFindChar->GetField();
	if( !pField )		return;

    SLOTCODE item_code = 0;
    SCSlotContainer* pContainer = pFindChar->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
    // NOTE: f110105.1L, fixed logical clearly that
    // the validation of the container is inconsistent with an empty checking.
    POSTYPE item_pos = pRecvMsg->m_atItemPos;
    if (pContainer->ValidPos(item_pos) && !pContainer->IsEmpty(item_pos))
    {
        item_code = pContainer->GetSlot(item_pos).GetCode();
    }

#ifdef _NA_20100307_BETAKEY_SYSTEM
    //pFindChar : 타겟, 목적위치
    // m_szGMCharName: 명령어를 사용한 GM
    pFindChar->ReCallPlayer(pRecvMsg->m_szGMCharName, pZone->GetKey(), pField->GetFieldCode(),
                            pFindChar->GetRandomPosInAround(3), (ePORTAL_TYPE)pRecvMsg->m_PortalType, 
                            item_pos, item_code);
#else
	// 추적대상의 위치정보를 담아서 GM을 이동시키도록 명령한다.
	MSG_AG_GM_GO_CMD AGMsg;
	_tcsncpy( AGMsg.m_szCharName, pRecvMsg->m_szGMCharName, MAX_CHARNAME_LENGTH );
	AGMsg.m_szCharName[MAX_CHARNAME_LENGTH] = '\0';
	AGMsg.m_RoomKey = pZone->GetKey();
	AGMsg.m_FieldCode = pField->GetFieldCode();
	AGMsg.m_vCurPos = pRecvMsg->m_vCurPos;
	AGMsg.m_vDestPos = pFindChar->GetRandomPosInAround( 3 );
	
	AGMsg.m_PortalType = pRecvMsg->m_PortalType;
	AGMsg.m_atItemPos = pRecvMsg->m_atItemPos;
    AGMsg.m_atItemCode = item_code;
	pFindChar->SendPacket( &AGMsg, sizeof(AGMsg) );
#endif
}




























