#include "StdAfx.h"
#include ".\handler_cg_quest.h"
#include ".\QuestManager_Concrete.h"
#include <PacketStruct_CG.h>
#include ".\Quest.h"
#include "RewardManager.h"
#include "Player.h"
#include <FatigueOptionParser.h>
#include "FatigueManager.h"
#include "ChaoState.h"

#include <ItemManager.h>
#include <SCItemSlotContainer.h>

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
#include "AchievementManager.h"
#endif
#include "ExtraNPCManager.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_QUEST::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_QUEST::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_QUEST, CG_QUEST_ACCEPT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_QUEST, CG_QUEST_COMPLETE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_QUEST, CG_QUEST_REWARD_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_QUEST, CG_QUEST_FAILED_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_QUEST, CG_QUEST_ABANDON_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_QUEST, CG_QUEST_EVENT_AREA_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_QUEST, CG_QUEST_INCOMPLETE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_CG_QUEST_IMPL( CG_QUEST_ACCEPT_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_QUEST_ACCEPT_SYN * pRecvMsg = (MSG_CG_QUEST_ACCEPT_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_QUEST_IMPL(CG_QUEST_ACCEPT_SYN)] Not Exist GameField" );
		return;
	}
	if( !pPlayer->GetChaoState().CanUseFuction( CHAO_USE_QUEST ) )
	{
		MSG_CG_QUEST_ACCEPT_NAK msg;
		msg.m_byErrorCode = RC::RC_QUEST_UNABLE_FUNCTION_FOR_CHAOSTATE;
		msg.m_byIsItemResult = 0;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}
	if( pPlayer->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING )
	{
		MSG_CG_QUEST_ACCEPT_NAK msg;
		msg.m_byErrorCode = RC::RC_QUEST_PLAYER_BEHAVE_UNEXPECTED_WAITING;
		msg.m_byIsItemResult = 0;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}	

	if( pPlayer->GetQuestManager()->IsQuestMax() )
	{
		MSG_CG_QUEST_ACCEPT_NAK msg;
		msg.m_byErrorCode = RC::RC_QUEST_CANNOTQUEST_MAXCOUNT;
		msg.m_byIsItemResult = 0;
		pPlayer->SendPacket( &msg, sizeof(msg) );

		return ;
	}

    QuestManager_Concrete* const questManager = pPlayer->GetQuestManager();
  
    const DWORD curQuestNum   = questManager->QuestNum();
    const DWORD childQuestNum = questManager->GetChildQuestNum(pRecvMsg->m_QuestCode);
    const DWORD preQuestNum   = curQuestNum + childQuestNum + 1;

    if( preQuestNum > QUESTPART_PROGR::MAX_QUEST_PROGR_NUM )
    {
        MSG_CG_QUEST_ACCEPT_NAK msg;
        msg.m_byErrorCode = RC::RC_QUEST_CANNOTQUEST_MAXCOUNT;
        msg.m_byIsItemResult = 0;
        pPlayer->SendPacket( &msg, sizeof(msg) );

        return ;        
    }
    //{__NA001282_20090206_MODIFY_QUEST_TIME
	// (lst1024)(090306)타임퀘스트는 한번에 하나만 받을수 있다.
	if( pPlayer->GetQuestManager()->GetProgressTimeQuest() && 
		pPlayer->GetQuestManager()->IsTimeQuest(pRecvMsg->m_QuestCode) )
	{
		MSG_CG_QUEST_ACCEPT_NAK msg;
		msg.m_byErrorCode = RC::RC_QUEST_ALREADY_PROGRESS_TIME_QUEST;
		msg.m_byIsItemResult = 0;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}
    //}

#ifdef _NA_20100307_BETAKEY_SYSTEM
    if(!pPlayer->GetBetaKeyInfo()->CanAbilityContent(questManager->GetBetakey(pRecvMsg->m_QuestCode)))
    {
        MSG_CG_QUEST_ACCEPT_NAK msg;
        msg.m_byErrorCode = RC::RC_QUEST_CANNOT_BETAKEY;
        msg.m_byIsItemResult = 0;
        pPlayer->SendPacket( &msg, sizeof(msg) );

        return;
    }
#endif

    //버그 수정: 5010 (이미 존재하는 퀘스트를 또 생성하려 할때) youngmoon 2009-12-17
	Quest * pQuest = pPlayer->GetQuestManager()->CreateQuest( pRecvMsg->m_QuestCode );

    if(!pQuest)
    {
        MSG_CG_QUEST_ACCEPT_NAK msg;
        msg.m_byErrorCode = RC::RC_QUEST_EXIST;
        msg.m_byIsItemResult = 0;
        pPlayer->SendPacket( &msg, sizeof(msg) );

        SUNLOG( eCRITICAL_LOG, "[CG_QUEST_ACCEPT_SYN] 이미 존재하는 퀘스트 Char[%s] QCode[%d]",
            pPlayer->GetCharName(), pRecvMsg->m_QuestCode );
        return;
    }
	
    if( pQuest->GetQuestInfo()->IsChildQuest() )
	{
		pPlayer->GetQuestManager()->DestroyQuest( pRecvMsg->m_QuestCode );

		MSG_CG_QUEST_ACCEPT_NAK msg;
		msg.m_byErrorCode = RC::RC_QUEST_CHILD;
		msg.m_byIsItemResult = 0;
		pPlayer->SendPacket( &msg, sizeof(msg) );

		SUNLOG( eCRITICAL_LOG, "[CG_QUEST_ACCEPT_SYN] 퀘스트 수락 실패 Char[%s] QCode[%d]",
			pPlayer->GetCharName(), pRecvMsg->m_QuestCode );
		return;
	}

	RC::eQUEST_RESULT rt = pPlayer->GetQuestManager()->CanDoAccept( pRecvMsg->m_QuestCode, pPlayer );
	if( rt == RC::RC_QUEST_SUCCESS )
	{
		MSG_CG_QUEST_ACCEPT_ACK msg;
		pPlayer->SendPacket( &msg, sizeof(msg) );

		pPlayer->SendAllFieldCollectionInfo();
        //__NA001277_090205_NAVI_SYSTEM_SUPPORT__
        MSG_CG_QUEST_OBJECT_NAVI_POS_CMD msgNavi;
        if(pPlayer->GetQuestManager()->MakeInfoQuestNpcNaviStream(&msgNavi, pRecvMsg->m_QuestCode))
            pPlayer->SendPacket(&msgNavi, msgNavi.GetSize());
        //
		GAMELOG->LogAction( ACT_QUEST_ACCEPT, pPlayer, NULL, pRecvMsg->m_QuestCode );
	}
	else
	{
		pPlayer->GetQuestManager()->DestroyQuest( pRecvMsg->m_QuestCode );

		MSG_CG_QUEST_ACCEPT_NAK msg;
		msg.m_byErrorCode = rt;
		msg.m_byIsItemResult = 0;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}
}

Handler_CG_QUEST_IMPL( CG_QUEST_COMPLETE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_QUEST_COMPLETE_SYN * pRecvMsg = (MSG_CG_QUEST_COMPLETE_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_QUEST_IMPL(CG_QUEST_COMPLETE_SYN)] Not Exist GameField" );
		return;
	}

	RC::eQUEST_RESULT rt = pPlayer->GetQuestManager()->CanDoComplete( pRecvMsg->m_QuestCode, pPlayer );
	if( rt == RC::RC_QUEST_SUCCESS )
	{
		MSG_CG_QUEST_COMPLETE_ACK msg;
		msg.m_QuestCode = pRecvMsg->m_QuestCode;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}
	else
	{
        // 퀘스트관련 버그를 위한 로그
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"][CriticalBug] Cannot Complete Quest! Not Exist Or Failed[Player(AID:%d/CID:%d):%s][Quest:%d][ResultCode:%d]", 
                pPlayer->GetUserGuid(), pPlayer->GetCharGuid(), pPlayer->GetCharName(), pRecvMsg->m_QuestCode, rt );
		MSG_CG_QUEST_COMPLETE_NAK msg;
		msg.m_byErrorCode = rt;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}
}

Handler_CG_QUEST_IMPL( CG_QUEST_REWARD_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_QUEST_REWARD_SYN * pRecvMsg = (MSG_CG_QUEST_REWARD_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_QUEST_IMPL(CG_QUEST_REWARD_SYN)] Not Exist GameField" );
		return;
	}
	if( !pPlayer->GetChaoState().CanUseFuction( CHAO_USE_QUEST ) )
	{
		MSG_CG_QUEST_REWARD_NAK msg;
		msg.m_byErrorCode = RC::RC_QUEST_UNABLE_FUNCTION_FOR_CHAOSTATE;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;		
	}

	if( pPlayer->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING )
	{
		MSG_CG_QUEST_REWARD_NAK msg;
		msg.m_byErrorCode = RC::RC_QUEST_PLAYER_BEHAVE_UNEXPECTED_WAITING;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}	

	Quest * pQuest = pPlayer->GetQuestManager()->FindQuest( pRecvMsg->m_QuestCode );
	if( !pQuest )
	{
		MSG_CG_QUEST_REWARD_NAK msg;
		msg.m_byErrorCode = RC::RC_QUEST_EMPTY;
		pPlayer->SendPacket( &msg, sizeof(msg) );

        // 퀘스트관련 버그를 위한 로그
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"][CriticalBug] Not Exist Quest[Player(AID:%d/CID:%d):%s][Quest:%d]", 
                pPlayer->GetUserGuid(), pPlayer->GetCharGuid(), pPlayer->GetCharName(), pRecvMsg->m_QuestCode );
        //_NA_007909_20141218_SUNLOG_QUESTLIST
        pPlayer->GetQuestManager()->PrintLogProgressQuestList();
		return;
	}

    if( pQuest->GetQState() != QS_COMPLETED )
    {
        MSG_CG_QUEST_REWARD_NAK msg;
        msg.m_byErrorCode = RC::RC_QUEST_NOT_COMPLETED_STATE;
        pPlayer->SendPacket( &msg, sizeof(msg) );
        return;
    }

	if( pQuest->GetQuestInfo()->IsChildQuest()  )
	{
		MSG_CG_QUEST_REWARD_NAK msg;
		msg.m_byErrorCode = RC::RC_QUEST_CANNOTREWARD_CHILDQ;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}

    DWORD dwNpcCode = pQuest->GetQuestInfo()->GetNPCCode_Reward();
    if (0 != dwNpcCode && 
        FALSE == g_ExtraNPCManager.IsLengthCanUseNPC(dwNpcCode, pPlayer))
    {
        MSG_CG_QUEST_REWARD_NAK msg;
        msg.m_byErrorCode = RC::RC_QUEST_DISTANCE_DIFF_TOO_LONG;
        pPlayer->SendPacket( &msg, sizeof(msg) );
        return;
    }

    if( pQuest->CanComplete(pPlayer) != RC::RC_QUEST_SUCCESS )
	{
		pQuest->SetQState( QS_NORMAL );

		MSG_CG_QUEST_REWARD_NAK msg;
		msg.m_byErrorCode = RC::RC_QUEST_TOCOMPLETED;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}
	
	MONEY PreMoney = pPlayer->GetMoney();
	EXPTYPE OldExp = pPlayer->GetExp();
	MSG_CG_QUEST_REWARD_ACK msg;

	const eCHAR_TYPE charType = pPlayer->GetCharType();
	const DWORD rewardCode = pQuest->GetRewardCode( charType );

#ifdef __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP
	//100레벨 체인지 업 보상 시에는 옷을 벗어야 한다.
	switch( (eSPECIAL_QUEST_REWARD)rewardCode )
	{
	case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_BERSERKER:
	case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_DRAGON_KNIGHT:
	case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_SHADOW:
	case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_VALKYRIE:
	case SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_ELEMENTALIST:
		{
			if( !pPlayer->GetItemManager()->IsUnEquipAll() )
			{
				MSG_CG_QUEST_REWARD_NAK msg;
				msg.m_byErrorCode = RC::RC_QUEST_MUST_UNEQUIP_ALL_ITEM;
				pPlayer->SendPacket( &msg, sizeof(msg) );
				return;
			}
		}
		break;
	};
#endif

	RC::eREWARD_RESULT rt;
	
	if( pPlayer->GetFatigueManager()->GetItemReward() == TRUE )
		rt = g_RewardManager.RewardItem( RewardManager::eREWARD_QUEST, rewardCode, pRecvMsg->m_SelectedItem, pPlayer, msg.m_ItemInfo );
	else
	{
		msg.m_ItemInfo.m_InvenCount = 0;
		msg.m_ItemInfo.m_TmpInvenCount = 0;
		rt = RC::RC_REWARD_SUCCESS; // 피로도 때문에 퀘스트 아이템 보상을 받지 못하는 경우라면 우선 아이템 성공으로 처리한다.
	}

	if( rt == RC::RC_REWARD_SUCCESS )
	{
		WORD byExpRatio = pPlayer->GetFatigueManager()->GetExpRatio();
        //_KR_0_20091021_FIRENDINVITATION_EXTEND //퀘스트 보상 경험치 비율을 증가 시킨다. 
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        WORD addExpRatio = pPlayer->GetInvitateFriend().GetAddQuestExpRate(pPlayer->GetField()->GetFieldCode());
        //어트리뷰트에 의한 퀘스트 보상 경험치 비율 증감 적용
        addExpRatio += pPlayer->GetAttr().GetQuestRewardExpRatio();
        msg.m_BunusExpRatio = addExpRatio;
        byExpRatio += addExpRatio;
#else  // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        byExpRatio += pPlayer->GetInvitateFriend().GetAddQuestExpRate(pPlayer->GetField()->GetFieldCode());
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
		rt = g_RewardManager.RewardExp( rewardCode, pPlayer, msg.m_i64Exp, byExpRatio );
		ASSERT( rt == RC::RC_REWARD_SUCCESS );
		BYTE byHeimRatio = pPlayer->GetFatigueManager()->GetHeimRewardRatio();
		rt = g_RewardManager.RewardHime( rewardCode, pPlayer, msg.m_Money, byHeimRatio );
		ASSERT( rt == RC::RC_REWARD_SUCCESS );

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
        rt = g_RewardManager.RewardAccumulatePoint( rewardCode, pPlayer, msg.m_wAccumulatePoint );

        if (rt == RC::RC_REWARD_SUCCESS) {
            GAMELOG->LogAccumulatePoint(ACCUM_ACQUISITION_QUEST, pPlayer, msg.m_wAccumulatePoint, 0);
        }
#endif
        rt = g_RewardManager.RewardSkill( rewardCode, pPlayer, msg.m_SkillCode );
		ASSERT( rt == RC::RC_REWARD_SUCCESS );
		rt = g_RewardManager.RewardChaoTime( rewardCode, pPlayer);
		ASSERT( rt == RC::RC_REWARD_SUCCESS );
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        rt = g_RewardManager.RewardReputePoint(rewardCode, pPlayer);
        ASSERT( rt == RC::RC_REWARD_SUCCESS );
#endif //_NA_003027_20111013_HONOR_SYSTEM
	
		//피로도 기능을 꺼버리면 현재 피로도 구간이 0일수 있다. 
		_FATIGUE_TYPE* pFatigue = pPlayer->GetFatigueManager()->GetCurFatigueType();
		if( pFatigue )
			msg.m_byFatigueType = pFatigue->m_byType;
		else
			msg.m_byFatigueType = 0;
		msg.m_wQuestCode = pQuest->GetQuestCode();

        // Send 'MSG_CG_QUEST_REWARD_ACK' Packet
		pPlayer->SendPacket( &msg, msg.GetSize() );

		// 보상을 받는 순간 완료 행동이 일어난다.
		pPlayer->GetQuestManager()->DoReward( pRecvMsg->m_QuestCode, pPlayer );
    
        QUESTPART_COMPL* pCompNum = pPlayer->GetQuestManager()->FindComplQuest(pRecvMsg->m_QuestCode);
        if( pCompNum && (0xFF == pCompNum->m_Num) )
        {
            //(lst1024)(090611) 무한 반복퀘를 255회 이상하는 유저가 많아졌다.
            // 혹시 다른 이유일수 있으니 로그 남긴다.
            SUNLOG( eDEV_LOG, "[CG_QUEST_REWARD_SYN] char(%s), guid(%d), Quest(%d), num(%d)",
                pPlayer->GetCharName(), pPlayer->GetCharGuid(), pRecvMsg->m_QuestCode, pCompNum->m_Num );
        }

		GAMELOG->LogAction( ACT_QUEST_REWARD, pPlayer, NULL, pRecvMsg->m_QuestCode );
		if( (msg.m_Money-PreMoney) != 0 )
			GAMELOG->LogMoney( MONEY_REWARD_QUEST, pPlayer, (msg.m_Money-PreMoney) );

		if( (msg.m_i64Exp-OldExp) != 0 )
			GAMELOG->LogExp( EXP_QUEST, pPlayer, msg.m_i64Exp, OldExp );

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED        
#ifdef _NA_001990_ACSYSTEM_ADD
        pPlayer->GetACManager()->ProcessQuest(  pQuest->GetQuestInfo()->GetQuestCode(),
                                                pQuest->GetQuestInfo()->GetACCode(),
                                                pQuest->GetQuestInfo()->GetACType());
#else
        pPlayer->GetACManager()->IncConditionValue(pQuest->GetQuestInfo()->GetACCode(),
                                                   pQuest->GetQuestInfo()->GetACType(),
                                                   pQuest->GetQuestInfo()->GetQuestCode());        
#endif //_NA_001990_ACSYSTEM_ADD
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        if(pPlayer->GetQuestManager()->IncCompleteQuestCount(pRecvMsg->m_QuestCode))
        {
            WORD normalQ_count, dailyQ_count;
            pPlayer->GetQuestManager()->GetCompleteQuestCount(normalQ_count, dailyQ_count);
            pPlayer->GetACManager()->ProcessQuestCount(normalQ_count, dailyQ_count);
        }
#endif //_NA_003027_20111013_HONOR_SYSTEM
#endif
	}
	else
	{
		MSG_CG_QUEST_REWARD_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_QUEST_INSUFFICIENT_SPACE;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}
}

Handler_CG_QUEST_IMPL( CG_QUEST_FAILED_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_QUEST_FAILED_SYN * pRecvMsg = (MSG_CG_QUEST_FAILED_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_QUEST_IMPL(CG_QUEST_FAILED_SYN)] Not Exist GameField" );
		return;
	}

	Quest * pQuest = pPlayer->GetQuestManager()->FindQuest( pRecvMsg->m_QuestCode );
    //{__NA001282_20090206_MODIFY_QUEST_TIME
	// 현재 퀘스트의 fail은 시간 만료시 들어온다.
	// Update()는 서버측에서는 주기적으로 하지않는다.
	// 서버측 Update()는 완료 가능 체크와 fail체크에서 한다.
 	if( pQuest )
 	{
 		pQuest->Update();
 		if( pQuest->GetQState() == QS_FAILED )
 		{
			pPlayer->GetQuestManager()->DoFailed(pRecvMsg->m_QuestCode);

 			MSG_CG_QUEST_FAILED_ACK msg;
 			pPlayer->SendPacket( &msg, sizeof(msg) );
 			return;
 		}
 	}
 
 	MSG_CG_QUEST_FAILED_NAK msg;
 	msg.m_byErrorCode = 1;
 	pPlayer->SendPacket( &msg, sizeof(msg) );
    //}
}

Handler_CG_QUEST_IMPL( CG_QUEST_ABANDON_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_QUEST_ABANDON_SYN * pRecvMsg = (MSG_CG_QUEST_ABANDON_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_QUEST_IMPL(CG_QUEST_ABANDON_SYN)] Not Exist GameField" );
		return;
	}

	Quest * pQuest = pPlayer->GetQuestManager()->FindQuest( pRecvMsg->m_QuestCode );
	if( !pQuest || pQuest->GetQuestInfo()->IsChildQuest() )

	{
		MSG_CG_QUEST_ABANDON_NAK msg;
		msg.m_byErrorCode = 1;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}
	else
	{
		pQuest->DoAbandon(pPlayer);

		pPlayer->GetQuestManager()->DestroyQuest( pRecvMsg->m_QuestCode );
		MSG_CG_QUEST_ABANDON_ACK msg;
		msg.m_Force = pRecvMsg->m_Force;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		GAMELOG->LogAction( ACT_QUEST_ABANDON, pPlayer, NULL, pRecvMsg->m_QuestCode );
	}
}

Handler_CG_QUEST_IMPL( CG_QUEST_EVENT_AREA_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_QUEST_EVENT_AREA_SYN * pRecvMsg = (MSG_CG_QUEST_EVENT_AREA_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

    GameField* pField = pPlayer->GetField();
	if( !pField )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_QUEST_IMPL(CG_QUEST_EVENT_AREA_SYN)] Not Exist GameField" );
		return;
	}

    FieldInfo* pFieldInfo = pField->GetFieldInfo();
    if( !pFieldInfo )
    {
        SUNLOG( eCRITICAL_LOG, "[Handler_CG_QUEST_IMPL(CG_QUEST_EVENT_AREA_SYN)] Not Exist FieldInfo" );
        return;
    }

    WzSpecialArea* pAreaInfo = pFieldInfo->GetAreaInfo( pRecvMsg->m_AreaID );
    if( !pAreaInfo )
    {
        SUNLOG( eCRITICAL_LOG, "[Handler_CG_QUEST_IMPL(CG_QUEST_EVENT_AREA_SYN)] Not Exist WzSpecialArea[%d]", pRecvMsg->m_AreaID );
        return;
    }

    if( FALSE == pPlayer->CorrectPosition(pRecvMsg->m_wvCurPos, WzVector()) )
    {
        MSG_CG_QUEST_EVENT_AREA_NAK msg;
        msg.m_byErrorCode = RC::RC_QUEST_INCORRECT_POSITION;
        pPlayer->SendPacket( &msg, sizeof(msg) );

        const WzVector& pos = pRecvMsg->m_wvCurPos;
        SUNLOG( eCRITICAL_LOG, "[Handler_CG_QUEST_IMPL(CG_QUEST_EVENT_AREA_SYN)] " 
                                "INCORRECT_POSITION X[%d] Y[%d] Z[%d]", 
                                pos.x, pos.y, pos.z );
        return;
    }

    //.//

	QUEST_AREA_MSG msg;
    msg.m_AreaID = pAreaInfo->m_wiIdentity;

	if( !pPlayer->GetQuestManager()->Event(pRecvMsg->m_QuestCode,QUEST_EVENT_AREA,&msg) )
	{
		MSG_CG_QUEST_EVENT_AREA_NAK msg;
        msg.m_byErrorCode = RC::RC_QUEST_NOT_AREAID;
		pPlayer->SendPacket( &msg, sizeof(msg) );

        SUNLOG( eCRITICAL_LOG, "[Handler_CG_QUEST_IMPL(CG_QUEST_EVENT_AREA_SYN)] Not Exist WzSpecialArea" );

        return;
	}
    //ACK메시지는 Event성공 처리 루틴에서.
}

Handler_CG_QUEST_IMPL( CG_QUEST_INCOMPLETE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_QUEST_INCOMPLETE_SYN * pRecvMsg = (MSG_CG_QUEST_INCOMPLETE_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_QUEST_IMPL(CG_QUEST_INCOMPLETE_SYN)] Not Exist GameField" );
		return;
	}

	RC::eQUEST_RESULT rt = RC::RC_QUEST_SUCCESS;
	
	Quest *pQuest = pPlayer->GetQuestManager()->FindQuest( pRecvMsg->m_QuestCode );
	if ( !pQuest )
	{
		MSG_CG_QUEST_INCOMPLETE_NAK msg;
		msg.m_byErrorCode = RC::RC_QUEST_EMPTY;
		pPlayer->SendPacket( &msg, sizeof(msg) );		
		return;
	}	
	
	if( pQuest->GetQState() != QS_COMPLETED )
		rt = RC::RC_QUEST_NOT_COMPLETED_STATE;	

	if( rt == RC::RC_QUEST_SUCCESS )
	{
		rt = pPlayer->GetQuestManager()->CanComplete( pRecvMsg->m_QuestCode, pPlayer );
	}

	if( rt == RC::RC_QUEST_SUCCESS )
	{
		MSG_CG_QUEST_INCOMPLETE_ACK msg;
		msg.m_QuestCode = pRecvMsg->m_QuestCode;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}
	else
	{
		MSG_CG_QUEST_INCOMPLETE_NAK msg;
		msg.m_byErrorCode = rt;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}
}