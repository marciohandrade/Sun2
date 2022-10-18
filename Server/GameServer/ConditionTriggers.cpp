#include "stdafx.h"
#include <PublicMath.h>

#include "./ConditionTriggers.h"

#include "./TriggerManagerEx.h"
#include "../../SolarTrigger/TriggerMsg.h"

//==================================================================================================

#include "Trigger.h"
#include "Player.h"
#include "ItemManager.h"
#include "MissionManager.h"

#include "GameZone.h"
#include "GameMissionRoom.h"
#include "Map.h"
#include "MapObject.h"


//==================================================================================================
//  CONDITION_ALWAYS
__IMPL_POOL(CONDITION_ALWAYS)
VOID
CONDITION_ALWAYS::OnInit()
{
    setSatisfied(TRUE);
}

//==================================================================================================
__IMPL_POOL(CONDITION_COMPARE_SWITCH)
BOOL
CONDITION_COMPARE_SWITCH::IsSatisfied()
{
    if(satisfied_)
        return TRUE;

    CONDITION_COMPARE_SWITCH_INFO& rINFO = *Get();
    const BYTE idx = rINFO.GetSwitchID();
    const BYTE curr_value = getMGR()->GetSwitchValue(idx);
    const BYTE cond_value = rINFO.GetValue();

    BOOL bSatisfied = FALSE;
    switch(rINFO.GetOperationType())
    {
    case eCOMPARE_EQUAL:    bSatisfied = curr_value == cond_value;   break;
    case eCOMPARE_MORE:     bSatisfied = curr_value >  cond_value;   break;
    case eCOMPARE_LESS:     bSatisfied = curr_value <  cond_value;   break;
    case eCOMPARE_LESSTHAN: bSatisfied = !(curr_value > cond_value); break;
    case eCOMPARE_MORETHAN: bSatisfied = !(curr_value < cond_value); break;
    case eCOMPARE_NOTEQUAL: bSatisfied = curr_value != cond_value;   break;
        // break;
    default:
        __debugbreak();
        break;
    }

    if(bSatisfied)
        setSatisfied(bSatisfied);

    return satisfied_;
}

//==================================================================================================
__IMPL_POOL(CONDITION_COMPARE_SWITCH_EX)
BOOL
CONDITION_COMPARE_SWITCH_EX::IsSatisfied()
{
	if(satisfied_)
		return TRUE;

	CONDITION_COMPARE_SWITCH_EX_INFO& rINFO = *Get();
	const BYTE idx = rINFO.GetSwitchID();
	BYTE curr_value = 0;

	if (rINFO.GetTarget() == ns_trigger::ZONE)
	{
#ifdef _NA_007369_20140625_ACTION_OPERATE_SWITCH_EX
		GameZone* const game_zone = getMGR()->GetField()->GetGameZone();
		if(game_zone->GetZoneType() == eZONETYPE_MISSION)
		{
			GameMissionRoom* mission_room = static_cast<GameMissionRoom*>(game_zone);
			curr_value = static_cast<BYTE>(mission_room->GetSwitchValue(rINFO.GetSwitchID()));
		}
#endif //_NA_007369_20140625_ACTION_OPERATE_SWITCH_EX
	}
	else
		return FALSE;

	const BYTE cond_value = rINFO.GetValue();

	BOOL bSatisfied = FALSE;
	switch(rINFO.GetOperationType())
	{
	case eCOMPARE_EQUAL:    bSatisfied = curr_value == cond_value;   break;
	case eCOMPARE_MORE:     bSatisfied = curr_value >  cond_value;   break;
	case eCOMPARE_LESS:     bSatisfied = curr_value <  cond_value;   break;
	case eCOMPARE_LESSTHAN: bSatisfied = !(curr_value > cond_value); break;
	case eCOMPARE_MORETHAN: bSatisfied = !(curr_value < cond_value); break;
	case eCOMPARE_NOTEQUAL: bSatisfied = curr_value != cond_value;   break;
		// break;
	default:
		__debugbreak();
		break;
	}

	if(bSatisfied)
		setSatisfied(bSatisfied);

	return satisfied_;
}

//==================================================================================================

__IMPL_POOL(CONDITION_CLICK_OBJECT)
VOID
CONDITION_CLICK_OBJECT::onMsg(TRIGGER_MSG* pmsg)
{
    switch(pmsg->m_dwMsgID)
    {
    case eTRIGGER_CLICK_OBJECT:
        {
            TRIGGER_CLICK_OBJECT* pMsg = (TRIGGER_CLICK_OBJECT *)pmsg;
            const CONDITION_CLICK_OBJECT_INFO* pInfo = Get();
            int object_id = pInfo->GetObjectID();
            ASSERT(object_id >= 0);
            if(pMsg->m_dwObjectKey != object_id)
                break;

            MAPOBJECT_INFO* map_object_info = getMGR()->FindMapObjectInfo(object_id);
            if (map_object_info == NULL)
                break;

            Player* player = PlayerManager::Instance()->FindPlayerByObjKey(pMsg->m_PlayerKey);
            if (player == NULL)
                break;

            float near_radius = DISTANCE_NPC_MEET + 1.5f;
            if(Math_VectorLength(*player->GetPosPtr(), map_object_info->wvPos) > near_radius)
                break;

            getParent()->SetClickedPlayerKey(pMsg->m_PlayerKey);
        #ifdef __TRIGGER_TEST_LOG
            SUNLOG(eFULL_LOG,
                    "[CONDITION_CLICK_OBJECT::onMsg] [TriggerID:%u]:CONDITION_CLICK_OBJECT",
                    getParent()->TriggerID());
        #endif
            setSatisfied(TRUE);

            player->Player_TransOff();

            MSG_CG_TRIGGER_CLICK_OBJECT_BRD msgBRD;
            msgBRD.m_dwClickedUserObjectKey = player->GetObjectKey();
            msgBRD.m_dwClickedObjectKey = pMsg->m_dwObjectKey;
            player->SendPacketAround(&msgBRD, sizeof(msgBRD));
        }
        break;
    }
}

VOID
CONDITION_CLICK_OBJECT::OnReset()
{
    getParent()->SetClickedPlayerKey(0);
    setSatisfied(FALSE);
}

//==================================================================================================

__IMPL_POOL(CONDITION_DAMAGE_OBJECT)
VOID
CONDITION_DAMAGE_OBJECT::onMsg(TRIGGER_MSG* pmsg)
{
    if(eTRIGGER_DAMAGE_OBJECT == pmsg->m_dwMsgID)
    {
        TRIGGER_DAMAGE_OBJECT* pMsg = (TRIGGER_DAMAGE_OBJECT *)pmsg;

        if(pMsg->m_dwObjectKey == Get()->GetObjectID())
        {
            //이 영역에 플레이어가 있는지 체크 필요...한가?
            MapObject* pMapObject = getMGR()->FindMapObject(pMsg->m_dwObjectKey);
            // 죽었으면???
            if(FlowControl::FCAssert(pMapObject))
            {
                setSatisfied(TRUE);
                return;
            }
            setSatisfied(FALSE);
        }
    }
}


//==================================================================================================

__IMPL_POOL(CONDITION_COUNT_NPCKILL)

VOID
CONDITION_COUNT_NPCKILL::OnInstanciate()
{
}
VOID
CONDITION_COUNT_NPCKILL::OnInit()
{
    ZeroMemory(m_PartyMember, sizeof(m_PartyMember));
}

VOID
CONDITION_COUNT_NPCKILL::onMsg(TRIGGER_MSG* pmsg)
{
    switch(pmsg->m_dwMsgID)
    {
    case eTRIGGER_KILL_MONSTER:
        {
            TRIGGER_KILL_MONSTER* pMsg = (TRIGGER_KILL_MONSTER *)pmsg;
            CONDITION_COUNT_NPCKILL_INFO* pInfo = Get();

            if(ns_trigger::PARTY == pInfo->GetIsParty())
            {
                //ASSERT(SAFE_NUMERIC_TYPECAST(int, pInfo->GetMonsterCode(), MONSTERCODE));

                PARTY_MEMBER* pMember = &(m_PartyMember[0]);
                MONSTERCODE monsterCode = pInfo->GetMonsterCode();

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
                GameZone* const game_zone = getMGR()->GetField()->GetGameZone();
                if(game_zone->GetZoneType() == eZONETYPE_MISSION)
                {
                    MONSTERCODE replaace_monster_code = \
                        AppliedNpcDependencyChain::Instance()->GetRepleaceMonsterCode(game_zone, monsterCode);

                    if (replaace_monster_code) {
                        monsterCode = replaace_monster_code;
                    }
                }
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

                if(pMsg->m_dwMonsterCode != monsterCode)
                {   // 대부분은 이 조건에 걸릴 것이다.
                    return;
                }
                else
                {   // else의 조건이다. 즉, 만족할 경우...
                    ++(pMember->m_iKillCount);
                }

                const int number_of_kills = pInfo->GetKillCount();
                if(number_of_kills > pMember->m_iKillCount)
                    return;

                //------------------------
                // 조건 만족
                // == or <
                setSatisfied(TRUE);
            #ifdef __TRIGGER_TEST_LOG
                SUNLOG(eFULL_LOG,
                       "["__FUNCTION__"] [ID:%u]:MONSTER:[%u], KILL CONDITION %u : KILL %u ",
                       getParent()->TriggerID(), pInfo->GetMonsterCode(),  \
                       number_of_kills, pMember->m_iKillCount);
            #endif
                //------------------------

                if(number_of_kills < pMember->m_iKillCount)
                {
                    // < 의 조건에 걸리는 경우가 있을까? 혹시 모른다.
                    // 있다면 찾도록 060601, WAVERIX
                    SUNLOG(eCRITICAL_LOG,
                           "["__FUNCTION__"] Over Count Killed! TriggerID [%u]: MonsterID [%u], "
                           "MAX [%u]<KillCount [%u]",
                           getParent()->TriggerID(), pInfo->GetMonsterCode(),
                           number_of_kills, pMember->m_iKillCount);
                }
            }
            else
            {
                ASSERT(!"현재 개인단위 NPC KILL은 허용하지 않는다.");
            }

        }
        break;
    }
}

//==================================================================================================
__IMPL_POOL(CONDITION_PASS_TIME)
VOID
CONDITION_PASS_TIME::OnInit()
{
    m_dwStartTick = GetTickCount();
}

BOOL
CONDITION_PASS_TIME::IsSatisfied()
{
    if(satisfied_)
        return TRUE;

    DWORD delay = (DWORD)Get()->GetPassTime();
    if((m_dwStartTick + delay) <= GetTickCount())
    {
        setSatisfied(TRUE);
    }

    return satisfied_;
}

VOID CONDITION_PASS_TIME::OnReset()
{
    setSatisfied(FALSE);
}

//==================================================================================================
// CHANGES: f110311.3L, AddSatisfiedPlayer processing replacement
#if SUN_CODE_BACKUP
class AddSatisfiedPlayer
{
private:
    Trigger* m_pTrigger;
    bool  m_bEnterPlayersMismatched;
    WORD m_NumberOfPlayers;
    PlayerManager* m_pPlayerMgr;

public:
    AddSatisfiedPlayer(Trigger* pTrigger)
        : m_pTrigger(pTrigger)
        , m_bEnterPlayersMismatched(false)
        , m_NumberOfPlayers(0)
    { m_pPlayerMgr = PlayerManager::Instance(); }

    inline bool IsEnterPlayersMismatched() const { return m_bEnterPlayersMismatched; }
    inline WORD GetNumberOfPlayers() const { return m_NumberOfPlayers; }
    bool ProcessCheckAndAdd(TriggerPlayerLink* link)
    {
        if(link->IsValidState() == 0)
        {
            m_bEnterPlayersMismatched = true;
            return false;
        }

        ns_trigger::TriggerPlayerLinkInGame* item =
            static_cast<ns_trigger::TriggerPlayerLinkInGame*>(link);

        if(FlowControl::FCAssert(item->player_ != 0))
            m_pTrigger->AddSatisfiedPlayer(item->player_);
        ++m_NumberOfPlayers;
        return true;
    }
};
#endif
//--------------------------------------------------------------------------------------------------

__IMPL_POOL(CONDITION_ENTER_AREA);

VOID
CONDITION_ENTER_AREA::OnInit()
{
    getParent()->RemoveAllSatisfiedPlayer();
}

VOID
CONDITION_ENTER_AREA::OnReset()
{
    getParent()->RemoveAllSatisfiedPlayer();
    setSatisfied(FALSE);
}

BOOL
CONDITION_ENTER_AREA::IsSatisfied()
{
    if(satisfied_)
        return TRUE;

    const CONDITION_ENTER_AREA_INFO& rInfo = *Get();
    int areaID = rInfo.GetAreaID();
    BOOL need_process = FALSE;

    TriggerManagerEx* pTriggerManager = getMGR();
    TriggerArea* pArea = pTriggerManager->GetAreaManager()->FindArea(areaID);
    if(pArea == 0)
        return FALSE;

    const DWORD number_of_entered = pArea->GetPlayerNumber();
    //pTriggerManager->GetPlayerNumForAreaID(areaID);

    if(rInfo.GetIsParty() == 0)
    {
        const DWORD number_of_players = (DWORD)rInfo.GetPlayerCount();
        bool is_accepted = number_of_players ? number_of_entered >= number_of_players
                                             : number_of_entered == number_of_players;
        if(is_accepted)
        {   // 조건에 만족한다. 플레이어의 유효성 체크. 유효하지 않는다면, 플레이어 제거
            need_process = TRUE;
        }
    }
    else
    {   // Party
        if(number_of_entered)  // 1명 이상으로 평가
            need_process = TRUE;
    }

    if (need_process == false) {
        return satisfied_;
    };
    //
    // CHANGES: f110311.3L, AddSatisfiedPlayer processing replacement
    class AdderIfExistSatisfiedConditionPlayer : public TriggerAreaLinkForeachFunctor
    {
    public:
        AdderIfExistSatisfiedConditionPlayer(
            const TriggerAreaManager* trigger_area_manager,
            Trigger* trigger,
            int area_id)
            : trigger_area_manager_(trigger_area_manager)
            , trigger_(trigger)
            , area_id_(area_id)
            , mismatched_enter_player_(false)
            , number_of_players_(0)
        {
        }
        bool Process()
        {
            TriggerArea* trigger_area = trigger_area_manager_->FindArea(area_id_);
            if (trigger_area == NULL) {
                return false;
            }
            trigger_area->Foreach(this);
            return true;
        }

        virtual TriggerAreaLinkForeachFunctor::eControl
        ProcessNode(const TriggerPlayerLink* trigger_player_link) const
        {
            if (trigger_player_link->IsValidState() == false)
            {
                mismatched_enter_player_ = true;
                return this->eStop;
            }

            const ns_trigger::TriggerPlayerLinkInGame* link_item =
                static_cast<const ns_trigger::TriggerPlayerLinkInGame*>(trigger_player_link);

            if (FlowControl::FCAssert(link_item->player_ != NULL)) {
                trigger_->AddSatisfiedPlayer(link_item->player_);
            };
            ++number_of_players_;
            //
            return this->eContinue;
        };
    //private:
        const TriggerAreaManager* const trigger_area_manager_;
        Trigger* const trigger_;
        const int area_id_;
        mutable bool mismatched_enter_player_;
        mutable int number_of_players_;
    } functor(getMGR()->GetAreaManager(), getParent(), areaID);

    if (functor.Process() == false) {
        return satisfied_;
    }

    if (functor.mismatched_enter_player_ == false)
    {   // success
        // NOTE: refactoring history. f090831.x, f110311.3L
        uint16_t new_updated = static_cast<uint16_t>(functor.number_of_players_);
        if (local_storage_.last_updated_number_of_chars_ < new_updated)
        {
            local_storage_.last_updated_number_of_chars_ = new_updated;
            functor.trigger_->NeedActionByRuntimeConfig();
        };
        setSatisfied(true);
    };
    //
    return satisfied_;
}

VOID
CONDITION_ENTER_AREA::onMsg(TRIGGER_MSG* pmsg)
{
    switch(pmsg->m_dwMsgID)
    {
    case eTRIGGER_ENTER_AREA:
        {
            TRIGGER_ENTER_AREA* pMsg = (TRIGGER_ENTER_AREA *)pmsg;
            if(Get()->GetAreaID() == pMsg->m_AreaID)
            {
                IsSatisfied();
            }
        }
        break;
    case eTRIGGER_LEAVE_AREA:
        {
            TRIGGER_LEAVE_AREA* pMsg = (TRIGGER_LEAVE_AREA *)pmsg;
            if(Get()->GetAreaID() == pMsg->m_AreaID)
            {
                IsSatisfied();
            }
        }
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER

__IMPL_POOL(CONDITION_NPC_ENTER_AREA);

void CONDITION_NPC_ENTER_AREA::OnInit()
{
    setSatisfied(false);
}

void CONDITION_NPC_ENTER_AREA::OnReset()
{
    setSatisfied(false);
}

BOOL CONDITION_NPC_ENTER_AREA::IsSatisfied()
{
    if (satisfied_) {
        return true;
    }
#ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
    const CONDITION_NPC_ENTER_AREA_INFO& trigger_info = *Get();
    const int area_id = trigger_info.GetAreaID();

    TriggerManagerEx* const trigger_manager = getMGR();
    TriggerAreaForNpc* const trigger_area =
        trigger_manager->GetAreaManagerForNpc()->FindArea(area_id);
    if (trigger_area == NULL) {
        return false;
    }
    const DWORD number_of_entered = trigger_area->GetNpcNumber();
    bool need_process = (number_of_entered != 0);
    if (need_process == false) {
        return satisfied_;
    };
    //
    // success
    // NOTE: reference from 'CONDITION_ENTER_AREA: f110311.3L'
    uint16_t new_updated = static_cast<uint16_t>(number_of_entered);
    if (local_storage_.last_updated_number_of_chars_ < new_updated)
    {
        local_storage_.last_updated_number_of_chars_ = new_updated;
        if (Trigger* trigger = getParent()) {
            trigger->NeedActionByRuntimeConfig();
        };
        setSatisfied(true);
    };
    //
#endif //_NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
    return satisfied_;
}

void CONDITION_NPC_ENTER_AREA::onMsg(TRIGGER_MSG* trigger_msg)
{
    // NOTE: f110318.2L, an area event message for npc is prefetched
    // by trigger initialization evnets. so that this method don't test the unit identification.
    const eTRIGGER_MSG msg_type = static_cast<eTRIGGER_MSG>(trigger_msg->m_dwMsgID);
    switch (msg_type)
    {
    case eTRIGGER_NPC_ENTER_AREA:
        {
            const TRIGGER_NPC_ENTER_AREA* const enter_msg = 
                static_cast<const TRIGGER_NPC_ENTER_AREA*>(trigger_msg);
            const CONDITION_NPC_ENTER_AREA_INFO& trigger_info = *Get();
            if (trigger_info.GetAreaID() == enter_msg->area_id) {
                this->IsSatisfied();
            }
        };
        break;
    case eTRIGGER_NPC_LEAVE_AREA:
        {
            const TRIGGER_NPC_LEAVE_AREA* const leave_msg = 
                static_cast<const TRIGGER_NPC_LEAVE_AREA*>(trigger_msg);
            const CONDITION_NPC_ENTER_AREA_INFO& trigger_info = *Get();
            if (trigger_info.GetAreaID() == leave_msg->area_id) {
                this->IsSatisfied();
            }
        };
        break;
    };
}

//==================================================================================================
class HaveItemPlayer
{
private:
    SLOTCODE    m_ItemCode;
    int         m_Count;
    BOOL       is_satisfied_;

    Trigger* parent_trigger_;

public:
    HaveItemPlayer(SLOTCODE itemCode, int count, Trigger* parent):
      is_satisfied_(FALSE), m_ItemCode(itemCode), m_Count(count), parent_trigger_(parent) {}
    ~HaveItemPlayer() {}

    BOOL Process(SATISFIED_PLAYER_PAIR rPair)
    {
        Player* pPlayer = rPair.second;
        ItemManager* pItemManager = pPlayer->GetItemManager();
        is_satisfied_ = (pItemManager->ExistItem(SI_INVENTORY, m_ItemCode, m_Count) == m_Count);

        if (is_satisfied_ == FALSE) {
            parent_trigger_->CheckUnsatisfiedPlayer(pPlayer->GetObjectKey());
        }
        return is_satisfied_;
    }
};


__IMPL_POOL(CONDITION_HAVE_ITEM)
VOID
CONDITION_HAVE_ITEM::OnInit()
{
    m_dwCheckTimer = GetTickCount();
}

BOOL
CONDITION_HAVE_ITEM::IsSatisfied()
{
    if(satisfied_)
        return TRUE;

    DWORD cur_tick = GetTickCount();
    if(m_dwCheckTimer + CONDITION_HAVE_ITEM::c_dwDelayCheckTime <= cur_tick)
    {
        m_dwCheckTimer = cur_tick;

        const CONDITION_HAVE_ITEM_INFO& rInfo = *Get();
        // 여러사람일 경우가 없지만 영역에 들어온 사람과 클릭한 사람을 저장
        DWORD clicked_player_key = getParent()->GetClickedPlayerKey();
        int number_of_items = rInfo.GetItemNum();
        int item_code = rInfo.GetItemCode();

        // 둘 중 하나
        if(clicked_player_key)
        {
            Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
            if(pPlayer)
            {
                BOOLEAN satisfied = number_of_items ==
                                    pPlayer->GetItemManager()->ExistItem(SI_INVENTORY,
                                                                         item_code,
                                                                         number_of_items);
                if(satisfied)
                {
                    setSatisfied(TRUE);
                    return TRUE;
                }
            }
        }
        else
        {
            //_NA_008069_20150210_ADD_SPA_SYSTEM
            HaveItemPlayer op(item_code, number_of_items, getParent());
            getParent()->ForeachSatisfiedPlayer(&HaveItemPlayer::Process, &op);

            setSatisfied(TRUE);
        }
    }

    return satisfied_;
}
//_NA_008069_20150210_ADD_SPA_SYSTEM
VOID CONDITION_HAVE_ITEM::OnReset()
{
    setSatisfied(false);
}

//==================================================================================================
__IMPL_POOL(CONDITION_MULTI_SWITCH)
BOOL
CONDITION_MULTI_SWITCH::IsSatisfied()
{
    if(satisfied_)
        return TRUE;

    CONDITION_MULTI_SWITCH_INFO& rInfo = *Get();
    TriggerManagerEx* pTriggerManager = getMGR();

    // (WAVERIX) (090621) (CHANGES) less_than, more_than 효율보다는 가독성 선택
    int satisfiedCount = 0;
    for(DWORD idx = 0; idx < rInfo._MAX_SWITCH_ID_NUM; ++idx)
    {
        BYTE switchID = rInfo.GetSwitchID((WORD)idx);
        if(INVALID_SWITCH_ID_VALUE == switchID)
            continue;

        const int curr_value = (int)pTriggerManager->GetSwitchValue(switchID);
        const int cond_value = rInfo.GetValue1();
        switch(rInfo.GetOperationType1())
        {
        case eCOMPARE_EQUAL:    if(curr_value == cond_value) ++satisfiedCount; break;
        case eCOMPARE_MORE:     if(curr_value >  cond_value) ++satisfiedCount; break;
        case eCOMPARE_LESS:     if(curr_value <  cond_value) ++satisfiedCount; break;
        case eCOMPARE_LESSTHAN: if(curr_value <= cond_value) ++satisfiedCount; break;
        case eCOMPARE_MORETHAN: if(curr_value >= cond_value) ++satisfiedCount; break;
        case eCOMPARE_NOTEQUAL: if(curr_value != cond_value) ++satisfiedCount; break;
        default: __debugbreak(); break;
        }
    }

    BOOL bSatisfied = FALSE;
    const int cond_count = rInfo.GetValue2();
    switch(rInfo.GetOperationType2())
    {
    case eCOMPARE_EQUAL:    bSatisfied = (satisfiedCount == cond_count); break;
    case eCOMPARE_MORE:     bSatisfied = (satisfiedCount >  cond_count); break;
    case eCOMPARE_LESS:     bSatisfied = (satisfiedCount <  cond_count); break;
    case eCOMPARE_LESSTHAN: bSatisfied = (satisfiedCount <= cond_count); break;
    case eCOMPARE_MORETHAN: bSatisfied = (satisfiedCount >= cond_count); break;
    case eCOMPARE_NOTEQUAL: bSatisfied = (satisfiedCount != cond_count); break;
    default: __debugbreak(); break;
    }

    if(bSatisfied)
        setSatisfied(bSatisfied);

    DEBUG_CODE(
        if(satisfied_)
            SUNLOG(eFULL_LOG,
                  "["__FUNCTION__"] [ID:%u]:TriggerMultiSwitchCondition", getParent()->TriggerID());
    );

    return satisfied_;
}



//==================================================================================================
__IMPL_POOL(CONDITION_MISSION_POINT)
VOID
CONDITION_MISSION_POINT::OnInit()
{
    //m_iNumberOfPlayersInArea = 0;
}

VOID
CONDITION_MISSION_POINT::onMsg(TRIGGER_MSG* pmsg)
{
    if(eTRIGGER_ENTER_AREA == pmsg->m_dwMsgID)
    {
        TRIGGER_ENTER_AREA* pMsg = (TRIGGER_ENTER_AREA*)pmsg;

        // 실제 플레이어가 이 영역에 있는가?
        Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(pMsg->m_PlayerKey);
        if(pPlayer == 0 || pPlayer->GetField() == 0)
            return;

        GameZone* pGameZone = pPlayer->GetGameZonePtr();
        int iMapCode = pGameZone ? (int)pGameZone->GetMapCode() : -1;

        if(SAFE_NUMERIC_TYPECAST(int, iMapCode, BYTE))
        {
            const CONDITION_MISSION_POINT_INFO& rInfo = *Get();
            const int iMissionPoint = pPlayer->GetMissionManager()->GetMissionPoint((BYTE)iMapCode);

            const int cond_mission_point = rInfo.GetMissionPoint();
            switch(rInfo.GetFlag())
            {
            case eCOMPARE_EQUAL: setSatisfied(iMissionPoint == cond_mission_point); break;
            case eCOMPARE_MORE:  setSatisfied(iMissionPoint >  cond_mission_point); break;
            case eCOMPARE_LESS:  setSatisfied(iMissionPoint <  cond_mission_point); break;
            default:
                __debugbreak();
                break;
            }
        }
    }
}

//==================================================================================================
__IMPL_POOL(CONDITION_COMPARE_VARIABLE)

BOOL
CONDITION_COMPARE_VARIABLE::IsSatisfied()
{
    return FALSE;
}

//==================================================================================================
//

#include <QuestInfo.h>
#include <QuestManager.h>

class QuestProgress
{
    BOOL&   m_bOrRet;
    QCODE   m_wQuestCode;
    CONDITION_QUEST_PROGRESS_INFO::ePROGRESS m_eCond;
public:
    QuestProgress(BOOL& bOrRet, QCODE wQuestCode, CONDITION_QUEST_PROGRESS_INFO::ePROGRESS iCond)
        : m_bOrRet(bOrRet), m_wQuestCode(wQuestCode), m_eCond(iCond) {}

    VOID operator()(Player* pPlayer)
    {
        QuestManager* pQuestMan = (QuestManager*)pPlayer->GetQuestManager();
        Quest* pQuest = NULL;

        switch(m_eCond)
        {
        case CONDITION_QUEST_PROGRESS_INFO::IN_PROGRESS:
            if(pQuestMan->FindQuest(m_wQuestCode))
                m_bOrRet = TRUE;
            break;
        case CONDITION_QUEST_PROGRESS_INFO::COMPLETED:
            if(pQuestMan->FindComplQuest(m_wQuestCode))
                m_bOrRet = TRUE;
            break;
        };
    }

    VOID Check(SATISFIED_PLAYER_PAIR rPair)
    {
        Player* pPlayer = rPair.second;
        QuestManager* pQuestMan = (QuestManager*)pPlayer->GetQuestManager();
        Quest* pQuest = NULL;

        switch(m_eCond)
        {
        case CONDITION_QUEST_PROGRESS_INFO::IN_PROGRESS:
            if(pQuestMan->FindQuest(m_wQuestCode))
                m_bOrRet = TRUE;
            break;
        case CONDITION_QUEST_PROGRESS_INFO::COMPLETED:
            if(pQuestMan->FindComplQuest(m_wQuestCode))
                m_bOrRet = TRUE;
            break;
        };
    }
};

//--------------------------------------------------------------------------------------------------

__IMPL_POOL(CONDITION_QUEST_PROGRESS)
BOOL
CONDITION_QUEST_PROGRESS::IsSatisfied()
{
    if(satisfied_)
        return TRUE;

    const CONDITION_QUEST_PROGRESS_INFO* pInfo = (const CONDITION_QUEST_PROGRESS_INFO*)Get();

    ASSERT(SAFE_NUMERIC_TYPECAST(ULONG, pInfo->GetQuestID(), QCODE));

    BOOL bOrRet = FALSE;
    QuestProgress op(bOrRet, (QCODE)pInfo->GetQuestID(),
                     (CONDITION_QUEST_PROGRESS_INFO::ePROGRESS)pInfo->GetProgress());

    DWORD clicked_player_key = getParent()->GetClickedPlayerKey();
    if(clicked_player_key != 0)
    {
        Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
        if(pPlayer != 0)
        {
            op(pPlayer);
            if(bOrRet)
                setSatisfied(TRUE);
        }
    }
    else
    {
        getParent()->ForeachSatisfiedPlayer(&QuestProgress::Check, &op);
        if(bOrRet)
            setSatisfied(TRUE);
    }

    return satisfied_;
}

//==================================================================================================

class CheckClassID
{
    BOOL&       m_bOrRet;
    eCHAR_TYPE  m_CharType;
public:
    CheckClassID(BOOL& bOrRet, eCHAR_TYPE eCharType) : m_bOrRet(bOrRet), m_CharType(eCharType) {}

    VOID    operator() (Player* pPlayer)
    {
        if(pPlayer->GetCharType() == m_CharType)
            m_bOrRet = TRUE;
    }

    VOID    Check(SATISFIED_PLAYER_PAIR rPair)
    {
        Player* pPlayer = rPair.second;
        if(pPlayer->GetCharType() == m_CharType)
            m_bOrRet = TRUE;
    }
};

//--------------------------------------------------------------------------------------------------

__IMPL_POOL(CONDITION_CHECK_CLASS_ID)
BOOL
CONDITION_CHECK_CLASS_ID::IsSatisfied()
{
    if(satisfied_)
        return TRUE;

    const CONDITION_CHECK_CLASS_ID_INFO* pInfo = (const CONDITION_CHECK_CLASS_ID_INFO*)Get();

    const DWORD clicked_player_key = getParent()->GetClickedPlayerKey();
    if(clicked_player_key)
    {
        Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
        if(NULL != pPlayer)
        {
            // (WAVERIX) (060606)
            // 일단은 조건으로 주어진 Class ID가 eCHAR_TYPE과 일치한다.
            // 이후를 위해 기획쪽에 이것의 변경이 없도록 요청할 것.
            eCHAR_TYPE eClass = (eCHAR_TYPE)pInfo->GetClassID();
            if(pPlayer->GetCharType() == eClass)
                setSatisfied(TRUE);
        }
    }
    else
    {   // 파티는 포함되지 않는다.
        BOOL bOrRet = FALSE;
        CheckClassID op(bOrRet, (eCHAR_TYPE)pInfo->GetClassID());
        getParent()->ForeachSatisfiedPlayer(&CheckClassID::Check, &op);
        if(bOrRet)
            setSatisfied(TRUE);
    }

    return satisfied_;
}

//==================================================================================================
__IMPL_POOL(CONDITION_NPCKILL_FOR_UNITID)
VOID
CONDITION_NPCKILL_FOR_UNITID::onMsg(TRIGGER_MSG* pmsg)
{
    switch(pmsg->m_dwMsgID)
    {
    case eTRIGGER_KILL_MONSTER:
        {
            TRIGGER_KILL_MONSTER* pMsg = (TRIGGER_KILL_MONSTER*)pmsg;
            CONDITION_NPCKILL_FOR_UNITID_INFO* pInfo = Get();

            if(pMsg->m_MonsterUnitID != pInfo->GetMonsterUnitID())
                return;
            //------------------------
            // 조건 만족
            setSatisfied(TRUE);
            DEBUG_CODE(
              SUNLOG(eFULL_LOG, "[CONDITION_NPCKILL_FOR_UNITID::onMsg] [ID:%u]:MONSTER:[%u]",
                     getParent()->TriggerID(), pInfo->GetMonsterUnitID());
            );
            //------------------------
        }
        break;
    }
}

//==================================================================================================

class CheckPartyMember
{
    BOOL&   m_bAndRet;
    BYTE    m_byLiveFlag;
public:
    CheckPartyMember(BOOL& bAndRet, BYTE byLiveFlag)
        : m_bAndRet(bAndRet), m_byLiveFlag(byLiveFlag) {}

    VOID operator()(Player* pPlayer)
    {
        if(m_bAndRet && pPlayer)
            m_bAndRet = CheckMember(pPlayer);
    }

    VOID operator()(std::pair<DWORD,Player *> pair_type) 
    { 
        operator()(pair_type.second);
    }

    BOOL CheckMember(Player* pPlayer)
    {
        switch(m_byLiveFlag)
        {
            // 생존 : 1인이라도 생존 시 TRUE
        case CONDITION_LIVE_PARTY_MEMBER_INFO::MEMBER_LIVE:
            return pPlayer->IsAlive();

            //사망 : 전원 사망일 시 TRUE
        case CONDITION_LIVE_PARTY_MEMBER_INFO::MEMBER_DIE:
            return !(pPlayer->IsAlive());
        }

        return FALSE;
    }
};

//--------------------------------------------------------------------------------------------------

__IMPL_POOL(CONDITION_LIVE_PARTY_MEMBER)
BOOL
CONDITION_LIVE_PARTY_MEMBER::IsSatisfied()
{
    if(satisfied_)
        return TRUE;

    const CONDITION_LIVE_PARTY_MEMBER_INFO* pInfo = (const CONDITION_LIVE_PARTY_MEMBER_INFO*)Get();

    BOOL and_result = TRUE;
    CheckPartyMember opr(and_result, pInfo->GetLiveFlag());

    getMGR()->ForeachPlayer(opr);

    if(and_result)
        setSatisfied(TRUE);

    return satisfied_;
}


//==================================================================================================

__IMPL_POOL(CONDITION_CHECK_OBJECT_HP)
VOID 
CONDITION_CHECK_OBJECT_HP::OnInit()
{
    m_dwStartTick = GetTickCount();
}


BOOL
CONDITION_CHECK_OBJECT_HP::IsSatisfied()
{
    if(satisfied_)
        return TRUE;

    if((m_dwStartTick + CONDITION_DELEY) <= GetTickCount())
    {
        const CONDITION_CHECK_OBJECT_HP_INFO* pInfo = (const CONDITION_CHECK_OBJECT_HP_INFO*)Get();

        const DWORD  dwObjKey = pInfo->GetdwObjKey();
        const MapNpc* pMapNpc = getMGR()->FindMapNpc(dwObjKey);
        if (NULL != pMapNpc)
        {
            const FLOAT infoHP = (FLOAT)pInfo->GetbtHPPercentage();    
            const FLOAT mapNpcHP = pMapNpc->GetHPRatio()*100.0f;
            if (infoHP > mapNpcHP)
                setSatisfied(TRUE);
        }
        m_dwStartTick = GetTickCount();
    }

    return satisfied_;
}

__IMPL_POOL(CONDITION_DESTROY_OBJECT)
VOID 
CONDITION_DESTROY_OBJECT::OnInit()
{
    m_dwStartTick = GetTickCount();
}

BOOL
CONDITION_DESTROY_OBJECT::IsSatisfied()
{
    if(satisfied_)
        return TRUE;

    if((m_dwStartTick + CONDITION_DELEY) <= GetTickCount())
    {
        const CONDITION_DESTROY_OBJECT_INFO* pInfo = (const CONDITION_DESTROY_OBJECT_INFO*)Get();

        const DWORD  dwObjKey = pInfo->GetdwObjKey();
        if (0 != dwObjKey)
        {
            const MapNpc* pMapNpc = getMGR()->FindMapNpc(dwObjKey);
            if (NULL != pMapNpc && pMapNpc->IsDead())
                setSatisfied(TRUE);
        }

        m_dwStartTick = GetTickCount();
    }

    return satisfied_;
}

__IMPL_POOL(CONDITION_CHECK_OBJECTKIND_HP)
VOID 
CONDITION_CHECK_OBJECTKIND_HP::OnInit()
{
    start_tick_ = GetTickCount();
}


BOOL
CONDITION_CHECK_OBJECTKIND_HP::IsSatisfied()
{
    if(satisfied_)
        return TRUE;

    if((start_tick_ + CONDITION_DELEY) <= GetTickCount())
    {
        const CONDITION_CHECK_OBJECTKIND_HP_INFO* info = Get();

        const BYTE object_type = info->GetbyObjectType();
        const DWORD object_key = info->GetdwObjectKey();

        NPC* npc = NULL;

        switch (object_type)
        {
        case UNITID_TYPE:
            npc = getMGR()->FindNPCForUnitID(object_key);
            break;

        case MAPNPC_TYPE:
            npc = getMGR()->FindMapNpc(object_key);
            break;

        default:
            break;
        }

        if (npc != NULL)
        {
            const FLOAT info_HP = static_cast<FLOAT>(info->GetbtHPPercentage());    
            const FLOAT npc_HP = npc->GetHPRatio()*100.0f;
            if (info_HP >= npc_HP)
            {
                setSatisfied(TRUE);
                getParent()->SetObjectKey(npc->GetObjectKey());
            }
        }

        start_tick_ = GetTickCount();
    }

    return satisfied_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER

__IMPL_POOL(CONDITION_COMPLETE_COLLECTION);

void CONDITION_COMPLETE_COLLECTION::OnInit()
{
    setSatisfied(false);
}

void CONDITION_COMPLETE_COLLECTION::OnReset()
{
    setSatisfied(false);
}

BOOL CONDITION_COMPLETE_COLLECTION::IsSatisfied()
{
    if (satisfied_) 
    {
        return true;
    }
    return satisfied_;
}

void CONDITION_COMPLETE_COLLECTION::onMsg(TRIGGER_MSG* trigger_msg)
{
    if (trigger_msg == NULL)
    {
        return;
    }

    const eTRIGGER_MSG msg_type = static_cast<eTRIGGER_MSG>(trigger_msg->m_dwMsgID);
    if (msg_type == eTRIGGER_COMPLETE_COLLECTION)
    {
        const TRIGGER_COMPLETE_COLLECTION* const msg = 
            static_cast<const TRIGGER_COMPLETE_COLLECTION* const>(trigger_msg);

        const CONDITION_COMPLETE_COLLECTION_INFO& info = *Get();
        if (MAP_OBJECT_KEY + info.GetObjectKey() == msg->object_key) 
        {
            setSatisfied(true);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// _NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER

__IMPL_POOL(CONDITION_SERVER_EVENT);

void CONDITION_SERVER_EVENT::OnInit()
{
    setSatisfied(false);
}

void CONDITION_SERVER_EVENT::OnReset()
{
    setSatisfied(false);
}

BOOL CONDITION_SERVER_EVENT::IsSatisfied()
{
    if (satisfied_) 
    {
        return true;
    }
    return satisfied_;
}

void CONDITION_SERVER_EVENT::onMsg(TRIGGER_MSG* trigger_msg)
{
    if (trigger_msg == NULL)
    {
        return;
    }

    const eTRIGGER_MSG msg_type = static_cast<eTRIGGER_MSG>(trigger_msg->m_dwMsgID);
    if (msg_type == eTRIGGER_SERVER_EVENT)
    {
        const TRIGGER_SERVER_EVENT* const msg = 
            static_cast<const TRIGGER_SERVER_EVENT* const>(trigger_msg);

        const CONDITION_SERVER_EVENT_INFO& server_event_info = *Get();
        if (server_event_info.GetEventID() == msg->event_id) 
        {
            setSatisfied(true);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM

class CheckTeam
{
    BOOL&   is_satisfied_;
    int     team_key_;
public:
    CheckTeam(BOOL& is_satisfied, int team_key) : is_satisfied_(is_satisfied), team_key_(team_key) 
    {
    }

    VOID    operator() (Player* const player)
    {
        if(player->GetTeamKey() == team_key_)
            is_satisfied_ = TRUE;
    }

    VOID    Check(SATISFIED_PLAYER_PAIR rPair)
    {
        Player* const player = rPair.second;
        if(player->GetTeamKey() == team_key_)
            is_satisfied_ = TRUE;
    }
};

__IMPL_POOL(CONDITION_CHECK_TEAM);
BOOL CONDITION_CHECK_TEAM::IsSatisfied()
{
    if (satisfied_) 
        return true;

    const CONDITION_CHECK_TEAM_INFO* const check_team_info = (const CONDITION_CHECK_TEAM_INFO*)Get();

    const DWORD clicked_player_key = getParent()->GetClickedPlayerKey();
    if(clicked_player_key)
    {
        const Player* const player = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
        if(NULL != player)
        {
            // (WAVERIX) (060606)
            // 일단은 조건으로 주어진 Class ID가 eCHAR_TYPE과 일치한다.
            // 이후를 위해 기획쪽에 이것의 변경이 없도록 요청할 것.
            const int team_key = check_team_info->GetTeamKey();
            if(player->GetTeamKey() == team_key)
                setSatisfied(TRUE);
        }
    }
    else
    {   
        BOOL is_result = FALSE;
        CheckTeam op(is_result, (int)check_team_info->GetTeamKey());
        getParent()->ForeachSatisfiedPlayer(&CheckTeam::Check, &op);
        if(is_result)
            setSatisfied(TRUE);
    }

    return satisfied_;
}

__IMPL_POOL(CONDITION_MONSTER_ENTER_AREA);
void CONDITION_MONSTER_ENTER_AREA::onMsg( TRIGGER_MSG* trigger_msg )
{
    const eTRIGGER_MSG msg_type = static_cast<eTRIGGER_MSG>(trigger_msg->m_dwMsgID);
    switch (msg_type)
    {
    case eTRIGGER_NPC_ENTER_AREA:
        {
            const TRIGGER_NPC_ENTER_AREA* const enter_msg = 
                static_cast<const TRIGGER_NPC_ENTER_AREA*>(trigger_msg);
            const CONDITION_MONSTER_ENTER_AREA_INFO& trigger_info = *Get();

            NPC* const npc = getMGR()->FindNPC(enter_msg->object_key);
            if (npc == NULL)
                return;

            if (trigger_info.GetAreaID() == enter_msg->area_id &&
                trigger_info.GetMonsterCode() == npc->GetBaseInfo()->m_MonsterCode)
            {
                bool satisfied = satisfied_;
                if (this->IsSatisfied() != satisfied) {
                    getParent()->SetObjectKey(enter_msg->object_key);
                }
            }
        };
        break;
    case eTRIGGER_NPC_LEAVE_AREA:
        {
            const TRIGGER_NPC_LEAVE_AREA* const leave_msg = 
                static_cast<const TRIGGER_NPC_LEAVE_AREA*>(trigger_msg);
            const CONDITION_MONSTER_ENTER_AREA_INFO& trigger_info = *Get();
            
            // to do
        };
        break;
    };
}

BOOL CONDITION_MONSTER_ENTER_AREA::IsSatisfied()
{
    if (satisfied_) {
        return true;
    }

    const CONDITION_MONSTER_ENTER_AREA_INFO& trigger_info = *Get();
    const int area_id = trigger_info.GetAreaID();

    TriggerManagerEx* const trigger_manager = getMGR();
    TriggerAreaForNpc* const trigger_area =
        trigger_manager->GetAreaManagerForNpc()->FindArea(area_id);
    if (trigger_area == NULL) {
        return false;
    }
    const DWORD number_of_entered = trigger_area->GetNpcNumber();
    bool need_process = (number_of_entered != 0);
    if (need_process == false) {
        return satisfied_;
    };
    //
    uint16_t new_updated = static_cast<uint16_t>(number_of_entered);
    if (local_storage_.last_updated_number_of_chars_ < new_updated)
    {
        local_storage_.last_updated_number_of_chars_ = new_updated;
        if (Trigger* trigger = getParent()) {
            trigger->NeedActionByRuntimeConfig();
        };
        setSatisfied(true);
    };
    //

    return satisfied_;
}

void CONDITION_MONSTER_ENTER_AREA::OnInit()
{
    setSatisfied(false);
}

void CONDITION_MONSTER_ENTER_AREA::OnReset()
{
    setSatisfied(false);
}
