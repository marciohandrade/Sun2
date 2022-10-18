#include "stdafx.h"
#include "./Player.h"

//#include "ServerOptionParser.h"
#include <ServerOptionParserEx.h>
#include <SkillAttrCalculator.h>

#include "ObjectDefine.h"
#include "GameZone.h"
#include "GameField.h"
#include "Sector.h"

#include "Item.h"

#include "TriggerManagerEx.h"
#include "StatusManager.h"
#include "ChaoState.h"
#include "DragonTransStatus.h"
#include "MovementChecker.h"
#include "View.h"
#include "Summoned.h"
#include "SummonManager.h"
#include "SummonedNPCs.h"
#include "CollectedList.h"
//#include "PacketStruct_DG.h"
//#include "PacketStruct_AG.h"

VarMsg2
Player::m_VarMsg2[Player::SENDINFO_MAX_BUFFER_SIZE];


#include "Player_Grid_Operator.h"

#include "GameDominationManager.h"
#include "DominationInfoParser.h"

#include "SkillSystem/PerkManager.h"

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
#include "MapInfoParser.h"
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
#ifdef _NA_008166_20150330_MODIFY_AC_LEVELUP
#include "AchievementManager.h"
#endif //_NA_008166_20150330_MODIFY_AC_LEVELUP

//==================================================================================================
//
//==================================================================================================
//
// for GameField
void
Player::OnEnterField(GameField& IN rField, const WzVector& IN rwzVec, int iAngle)
{
	Character::OnEnterField(rField, rwzVec, iAngle);

	SetState(PLAYER_MAP_ENTER);	

#ifdef _DEV_TAIYO_PATHENGINE_CODE
	// 플레이어 컬리전 등록
	GetWorldBase()->x_pPathFinder->FindPath_Requst_NewAgent(GetPathExplorer());
#endif

    Rider.SyncUpdateRidingStatus(this);
    // CHANGES: f110727.2L, fixed a missing stopped timer resume event
    this->perk_manager_->OnEnterField(&rField);

    ;{
        MSG_CG_STATUS_ATTR_CHANGE_BRD sendMsg;
        sendMsg.m_dwObjKey = GetObjectKey();
        sendMsg.m_AttrType = (BYTE)eATTR_ATTACK_SPEED;
        sendMsg.m_dwValue = GetAttSpeedRatio();
        SendPacket(&sendMsg, sizeof(sendMsg));
    };
    ;{
        MSG_CG_STATUS_ATTR_CHANGE_BRD sendMsg;
        sendMsg.m_dwObjKey = GetObjectKey();
        sendMsg.m_AttrType = (BYTE)eATTR_MOVE_SPEED;
        sendMsg.m_dwValue = GetMoveSpeedRatio();
        SendPacket(&sendMsg, sizeof(sendMsg));
    };

    GetExtraInfoManager().OnEnterField();

#ifdef _NA_008166_20150330_MODIFY_AC_LEVELUP
    AchievementManager* ac_manager = this->GetACManager();
    if (ac_manager != NULL)
    {
        if (ac_manager->GetFlag_InitializeLevelUpAC() == FALSE) {
            this->GetACManager()->ProcessLevelUp(this->GetLevel());
        }
    }
#endif //_NA_008166_20150330_MODIFY_AC_LEVELUP
}

// __NA_S00015_20080828_NEW_HOPE_SHADOW
// 포털이동이나 필드이동시 몬스터를 다시 보여지게 만드는 함수.
void
Player::CallSummonMonster()
{
#if !defined _NA_002962_20110630_CHANGE_SUMMONED_PORTAL_MOVE
    // 쉐도우일때만 다시 생성한다.
    if (GetCharType() != eCHAR_SHADOW)
    {
        return;
    }
#endif

    //현재 소환 NPC 리스트가 있는지 확인한다. 없으면 생성한다.
	SummonedNPCs* summoned_npcs = SummonManager::Instance()->FindSummonNPCs(GetObjectKey());
	if (summoned_npcs == NULL)
	{
		summoned_npcs = SummonManager::Instance()->AllocSummonNPCs(this, TRUE);
		if (summoned_npcs == NULL)
        {
            return;
        }
    }

#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
	SetSummonerKey(summoned_npcs->GetSummonerKey());
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

	WzVector summon_pos;
	GetPos(&summon_pos);

    PLAYER_SUMMON_INFO_LIST::const_iterator it = m_listPlayerSummonInfo.begin();
	for (; it != m_listPlayerSummonInfo.end(); ++it)
	{
		const PLAYER_SUMMON_INFO* const summon_info = *it;
        if (summon_info == NULL)
        {
            continue;
        }
		
		//3. 해당 몬스터를 소환한다.
        if (GetCharType() == eCHAR_SHADOW)
        {
            Summoned* const summoned = GetField()->CreateSummonDeadMonster(
                summon_info->monster_code, &summon_pos, this, summoned_npcs, 0);
            if (summoned == NULL)
            {
                continue;
            }
                
            //몬스터의 HP를 비율만큼 줄인다.
            summoned->SetHP(summon_info->hp);
#ifdef _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY
            // 필드 이동 시 섀도우 소울컨트롤(소환수)도 강철모드 적용..
            ALLOC_STATUS_PAIR steel = summoned->GetStatusManager()->AllocStatus(eCHAR_STATE_STEEL, 
                                                                           LOADING_STEEL_MODE_TIME);
            if (steel.second)
                steel.second->SendStatusAddBRD();
#endif //_NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY
#ifdef _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : AllocStatus(eCHAR_STATE_STEEL)");
#endif // _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG

            // 위에 Create할때 벌써 클라한테 HP정보가 보내진다. 그후에 HP를 변경하니 변경된 정보를 보내준다.
            MSG_CG_SUMMON_CURRENT_HP_BRD brdMsg;
            brdMsg.m_dwSummonedObjKey = summoned->GetObjectKey();
            brdMsg.m_dwMaxHP = summoned->GetMaxHP();
            brdMsg.m_dwHP = summoned->GetHP();
            brdMsg.m_dwSpeedRatio = summoned->GetMoveSpeedRatio();
            summoned->SendPacketAround(&brdMsg, sizeof(MSG_CG_SUMMON_CURRENT_HP_BRD));
            
            continue;
        }
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        //
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

        // 한번에 이전 상태를 반영하여 생성한다. 쉐도우 같은 방식은 메시지가 두번 간다.
        Summoned* const summoned = GetField()->CreateSummonByPlayer(
            summon_info->monster_code, 
            &summon_pos, 
            this, 
            summoned_npcs, 
            summon_info->remained_time, 
            summon_info->skill_code, 
            summon_info);
#ifdef _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY
        if (summoned == NULL) 
        {
            continue;
        }
        // 필드 이동 시 발키리 소환수 강철모드 적용..
        ALLOC_STATUS_PAIR steel = summoned->GetStatusManager()->AllocStatus(eCHAR_STATE_STEEL, 
                                                                            LOADING_STEEL_MODE_TIME);
        if (steel.second)
            steel.second->SendStatusAddBRD();
#endif //_NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY
#ifdef _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : AllocStatus(eCHAR_STATE_STEEL)");
#endif // _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG
	}
}

// __NA_S00015_20080828_NEW_HOPE_SHADOW
void
Player::InsertSummonInfo(PLAYER_SUMMON_INFO* pInfo)
{
	if(!pInfo)		return;
	
	m_listPlayerSummonInfo.push_back(pInfo);
}
// __NA_S00015_20080828_NEW_HOPE_SHADOW
void
Player::DestroySummonInfo()
{
	PLAYER_SUMMON_INFO_LIST_ITER iter;
	for(iter = m_listPlayerSummonInfo.begin(); iter != m_listPlayerSummonInfo.end(); ++iter)
	{
		delete *iter;
	}

	m_listPlayerSummonInfo.clear();
}

//--------------------------------------------------------------------------------------------------
// __NA_S00015_20080828_NEW_HOPE_SHADOW
class GetSummonInfo
{
public:
    GetSummonInfo(Player* pPlayer):player_(pPlayer) {}
    ~GetSummonInfo() {}

public:
    void operator()(const SUMMONED_MAP_PAIR& value)
    {
        if (player_ == NULL)
        {
            return;
        }
        
        NPC* const summoned_npc = static_cast<NPC*>(value.second);
        if (obj_util::IsValid(summoned_npc, __FUNCTION__, __LINE__) == false) {
            return;
        }

        PLAYER_SUMMON_INFO* summon_info = new PLAYER_SUMMON_INFO;
        summon_info->monster_code = summoned_npc->GetBaseInfo()->m_MonsterCode;
        summon_info->hp = summoned_npc->GetHP();

        const Summoned* const summoned = static_cast<const Summoned* const>(summoned_npc);
        if (player_->GetCharType() != eCHAR_SHADOW)
        {
            summon_info->mp = summoned->GetMP();
            summon_info->remained_time = summoned->GetRemainedTime();
            summon_info->command = summoned->GetCommandState();
            summon_info->skill_code = summoned->GetSummonSkillCode();
        }
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        summon_info->summoned_type = summoned->GetSummonType();
        if (summon_info->summoned_type != eSUMMON_IMMOVABLE) {
            player_->InsertSummonInfo(summon_info);
        }
#else
        player_->InsertSummonInfo(summon_info);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    }
private:
    Player* player_;
};

//--------------------------------------------------------------------------------------------------

void
Player::OnLeaveField()
{
    GameField* pGameField = GetField();
	if(pGameField == 0)
        return;

    GetExtraInfoManager().OnLeaveField();

#ifdef _DEV_TAIYO_PATHENGINE_CODE
	// 플레이어 컬리전 해제
	GetWorldBase()->x_pPathFinder->FindPath_Requst_DestroyAgent(GetPathExplorer());
#endif

	// field를 떠나면 트리거에서 나왔다고 알려준다.
    // (CHANGES) (f100614.4L) add a slot type for a trigger area linker to support a multi-area link
    trigger_links_.Disconnect(pGameField->GetTriggerManager());
    //pGameField->GetTriggerManager()->Disconnected(&m_TriggerLink);
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    if (nsSync::CSSyncTuner* manager = m_pMovementChecker->GetSyncTuner()) {
        manager->Initialize(this);
    }
#endif
    // __NA_S00015_20080828_NEW_HOPE_SHADOW
	DestroySummonInfo();

#if !defined _NA_002962_20110630_CHANGE_SUMMONED_PORTAL_MOVE
    if (GetCharType() == eCHAR_SHADOW)
#endif
    {
        SummonedNPCs* pSummonNPCs = SummonManager::Instance()->FindSummonNPCs(GetObjectKey());
        if(pSummonNPCs)
        {
            // 0을 입력하면 현재 소환된 몬스터들을 모두 필드에서 없앤다...
            // 그리고, 이동해도 기억할 만한 소환수 정보를 저장하자.
            GetSummonInfo SummonInfo(this);
            pSummonNPCs->ForeachSummonedList(SummonInfo);
        }
    }
	// TODO: check sequence... _NA001385_20090924_DOMINATION_ETC
    _LeaveEtcStatus();

	Character::OnLeaveField();
}


void
Player::OnEnterSector()
{
    //__NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
    _EnterEtcStatus();      // 서버에서 부여하는 상태 추가.
	// 자신의 위치 정보
	MSG_AG_SYNC_PLAYER_ENTER_ACK msgACK;

	GetPos((WzVector*)msgACK.m_fPos);
	msgACK.m_TransSkillCode = 0;

	StatusManager* pSTATMGR = GetStatusManager();
	if(pSTATMGR->Status.IsDragonTransforming())
	{
		DragonTransStatus* pDragonTransStatus = (DragonTransStatus*)pSTATMGR->FindStatus(eCHAR_STATE_TRANSFORMATION);
		msgACK.m_TransSkillCode = pDragonTransStatus->GetSkillCode();
	};

    //_KR_001385_20091103_DOMINATION_ZONEMOVE_NOTIC
    GameField* pGameField = GetField();
    if (pGameField == NULL) {
        return;
    };
    msgACK.continued_wings_spreading_ = false;
#ifdef _NA001955_110210_WING_COSTUME
    msgACK.continued_wings_spreading_ = pSTATMGR->Status.IsSpreadWings();
#endif
    MAPCODE mapCode = pGameField->GetFieldCode();

    ZeroMemory(&msgACK.m_GuildInfo, sizeof(msgACK.m_GuildInfo));
    mapCode = DominationInfoParser::Instance()->GetDominationFieldCode( mapCode );
    if (mapCode != 0)
    {
        GameDominationManager::Instance()->GetInGuildInfo(mapCode, msgACK.m_GuildInfo);
    }

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    //필드로 진입할 시에 대륙 영주 정보도 보내준다.
    MAPCODE field_code = pGameField->GetFieldCode();
    const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(field_code);
    if (pInfo != NULL)
    {
        const sDOMINATION_CONTINENT_LORD_INFO* lord_info = 
            GameDominationManager::Instance()->GetDominationContinentLordInfo(pInfo->ContienetNumber);
        //lord_info가 NULL일 경우는 해당 대륙은 영주가 없거나 대륙번호가 잘못되었을 경우이다.
        if (lord_info != NULL)
        {
            msgACK.m_ContinentLordInfo = (*lord_info);
        }
    }
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

	SendPacket(&msgACK, sizeof(msgACK)); 

	MSG_CG_STATUS_ALL_STATEINFO_CMD msgCMD;

    pSTATMGR->SerialzieStatusInfo(msgCMD.m_StateTotalInfo);
	if(msgCMD.m_StateTotalInfo.m_Count)
	{
		SendPacket(&msgCMD, msgCMD.GetSize());
	}
    //{__NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
    MSG_CG_STATUS_ETC_STATEINFO_CMD msgEtcCMD;
    BYTE cntEtcState = pSTATMGR->SerialzieEtcStatusInfo(msgEtcCMD.m_stEtcStateTotalInfo);
    if(cntEtcState)
        SendPacket(&msgEtcCMD, msgEtcCMD.GetSize());
    //}
	ApplyGMState();

#ifdef __NA_821_STAT_BUG_RECOVERY
	if(!IsGMUser() && !GetCheckedStat())
	//if(!GetCheckedStat())
	{
		if(!CheckValidStatPoint())
		{
			RecoveryStatPoint();
		}
	}
#endif

	UnexpectedUserEnterRoom() = FALSE;

	GetChaoState().SendLeavedChaoTime();

    ;{
        m_pMovementChecker->Reset();
    #ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
        if (nsSync::CSSyncTuner* manager = m_pMovementChecker->GetSyncTuner())
        {
            manager->Initialize(this);
            MSG_CG_MAP_KBMOVE_SERIAL_CHECK_CMD msg_cmd;
            manager->EnterField(&msg_cmd);
            this->SendPacket(&msg_cmd, sizeof(msg_cmd));
        }
    #endif
    };

    // 섹터 진입 처리와 기타 상태 설정이 끝난 후 펫 소환 정보를 설정하도록 한다.
    m_PetManager.SetSummonInfo();
    //_NA_004035_20120227_COSTUME_ITEM
    costume_handler_.SetCostumeInfo();

	GAMELOG->LogAction(ACT_JOIN_MAP, this);
}

//{__NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
void
Player::_EnterEtcStatus()
{
    SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();

    const INT iExpireTime = BASE_EXPIRE_TIME_INFINITY;
    const int iHPPeriod = stServerOptionInfo.m_wPLAYER_HP_REGEN_PERIOD;
    const int iMPPeriod = stServerOptionInfo.m_wPLAYER_MP_REGEN_PERIOD;
    const int iSDPeriod = stServerOptionInfo.m_wPLAYER_SD_REGEN_PERIOD;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    const int FP_period = stServerOptionInfo.player_fp_regen_period;
    const int FP_maintain_time = stServerOptionInfo.player_fp_maintain_time;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    const eCHAR_STATE_TYPE eStateIDHP = eCHAR_STATE_ETC_AUTO_RECOVER_HP;
    const eCHAR_STATE_TYPE eStateIDMP = eCHAR_STATE_ETC_AUTO_RECOVER_MP;
    const eCHAR_STATE_TYPE eStateIDSD = eCHAR_STATE_ETC_AUTO_RECOVER_SD;
    const eCHAR_STATE_TYPE eStateIDHPMP = eCHAR_STATE_ETC_AUTO_RECOVER_HPMP;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    const eCHAR_STATE_TYPE state_id_FP = eCHAR_STATE_ETC_AUTO_RECOVER_FP;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    // SD가 0인 상태(신규케릭등...) 에서 Enter시 
    // RECOVER되는 SD값의 변화로 상태정보를 클라에 주는것을 막는다.  [8/20/2009 lst1024]
    if(0 == GetSD())
        SetSD(1);

    if(GetCharType() == eCHAR_BERSERKER)                  // 버서커와 섀도우는 HP, MP 리젠 주기가 다르다.
    {
        m_pStatusManager->AllocStatus(eStateIDHP, iExpireTime, iHPPeriod); // HP 리젠
        m_pStatusManager->AllocStatus(eStateIDMP, iExpireTime, iMPPeriod); // MP 리젠
    }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    else if (GetCharType() == eCHAR_WITCHBLADE)
    {
        
        m_pStatusManager->AllocStatus(eStateIDHPMP, iExpireTime, iHPPeriod); // HP, MP 리젠
        ALLOC_STATUS_PAIR fp_status = m_pStatusManager->AllocStatus(state_id_FP, iExpireTime, FP_period);  // FP 리젠

        if (fp_status.first == true) {
            fp_status.second->SetFPMaintainTime(FP_maintain_time);
        }
    }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    else                                                    // 발키리, 엘리, 드래곤
    {
        m_pStatusManager->AllocStatus(eStateIDHPMP, iExpireTime, iHPPeriod); // HP, MP 리젠
    }

    m_pStatusManager->AllocStatus(eStateIDSD, iExpireTime, iSDPeriod);    // SD 리젠


    eZONE_STATE zsSTATE = GetGameZoneState();
    BOOL bApplySheld = ePRS_AT_LOBBY != zsSTATE;

    if(bApplySheld)
        m_pStatusManager->AllocStatus(eCHAR_STATE_ETC_EXIST_SHELD_POINT);


    BOOL bApplySteelMode = TRUE;                // 로딩시간을 벌어주기 위하여 강철상태를 걸어준다.
    switch(zsSTATE)
    {
    case ePRS_AT_INSTANCE:
    case ePRS_AT_MISSION:
    case ePRS_AT_VILLAGE:
    case ePRS_AT_LOBBY:
        bApplySteelMode = UnexpectedUserEnterRoom();
        break;
    }

    if(bApplySteelMode)
    {
        int steel_time = LOADING_STEEL_MODE_TIME;  

        // 점령전에서 로딩 강철 상태 악용으로 변경. 
        if (zsSTATE == ePRS_AT_DOMINATION_FIELD) 
            steel_time = DOMINATION_LOADING_STEEL_MODE_TIME; 

        m_pStatusManager->AllocStatus(eCHAR_STATE_STEEL, steel_time);
#ifdef _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : AllocStatus(eCHAR_STATE_STEEL)");
#endif // _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG
    }

    if (INVALID_GUILDGUID != GetGuildGuid())
        GameDominationManager::Instance()->ApplyRewardField(this, GetField()->GetFieldCode());
}
//}__NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE

void
Player::_LeaveEtcStatus()
{
    if (INVALID_GUILDGUID != GetGuildGuid())
        GameDominationManager::Instance()->ApplyRewardField(this, GetField()->GetFieldCode(),false);
}

// for Sector
// 자신의 정보를 주변에게 알림
void
Player::OnEnterObject(const Sector& IN rSector)
{
    GameZone* pGameZone = GetGameZonePtr();
    if(pGameZone == 0)
        return;
    // CHANGES: f110415.6L, at this point in time, the usage case of 'OnEnterObject' only has
    // notification to users so that if the sector has no users,
    // it is no need to process contiuously.
    // in this case, that player is related to other player, skip check this player
    // because it's complex that finding a player whether this player is located in this sector
    if (rSector.GetPlayerNum() == 0) {
        return;
    };

    GameField* const pGameField = GetField();
    if (pGameField == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][SectorBug] Invalid GameField![PlayerGuid:%d][ZoneMapCode:%d][EntryFieldCode:%d]", 
               this->GetCharGuid(), pGameZone->GetMapCode(), pGameZone->GetEntryFieldCode());
        return;
    }

    StatusManager* const pStatusManager = GetStatusManager();
    StatusField& rStatus = pStatusManager->Status;
    const BOOLEAN bNeedNtf = !(rStatus.FIELD & rStatus.ENABLE_OBSERVER_MODE);
    if(!bNeedNtf)
        return;

    // 나의 정보를 주변에 알릴 필요가 없다. 각각의 플레이어마다 주기적으로 주변을 체크해서 정보를 갱신한다.
    VarMsg2* pVarMsgEnterObj = &(m_VarMsg2[Player::SENDINFO_ENTER_OBJECT]);

    MSG_CG_SYNC_PLAYER_ENTER_BRD* pEnterBrdMsg = \
        pVarMsgEnterObj->StartTypePtr(Param2Class<MSG_CG_SYNC_PLAYER_ENTER_BRD>());
    pVarMsgEnterObj->SetSize(pEnterBrdMsg->GetSize());
    {
        PLAYER_RENDER_INFO_EX* pack_info = (PLAYER_RENDER_INFO_EX*)pVarMsgEnterObj->NextPtr();
        int packed_len = this->GetPlayerRenderInfo(pGameZone->GetZoneType() != eZONETYPE_VILLAGE, pack_info);
        pVarMsgEnterObj->SetSize((WORD)packed_len);
    };

    //{__NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
    {
        ETC_STATE_TOTAL_INFO *pStateRenderMsg = (ETC_STATE_TOTAL_INFO*)pVarMsgEnterObj->NextPtr();
        this->GetEtcStateTotalInfo(pStateRenderMsg);
        pVarMsgEnterObj->SetSize(pStateRenderMsg->GetSize());
    }
    //}
    //__NA_0_20090720_GUILD_RENDER_INFO_MODIFY
    // 길드 정보는 따로 보낸다.(대인 관계 포함됨)

    const DWORD object_key = GetObjectKey();
    // 장착 정보 셋팅
    {
        EQUIP_ITEM_BASEINFO* pEquipMsg = (EQUIP_ITEM_BASEINFO*)pVarMsgEnterObj->NextPtr();
        pEquipMsg->m_dwObjectKey = object_key;

        EQUIP_ITEM_BASEINFO::data_type* pItemSlot = 
            (EQUIP_ITEM_BASEINFO::data_type*)((BYTE*)pEquipMsg + sizeof(EQUIP_ITEM_BASEINFO));
        pEquipMsg->m_Count = this->SerializeRenderEquipItemBaseInfo(pItemSlot);
        pVarMsgEnterObj->SetSize(pEquipMsg->GetSize());
    }
    // 아이템 이펙트 랜더링 정보를 설정한다.
    {
        sMSG_RENDER_EFFECT_ITEM_LIST* const render_effect = 
            (sMSG_RENDER_EFFECT_ITEM_LIST*)pVarMsgEnterObj->NextPtr();
        render_effect->m_PlayerKey = object_key;
        const DWORD render_item_count = RENDER_EFFECT.SerializeRenderInfo(
                render_effect->m_BitRender, 
                render_effect->m_ItemCodes, 
                sMSG_RENDER_EFFECT_ITEM_LIST::MAX_EFFECT_ITEMS);
        render_effect->m_ItemCount = static_cast<BYTE>(render_item_count);
        pVarMsgEnterObj->SetSize(render_effect->GetSize());
    };

    pGameField->SendExPacketToSector(rSector.GetSectorIndex(),
                                     pVarMsgEnterObj->GetMsgCount(),
                                     pVarMsgEnterObj->GetMsgPPtr(),
                                     pVarMsgEnterObj->GetMsgSizePtr(),
                                     object_key);

    // added by __NA_0_20090720_GUILD_RENDER_INFO_MODIFY
    OnSendAllPlayersGuildRenderInfoToNeighbor(rSector);

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    OnSendAllPlayersSunRankingGradeInfoToNeighbor(rSector);
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

    // 자신의 이동정보를 보낸다.
    this->SendMoveInfo(NULL, rSector);
}

// 주변정보를 자신에게 알림
void
Player::OnEnterNeighbor(const Sector& IN rSector)
{
	// 주변정보를 바로 받을 필요가 없다. 어차피 주기적으로 주변을 체크해서 정보를 갱신한다.
	{
		OnSendAllPlayersRenderInfoAllKinds(rSector);
        // added by __NA_0_20090720_GUILD_RENDER_INFO_MODIFY
        OnSendAllPlayersGuildRenderInfo(rSector);

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        OnSendAllPlayersSunRankingGradeInfo(rSector);
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
	}

	// 주변 몬스터 정보
	OnSendAllMonsterInfo(rSector);

	// 주변 아이템 정보
	OnSendAllFieldItemInfo(rSector);
	// 주변 채집물 정보
	OnSendAllFieldCollectionInfo(rSector);
}

void
Player::FirstGuildMemberEnter() 
{
    GameField* pGameField = GetField();
    if(NULL == pGameField)
        return;

    pGameField->FirstGuildMemberEnter(this);
}

// 자신의 정보를 주변에게
void
Player::OnLeaveObject(const Sector& IN rSector)
{
	StatusManager* const pStatusManager = GetStatusManager();
	StatusField& rStatus = pStatusManager->Status;
	const BOOLEAN bSendPacket = !(rStatus.FIELD & rStatus.ENABLE_OBSERVER_MODE);
	// 나의 정보를 주변에 알릴 필요가 없다. 각각의 플레이어마다 주기적으로 주변을 체크해서 정보를 갱신한다.

	if(bSendPacket)
	{
		MSG_CG_SYNC_PLAYER_LEAVE_BRD brdMsg;
		brdMsg.m_byCount = 1;

		const DWORD object_key = this->GetObjectKey();
		brdMsg.m_PlayerKey[0] = (WORD)object_key;

		GameField* const pGameField = GetField();
		pGameField->SendPacketToSector(rSector.GetSectorIndex(), &brdMsg, brdMsg.GetSize(), object_key);
	}
}

//==================================================================================================
//	<Spec.>
//

