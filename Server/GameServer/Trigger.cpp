#include "stdafx.h"
#include "..\\..\\SolarTrigger\\TriggerCommon.h"
#include "..\\..\\SolarTrigger\\ConditionInfo.h"
#include "..\\..\\SolarTrigger\\ActionInfo.h"

//==================================================================================================

#include "Trigger.h"
#include "IConditionTriggerEx.h"
#include "IActionTriggerEx.h"
#include "ConditionTriggers.h"
#include "ActionTriggers.h"
#include "TriggerManagerEx.h"

//==================================================================================================

#define __TRIGGER_BEGIN_SWITCH            sSWITCH(pTrigger->TriggerType())
#define __TRIGGER_DESTROY_CASE(className) CASE(e##className) className::Destroy((className*)pTrigger);
#define __TRIGGER_CREATE_CASE(className)  CASE(e##className)                            \
                                                className* pNew = className::Create();  \
                                                pNew->Set((className##_INFO*)pTrigger); \
                                                pITrigger = pNew;
#define __TRIGGER_EXCEPT(str)             DEFAULT ASSERT(!str);
#define __TRIGGER_END_SWITCH              eSWITCH

//==================================================================================================

__IMPL_POOL(Trigger);

Trigger::Trigger(void)
{
    unsatisfied_players.clear();
}

Trigger::~Trigger(void)
{
}

void
Trigger::OnRelease()
{
    RemoveAllSatisfiedPlayer();

    unsatisfied_players.clear();
}

BOOL
Trigger::AddSatisfiedPlayer(Player* pPlayer)
{
    const DWORD player_key = pPlayer->GetObjectKey();
    BOOLEAN finded = false;
    FOREACH_CONTAINER(const SATISFIED_PLAYER_LIST::value_type& rPlayer, m_SatisfiedPlayers,
                      SATISFIED_PLAYER_LIST)
    {
        if(player_key == rPlayer.first)
        {
            finded = true;
            break;
        }
    }

    if(finded == false)
    {
        // (WAVERIX) (090621) (CHANGES)
        if(m_SatisfiedPlayers.capacity() == 0)
            m_SatisfiedPlayers.reserve(MAX_PARTYMEMBER_NUM);
        m_SatisfiedPlayers.push_back(std::make_pair(player_key, pPlayer));
    }

    return finded == false;
}

void
Trigger::RemoveAllSatisfiedPlayer()
{
    m_SatisfiedPlayers.clear();
}

Player*
Trigger::GetFirstSatisfiedPlayer()
{
    if(m_SatisfiedPlayers.empty())
        return 0;
    return m_SatisfiedPlayers.front().second;
}

BOOL
Trigger::Update()
{
    // is satisfied되면 update를 하지 않는다.
    switch(m_state)
    {
    case DONTUPDATE_STATE:
        return TRUE;
    case UPDATE_STATE:
        {
            // 시간이 지나가면이라는 타입때문에 UPDATE_STATE 상태가 추가됨
            // 시간이 지나가면이 포함된 트리거는 update가 되기때문에 오버헤드가 큰 다른 트리거와 같이 사용하지 않는편이 좋다.
            // 추후, 트리거 업데이트 틱을 늘린다!
            // 최적화 할 여지가 있음
            Execute();
        }
        break;
    case ACTIONUPDATE_STATE: 
        if(ActionUpdate() == FALSE) 
        {
            m_state = TERMINATING_STATE; 
        } 
        break;
    case TERMINATING_STATE:
        return FALSE;
    }

    return TRUE;
}

// return FALSE이면 트리거 종료
BOOL
Trigger::ActionUpdate()
{
    BOOLEAN bOrResult = FALSE;
    FOREACH_CONTAINER(const ACTION_TRIGGER_LIST::value_type& rNode, m_listActionTrigger,
                      ACTION_TRIGGER_LIST)
    {
        IActionTrigger* pIAction = rNode;
        if(pIAction && pIAction->OnActionUpdate())
            bOrResult = TRUE;
    }

    return bOrResult;
}

void
Trigger::_OnAction()
{
    MSG_CG_TRIGGER_DO_ACTION_BRD msgBRD;
    msgBRD.m_dwTriggerID = TriggerID();
    // (WAVERIX) (090614) (REMARK) field type 구분할 필요가 있다.
    // 배틀존은 전체로 보내도 되지만, 다른 것은... 너무 정도가 심해진다. 하지만 상태 변경 계열
    //이면 미묘한뎅....
    static_cast<TriggerManagerEx*>(m_pTriggerManager)->SendToAll(&msgBRD, sizeof(msgBRD));
}

#pragma warning (push)
#pragma warning (disable : 4311)

void
Trigger::_createCondition(ConditionInfo* pTrigger)
{
    IConditionTriggerEx* pITrigger = NULL;

    __TRIGGER_BEGIN_SWITCH
        __TRIGGER_CREATE_CASE(CONDITION_ALWAYS)
        __TRIGGER_CREATE_CASE(CONDITION_COMPARE_SWITCH)
		__TRIGGER_CREATE_CASE(CONDITION_COMPARE_SWITCH_EX)
        __TRIGGER_CREATE_CASE(CONDITION_CLICK_OBJECT)
        __TRIGGER_CREATE_CASE(CONDITION_DAMAGE_OBJECT)
        __TRIGGER_CREATE_CASE(CONDITION_COUNT_NPCKILL)
        __TRIGGER_CREATE_CASE(CONDITION_PASS_TIME)
        __TRIGGER_CREATE_CASE(CONDITION_ENTER_AREA)
            TriggerAreaLocalStorage* local_storage = &pNew->local_storage_;
            m_pTriggerManager->CreateArea(pNew->Get()->GetAreaID(), local_storage);
        __TRIGGER_CREATE_CASE(CONDITION_HAVE_ITEM)
        __TRIGGER_CREATE_CASE(CONDITION_MULTI_SWITCH)
        __TRIGGER_CREATE_CASE(CONDITION_MISSION_POINT)
            //m_pMGR->CreateAreaHash(pNew->Get()->GetAreaID()); //< TriggerManager의 TriggerAreaHash생성
        __TRIGGER_CREATE_CASE(CONDITION_COMPARE_VARIABLE)
        __TRIGGER_CREATE_CASE(CONDITION_QUEST_PROGRESS)
        __TRIGGER_CREATE_CASE(CONDITION_CHECK_CLASS_ID)
        __TRIGGER_CREATE_CASE(CONDITION_NPCKILL_FOR_UNITID)
        __TRIGGER_CREATE_CASE(CONDITION_LIVE_PARTY_MEMBER)
        __TRIGGER_CREATE_CASE(CONDITION_CHECK_OBJECT_HP)
        __TRIGGER_CREATE_CASE(CONDITION_DESTROY_OBJECT)
        __TRIGGER_CREATE_CASE(CONDITION_CHECK_OBJECTKIND_HP)
        // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
        __TRIGGER_CREATE_CASE(CONDITION_NPC_ENTER_AREA)
        #ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
            TriggerAreaLocalStorageForNpc* local_storage = &pNew->local_storage_;
            CONDITION_NPC_ENTER_AREA_INFO* condition_info = pNew->Get();
            static_cast<TriggerManagerEx*>(m_pTriggerManager)->CreateAreaForNpc(
                condition_info->GetAreaID(), condition_info->GetNPCUnitID(), local_storage);
        #endif
        //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
        __TRIGGER_CREATE_CASE(CONDITION_COMPLETE_COLLECTION)
        // _NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER
        __TRIGGER_CREATE_CASE(CONDITION_SERVER_EVENT)
        // _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM
        __TRIGGER_CREATE_CASE(CONDITION_CHECK_TEAM)
        __TRIGGER_CREATE_CASE(CONDITION_MONSTER_ENTER_AREA)
        {
            TriggerAreaLocalStorageForNpc* local_storage = &pNew->local_storage_;
            CONDITION_MONSTER_ENTER_AREA_INFO* condition_info = pNew->Get();
            static_cast<TriggerManagerEx*>(m_pTriggerManager)->CreateAreaForMonster(
                condition_info->GetAreaID(), condition_info->GetMonsterCode(), local_storage);
        }

        __TRIGGER_EXCEPT("등록되지 않은 컨디션 트리거입니다.1")
    __TRIGGER_END_SWITCH

    pITrigger->Init(m_pTriggerManager, this);
    m_listConditionTrigger.push_back(pITrigger);
}


void
Trigger::_createAction(ActionInfo* pTrigger)
{
    IActionTriggerEx* pITrigger = NULL;

    __TRIGGER_BEGIN_SWITCH
        //__TRIGGER_CREATE_CASE(ACTION_CHANGE_SWITCH)
        __TRIGGER_CREATE_CASE(ACTION_CHANGE_OBJECTANI)
        __TRIGGER_CREATE_CASE(ACTION_CHANGE_PATHTILE)
        __TRIGGER_CREATE_CASE(ACTION_CHANGE_OBJECTSTATE)
        __TRIGGER_CREATE_CASE(ACTION_PLAY_EVENT)
        __TRIGGER_CREATE_CASE(ACTION_SET_ISATTACK)
        __TRIGGER_CREATE_CASE(ACTION_PORTAL_PLAYER)
        __TRIGGER_CREATE_CASE(ACTION_SET_OBJECTTHRUST)
        __TRIGGER_CREATE_CASE(ACTION_APPLY_DAMAGE)
        __TRIGGER_CREATE_CASE(ACTION_REFLECT_DAMAGE)
        __TRIGGER_CREATE_CASE(ACTION_CREATE_MONSTER)
        __TRIGGER_CREATE_CASE(ACTION_DISPLAY_MSGBOX)
        __TRIGGER_CREATE_CASE(ACTION_REMOVE_ITEM)
        //__TRIGGER_CREATE_CASE(ACTION_PRESERVER)
        __TRIGGER_CREATE_CASE(ACTION_REWARD_PLAYER)
        __TRIGGER_CREATE_CASE(ACTION_CLEAR_MISSION)
        __TRIGGER_CREATE_CASE(ACTION_PORTAL_RANDOM)
        __TRIGGER_CREATE_CASE(ACTION_NPC_RANDOM_AREA_MOVE)
        __TRIGGER_CREATE_CASE(ACTION_ACTIVATE_TRIGGER)
        __TRIGGER_CREATE_CASE(ACTION_AREA_DAMAGE)
        __TRIGGER_CREATE_CASE(ACTION_OPERATE_SWITCH)
		__TRIGGER_CREATE_CASE(ACTION_OPERATE_SWITCH_EX)
        __TRIGGER_CREATE_CASE(ACTION_ROAD_CONNECT)
        __TRIGGER_CREATE_CASE(ACTION_CREATE_DIR_MONSTER)
        __TRIGGER_CREATE_CASE(ACTION_CHANGE_LIGHT)
        __TRIGGER_CREATE_CASE(ACTION_CHANGE_OBJECT_TYPE)
        __TRIGGER_CREATE_CASE(ACTION_QUEST_SATISFACTION)
        __TRIGGER_CREATE_CASE(ACTION_QUEST_GRANT)
        __TRIGGER_CREATE_CASE(ACTION_QUEST_SETUP)
        __TRIGGER_CREATE_CASE(ACTION_CREATE_MONSTER_GROUP)
        __TRIGGER_CREATE_CASE(ACTION_SHOW_OR_HIDE_UNIT)
        __TRIGGER_CREATE_CASE(ACTION_SHOW_GUIDE_MSGBOX)
        __TRIGGER_CREATE_CASE(ACTION_SHOW_OR_HIDE_MSGBOX)
        __TRIGGER_CREATE_CASE(ACTION_RANDOM_MONSTER_SPAWN)
        __TRIGGER_CREATE_CASE(ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR)
        __TRIGGER_CREATE_CASE(ACTION_RANDOM_MAP_MOVE)
        __TRIGGER_CREATE_CASE(ACTION_RANDOM_AREA_MOVE)
        __TRIGGER_CREATE_CASE(ACTION_REMOVE_MONSTER)
        __TRIGGER_CREATE_CASE(ACTION_ADD_EXP)
        __TRIGGER_CREATE_CASE(ACTION_MONSTER_STAT_UP_OR_DOWN)
        __TRIGGER_CREATE_CASE(ACTION_RANDUM_TRIGER_ON_OR_OFF)
        __TRIGGER_CREATE_CASE(ACTION_PLAY_MUSIC)
        __TRIGGER_CREATE_CASE(ACTION_DOWN_OBJETC_HP)
        __TRIGGER_CREATE_CASE(ACTION_CHANGE_ATTACK_KIND)
        __TRIGGER_CREATE_CASE(ACTION_ACTIVE_STAMP)
        __TRIGGER_CREATE_CASE(ACTION_ATTACK_OBJECTKIND)
        __TRIGGER_CREATE_CASE(ACTION_ACTIVE_SKILL_OBJECTKIND)
        __TRIGGER_CREATE_CASE(ACTION_SHOW_MESSAGEBOX_OBJECTKIND)
        __TRIGGER_CREATE_CASE(ACTION_SET_ATTACK_STATE_OBJECTKIND)
        __TRIGGER_CREATE_CASE(ACTION_DESTROY_OBJECTKIND)
        __TRIGGER_CREATE_CASE(ACTION_CREATE_MONSTER_LINKED_MAPOBJECT)
        __TRIGGER_CREATE_CASE(ACTION_CONTROL_TIMER)
        __TRIGGER_CREATE_CASE(ACTION_DO_COMMAND) //_NA_0_20100705_DOMINATION_EVENT
        //_NA_0_20100817_HELLON_LAST_TRIGGER = {
        __TRIGGER_CREATE_CASE(ACTION_MOVE_CONTROL)
        __TRIGGER_CREATE_CASE(ACTION_ANIMATION_CONTROL)
        // }
        // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
        __TRIGGER_CREATE_CASE(ACTION_NPC_APPLY_DAMAGE)
        //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
        __TRIGGER_CREATE_CASE(ACTION_COMPLETE_COLLECTION)
        // _NA_000000_20130104_ADD_ACTION_TRIGGER_APPLY_HPMPSD
        __TRIGGER_CREATE_CASE(ACTION_APPLY_HPMPSD)
        //_NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY
        __TRIGGER_CREATE_CASE(ACTION_CHANGE_OBJECT_HP)
        //_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
        __TRIGGER_CREATE_CASE(ACTION_CHANGE_RADAR_MAP)
        // _NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY
        __TRIGGER_CREATE_CASE(ACTION_ATTACH_STATE_RANGE)
        //_NA_000000_20130925_ADD_ACTION_TRIGGER_CHANGE_PLAYER_HP
        __TRIGGER_CREATE_CASE(ACTION_CHANGE_PLAYER_HP)

        __TRIGGER_EXCEPT("등록되지 않은 액션 트리거입니다.1")
    __TRIGGER_END_SWITCH

    pITrigger->Init(m_pTriggerManager, this);
    m_listActionTrigger.push_back(pITrigger);
}

#pragma warning (pop)


void
Trigger::_destroyCondition(IConditionTrigger* pTrigger)
{
    __TRIGGER_BEGIN_SWITCH
        __TRIGGER_DESTROY_CASE(CONDITION_ALWAYS)
        __TRIGGER_DESTROY_CASE(CONDITION_COMPARE_SWITCH)
		__TRIGGER_DESTROY_CASE(CONDITION_COMPARE_SWITCH_EX)
        __TRIGGER_DESTROY_CASE(CONDITION_CLICK_OBJECT)
        __TRIGGER_DESTROY_CASE(CONDITION_DAMAGE_OBJECT)
        __TRIGGER_DESTROY_CASE(CONDITION_COUNT_NPCKILL)
        __TRIGGER_DESTROY_CASE(CONDITION_PASS_TIME)
        __TRIGGER_DESTROY_CASE(CONDITION_ENTER_AREA)
        __TRIGGER_DESTROY_CASE(CONDITION_HAVE_ITEM)
        __TRIGGER_DESTROY_CASE(CONDITION_MULTI_SWITCH)
        __TRIGGER_DESTROY_CASE(CONDITION_MISSION_POINT)
        __TRIGGER_DESTROY_CASE(CONDITION_COMPARE_VARIABLE)
        __TRIGGER_DESTROY_CASE(CONDITION_QUEST_PROGRESS)
        __TRIGGER_DESTROY_CASE(CONDITION_CHECK_CLASS_ID)
        __TRIGGER_DESTROY_CASE(CONDITION_NPCKILL_FOR_UNITID)
        __TRIGGER_DESTROY_CASE(CONDITION_LIVE_PARTY_MEMBER)
        __TRIGGER_DESTROY_CASE(CONDITION_CHECK_OBJECT_HP)
        __TRIGGER_DESTROY_CASE(CONDITION_DESTROY_OBJECT)
        __TRIGGER_DESTROY_CASE(CONDITION_CHECK_OBJECTKIND_HP)
        // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
        __TRIGGER_DESTROY_CASE(CONDITION_NPC_ENTER_AREA)
        //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
        __TRIGGER_DESTROY_CASE(CONDITION_COMPLETE_COLLECTION)
        //_NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER
        __TRIGGER_DESTROY_CASE(CONDITION_SERVER_EVENT)
        // _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM
        __TRIGGER_DESTROY_CASE(CONDITION_CHECK_TEAM)
        __TRIGGER_DESTROY_CASE(CONDITION_MONSTER_ENTER_AREA)

        __TRIGGER_EXCEPT("등록되지 않은 컨디션 트리거입니다.2")
    __TRIGGER_END_SWITCH
}

void
Trigger::_destroyAction(IActionTrigger* pTrigger)
{
    __TRIGGER_BEGIN_SWITCH
        //  __TRIGGER_DESTROY_CASE(ACTION_CHANGE_SWITCH)
        __TRIGGER_DESTROY_CASE(ACTION_CHANGE_OBJECTANI)
        __TRIGGER_DESTROY_CASE(ACTION_CHANGE_PATHTILE)
        __TRIGGER_DESTROY_CASE(ACTION_CHANGE_OBJECTSTATE)
        __TRIGGER_DESTROY_CASE(ACTION_PLAY_EVENT)
        __TRIGGER_DESTROY_CASE(ACTION_SET_ISATTACK)
        __TRIGGER_DESTROY_CASE(ACTION_PORTAL_PLAYER)
        __TRIGGER_DESTROY_CASE(ACTION_SET_OBJECTTHRUST)
        __TRIGGER_DESTROY_CASE(ACTION_APPLY_DAMAGE)
        __TRIGGER_DESTROY_CASE(ACTION_REFLECT_DAMAGE)
        __TRIGGER_DESTROY_CASE(ACTION_CREATE_MONSTER)
        __TRIGGER_DESTROY_CASE(ACTION_DISPLAY_MSGBOX)
        __TRIGGER_DESTROY_CASE(ACTION_REMOVE_ITEM)
        //  __TRIGGER_DESTROY_CASE(ACTION_PRESERVER)
        __TRIGGER_DESTROY_CASE(ACTION_REWARD_PLAYER)
        __TRIGGER_DESTROY_CASE(ACTION_CLEAR_MISSION)
        __TRIGGER_DESTROY_CASE(ACTION_PORTAL_RANDOM)
        __TRIGGER_DESTROY_CASE(ACTION_NPC_RANDOM_AREA_MOVE)
        __TRIGGER_DESTROY_CASE(ACTION_ACTIVATE_TRIGGER)
        __TRIGGER_DESTROY_CASE(ACTION_AREA_DAMAGE)
        __TRIGGER_DESTROY_CASE(ACTION_OPERATE_SWITCH)
		__TRIGGER_DESTROY_CASE(ACTION_OPERATE_SWITCH_EX)
        __TRIGGER_DESTROY_CASE(ACTION_ROAD_CONNECT)
        __TRIGGER_DESTROY_CASE(ACTION_CREATE_DIR_MONSTER)
        __TRIGGER_DESTROY_CASE(ACTION_CHANGE_LIGHT)
        __TRIGGER_DESTROY_CASE(ACTION_CHANGE_OBJECT_TYPE)
        __TRIGGER_DESTROY_CASE(ACTION_QUEST_SATISFACTION)
        __TRIGGER_DESTROY_CASE(ACTION_QUEST_GRANT)
        __TRIGGER_DESTROY_CASE(ACTION_QUEST_SETUP)
        __TRIGGER_DESTROY_CASE(ACTION_CREATE_MONSTER_GROUP)
        __TRIGGER_DESTROY_CASE(ACTION_SHOW_OR_HIDE_UNIT)
        __TRIGGER_DESTROY_CASE(ACTION_SHOW_GUIDE_MSGBOX)
        __TRIGGER_DESTROY_CASE(ACTION_SHOW_OR_HIDE_MSGBOX)
        __TRIGGER_DESTROY_CASE(ACTION_RANDOM_MONSTER_SPAWN)
        __TRIGGER_DESTROY_CASE(ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR)
        __TRIGGER_DESTROY_CASE(ACTION_RANDOM_MAP_MOVE)
        __TRIGGER_DESTROY_CASE(ACTION_RANDOM_AREA_MOVE)
        __TRIGGER_DESTROY_CASE(ACTION_REMOVE_MONSTER)
        __TRIGGER_DESTROY_CASE(ACTION_ADD_EXP)
        __TRIGGER_DESTROY_CASE(ACTION_MONSTER_STAT_UP_OR_DOWN)
        __TRIGGER_DESTROY_CASE(ACTION_RANDUM_TRIGER_ON_OR_OFF)
        __TRIGGER_DESTROY_CASE(ACTION_PLAY_MUSIC)
        __TRIGGER_DESTROY_CASE(ACTION_DOWN_OBJETC_HP)
        __TRIGGER_DESTROY_CASE(ACTION_CHANGE_ATTACK_KIND)
        __TRIGGER_DESTROY_CASE(ACTION_ACTIVE_STAMP)
        __TRIGGER_DESTROY_CASE(ACTION_ATTACK_OBJECTKIND)
        __TRIGGER_DESTROY_CASE(ACTION_ACTIVE_SKILL_OBJECTKIND)
        __TRIGGER_DESTROY_CASE(ACTION_SHOW_MESSAGEBOX_OBJECTKIND)
        __TRIGGER_DESTROY_CASE(ACTION_SET_ATTACK_STATE_OBJECTKIND)
        __TRIGGER_DESTROY_CASE(ACTION_DESTROY_OBJECTKIND)
        __TRIGGER_DESTROY_CASE(ACTION_CREATE_MONSTER_LINKED_MAPOBJECT)
        __TRIGGER_DESTROY_CASE(ACTION_CONTROL_TIMER)
        __TRIGGER_DESTROY_CASE(ACTION_DO_COMMAND) //_NA_0_20100705_DOMINATION_EVENT
        //_NA_0_20100817_HELLON_LAST_TRIGGER = {
        __TRIGGER_DESTROY_CASE(ACTION_MOVE_CONTROL)
        __TRIGGER_DESTROY_CASE(ACTION_ANIMATION_CONTROL)
        //}
        // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
        __TRIGGER_DESTROY_CASE(ACTION_NPC_APPLY_DAMAGE)
        //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
        __TRIGGER_DESTROY_CASE(ACTION_COMPLETE_COLLECTION)
        // _NA_000000_20130104_ADD_ACTION_TRIGGER_APPLY_HPMPSD
        __TRIGGER_DESTROY_CASE(ACTION_APPLY_HPMPSD)
        //_NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY
        __TRIGGER_DESTROY_CASE(ACTION_CHANGE_OBJECT_HP)
        //_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
        __TRIGGER_DESTROY_CASE(ACTION_CHANGE_RADAR_MAP)
        // _NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY
        __TRIGGER_DESTROY_CASE(ACTION_ATTACH_STATE_RANGE)
        //_NA_000000_20130925_ADD_ACTION_TRIGGER_CHANGE_PLAYER_HP
        __TRIGGER_DESTROY_CASE(ACTION_CHANGE_PLAYER_HP)

        __TRIGGER_EXCEPT("등록되지 않은 액션 트리거입니다.2")
    __TRIGGER_END_SWITCH
}

VOID Trigger::RemoveUnsatisfiedPlayer()
{
    STLX_VECTOR<DWORD>::iterator itr = unsatisfied_players.begin();

    for (itr; itr != unsatisfied_players.end(); ++itr) 
    {
        Object::Key player_key = (*itr);
        Player* player = PlayerManager::Instance()->FindPlayerByObjKey(player_key);

        SATISFIED_PLAYER_PAIR pair = std::make_pair((DWORD)player_key, player);
        SATISFIED_PLAYER_LIST::iterator find_itr = 
            std::find(m_SatisfiedPlayers.begin(), m_SatisfiedPlayers.end(), pair);

        if (find_itr != m_SatisfiedPlayers.end())
        {
            m_SatisfiedPlayers.erase(find_itr);
        }
    }
}

VOID Trigger::CheckUnsatisfiedPlayer( Object::Key player_key )
{
    unsatisfied_players.push_back(player_key);
}

