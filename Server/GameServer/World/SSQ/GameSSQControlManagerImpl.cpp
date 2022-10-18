#include "stdafx.h"

#include <PacketStruct_AG.h>
#include <NPCInfoParser.h>

#include "GameInstanceDungeon.h"
#include "ItemManager.h"
#include "ItemExpDistributor.h"
#include "StatusManager.h"
#include "TradeManager.h"
#include "SummonManager.h"
#include "MovingPlanner.h"


#include <World/GameWarControlManager.h>
#ifdef _NA_001990_ACSYSTEM_ADD
#include "AchievementManager.h"
#endif //_NA_001990_ACSYSTEM_ADD

// for debugging control
#undef WAVERIX_DEBUGGING_OUTPUTn
#undef WAVERIX_DEBUGGING_OUTPUTnV

#define WAVERIX_DEBUGGING_OUTPUTn(msg)	\
SUNLOG(eCRITICAL_LOG, "[%s][%u][%u] %s\n", __FUNCTION__, __LINE__,	\
	   (DWORD)((GetTickCount()-tickvalue)/(1000)), msg);					\
	   tickvalue = GetTickCount()

#define WAVERIX_DEBUGGING_OUTPUTnV(value)	\
{ CHAR _message[10];					\
WAVERIX_DEBUGGING_OUTPUTn(itoa(value, _message, sizeof(_message))); }

static DWORD tickvalue = GetTickCount();

#include "GameSSQControlInfos.hxx"
#include "GameSSQPacketProcess.hxx"
#include "GameSSQUpdateCtrl.hxx"
#include "GameSSQControlManagerImpl.hxx"
#include "SSQHandlerInGame.h"


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	<GameSSQControlManagerImpl><Implementation>
//

GameSSQControlManagerImpl::GameSSQControlManagerImpl()
{
	const eSERVER_TYPE serverType = g_pGameServer->GetServerType();
	serverType == BATTLE_SERVER
		?	Control.RegisterBattleServerFunction()
		:	Control.RegisterFieldServerFunction()
		;
}

GameSSQControlManagerImpl::~GameSSQControlManagerImpl()
{
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


namespace GameSSQControlInfo
{
	//struct OpSSQInfoController
	//{
	//	typedef SSQInfoParser::SSQINFO_PAIR		SSQINFO_PAIR;
	//	GameSSQControlManagerImpl* const	m_pSSQImpl;
	//	sTIME_INFO						m_TimeInfo;

	//	OpSSQInfoController(GameSSQControlManagerImpl* const pImpl) : m_pSSQImpl(pImpl)
	//	{
	//		m_TimeInfo.CurTick = GetTickCount();
	//		util::TimeSync::GetLocalTime(&m_TimeInfo.SysTime);
	//		m_TimeInfo.WzTime = m_TimeInfo.SysTime.wHour*100 + m_TimeInfo.SysTime.wMinute;
	//	}

	//	VOID		Register(const SSQINFO_PAIR& rPair) const 
	//	{
	//		const INDEX_T IDofSSQ = rPair.first;
	//		SSQInfo* const pSSQInfo = rPair.second;
	//		GameSSQUpdateCtrl_None* const pCtrl = GameSSQUpdateCtrl_None_Info::Allocate();
	//		SSQMsgRouterInGame* const pRouter = router_info::Allocate();

	//		pCtrl->MigrationDefault(IDofSSQ, m_pSSQImpl, pRouter, pSSQInfo);
	//		pCtrl->Init();

	//		m_pSSQImpl->_RegisterUpdateCtrl(IDofSSQ, pCtrl);
	//	}
	//};
};


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


SSQ_UPDATERS_IT
GameSSQControlManagerImpl::_FindCodeOfSSQ(const SSQCODE CodeOfSSQ)
{
	SSQ_UPDATERS_IT itFind =
		std::find_if(UpdateList.begin(), UpdateList.end()
		, boost::bind(std::equal_to<SSQCODE>()
		, boost::bind(&SSQ_UPDATER_NODE::GetCodeOfSSQ, _1)
		, CodeOfSSQ
		));
	return itFind;
}

SSQ_UPDATERS_IT
GameSSQControlManagerImpl::_FindZoneKeyofSSQ(const KEYTYPE KeyOfSSQ)
{
	SSQ_UPDATERS_IT itFind =
		std::find_if(UpdateList.begin(), UpdateList.end()
		, boost::bind(std::equal_to<KEYTYPE>()
		, boost::bind(&SSQ_UPDATER_NODE::GetKey, _1)
		, KeyOfSSQ
		));
	return itFind;
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


VOID
GameSSQControlManagerImpl::_RegisterUpdateCtrl(const SSQCODE CodeOfSSQ, GameSSQUpdateCtrl* const pCtrl)
{
	SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(CodeOfSSQ);

	if(itFind != UpdateList.end())
	{
		SSQ_UPDATER_NODE& rNode = *itFind;
		rNode.pUpdater = pCtrl;
	}
	else
	{
		ASSERT(!"Impossible");
	}
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


VOID
GameSSQControlManagerImpl::_InitializeUpdateCtrl()
{
	SSQInfoParser* const pSSQInfoParser = SSQInfoParser::Instance();

	sTIME_INFO timeInfo;
	sTIME_INFO::SetTimeValue(timeInfo);

	typedef SSQInfoParser::SSQINFO_TABLE	SSQINFO_TABLE;
	const SSQINFO_TABLE& rTable = pSSQInfoParser->GetRefSSQInfo();
	FOREACH_CONTAINER(const SSQINFO_TABLE::value_type& rNode, rTable, SSQINFO_TABLE)
	{
		const SSQCODE CodeOfSSQ = rNode.first;
		SSQInfo* const pSSQInfo = rNode.second;
		GameSSQUpdateCtrl_None* const pCtrl = TAllocNew(GameSSQUpdateCtrl_None_Info::value_type);
		SSQMsgRouterInGame* const pRouter = TAllocNew(router_info::value_type);

		pCtrl->MigrationDefault(CodeOfSSQ, this, pRouter, pSSQInfo);
		pCtrl->Init();
		GameSSQUpdateCtrl::ConstVarsInit(pCtrl);

		//_RegisterUpdateCtrl(IDofSSQ, pCtrl);	<- »ç¿ë»ó È¥Àç·Î ÄÚµå¸¦ ÀÌ°÷À¸·Î ¿Å°Ü¹ö·È´Ù.
		SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(CodeOfSSQ);
		ASSERT(itFind == UpdateList.end());
		UpdateList.push_back(SSQ_UPDATER_NODE(CodeOfSSQ, pCtrl));
	}

	//OpSSQInfoController opr(this);
	//pSSQInfoParser->ForeachSSQInfoBin(&OpSSQInfoController::Register, opr);
	pSSQInfoParser->_LoadWat("./data/Map/Object/map_event01/map_event01.wat", false);
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


VOID
GameSSQControlManagerImpl::_ReleaseUpdateCtrl()
{
	FOREACH_CONTAINER(const SSQ_UPDATER_NODE& rNode, UpdateList, SSQ_UPDATERS)
	{
		if(!rNode.pUpdater)
			continue;

		GameSSQUpdateCtrl* const pCtrl = rNode.pUpdater;
		GameSSQUpdateCtrl::ConstVarsRelease(pCtrl);
		TAllocDelete(router_info::value_type, pCtrl->m_pMsgParser);

		switch(pCtrl->m_CurType)
		{
#undef SSQ_UPDATE_CTRL_NODE
#define SSQ_UPDATE_CTRL_NODE(_type)	\
    case _type::value: TAllocDelete(_type::value_type, pCtrl); break;

			SSQ_UPDATE_CTRL_LIST();
		}
	}
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


VOID
GameSSQControlManagerImpl::_ChangeUpdater(const SSQ_UPDATER_NODE& rNode)
{
}




//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	<Internal Message Handler Implementation>
//
VOID
GameSSQControlManagerImpl::_OnDead(GameSSQUpdateCtrl* const pUpdateCtrl, Character* const pTarget, Character* const pKiller)
{
	const DWORD ERRCODE_DEFAULT		= 100;
	//	Æ÷ÀÎÆ® °è»ê
	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FLOWCONTROL_START()
    {
		if(!FlowControl::FCAssert(pTarget))
			break;

		const eOBJECT_TYPE object_type_of_target = pTarget->GetObjectType();
		const bool target_isPlayer = (object_type_of_target&PLAYER_OBJECT)==PLAYER_OBJECT;
		const bool target_isSSQMob = (object_type_of_target&SSQMONSTER_OBJECT)==SSQMONSTER_OBJECT;
		const SSQInfo* const pSSQInfo = pUpdateCtrl->GetSSQInfo();

		// ±âÅ¸ npc, ¼ÒÈ¯¼ö µîµîÀÏ ¼öµµ ÀÖ´Ù... ÀÖ³ª? ±×·± °æ¿ì ½ºÅµ
		if(!(target_isPlayer ^ target_isSSQMob))
			break;

		BOOLEAN need_update = FALSE;	// reward Ã³¸® ¿©ºÎ, Á×À½ Ã³¸® ¿©ºÎ (ºÎÈ° »óÈ² ±ÝÁö)
		SSQCharacterStatusInGame* pControlStatus = 0;
		if(target_isPlayer)
		{	// ¹º°¡ÀÇ ÀÌÀ¯·Î Á×¾î¹ö¸± ¼ö ÀÖ´Ù.
			Player* const pDeadPlayer = static_cast<Player*>(pTarget);
			pControlStatus = &pDeadPlayer->SSQHandler->ControlStatus;
		}
		else
		{
			SSQMonster* const pDeadMob = static_cast<SSQMonster*>(pTarget);
			pControlStatus = &pDeadMob->ControlStatus;
		}

		SSQCharacterStatusInGame& rStatus = *pControlStatus;
		need_update = !(rStatus.FIELD & rStatus.CHECKED_DEAD);

		if(need_update)
		{
			rStatus.AddField(rStatus.CHECKED_DEAD);	// for sync
			pUpdateCtrl->SharedVars.UpdateDecreaseCount(target_isPlayer);
		}

		const eTIME_TYPE updater_type = pUpdateCtrl->GetTimeType();
		if(updater_type != GameSSQControlInfo::TT_BATTLE)
			break;

		//////////////////////////////////////////////////////////////////////////
		//	Target Leave Process <- ¹èÆ²½ÃÁ¡¿¡ ³»º¸³¾...±î?
		if(need_update && target_isPlayer)
		{
			Player* const pDeadPlayer = static_cast<Player*>(pTarget);
			ISSQHandlerInGame& rHandler = pDeadPlayer->SSQHandler;
			rHandler->ConfigEvent_OnDead(pDeadPlayer, SSQHandlerInGame::LeaveTimeout_OnDead);
		}

		//////////////////////////////////////////////////////////////////////////
		//	Killer Process
		//	ÀÌÈÄ Ã³¸®´Â Killer°¡ PlayerÀÎ °æ¿ì¿¡ Ã³¸®ÇÏ±â·Î ÇÑ´Ù. Killer°¡ ¾ø´Â °æ¿ìµµ ¸¶Âù°¡Áö.
		const bool killer_isPlayer = pKiller && pKiller->IsEqualObjectKind(PLAYER_OBJECT);
		if(!killer_isPlayer)
			break;

		Player* const pKillerPlayer = static_cast<Player*>(pKiller);

		ISSQHandlerInGame& rHandler = pKillerPlayer->SSQHandler;
		if(need_update)
		{
			//////////////////////////////////////////////////////////////////////////
			//	Point Process
			SSQ_RESERVED_REWARD_INFO::QuestFail rPoint;
			if(!rHandler->GetInfo_QuestFail(rPoint))
			{
				// ¿¡·¯ Ã³¸® ÇØ¾ß ÇÏ±ä ÇØ¾ß ÇÏ´Âµ¥....
				break;
			}

			const DWORD kills_of_mob	= (!!target_isSSQMob) + rPoint.MonsterKillNum;
			const DWORD kills_of_player	= (!!target_isPlayer) + rPoint.PlayerKillNum;

			rHandler->UpdateInfo_PlayPoint(0, 0, (BYTE)kills_of_mob, (BYTE)kills_of_player);
		};

		if(need_update)
		{	//	Reward In Battle
			//	need apply - battle condition (ÀÔ·ÂµÈ ¾ÆÀÌÅÛ ¼ÒÁøÀÌ ¿ä±¸µÈ´Ù. ¿©·¯°¡Áö °í·Á»çÇ×ÀÌ ¸¹´Ù.)
			RewardInBattle(pUpdateCtrl, pKillerPlayer);
		};

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;


	return /*RETURN FALSE VALUE*/;
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


BOOLEAN
GameSSQControlManagerImpl::_OnEnterEvent(GameSSQUpdateCtrl* const pUpdateCtrl, Character* const pCharacter)
{
	const eTIME_TYPE updater_type = pUpdateCtrl->GetTimeType();
	const bool bUseSkillOnlyMe = updater_type != TT_BATTLE;
	const bool isPlayer = pCharacter->IsEqualObjectKind(PLAYER_OBJECT);

	StatusManager* const pStatusManager = pCharacter->GetStatusManager();
	pStatusManager->Status.On_FilterControl(bUseSkillOnlyMe, TRUE, pUpdateCtrl->ConstVars.pCheckFilters);
	pStatusManager->ChangeInterState(eCHAR_STATE_SSQ_CTRL_BLOCK_ATTACK, bUseSkillOnlyMe);

	if(isPlayer)
	{
		Player* pPlayer = static_cast<Player*>(pCharacter);
		ISSQHandlerInGame& rRewardHandler = pPlayer->SSQHandler;
		if(!rRewardHandler.IsValidState())
			rRewardHandler.Init();

		const SSQInfo* const pSSQInfo = pUpdateCtrl->GetSSQInfo();

        //!_NA_20110630_BATTLE_ZONE_RENEWAL
		const DWORD object_key = pPlayer->GetObjectKey();

    #if WAVERIX_CODE_RENEWAL
        if(GameField* pGameField = pPlayer->GetField())
        {
            WAR_MSG_SINGLE_ASSEMBLER<MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD, \
                                     MSGSUB_SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED> assembler;
            assembler.MSG.ObjectKey = object_key;
            assembler.MSG.Cost = DWORD(pSSQInfo->m_Cost);
            assembler.Completion();
            MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD* pMsg = assembler.GetPacket();
            pMsg->m_dwKey = pSSQInfo->m_MapCode;
            pGameField->SendPacketToAgent(pMsg, assembler.GetSize());
        }
        else
        {
            ASSERT(!"Unexpected Status");
        }
    #endif
    #if WAVERIX_CODE_BACKUP
		BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = pUpdateCtrl->GetMsgRouter()->GetCMD_AG();
		WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
		{
			MSGSUB_SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED msg;
			msg.ObjectKey	= object_key;
			msg.Cost		= (DWORD)pSSQInfo->m_Cost;
			msg.SerializeTo(pHeader);
		}
    #endif
        // ÆêÀ» ¼ÒÈ¯ÇØÁ¦ ÇÑ´Ù.
        CPetManager& pet_manager = pPlayer->m_PetManager;
        pet_manager.ReturnPet(TRUE);
	}

	BOOLEAN counter_update = isPlayer || pCharacter->IsEqualObjectKind(SSQMONSTER_OBJECT);
	if(counter_update)
		pUpdateCtrl->SharedVars.UpdateIncreaseCount(isPlayer);

	return TRUE;
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


BOOLEAN
GameSSQControlManagerImpl::_OnLeaveEvent(GameSSQUpdateCtrl* const pUpdateCtrl,
                                         Character* const pCharacter)
{
	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		const BOOLEAN isPlayer = pCharacter->IsEqualObjectKind(PLAYER_OBJECT);
		if(!isPlayer)
			return TRUE;

		Player* pPlayer = static_cast<Player*>(pCharacter);
		ISSQHandlerInGame& rRewardHandler = pPlayer->SSQHandler;
		if(!FlowControl::FCAssert(rRewardHandler.IsValidState()))
			return FALSE;

		SSQCharacterStatusInGame& rStatus = rRewardHandler->ControlStatus;
		const BOOLEAN isAlive = !(rStatus.FIELD & rStatus.CHECKED_DEAD);
		if(isAlive)
			pUpdateCtrl->SharedVars.UpdateDecreaseCount(TRUE);

		const eEVENT_MODE_MULTIUSE reward_type = rRewardHandler->GetSSQEventID();
		switch(reward_type)
		{
		case eEVENT_MULTIUSE_SSQ_REFUND:			// ÀüÅõ Àü ³ª°£ ÇÃ·¹ÀÌ¾î ´ë»ó
		case eEVENT_MULTIUSE_SSQ_WINNER_REWARD:		// ÀüÅõ ÈÄ ³ª°£ ÇÃ·¹ÀÌ¾î ´ë»ó, °è»êÀº ±×¶§ ÀÌ·ïÁø´Ù. (¼øÀ§ µîµîÀÌ ÇÊ¿äÇÏ¹Ç·Î...)
			break;
		case eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO:	// ÀüÅõ Áß ³ª°£ ÇÃ·¹ÀÌ¾î ´ë»ó
			SettleAccounts(pUpdateCtrl, pPlayer);
			UpdateToRankingAndExtra(pUpdateCtrl, pPlayer);
			break;
		}

		//	DB Update´Â ¸ðµÎ ¿©±â¼­ ÀÌ·ïÁø´Ù.
		MSG_DG_EVENT_SSQ_REWARD_UPDATE_SYN msgSYN;
		msgSYN.m_dwKey		= pPlayer->GetUserGuid();
		msgSYN.m_CharGuid	= pPlayer->GetCharGuid();
		if(rRewardHandler->GetRewardInfo(msgSYN.m_RewardInfo))
		{
			g_pGameServer->SendToServer(GAME_DBPROXY, &msgSYN, msgSYN.GetSize());
		}

		return TRUE;
    }
    FLOWCONTROL_END;

	return FALSE;
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


BOOLEAN
GameSSQControlManagerImpl::ProcessGamble(
								GameSSQUpdateCtrl* const pUpdateCtrl
								, GameField* const pField
								, const DWORD prize_winner
								, MSGSUB_SSQ_S2S_GAMBLE_RESULT& msg
								)
{
	const SSQInfo* const pSSQInfo = pUpdateCtrl->GetSSQInfo();

	typedef GameField::_PLAYER_HASH		_PLAYER_HASH;
	const _PLAYER_HASH& rPlayers	= pField->GetRefPlayerList();
	const DWORD player_num			= (DWORD)rPlayers.size();

	DWORD selector = 0;
	FOREACH_CONTAINER(_PLAYER_HASH::value_type& rPlayer, rPlayers, _PLAYER_HASH)
	{
		Player* const pPlayer = rPlayer.second;
        if (obj_util::IsValid(pPlayer, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }

		ISSQHandlerInGame& rRewarder = pPlayer->SSQHandler;
		rRewarder->SetInfo_QuestFailed(0, 0, 0, 0);	// ÀÌ ½ÃÁ¡ºÎÅÍ´Â È¯ºÒÃ³¸®µÇÁö ¾Ê´Â´Ù.

		if(prize_winner == selector)
		{
			const MONEY sumOfAdmissionFee = (MONEY)pSSQInfo->m_Cost * player_num;
			const SSQInfo::RATIO_T chargeRatio = min(pSSQInfo->m_GamebleInfo.m_ChargeRatio, 100);
			const MONEY resultHeim =
                MONEY((sumOfAdmissionFee * DWORD((BYTE)(100 - chargeRatio))) / 100);

			//	°·ºíº¸»óÀ» ÀÌ ½ÃÁ¡¿¡ Ã³¸®ÇÒ °ÍÀÎ°¡? ¾Æ´Ô ³ªÁß¿¡ Ã³¸®ÇÒ °ÍÀÎ°¡?
			pPlayer->PlusMoney(resultHeim);

			ASSERT(SAFE_NUMERIC_TYPECAST(MONEY, resultHeim, DWORD));
			msg.RewardHeim = (DWORD)resultHeim;
			msg.CharName = pPlayer->GetCharName();

			GAMELOG->LogMoney(MONEY_REWARD_SSQ_GAMBLE, pPlayer, resultHeim);
		}
		++selector;
	}

	return TRUE;
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


DWORD
GameSSQControlManagerImpl::SettleAccounts(GameSSQUpdateCtrl* const pUpdateCtrl,
                                          Player* const pPlayer,
                                          const SSQHandlerInGame::eOrder order)
{
	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		ISSQHandlerInGame& rRewarder = pPlayer->SSQHandler;
		SSQ_RESERVED_REWARD_INFO info;
		if(!FlowControl::FCAssert(rRewarder->GetRewardInfo(info)))
			break;

		const BOOLEAN bFailInfo			= eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO == info.m_SharedInfo.EventID;

		const SSQInfo* const pSSQInfo	= pUpdateCtrl->GetSSQInfo();
		const eCHAR_TYPE char_type		= pPlayer->GetCharType();

		const DWORD point_of_player		= pSSQInfo->m_PlayerHuntingInfo[char_type].m_HuntingPoint;
		const DWORD point_of_monster	= pSSQInfo->m_NpcHuntingInfo[char_type].m_HuntingPoint;

		const FLOAT point_ratio =
			((DWORD)order < (DWORD)SSQHandlerInGame::eOrder_None)
			?	pSSQInfo->m_SuccessPointInfo[ order ].m_FloatPoint
			:	1.0f
			;

		const DWORD kills_of_player	= bFailInfo ? info.m_QuestFail.PlayerKillNum : info.m_WinnerReward.PlayerKillNum;
		const DWORD kills_of_monster= bFailInfo ? info.m_QuestFail.MonsterKillNum : info.m_WinnerReward.MonsterKillNum;

		//	Formula by Planning
		//	-	(PC HuntingPoints + NPC HuntingPoints)*order_type
		const DWORD total_point =
            DWORD(((point_of_player*kills_of_player) + (point_of_monster*kills_of_monster)) * point_ratio);

		const SSQInfo::SUMMONDED_NPC_INFO& rNPCs = pSSQInfo->m_SummonedNpcInfo;
		const MONSTERCODE monster_code = rNPCs.List[0].m_NPCCode;
		const BASE_NPCINFO* const pNpcInfo = NPCInfoParser::Instance()->GetNPCInfo(monster_code);
		const FLOAT exp_ratio_monster = pNpcInfo->m_fGradeExpRatio;

		GameZone* const pGameZone = pUpdateCtrl->GetSSQField();
		DistributeExpOprArgs args(pGameZone,
                                  pGameZone->GetEntryField(),
                                  pGameZone->GetZoneType(),
                                  0,
                                  0,
                                  1,
                                  pPlayer->GetLevel(),
                                  0.0f,
                                  monster_code,
                                  0,
                                  exp_ratio_monster,
                                  pNpcInfo->m_dwMaxHP,	// ÀÌ ¼öÄ¡´Â Á»´õ µûÁ®º¼ ÇÊ¿ä ÀÖ´Ù.
			                      pNpcInfo->m_Level);	// ÀÌ ¼öÄ¡´Â Á»´õ µûÁ®º¼ ÇÊ¿ä ÀÖ´Ù.

		const FLOAT exp_ratio_player_weight = pSSQInfo->m_ExpRatioInfo[char_type].m_FloatPoint;
		DistributeExpOprResult query_result;
		g_ItemExpDistributor.GetCalcedExpValue(pPlayer, &args, &query_result);
		const EXPTYPE& result_exp_monster	= query_result.TotalExp;
		const EXPTYPE result_exp_player		= (EXPTYPE)(result_exp_monster * exp_ratio_player_weight);
		//	Formula by Planning
		const DWORD total_exp =
			(DWORD)((result_exp_player*exp_ratio_player_weight)*kills_of_player)
			+ (DWORD)((result_exp_monster*exp_ratio_player_weight)*kills_of_monster)
			;

		rRewarder->UpdateInfo_PlayPoint(total_point, total_exp, 0, 0);
		rRewarder->SetBonusExpRatio(query_result.BonusExp);

		return total_point;
    }
    FLOWCONTROL_END;

	return 0;
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


VOID
GameSSQControlManagerImpl::UpdateToRankingAndExtra(GameSSQUpdateCtrl* const pUpdateCtrl,
                                                   Player* const pPlayer)
{
	ISSQHandlerInGame& rRewarder = pPlayer->SSQHandler;
	SSQ_RESERVED_REWARD_INFO info;
	if(!rRewarder->GetRewardInfo(info))
		return;

	const eEVENT_MODE_MULTIUSE reward_type = rRewarder->GetSSQEventID();
	switch(reward_type)
	{
	case eEVENT_MULTIUSE_SSQ_REFUND:			// ÀüÅõ Àü ³ª°£ ÇÃ·¹ÀÌ¾î ´ë»ó
		break;
	case eEVENT_MULTIUSE_SSQ_WINNER_REWARD:		// ÀüÅõ ÈÄ ³ª°£ ÇÃ·¹ÀÌ¾î ´ë»ó, °è»êÀº ±×¶§ ÀÌ·ïÁø´Ù. (¼øÀ§ µîµîÀÌ ÇÊ¿äÇÏ¹Ç·Î...)
	case eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO:	// ÀüÅõ Áß ³ª°£ ÇÃ·¹ÀÌ¾î ´ë»ó
		{
        #ifdef _NA000000_100427_SSQ_DEFAULT_REWARD_
            // (NOTE) (f100427.1L) default reward
            RewardDefault(pUpdateCtrl, pPlayer);
        #endif

			GameInstanceDungeon* const pIndun = pUpdateCtrl->GetSSQField();
#ifdef _NA_008012_20150130_RANKING_SYSTEM
			RankingSystemCtl* const pRanking = pIndun->GetRankingSystemCtl();
			if(!pRanking)
				break;
#endif // _NA_008012_20150130_RANKING_SYSTEM

			const DWORD total_point
				= eEVENT_MULTIUSE_SSQ_WINNER_REWARD == reward_type
				?	info.m_WinnerReward.Point
				:	info.m_QuestFail.Point
				;
#ifdef _NA_008012_20150130_RANKING_SYSTEM
			pRanking->SetLastSettleAccount(pPlayer, (WORD)total_point);
#endif // _NA_008012_20150130_RANKING_SYSTEM

			const DWORD total_exp =
                eEVENT_MULTIUSE_SSQ_WINNER_REWARD == reward_type ? info.m_WinnerReward.Exp
                                                                 : info.m_QuestFail.Exp;

			pPlayer->AddExp(total_exp, pPlayer->GetObjectKey(), rRewarder->GetBonusExpRatio());
		}
		//SettleAccounts(pUpdateCtrl, pPlayer);
		break;
	}
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


BOOLEAN
GameSSQControlManagerImpl::_RewardProcess(const eRewardSelect reward_selector,
                                          GameSSQUpdateCtrl* const pUpdateCtrl,
                                          Player* const pPlayer,
                                          const SSQInfo::REWARD_NODE& rRewardNode,
                                          const SSQHandlerInGame::eOrder order)
{
	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FLOWCONTROL_START()
    {
		// Check Inventory Constraint
		ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
		if(!rPROTECT.IsUsable())
			return FALSE;

		SLOTCODE item_code = rRewardNode.m_RewardCode;
		POSTYPE item_num = (POSTYPE)rRewardNode.m_RewardLimit;

		if(!(item_code && item_num))
			return FALSE;

		ItemManager* const pItemManager = pPlayer->GetItemManager();

		BYTE msg_stream[max(sizeof(MSG_CG_ZONE_EX_SSQ_WINNER_REWARD_CMD),
                            sizeof(MSG_CG_ITEM_OBTAIN_ITEM_CMD))];

		MSG_CG_ZONE_EX_SSQ_WINNER_REWARD_CMD* pCmdWinner = 0;
		MSG_CG_ITEM_OBTAIN_ITEM_CMD* pCmdBattle = 0;

		if(reward_selector & eReward_ForWinner)
			pCmdWinner = new (msg_stream) MSG_CG_ZONE_EX_SSQ_WINNER_REWARD_CMD;
		else
			pCmdBattle = new (msg_stream) MSG_CG_ITEM_OBTAIN_ITEM_CMD;

		// ÀÌ·¸°Ô ÇØ¾ß ÇÑ´Ù. Winner Reward JUST_~_INFO->INVEN~_INFO·Î º¯È¯ÇÒ °Í. resultµµ Æ÷ÇÔÇÒ °Í.
		INVENTORY_TOTAL_INFO* pInventoryTotalInfo = reward_selector&eReward_ForWinner
			                                      ?     &pCmdWinner->m_TotalInfo
			                                      :     &pCmdBattle->m_tagInventory;
        RC::eITEM_RESULT result = RC::RC_ITEM_SUCCESS;
        ;{  // (CHANGES) (f100427.1L) give the SSQ default reward regardless dead state
            // to support do need the dead control with NO event
            bool dead_control = ((reward_selector & eReward_ForWinner) == 0 &&
                                 pPlayer->IsDead() != false);
            if (dead_control) {
                pPlayer->SetHP(1);
            }
            result = pItemManager->ObtainAllKindOfItem(1, &item_code, &item_num, pInventoryTotalInfo);
            if (dead_control) {
                pPlayer->SetHP(0);
            }
        };
        const WORD msg_size = (reward_selector & eReward_ForWinner) ?
            pCmdWinner->GetSize() : pCmdBattle->GetSize();

        if (reward_selector & eReward_ForWinner)
        {
            pCmdWinner->m_Result = result;
            pPlayer->SendPacket((MSG_BASE_FORWARD*)msg_stream, msg_size);
            if(result != RC::RC_ITEM_SUCCESS)
                break;
            //GAMELOG-> º¸»ó Ã³¸® °ü·Ã, º¸»ó ¼º°ø/½ÇÆÐ ·Î±×·Î ÇØ¼­ ³ªÁß¿¡ º¹±¸Ã³¸®ÇÒ ¼ö ÀÖµµ·Ï Áö¿øÇÏÀÚ.
            GAMELOG->LogMultiItem(ITEM_REWARD_SSQ_WINNER, pPlayer, 1, &item_code, &item_num);
        }
        else if (result == RC::RC_ITEM_SUCCESS)
        {   //  ÀüÅõ Áß °æÇ° Ã³¸® ½ÇÆÐÀÎ °æ¿ì¶ó¸é Â÷¶ó¸® º¸³»Áö ¾Ê´Â ÆíÀÌ ³ªÀ» °ÍÀÌ´Ù.
            pPlayer->SendPacket((MSG_BASE_FORWARD*)msg_stream, msg_size);
            // NOTE: f110602.3L, the selection whether the player was basically rewarded
            // for his particifation.
            const int log_type = (reward_selector & this->eReward_InBattle) ?
                ITEM_REWARD_SSQ_BATTLE_PRIZE : ITEM_REWARD_SSQ_DEFAULT_PRIZE;

            GAMELOG->LogMultiItem(log_type, pPlayer, 1, &item_code, &item_num);
        }
        else
        {
            break;
        }

#ifdef _NA_001990_ACSYSTEM_ADD
        if(RC::RC_ITEM_SUCCESS == result)
        {
            const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
            if (item_info->m_wACCode)
                pPlayer->GetACManager()->ProcessItemObtain(item_info->m_Code, item_info->m_wACCode, 
                    item_info->m_byACReferenceID, item_num);
        }
#endif //_NA_001990_ACSYSTEM_ADD
		
		return TRUE;
    }
    FLOWCONTROL_END;

	return FALSE;
}


BOOLEAN
GameSSQControlManagerImpl::RewardForWinner(GameSSQUpdateCtrl* const pUpdateCtrl,
                                           Player* const pPlayer,
                                           const SSQHandlerInGame::eOrder order) // 1,2,3µî 
{
	//	ÃÖÁ¾ ¿ì½ÂÀÚ¿¡ ´ëÇÑ ¾ÆÀÌÅÛ Áö±Þ Ã³¸®, UI reportingÀº ¸¶À»|ÇÊµå¿¡¼­ º°µµ·Î Ã³¸®µÈ´Ù.

	const SSQInfo* const pSSQInfo = pUpdateCtrl->GetSSQInfo();

	ASSERT((DWORD)SSQHandlerInGame::eOrder_None > (DWORD)order);
	const SSQInfo::REWARD_WINNER_CHAR_INFO& rInfo = pSSQInfo->m_RewardWinnerInfo;
	const eCHAR_TYPE char_type = pPlayer->GetCharType(); 
	const SSQInfo::REWARD_NODE& rNode = rInfo[order][char_type];

	const BOOLEAN item_rewarded = _RewardProcess(eReward_ForWinner, pUpdateCtrl, pPlayer, rNode, order);

	ISSQHandlerInGame& rRewarder = pPlayer->SSQHandler;
	SSQ_RESERVED_REWARD_INFO::WinnerReward rWinner;
	if(!FlowControl::FCAssert(rRewarder->GetInfo_QuestWinner(rWinner)))
		return FALSE;

	rWinner.ItemCode = !item_rewarded ? rNode.m_RewardCode : 0 ;
	rRewarder->SetInfo_WinnerReward(rWinner.Point, rWinner.Exp, rWinner.MonsterKillNum, rWinner.PlayerKillNum, rWinner.ItemCode);

	return TRUE;
}

BOOLEAN
GameSSQControlManagerImpl::RewardInBattle(GameSSQUpdateCtrl* const pUpdateCtrl,
                                          Player* const pPlayer)
{
	const eTIME_TYPE updater_type = pUpdateCtrl->GetTimeType();
	if(updater_type != GameSSQControlInfo::TT_BATTLE)
		return FALSE;

	GameSSQUpdateCtrl_Battle* const pBattleUpdater = static_cast<GameSSQUpdateCtrl_Battle*>(pUpdateCtrl);
	GameWarSSQItemDistribution& rItemDistributer = pBattleUpdater->ItemDistributer;

	const BYTE cur_earthquake_step = pBattleUpdater->GetCurEarthquakeStep();
	const SSQInfo::REWARD_NODE rNode = rItemDistributer.Drop(cur_earthquake_step);
	if(!rNode.m_RewardCode)
		return TRUE;

	if(_RewardProcess(eReward_InBattle, pUpdateCtrl, pPlayer, rNode, SSQHandlerInGame::eOrder_None))
	{
		SSQMsgRouterInGame* const pMsgRouter = pUpdateCtrl->GetMsgRouter();
		BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = pMsgRouter->GetCMD_AG();
		WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
		{
			MSGSUB_SSQ_PROTOCOL_INFORMATION_REWARD_ON_PLAYING msg;
			msg.CharName = pPlayer->GetCharName();
			msg.SerializeTo(pHeader);
		}

		return TRUE;
	}
	return FALSE;
}

#ifdef _NA000000_100427_SSQ_DEFAULT_REWARD_
// (NOTE) default reward process for all joined players
BOOLEAN
GameSSQControlManagerImpl::RewardDefault(GameSSQUpdateCtrl* const update_ctrl,
                                         Player* const player)
{
    const eTIME_TYPE updater_type = update_ctrl->GetTimeType();
    if (updater_type < GameSSQControlInfo::TT_BATTLE) {
        return false;
    }

    const SSQInfo* ssq_info = update_ctrl->GetSSQInfo();
    if (_RewardProcess(eReward_Default, update_ctrl, player, ssq_info->m_DefaultRewardInfo,
                       SSQHandlerInGame::eOrder_None))
    {
        return true;
    }
    return false;
}
#endif

//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	<Internal Message Handler Implementation>
//

namespace GameSSQControlInfo
{
	static const GM_Spec::SSQ _GM_CMDs[GM_Spec::eCmd_upperbound]
	= {
		{ GM_Spec::eCmd_open,			"open",		},
		{ GM_Spec::eCmd_close,			"close",	},
		{ GM_Spec::eCmd_warp,			"warp",		},
		{ GM_Spec::eCmd_status,			"status",	},
		{ GM_Spec::eCmd_jump,			"jump",		},
		{ GM_Spec::eCmd_summon,			"summon",	},
		{ GM_Spec::eCmd_observer,		"observer",	},
		//	insert items
		//	...
	};
};

#include <SinTable.h>

#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
extern BOOLEAN	Variation_SSQ_Summon_R1(const GameSSQCtrlCmd::GMCmd& rInfo, GameSSQUpdateCtrl* const pCtrl, Player* const pGmPlayer);
extern BOOLEAN	Variation_SSQ_Summon_R2(const GameSSQCtrlCmd::GMCmd& rInfo, GameSSQUpdateCtrl* const pCtrl, Player* const pGmPlayer);
#endif

BOOLEAN
GameSSQControlManagerImpl::GM_Command_Process(const GameSSQCtrlCmd::GMCmd& rInfo, Player* const pGmPlayer)
{
	//
	GM_Spec::eCmd cmd_type = GM_Spec::eCmd_upperbound;

	{
		typedef util::ARRAY_TYPEINFO< const GM_Spec::SSQ,
            0, GM_Spec::eCmd_upperbound >	array_info;
		FOREACH_ARRAY(const GM_Spec::SSQ& rSpec, _GM_CMDs, array_info)
		{
			const INT iRet = memcmp(rSpec.CmdString, rInfo.Arguments[1], sizeof(GM_Spec::String));
			if(iRet)
				continue;
			cmd_type = rSpec.GM_Cmd;
			break;
		}
	};

	if(GM_Spec::eCmd_upperbound == cmd_type)
		return FALSE;

	//	parse [2] string, [3] string
	SSQInfo::SSQCODE CodeOfSSQ = 0;
    GM_Spec::eOption gm_option = GM_Spec::eOption_None;
	switch(cmd_type)
	{
	case GM_Spec::eCmd_open:	//ssq open [mapcode]
	case GM_Spec::eCmd_close:	//ssq close [mapcode]
	case GM_Spec::eCmd_status:	//ssq status [mapcode]
		{
			CodeOfSSQ = (SSQInfo::SSQCODE)atoi(rInfo.Arguments[2]);
            if(rInfo.Arguments[3][0] != '\0')
            {   //³ªÁß¿¡ Å×ÀÌºíÈ­ ½ÃÅ°ÀÚ.
                if(strncmp(rInfo.Arguments[3], "fast", _countof(rInfo.Arguments[3])) == 0)
                    gm_option = GM_Spec::eOption_FastTrack;
            }
		}
		break;
	case GM_Spec::eCmd_warp:	//ssq warp
	case GM_Spec::eCmd_jump:	//ssq jump	(´Ü°è·Î ³Ñ¾î°£´Ù)
	//case GM_Spec::eCmd_summon:	//ssq summon
	case GM_Spec::eCmd_observer:
		{
			GameField* pCurField = pGmPlayer->GetField();
			GameZone* pCurZone = 0;
			if(!(pCurField
				&& (pCurZone=pCurField->GetGameZone())
				&& (pCurZone->GetZoneType() == eZONETYPE_INSTANCE)
				))
				return FALSE;

			CodeOfSSQ = pCurZone->GetMapCode();
		}
		break;
	case GM_Spec::eCmd_summon:
		{
#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
			if(!Variation_SSQ_Summon_R1(rInfo, 0, pGmPlayer))
				return FALSE;
#endif
		}
		break;
	}

	SSQ_UPDATERS_IT itFind	= _FindCodeOfSSQ(CodeOfSSQ);
	if(itFind == UpdateList.end())
		return FALSE;

	SSQ_UPDATER_NODE& rNode			= *itFind;
	GameSSQUpdateCtrl* const pCtrl	= rNode.pUpdater;

	GameInstanceDungeon* const pDungeon = pCtrl->GetSSQField();
	if(!pDungeon && GM_Spec::eCmd_jump == cmd_type)
	{
		return FlowControl::FCAssert(!"Impossible Logic");
	}
	if(pDungeon && GM_Spec::eCmd_open == cmd_type)
	{
		MSG_CG_ETC_DEBUGINFO_CMD msgCMD;
		INT iLen = _snprintf(msgCMD.m_pszChatMsg, _countof(msgCMD.m_pszChatMsg),
                             "Already Opened - mapcode [%u]",
                             CodeOfSSQ);
		msgCMD.m_byMsgLength = (BYTE)iLen;
		pGmPlayer->SendPacket(&msgCMD, msgCMD.GetSize());
		return TRUE;
	}

	switch(cmd_type)
	{
	case GM_Spec::eCmd_open:
	case GM_Spec::eCmd_close:
	case GM_Spec::eCmd_status:
	case GM_Spec::eCmd_jump:
		{
			SSQMsgRouterInGame* const pMsgRouter = pCtrl->GetMsgRouter();
			WAR_MSG_SINGLE_ASSEMBLER< MSG_AG_ZONE_SSQ_ROOM_INFO_BRD, MSGSUB_SSQ_S2S_GM_CONTROL_REQ > assembler;
			GM_Spec::MsgFormat& rCommand = assembler.MSG.Command;
			rCommand.Cate		= GM_Spec::eCategory_SSQ;
			rCommand.GM_Cmd		= cmd_type;
			rCommand.CodeOfSSQ	= CodeOfSSQ;
			rCommand.GMObjectKey= pGmPlayer->GetObjectKey();
            rCommand.Option     = gm_option;
			assembler.Completion();

			MSG_AG_ZONE_SSQ_ROOM_INFO_BRD* const pMsg = assembler.GetPacket();
			pMsg->m_dwKey = CodeOfSSQ;
			g_pGameServer->SendToServer(AGENT_SERVER, assembler.GetPacket(), assembler.GetSize());
		}
		break;
	case GM_Spec::eCmd_warp:
		{
			GameField* pField = pGmPlayer->GetField();
			WzVector center_pos = { SSQClosedSpaceCenter_X, SSQClosedSpaceCenter_Y, SSQClosedSpaceCenter_Z+1.0f };
			pField->TeleportObject(pGmPlayer, &center_pos, TRUE);
		}
		break;
	case GM_Spec::eCmd_observer:
		{
			const SSQInfo* const pSSQInfo = pCtrl->GetSSQInfo();
			GameInstanceDungeon* const pIndun = pCtrl->GetSSQField();
			if(!pIndun)
				break;

			using namespace nsSSQ::Observer;
			GameWarObserverMode observer(pIndun);
			if(rInfo.Arguments[2][0] == '\0')
			{
                const Rules::eC2G_ReqRule rules =
                    Rules::eC2G_ReqRule(Rules::eC2G_ReqRule_CondInSSQField |
                                        Rules::eC2G_ReqRule_CondObserverModeOff |
                                        Rules::eC2G_ReqRule_CondDeadState);
				observer.Enter(rules, pGmPlayer);
			}
			else
			{
                const Rules::eC2G_ReqRule rules =
                    Rules::eC2G_ReqRule(Rules::eC2G_ReqRule_CondInSSQField |
                                        Rules::eC2G_ReqRule_CondObserverModeOn |
                                        Rules::eC2G_ReqRule_CondDeadState);
				const Info::eKeyCmd key_cmd = (Info::eKeyCmd)atoi(rInfo.Arguments[2]);
				observer.ChangeCoordinates(rules, pGmPlayer, key_cmd);
			}
		}
		break;
	case GM_Spec::eCmd_summon:
		{
        #ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
			if(!Variation_SSQ_Summon_R2(rInfo, pCtrl, pGmPlayer))
				return FALSE;
        #endif
		}
		break;
	}

	return TRUE;
}


//¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬¦¬
//	<Packet Handler Implementation>
//

SSQ_PACKET_HANDLER_AG_DECL(AG_ZONE,		AG_ZONE_SSQ_JOIN_INFO_SYN)
{
	const DWORD userKey = pRecvMsg->m_dwKey;
	Player* const pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(userKey);
	if(!pPlayer) return;

	const SSQCODE CodeOfSSQ = SSQCODE(pRecvMsg->m_CodeOfSSQ);

	//////////////////////////////////////////////////////////////////////////
	//	(WAVERIX)(NOTE)(080217)
	//	-	Çã¿ë Á¶°Ç(in Client)
	//		-	1) in FIELD_SERVER -- checked
	//		-	2) Ä«¿À »óÅÂ ºÒ°¡ -- checked
	//		-	3) ÀÔÀå °¡´É½Ã°£ -- checked
	//		-	4) ÇØ´ç ¾ÆÀÌÅÛ Á¸Àç	<- GameServer
	//		-	5) ÀÔÀå °¡´É ÇÏÀÓ º¸À¯ ¿©ºÎ <- GameServer
	//		-	6) ÀÔÀå °¡´É ·¹º§ -- checked
	//////////////////////////////////////////////////////////////////////////

	BYTE errorGroup = MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::RET_ROOM;

	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		//	<assertion, Å°°¡ ÀÖÀ¸¸é pInstanceDungeonµµ Á¸ÀçÇÑ´Ù.

		//	ÀÌ ÆÐÅ¶Àº FieldServer¿¡¼­¸¸ Ã³¸®µÇ´Â ÆÐÅ¶ÀÌ´Ù. <- error code Ãß°¡ÇÒ °Í
		IfFailThenBreak(Control.IsCheckedFieldServer(), RC::RC_ROOM_INVALID_ROOM_TYPE);

		SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(CodeOfSSQ);
		if(itFind == UpdateList.end())
			return;

		SSQ_UPDATER_NODE& rNode = *itFind;
		GameSSQUpdateCtrl* const pCtrl = rNode.GetUpdater();
		//	Agent¿¡ ÀÇÇØ Level, Chao, µîÀÌ °Ë»çµÇ¾úÀ½.

		const SSQInfo* const pSSQInfo = pCtrl->GetSSQInfo();

		{	//	º¸»ó/È¯ºÒ Á¤º¸ ÀÖ´ÂÁö °Ë»ç
			ISSQHandlerInGame& rHandler = pPlayer->SSQHandler;
			if(rHandler.IsValidState())
			{
				errorGroup = MSG_AG_ZONE_SSQ_JOIN_ACK::RET_ROOM;
				//IfFailThenBreak(!rHandler->IsDBTransaction(), RC::RC_ROOM_FAILED);
				//	// ½Ã°£ µ¿±âÈ­ »óÀÇ ¹®Á¦, º¹ÀâÇÑ ÆÐÅ¶ÇÃ·Î¿ì¸¦ ¹èÁ¦ÇÑ °á°ú·Î ÀÎÇÑ °Í... ±×³É Àç ¿äÃ»ÇÏµµ·Ï À¯µµÇÏÀÚ.

				SSQ_RESERVED_REWARD_INFO info;	// º¸»ó/È¯ºÒ Á¤º¸°¡ ¾ø¾î¾ßÁö ´ÙÀ½ ´Ü°è·Î ³Ñ¾î°¥ ¼ö ÀÖ´Ù.
				IfFailThenBreak(!rHandler->GetRewardInfo(info),
                                RC::RC_ROOM_CANNOT_ENTER_BEFORE_REWARD_OR_REFUND);
			}
		};

		SLOTCODE rItemCode = 0;
		{	//	Á¦¾à Á¶°Ç °Ë»ç
			errorGroup = MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::RET_ROOM;
			const MONEY money = pPlayer->GetMoney();
			IfFailThenBreak(money >= pSSQInfo->m_Cost, RC::RC_ROOM_INSUFFICIENT_MONEY);

            //!_NA_20110630_BATTLE_ZONE_RENEWAL
		};
		{	//	°­Á¦ »óÅÂ º¯°æ Ã³¸®
			//	1)	ÆÄÆ¼ »óÅÂ ÇØÁ¦								<- by Agent
			//	--JOIN--2)	°³ÀÎ»óÁ¡ »óÅÂ ÇØÁ¦
			//	--JOIN--3)	Pet¼ÒÈ¯ »óÅÂ ÇØÁ¦
			//	--JOIN--4)	¼ÒÈ¯Ã¼ »óÅÂ(Summon Valkyrie)ÇØÁ¦
			//	°­Á¦ »óÅÂ ÇØÁ¦´Â JOIN_ACKÀ» Ã³¸®ÇÏ±â Àü¿¡ Ã³¸®ÇÏ´Â ÇüÅÂ·Î È®Á¤
			//	TODO(WAVERIX)
		};

		{	//	Success
			MSG_AG_ZONE_SSQ_JOIN_INFO_ACK msgACK;
			msgACK.SetSuccessResult(CodeOfSSQ, rItemCode);
			pPlayer->SendPacket(&msgACK, msgACK.GetSize());
		};

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;
	
	//////////////////////////////////////////////////////////////////////////
	//	<ERROR CONTROL>
	const DWORD errorCode = flow.GetErrorCode();

	MSG_AG_ZONE_SSQ_JOIN_INFO_ACK msgFAIL;
	msgFAIL.SetFailResult(errorGroup, errorCode);
	pPlayer->SendPacket(&msgFAIL, msgFAIL.GetSize());

	//	Post Process
	if(MSG_AG_ZONE_SSQ_JOIN_ACK::RET_ROOM == errorGroup)
	{
		switch(errorCode)
		{
		case RC::RC_ROOM_CANNOT_ENTER_BEFORE_REWARD_OR_REFUND:
			//pPlayer->SSQHandler->Send_RewardExistInfo(pPlayer);	// not check SSQHandler.IsValidState()
			pPlayer->SSQHandler->Send_RewardProcess(pPlayer, FALSE);
			break;
		}
	}
	
	return /*RETURN FALSE VALUE*/;
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


SSQ_PACKET_HANDLER_AG_DECL(AG_ZONE,		AG_ZONE_SSQ_JOIN_SYN)
{
	WAVERIX_DEBUGGING_OUTPUTn(0);

	const SSQInfo::INDEX_T IDofSSQ = (SSQInfo::INDEX_T)pRecvMsg->m_CodeOfSSQ;

	//////////////////////////////////////////////////////////////////////////
	//	(WAVERIX)(NOTE)(080217)
	//	-	Çã¿ë Á¶°Ç(in Client)
	//		-	1) in FIELD_SERVER -- checked
	//		-	2) Ä«¿À »óÅÂ ºÒ°¡ -- checked
	//		-	3) ÀÔÀå °¡´É½Ã°£ -- checked
	//		-	4) ÇØ´ç ¾ÆÀÌÅÛ Á¸Àç	<- GameServer
	//		-	5) ÀÔÀå °¡´É ÇÏÀÓ º¸À¯ ¿©ºÎ <- GameServer
	//		-	6) ÀÔÀå °¡´É ·¹º§ -- checked
	//////////////////////////////////////////////////////////////////////////

	BYTE errorGroup = MSG_AG_ZONE_SSQ_JOIN_ACK::RET_ROOM;
    Player* pPlayer = 0;

	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		//	<assertion, Å°°¡ ÀÖÀ¸¸é pInstanceDungeonµµ Á¸ÀçÇÑ´Ù.

		//	ÀÌ ÆÐÅ¶Àº FieldServer¿¡¼­¸¸ Ã³¸®µÇ´Â ÆÐÅ¶ÀÌ´Ù.
		if(Control.IsCheckedFieldServer())
		{
			const DWORD userKey = pRecvMsg->m_dwKey;
			pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(userKey);
			if(pPlayer == 0)
                return;

			SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(IDofSSQ);
			if(itFind == UpdateList.end())
				return;

			SSQ_UPDATER_NODE& rNode			= *itFind;
			GameSSQUpdateCtrl* const pCtrl	= rNode.GetUpdater();
			//	Agent¿¡ ÀÇÇØ Level, Chao, µîÀÌ °Ë»çµÇ¾úÀ½.

			const SSQInfo* const pSSQInfo = pCtrl->GetSSQInfo();

			{	//	Á¦¾à Á¶°Ç °Ë»ç
				errorGroup = MSG_AG_ZONE_SSQ_JOIN_ACK::RET_ROOM;
				const MONEY money = pPlayer->GetMoney();
				IfFailThenBreak(money >= pSSQInfo->m_Cost, RC::RC_ROOM_INSUFFICIENT_MONEY);

                //!_NA_20110630_BATTLE_ZONE_RENEWAL
			};

			//	°­Á¦ »óÅÂ º¯°æ Ã³¸®
			//	1)	ÆÄÆ¼ »óÅÂ ÇØÁ¦								<- by Agent
			//	--JOIN--2)	°³ÀÎ»óÁ¡ »óÅÂ ÇØÁ¦
			//	--JOIN--3)	Pet¼ÒÈ¯ »óÅÂ ÇØÁ¦
			//	--JOIN--4)	¼ÒÈ¯Ã¼ »óÅÂ(Summon Valkyrie)ÇØÁ¦
			{	//	--JOIN--2)	°³ÀÎ»óÁ¡ »óÅÂ ÇØÁ¦
                ePLAYER_BEHAVE_STATE behaveState = pPlayer->GetBehave();
				if(behaveState == PLAYER_BEHAVE_TRADE_STATE)
				{
					MSG_CG_TRADE_CANCEL_CMD msgCMD;
					Player* pCounterPlayer = pPlayer->GetTradeContainer()->GetCounterPlayer();
					if(pCounterPlayer->GetBehave() != PLAYER_BEHAVE_TRADE_STATE)
					{
						SUNLOG(eCRITICAL_LOG, "Move½ÃTRADE_STATE»óÅÂÀÌ»ó[1:%s][2:%s],%u", 
							   pPlayer->GetCharName(),
                               pCounterPlayer->GetCharName(), pCounterPlayer->GetBehave());
					}
					else
					{	
						pCounterPlayer->SendPacket(&msgCMD, sizeof(msgCMD));
					}

					g_TradeManager.EndTrade(pPlayer);
                    behaveState = pPlayer->GetBehave();
					pPlayer->SendPacket(&msgCMD, sizeof(msgCMD));
				}

                errorGroup = MSG_AG_ZONE_SSQ_JOIN_ACK::RET_ROOM;
                IfFailThenBreak(behaveState == PLAYER_BEHAVE_IDLE_STATE, RC::RC_ROOM_INVALID_PLAYER_STATE);
			};
			{	//	--JOIN--3)	Pet¼ÒÈ¯ »óÅÂ ÇØÁ¦
				//const BOOLEAN bSummoned = pPlayer->m_PetManager.AlreadySummonedPet();
				//<- ÇÃ·¹ÀÌ¾î·ÎºÎÅÍ Æê Á¤º¸¸¦ ¹Þ¾Æ¼­ Ã³¸®ÇÒ ¼ö ÀÖµµ·Ï ÇÒ °Í.
                CPetManager& pet_manager = pPlayer->m_PetManager;
                pet_manager.ReturnPet(TRUE);
			};
            {   // (CHANGES) (091208) (WAVERIX) Å» °Í Ãß°¡µÇ¸é¼­ ¼ÒÈ¯ ÇØÁ¦ Ã³¸®¸¦ ¼öÇàÇÑ´Ù.
                pPlayer->Rider.Command_SummonRelease(IVehicleEvent::EnterField());
            };
			{	//	--JOIN--4)	¼ÒÈ¯Ã¼ »óÅÂ(Summon Valkyrie)ÇØÁ¦
				//const DWORD summonerKey = pPlayer->GetSummonerKey();
				SummonManager* const pSummonManager = SummonManager::Instance();
				pSummonManager->LeaveSummonNPCs(pPlayer);
			};
			{
				StatusManager* const pStatusManager = pPlayer->GetStatusManager();
				pStatusManager->RemoveNonCashStatus();
			}

			{	//	Success
				MSG_AG_ZONE_SSQ_JOIN_ACK msgACK;
				msgACK.SetSuccessResult((MAPCODE)IDofSSQ);
				pPlayer->SendPacket(&msgACK, msgACK.GetSize());
			};
		}
		else //if(Control.IsCheckedBattleServer)
		{
			//	Çö ½ÃÁ¡, BattleServer¿¡´Â ÇØ´ç ÇÃ·¹ÀÌ¾î°¡ Á¸ÀçÇÏÁö ¾Ê´Â´Ù.
			SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(IDofSSQ);
			if(itFind == UpdateList.end())
				return;

			SSQ_UPDATER_NODE& rNode = *itFind;
			GameSSQUpdateCtrl& rCtrl = *rNode.GetUpdater();
			//	Agent¿¡ ÀÇÇØ Level, Chao, µîÀÌ °Ë»çµÇ¾úÀ½.

			const SSQInfo* const pSSQInfo = rCtrl.GetSSQInfo();

			{	//	Á¦¾à Á¶°Ç °Ë»ç
				//	ÀÎ¿ø °Ë»ç, '³ª(BattleServer) »ì¾Æ ÀÖ¾î' ÇÏ´Â ÀÀ´äÀÌ¶ó°í º¸¸é µÉ µí ÇÏ´Ù.
			};

			{	//	Success
				MSG_AG_ZONE_SSQ_JOIN_ACK msgACK;
				msgACK.SetSuccessResult((MAPCODE)IDofSSQ);
				pSession->SendPacket(&msgACK, msgACK.GetSize());
			};
		}

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

    if(pPlayer)
    {
        MSG_AG_ZONE_SSQ_JOIN_ACK msgFail;
        msgFail.SetFailResult(errorGroup, flow.GetErrorCode());
        pPlayer->SendPacket(&msgFail, msgFail.GetSize());
    }
    return;
}


//¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡


SSQ_PACKET_HANDLER_AG_DECL(AG_ZONE,		AG_ZONE_SSQ_ROOM_CTRL_CMD)
{
	WAVERIX_DEBUGGING_OUTPUTn(0);

	//	¼Ò¸êµÉ ¶§´Â Å°¸¦ ÇØÁ¦ÇØ¾ß ÇÑ´Ù. ÇØ´ç Æ÷ÀÎÅÍ¸¦ ÇÒ´çÇÒ±î? À½...
	const SSQCODE CodeOfSSQ	= (SSQCODE)pRecvMsg->m_dwKey;
	SSQ_UPDATERS_IT itFind	= _FindCodeOfSSQ(CodeOfSSQ);
	if(itFind == UpdateList.end())
		return;

	SSQ_UPDATER_NODE& rNode = *itFind;
	const KEYTYPE KEYofSSQ = rNode.GetKey();
	if(!KEYofSSQ)
		return;

	//AutoTransactionCtrlNode trans(&rNode);
	rNode.pUpdater->RecvedMsgProcess(pRecvMsg->m_Header);
}

