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
	// �÷��̾� �ø��� ���
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
// �����̵��̳� �ʵ��̵��� ���͸� �ٽ� �������� ����� �Լ�.
void
Player::CallSummonMonster()
{
#if !defined _NA_002962_20110630_CHANGE_SUMMONED_PORTAL_MOVE
    // �������϶��� �ٽ� �����Ѵ�.
    if (GetCharType() != eCHAR_SHADOW)
    {
        return;
    }
#endif

    //���� ��ȯ NPC ����Ʈ�� �ִ��� Ȯ���Ѵ�. ������ �����Ѵ�.
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
		
		//3. �ش� ���͸� ��ȯ�Ѵ�.
        if (GetCharType() == eCHAR_SHADOW)
        {
            Summoned* const summoned = GetField()->CreateSummonDeadMonster(
                summon_info->monster_code, &summon_pos, this, summoned_npcs, 0);
            if (summoned == NULL)
            {
                continue;
            }
                
            //������ HP�� ������ŭ ���δ�.
            summoned->SetHP(summon_info->hp);
#ifdef _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY
            // �ʵ� �̵� �� ������ �ҿ���Ʈ��(��ȯ��)�� ��ö��� ����..
            ALLOC_STATUS_PAIR steel = summoned->GetStatusManager()->AllocStatus(eCHAR_STATE_STEEL, 
                                                                           LOADING_STEEL_MODE_TIME);
            if (steel.second)
                steel.second->SendStatusAddBRD();
#endif //_NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY
#ifdef _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : AllocStatus(eCHAR_STATE_STEEL)");
#endif // _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG

            // ���� Create�Ҷ� ���� Ŭ������ HP������ ��������. ���Ŀ� HP�� �����ϴ� ����� ������ �����ش�.
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

        // �ѹ��� ���� ���¸� �ݿ��Ͽ� �����Ѵ�. ������ ���� ����� �޽����� �ι� ����.
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
        // �ʵ� �̵� �� ��Ű�� ��ȯ�� ��ö��� ����..
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
	// �÷��̾� �ø��� ����
	GetWorldBase()->x_pPathFinder->FindPath_Requst_DestroyAgent(GetPathExplorer());
#endif

	// field�� ������ Ʈ���ſ��� ���Դٰ� �˷��ش�.
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
            // 0�� �Է��ϸ� ���� ��ȯ�� ���͵��� ��� �ʵ忡�� ���ش�...
            // �׸���, �̵��ص� ����� ���� ��ȯ�� ������ ��������.
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
    _EnterEtcStatus();      // �������� �ο��ϴ� ���� �߰�.
	// �ڽ��� ��ġ ����
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
    //�ʵ�� ������ �ÿ� ��� ���� ������ �����ش�.
    MAPCODE field_code = pGameField->GetFieldCode();
    const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(field_code);
    if (pInfo != NULL)
    {
        const sDOMINATION_CONTINENT_LORD_INFO* lord_info = 
            GameDominationManager::Instance()->GetDominationContinentLordInfo(pInfo->ContienetNumber);
        //lord_info�� NULL�� ���� �ش� ����� ���ְ� ���ų� �����ȣ�� �߸��Ǿ��� ����̴�.
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

    // ���� ���� ó���� ��Ÿ ���� ������ ���� �� �� ��ȯ ������ �����ϵ��� �Ѵ�.
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

    // SD�� 0�� ����(�ű��ɸ���...) ���� Enter�� 
    // RECOVER�Ǵ� SD���� ��ȭ�� ���������� Ŭ�� �ִ°��� ���´�.  [8/20/2009 lst1024]
    if(0 == GetSD())
        SetSD(1);

    if(GetCharType() == eCHAR_BERSERKER)                  // ����Ŀ�� ������� HP, MP ���� �ֱⰡ �ٸ���.
    {
        m_pStatusManager->AllocStatus(eStateIDHP, iExpireTime, iHPPeriod); // HP ����
        m_pStatusManager->AllocStatus(eStateIDMP, iExpireTime, iMPPeriod); // MP ����
    }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    else if (GetCharType() == eCHAR_WITCHBLADE)
    {
        
        m_pStatusManager->AllocStatus(eStateIDHPMP, iExpireTime, iHPPeriod); // HP, MP ����
        ALLOC_STATUS_PAIR fp_status = m_pStatusManager->AllocStatus(state_id_FP, iExpireTime, FP_period);  // FP ����

        if (fp_status.first == true) {
            fp_status.second->SetFPMaintainTime(FP_maintain_time);
        }
    }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    else                                                    // ��Ű��, ����, �巡��
    {
        m_pStatusManager->AllocStatus(eStateIDHPMP, iExpireTime, iHPPeriod); // HP, MP ����
    }

    m_pStatusManager->AllocStatus(eStateIDSD, iExpireTime, iSDPeriod);    // SD ����


    eZONE_STATE zsSTATE = GetGameZoneState();
    BOOL bApplySheld = ePRS_AT_LOBBY != zsSTATE;

    if(bApplySheld)
        m_pStatusManager->AllocStatus(eCHAR_STATE_ETC_EXIST_SHELD_POINT);


    BOOL bApplySteelMode = TRUE;                // �ε��ð��� �����ֱ� ���Ͽ� ��ö���¸� �ɾ��ش�.
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

        // ���������� �ε� ��ö ���� �ǿ����� ����. 
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
// �ڽ��� ������ �ֺ����� �˸�
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

    // ���� ������ �ֺ��� �˸� �ʿ䰡 ����. ������ �÷��̾�� �ֱ������� �ֺ��� üũ�ؼ� ������ �����Ѵ�.
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
    // ��� ������ ���� ������.(���� ���� ���Ե�)

    const DWORD object_key = GetObjectKey();
    // ���� ���� ����
    {
        EQUIP_ITEM_BASEINFO* pEquipMsg = (EQUIP_ITEM_BASEINFO*)pVarMsgEnterObj->NextPtr();
        pEquipMsg->m_dwObjectKey = object_key;

        EQUIP_ITEM_BASEINFO::data_type* pItemSlot = 
            (EQUIP_ITEM_BASEINFO::data_type*)((BYTE*)pEquipMsg + sizeof(EQUIP_ITEM_BASEINFO));
        pEquipMsg->m_Count = this->SerializeRenderEquipItemBaseInfo(pItemSlot);
        pVarMsgEnterObj->SetSize(pEquipMsg->GetSize());
    }
    // ������ ����Ʈ ������ ������ �����Ѵ�.
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

    // �ڽ��� �̵������� ������.
    this->SendMoveInfo(NULL, rSector);
}

// �ֺ������� �ڽſ��� �˸�
void
Player::OnEnterNeighbor(const Sector& IN rSector)
{
	// �ֺ������� �ٷ� ���� �ʿ䰡 ����. ������ �ֱ������� �ֺ��� üũ�ؼ� ������ �����Ѵ�.
	{
		OnSendAllPlayersRenderInfoAllKinds(rSector);
        // added by __NA_0_20090720_GUILD_RENDER_INFO_MODIFY
        OnSendAllPlayersGuildRenderInfo(rSector);

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        OnSendAllPlayersSunRankingGradeInfo(rSector);
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
	}

	// �ֺ� ���� ����
	OnSendAllMonsterInfo(rSector);

	// �ֺ� ������ ����
	OnSendAllFieldItemInfo(rSector);
	// �ֺ� ä���� ����
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

// �ڽ��� ������ �ֺ�����
void
Player::OnLeaveObject(const Sector& IN rSector)
{
	StatusManager* const pStatusManager = GetStatusManager();
	StatusField& rStatus = pStatusManager->Status;
	const BOOLEAN bSendPacket = !(rStatus.FIELD & rStatus.ENABLE_OBSERVER_MODE);
	// ���� ������ �ֺ��� �˸� �ʿ䰡 ����. ������ �÷��̾�� �ֱ������� �ֺ��� üũ�ؼ� ������ �����Ѵ�.

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

