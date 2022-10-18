#include "stdafx.h"
#include ".\TriggerManagerEx.h"

#include <PublicMath.h>
#include <AreaConquestCommonHeader.h>
#include "FatigueManager.h"

#include "GameZone.h"
#include "GameMissionRoom.h"
#include "GameInstanceDungeon.h"
#include "MapObject.h"
#include "MissionManager.h"
#include "MissionRewardManager.h"
#include "Monster.h"
#include "MovingPlanner.h"

#include "ObjectFactory.h"
#include "PacketStruct_AG.h"

#include "Trigger.h"
#include "..\\..\\SolarTrigger\\TriggerGroupInfo.h"

#ifdef _NA_001990_ACSYSTEM_ADD
#include "BattleRecorder.h"
#include "GameBattleResult.h"
#endif //_NA_001990_ACSYSTEM_ADD

#ifdef _NA_006607_20130402_ADD_QUESTTYPE
#include "QuestManager.h"
#include "QuestManager_Concrete.h"
#endif

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRanking/SunRankingManager.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

//==================================================================================================
//
BaseTrigger* TriggerManagerEx::AllocTrigger()
{
    return Trigger::Create();
}

void TriggerManagerEx::FreeTrigger(BaseTrigger* trigger)
{
    return Trigger::Destroy(static_cast<Trigger*>(trigger));
}

//==================================================================================================
//  <TriggerManagerEx Implementation>
//
TriggerManagerEx::TriggerManagerEx()
    : m_pField(NULL)
    , m_bInitCompleted(false)
{
}

TriggerManagerEx::~TriggerManagerEx()
{
}

void TriggerManagerEx::Init(TriggerGroupInfo& IN trigger_group_info, GameField* game_field)
{
    // 트리거를 두번 초기화 하지 않는다.
    if (m_bInitCompleted != false) {
        return;
    };
    ASSERT(m_listTrigger.empty());
    ASSERT(GetTriggerAreaCount() == 0);

    m_pField = game_field;
    SWITCH.Clear();

    const FIELDCODE field_code = game_field->GetFieldCode();
    const TRIGGER_INFO_LIST& trigger_info_list = trigger_group_info.GetTriggerInfos();
    m_listTrigger.reserve(trigger_info_list.size());
    FOREACH_CONTAINER(const TRIGGER_INFO_LIST::value_type& info_node, trigger_info_list,
                      TRIGGER_INFO_LIST)
    {
        TriggerInfo* trigger_info = info_node;
        assert(trigger_info);

        Trigger* trigger = static_cast<Trigger*>(this->AllocTrigger());
        trigger->Init(this, trigger_info);

        if (FlowControl::FCAssert(this->InsertTrigger(trigger)) == false)
        {
            SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't create trigger|")
                   _T("|TRIGGERID[%u][%s], FIELDCODE[%d]|"),
                   trigger->TriggerID(), trigger_info->CategoryName(),
                   field_code);
            this->FreeTrigger(trigger);
            // NOTE: f110720.2L, dangerous state
            return;
        }

        if (trigger_info->IsActive()) {
            this->InsertActiveTrigger(trigger->TriggerID());
        };
    }; //end 'FOREACH_CONTAINER'
    m_bInitCompleted = true;
}

void
TriggerManagerEx::Release()
{
    TriggerManager::Release();
#ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
    trigger_area_manager_for_npc_.Release();
#endif
    m_bInitCompleted = FALSE;
    m_pField = NULL;
}

#ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
bool TriggerManagerEx::CreateAreaForNpc(
    int area_id, CODETYPE unit_id, TriggerAreaLocalStorageForNpc* area_item)
{
    bool success = trigger_area_manager_for_npc_.InsertNewArea(area_id, area_item);
    if (success == false) {
        return false;
    };

    if (ns_trigger::TriggerNpcLinkMatchTable* table = m_pField->GetNpcTriggerEvents()) {
        table->AddEventNpcTriggerAreaEvents(area_id, unit_id);
    };

    return success;
}

bool TriggerManagerEx::OnEnterAreaForNpc(TRIGGER_NPC_ENTER_AREA* msg)
{
    bool success = trigger_area_manager_for_npc_.EnterArea(msg);
    if (success) {
        OnMsg(msg);
    };

    return success;
}

bool TriggerManagerEx::OnLeaveAreaForNpc(TRIGGER_NPC_LEAVE_AREA* msg)
{
    bool success = trigger_area_manager_for_npc_.LeaveArea(msg);
    if (success) {
        OnMsg(msg);
    };

    return success;
}
#endif

bool TriggerManagerEx::CreateAreaForMonster( \
    int area_id, CODETYPE monster_code, TriggerAreaLocalStorageForNpc* area_item)
{
    bool success = trigger_area_manager_for_npc_.InsertNewArea(area_id, area_item);
    if (success == false) {
        return false;
    };

    if (ns_trigger::TriggerNpcLinkMatchTable* table = m_pField->GetNpcTriggerEvents()) {
        table->AddEventMonsterTriggerAreaEvents(area_id, monster_code);
    };

    return success;
}


// NOTE: f110928.3L, prevent the client enter area request hacking loophole.
// USAGES: CheckAreaIdBasedOnPos(id, NULL, &tile_no);CheckAreaIdBasedOnPos(id, &position, NULL);
// WARNING: known problems, the time stamp of client request will be mismatched with server position
bool TriggerManagerEx::CheckAreaIdBasedOnPos(
    const int area_id, const WzVector* check_pos, const int* or_tile_no)
{
    // NOTE: 'm_pField' checked
    if (check_pos == NULL && or_tile_no == NULL) {
        return false;
    };

    WzSpecialArea* special_area = this->GetAreaInfo(area_id);
    if (special_area == NULL) {
        return false;
    };
    const bool used_to_path_tile = \
        (special_area->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX);

    int finding_tile_no = -1;
    //
    ;;;; if ((or_tile_no != NULL) && (used_to_path_tile))
    {
        return false;
    }
    else if ((or_tile_no != NULL))
    {
        finding_tile_no = *or_tile_no;
    }
    else if ((used_to_path_tile == false))
    {   // assert(check_pos != NULL)
        if (special_area->m_wbvRegion.m_eBvType == BVT_AABB)
        {
            const BvAABB& aabb = special_area->m_wbvRegion.m_aabb;
            if (check_pos->x < aabb.m_wvMin.x - 5.0f || aabb.m_wvMax.x + 5.0f < check_pos->x ||
                check_pos->y < aabb.m_wvMin.y - 5.0f || aabb.m_wvMax.y + 5.0f < check_pos->y ||
                check_pos->z < aabb.m_wvMin.z - 5.0f || aabb.m_wvMax.z + 5.0f < check_pos->z)
            {
                return false;
            }
            return true;
        }
        else if (special_area->m_wbvRegion.m_eBvType == BVT_SPHERE)
        {
            const BvSphere& sphere = special_area->m_wbvRegion.m_BvSphere;
            float distance = Math_VectorLength(*check_pos, sphere.m_wvCenter);
            return (distance <= sphere.m_fRadius);
        }

        return false;
    }
    else if (nsAI::MovingPlanner* const moving_planner = m_pField->GetMovingPlanner())
    {   // assert(check_pos != NULL)
        finding_tile_no = moving_planner->GetTileToStand(*check_pos);
        if (finding_tile_no < 0) {
            return false;
        };
    }

    nsAI::MovingPlanner* const moving_planner = m_pField->GetMovingPlanner();
    const BvPathTileIndex& tile_array = special_area->m_wbvRegion.m_BvPathTileIndex;
    const DWORD* tile_it = tile_array.m_pdwData,
               * const tile_end = &tile_array.m_pdwData[tile_array.m_nCount];
    for (; tile_it != tile_end; ++tile_it)
    {
        const nsAI::TileEvent* tile_event;
        nsAI::PlanCmd::TileWeightArg cmd(nsAI::PlanCmd::eTileWeight_GetTileEvent, *tile_it);
        moving_planner->PathPlan_Command(cmd, &tile_event);

        if (tile_event->Index == tile_event->InvalidTileIndex) {
            continue;
        };

        if (tile_event->Index == finding_tile_no) {
            return true;
        };

        for (const WORD* near_it = tile_event->NearNodes;
             near_it != &tile_event->NearNodes[_countof(tile_event->NearNodes)];
             ++near_it)
        {
            if (*near_it == finding_tile_no) {
                return true;
            };
        };
    };
    return false;
};

WzSpecialArea*
TriggerManagerEx::GetAreaInfo(INT index) 
{ 
    if(!IsCanProcess())
        return NULL;

    // IsCanProcess Field관련 처리가 문제 없음을 명시한다.
    FieldInfo* pFieldInfo = m_pField->GetFieldInfo();
    return pFieldInfo->GetAreaInfoByIndex(index); 
}

void
TriggerManagerEx::Update()
{
    if (IsCanProcess() == false) {
        return;
    };
    ns_trigger::ActiveTriggerLink* it = this->active_trigger_list_.next;
    const ns_trigger::ActiveTriggerLink* const end = \
        static_cast<ns_trigger::ActiveTriggerLink*>(&this->active_trigger_list_);
    while (it != end)
    {
        BaseTrigger* trigger = it->trigger;
        assert(trigger);
        // pre-check
        if (trigger->CheckRequestedDeletion())
        {
            trigger->OnRelease();
            trigger->SetActive(false);
            it = util::LList::Delete(it);
            continue;
        };
        //
        if (trigger->Update() == false)
        {
            if (trigger->IsNextActive()) {
                ActiveNextTrigger(trigger->TriggerID());
            };
            if (trigger->CheckRequestedDeletion() == false && trigger->IsLoop())
            {
                trigger->OnRelease();
                trigger->OnInit();
            }
            else
            {
                trigger->OnRelease();
                trigger->SetActive(false);
                it = util::LList::Delete(it);
                continue;
            };
        }; //end if (update)
        // post-check
        if (trigger->CheckRequestedDeletion())
        {
            trigger->OnRelease();
            trigger->SetActive(false);
            it = util::LList::Delete(it);
            continue;
        };
        //
        it = it->next;
    }; //end 'while'
}


void
TriggerManagerEx::SetAttribute(const INT iTileID, const WORD attribute) 
{ 
    if(!IsCanProcess())
        return;

    nsAI::MovingPlanner* const pMovingPlanner = m_pField->GetMovingPlanner();
    pMovingPlanner->SetAttribute(iTileID, attribute);
}

void
TriggerManagerEx::UnsetAttribute(const INT iTileID, const WORD attribute)
{ 
    if(!IsCanProcess())
        return;

    nsAI::MovingPlanner* const pMovingPlanner = m_pField->GetMovingPlanner();
    pMovingPlanner->UnsetAttribute(iTileID, attribute);
}

int
TriggerManagerEx::CreateNPC(WORD monsterCode, DWORD dwNumberOfNPC,
                            WzVector* pvPos, DWORD areaID, WORD wGroupID, int iAngle)
{
    if(!IsCanProcess())
        return 0;

    if(!m_pField->IsMovableCoordinates(pvPos))
    {
        SUNLOG(eCRITICAL_LOG,
               "[TriggerManagerEx::CreateNPC] FieldCode[%u] MonsterCode[%u] areaID[%u] "
               "x[%f] y[%f] z[%f]", 
               m_pField->GetFieldCode(), monsterCode, areaID, pvPos->x, pvPos->y, pvPos->z);
        
        return 0;
    }

    int created_npc_count = 0;
    for(DWORD i = 0; i < dwNumberOfNPC; ++i)
    {
        if (m_pField->CreateMonster(monsterCode,
                                pvPos, wGroupID,
                                (iAngle + 180) % 360, 0, 0, (WORD)(areaID)))
        {
            ++created_npc_count;
        }
    }
    return created_npc_count;
}


WzVector
TriggerManagerEx::GetRandomPosInTile(int iTile)
{
    nsAI::MovingPlanner* const pMovingPlanner = m_pField->GetMovingPlanner();
    return pMovingPlanner->GetRandomPosInTile(iTile);
}

MapNpc*
TriggerManagerEx::FindMapNpc(const DWORD objectKey)
{
    if(!IsCanProcess())
        return NULL;

    return m_pField->FindMapNpc(Conv_MapObjectKey(objectKey));
}

NPC* TriggerManagerEx::FindNPC( const DWORD object_key )
{
    if(!IsCanProcess())
        return NULL;

    return static_cast<NPC*>(m_pField->FindObject(MONSTER_OBJECT, object_key));
}

MapObject*
TriggerManagerEx::FindMapObject(const DWORD objectKey)
{
    if(!IsCanProcess())
        return NULL;

    // 필드는 200000만 대역을 쓴다.
    return m_pField->FindMapObject(Conv_MapObjectKey(objectKey));
}

NPC*
TriggerManagerEx::CreateNPCForUnitID(const DWORD unitID)
{
    if(!IsCanProcess())
        return NULL;

    UnitRegenInfo* pInfo = m_pField->GetUnitRegenMgr()->GetUnitRegenInfoManager()->FindUnit(unitID);
    if(!pInfo)
    {
        SUNLOG(eFULL_LOG, "CreateNPCForUnitID:UnitRegenInfo(%u) is not found", unitID);
        return NULL;
    }

    return m_pField->SpawnMonster(pInfo);
}

void
TriggerManagerEx::DestroyNPCForUnitID(const DWORD unitID)
{
    if(!IsCanProcess())
        return;

    // 리젠되어있는 몬스터 삭제
    NPC* pNPC = m_pField->FindNPCForUnitID(unitID);
    if(NULL != pNPC)
    {
        // 이후 호출되는 업데이트안에서 일괄 삭제 하도록 여기서는 예약만 한다.
        m_pField->DestroyObject(pNPC);
    }
    else
    {
        char temp[10];
        WzIDToString(temp,unitID);
        SUNLOG(eDEV_LOG, "DestroyNPCForUnitID:Unit(%u) not found(%s)", unitID,temp );
    }
}

NPC*
TriggerManagerEx::FindNPCForUnitID(const DWORD unitID)
{
    if(!IsCanProcess())
        return NULL;

    return m_pField->FindNPCForUnitID(unitID);
}

void
TriggerManagerEx::SendToAll(MSG_BASE_FORWARD* pMsg, WORD wSize)
{
    if(!IsCanProcess())
        return;

    m_pField->SendPacketToAll(pMsg, wSize);
}

DWORD
TriggerManagerEx::GetPlayerNum()
{
    if(!IsCanProcess())
        return 0;

    return m_pField->GetPlayerNum();
}

class ClearMissionMessenger
{
public:
    ClearMissionMessenger()
    {
    #ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
        m_bGuildEvent = false;
    #endif
        m_wMapCode = 0;
    }
    ~ClearMissionMessenger() {}
    typedef std::pair<DWORD, Player*> PLAYER_INFO_PAIR;

#ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
private:
    bool m_bGuildEvent;

public:
    void SetGuildEvent(bool bGuildEvent) { m_bGuildEvent = bGuildEvent; }
    bool IsGuidlEvent() { return m_bGuildEvent; }
#endif //_NA_20081121_ABUMAN_NIJOT_EVENT

private:
    MAPCODE m_wMapCode;
public:
    inline void SetMapCode(MAPCODE Mapcode) { m_wMapCode = Mapcode; }

    void ProcessReward(PLAYER_INFO_PAIR rPair)
    {
        Player* pPlayer = rPair.second;

        FlowControl flow;
        FLOWCONTROL_START()
        {
            // start LOGIC_BLOCK
            GameZone* pGameZone = pPlayer->GetGameZonePtr();
            IfFailThenBreak(pGameZone, 10);

            MAPCODE mcCODE = pGameZone->GetMapCode();
            IfFailThenBreak(SAFE_NUMERIC_TYPECAST(MAPCODE, mcCODE, BYTE), 20);

            BYTE missionCode = (BYTE)mcCODE;
            MissionManager* pMGR = pPlayer->GetMissionManager();
            IfFailThenBreak(!pMGR->EnableWindowOpen() &&
                            pMGR->GetMissionState(missionCode) != MISSION_REWARD, 30);

        #ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
            pMGR->SetGuildEvent(m_bGuildEvent);
        #endif

            MissionRewardManager* pMissionMGR = MissionRewardManager::Instance();
            pMissionMGR->Init();

            MissionRewardArgs& rMRA = pMissionMGR->GetMissionRewardArgs();
            rMRA.SetPlayer(pPlayer);
            rMRA.SetMissionManager(pMGR);
            rMRA.SetNum(missionCode);

            // 필드의 모든 사람에게 미션 클리어 정보 저장
            // 중복 보상은 없다!
            WORD wRewardCode = pMissionMGR->GetRewardCode(TRUE);
            IfFailThenBreak(0xFFFF != wRewardCode, 40);

            MSG_CG_TRIGGER_REWARD_WINDOW_CMD msgCMDforReward;
            msgCMDforReward.m_RewardCode = wRewardCode;
            pPlayer->SendPacket(&msgCMDforReward, sizeof(msgCMDforReward));

            ASSERT(pMGR->GetMissionState(missionCode) != MISSION_REWARD);	// hummm...
            pMGR->SetMissionState(missionCode, MISSION_REWARD);
            pMGR->SetMissionPoint(missionCode, pMGR->GetMissionPoint(missionCode)+1);

            ;{
            MSG_CG_MISSION_SET_CLEAR_COUNT_CMD cmd;
            cmd.m_MapCode		= missionCode;
            cmd.m_ClearCount	= pMGR->GetMissionPoint(missionCode);
            pPlayer->SendPacket(&cmd, sizeof(cmd));
            };
            MSG_AG_MISSION_CLEAR_CMD msgCMDforClear;
            msgCMDforClear.m_MissionNo = missionCode;
            pPlayer->SendPacket(&msgCMDforClear, sizeof(msgCMDforClear));

            ;{
                TCHAR tszAddInfo[MAX_ADDINFO_SIZE + 1] = { 0, };
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
                _snprintf(tszAddInfo, MAX_ADDINFO_SIZE, "RewardCode:%d,Class:%u", wRewardCode,
                    (pPlayer->GetCharInfo() != NULL) ? pPlayer->GetCharInfo()->m_byClassCode : eCLASS_NONE);
#else  // _NA_007750_20141113_WZDAT_MODIFY_LOG
                _snprintf(tszAddInfo, MAX_ADDINFO_SIZE, "RewardCode:%d", wRewardCode);
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
                GAMELOG->LogSimpleAction(ACT_MISSION_CLEAR, pPlayer, tszAddInfo);
            };

            SUNLOG(eFULL_LOG,  "[Handler_CG_SYNC::OnCG_SYNC_PLAYER_ENTER_SYN] [ID:%s]:미션 보상창을 띄웠다.", pPlayer->GetUserID());
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
			pPlayer->GetACManager()->IncMissionValue(missionCode);
#ifdef _NA_001990_ACSYSTEM_ADD
            if(AchievementManager* pACManager = pPlayer->GetACManager())
            {
#ifdef _NA_008012_20150130_RANKING_SYSTEM
                if(RankingSystemCtl* pCtl = pPlayer->GetRankingSystemCtl())
                {
                    WORD cleartime = pCtl->GetMissionClearTime();
                    pACManager->ProcessMissionClearTime(mcCODE, cleartime);
                }
#endif // _NA_008012_20150130_RANKING_SYSTEM

                if(GameBattleResult* pBattleResult = pGameZone->GetGameBattleResult())
                {
                    const BattleRecorder::Record* pBattleRecord = pBattleResult->GetPlayerBattleResult(pPlayer);

                    if(pBattleRecord)
                    {
                        pACManager->ProcessBattleResult(mcCODE,
                                                        pBattleRecord->total_heal_to_party,
                                                        pBattleRecord->max_damage_to_enemy,
                                                        pBattleRecord->total_damage_to_enemy,
                                                        pBattleRecord->total_damage_by_enemy,
                                                        pBattleRecord->monster_kill_count,
                                                        pBattleRecord->dead_count);
                    }
                }
            }
            
#endif //_NA_001990_ACSYSTEM_ADD
#endif  
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
            QUEST_MISSION_CLEAR_MSG quest_msg;
            quest_msg.mission_code = missionCode;
            pPlayer->GetQuestManager()->Event(QUEST_EVENT_MISSION_CLEAR, &quest_msg);
#endif //_NA_006607_20130402_ADD_QUESTTYPE
            return /*RETURN TRUE VALUE*/;
        }
        FLOWCONTROL_END;

        switch(flow.GetErrorCode())
        {
        case 10:
            // Sync flow Error
            break;
        case 20: case 30:
            {// Hack?
                SECURELOG(SECURELOG_PLAYERINFO(pPlayer, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_MISSION_REWARD),
                          _T("[ClearMissionMessenger::ProcessReward] Mission Reward Violation"));
            }
            break;
        case 40:
            {
                // 이것은 스크립트의 문제이거나 특수상황의 오버플로일 가능성이 있다.
                MSG_CG_TRIGGER_REWARD_NAK msgNAK;
                msgNAK.m_dwErrorCode = 4121;	// 임시코드
                pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));

                SUNLOG(eCRITICAL_LOG, _T("[ProcessMissionReward] %s"), flow.GetErrorString());
            }
            break;
        }

        //  (FAIL)
        return;
    } // end procedure ProcessReward

    void ProcessMissionRecord(PLAYER_INFO_PAIR rPair)
    {
        Player* pPlayer = rPair.second;

        FlowControl flow;
        FLOWCONTROL_START()
        {
            // start LOGIC_BLOCK
            GameZone* pGameZone = pPlayer->GetGameZonePtr();
            IfFailThenBreak(pGameZone, 10);

            MAPCODE mcCODE = pGameZone->GetMapCode();
            //  (WAVERIX) (081117) (LOGIC-CHANGE)	ranking은 mission만을 위한 것이 아니게 되었다.
            //IfFailThenBreak(SAFE_NUMERIC_TYPECAST(MAPCODE, mcCODE, BYTE), 20);

#ifdef _NA_008012_20150130_RANKING_SYSTEM
            if(RankingSystemCtl* pCtl = pPlayer->GetRankingSystemCtl())
            {
                pCtl->UpdateClearTime(pPlayer);
            }
#endif // _NA_008012_20150130_RANKING_SYSTEM

            return /*RETURN TRUE VALUE*/;
        }
        FLOWCONTROL_END;

        // error control ... -_-;; nai, nai!
    }

    void ProcessACCurseTower(PLAYER_INFO_PAIR rPair)
    {
        Player* pPlayer = rPair.second;
        if(!pPlayer)
            return;

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
		pPlayer->GetACManager()->IncCurseTowerValue(m_wMapCode);
#else
		const std::vector<DWORD>& indices = \
			AreaConquestInfoParser::Instance()->GetIndexByCurseTowerMapCode(m_wMapCode);
		std::vector<DWORD>::const_iterator it, end = indices.end();
		for (it = indices.begin(); it != end; ++it) {
			DWORD index = *it;
			if (index) {
				pPlayer->GetACManager()->IncConditionValue(index, eACC_MISSION_CURSE_TOWER);
			}
		}
#endif
    }

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    void ProcessSunRanking(PLAYER_INFO_PAIR rPair)
    {
        Player* pPlayer = rPair.second;
        if(!pPlayer)
            return;

        GameZone* pGameZone = pPlayer->GetGameZonePtr();
        if(GameBattleResult* pBattleResult = pGameZone->GetGameBattleResult())
        {
            const BattleRecorder::Record* pBattleRecord = pBattleResult->GetPlayerBattleResult(pPlayer);
            if(pBattleRecord)
            {
                SunRankingManager::Instance()->ProcessBattleZone(*pBattleRecord, pPlayer);
            }
        }
    }
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
};


void
TriggerManagerEx::ClearMission()
{
    if(!IsCanProcess())
        return;

    if(GameZone* pGameZone = m_pField->GetGameZone())
    {
        SUNLOG(eFULL_LOG, "미션완료[%u][인원:%u]", pGameZone->GetMapCode(), GetPlayerNum());

        eZONETYPE zoneType = pGameZone->GetZoneType();
        if(zoneType != eZONETYPE_MISSION &&
           zoneType != eZONETYPE_INSTANCE)
        {
            ASSERT(FALSE);
            return;
        }

#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
        const _PLAYER_HASH& players = m_pField->GetGameZone()->GetRefPlayerList();
        _PLAYER_HASH::const_iterator cur_iter = players.begin();
        _PLAYER_HASH::const_iterator end_iter = players.end();
        TCHAR tszAddInfo[MAX_ADDINFO_SIZE + 1] = { 0, };
        for (int idx = 0;cur_iter != end_iter; ++cur_iter, ++idx)
        {
            const Player* player = cur_iter->second;
            if (obj_util::IsValid(player) == false)
            {
                continue;
            }
            _snprintf(tszAddInfo + strlen(tszAddInfo), MAX_ADDINFO_SIZE - strlen(tszAddInfo),
                "C%d:%u,",idx, player->GetCharType());
        }
        _snprintf(tszAddInfo + strlen(tszAddInfo), MAX_ADDINFO_SIZE - strlen(tszAddInfo),
            "MapCode:%u,NumberOfPlayers:%u",pGameZone->GetMapCode(), GetPlayerNum());
        GAMELOG->LogWzDat(ACT_MISSION_CLEAR_TRIGGER, tszAddInfo);
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
        GameRoom* pRoom = static_cast<GameRoom *>(m_pField->GetGameZone());
        if(pRoom)
        {
#ifdef _NA_008012_20150130_RANKING_SYSTEM
            RankingSystemCtl* pRankingSystemCtrl = pRoom->GetRankingSystemCtl();
            if(pRankingSystemCtrl)
                pRankingSystemCtrl->ClearMission(m_pField->GetGameZone());
#endif // _NA_008012_20150130_RANKING_SYSTEM
            ClearMissionMessenger op;
            GameZone* pGameZone = m_pField->GetGameZone();
            //__NA_1288_AC_RENEWAL_FOR_TIME_LINE
            if(m_pField->GetGameZone()->GetZoneType() == eZONETYPE_INSTANCE)
            {
                GameInstanceDungeon* pGameIndun = dynamic_cast<GameInstanceDungeon*>(m_pField->GetGameZone());
                if(pGameIndun)
                {
                    if(pGameIndun->GetTypeOfIndun() == INDUN_KIND_TOP_OF_TRIAL)
                    {
                        op.SetMapCode(pGameIndun->GetMapCode());
                        pGameZone->ForeachPlayer(&ClearMissionMessenger::ProcessACCurseTower, &op);

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
                        pGameZone->ForeachPlayer(&ClearMissionMessenger::ProcessSunRanking, &op);
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
                    }
                }
            }

#ifdef _NA_008012_20150130_RANKING_SYSTEM
            if(pRankingSystemCtrl)
                pGameZone->ForeachPlayer(&ClearMissionMessenger::ProcessMissionRecord, &op);
#endif // _NA_008012_20150130_RANKING_SYSTEM

            if(pGameZone->GetZoneType() == eZONETYPE_MISSION)
            {
            #ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
                // 현재 길드 이벤트 중이면..
                SunEventInfoParser* pEventParser = SunEventInfoParser::Instance();
                EVENT_INFO* pEventInfo = pEventParser->FindEventInfoFromMode(eEVENT_GUILD_MISSION_EVENT);
                if(pEventInfo && pEventInfo->CheckEvent())
                {
                    //모두 같은 길드원이고, 2명 이상이면..
                    if(pGameZone->IsAllSameGuildMember() && pGameZone->GetPlayerNum() >= 2)
                        op.SetGuildEvent(true);
                }
            #endif
                pGameZone->ForeachPlayer(&ClearMissionMessenger::ProcessReward, &op);

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
                pGameZone->ForeachPlayer(&ClearMissionMessenger::ProcessSunRanking, &op);
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
            }
#ifdef _NA_008012_20150130_RANKING_SYSTEM
            if(pRankingSystemCtrl)
                pRankingSystemCtrl->UpdateClearTimeParty(m_pField->GetGameZone());
#endif // _NA_008012_20150130_RANKING_SYSTEM
        }
    }
}

BOOL
TriggerManagerEx::Check_RewardWindow(Player* pPlayer, BOOL bGMSpecial)
{
    if(!pPlayer)
    {
        MessageOutEx(g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_MISSION,
            _T("[TriggerManager::Process_RewardWindow] 정상적인 상황이 아니다."));
        return FALSE;
    }

    if(!bGMSpecial)
    {
        GameZone * pEnterZone = NULL;
        MAPCODE FieldCode = 0;
        WzID AreaID = 0;
        eZONE_STATE eRoomState = pPlayer->GetGameZone(pEnterZone, FieldCode, &AreaID);

        // 보상창 관련이 두 부분으로 구분되었다. 이 루틴은 PRE-zone, next-zone 사이에 검사하는 단계다.
        if(/*ePRS_AT_MISSION*/ ePRS_BEFORE_ENTER_MISSION == eRoomState)
        {
            SECURELOG(SECURELOG_PLAYERINFO(pPlayer, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_MISSION_REWARD),
                _T("[Check_RewardWindow] Mission Reward Violation"));
            return FALSE;
        }
    }

    return TRUE;
}

BOOL
TriggerManagerEx::Process_RewardWindow(Player* pPlayer)
{
    MissionManager* pMGR = pPlayer->GetMissionManager();

    MissionRewardManager* pManager = MissionRewardManager::Instance();
    pManager->Init();

    MissionRewardArgs& rMRA = pManager->GetMissionRewardArgs();
    rMRA.SetPlayer(pPlayer);

    rMRA.SetMissionManager(pMGR);
    rMRA.SetNum(pMGR->ReservedMissionNum());

    ASSERT(pMGR->GetMissionState(pMGR->ReservedMissionNum()) == MISSION_REWARD);
    if(pMGR->GetMissionState(pMGR->ReservedMissionNum()) == MISSION_REWARD)
    {
        WORD wRewardCode = pManager->GetRewardCode(FALSE);
        //ASSERT(0xFFFF != wRewardCode);

        if(0xFFFF != wRewardCode)
        {
            MSG_CG_TRIGGER_REWARD_WINDOW_CMD cmsg;
            cmsg.m_RewardCode = wRewardCode;
            pPlayer->SendPacket(&cmsg, sizeof(cmsg));

            MessageOutEx(g_pSunLog, CSunLog::cFULL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_MISSION,
                _T("[Handler_CG_SYNC::OnCG_SYNC_PLAYER_ENTER_SYN] [ID:%s]:미션 보상창을 띄웠다."), pPlayer->GetUserID());

            return TRUE;
        }
    }

    MSG_CG_TRIGGER_REWARD_NAK nmsg;
    nmsg.m_dwErrorCode = 4121;	// 임시코드
    rMRA.GetPlayer()->SendPacket(&nmsg, sizeof(nmsg));

    MessageOutEx(g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_MISSION,
        _T("[ID:%s][%08u] 미션보상 받을 수 있는 상태가 아닌데 미션 보상창이 열리는 조건이다."),
        pPlayer->GetUserID(), pPlayer->GetUserKey());

    // 보상창이 열리지 않도록 설정 arycoat.2012.02.02
    pMGR->SetMissionState(pMGR->ReservedMissionNum(), MISSION_NONE);

    return FALSE;
}

BOOL
TriggerManagerEx::KillMonster(MONSTERCODE MonsterCode, Player* pPlayer, BOOL IsExpItemDrop)
{
    if(!IsCanProcess())
        return FALSE;

    m_pField->KillMonster(MonsterCode, pPlayer, IsExpItemDrop);

    return TRUE;
}

// added by _NA_0_20100222_UNIT_TRIGGERS_ADD = {
BOOL
TriggerManagerEx::KillOneMonster(NPC* pNpc, Player* pPlayer, BOOL IsExpItemDrop)
{
    if(!IsCanProcess())
        return FALSE;

    m_pField->KillOneMonster(pNpc, pPlayer, IsExpItemDrop);

    return TRUE;
}

MAPOBJECT_INFO* 
TriggerManagerEx::FindMapObjectInfo(const DWORD objectKey)
{
    if(!IsCanProcess())
        return NULL;

    return m_pField->FindMapObjectInfo(Conv_MapObjectKey(objectKey));
}

void
TriggerManagerEx::CreateMapNPC(WORD NPCCode, const WzVector* pvPos, MAPOBJECT_INFO* pMapInfo)
{
    if(!IsCanProcess())
        return;

    m_pField->CreateMapNPC(NPCCode, pvPos, pMapInfo);
}

// }

void
TriggerManagerEx::AddExpAllMember(DWORD64 Exp)
{
    m_pField->AddExpAllMember(Exp);
}

