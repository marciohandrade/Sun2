#include "stdafx.h"
#include "QuestAction_Concrete.h"

#include <PacketStruct_CG.h>
#include <PacketStruct_DG.h>

#include "QuestPacket.h" //_NA_003027_20111013_HONOR_SYSTEM
#include "Quest.h"
#include "HonorSystem/HonorSystemPacket.h" //_NA_003027_20111013_HONOR_SYSTEM
#include "AchievementManager.h"

#include <MapInfoParser.h>
#include "NPCInfoParser.h"

#include "QuestManager_Concrete.h"
#include "ItemManager.h"
#include "Player.h"
#include "Monster.h"
#include "MovingPlanner.h"
#include "GameField.h"
#include "StatusManager.h"
#include "ExtraNPCManager.h"
#include "CollectedList.h"


__IMPL_CUSTOMPOOL_PTR( QUEST_ACTION_ITEM )
VOID	QUEST_ACTION_ITEM::DoAccept( VOID * pvParam )
{
	Player * pPlayer = (Player *)pvParam;

	if( 0 == GetInfo()->GetItemArrayCount() ) return;
	
	if( GetInfo()->IsDelete() )
	{
		// 삭제
		MSG_CG_QUEST_ITEM_DELETE_CMD cmsg;
		cmsg.m_QuestCode = GetQuest()->GetQuestCode();
		RC::eITEM_RESULT rt = pPlayer->GetItemManager()->DeleteAllKindOfItem( GetInfo()->GetItemArrayCount(), GetInfo()->GetItemCodePtr(), GetInfo()->GetItemNumPtr(), &cmsg.m_TotalInfo );

		//<= 포기시 지울 아이템이 없을 경우 발생
		//ASSERT( RC::RC_ITEM_SUCCESS == rt && "퀘스트액션:지울아이템이 없다!" );
		if( rt == RC::RC_ITEM_SUCCESS )
		{
			GAMELOG->LogMultiItem( ITEM_QUEST_DELETE, pPlayer, GetInfo()->GetItemArrayCount(), GetInfo()->GetItemCodePtr(), GetInfo()->GetItemNumPtr() );
			pPlayer->SendPacket( &cmsg, cmsg.GetSize() );
		}
	}
	else
	{
		// 부여
		static MSG_CG_QUEST_ITEM_OBTAIN_CMD cmsg;
		cmsg.m_QuestCode = GetQuest()->GetQuestCode();
		RC::eITEM_RESULT rt = pPlayer->GetItemManager()->ObtainAllKindOfItem( GetInfo()->GetItemArrayCount(), GetInfo()->GetItemCodePtr(), GetInfo()->GetItemNumPtr(), &cmsg.m_TotalInfo );

		//<= 수락시 부여 아이템을 넣을 공간이 없을 경우
		ASSERT( RC::RC_ITEM_SUCCESS == rt && "퀘스트액션:공간이 부족하다!" );
		if( rt == RC::RC_ITEM_SUCCESS )
		{
			GAMELOG->LogMultiItem( ITEM_QUEST_OBTAIN, pPlayer, GetInfo()->GetItemArrayCount(), GetInfo()->GetItemCodePtr(), GetInfo()->GetItemNumPtr() );
			pPlayer->SendPacket( &cmsg, cmsg.GetSize() );
		}
	}
}
VOID	QUEST_ACTION_ITEM::DoComplete( VOID * pvParam )
{
	DoAccept(pvParam);
}


VOID	QUEST_ACTION_CHANGEofCLASS::DoAccept( VOID* pvParam )
{
}

VOID	QUEST_ACTION_CHANGEofCLASS::DoComplete( VOID* pvParam )
{
	Player* pPlayer = (Player*)pvParam;
	// (1) 서버 BASE_PLAYERINFO UPDATE
	// (2) SendToClient : about STEP [CHANGEofCLASS]

	BASE_PLAYERINFO& rPlayerInfo = *pPlayer->GetCharInfo();
	PROPERTY<BYTE>& rChangeOfClass = rPlayerInfo.m_ChangeOfClassStep;

	const eCHAR_TYPE charType = (eCHAR_TYPE)rPlayerInfo.m_byClassCode;
	switch( GetInfo()->ChangeOfClassStep )
	{
	case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL:	// 100 Level ChangeUp
		switch( charType )
		{
		case eCHAR_BERSERKER:		rChangeOfClass.Set( (BYTE)eCHAREX_BERSERKER_100LEVEL );		break;
		case eCHAR_DRAGON:			rChangeOfClass.Set( (BYTE)eCHAREX_DRAGON_KNIGHT_100LEVEL );	break;
		case eCHAR_SHADOW:			rChangeOfClass.Set( (BYTE)eCHAREX_SHADOW_100LEVEL );		break;
		case eCHAR_VALKYRIE:		rChangeOfClass.Set( (BYTE)eCHAREX_VALKYRIE_100LEVEL );		break;
		case eCHAR_ELEMENTALIST:	rChangeOfClass.Set( (BYTE)eCHAREX_ELEMENTALIST_100LEVEL );	break;
        case eCHAR_MYSTIC:          rChangeOfClass.Set( (BYTE)eCHAREX_MYSTIC_100LEVEL );        break;
        case eCHAR_HELLROID:        rChangeOfClass.Set( (BYTE)eCHAREX_HELLROID_100LEVEL );      break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eCHAR_WITCHBLADE:      rChangeOfClass.Set( (BYTE)eCHAREX_WITCHBLADE_100LEVEL );      break;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		default:
			ASSERT( !"Unexpected Status" );
			return;
		};
#ifdef __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP
        // 스텟 초기화를 따로 퀘스트 액션으로 뺐어야 함. [4/12/2010 lst1024]
		if( pPlayer->GetItemManager()->IsUnEquipAll() )
			pPlayer->InitAllStatByChangeUp();
#endif
		GAMELOG->CharacterChangeUpLog( pPlayer, charType, (BYTE)rChangeOfClass.Get() );
	}

	MSG_CG_QUEST_CHANGEofCLASS_CMD msgCMD;
	msgCMD.m_ChangeOfClassCode = rChangeOfClass;
	pPlayer->SendPacket( &msgCMD, sizeof(msgCMD) );

//#	error "Waverix:not implementation"
}


// 퀘스트 수락시 몬스터 소환.
VOID	QUEST_ACTION_SPAWN_MONSTER::DoAccept( VOID* pvParam )
{
	Player* pPlayer = (Player*)pvParam;
	if( !pPlayer )
		return;

	if( GetInfo()->GetSummonMonsterCode() == 0 )
		return;

	GameField* pField = pPlayer->GetField();
	if( !pField )
		return;

	Quest* pQuest = GetQuest();
	if( !pQuest )
		return;

	DWORD dwStartNPCCode = pQuest->GetQuestInfo()->GetNPCCode_Start();

	if( !g_ExtraNPCManager.IsLengthCanUseNPC( dwStartNPCCode, pPlayer ) )
		return;

	// 조건을 받아 들였으면 npc는 감추고, 몬스터를 생성시킨다.
	DWORD dwMonsterCode = GetInfo()->GetSummonMonsterCode();

	// 플레이어가 위치한 타일위에서 랜덤한 위치에 몬스터 생성(20050624)
	int iTileIdx = pPlayer->GetPathExplorer()->GetTile();
	if( -1 == iTileIdx )
		return;

	WzVector vPlayerPos;
	pPlayer->GetPos( &vPlayerPos );	

	nsAI::MovingPlanner* const pMovingPlanner = pField->GetMovingPlanner();
	vPlayerPos = pMovingPlanner->GetRandomPosInTile(iTileIdx);

	BASE_NPCINFO *pBaseNPCInfo = NPCInfoParser::Instance()->GetNPCInfo( dwMonsterCode );
	if( !pBaseNPCInfo )
		return;

    //__NA000000_090320_v0902_LOGIC_CHANGE_ABOUT_QUEST__
    Monster* pMonster = pField->CreateMonster( (MONSTERCODE)dwMonsterCode, &vPlayerPos);
    if(pMonster == 0)
    {
        SUNLOG(eCRITICAL_LOG, "Can't CreateMonster %u\n", dwMonsterCode);
        return;
    }

    QuestManager_Concrete* pQuestManager = pPlayer->GetQuestManager();
    pQuestManager->RegisterNpcSummonEvent((MONSTERCODE)dwMonsterCode, pMonster->GetObjectKey());
    //
	GAMELOG->WriteCreateMonster( pPlayer, dwMonsterCode, 1 );
}

VOID	QUEST_ACTION_SPAWN_MONSTER::DoComplete( VOID* pvParam )
{
	

}

VOID
QUEST_ACTION_SPAWN_MONSTER_ABANDON::DoComplete(VOID* pvParam)
{
    Player* pPlayer = (Player*)pvParam;
    QuestManager_Concrete* pQuestManager = pPlayer->GetQuestManager();
    pQuestManager->OnEventNpcSummonQuestAbandon();
}

//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

VOID
QUEST_ACTION_CREATE_PORTAL::DoAccept(VOID* pvParam)
{
    Player* pPlayer = (Player*)pvParam;
    GameField* pField = pPlayer->GetField();

    // 이 Action은 Condition과 짝을 이루고 있다. 각종 예외 사항은 모두 검증되었다.
    QUEST_ACTION_CREATE_PORTAL_INFO* pInfo = GetInfo();
    Quest* pQuest = GetQuest();
    QuestInfo* pQuestInfo = pQuest->GetQuestInfo();

    const MAPCODE mapCode = pInfo->GetMapPortalCode();
    const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(mapCode);

    nsAI::MovingPlanner* pMovingPlanner = pField->GetMovingPlanner();
    const INT iTile = pPlayer->GetPathExplorer()->GetTile();
    const WzVector randomPos = pMovingPlanner->GetRandomPosInTile(iTile);

    // Portal Create Command (C->G)
    MSG_AG_ZONE_QUEST_PORTAL_REQUEST_SYN cmd;
    QuestPortalInfo& rInfo = cmd.questPortalInfo;
    ZeroMemory(&rInfo, sizeof(rInfo));
    //rInfo.portalKey = by Agent Setting; 
    rInfo.enterableMapCode = mapCode;
    rInfo.requesterKey = (WORD)pPlayer->GetObjectKey();
    rInfo.questCode = pQuestInfo->GetQuestCode();
    rInfo.portalLifetime = 10*60*1000;
    rInfo.portalPosition = randomPos;
    pPlayer->SendPacket(&cmd, sizeof(cmd));
}

VOID
QUEST_ACTION_CREATE_PORTAL::DoComplete(VOID* pvParam)
{
}

VOID
QUEST_ACTION_CLOSE_PORTAL::DoComplete(VOID* pvParam)
{
    Player* pPlayer = (Player*)pvParam;
    GameField* pField = pPlayer->GetField();

    MSG_AG_ZONE_QUEST_PORTAL_CONTROL_CMD cmd;
    cmd.command = cmd.eCMD_CLOSE;
    pPlayer->SendPacket(&cmd, sizeof(cmd));
}

//}__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

#include "CollectionParser.h"

class DelCollectionIDOpr
{

public:		
	DelCollectionIDOpr( Player* pPlayer, QCODE QCode ) : m_pPlayer(pPlayer), m_QCode(QCode) {}
	~DelCollectionIDOpr(){}
	VOID operator() ( const CollectionInfo* const pCollectionInfo ) 
	{ 
		if( m_QCode != pCollectionInfo->m_Condition[CollectionInfo::QUEST] )
			return;
		
		if(	0 == m_pPlayer->GetCollectedList()->GetCount(pCollectionInfo->m_CollectionID) )
			return;

		MSG_DG_COLLECT_DEL_SYN msg;
		msg.m_CollectionID = pCollectionInfo->m_CollectionID;
		m_pPlayer->SendToGameDBPServer( &msg, sizeof(msg) );
	}

private:
	Player* m_pPlayer;
	QCODE	m_QCode;
};

VOID
QUEST_ACTION_DELETE_COLLECTIONID::DoComplete(VOID* pvParam)
{
	CollectionParser::Instance()->for_each(DelCollectionIDOpr((Player*)pvParam,GetQuest()->GetQuestCode()));
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM

VOID QUEST_ACTION_DAILY_QUEST::DoAccept( VOID* pvParam )
{
    Player* player = (Player*)pvParam;
    Quest* quest = GetQuest();
    QuestInfo* quest_info = quest->GetQuestInfo();

    if( quest_info->IsDailyQuest() )
    {
        QCODE code = quest_info->GetQuestCode();
        QuestManager_Concrete* quest_manager = player->GetQuestManager();
        quest_manager->DoAcceptDailyQuest(code);

        //
        const QUEST_DAILY_INFO* data = quest_manager->GetDailyQuestData(code);
        if(data == NULL)
        {
            SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"][CriticalBug] DailyQuestInfo is NULL![QCode:%d]", 
                    quest->GetQuestCode());
            return;
        }

        MSG_DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN msg;
        memcpy(&msg.quest_data, data, sizeof(QUEST_DAILY_INFO));
        player->SendToGameDBPServer(&msg, sizeof(msg));

        MSG_CG_QUEST_DAILYQUEST_INFO_CMD packet;
        memcpy(&packet.daily_quest_info, data, sizeof(QUEST_DAILY_INFO));
        player->SendPacket(&packet, sizeof(packet));
    }
}

VOID QUEST_ACTION_DAILY_QUEST::DoComplete(VOID* pvParam)
{
    Player* player = (Player*)pvParam;
    Quest* quest = GetQuest();
    QuestInfo* quest_info = quest->GetQuestInfo();

    if( quest_info->IsDailyQuest() )
    {
        QCODE code = quest_info->GetQuestCode();
        QuestManager_Concrete* quest_manager = player->GetQuestManager();
        
        ;{
            // 방금 완료한 일일퀘스트를 저장하기 전에 일일퀘스트 연속 완료 여부를 검사..
            // 오늘 완료한 적이 없으면 성공
            const int TODAY = 0, YESTERDAY = -1;
            AchievementManager* achevement_manager = player->GetACManager();

            if(!quest_manager->HaveDoneDailyQuest(TODAY))
            {
                achevement_manager->ProcessDailyQuest(TRUE);
            }
        };
    
        quest_manager->DoCompleteDailyQuest(code);

        //
        const QUEST_DAILY_INFO* data = quest_manager->GetDailyQuestData(code);
        if(!data) 
            return;

        MSG_DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN msg;
        memcpy(&msg.quest_data, data, sizeof(QUEST_DAILY_INFO));
        player->SendToGameDBPServer(&msg, sizeof(msg));

        MSG_CG_QUEST_DAILYQUEST_INFO_CMD packet;
        memcpy(&packet.daily_quest_info, data, sizeof(QUEST_DAILY_INFO));
        player->SendPacket(&packet, sizeof(packet));
    }
}

VOID QUEST_ACTION_DAILY_QUEST_DELETE::DoComplete(VOID* pvParam)
{
    Player* player = (Player*)pvParam;
    Quest* quest = GetQuest();
    QuestInfo* quest_info = quest->GetQuestInfo();

    if( quest_info->IsDailyQuest() )
    {
        QCODE code = quest_info->GetQuestCode();
        QuestManager_Concrete* quest_manager = player->GetQuestManager();

        quest_manager->DoAbandonDailyQuest(code);

        //
        const QUEST_DAILY_INFO* data = quest_manager->GetDailyQuestData(code);
        if(!data) 
            return;

        MSG_DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN msg;
        memcpy(&msg.quest_data, data, sizeof(QUEST_DAILY_INFO));
        player->SendToGameDBPServer(&msg, sizeof(msg));

        MSG_CG_QUEST_DAILYQUEST_INFO_CMD packet;
        memcpy(&packet.daily_quest_info, data, sizeof(QUEST_DAILY_INFO));
        player->SendPacket(&packet, sizeof(packet));
    }
}
#endif //_NA_003027_20111013_HONOR_SYSTEM


