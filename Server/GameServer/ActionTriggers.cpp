#include "stdafx.h"
#include "./ActionTriggers.h"

//==================================================================================================

#include <Global.h>
#include <PublicMath.h>
#include <3DEngine/Common/MathAssistant.h>

#include <PacketStruct_CG.h>
#include <PacketStruct_AG.h>

#include <SCSlotContainer.h>
#include <SCItemSlotContainer.h>

#include <ItemInfoParser.h>
#include <MapInfoParser.h>

#include <DropRatioInfoList.h>

#include "..\\..\\SolarTrigger\\ActionInfo.h"
#include "TriggerManagerEx.h"

#include "./Trigger.h"
#include "./Player.h"

#include <QuestManager.h>
#include <QuestManager_Concrete.h>

#include "./Map.h"
#include "GameZone.h"
#include "MovingPlanner.h"

#include "ItemManager.h"
#include "./MissionManager.h"
#include "./MissionRewardManager.h"

// 범위 데미지 위해
#include "MapObject.h"
#include "DropManager.h"

// 방향 지정 몬스터 리젠 위해
#include "PacketHandler//Handler_CG_SYNC.h"

#include "GameParty/NewGameParty.h"
#include "GameParty/NewGamePartyManager.h"

#include "StatusManager.h"
#include "GameField.h"
#include "SkillAttrCalculator.h"
#include "GameDominationField.h"
#include "AITypes.h"
#include "TargetManager.h"

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
#include "NPCReplaceInfoParser.h"
#endif 
#include "CollectionStateInfo.h"
#include "CollectionManager.h"
#include "Collection.h"

#include "SummonManager.h"
#include "SummonedNPCs.h"
#include "Summoned.h"

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#include "GameGuild.h"
#include "GameGuildManager.h"
#endif _NA_006826_20130722_DOMINATION_RENEWAL

#include "GameMissionRoom.h"

#ifdef _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
#include "NPCStateManager.h"
#endif // _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER

//==================================================================================================

__IMPL_POOL(ACTION_CHANGE_OBJECTANI)
void
ACTION_CHANGE_OBJECTANI::OnAction()
{
    WzID triggerID = getParent()->TriggerID();
    getMGR()->MAP_STATE.Change(triggerID);
#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG, _T("[TriggerID:%u]:ACTION_CHANGE_OBJECTANI"), triggerID);
#endif
}

//==================================================================================================

__IMPL_POOL(ACTION_CHANGE_PATHTILE)
void
ACTION_CHANGE_PATHTILE::OnInit()
{
}

void
ACTION_CHANGE_PATHTILE::OnAction()
{
    const ACTION_CHANGE_PATHTILE_INFO* pInfo = Get();
    const BYTE attribute = pInfo->GetAttribute();
    TriggerManagerEx* pTriggerManager = getMGR();
    if(attribute == ns_trigger::CANMOVE)
        pTriggerManager->UnsetAttribute(pInfo->GetTileID(), PTA_NO_WALK);
    if(attribute == ns_trigger::CANNOTMOVE)
        pTriggerManager->SetAttribute(pInfo->GetTileID(), PTA_NO_WALK);

    pTriggerManager->MAP_STATE.Change(getParent()->TriggerID());

#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG,  "[TriggerID:%u]:ACTION_CHANGE_PATHTILE", getParent()->TriggerID());
#endif
}


//==================================================================================================


__IMPL_POOL(ACTION_CHANGE_OBJECTSTATE)
void
ACTION_CHANGE_OBJECTSTATE::OnAction()
{
    const ACTION_CHANGE_OBJECTSTATE_INFO* pInfo = Get();
    int object_id = pInfo->GetObjectID();
    MapObject* pMapObject = getMGR()->FindMapObject(object_id);
    DEBUG_CODE(if(!pMapObject) return ;);

    switch(pInfo->GetState())
    {
    case ACTION_CHANGE_OBJECTSTATE_INFO::APPEAR:
        {
        }
        break;
    case ACTION_CHANGE_OBJECTSTATE_INFO::DISAPPEAR:
        {
        }
        break;
    case ACTION_CHANGE_OBJECTSTATE_INFO::REMOVE:
        {
            SUNLOG(eCRITICAL_LOG,
                   "["__FUNCTION__"] [ID:%u]:ACTION_CHANGE_OBJECTSTATE is Failed!!",
                   getParent()->TriggerID());
        }
        break;
    }

    getMGR()->MAP_STATE.Change(getParent()->TriggerID());

#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG,  "[TriggerID:%u]:ACTION_CHANGE_OBJECTSTATE", getParent()->TriggerID());
#endif
}


//==================================================================================================


__IMPL_POOL(ACTION_PLAY_EVENT)
void
ACTION_PLAY_EVENT::OnAction()
{
    // 이벤트를 플레이하라고 알려라!
    DWORD clicked_player_key = getParent()->GetClickedPlayerKey();
    Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
    if(pPlayer == 0)
    {
        SUNLOG(eFULL_LOG, "[ACTION_PLAY_EVENT::OnAction] [U:%d]Player NULL!!", clicked_player_key);
        return;
    }
#if SUN_CODE_BACKUP
    // (f100721.2L) remove code
    //MSG_CG_TRIGGER_PLAY_EVENT_CMD msg;
    //msg.m_nEventIndex = Get()->GetEventID();
    //pPlayer->SendPacket(&msg, sizeof(msg));
    // (f100721.2L) add routine to present a cinematic effect
    getMGR()->MAP_STATE.Change(getParent()->TriggerID());
#endif

#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG,  "[TriggerID:%u]:ACTION_PLAY_EVENT", getParent()->TriggerID());
#endif
}


//==================================================================================================

class ActionSetAttack
{
public:
    BYTE m_byTargetType;
    BOOL m_bAttack;

    ActionSetAttack(BYTE byTargetType, BOOL bAttack)
        : m_byTargetType(byTargetType), m_bAttack(bAttack) {}
    void operator()(Player* pPlayer)
    {
        // (WAVERIX) (090614) pStatusManager->Remove의 사용은 좀 고민해봐야 할 듯 ...
        StatusManager* pStatusManager = pPlayer->GetStatusManager();
        if(m_byTargetType == ACTION_SET_ISATTACK_INFO::TARGET_TYPE_USER || 
           m_byTargetType == ACTION_SET_ISATTACK_INFO::TARGET_TYPE_ALL)
        {
            if(m_bAttack == ACTION_SET_ISATTACK_INFO::CAN_NOT_ATTACK)
                pStatusManager->AllocStatus(eCHAR_STATE_ETC_DISABLE_VISION_TRIGGER);
            else
                pStatusManager->Remove(eCHAR_STATE_ETC_DISABLE_VISION_TRIGGER);
        }

        if(m_byTargetType == ACTION_SET_ISATTACK_INFO::TARGET_TYPE_MONSTER || 
           m_byTargetType == ACTION_SET_ISATTACK_INFO::TARGET_TYPE_ALL)
        {
            if(m_bAttack == ACTION_SET_ISATTACK_INFO::CAN_NOT_ATTACK)
                pStatusManager->AllocStatus(eCHAR_STATE_BLUR_TRIGGER);
            else
                pStatusManager->Remove(eCHAR_STATE_BLUR_TRIGGER);
        }
    }

    void SetAttack(SATISFIED_PLAYER_PAIR rPair)
    {
        operator()(rPair.second);
    }
};

//==================================================================================================

// 특정 유저(A)를 대상으로 A가 다른 객체 B를 공격 할수 없도록 만들거나,
// B 객체로 부터 공격 당하지 않게 만든다.
__IMPL_POOL(ACTION_SET_ISATTACK)
void
ACTION_SET_ISATTACK::OnAction()
{
    const ACTION_SET_ISATTACK_INFO* pInfo = Get();
    ActionSetAttack op(pInfo->GetTargetType(), pInfo->GetIsAttack());

    Trigger* pTrigger = getParent();
    if(DWORD clicked_player_key = pTrigger->GetClickedPlayerKey())
    {
        // pPlayer == 0인 조건이 생길 수 있다. 조건 추가한다.
        if(Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key))
            op(pPlayer);
    }
    else
    {
        pTrigger->ForeachSatisfiedPlayer(&ActionSetAttack::SetAttack, &op);
        pTrigger->RemoveAllSatisfiedPlayer();
    }

#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG, "[TriggerID:%u]:ACTION_SET_ISATTACK", pTrigger->TriggerID());
#endif
}


//==================================================================================================

class WARP_PLAYER
{
public:
    static void Warp(Player* pPlayer, MAPCODE enterFieldCode, WzID areaID)
    {
        GameZone* pZone = pPlayer->GetGameZonePtr();
        if(pZone == 0)
            return;

        //  (WAVERIX)(080610)(BUG-FIX) - 서버 강제 이동시 (죽은 상태일 경우) 발생되는 문제 해결 목적
        //  - related info (080604)
        if(pPlayer->GetHP() == 0)
            pPlayer->SetHP(1);

        pZone->Warp(pPlayer, enterFieldCode, areaID);

        MSG_CG_TRIGGER_PORTAL_ACK msg;
        msg.m_LeaveType = msg.eLeaveType_Default;
        msg.m_MapCode = pZone->GetMapCode();
        msg.m_FieldCode = enterFieldCode;
        msg.Encode();
        pPlayer->SendPacket(&msg, sizeof(msg));
        return;
    }
};


class WarpControl
{
    typedef GameField::_PLAYER_HASH GF_PLAYER_LIST;
public:
    // Iterator 무결성 문제로 재작성
    static void WarpAllPlayer(GameField* const pField, const MAPCODE fieldCode, const WzID areaID)
    {
        const GF_PLAYER_LIST& rPlayers = pField->GetRefPlayerList();
        GF_PLAYER_LIST::const_iterator it = rPlayers.begin();
        while(it != rPlayers.end())
        {
            Player* const pPlayer = it->second;
            ++it; // 아래 주석 참조.
            if (obj_util::IsValid(pPlayer, __FUNCTION__, __LINE__) == false)
            {
                continue;
            }
            WARP_PLAYER::Warp(pPlayer, fieldCode, areaID);
        }
        /*
        // 저 위에서 말한 iterator 무결성 문제란..
        // WARP_PLAYER::Warp(...);
        //  => pZone->Warp(...);
        //  ==> pEnterZone->LeaveForWarp(...);
        //  ===> GameZone::Leave(...);
        //       {
        //         ...
        //         m_PlayerHash.erase(it);  <= 이거 때문에 for 문을 사용하면 안됨.
        //       }
        */
    }

};

class BackToTheVillage
{
public:
    void operator()(Player* pPlayer) 
    { 
        MSG_AG_ZONE_MISSION_LEAVE_CMD msg;
        msg.m_LeaveType = msg.eLeaveType_Default;
        msg.m_ReturnMapCode = 0;
        pPlayer->SendPacket(&msg, sizeof(msg));
    }
    void operator()(std::pair<DWORD,Player *> pair_type)
    {
        operator()(pair_type.second);
    }
};

//__NA001310_BATTLEZONE_PORTAL_RETURN_TARGET_MOVE_SUPPORT__
class ReturnToTargetStaticField
{
public:
    ReturnToTargetStaticField(MAPCODE returnMapCode, FIELDCODE enterFieldCode, WzID areaID)
        : m_ReturnMapCode(returnMapCode)
        , m_EnterFieldCode(enterFieldCode)
        , m_AreaID(areaID)
    {}
    ~ReturnToTargetStaticField() {}

    void operator()(Player* pPlayer)
    {
        pPlayer->SetStartMapCode(m_ReturnMapCode);
        {
            MSG_CG_TRIGGER_PORTAL_ACK msg;
            msg.m_LeaveType = msg.eLeaveType_Target;
            msg.m_MapCode   = m_ReturnMapCode;
            msg.m_FieldCode = m_EnterFieldCode;
            msg.Encode();
            pPlayer->SendPacket(&msg, sizeof(msg));
        }
        {
            MSG_AG_ZONE_MISSION_LEAVE_CMD msg;
            msg.m_LeaveType = msg.eLeaveType_Target;
            msg.m_ReturnMapCode = m_ReturnMapCode;
            pPlayer->SendPacket(&msg, sizeof(msg));
        }
    }
    void operator()(std::pair<DWORD, Player*> pair_type)
    {
        operator()(pair_type.second);
    }
private:
    const MAPCODE m_ReturnMapCode;
    const FIELDCODE m_EnterFieldCode;
    const WzID m_AreaID;
};
//

class WarpPlayerForField
{
    MAPCODE m_FieldCode;
    WzID m_AreaID;
public: 
    WarpPlayerForField(MAPCODE fieldCode, WzID areaID) : m_FieldCode(fieldCode),m_AreaID(areaID) {}
    ~WarpPlayerForField() {}

    void operator()(Player* pPlayer)
    { 
        WARP_PLAYER::Warp(pPlayer, m_FieldCode, m_AreaID);
    }

    void operator()(std::pair<DWORD,Player *> pair_type)
    { 
        operator()(pair_type.second);
    }
};


__IMPL_POOL(ACTION_PORTAL_PLAYER)
void
ACTION_PORTAL_PLAYER::OnAction()
{
    Trigger* pTrigger = getParent();
    ACTION_PORTAL_PLAYER_INFO* pInfo = Get();
    MAPCODE enterFieldCode = static_cast<MAPCODE>(pInfo->GetMapCode());
    int moveType = pInfo->GetType();

    switch(moveType)
    {
    case pInfo->FIELDCODE:
        {
            if(pInfo->GetIsParty() == ns_trigger::PARTY)
            {
                // 현재 필드에 있는 사람을 모두 워프
                // 조건에 해당하는 사람을 알 필요 없음
                const WzID areaID = pInfo->GetAreaID();
                WarpControl::WarpAllPlayer(getMGR()->GetField(), enterFieldCode, areaID);
            }
            else
            {
                ASSERT(pInfo->GetIsParty() == ns_trigger::PERSONAL);

                if(DWORD clicked_player_key = pTrigger->GetClickedPlayerKey())
                {
                    Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
                    if(pPlayer == 0)
                    {
                        SUNLOG(eFULL_LOG,
                               "["__FUNCTION__"] [U:%d]ACTION_REWARD_PLAYER Player NULL!!",
                               clicked_player_key);
                        return;
                    }
                    WARP_PLAYER::Warp(pPlayer, enterFieldCode, pInfo->GetAreaID());
                }
                else
                {
                    /*
                    WarpPlayerForCondition op(enterFieldCode,Get()->GetAreaID());
                    getParent()->FindPlayer(op);
                    */
                    WarpPlayerForField op(enterFieldCode, pInfo->GetAreaID());
                    pTrigger->ForeachSatisfiedPlayer(op);
                    pTrigger->RemoveAllSatisfiedPlayer();
                }
            }
        }
        break;
        // 기존 코드가 선행검사(FIELDCODE == moveType)이냐 아니냐로 평가 하는 바람에 3가지 타입
        //이 실재로 2가지 타입으로 사용되고 있었다. 별 수 없이 동일하게 처리한다.
    case pInfo->RELATEDVILLAGE:
    case pInfo->VILLAGECODE:
        {
            if(pInfo->GetIsParty() == ns_trigger::PARTY)
            {
                // 현재 필드에 있는 사람을 모두 워프
                // 조건에 해당하는 사람을 알 필요 없음
                BackToTheVillage op;
                getMGR()->ForeachPlayer(op);
                //WarpPlayerForField op(enterFieldCode,Get()->GetAreaID());
            }
            else
            {
                ASSERT(pInfo->GetIsParty() == ns_trigger::PERSONAL);

                if(DWORD clicked_player_key = pTrigger->GetClickedPlayerKey())
                {
                    Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
                    if(pPlayer == 0)
                    {
                        SUNLOG(eFULL_LOG,
                               "["__FUNCTION__"] [U:%d]ACTION_REWARD_PLAYER Player NULL!!",
                               pTrigger->GetClickedPlayerKey());
                        return;
                    }

                    BackToTheVillage op;
                    op(pPlayer);
                }
                else
                {
                    BackToTheVillage op;
                    pTrigger->ForeachSatisfiedPlayer(op);
                    pTrigger->RemoveAllSatisfiedPlayer();
                }
            }
        }
        break;
    //__NA001310_BATTLEZONE_PORTAL_RETURN_TARGET_MOVE_SUPPORT__
    case pInfo->TARGET_RETURN:
        {
            BOOLEAN bParty = pInfo->GetIsParty() == ns_trigger::PARTY;
            DWORD clicked_player_key = bParty ? 0 : pTrigger->GetClickedPlayerKey();

            MAPCODE returnMapCode = enterFieldCode; //이 시점은 FieldCode가 아니다. MapCode
            // 아래 pMapInfo에서 뻗는다면 테스트 시점 반드시 발견된다.
            const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(returnMapCode);
            BOOLEAN acceptableField = pMapInfo->byMKind == eZONETYPE_VILLAGE ||
                                      pMapInfo->byMKind == eZONETYPE_FIELD ||
                                      pMapInfo->byMKind == eZONETYPE_DOMINATION_FIELD ||
                                      pMapInfo->byMKind == eZONETYPE_TUTORIAL_FIELD;

            if(acceptableField == 0)
                break;

            enterFieldCode = pMapInfo->FCode[0];
            ReturnToTargetStaticField op(returnMapCode, enterFieldCode, 0);
            //3rd Arg: pInfo->GetAreaID() < 지원 여부 고민하자.
            if(bParty)
            {
                getMGR()->ForeachPlayer(op);
            }
            else if(clicked_player_key)
            {
                Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
                if(pPlayer == NULL)
                {
                    SUNLOG(eFULL_LOG, "["__FUNCTION__"] [U:%d] Player NULL!!", clicked_player_key);
                    return;
                }
                op(pPlayer);
            }
            else
            {
                pTrigger->ForeachSatisfiedPlayer(op);
                pTrigger->RemoveAllSatisfiedPlayer();
            }
        }
        break;
    }
#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG,
           "[TriggerID:%u]:ACTION_PORTAL_PLAYER",
           pTrigger->TriggerID());
#endif
}


//==================================================================================================


__IMPL_POOL(ACTION_SET_OBJECTTHRUST)
void
ACTION_SET_OBJECTTHRUST::OnAction()
{
}


//==================================================================================================

// 조금 이상한 트리거지만 기획에서...
// 전체 유저에게 데미지를 준다. (영역데미지에서 영역이 빠진 버전)

__IMPL_POOL(ACTION_APPLY_DAMAGE)

class ApplyDamageHandler
{
public:
    ApplyDamageHandler(DAMAGETYPE damage) : damage_(damage) {}
    ~ApplyDamageHandler(){}

    void operator()(Player* pPlayer) 
    { 
        ASSERT(pPlayer);

        DAMAGETYPE apply_damage = static_cast<DAMAGETYPE>(pPlayer->DecreaseHP(damage_));

        SUNLOG(eDEV_LOG, "데미지 부여 : 트리거 데미지[%d] 실제 피해 데미지[%d] 플레이어 hp[%d]",
            damage_, apply_damage, pPlayer->GetHP());

        MSG_CG_TRIGGER_AREADAMAGE_BRD msg;
        msg.m_dwObjectKey = pPlayer->GetObjectKey();
        msg.m_wDamage     = damage_;
        msg.m_dwHP        = pPlayer->GetHP();

        pPlayer->SendPacketAround((MSG_BASE_FORWARD*)&msg, sizeof(msg));
    }

    void operator()(std::pair<DWORD,Player *> pair_type)
    {
        operator()(pair_type.second);
    }

private:
    DAMAGETYPE damage_;
};

void
ACTION_APPLY_DAMAGE::OnAction()
{
    ACTION_APPLY_DAMAGE_INFO* pInfo = Get();

    ApplyDamageHandler op((DAMAGETYPE)pInfo->GetDamage());

    const BYTE byTarget = pInfo->GetTargetType(); // (CHANGES) (f100608.1L) (WAVERIX) add type
    switch(byTarget)
    {
    case ACTION_APPLY_DAMAGE_INFO::TARGET_FIELD:
        getMGR()->ForeachPlayer(op);
        break;

    case ACTION_APPLY_DAMAGE_INFO::TARGET_PERSONAL:
        {
            Player*  pPlayer  = NULL;
            Trigger* pTrigger = getParent();
            DWORD clickPlayer = pTrigger->GetClickedPlayerKey();

            if (clickPlayer)
                pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clickPlayer);
            else
                pPlayer = pTrigger->GetFirstSatisfiedPlayer();

            if (pPlayer)
                op(pPlayer);
            else
                SUNLOG(eDEV_LOG, "[TriggerID:%u]:ACTION_APPLY_DAMAGE : Cant not Find Player",
                                 pTrigger->TriggerID());
        }
        break;
    }
}

//==================================================================================================

__IMPL_POOL(ACTION_REFLECT_DAMAGE)
void
ACTION_REFLECT_DAMAGE::OnAction()
{
}

//==================================================================================================
namespace ns_trigger {

struct CreateMonsterArgs
{
    enum { MAX_NUMBER_OF_MONSTERS = 100, };
    int area_id_;
    int group_id_;
    int number_of_monsters_;
    struct MonsterInfo {
        MONSTERCODE monster_code_;
        short angle_;
    } monters_list_[MAX_NUMBER_OF_MONSTERS];

    inline void Clear() {
        ZeroMemory(this, sizeof(*this));
    }
    inline void Init(int area_id, int group_id) {
        area_id_ = area_id;
        group_id_ = group_id;
    }
    inline void AddInfo(MONSTERCODE monster_code, short angle) {
        if(number_of_monsters_ >= _countof(monters_list_))
            return;
        MonsterInfo& node = monters_list_[number_of_monsters_++];
        node.monster_code_ = monster_code;
        node.angle_ = angle;
    }
};

static int /*number of regened*/
CreateMonsterByTrigger(TriggerManagerEx* pTriggerManager, CreateMonsterArgs* args)
{
    WzSpecialArea* pAreaInfo = pTriggerManager->GetAreaInfo(args->area_id_);
    nsAI::MovingPlanner* pMovingPlanner = pTriggerManager->GetField()->GetMovingPlanner();
    //assert(pAreaInfo && pGameField);
    //assert(args->number_of_monters > 0);
    int number_of_regened = 0;
    const ENUM_BVTYPE bounding_volume_type = pAreaInfo->m_wbvRegion.m_eBvType;
    if(0) {
    }
    else if(bounding_volume_type == BVT_PATH_TILE_INDEX)
    {
        BvPathTileIndex& rTileIndex = pAreaInfo->m_wbvRegion.m_BvPathTileIndex;

        const WORD number_of_max_tiles = min(WORD(rTileIndex.m_nCount), args->MAX_NUMBER_OF_MONSTERS);
        //assert(number_of_max_tiles != 0);
        WORD number_of_tiles = 0;
        WORD tile_list[args->MAX_NUMBER_OF_MONSTERS] = { 0, };
        // tile은 word값을 넘지 않는다. 문제가 생기면 MovingPlanner에서 판단 가능.

        DWORD* tile_it = rTileIndex.m_pdwData;
        for(DWORD loop = number_of_max_tiles; loop; --loop, ++tile_it)
        {
            const DWORD tile_id = *tile_it;
            WORD tile_attr = pMovingPlanner->GetAttribute(tile_id);
            if(tile_attr & PTA_NO_WALK)
                continue;
            tile_list[number_of_tiles++] = WORD(tile_id);
        }

        if (number_of_tiles == 0) {
            SUNLOG(eCRITICAL_LOG, "[%s:%d] number_of_tiles == 0", __FUNCTION__, __LINE__);

            return 0;
        }

        CreateMonsterArgs::MonsterInfo* pMonsterInfo = args->monters_list_;
        //assert args->number_of_monsters_ <= args->MAX_NUMBER_OF_MONSTERS
        WORD indexes_of_tiles = number_of_tiles - 1; //if assert failed then '-1' protection fault
        WzVector spawn_pos;

        for(DWORD loop = args->number_of_monsters_; loop; --loop)
        {
            DWORD random_index = random(0, indexes_of_tiles);
            pMovingPlanner->GetRandomPosInTile(tile_list[random_index], &spawn_pos);
            number_of_regened += \
                pTriggerManager->CreateNPC(pMonsterInfo->monster_code_, 1, &spawn_pos,
                                       args->area_id_, args->group_id_, pMonsterInfo->angle_);
            ++pMonsterInfo;
        }
    }
    else if(bounding_volume_type == BVT_SPHERE)
    {
        BvSphere& bvSphere = pAreaInfo->m_wbvRegion.m_BvSphere;
        WORD tile_attr;
        const int tile_no_1st =
            pMovingPlanner->GetTileToStand(bvSphere.m_wvCenter, 0, -1.0f, -1.0f, &tile_attr);
        static const float ratio_x = g_SinTable.FCOS(45);
        static const float ratio_y = g_SinTable.FSIN(45);

        int tile_no = -1;
        WzVector selected_pos;
        if((tile_attr & PTA_NO_WALK) == 0)
        {
            selected_pos = bvSphere.m_wvCenter;
            tile_no = tile_no_1st;
        }
        else
        {
            SetVector(&selected_pos,
                      ratio_x * bvSphere.m_fRadius, ratio_y * bvSphere.m_fRadius, 0.0f);
            ViewBoxCubic cubic;
            cubic.SetInit3D(&bvSphere.m_wvCenter, &selected_pos);
            WzVector& max_point = cubic.Cubic.m_wvMax;
            WzVector& min_point = cubic.Cubic.m_wvMin;
            
            WzVector points[4] = {
                { max_point.x, max_point.y, bvSphere.m_wvCenter.z },
                { max_point.x, min_point.y, bvSphere.m_wvCenter.z },
                { min_point.x, max_point.y, bvSphere.m_wvCenter.z },
                { min_point.x, min_point.y, bvSphere.m_wvCenter.z },
            };
            for(int i = 0; i < 4; ++i)
            {
                CopyMemory(&selected_pos, &points[i], sizeof(selected_pos));
                tile_no = pMovingPlanner->GetTileToStand(selected_pos, 0, -1.0f, -1.0f, &tile_attr);
                if(tile_no_1st != tile_no && (tile_attr & PTA_NO_WALK) == 0)
                    if(tile_no >= 0)
                        break;
            }
        }

        for (int count = (args->number_of_monsters_ - 1); count >= 0; --count)
        {
            CreateMonsterArgs::MonsterInfo& monster_info = args->monters_list_[count];
            number_of_regened = \
                pTriggerManager->CreateNPC(monster_info.monster_code_, 1, &selected_pos,
                args->area_id_, args->group_id_, monster_info.angle_);
        }
    }
    else
    {
        //...
    }
    //pTriggerManager->CreateNPC(pInfo->GetMonsterCode(), 1, &vPos, areaID, pInfo->GetType());
    return number_of_regened;
}

}; //end of namespace 'ns_trigger'
//==================================================================================================


__IMPL_POOL(ACTION_CREATE_MONSTER)
void
ACTION_CREATE_MONSTER::OnAction()
{
    m_iRegenCount = 0;
    m_RegenTimer.SetTimer(0);   //< 처음에는 반드시 한번 실행한다.
    ACTION_CREATE_MONSTER_INFO* pInfo = Get();
    if(pInfo->GetIsParty() == ns_trigger::PERSONAL)
    {
        // 아직 유저가 맵에 들어 오지 않은상황에서 0이 될 수 있다.!!!!!@@@@
        DWORD number_of_players = getMGR()->GetPlayerNum();
        m_iMaxCreateCount = pInfo->GetCreateCount() * (number_of_players == 0 ? 1 : number_of_players);
    }
    else
        m_iMaxCreateCount = pInfo->GetCreateCount();

#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG, "[TriggerID:%u]:ACTION_CREATE_MONSTER, 개수(%d)",
           getParent()->TriggerID(), m_iMaxCreateCount);
#endif
}


BOOL
ACTION_CREATE_MONSTER::OnActionUpdate()
{
    const ACTION_CREATE_MONSTER_INFO* pInfo = Get();
    if(m_iRegenCount == pInfo->GetLoopCount()) 
    {
        return FALSE;
    }

    if(m_RegenTimer.IsExpired() == 0)
        return TRUE;

    TriggerManagerEx* pTriggerManager = getMGR();
    const int areaID = pInfo->GetAreaID();

    m_RegenTimer.SetTimer(pInfo->GetDelayTime());

    ns_trigger::CreateMonsterArgs args;
    args.Clear();

    args.Init(areaID, pInfo->GetType());
    MONSTERCODE monsterCode = (MONSTERCODE)pInfo->GetMonsterCode();

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    GameZone* const game_zone = getMGR()->GetField()->GetGameZone();
    if(game_zone->GetZoneType() == eZONETYPE_MISSION) 
    {
        const MONSTERCODE replaace_monster_code = \
            AppliedNpcDependencyChain::Instance()->GetRepleaceMonsterCode(game_zone, monsterCode, true);
        
        if (replaace_monster_code) {
            monsterCode = replaace_monster_code;
        }
    }
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

    for(int loop = m_iMaxCreateCount; loop; --loop)
        args.AddInfo(monsterCode, -1);

    ns_trigger::CreateMonsterByTrigger(pTriggerManager, &args);
    ++m_iRegenCount;

    return TRUE;
}


//==================================================================================================

class DisPlayMsgBoxOpr
{
public:     
    DisPlayMsgBoxOpr(MSG_CG_TRIGGER_DISPLAY_MSGBOX_CMD& rMsgCmd, BYTE byTarget)
        : m_rMsgCmd(rMsgCmd) 
        , m_byTarget(byTarget) {}

    ~DisPlayMsgBoxOpr(){}

    void operator()(Player* pPlayer) 
    { 
        if (ACTION_DISPLAY_MSGBOX_INFO::TARGET_FIELD == m_byTarget)
        {
            pPlayer->SendPacket(&m_rMsgCmd, sizeof(m_rMsgCmd));
            return;
        }
        
        GUILDGUID guildGuid = pPlayer->GetGuildGuid();
        GameZone* pZone = pPlayer->GetGameZonePtr();

        if (!pZone || 
            INVALID_GUILDGUID == guildGuid ||
            eZONETYPE_DOMINATION_FIELD != pZone->GetZoneType())
            return;

        GameDominationField* pDomiZone = (GameDominationField*)pZone;
        
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
		GUILDGUID defence_guild_guid = pDomiZone->GetDefenseGuild();
		GameGuild* const defense_game_guild = g_GameGuildManager.FindGuild(defence_guild_guid);

		bool is_defence_guild = false; // 방어측 길드인지

		if (defense_game_guild == NULL)
		{
			// 방어길드가 존재하지 않으면 여기있는 사람들은 모두 공격측.
			is_defence_guild = false;
		}
		else
		{
			if (pPlayer->GetGuildGuid() == defence_guild_guid)
			{
				is_defence_guild = true;
			}
			else
			{
				GuildRelationSystem::GuildRelationEntry* defense_guild_entry = \
					defense_game_guild->GetRelationEntry();
				eGUILD_RELATION player_relation = defense_guild_entry->GetRelationType(pPlayer->GetGuildGuid());
				if (player_relation == eGUILD_RELATION_ALLIENCE)
				{
					is_defence_guild = true;
				}
				else
				{
					is_defence_guild = false;
				}
			}
		}

		// 공성,수성 여부 파악해서 메세지 전달
		switch(m_byTarget)
		{
		case ACTION_DISPLAY_MSGBOX_INFO::TARGET_OFFENCE:
			if (is_defence_guild == false)
				pPlayer->SendPacket(&m_rMsgCmd, sizeof(m_rMsgCmd));
			break;

		case ACTION_DISPLAY_MSGBOX_INFO::TARGET_DEFENCE:
			if (is_defence_guild == true)
				pPlayer->SendPacket(&m_rMsgCmd, sizeof(m_rMsgCmd));
			break;
		}
#else
		switch(m_byTarget)
		{
		case ACTION_DISPLAY_MSGBOX_INFO::TARGET_OFFENCE:
			if (guildGuid == pDomiZone->GetOffenseGuild())
				pPlayer->SendPacket(&m_rMsgCmd, sizeof(m_rMsgCmd));
			break;

		case ACTION_DISPLAY_MSGBOX_INFO::TARGET_DEFENCE:
			if (guildGuid == pDomiZone->GetDefenseGuild())
				pPlayer->SendPacket(&m_rMsgCmd, sizeof(m_rMsgCmd));
			break;
		}

#endif // _NA_006826_20130722_DOMINATION_RENEWAL
    }
    void operator()(std::pair<DWORD,Player *> pair_type)
    {
        operator()(pair_type.second);
    }

private:
    BYTE m_byTarget;
    MSG_CG_TRIGGER_DISPLAY_MSGBOX_CMD& m_rMsgCmd;
};

__IMPL_POOL(ACTION_DISPLAY_MSGBOX)
void
ACTION_DISPLAY_MSGBOX::OnAction()
{
    const ACTION_DISPLAY_MSGBOX_INFO* pInfo = Get();

    const BYTE byTarget   = pInfo->GetTarget();
    const BYTE byPersonal = ACTION_DISPLAY_MSGBOX_INFO::TARGET_PERSONAL;   

    MSG_CG_TRIGGER_DISPLAY_MSGBOX_CMD msg;
    msg.m_nTextID = pInfo->GetTextID();

    if (byPersonal == byTarget)
    {
        Player*  pPlayer  = NULL;
        Trigger* pTrigger = getParent();
        DWORD clickPlayer = pTrigger->GetClickedPlayerKey();

        if (clickPlayer)
            pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clickPlayer);
        else
            pPlayer = pTrigger->GetFirstSatisfiedPlayer();


        if (pPlayer)
            pPlayer->SendPacket(&msg, sizeof(msg));
        else
            SUNLOG(eDEV_LOG, "[TriggerID:%u]:ACTION_DISPLAY_MSGBOX : Cant not Find Player",
            pTrigger->TriggerID());

        return;
    }

    DisPlayMsgBoxOpr op(msg,byTarget);
    getMGR()->ForeachPlayer(op);
}

//==================================================================================================

class RemoveItemFromInventoryForCondition
{
    SLOTCODE m_ItemCode;
    int      m_iCount;
public:
    RemoveItemFromInventoryForCondition(SLOTCODE itemCode, int count)
        : m_ItemCode(itemCode), m_iCount(count) {}
    ~RemoveItemFromInventoryForCondition() {}

    BOOL operator()(Player* pPlayer)
    {
        ASSERT(SAFE_NUMERIC_TYPECAST(int, m_ItemCode, SLOTCODE));

        ItemManager* pItemMan = pPlayer->GetItemManager();
        SLOTCODE slotCode = (SLOTCODE)m_ItemCode;
        POSTYPE pos = m_iCount;

        MSG_CG_ITEM_LOSE_ACK msg;
        if(RC::RC_ITEM_SUCCESS == pItemMan->DeleteAllKindOfItem(1, &slotCode, &pos, &(msg.m_ItemInfo)))
        {
            if(msg.m_ItemInfo.m_Count != 0)
                pPlayer->SendPacket(&msg, msg.GetSize());
            return TRUE;
        }

        return FALSE;
    }

    //BOOL operator()(Trigger::PLAYER_TRIGGER_INFO* pPlayerInfo)
    //{
    //  //if(pPlayerInfo->m_State)
    //  {
    //      Player* pPlayer = pPlayerInfo->m_pPlayer;
    //      pPlayer->GetItemManager()->Lose(m_ItemCode, m_byAllOfItem);
    //  }
    //  return FALSE;
    //}
};


//==================================================================================================


__IMPL_POOL(ACTION_REMOVE_ITEM)
void
ACTION_REMOVE_ITEM::OnAction()
{
    const DWORD clicked_player_key = getParent()->GetClickedPlayerKey();
    if(clicked_player_key != 0)
    {
        Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
        if(pPlayer != 0)
        {
            int iItemCode = Get()->GetItemCode();
            ASSERT(SAFE_NUMERIC_TYPECAST(int, iItemCode, SLOTCODE));

            ItemManager* pItemMan = pPlayer->GetItemManager();
            SLOTCODE slotCode = (SLOTCODE)iItemCode;
            POSTYPE pos = Get()->GetItemNum();

            MSG_CG_ITEM_LOSE_ACK msg;
            if(RC::RC_ITEM_SUCCESS == pItemMan->DeleteAllKindOfItem(1, &slotCode, &pos, &(msg.m_ItemInfo)))
            {
                if(msg.m_ItemInfo.m_Count != 0)
                    pPlayer->SendPacket(&msg, msg.GetSize());
            }
        }
        else
        {
            SUNLOG(eFULL_LOG, "[U:%d]ACTION_REMOVE_ITEM Player NULL!!", clicked_player_key);
            return;
        }
    }
    else
    {
        RemoveItemFromInventoryForCondition op(Get()->GetItemCode(), Get()->GetItemNum());
        getParent()->ForeachSatisfiedPlayer(op);
        getParent()->RemoveAllSatisfiedPlayer();
    }

#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG,  "[TriggerID:%u]:ACTION_REMOVE_ITEM", getParent()->TriggerID());
#endif
}


// 필드상에 있는 플레이어에게 인벤토리에 보상아이템을 준다.
class DistributeItemToInventoryOnField : public std::unary_function<Character *, void>
{
    ACTION_REWARD_PLAYER* m_pReward;
public :
    DistributeItemToInventoryOnField(ACTION_REWARD_PLAYER* pReward):m_pReward(pReward) {}
    ~DistributeItemToInventoryOnField(){}

    void operator()(Player* pPlayer) const
    {
        GameField* pField = pPlayer->GetField();
        if(pField == 0)
            return;

        WzVector vec = pPlayer->GetRandomPosInAround(2);
        SCItemSlot slot;
        slot.SetCode(m_pReward->Get()->GetItemCode());
        g_DropManager.DropItemToField(pField, &vec, pPlayer->GetObjectKey(), 0, slot);
    }

    void operator()(std::pair<DWORD,Player *> pair_type)
    {
        operator()(pair_type.second);
    }
};


#ifdef __WAVERIX_UNUSED_SECTION__
// 조건에 합당한 플레이어에게 인벤토리에 보상을 준다.
class DistributeItemToInventoryForCondition
{
    CODETYPE m_ItemCode;
    int      m_Count;
public:
    DistributeItemToInventoryForCondition(CODETYPE ItemCode, int count)
        : m_ItemCode(ItemCode), m_Count(count) {}
    ~DistributeItemToInventoryForCondition() {}

    BOOL operator()(Trigger::PLAYER_TRIGGER_INFO* pPlayerInfo)
    {
        Player* pPlayer = pPlayerInfo->m_pPlayer;
        ///g_DropManager.DropItemToField(pPlayer->GetUserKey(), m_ItemCode);
        ///!!!!! temp TEMPTEMPTEMTPE
        ///!!!!! temp TEMPTEMPTEMTPE
        ///!!!!! temp TEMPTEMPTEMTPE
        ///!!!!! temp TEMPTEMPTEMTPE
        ///!!!!! temp TEMPTEMPTEMTPE
        ///!!!!! temp TEMPTEMPTEMTPE
        ///!!!!! temp TEMPTEMPTEMTPE

        return FALSE;
    }
};
#endif


//==================================================================================================


__IMPL_POOL(ACTION_REWARD_PLAYER)
void
ACTION_REWARD_PLAYER::OnAction()
{
    const BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(Get()->GetItemCode());
    if(pItemInfo == 0)
    {
        // 에러 처리
        return;
    }

    ASSERT(Get()->GetCount() > 0);

	DWORD clicked_player_key = getParent()->GetClickedPlayerKey();

	// 보상 대상이 개인으로 되어있고, 클릭한 플레이어가 존재할경우에,, 해당 플레이어 주변에 아이템준다
	if( Get()->GetIsParty() == false && clicked_player_key != 0 )
	{
		Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
		if(pPlayer)
		{
			DistributeItemToInventoryOnField op((ACTION_REWARD_PLAYER*)this);
			op(pPlayer);

		}
	}
	// 클릭한 플레이어가 존재하는 경우, 해당 플레이어가 속한 파티맴버들 주변에 아이템준다
	else if( clicked_player_key != 0 )
    {
        Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
        if(pPlayer)
        {
            WORD partyKey = pPlayer->GetPartyState().GetPartyKey();

            DistributeItemToInventoryOnField Opr(this);
            if(!NewGamePartyManager::Instance()->For_eachPlayers(partyKey, Opr))
                SUNLOG(eCRITICAL_LOG,
                       "[ACTION_REWARD_PLAYER] GameParty is NULL! (%s)'s PartyKey (%u)",
                       pPlayer->GetCharName(), partyKey);
            return;
        }
        SUNLOG(eCRITICAL_LOG, "[ACTION_REWARD_PLAYER] Player is NULL! ClickedUserKey %u",
               clicked_player_key);
    }
	// 모든 사람에게 아이템을 준다
    else
    {
        DistributeItemToInventoryOnField op((ACTION_REWARD_PLAYER*)this);
        getMGR()->ForeachPlayer(op);
    }

}


//==================================================================================================


__IMPL_POOL(ACTION_CLEAR_MISSION)
void
ACTION_CLEAR_MISSION::OnAction()
{
    getMGR()->ClearMission();

#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG,  "[TriggerID:%u]:ACTION_CLEAR_MISSION", getParent()->TriggerID());
#endif
}


//==================================================================================================


__IMPL_POOL(ACTION_PORTAL_RANDOM)
void
ACTION_PORTAL_RANDOM::OnAction()
{
    ACTION_PORTAL_RANDOM_INFO* pInfo = Get();
    // 해당 영역을 얻어와서
    MAPCODE enterFieldCode = static_cast<MAPCODE>(pInfo->GetMapID());
    if(pInfo->GetRandom() == 0)
    {
        int nRandArea = rand() % 8;
        // 랜덤하게 워프
        if(pInfo->GetTarget() == ns_trigger::PARTY)
        {
            // 현재 필드에 있는 사람을 모두 워프
            // 조건에 해당하는 사람을 알 필요 없음
            const WzID areaID = pInfo->GetAreaID(nRandArea);
            WarpControl::WarpAllPlayer(getMGR()->GetField(), enterFieldCode, areaID);
        }
        else
        {
            ASSERT(pInfo->GetTarget() == ns_trigger::PERSONAL);

            if(DWORD clicked_player_key = getParent()->GetClickedPlayerKey())
            {
                Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
                if(NULL == pPlayer)
                {
                    SUNLOG(eFULL_LOG, "["__FUNCTION__"] [U:%d] Player NULL!!", clicked_player_key);
                    return;
                }
                WARP_PLAYER::Warp(pPlayer, enterFieldCode, pInfo->GetAreaID(nRandArea));
            }
            else
            {
                WarpPlayerForField op(enterFieldCode,pInfo->GetAreaID(nRandArea));
                getParent()->ForeachSatisfiedPlayer(op);
                getParent()->RemoveAllSatisfiedPlayer();
            }
        }
    }
    else //if(pInfo->GetRandom() == ACTION_PORTAL_RANDOM::_ORDER)
    {
        // 파티원들을 순서대로 이동시킨다
        if(pInfo->GetTarget() == ns_trigger::PARTY)
        {
            // 현재 필드에 있는 사람을 모두 워프
            // 조건에 해당하는 사람을 알 필요 없음
            BackToTheVillage op;
            getMGR()->ForeachPlayer(op);
            //WarpPlayerForField op(EnterFieldCode,pInfo->GetAreaID());
        }
        else
        {
            ASSERT(pInfo->GetTarget() == ns_trigger::PERSONAL);

            if(DWORD clicked_player_key = getParent()->GetClickedPlayerKey())
            {
                Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
                if(NULL == pPlayer)
                {
                    SUNLOG(eFULL_LOG, "["__FUNCTION__"] [U:%d]ACTION_REWARD_PLAYER Player NULL!!",
                           clicked_player_key);
                    return;
                }

                BackToTheVillage op;
                op(pPlayer);
            }
            else
            {
                BackToTheVillage op;
                getParent()->ForeachSatisfiedPlayer(op);
                getParent()->RemoveAllSatisfiedPlayer();
            }
        }
    }
}


__IMPL_POOL(ACTION_NPC_RANDOM_AREA_MOVE)
void
ACTION_NPC_RANDOM_AREA_MOVE::OnAction()
{
    ACTION_NPC_RANDOM_AREA_MOVE_INFO* pInfo = Get();

    int nRandArea = rand() % 8;
    const WzID areaID = pInfo->GetAreaID(nRandArea);

    DWORD object_key = getParent()->GetObjectKey();
    NPC* const npc = getMGR()->FindNPC(object_key);
    if (npc == NULL)
        return;

    GameField* pField = npc->GetField();

    CWorldBase* pWorldBase = pField->GetFieldInfo()->GetWorldBase();
    const int number_of_special_areas = pWorldBase->GetNumberOfSpecialArea();
    WzSpecialArea* pSpecialArea = NULL;
    if (!FlowControl::FCAssert(areaID < number_of_special_areas &&
        (pSpecialArea = pWorldBase->GetSpecialAreaByIndex(areaID)) != NULL))
    {
        SUNLOG(eCRITICAL_LOG,
            _T("|["__FUNCTION__"]|Msg=can't find special area (index = %u)"),
            areaID);
        return;
    }

    WzBoundingVolume& rVolume = pSpecialArea->m_wbvRegion;
    if(BVT_PATH_TILE_INDEX == rVolume.m_eBvType)
    {
        WzVector wvTarget = {0,0,0};
        BvPathTileIndex& rPATHTILE = rVolume.m_BvPathTileIndex;
        DWORD dwSelectedTile = random(0, rPATHTILE.m_nCount - 1);
        nsAI::MovingPlanner* const pMovingPlanner = pField->GetMovingPlanner();
        wvTarget = pMovingPlanner->GetRandomPosInTile(rPATHTILE.m_pdwData[dwSelectedTile]);

        if (pInfo->GetMoveOrWarp() == 0) { // 0: move
#ifdef _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
            npc->SetRegenPos(wvTarget);
			npc->GetStateManager()->SetMoveAreaID(pSpecialArea->m_wiIdentity);
            npc->ChangeState(STATE_ID_TRACK_AREA);
#else
			 npc->MoveAndBroadcast(&wvTarget, CMS_RUN);
#endif // _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
        }
        else if (pInfo->GetMoveOrWarp() == 1 ) {// 1: teleport
            pField->TeleportObject(npc, &wvTarget);
        }
    }
}

//==================================================================================================


__IMPL_POOL(ACTION_ACTIVATE_TRIGGER)
void
ACTION_ACTIVATE_TRIGGER::OnAction()
{
    const DWORD wz_trigger_id = Get()->GetWzTriggerID();

    if(Get()->GetFlag() == 0)
        getMGR()->InsertActiveTrigger(wz_trigger_id);
    else
        getMGR()->RequestRemoveActiveTrigger(wz_trigger_id);
    // (f100721.2L) add routine to support a various planner trigger logic
    getMGR()->MAP_STATE.Change(getParent()->TriggerID());
#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG, "[TriggerID:%u]:ACTION_ACTIVATE_TRIGGER", getParent()->TriggerID());
#endif
}


//==================================================================================================


__IMPL_POOL(ACTION_AREA_DAMAGE)

class AreaDamageHandler
{
    DWORD    m_dwObjectKey;
    float    m_dist;
    WzVector m_vPos;
    DAMAGETYPE m_wDamage;

public:
    AreaDamageHandler(DWORD dwObjectKey, WzVector* pvPos, float dist, DAMAGETYPE wDamage)
    {
        m_dwObjectKey = dwObjectKey;
        m_vPos = *pvPos;
        m_dist = dist * dist;
        m_wDamage = wDamage;
    }
    ~AreaDamageHandler(){}

    void operator()(Player* const pPlayer)
    {
        const WzVector& curPos = *pPlayer->GetPosPtr();

        WzVector wvDist = m_vPos - curPos;
        float fDist2 = VectorLength2(&wvDist);

        if(fDist2 <= m_dist)
        {
            pPlayer->DecreaseHP(m_wDamage);
            // (WAVERIX) (061018) 패킷 처리 최적화할 여지가 보인다
            MSG_CG_TRIGGER_AREADAMAGE_BRD msg;
            msg.m_dwObjectKey = pPlayer->GetObjectKey();
            msg.m_wDamage     = m_wDamage;
            msg.m_dwHP        = pPlayer->GetHP();
            pPlayer->SendPacketAround((MSG_BASE_FORWARD*)&msg, sizeof(msg));
        }
    }

    void operator()(std::pair<DWORD, Player *> pair_type)
    {
        operator()(pair_type.second);
    }
};

void
ACTION_AREA_DAMAGE::OnAction()
{
    ACTION_AREA_DAMAGE_INFO* pInfo = Get();

    // 오브젝트 받아서
    MapObject* pMapObject = getMGR()->FindMapObject(pInfo->GetObjectIndex());
    ASSERT(NULL != pMapObject);
    DEBUG_CODE(if(!pMapObject) return ;);

    // 범위 안에 타겟(개인/파티) 있는지 체크하여 데미지 주자
    WzVector vPos = *pMapObject->GetPosPtr();

    float fRange = pInfo->GetRange();
    AreaDamageHandler handler(pInfo->GetObjectIndex(), &vPos, fRange,
                              static_cast<DAMAGETYPE>(pInfo->GetDamage()));
    getMGR()->ForeachPlayer(handler);   // 범위 안에 개인이든 파티든 있으면 다 주는 거 아닌가?

#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG, "[ID:%u]:ACTION_AREA_DAMAGE", getParent()->TriggerID());
#endif
}

//==================================================================================================

__IMPL_POOL(ACTION_OPERATE_SWITCH)
void
ACTION_OPERATE_SWITCH::OnAction()
{
    const ACTION_OPERATE_SWITCH_INFO& rInfo = *Get();
    int cond_value = (int)getMGR()->GetSwitchValue(rInfo.GetSwitchID());
    int argument = rInfo.GetArgument();

    int apply_value = 0;
    switch(rInfo.GetOperationType())
    {
    case eOPERATION_EQUAL:    apply_value = argument;                                     break;
    case eOPERATION_PLUS:     apply_value = (int)cond_value + argument;                   break;
    case eOPERATION_MINUS:    apply_value = (int)cond_value - argument;                   break;
    case eOPERATION_MULTIPLE: apply_value = (int)cond_value * argument;                   break;
    case eOPERATION_DIVIDE:   argument ? apply_value = (int)(cond_value / argument) : 0;  break;
    }

    if(DWORD(apply_value) >= 256 || 
       (argument == 0 && rInfo.GetOperationType() == eOPERATION_DIVIDE))
    {
        SUNLOG(eCRITICAL_LOG, "[ACTION_OPERATE_SWITCH] OperationType[%u] Result[%d]",
               rInfo.GetOperationType(), apply_value);
        return;
    }

    if(SAFE_NUMERIC_TYPECAST(int, apply_value, BYTE))
        getMGR()->SetSwitchValue(rInfo.GetSwitchID(), (BYTE)apply_value);
    else
    {
#ifdef __TRIGGER_TEST_LOG
        SUNLOG(eDEV_LOG, "[TriggerID:%u]:ACTION_OPERATE_SWITCH", getParent()->TriggerID());
#endif
    }
}


__IMPL_POOL(ACTION_OPERATE_SWITCH_EX)
void
ACTION_OPERATE_SWITCH_EX::OnAction()
{
	const ACTION_OPERATE_SWITCH_EX_INFO& rInfo = *Get();

	if (rInfo.GetTarget() == ns_trigger::ZONE)
	{
#ifdef _NA_007369_20140625_ACTION_OPERATE_SWITCH_EX
		GameZone* const game_zone = getMGR()->GetField()->GetGameZone();
		if(game_zone->GetZoneType() == eZONETYPE_MISSION)
		{
			GameMissionRoom* mission_room = static_cast<GameMissionRoom*>(game_zone);
			int cond_value = (int)mission_room->GetSwitchValue(rInfo.GetSwitchID());
			int argument = rInfo.GetArgument();

			int apply_value = 0;
			switch(rInfo.GetOperationType())
			{
				case eOPERATION_EQUAL:    apply_value = argument;                                     break;
				case eOPERATION_PLUS:     apply_value = (int)cond_value + argument;                   break;
				case eOPERATION_MINUS:    apply_value = (int)cond_value - argument;                   break;
				case eOPERATION_MULTIPLE: apply_value = (int)cond_value * argument;                   break;
				case eOPERATION_DIVIDE:   argument ? apply_value = (int)(cond_value / argument) : 0;  break;
			}

			if(DWORD(apply_value) >= 256 || 
				(argument == 0 && rInfo.GetOperationType() == eOPERATION_DIVIDE))
			{
				SUNLOG(eCRITICAL_LOG, "[ACTION_OPERATE_SWITCH] OperationType[%u] Result[%d]",
					rInfo.GetOperationType(), apply_value);
				return;
			}

			if(SAFE_NUMERIC_TYPECAST(int, apply_value, BYTE))
				mission_room->SetSwitchValue(rInfo.GetSwitchID(), (BYTE)apply_value);
			else
			{
#ifdef __TRIGGER_TEST_LOG
				SUNLOG(eDEV_LOG, "[TriggerID:%u]:ACTION_OPERATE_SWITCH", getParent()->TriggerID());
#endif
			}
		}
#endif //_NA_007369_20140625_ACTION_OPERATE_SWITCH_EX
	}
}


//==================================================================================================


__IMPL_POOL(ACTION_ROAD_CONNECT)
void
ACTION_ROAD_CONNECT::OnAction() // 일정 영역의 길 연결을 설정 또는 해제 한다
{
    const ACTION_ROAD_CONNECT_INFO* road_connect_action = Get();
    const INT area_index = road_connect_action->GetAreaIndex();
    TriggerManagerEx* const trigger_manager = getMGR();
    
    // 지역이 유효한지 검사한다.
    const WzSpecialArea* const area = trigger_manager->GetAreaInfo(area_index);
    if (!area)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|지역이 유효하지 않습니다.|AreaIndex = %d|"), 
               __FUNCTION__, 
               area_index);
        return;
    }
    
    // 영역 형식이 유효한지 검사한다.
    const WzBoundingVolume& region = area->m_wbvRegion;
    if (region.m_eBvType != BVT_PATH_TILE_INDEX)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|영역 형식이 유효하지 않습니다.|RegionType = %d|"), 
               __FUNCTION__, 
               region.m_eBvType);
        return;
    }
    
    // 설정값이 유효한지 검사한다.
    const BYTE can_go = road_connect_action->GetCanGo();
    if (can_go != ns_trigger::CANMOVE && can_go != ns_trigger::CANNOTMOVE)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|설정값이 유효하지 않습니다.|CanGo = %u|"), 
               __FUNCTION__, 
               can_go);
        return;
    }

    // 각각의 타일에 대해 설정값을 적용한다.
    const BvPathTileIndex& tile_list = region.m_BvPathTileIndex;
    for (DWORD i = 0; i < tile_list.m_nCount; ++i)
    {
        const INT tile_id = static_cast<INT>(tile_list.m_pdwData[i]);
        if (can_go == ns_trigger::CANMOVE)
        {
            trigger_manager->UnsetAttribute(tile_id, PTA_NO_WALK);
        }
        else if (can_go == ns_trigger::CANNOTMOVE)
        {
            trigger_manager->SetAttribute(tile_id, PTA_NO_WALK);
        }
    }
    (trigger_manager->MAP_STATE).Change(getParent()->TriggerID());
}


//==================================================================================================


__IMPL_POOL(ACTION_CREATE_DIR_MONSTER)
void
ACTION_CREATE_DIR_MONSTER::OnAction()
{
    const ACTION_CREATE_DIR_MONSTER_INFO& rInfo = *Get();

    m_iRegenCount = 0;
    m_RegenTimer.SetTimer(0);	//< 처음에는 반드시 한번 실행한다.
    if(rInfo.GetIsParty() == ns_trigger::PERSONAL)
    {
        // 아직 유저가 맵에 들어 오지 않은상황에서 0이 될 수 있다.!!!!!@@@@
        DWORD userCount = getMGR()->GetPlayerNum();
        m_iMaxCreateCount = rInfo.GetCreateCount() * (userCount == 0 ? 1 : userCount);
    }
    else
        m_iMaxCreateCount = rInfo.GetCreateCount();

#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG,  "[TriggerID:%u]:ACTION_CREATE_MONSTER,개수(%d)",
           getParent()->TriggerID(), m_iMaxCreateCount);
#endif
}


BOOL
ACTION_CREATE_DIR_MONSTER::OnActionUpdate()
{
    const ACTION_CREATE_DIR_MONSTER_INFO& rInfo = *Get();
    if(m_iRegenCount == rInfo.GetLoopCount()) 
    {
        return FALSE;
    }

    if(m_RegenTimer.IsExpired() == 0)
        return TRUE;

    m_RegenTimer.SetTimer(rInfo.GetDelayTime());

    ns_trigger::CreateMonsterArgs args;
    args.Clear();

    args.Init(rInfo.GetAreaID(), rInfo.GetType());
    MONSTERCODE monsterCode = (MONSTERCODE)rInfo.GetMonsterCode();

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    GameZone* game_zone = getMGR()->GetField()->GetGameZone();
    if(game_zone->GetZoneType() == eZONETYPE_MISSION) 
    {
        MONSTERCODE replaace_monster_code = \
            AppliedNpcDependencyChain::Instance()->GetRepleaceMonsterCode(game_zone, monsterCode, true);

        if (replaace_monster_code) {
            monsterCode = replaace_monster_code;
        }
    }
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

    WzVector vDirection = { rInfo.GetDirX(), rInfo.GetDirY(), rInfo.GetDirZ() };
    short angle = 0;
    {
        float fangle = Math_GetAngleFromVector(&vDirection); // (NOTE) 안에서 Normalize수행.
        angle = static_cast<short>(fangle * 180.0f / WZ_PI);
    }
    for(int loop = m_iMaxCreateCount; loop; --loop)
        args.AddInfo(monsterCode, angle);

    ns_trigger::CreateMonsterByTrigger(getMGR(), &args);
    ++m_iRegenCount;

    return TRUE;
}


//==================================================================================================


__IMPL_POOL(ACTION_CHANGE_LIGHT)
void
ACTION_CHANGE_LIGHT::OnAction()
{
    getMGR()->MAP_STATE.Change(getParent()->TriggerID());
}


//==================================================================================================


__IMPL_POOL(ACTION_CHANGE_OBJECT_TYPE)
void
ACTION_CHANGE_OBJECT_TYPE::OnAction()
{
    getMGR()->MAP_STATE.Change(getParent()->TriggerID());
}

struct ActionOperator
{
    class QUEST
    {
    public:
        // ActionTrigger <퀘스트 만족여부>
        class Satisfaction
        {
            QCODE   m_QuestCode;
        public:
            Satisfaction(QCODE wQuestCode) : m_QuestCode(wQuestCode) {}
            void operator()(Player* pPlayer)
            {
                // <060606, waverix>
                //   not implementation
                SUNLOG(eFULL_LOG, "ActionOperator::QUEST::Satisfaction is not implemented.");
                //Quest* pQuest = pPlayer->GetQuestManager()->FindQuest(m_QuestCode);
                //if(!pQuest)
                //{
                //  
                //}
            }

            void Process(SATISFIED_PLAYER_PAIR rPair)
            {
                // <060606, waverix>
                //   not implementation
                SUNLOG(eCRITICAL_LOG, "ActionOperator::QUEST::Satisfaction is not implemented.");
            }
        };  //ActionOperator::QUEST::Satisfaction

        // ActionTrigger <퀘스트 부여>
        class Grant
        {
            QCODE   m_QuestCode;
        public:
            Grant(QCODE wQuestCode) : m_QuestCode(wQuestCode) {}

            // 아래의 코드는
            void operator()(Player* pPlayer)
            {
                QuestManager* pQuestManager = pPlayer->GetQuestManager();
                //__NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM
                if(pQuestManager->IsQuestMax())
                {
                    MSG_CG_QUEST_ACCEPT_NAK msg;
                    msg.m_byErrorCode = RC::RC_QUEST_CANNOTQUEST_MAXCOUNT;
                    pPlayer->SendPacket(&msg, sizeof(msg));
                    return ;
                }
                //
                Quest* pQuest = pQuestManager->CreateQuest(m_QuestCode);

                if(pQuest && (!pQuest->GetQuestInfo()->IsChildQuest()))
                {
            
                    if(pQuestManager->CanDoAccept(m_QuestCode, pPlayer) == RC::RC_QUEST_SUCCESS)            
                    {
                        // (WAVERIX) (060607) 아래의 패킷은 트리거용으로 하나 새로 만들어서 사용할 것.
                        MSG_CG_QUEST_ACCEPT_ACK msg;
                        pPlayer->SendPacket(&msg, sizeof(msg));
                        return;
                    }
                }

                pQuestManager->DestroyQuest(m_QuestCode);
            }

            void Process(SATISFIED_PLAYER_PAIR rPair)
            {
                Player* pPlayer = rPair.second;
                QuestManager* pQuestMan = pPlayer->GetQuestManager();
                //__NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM
                if(pQuestMan->IsQuestMax())
                {
                    MSG_CG_QUEST_ACCEPT_NAK msg;
                    msg.m_byErrorCode = RC::RC_QUEST_CANNOTQUEST_MAXCOUNT;
                    pPlayer->SendPacket(&msg, sizeof(msg));
                    return ;
                }
                //
                Quest* pQuest = pQuestMan->CreateQuest(m_QuestCode);

                if(pQuest && (!pQuest->GetQuestInfo()->IsChildQuest()))
                {
            
                    if(pQuestMan->CanDoAccept(m_QuestCode, pPlayer) == RC::RC_QUEST_SUCCESS)            
                    {
                        // (WAVERIX) (060607) 아래의 패킷은 트리거용으로 하나 새로 만들어서 사용할 것.
                        MSG_CG_QUEST_ACCEPT_ACK msg;
                        pPlayer->SendPacket(&msg, sizeof(msg));
                        return;
                    }
                }

                pQuestMan->DestroyQuest(m_QuestCode);
            }
        };  // ActionOperator::QUEST::Grant

        class Setup
        {
            QCODE m_QuestCode;

        public:
            Setup(QCODE wQuestCode) : m_QuestCode(wQuestCode) {}

            // 아래의 코드는
            void operator()(Player* pPlayer)
            {
                QuestManager* pQuestManager = pPlayer->GetQuestManager();
                Quest* pQuest = pQuestManager->FindQuest(m_QuestCode);
                if(pQuest == 0)
                    return;
            
                if(pQuestManager->CanComplete(m_QuestCode, pPlayer) == RC::RC_QUEST_SUCCESS)            
                {
                    // 보상 체계를 갖춰야 하는가?
                    pQuestManager->DoReward(m_QuestCode, pPlayer);
                    //__NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM
                    QUESTPART_COMPL* pCompNum = pQuestManager->FindComplQuest(m_QuestCode);
                    if(pCompNum && (0xFF == pCompNum->m_Num))
                    {
                        //(lst1024)(090611) 무한 반복퀘를 255회 이상하는 유저가 많아졌다.
                        // 혹시 다른 이유일수 있으니 로그 남긴다.
                        SUNLOG(eCRITICAL_LOG,
                               "["__FUNCTION__"] char(%s), guid(%d), Quest(%d), num(%d)",
                               pPlayer->GetCharName(), pPlayer->GetCharGuid(),
                               m_QuestCode, pCompNum->m_Num);
                    }
                    //SUNLOG(eCRITICAL_LOG, _T("Setup %u"), m_QuestCode);
                }

                return;
            }

            void Process(SATISFIED_PLAYER_PAIR rPair)
            {
                Player* pPlayer = rPair.second;
                QuestManager* pQuestManager = pPlayer->GetQuestManager();
                Quest* pQuest = pQuestManager->FindQuest(m_QuestCode);
                if(pQuest == 0)
                    return;
            
                if(pQuestManager->CanComplete(m_QuestCode, pPlayer) == RC::RC_QUEST_SUCCESS)            
                {
                    // 보상 체계를 갖춰야 하는가?
                    pQuestManager->DoReward(m_QuestCode, pPlayer);
                    //__NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM
                    QUESTPART_COMPL* pCompNum = pQuestManager->FindComplQuest(m_QuestCode);
                    if(pCompNum && (0xFF == pCompNum->m_Num))
                    {
                        //(lst1024)(090611) 무한 반복퀘를 255회 이상하는 유저가 많아졌다.
                        // 혹시 다른 이유일수 있으니 로그 남긴다.
                        SUNLOG(eCRITICAL_LOG,
                               "["__FUNCTION__"] char(%s), guid(%d), Quest(%d), num(%d)",
                               pPlayer->GetCharName(), pPlayer->GetCharGuid(),
                               m_QuestCode, pCompNum->m_Num);
                    }
                    //SUNLOG(eCRITICAL_LOG, _T("Setup %u"), m_QuestCode);
                }

                return;
            }
        };
    };  // ActionOperator::QUEST
};  // ActionOperator


//==================================================================================================


// <Waverix>
// Quest 관련 미구현 또는 분석 미비로 인한 트리거 항목 완료 미설정
__IMPL_POOL(ACTION_QUEST_SATISFACTION)
void
ACTION_QUEST_SATISFACTION::OnAction()
{
    const ACTION_QUEST_SATISFACTION_INFO* pInfo = (const ACTION_QUEST_SATISFACTION_INFO*)Get();
    ASSERT(SAFE_NUMERIC_TYPECAST(ULONG, pInfo->GetQuestID(), QCODE));

    ActionOperator::QUEST::Satisfaction op((QCODE)pInfo->GetQuestID());

    if(DWORD clicked_player_key = getParent()->GetClickedPlayerKey())
    {
        Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
        if(pPlayer)
        {
            op(pPlayer);
            return;
        }

        SUNLOG(eFULL_LOG, "[ACTION_QUEST_SATISFACTION::OnAction] [U:%d] Player NULL!!",
               clicked_player_key);
        return;
    }
    else
    {
        // Satisfied Player
        getParent()->ForeachSatisfiedPlayer(&ActionOperator::QUEST::Satisfaction::Process, &op);
        getParent()->RemoveAllSatisfiedPlayer();
    }
}


//==================================================================================================


__IMPL_POOL(ACTION_QUEST_GRANT)
void
ACTION_QUEST_GRANT::OnAction()
{
    const ACTION_QUEST_GRANT_INFO* pInfo = (const ACTION_QUEST_GRANT_INFO*)Get();
    ASSERT(SAFE_NUMERIC_TYPECAST(ULONG, pInfo->GetQuestID(), QCODE));

    ActionOperator::QUEST::Grant op((QCODE)pInfo->GetQuestID());

    if(DWORD clicked_player_key = getParent()->GetClickedPlayerKey())
    {
        Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
        if(pPlayer)
        {
            op(pPlayer);
            return;
        }

        SUNLOG(eFULL_LOG, "[ACTION_QUEST_SATISFACTION::OnAction] [U:%d] Player NULL!!",
               clicked_player_key);
    }
    else
    {
        // Satisfied Player
        getParent()->ForeachSatisfiedPlayer(&ActionOperator::QUEST::Grant::Process, &op);
        getParent()->RemoveAllSatisfiedPlayer();
    }
}

__IMPL_POOL(ACTION_QUEST_SETUP)
void
ACTION_QUEST_SETUP::OnAction()
{
    const ACTION_QUEST_SETUP_INFO* pInfo = (const ACTION_QUEST_SETUP_INFO*)Get();
    ASSERT(SAFE_NUMERIC_TYPECAST(ULONG, pInfo->GetQuestID(), QCODE));
    QCODE questCode = (QCODE)pInfo->GetQuestID();

    ActionOperator::QUEST::Setup op((QCODE)pInfo->GetQuestID());

    const DWORD clicked_player_key = getParent()->GetClickedPlayerKey();
    if(clicked_player_key != 0)
    {
        Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
        if(pPlayer)
        {
            op(pPlayer);
            return;
        }

        SUNLOG(eFULL_LOG, "[ACTION_QUEST_SATISFACTION::OnAction] [U:%d] Player NULL!!", clicked_player_key);
    }
    else
    {
        // Satisfied Player
        getParent()->ForeachSatisfiedPlayer(&ActionOperator::QUEST::Setup::Process, &op);
        getParent()->RemoveAllSatisfiedPlayer();
    }
}


//==================================================================================================


__IMPL_POOL(ACTION_USE_CATEGORY)
void
ACTION_USE_CATEGORY::OnAction()
{
}


//==================================================================================================


// 기존에 존재하는 액션 트리거
//__IMPL_POOL(ACTION_CREATE_MONSTER)

__IMPL_POOL(ACTION_CREATE_MONSTER_GROUP)
void
ACTION_CREATE_MONSTER_GROUP::OnAction()
{
    // 다음의 루틴은 ACTION_CREATE_MONSTER 의 로직을 따라한 것이다.
    m_iRegenCount = 0;
    m_dwTimeout = 0;

    const ACTION_CREATE_MONSTER_GROUP_INFO* pInfo = Get();
    if(ns_trigger::PARTY == pInfo->GetPartyState())
    {
        m_iMaxCreateCount = pInfo->GetGroupCount();
    }
    else //ns_trigger::PERSONAL
    {
        // 아직 유저가 맵에 들어 오지 않은상황에서 0이 될 수 있다.!!!!!@@@@
        DWORD count = getMGR()->GetPlayerNum();
        m_iMaxCreateCount = pInfo->GetGroupCount() * (count == 0 ? 1 : count);
    }
}

BOOL
ACTION_CREATE_MONSTER_GROUP::OnActionUpdate()
{
    const ACTION_CREATE_MONSTER_GROUP_INFO* pInfo = Get();

    if(pInfo->GetGroupCount() == m_iRegenCount)
    {
        return FALSE;
    }
    else
    {
        // 몬스터 그룹 생성은 주기가 없다. 하지만 한번에 생성함으로 인한 부하(꼭 서버만의 부하는 아닐터)
        // 를 감안해 적당한 주기를 주고 생성하기로 하자. 단 이를 위한 테스트는 해봐야 할 것이다.
        const DWORD dwInterval = 100000;    //10초?
        DWORD dwCurTick = GetTickCount();
        if(m_dwTimeout < dwCurTick)
        {
            m_dwTimeout = dwCurTick + dwInterval;

            ASSERT(SAFE_NUMERIC_TYPECAST(int, pInfo->GetAreaID(), DWORD));
            ASSERT(SAFE_NUMERIC_TYPECAST(int, pInfo->GetAreaID(), WORD));

            // !!!!!!!몬스터 그룹 생성 코드가 빠져있다!!!!!!

        }
    }
    return TRUE;
}


//==================================================================================================


__IMPL_POOL(ACTION_SHOW_OR_HIDE_UNIT)
void
ACTION_SHOW_OR_HIDE_UNIT::OnAction()
{
    // 맵툴의 관점에서는 몬스터 나타남/사라짐이지만, 실제 서버에서는
    // 몬스터 유닛 생성 / 파괴
    //__NA_276_20070213_UNIT_TRIGGER
    const ACTION_SHOW_OR_HIDE_UNIT_INFO* pInfo = (const ACTION_SHOW_OR_HIDE_UNIT_INFO*)Get();
    const INT is_hide = pInfo->GetIsHide();
    const WzID unit_ID = pInfo->GetMonsterUnitID();

    switch (is_hide)
    {
    case ACTION_SHOW_OR_HIDE_UNIT_INFO::SHOW:
        getMGR()->CreateNPCForUnitID(unit_ID);
        break;
       
    case ACTION_SHOW_OR_HIDE_UNIT_INFO::HIDE:
        getMGR()->DestroyNPCForUnitID(unit_ID);
        break;

    default:
        SUNLOG(eCRITICAL_LOG, "ACTION_SHOW_OR_HIDE_UNIT:Invalid Value(%u)", pInfo->GetIsHide());
        return;
    }
}


//==================================================================================================


__IMPL_POOL(ACTION_SHOW_GUIDE_MSGBOX)
void
ACTION_SHOW_GUIDE_MSGBOX::OnAction()
{
#ifndef __WAVERIX_NEW_TRIGGER_WORKS_TEST__
    SUNLOG(eCRITICAL_LOG, "ACTION_SHOW_GUIDE_MSGBOX::OnAction");
#endif
}


//==================================================================================================


__IMPL_POOL(ACTION_SHOW_OR_HIDE_MSGBOX)
void
ACTION_SHOW_OR_HIDE_MSGBOX::OnAction()
{
}


//==================================================================================================


// 몬스터 랜덤 스폰
__IMPL_POOL(ACTION_RANDOM_MONSTER_SPAWN)
BOOST_STATIC_ASSERT(ACTION_RANDOM_MONSTER_SPAWN::MAX_RANDOM_STEP_NUM ==
                    DropRatioInfoList::MAX_RANDOM_STEP_NUM);
void
ACTION_RANDOM_MONSTER_SPAWN::OnAction()
{
    m_nCurRegenCount = 0;
    m_RegenTimer.SetTimer(0);   //< 처음에는 반드시 한번 실행한다.

    m_MonsterRandomizer.Clear();

    const ACTION_RANDOM_MONSTER_SPAWN_INFO* pInfo = Get();
    BOOL rt = FALSE;
    // 몬스터 1부터 5까지 대입한다.
    rt = m_MonsterRandomizer.AddRatio(pInfo->GetMonsterID_1(), pInfo->GetMonsterRate_1(),
                                      MAX_MONSTER_RATIO);
    ASSERT(rt);
    rt = m_MonsterRandomizer.AddRatio(pInfo->GetMonsterID_2(), pInfo->GetMonsterRate_2(),
                                      MAX_MONSTER_RATIO);
    ASSERT(rt);
    rt = m_MonsterRandomizer.AddRatio(pInfo->GetMonsterID_3(), pInfo->GetMonsterRate_3(),
                                      MAX_MONSTER_RATIO);
    ASSERT(rt);
    rt = m_MonsterRandomizer.AddRatio(pInfo->GetMonsterID_4(), pInfo->GetMonsterRate_4(),
                                      MAX_MONSTER_RATIO);
    ASSERT(rt);
    rt = m_MonsterRandomizer.AddRatio(pInfo->GetMonsterID_5(), pInfo->GetMonsterRate_5(),
                                      MAX_MONSTER_RATIO);
    ASSERT(rt);
}

BOOL
ACTION_RANDOM_MONSTER_SPAWN::OnActionUpdate()
{
    const ACTION_RANDOM_MONSTER_SPAWN_INFO* pInfo = Get();
    // 현재 리젠된 횟수와 Max리젠 횟수가 같다면... 그만 한다.
    if(m_nCurRegenCount == pInfo->GetMaxRegenCount()) 
    {
        return FALSE;
    }

    if(m_RegenTimer.IsExpired() == 0)
        return TRUE;

    //1. 리젠시간이 지났다면.. 리젠시간을 다시 적용하고..
    m_RegenTimer.SetTimer(pInfo->GetRegenTime());

    ns_trigger::CreateMonsterArgs args;
    args.Clear();

    args.Init(pInfo->GetAreaID(), 0);
    WzVector vDirection = { pInfo->GetDirX(), pInfo->GetDirY(), pInfo->GetDirZ() };
    short angle = 0;
    {
        float fangle = Math_GetAngleFromVector(&vDirection); // (NOTE) 안에서 Normalize수행.
        angle = static_cast<short>(fangle * 180.0f / WZ_PI);
    }
    for(int loop = GetMonsterCount(); loop; --loop) 
    {
        MONSTERCODE monsterCode = GetRandomMonster();

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
        GameZone* game_zone = getMGR()->GetField()->GetGameZone();
        if(game_zone->GetZoneType() == eZONETYPE_MISSION) 
        {
            MONSTERCODE replaace_monster_code = \
                AppliedNpcDependencyChain::Instance()->GetRepleaceMonsterCode(game_zone, monsterCode, true);

            if (replaace_monster_code) {
                monsterCode = replaace_monster_code;
            }
        }
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

        args.AddInfo(monsterCode, angle);
    }

    ns_trigger::CreateMonsterByTrigger(getMGR(), &args);
    ++m_nCurRegenCount;

    return TRUE;
}

//리젠시킬 몬스터의 개수를 구한다.
int
ACTION_RANDOM_MONSTER_SPAWN::GetMonsterCount()
{
    int nMonsterCount = 0;
    int nRandomCnt = random(Get()->GetMinCreateMonsterCnt(), Get()->GetMaxCreateMonsterCnt());

    if(Get()->GetIsParty() == ns_trigger::PERSONAL)
    {
        // 아직 유저가 맵에 들어 오지 않은상황에서 0이 될 수 있다.!!!!!@@@@
        DWORD dwPlayerNum = getMGR()->GetPlayerNum();
        nMonsterCount = nRandomCnt * (dwPlayerNum == 0 ? 1 : dwPlayerNum);
    }
    else
        nMonsterCount = nRandomCnt;

#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG,  "[TriggerID:%u]:ACTION_RANDOM_MONSTER_SPAWN, 개수(%d)",
           getParent()->TriggerID(), nMonsterCount);
#endif

    return nMonsterCount;
}

int
ACTION_RANDOM_MONSTER_SPAWN::GetRandomMonster()
{
    return m_MonsterRandomizer.RandomizedKey();
}


//==================================================================================================


class ShowTheFloor
{
private:
    MSG_CG_TRIGGER_SHOW_THE_FLOOR_CMD&  m_rMsgCMD;
    WORD                                m_PartyKey;
public:
    ShowTheFloor(MSG_CG_TRIGGER_SHOW_THE_FLOOR_CMD& rMsgCMD) : m_rMsgCMD(rMsgCMD), m_PartyKey(0) {}
    void operator()(Player* pPlayer) 
    {
        WORD partyKey = pPlayer->GetPartyState().GetPartyKey();
        m_rMsgCMD.m_dwPlayerKey = pPlayer->GetObjectKey();
        m_PartyKey = partyKey;

        if(!NewGamePartyManager::Instance()->SendPacketAllMember(partyKey, &m_rMsgCMD, sizeof(m_rMsgCMD)))
        {
            pPlayer->SendPacket(&m_rMsgCMD, sizeof(m_rMsgCMD));
        }
    }
};

__IMPL_POOL(ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR)
void
ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR::OnAction()
{
    const ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR_INFO* pInfo = Get();

    MSG_CG_TRIGGER_SHOW_THE_FLOOR_CMD msgCMD;
    msgCMD.m_byFloor = pInfo->GetFloor();
    DWORD clicked_player_key = getParent()->GetClickedPlayerKey();

    ShowTheFloor fnShowTheFloor(msgCMD);
    if(0 != clicked_player_key)
    {
        Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
        if(pPlayer)
            fnShowTheFloor(pPlayer);
    }
    else
    {
        getParent()->ForeachSatisfiedPlayer(fnShowTheFloor);
    }
}

//#endif //


//==================================================================================================


class IActionTriggerEx;
class WarpPlayerForRandomFieldArea
{
public:
    enum eRANDOM_FIELDnAREA { RANDOM_FIELDnAREA_MAP = 0, RANDOM_FIELDnAREA_AREA = 1 };
private:

    eRANDOM_FIELDnAREA  m_Type;
    IActionTrigger*     m_pIActionObj;
    DWORD               m_FixedValue;
public: 
    WarpPlayerForRandomFieldArea(eRANDOM_FIELDnAREA eType, DWORD dwFixedVal,
                                 IActionTrigger* pIActionObj)
        : m_Type(eType), m_pIActionObj(pIActionObj), m_FixedValue(dwFixedVal)
    {}
    ~WarpPlayerForRandomFieldArea() {}

    void operator()(Player* pPlayer) 
    {
        GetRandomFieldArea_N_Warp(pPlayer);
    }
    void operator()(std::pair<DWORD,Player *> pair_type) 
    { 
        operator()(pair_type.second);
    }

    BOOL GetRandomFieldArea_N_Warp(Player* pPlayer)
    {
        GameZone* pZone = pPlayer->GetGameZonePtr();
        GameField* pField = pPlayer->GetField();
        if(pZone == 0 || pField == 0)
            return FALSE;

        switch(m_Type)
        {
        case RANDOM_FIELDnAREA_MAP:
            {
                ACTION_RANDOM_MAP_MOVE* pAction = (ACTION_RANDOM_MAP_MOVE*)m_pIActionObj;
                ACTION_RANDOM_MAP_MOVE_INFO* pInfo = pAction->Get();
                WORD wSelectIdx = (WORD)pAction->PrepareNextCode();
                FIELDCODE fcCODE = (FIELDCODE)pInfo->GetFieldID(wSelectIdx);

                WARP_PLAYER::Warp(pPlayer, (MAPCODE)fcCODE, (WzID)m_FixedValue);
            }
            break;
        case RANDOM_FIELDnAREA_AREA:
            {
                ACTION_RANDOM_AREA_MOVE* pAction = (ACTION_RANDOM_AREA_MOVE*)m_pIActionObj;
                int iAreaIDX;
                if(m_FixedValue)
                    iAreaIDX = (int)m_FixedValue;
                else
                {
                    ACTION_RANDOM_AREA_MOVE_INFO* pInfo = pAction->Get();
                    WORD wSelectIdx = (WORD)pAction->PrepareNextCode();
                    iAreaIDX = (int)pInfo->GetAreaIDX(wSelectIdx);
                }

                //FIELDCODE fcCODE = pField->GetFieldCode();
                //WARP_PLAYER::Warp(pPlayer, (MAPCODE)fcCODE, wzAreaID);
                //pPlayer->Portal(ePORTAL_TYPE_GM, fcCODE);

                //const CHAR* RNDAREA = "STR0";
                //wzAreaID = StrToWzID(RNDAREA);
                CWorldBase* pWorldBase = pField->GetFieldInfo()->GetWorldBase();
                const int number_of_special_areas = pWorldBase->GetNumberOfSpecialArea();
                WzSpecialArea* pSpecialArea = NULL;
                if (!FlowControl::FCAssert(iAreaIDX < number_of_special_areas &&
                        (pSpecialArea = pWorldBase->GetSpecialAreaByIndex(iAreaIDX)) != NULL))
                {
                    SUNLOG(eCRITICAL_LOG,
                           _T("|["__FUNCTION__"]|Msg=can't find special area (index = %u)"),
                           iAreaIDX);
                    return false;
                }

                WzBoundingVolume& rVolume = pSpecialArea->m_wbvRegion;
                if(BVT_PATH_TILE_INDEX == rVolume.m_eBvType)
                {
                    WzVector wvTarget = {0,0,0};
                    BvPathTileIndex& rPATHTILE = rVolume.m_BvPathTileIndex;
                    DWORD dwSelectedTile = random(0, rPATHTILE.m_nCount - 1);
                    nsAI::MovingPlanner* const pMovingPlanner = pField->GetMovingPlanner();
                    wvTarget = pMovingPlanner->GetRandomPosInTile(rPATHTILE.m_pdwData[dwSelectedTile]);
                    pField->TeleportObject(pPlayer, &wvTarget);
                }
            }
            break;
        }

        return TRUE;
    }

};


//==================================================================================================


__IMPL_POOL(ACTION_RANDOM_MAP_MOVE)

void
ACTION_RANDOM_MAP_MOVE::OnInit()
{
    //if(!m_INIT.IsInited())
    //{
    ACTION_RANDOM_MAP_MOVE_INFO* pInfo = Get();

    m_List.clear();
    int* pFIELDs = pInfo->GetFieldIDPtr();
    for(DWORD i=0 ; i<ACTION_RANDOM_MAP_MOVE_INFO::_MAX_FIELDCODE_ID ; ++i, ++pFIELDs)
    {
        int iFieldCode = *pFIELDs;
        if(ULONG_MAX != (DWORD)iFieldCode)
            m_List.push_back((BYTE)i);
    }

    ENUM_T bIsRandom = (ENUM_T)pInfo->GetRandom();

    // 처음 상태일 때, 순차와 동일해주므로 한번 주사위 굴려준다.
    if(ACTION_RANDOM_MAP_MOVE_INFO::RNDMODE_RANDOM == bIsRandom)
        PrepareNextCode();

    //  m_INIT.Inited();
    //}
}

DWORD
ACTION_RANDOM_MAP_MOVE::PrepareNextCode()
{
    ACTION_RANDOM_MAP_MOVE_INFO* pInfo = Get();
    ENUM_T bIsRandom = (ENUM_T)pInfo->GetRandom();

    DWORD dwRET = *(m_List.begin());

    if(ACTION_RANDOM_MAP_MOVE_INFO::RNDMODE_RANDOM == bIsRandom)
        random_shuffle(m_List.begin(), m_List.end());
    else
        rotate(m_List.begin(), m_List.begin()+1, m_List.end());

    return dwRET;
}

void
ACTION_RANDOM_MAP_MOVE::OnAction()
{
    ACTION_RANDOM_MAP_MOVE_INFO* pInfo = Get();
    // 해당 영역을 얻어와서
    WzID areaID_for_Moving = pInfo->GetAreaID();  //RZ~~
    ENUM_T bIsRandom = (ENUM_T)pInfo->GetRandom();
    ENUM_T bIsParty = (ENUM_T)pInfo->GetTarget();
    ENUM_T retMapType = (ENUM_T)pInfo->GetType();
    //BOOL bIsLoop = getParent()->IsLoop() ? TRUE : FALSE;

    if(ACTION_RANDOM_MAP_MOVE_INFO::TARGET_PARTY == bIsParty)
    {
        // 선택된 맵코드
        WORD wSelectedIdx = (WORD)*m_List.begin();
        FIELDCODE fcCODE = (FIELDCODE)pInfo->GetFieldID(wSelectedIdx);

        switch(retMapType)
        {
        case ACTION_RANDOM_MAP_MOVE_INFO::RETMAP_VILLAGE:
        case ACTION_RANDOM_MAP_MOVE_INFO::RETMAP_STARTINGVILLAGE:
            {
                // 뭐냐 이건... 기존 형식 호환?
                ASSERT(!"Not Supported RANDOM_MAP_MOVE");
            }
            break;
        case ACTION_RANDOM_MAP_MOVE_INFO::RETMAP_BATTLEMAP:
            {
                const WzID areaID = Get()->GetAreaID();
                WarpControl::WarpAllPlayer(getMGR()->GetField(), fcCODE, areaID_for_Moving);
            }
            break;
        };
    }
    else
    {
        switch(retMapType)
        {
        case ACTION_RANDOM_MAP_MOVE_INFO::RETMAP_VILLAGE:
        case ACTION_RANDOM_MAP_MOVE_INFO::RETMAP_STARTINGVILLAGE:
            {
                // 뭐냐 이건... 기존 형식 호환?
                ASSERT(!"Not Supported RANDOM_MAP_MOVE");
            }
            break;
        case ACTION_RANDOM_MAP_MOVE_INFO::RETMAP_BATTLEMAP:
            {
                WarpPlayerForRandomFieldArea oper(WarpPlayerForRandomFieldArea::RANDOM_FIELDnAREA_MAP,
                                                  areaID_for_Moving, this);

                DWORD clicked_player_key = getParent()->GetClickedPlayerKey();
                if(clicked_player_key != 0)
                {
                    Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
                    if(pPlayer)
                        oper(pPlayer);
                    else
                    {
                        SUNLOG(eFULL_LOG, "[ACTION_RANDOM_MAP_MOVE::OnAction] [U:%d] Player NULL!!",
                               getParent()->GetClickedPlayerKey());
                        return;
                    }
                }
                else
                {
                    getParent()->ForeachSatisfiedPlayer(oper);
                    getParent()->RemoveAllSatisfiedPlayer();
                }
            }
            break;
        }
    }
}


//==================================================================================================


__IMPL_POOL(ACTION_RANDOM_AREA_MOVE)

void
ACTION_RANDOM_AREA_MOVE::OnInit()
{
    //if(!m_INIT.IsInited())
    //{
    ACTION_RANDOM_AREA_MOVE_INFO* pInfo = Get();

    m_List.clear();
    int* pAREAs = pInfo->GetAreaIDXPtr();
    for(DWORD i=0 ; i<ACTION_RANDOM_AREA_MOVE_INFO::_MAX_AREA_ID ; ++i, ++pAREAs)
    {
        int wiArea = *pAREAs;
        if(ULONG_MAX != (DWORD)wiArea)
            m_List.push_back((BYTE)i);
    }

    // 기본 랜덤
    PrepareNextCode();

    //  m_INIT.Inited();
    //}
}

DWORD
ACTION_RANDOM_AREA_MOVE::PrepareNextCode()
{
    ACTION_RANDOM_AREA_MOVE_INFO* pInfo = Get();
    //ENUM_T procType = (ENUM_T)pInfo->GetProcType();

    random_shuffle(m_List.begin(), m_List.end());

    return *(m_List.begin());;
}

void
ACTION_RANDOM_AREA_MOVE::OnAction()
{
    ACTION_RANDOM_AREA_MOVE_INFO* pInfo = Get();

    ENUM_T bIsParty = (ENUM_T)pInfo->GetTarget();
    ENUM_T procType = (ENUM_T)pInfo->GetProcType();
    //BOOL bIsLoop = getParent()->IsLoop() ? TRUE : FALSE;

    // 선택된 맵코드
    WORD wSelectedIdx = (WORD)*m_List.begin();
    int iAreaID = (int)pInfo->GetAreaIDX(wSelectedIdx);

    if(ACTION_RANDOM_AREA_MOVE_INFO::PROC_EACH == procType)
        PrepareNextCode();

    if(ACTION_RANDOM_AREA_MOVE_INFO::TARGET_PARTY == bIsParty)
    {
        ASSERT(!"Not Supported RANDOM_AREA_MOVE");
    }
    else
    {
        DWORD clicked_player_key = getParent()->GetClickedPlayerKey();
        if(clicked_player_key != 0)
        {
            Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
            WarpPlayerForRandomFieldArea oper(WarpPlayerForRandomFieldArea::RANDOM_FIELDnAREA_AREA, (DWORD)iAreaID, this);

            if(pPlayer)
                oper(pPlayer);
            else
            {
                SUNLOG(eFULL_LOG, "[ACTION_RANDOM_MAP_MOVE::OnAction] [U:%d] Player NULL!!", getParent()->GetClickedPlayerKey());
                return;
            }
        }
        else
        {
            if(ACTION_RANDOM_AREA_MOVE_INFO::PROC_EACH == procType)
                iAreaID = 0;

            WarpPlayerForRandomFieldArea oper(WarpPlayerForRandomFieldArea::RANDOM_FIELDnAREA_AREA, (DWORD)iAreaID, this);
            getParent()->ForeachSatisfiedPlayer(oper);
            getParent()->RemoveAllSatisfiedPlayer();
        }
    }
}


//==================================================================================================


__IMPL_POOL(ACTION_REMOVE_MONSTER)
void
ACTION_REMOVE_MONSTER::OnAction()
{
    const ACTION_REMOVE_MONSTER_INFO* pActionInfo = (const ACTION_REMOVE_MONSTER_INFO*)Get();
    if(!pActionInfo)        return;

    Player* pPlayer = NULL;

    //1. 먼저 현재 클릭한 유저가 있는지 체크
    DWORD clicked_player_key = getParent()->GetClickedPlayerKey();
    if(clicked_player_key)
    {
        pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
    }
    else
    {
        //2. 클릭한 유저가 없으면 현재 Area에 들어온 유저가 있는지 체크.
        pPlayer = getParent()->GetFirstSatisfiedPlayer();
    }

    getMGR()->KillMonster(pActionInfo->GetMonsterCode(), pPlayer, pActionInfo->GetIsExpItemDrop());

}

__IMPL_POOL(ACTION_ADD_EXP)

class ApplyAddExpHandler
{
public:
    ApplyAddExpHandler(EXPTYPE exp) : add_exp_(exp) {}
    ~ApplyAddExpHandler() {}

    void operator()(Player* player)
    {
        //플레이어 상태검증
        if (player == NULL) {
            return;
        }
        if (player->IsDead()) {
            return;
        }

        player->AddExp(add_exp_, 0, 0, TRUE);
    }
private:
    EXPTYPE add_exp_;
};

void
ACTION_ADD_EXP::OnAction()
{
    const ACTION_ADD_EXP_INFO* pActionInfo = Get();
    if(!pActionInfo)        return;

    //이 트리거는 원래 클릭한 유저만 하기로 했지만, 보너스로 지역에 들어온 유저도 해주자. 단 지역에 들어온 유저는 개인만 된다.
    Player* pPlayer = NULL;
    if(pActionInfo->GetIsParty())
    {
        // 파티면... 현재 맵의 모든 유저에게 경험치를 지급한다.
        getMGR()->AddExpAllMember(pActionInfo->GetExp());
    }
    else
    {
        //개인이면..
        ApplyAddExpHandler op(pActionInfo->GetExp());
        getParent()->ForeachSatisfiedPlayer(op);
    }
}


//==================================================================================================

class CalcAttrOpr
{
public:
    CalcAttrOpr(const ACTION_MONSTER_STAT_UP_OR_DOWN_INFO* pInfo, GameField* pGameField) : m_pGameField(pGameField)
    {
        m_MonsterCode               = pInfo->GetMonsterCode();
        m_AbilityInfo.m_wAbilityID  = pInfo->GetAbilityID(); 
        m_AbilityInfo.m_iOption1    = eATTACK_TYPE_PHYSICAL_OPTION; // NOTE: f110719.4L, ...
        m_AbilityInfo.m_iParam[0]   = pInfo->GetAbilityParameter1();
        m_AbilityInfo.m_iParam[1]   = pInfo->GetAbilityParameter2();
        m_OperationType             = pInfo->GetOperationType();
        // NOTE: f110719.4L
        if (m_OperationType != ACTION_MONSTER_STAT_UP_OR_DOWN_INFO::eOPERATION_PLUS &&
            m_OperationType != ACTION_MONSTER_STAT_UP_OR_DOWN_INFO::eOPERATION_MINUS)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|[%s]|Msg=unexpected monster stat control operation type = %d ")
                   _T("of Npc(%d) of GameField(%d)|"),
                   __FUNCTION__, m_OperationType, m_MonsterCode, pGameField->GetFieldCode());
        };
        _CalcAttr();
    }

    ~CalcAttrOpr() {}

    void _CalcAttr()
    {
        typedef GameField::_NPC_HASH    _NPC_HASH;
        const _NPC_HASH& rMobs  = m_pGameField->GetRefNPCList();
        FOREACH_CONTAINER(const _NPC_HASH::value_type& rMob, rMobs, _NPC_HASH)
        {
            NPC* const npc = rMob.second;
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
            if (npc != NULL && npc->IsDeleted())
            {
                const Object::Key key = npc->GetObjectKey();
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    key
                );
                continue;
            }
//#endif
            _SetNPC(rMob.second);
        }
    }

    void _SetNPC(NPC* const npc)
    {
        if (npc->GetBaseInfo()->m_MonsterCode != m_MonsterCode) {
            return;
        };

        eATTR_TYPE attr_type = m_AbilityInfo.GetAttrType();
        if (attr_type <= eATTR_TYPE_INVALID || attr_type >= eATTR_MAX) {
            return;
        };

        SkillAttrCalculator skill_calculator(npc->GetAttr());
        if (m_OperationType == ACTION_MONSTER_STAT_UP_OR_DOWN_INFO::eOPERATION_PLUS) {
            skill_calculator.AddAttr(attr_type, \
                m_AbilityInfo.m_iParam[0], m_AbilityInfo.m_iParam[1]);
        }
        else if(m_OperationType == ACTION_MONSTER_STAT_UP_OR_DOWN_INFO::eOPERATION_MINUS) {
            skill_calculator.DelAttr(attr_type, \
                m_AbilityInfo.m_iParam[0], m_AbilityInfo.m_iParam[1]);
        }
    }

private:
    GameField*          m_pGameField;
    int                 m_MonsterCode;
    BASE_ABILITYINFO    m_AbilityInfo;
    BYTE                m_OperationType;
};


//==================================================================================================


// 
__IMPL_POOL(ACTION_MONSTER_STAT_UP_OR_DOWN)
void
ACTION_MONSTER_STAT_UP_OR_DOWN::OnAction()
{
    const ACTION_MONSTER_STAT_UP_OR_DOWN_INFO* pInfo = (const ACTION_MONSTER_STAT_UP_OR_DOWN_INFO*)Get();
    CalcAttrOpr(pInfo, getMGR()->GetField());

#ifdef __TRIGGER_TEST_LOG
    SUNLOG(eDEV_LOG,  "[TriggerID:%u]:ACTION_MONSTER_STAT_UP_OR_DOWN", getParent()->TriggerID());
#endif
}


//==================================================================================================


__IMPL_POOL(ACTION_RANDUM_TRIGER_ON_OR_OFF)
void
ACTION_RANDUM_TRIGER_ON_OR_OFF::OnAction()
{
    ACTION_RANDUM_TRIGER_ON_OR_OFF_INFO* pInfo = (ACTION_RANDUM_TRIGER_ON_OR_OFF_INFO*)Get();

    // 트리거ID는 최대한 8개를 넣을수 있도록 한다.
    // 랜덤으로 돌리려는 트리거가 3개라면 3개만 입력하고, 나머지 5개를 공란으로 넣어도 작용한다.
    int i = 0;
    for(; ACTION_RANDUM_TRIGER_ON_OR_OFF_INFO::_MAX_TRIGGER_ID > i; ++i)
    {
        if(pInfo->GetWzTriggerID(i) == 0) 
            break;
    }

    // 1개 짜리는 따로 있으므로 2이상이 들어온다.
    if(i > 1)
    {
        BYTE idx = random(1,i) - 1; // 1~8 => 0~7 로.
        DWORD WzTriggerID = pInfo->GetWzTriggerID(idx);
        if(ACTION_RANDUM_TRIGER_ON_OR_OFF_INFO::_TRIGGER_STATE_ON == pInfo->GetTriggerState())
            getMGR()->InsertActiveTrigger(WzTriggerID);
        else
            getMGR()->RequestRemoveActiveTrigger(WzTriggerID);
    }
}


//==================================================================================================

template<typename T> class NotifyMessageOpr
{

public:     
    NotifyMessageOpr(T& msg) : msg_(msg){}
    ~NotifyMessageOpr(){}

    void operator()(Player* pPlayer) 
    { 
        pPlayer->SendPacket(&msg_, sizeof(msg_));
    }

    void operator()(std::pair<DWORD,Player *> pair_type)
    {
        operator()(pair_type.second);
    }

private:
    T& msg_;
    //
    void operator=(const NotifyMessageOpr<T>&);
};

template<typename T> NotifyMessageOpr<T> NotifyMessage(T& msg)
{
    return NotifyMessageOpr<T>(msg);
}

// 
//{__NA001140_20080128_SOUND_TRIGGER_ADD
__IMPL_POOL(ACTION_PLAY_MUSIC)
void
ACTION_PLAY_MUSIC::OnAction()
{
    const ACTION_PLAY_MUSIC_INFO* pInfo = Get();

    // _NA001385_20090924_DOMINATION_TRIGGER [개인,파티 -> 개인,맵전체로 변경]
    
    const BYTE byTarget   = pInfo->GetTarget();
    const BYTE byField    = ACTION_PLAY_MUSIC_INFO::TARGET_FIELD;
    const BYTE byPersonal = ACTION_PLAY_MUSIC_INFO::TARGET_PERSONAL;    

    MSG_CG_TRIGGER_PLAY_MUSIC_CMD msg;
    msg.m_byTarget = byTarget;
    msg.m_nMusCode = pInfo->GetMusCode();

    switch(byTarget)
    {
    case byPersonal:
        {
            Player*  pPlayer  = NULL;
            Trigger* pTrigger = getParent();
            DWORD clickPlayer = pTrigger->GetClickedPlayerKey();

            if (clickPlayer)
                pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(clickPlayer);
            else
                pPlayer = pTrigger->GetFirstSatisfiedPlayer();


            if (pPlayer)
                pPlayer->SendPacket(&msg, sizeof(msg));
            else
                SUNLOG(eDEV_LOG, "[TriggerID:%u]:ACTION_PLAY_MUSIC : Cant not Find Player",
                                 pTrigger->TriggerID());
        }
        break;

    case byField:
        {
            getMGR()->ForeachPlayer(NotifyMessage(msg));
        }
        break;
    }
}
//}__NA001140_20080128_SOUND_TRIGGER_ADD

__IMPL_POOL(ACTION_DOWN_OBJETC_HP)
void
ACTION_DOWN_OBJETC_HP::OnAction()
{
    const ACTION_DOWN_OBJETC_HP_INFO* pInfo = Get();

    const DWORD  dwObjKey = pInfo->GetdwObjKey();
    MapNpc* pMapNpc = getMGR()->FindMapNpc(dwObjKey);
    if (NULL == pMapNpc)
        return;

    const BYTE  btHPPercentage = pInfo->GetbtHPPercentage();
    const DWORD dwMaxHP = pMapNpc->GetMaxHP();
    const DWORD dwDecrement = (DWORD)(dwMaxHP * (btHPPercentage/100.0f));

    pMapNpc->DecreaseHP(dwDecrement);
    pMapNpc->SendHPChange();
}


__IMPL_POOL(ACTION_CHANGE_ATTACK_KIND)
void
ACTION_CHANGE_ATTACK_KIND::OnAction()
{
    const ACTION_CHANGE_ATTACK_KIND_INFO* pInfo = Get();

    const DWORD  dwObjKey = pInfo->GetdwObjKey();
    MapNpc* pMapNpc = getMGR()->FindMapNpc(dwObjKey);
    if (NULL == pMapNpc)
        return;

    const BYTE attackImpossibility = ACTION_CHANGE_ATTACK_KIND_INFO::ATTACK_IMPOSSIBILITY;
    const BYTE attackPossibility   = ACTION_CHANGE_ATTACK_KIND_INFO::ATTACK_POSSIBILITY;

    const BYTE btPossibility = pInfo->GetbtPossibility();
    StatusManager* pStatusManager = pMapNpc->GetStatusManager();

    if (attackImpossibility == btPossibility) {
        //pStatusManager->AllocStatus(eCHAR_STATE_STEEL/*eCHAR_STATE_ETC_DISABLE_VISION_TRIGGER*/); <- X
        pStatusManager->AllocStatus(eCHAR_STATE_STEEL, 15 * 1000/*eCHAR_STATE_ETC_DISABLE_VISION_TRIGGER*/); //<- better...
#ifdef _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]:AllocStatus(eCHAR_STATE_STEEL, BASE_EXPIRE_TIME_INFINITY)");
#endif // #ifdef _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG
    }
    else if(attackPossibility == btPossibility) {
        pStatusManager->Remove(eCHAR_STATE_STEEL);
    }
}

class NotifyStampOpr
{

public:     
    NotifyStampOpr(){}
    ~NotifyStampOpr(){}
  
    void operator()(Player* pPlayer) 
    { 
        GUILDGUID guildGuid = pPlayer->GetGuildGuid();
        GameZone* pZone = pPlayer->GetGameZonePtr();

        if (!pZone || 
            INVALID_GUILDGUID == guildGuid ||
            eZONETYPE_DOMINATION_FIELD != pZone->GetZoneType())
            return;

        GameDominationField* pDomiZone = (GameDominationField*)pZone;

        if (guildGuid == pDomiZone->GetOffenseGuild())
            pPlayer->SendAllFieldCollectionInfo();
    }

    void operator()(std::pair<DWORD,Player *> pair_type)
    {
        operator()(pair_type.second);
    }
};

__IMPL_POOL(ACTION_ACTIVE_STAMP)
void
ACTION_ACTIVE_STAMP::OnAction()
{
    NotifyStampOpr op;
    getMGR()->ForeachPlayer(op);
}

//==================================================================================================

__IMPL_POOL(ACTION_ATTACK_OBJECTKIND)

class AattackActionNPCCodeTypeOpr
{
public:     
    AattackActionNPCCodeTypeOpr(const MONSTERCODE monster_code,
                                const eACTION_ATTACK_TYPE attack_type)
        : monster_code_(monster_code) 
        , attack_type_(attack_type) {}

    ~AattackActionNPCCodeTypeOpr(){}

    void operator()(NPC* npc) 
    {
        const BASE_NPCINFO* const base_info = npc->GetBaseInfo();
        const MONSTERCODE monster_code = base_info->m_MonsterCode;

        if (monster_code == monster_code_)
        {
            StatusManager* status_manager = npc->GetStatusManager();
            ACTION_ATTACK_OBJECTKIND::StatusProc(*status_manager, attack_type_);
        }
    }

    void operator()(std::pair<DWORD,NPC*> pair_type)
    {
        operator()(pair_type.second);
    }

private:
    const MONSTERCODE monster_code_;
    const eACTION_ATTACK_TYPE attack_type_;
};

void
ACTION_ATTACK_OBJECTKIND::OnAction()
{
    const ACTION_ATTACK_OBJECTKIND_INFO* info = Get();

    const DWORD object_key = info->GetdwObjectKey();
    const eACTION_ATTACK_TYPE attack_type = static_cast<eACTION_ATTACK_TYPE>(info->GetbyAttack());
    const eOBJECTCODE_TYPE object_type = static_cast<eOBJECTCODE_TYPE>(info->GetbyObjectType());
    
    ProcessFunction action;

    action = GetProcessFunction(object_type);
    (this->*action)(object_key, attack_type);
}

ACTION_ATTACK_OBJECTKIND::ProcessFunction 
ACTION_ATTACK_OBJECTKIND::GetProcessFunction(const eOBJECTCODE_TYPE object_type)
{
    switch(object_type)
    {
    case NPCCODE_TYPE:
        return &ACTION_ATTACK_OBJECTKIND::ActionNPCCodeType;

    case UNITID_TYPE:
        return &ACTION_ATTACK_OBJECTKIND::ActionUnitIDType;

    case MAPNPC_TYPE:
        return &ACTION_ATTACK_OBJECTKIND::ActionMapNpcType;
    }

    return &ACTION_ATTACK_OBJECTKIND::ActionDefaultType;
}

VOID 
ACTION_ATTACK_OBJECTKIND::ActionDefaultType(const DWORD object_key, 
                                            const eACTION_ATTACK_TYPE attack_type)
{
    __UNUSED((object_key, attack_type));
    SUNLOG(eDEV_LOG, "[ACTION_ATTACK_OBJECTKIND]:TYPE_ERROR");
}

VOID 
ACTION_ATTACK_OBJECTKIND::ActionNPCCodeType(const DWORD object_key, 
                                            const eACTION_ATTACK_TYPE attack_type)
{
    const MONSTERCODE npc_code = static_cast<MONSTERCODE>(object_key);
    AattackActionNPCCodeTypeOpr op(npc_code,attack_type);
    getMGR()->ForeachNPC(op);
}

VOID 
ACTION_ATTACK_OBJECTKIND::ActionUnitIDType(const DWORD object_key, 
                                           const eACTION_ATTACK_TYPE attack_type)
{
    NPC* const npc = getMGR()->FindNPCForUnitID(object_key);
    if (npc == NULL)
        return;

    StatusManager* const status_manager = npc->GetStatusManager();
    StatusProc(*status_manager, attack_type);
}

VOID 
ACTION_ATTACK_OBJECTKIND::ActionMapNpcType(const DWORD object_key, 
                                           const eACTION_ATTACK_TYPE attack_type)
{
    MapNpc* const map_npc = getMGR()->FindMapNpc(object_key);
    if (map_npc == NULL)
        return;

    StatusManager* const status_manager = map_npc->GetStatusManager();
    StatusProc(*status_manager, attack_type);
}

//[몬스터/유닛/오브젝트]를 공격 할 수 [있게/없게] 만든다 
// 특정 몬스터 객체가 플레이어에게 공격 당하지 않게 만든다.

VOID
ACTION_ATTACK_OBJECTKIND::StatusProc(StatusManager& status_manager, 
                                     const eACTION_ATTACK_TYPE attack_type)
{
    switch(attack_type)
    {
    case ATTACK_IMPOSSIBILITY:
        status_manager.AllocStatus(eCHAR_STATE_ETC_DISABLE_VISION_TRIGGER);            
        break;

    case ATTACK_POSSIBILITY:
        status_manager.Remove(eCHAR_STATE_ETC_DISABLE_VISION_TRIGGER);            
        break;

    default:
        break;
    }
}

//==================================================================================================

__IMPL_POOL(ACTION_ACTIVE_SKILL_OBJECTKIND)

class ActiveSkillActionNPCCodeTypeOpr
{
public:     
    ActiveSkillActionNPCCodeTypeOpr(const MONSTERCODE monster_code, const SLOTCODE skill_code)
        : monster_code_(monster_code),
        skill_code_(skill_code)
    {}

    ~ActiveSkillActionNPCCodeTypeOpr(){}

    void operator()(NPC* npc) 
    {
        const BASE_NPCINFO* const base_info = npc->GetBaseInfo();
        const MONSTERCODE monster_code = base_info->m_MonsterCode;

        if (monster_code == monster_code_)
        {
            npc->UseSkill(skill_code_);
        }
    }

    void operator()(std::pair<DWORD,NPC*> pair_type)
    {
        operator()(pair_type.second);
    }

private:
    const MONSTERCODE monster_code_;
    const SLOTCODE skill_code_;
};

void
ACTION_ACTIVE_SKILL_OBJECTKIND::OnAction()
{
    const ACTION_ACTIVE_SKILL_OBJECTKIND_INFO* info = Get();

    const DWORD object_key = info->GetdwObjectKey();
    const eOBJECTCODE_TYPE object_type = static_cast<eOBJECTCODE_TYPE>(info->GetbyObjectType());
    const SLOTCODE skill_code = static_cast<SLOTCODE>(info->GetdwSkillCode());    

    NPC* npc = NULL;

    switch(object_type)
    {
    case UNITID_TYPE:
        npc = getMGR()->FindNPCForUnitID(object_key);
        if (npc != NULL)
            npc->UseSkill(skill_code);
        break;

    case MAPNPC_TYPE:
        npc = getMGR()->FindMapNpc(object_key);
        if (npc != NULL)
            npc->UseSkill(skill_code);
        break;

    case NPCCODE_TYPE:
        {
            const MONSTERCODE npc_code = static_cast<MONSTERCODE>(object_key);
            ActiveSkillActionNPCCodeTypeOpr op(npc_code, skill_code);
            getMGR()->ForeachNPC(op);
        }
        break;

    default:
        SUNLOG(eDEV_LOG, "[ACTION_ACTIVE_SKILL_OBJECTKIND]:TYPE_ERROR");
        return;
    }

}

//==================================================================================================

__IMPL_POOL(ACTION_SHOW_MESSAGEBOX_OBJECTKIND)

class ShowMessageBoxActionNPCCodeTypeOpr
{
public:     
    ShowMessageBoxActionNPCCodeTypeOpr(const MONSTERCODE monster_code, const DWORD message_code)
        : monster_code_(monster_code)
    {
        msg_cmd_.m_dwMessageCode = message_code;
    }

    ~ShowMessageBoxActionNPCCodeTypeOpr(){}

    void operator()(NPC* npc) 
    {
        const BASE_NPCINFO* const base_info = npc->GetBaseInfo();
        const MONSTERCODE monster_code = base_info->m_MonsterCode;

        if (monster_code == monster_code_)
        {
            msg_cmd_.m_dwObjectKey = npc->GetObjectKey();
            npc->SendPacketAround(&msg_cmd_, sizeof(msg_cmd_));
        }
    }

    void operator()(std::pair<DWORD,NPC*> pair_type)
    {
        operator()(pair_type.second);
    }

private:
    const MONSTERCODE monster_code_;
    MSG_CG_TRIGGER_DISPLAY_BALLOON_MSGBOX_CMD msg_cmd_;
};

void
ACTION_SHOW_MESSAGEBOX_OBJECTKIND::OnAction()
{
    const ACTION_SHOW_MESSAGEBOX_OBJECTKIND_INFO* info = Get();

    const DWORD object_key = info->GetdwObjectKey();
    const eOBJECTCODE_TYPE object_type = static_cast<eOBJECTCODE_TYPE>(info->GetbyObjectType());
    const DWORD message_code = info->GetdwMessageCode();    

    MSG_CG_TRIGGER_DISPLAY_BALLOON_MSGBOX_CMD msg_cmd;
    msg_cmd.m_dwMessageCode = message_code;

    NPC* npc = NULL;

    switch(object_type)
    {
    case UNITID_TYPE:
        npc = getMGR()->FindNPCForUnitID(object_key);
        if (npc != NULL)
        {
            msg_cmd.m_dwObjectKey = npc->GetObjectKey();
            npc->SendPacketAround(&msg_cmd, sizeof(msg_cmd));
        }

        break;

    case MAPNPC_TYPE:
        npc = getMGR()->FindMapNpc(object_key);
        if (npc != NULL)
        {
            msg_cmd.m_dwObjectKey = npc->GetObjectKey();
            npc->SendPacketAround(&msg_cmd, sizeof(msg_cmd));
        }

        break;

    case NPCCODE_TYPE:
        {
            const MONSTERCODE npc_code = static_cast<MONSTERCODE>(object_key);
            ShowMessageBoxActionNPCCodeTypeOpr op(npc_code, message_code);
            getMGR()->ForeachNPC(op);
        }
        break;

    default:
        SUNLOG(eDEV_LOG, "[ACTION_SHOW_MESSAGEBOX_OBJECTKIND]:TYPE_ERROR");
        return;
    }


}

//==================================================================================================

__IMPL_POOL(ACTION_SET_ATTACK_STATE_OBJECTKIND)

class SetAttackStateActionNPCCodeTypeOpr
{
public:     
    SetAttackStateActionNPCCodeTypeOpr(const MONSTERCODE monster_code, 
                                       const eACTION_ATTACK_TYPE attack_type,
                                       const eTARGET_SEARCH_TYPE search_type,
                                       const DWORD dest_object_key)
        : monster_code_(monster_code),
        attack_type_(attack_type),
        search_type_(search_type),
        dest_object_key_(dest_object_key){}

    ~SetAttackStateActionNPCCodeTypeOpr(){}

    void operator()(NPC* npc) 
    {
        const BASE_NPCINFO* const base_info = npc->GetBaseInfo();
        const MONSTERCODE monster_code = base_info->m_MonsterCode;

        if (monster_code == monster_code_)
        {
            ACTION_SET_ATTACK_STATE_OBJECTKIND::StatusProc(npc, attack_type_, 
                                                            search_type_, dest_object_key_);
        }
    }

    void operator()(std::pair<DWORD,NPC*> pair_type)
    {
        operator()(pair_type.second);
    }

private:
    const MONSTERCODE monster_code_;
    const eACTION_ATTACK_TYPE attack_type_;
    const eTARGET_SEARCH_TYPE search_type_;
    const DWORD dest_object_key_;
};


VOID
ACTION_SET_ATTACK_STATE_OBJECTKIND::StatusProc(NPC* src_npc, 
                                               const eACTION_ATTACK_TYPE attack_type,
                                               const eTARGET_SEARCH_TYPE search_type,
                                               const DWORD dest_object_key)
{
    StatusManager* const status_manager = src_npc->GetStatusManager();

    switch(attack_type)
    {
    case ATTACK_IMPOSSIBILITY:
        {
            const BASE_NPCINFO* const info = src_npc->GetBaseInfo();
            const BYTE attitude = info->m_byAttitude;
            const eTARGET_SEARCH_TYPE search_type = src_npc->FindSearchTypeByAttitude(attitude);

            src_npc->SetSearchType(search_type, 0);
            status_manager->Remove(eCHAR_STATE_ETC_SKIP_UPDATE_TARGET_LIST);
            src_npc->ChangeState(STATE_ID_WANDER);
        }
        break;

    case ATTACK_POSSIBILITY:
        {
            src_npc->SetSearchType(search_type, dest_object_key);
            status_manager->AllocStatus(eCHAR_STATE_ETC_SKIP_UPDATE_TARGET_LIST);

            TargetManager* const target_manager = src_npc->GetTargetManager();
            target_manager->ClearBattleRecord();
        }
        break;

    default:
        break;
    }
}

void
ACTION_SET_ATTACK_STATE_OBJECTKIND::OnAction()
{
    const ACTION_SET_ATTACK_STATE_OBJECTKIND_INFO* info = Get();


    const DWORD object_key = info->GetdwObjectKey();
    const DWORD dest_object_key = info->GetdwDestObjectKey();

    eACTION_ATTACK_TYPE action_attack_type = 
        static_cast<eACTION_ATTACK_TYPE>(info->GetbyAttack());

    const eOBJECTCODE_TYPE object_type = static_cast<eOBJECTCODE_TYPE>(info->GetbyObjectType());
    const eOBJECTCODE_TYPE dest_object_type =
        static_cast<eOBJECTCODE_TYPE>(info->GetbyDestObjectType());

    //.//

    eTARGET_SEARCH_TYPE search_type = eRARGET_NOT_SEARCH;
    switch(dest_object_type)
    {
    case UNITID_TYPE:
        search_type = eRARGET_SEARCH_UNITID;
        break;
    case MAPNPC_TYPE:
        search_type = eRARGET_SEARCH_MAPOBJECTID;
        break;
    case NPCCODE_TYPE:
        search_type = eRARGET_SEARCH_MONSTERCODE;
        break;
    case NON_TYPE:  // _NA_0_20100817_HELLON_LAST_TRIGGER (공격이 멈춰진 상태)
        search_type = eRARGET_SEARCH_NOT_SEARCH_AND_TARGET_CHNAGE;
        action_attack_type = static_cast<eACTION_ATTACK_TYPE>(!info->GetbyAttack());
        break;
    }

    //.//

    NPC* src_npc = NULL;

    switch(object_type)
    {
    case UNITID_TYPE:
        src_npc = getMGR()->FindNPCForUnitID(object_key);
        break;

    case MAPNPC_TYPE:
        src_npc = getMGR()->FindMapNpc(object_key);
        break;
    }

    //.//

    if (src_npc != NULL)
    {
        StatusProc(src_npc, action_attack_type, search_type, dest_object_key);
    }
    else
    {
        const MONSTERCODE npc_code = static_cast<MONSTERCODE>(object_key);
        SetAttackStateActionNPCCodeTypeOpr op(npc_code, action_attack_type,
                                              search_type, dest_object_key);
        getMGR()->ForeachNPC(op);
    }
}

//==================================================================================================

__IMPL_POOL(ACTION_DESTROY_OBJECTKIND)
void
ACTION_DESTROY_OBJECTKIND::OnAction()
{
    const ACTION_DESTROY_OBJECTKIND_INFO* info = Get();

    const DWORD object_key = info->GetdwObjectKey();
    const BOOL is_reward = 
        info->GetbyReward() == ACTION_DESTROY_OBJECTKIND_INFO::REWARD ? TRUE : FALSE;

    Player* player = NULL;
    const DWORD clicked_player_key = getParent()->GetClickedPlayerKey();

    if(clicked_player_key)
        player = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
    else
        player = getParent()->GetFirstSatisfiedPlayer();

    NPC* npc = NULL;
    const eOBJECTCODE_TYPE object_type = static_cast<eOBJECTCODE_TYPE>(info->GetbyObjectType());

    switch(object_type)
    {
    case NPCCODE_TYPE:
        getMGR()->KillMonster(static_cast<MONSTERCODE>(object_key), player, is_reward);
        return;

    case UNITID_TYPE:
        npc = getMGR()->FindNPCForUnitID(object_key);
        if (npc)
            getMGR()->KillOneMonster(npc, player, is_reward);
        break;

    case MAPNPC_TYPE:
        npc = getMGR()->FindMapNpc(object_key);
        if (npc)
            getMGR()->KillOneMonster(npc, player, is_reward);
        break;

    default:
        SUNLOG(eDEV_LOG, "[ACTION_DESTROY_OBJECTKIND]:TYPE_ERROR");
        return;
    }
}


__IMPL_POOL(ACTION_CREATE_MONSTER_LINKED_MAPOBJECT)
void
ACTION_CREATE_MONSTER_LINKED_MAPOBJECT::OnAction()
{
    const ACTION_CREATE_MONSTER_LINKED_MAPOBJECT_INFO* pInfo = 
        (const ACTION_CREATE_MONSTER_LINKED_MAPOBJECT_INFO*)Get();

    const DWORD mapobjectID = pInfo->GetdwObjectKey();
    MAPOBJECT_INFO* mapobject_info = getMGR()->FindMapObjectInfo(mapobjectID);

    DWORD dwCode = mapobject_info->dwExtraInfos[eMAPOBJECT_MONSTER];
    ASSERT(SAFE_NUMERIC_TYPECAST( DWORD, dwCode, WORD ));
    WORD NPCCode = (WORD)dwCode;

    getMGR()->CreateMapNPC( NPCCode, &mapobject_info->wvPos, mapobject_info );
}
//==================================================================================================

__IMPL_POOL(ACTION_CONTROL_TIMER)
void
ACTION_CONTROL_TIMER::OnAction()
{
    const ACTION_CONTROL_TIMER_INFO* pInfo = (const ACTION_CONTROL_TIMER_INFO*)Get();

    MSG_CG_TRIGGER_CONTROL_TIMER_CMD msg;
    msg.m_dwTime = pInfo->GetdwTime();
    msg.m_byControlType = pInfo->GetbyControlType();

    getMGR()->ForeachPlayer(NotifyMessage(msg));
}

// implemented by _NA_0_20100705_DOMINATION_EVENT
__IMPL_POOL(ACTION_DO_COMMAND)
void
ACTION_DO_COMMAND::OnAction()
{
    const ACTION_DO_COMMAND_INFO* pInfo = (const ACTION_DO_COMMAND_INFO*)Get();

    switch(pInfo->GetbyCommandType())
    {
    case ACTION_DO_COMMAND_INFO::END_REGEN_EVENT:   // 점령전 보상 이벤트 트리거의 종료 메시지(보상 종료 처리)
        {
            GameField* field = getMGR()->GetField();
            GameZone* zone = field->GetGameZone();
            if (zone->GetZoneType() != eZONETYPE_DOMINATION_FIELD)
            {
                return;
            }

            GameDominationField* domination = static_cast<GameDominationField*>(zone);
            domination->EndEventTrigger();
            //_NA_20100604_BETTER_DOMINATION_MANAGER
            //점령전 보상 몬스터 사냥 카운트 횟수를 늘린다.
            domination->UpdateGuildInfoByDomination(field->GetFieldCode());
        }
        break;
    }
}    

// ACTION_MOVE_CONTROL was implemented by _NA_0_20100817_HELLON_LAST_TRIGGER

__IMPL_POOL(ACTION_MOVE_CONTROL)

class MoveControlNPCCodeTypeOpr
{
public:     
    MoveControlNPCCodeTypeOpr(const MONSTERCODE monster_code, const eACTION_MOVE_TYPE move_type)
        : monster_code_(monster_code)
        , move_type_(move_type)
    {}

    ~MoveControlNPCCodeTypeOpr(){}

    void operator()(NPC* npc) 
    {
        const BASE_NPCINFO* const base_info = npc->GetBaseInfo();
        const MONSTERCODE monster_code = base_info->m_MonsterCode;

        if (monster_code == monster_code_)
            ACTION_MOVE_CONTROL::StatusProc(npc, move_type_);
    }

    void operator()(std::pair<DWORD,NPC*> pair_type)
    {
        operator()(pair_type.second);
    }

private:
    const MONSTERCODE monster_code_;
    const eACTION_MOVE_TYPE move_type_;
};

VOID
ACTION_MOVE_CONTROL::StatusProc(NPC* npc, const eACTION_MOVE_TYPE move_type)
{
    StatusManager* const status_manager = npc->GetStatusManager();

    switch(move_type)
    {
    case MOVE_IMPOSSIBILITY:
        status_manager->AllocStatus(eCHAR_STATE_ETC_TRIGGER_HOLDING);
        npc->ChangeState(STATE_ID_STOP_IDLE);
        break;

    case MOVE_POSSIBILITY:
        status_manager->Remove(eCHAR_STATE_ETC_TRIGGER_HOLDING);        
        npc->ChangeState(STATE_ID_WANDER);
        break;

    default:
        break;
    }
}

void
ACTION_MOVE_CONTROL::OnAction()
{
    const ACTION_MOVE_CONTROL_INFO* pInfo = (const ACTION_MOVE_CONTROL_INFO*)Get();

    const DWORD object_key = pInfo->GetdwObjectKey();
    const eOBJECTCODE_TYPE object_type = static_cast<eOBJECTCODE_TYPE>(pInfo->GetbyObjectType());
    const eACTION_MOVE_TYPE move_type = static_cast<eACTION_MOVE_TYPE>(pInfo->GetbyMove());

    switch(object_type)
    {
    case UNITID_TYPE:
        {
            NPC* npc = getMGR()->FindNPCForUnitID(object_key);
            if (npc)
                StatusProc(npc, move_type);
        }
        break;

    case NPCCODE_TYPE:
        {
            const MONSTERCODE npc_code = static_cast<MONSTERCODE>(object_key);
            MoveControlNPCCodeTypeOpr op(npc_code, move_type);
            getMGR()->ForeachNPC(op);
        }
        break;

    default:
        SUNLOG(eDEV_LOG, "[ACTION_MOVE_CONTROL]:TYPE_ERROR");
        return;
    }
}

__IMPL_POOL(ACTION_ANIMATION_CONTROL)

class AnimationControlNPCCodeTypeOpr
{
public:     
    AnimationControlNPCCodeTypeOpr(const MONSTERCODE monster_code, const DWORD animation_code)
        : monster_code_(monster_code)
        , animation_code_(animation_code)
    {}

    ~AnimationControlNPCCodeTypeOpr(){}

    void operator()(NPC* npc) 
    {
        const BASE_NPCINFO* const base_info = npc->GetBaseInfo();
        const MONSTERCODE monster_code = base_info->m_MonsterCode;

        if (monster_code == monster_code_)
            npc->SetTriggerAnimationCode(animation_code_);
    }

    void operator()(std::pair<DWORD,NPC*> pair_type)
    {
        operator()(pair_type.second);
    }

private:
    const MONSTERCODE monster_code_;
    const DWORD animation_code_;
};

void
ACTION_ANIMATION_CONTROL::OnAction()  
{    
    const ACTION_ANIMATION_CONTROL_INFO* pInfo = (const ACTION_ANIMATION_CONTROL_INFO*)Get();

    DWORD animation_code = 0;
    if (pInfo->GetbyLoop2() == ACTION_ANIMATION_CONTROL_INFO::LOOP)
    {
        animation_code = pInfo->GetdwAnimationCode2();
    }
    else if (pInfo->GetbyLoop1() == ACTION_ANIMATION_CONTROL_INFO::LOOP)
    {
        animation_code = pInfo->GetdwAnimationCode1();
    }

    const DWORD object_key = pInfo->GetdwObjectKey();
    const eOBJECTCODE_TYPE object_type = static_cast<eOBJECTCODE_TYPE>(pInfo->GetbyObjectType());

    switch(object_type)
    {
    case UNITID_TYPE:
        {
            NPC* npc = getMGR()->FindNPCForUnitID(object_key);
            if (npc)
                npc->SetTriggerAnimationCode(animation_code);
        }
        break;
    
    case MAPNPC_TYPE:
        {
            NPC* npc = getMGR()->FindMapNpc(object_key);
            if (npc)
                npc->SetTriggerAnimationCode(animation_code);
        }
        break;

    case NPCCODE_TYPE:
        {
            const MONSTERCODE npc_code = static_cast<MONSTERCODE>(object_key);
            AnimationControlNPCCodeTypeOpr op(npc_code, animation_code);
            getMGR()->ForeachNPC(op);
        }
        break;

    default:
        SUNLOG(eDEV_LOG, "[ACTION_ANIMATION_CONTROL]:TYPE_ERROR");
        return;
    }
}    

// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
__IMPL_POOL(ACTION_NPC_APPLY_DAMAGE)

class NPCApplyDamageOper
{
public:     
    NPCApplyDamageOper(const MONSTERCODE monster_code, const INT damage) : 
        monster_code_(monster_code), damage_(damage)
    {
    }
    ~NPCApplyDamageOper() {}

    void operator()(NPC* npc) 
    {
        const BASE_NPCINFO* const base_info = npc->GetBaseInfo();
        const MONSTERCODE monster_code = base_info->m_MonsterCode;
        if (monster_code == monster_code_)
        {
            npc->DecreaseHP(damage_);
            npc->SendAreaDamage(damage_);
        }
    }
    void operator()(std::pair<DWORD, NPC*> pair_type)
    {
        operator()(pair_type.second);
    }

private:
    const MONSTERCODE monster_code_;
    const INT damage_;
};

void ACTION_NPC_APPLY_DAMAGE::OnAction()
{
    const ACTION_NPC_APPLY_DAMAGE_INFO* const apply_damage_info = Get();
    const eOBJECTCODE_TYPE object_type = 
        static_cast<eOBJECTCODE_TYPE>(apply_damage_info->GetObjectType());
    const DWORD object_key = apply_damage_info->GetObjectKey();
    const INT damage = apply_damage_info->GetDamage();

    switch(object_type)
    {
    case UNITID_TYPE:
        {
            NPC* const npc_unit = getMGR()->FindNPCForUnitID(object_key);
            if (npc_unit != NULL)
            {
                npc_unit->DecreaseHP(damage);
                npc_unit->SendAreaDamage(damage);
            }
        }
        break;
    case MAPNPC_TYPE:
        {
            NPC* const npc_map_obj = getMGR()->FindMapNpc(object_key);
            if (npc_map_obj != NULL)
            {
                npc_map_obj->DecreaseHP(damage);
                npc_map_obj->SendAreaDamage(damage);
            }
        }
        break;
    case NPCCODE_TYPE:
        {
            const MONSTERCODE monster_code = static_cast<MONSTERCODE>(object_key);
            NPCApplyDamageOper oper(monster_code, damage);
            getMGR()->ForeachNPC(oper);
        }
        break;
    default:
        SUNLOG(eDEV_LOG, "[ACTION_NPC_APPLY_DAMAGE]:TYPE_ERROR");
        return;
    }
}

//------------------------------------------------------------------------------
//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
//------------------------------------------------------------------------------
class CollectAllCollectiveCmd
{
public:
    CollectAllCollectiveCmd(GameField* game_field, bool val, CODETYPE* object_key_ptr) : 
          game_field_(game_field),
          is_active_(val),
          object_key_ptr_(object_key_ptr)
    {
    }
    
    void operator()(Player* player) 
    { 
        if (is_active_) {
            player->SendAllFieldCollectionInfo();
        }
        else
        {
            CollectionManager* const collection_manager = game_field_->GetCollectManeger();
            if (collection_manager == NULL)
            {
                return;
            }

            MSG_CG_COLLECT_DEACTIVE_BRD brd_msg;
            brd_msg.m_byCount = 0;
            for (int i = 0; i < MSG_CG_COLLECT_DEACTIVE_BRD::_MAX_FIELDCOLLECT_INFO_SIZE; ++i)
            {
                CODETYPE object_key = MAP_OBJECT_KEY + object_key_ptr_[i];
                Collection* collection = static_cast<Collection*>(game_field_->FindObject(COLLECTION_OBJECT, object_key));
                if (obj_util::IsValid(collection) == false)
                {
                    continue;
                }
                if (collection->is_active() == true)
                {
                    continue;
                }

                MAPOBJECT_INFO* pMapInfo = collection->GetMapObjectInfo();
                RC::eCOLLECT_RESULT rc   = collection_manager->ChkCondision(player, pMapInfo->CollectionID);
                if(RC::RC_COLLECT_SUCCESS != rc)
                    continue;
                
                brd_msg.m_ObjKeyCollection[brd_msg.m_byCount++] = collection->GetObjectKey();
            }

            if(brd_msg.m_byCount)
            {
                player->SendPacket(&brd_msg, (WORD)brd_msg.GetSize());
            }
        }

    }
    void operator()(std::pair<DWORD,Player *> pair_type)
    {
        operator()(pair_type.second);
    }

private:
    GameField* game_field_;
    bool is_active_;
    CODETYPE* object_key_ptr_;
};

//------------------------------------------------------------------------------
__IMPL_POOL(ACTION_COMPLETE_COLLECTION)

//------------------------------------------------------------------------------
VOID ACTION_COMPLETE_COLLECTION::OnAction()
{
    GameField* const field = getMGR()->GetField();
    if (field == NULL)
    {
        return;
    }
    CollectionManager* const collection_manager = field->GetCollectManeger();
    if (collection_manager == NULL)
    {
        return;
    }

    ACTION_COMPLETE_COLLECTION_INFO* const info = Get();
    if (info == NULL)
    {
        return;
    }

    for (int i = 0; i < ACTION_COMPLETE_COLLECTION_INFO::kMaxCollectObject; ++i)
    {
        CODETYPE object_key = MAP_OBJECT_KEY + info->GetObjectKey(i);
        Collection* collection = static_cast<Collection*>(field->FindObject(COLLECTION_OBJECT, object_key));
        if (collection != NULL)
        {
            collection->set_active(info->GetEnableType() != 0);
        }
    }

    CollectAllCollectiveCmd op(field, info->GetEnableType() != 0, info->GetObjectKeyPtr());
    getMGR()->ForeachPlayer(op);
}
//-------------------------------------------------------------------------------------------------
// _NA_000000_20130104_ADD_ACTION_TRIGGER_APPLY_HPMPSD
//-------------------------------------------------------------------------------------------------
__IMPL_POOL(ACTION_APPLY_HPMPSD)

class ApplyHPMPSDHandler
{
public:
    ApplyHPMPSDHandler(const BYTE point_type, const DWORD figure, const BYTE apply_type, const int team_key) :
            point_type_(point_type),
            figure_(figure),
            apply_type_(apply_type),
            team_key_(team_key),
            is_team_(false)
      {
          if (team_key_ > -1)
              is_team_ = true;
      }
    ~ApplyHPMPSDHandler(){}

    void operator()(Player* player) 
    { 
        ASSERT(player);

        // 플레이어가 죽었다면 트리거 적용대상에서 제외..
        if (player->IsDead())
            return;

        // 플레어의 팀을 고려해야 한다면..
        if (is_team_)
        {
            if (team_key_ != player->GetTeamKey())
                return;
        }
        
        DWORD apply_figure = 0;
        switch(point_type_)
        {
        case ACTION_APPLY_HPMPSD_INFO::kHP:
            {                                         
                if (apply_type_ == ACTION_APPLY_HPMPSD_INFO::kIncrease)
                {
                    apply_figure = player->IncreaseHP(figure_);

#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
                    // 소환수 고려..
                    SummonedNPCs* const summon_npc = \
                        SummonManager::Instance()->FindSummonNPCs(player->GetSummonerKey());
                    if (summon_npc == NULL)
                        break;
                    Summoned* const summoned = summon_npc->FindSummoned(0);
                    if (summoned == NULL)
                        break;
                    
                    summoned->IncreaseHP(figure_);
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
                }
                else
                {
                    apply_figure = player->DecreaseHP(figure_);
                    MSG_CG_TRIGGER_AREADAMAGE_BRD msg;
                    msg.m_dwObjectKey = player->GetObjectKey();
                    msg.m_wDamage     = figure_;
                    msg.m_dwHP        = player->GetHP();
                    player->SendPacketAround((MSG_BASE_FORWARD*)&msg, sizeof(msg));

#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
                    // 소환수 고려..
                    SummonedNPCs* const summon_npc = \
                        SummonManager::Instance()->FindSummonNPCs(player->GetSummonerKey());
                    if (summon_npc == NULL)
                        break;
                    // 아직 소환할수있는 개체수가 1개이기 때문에 objectkey값을 구현하지 않았다..
                    // object 키값을 구현하지 않아 말도 안되는 0값으로 소환수를 찾는다..
                    // FindSummoned 함수 내부를 보면 오브젝트 키값이 있으면 안되는 구조다..
                    Summoned* const summoned = summon_npc->FindSummoned(0);
                    if (summoned)
                    {
                        summoned->DecreaseHP(figure_);
                        MSG_CG_TRIGGER_AREADAMAGE_BRD msg;
                        msg.m_dwObjectKey = summoned->GetObjectKey();
                        msg.m_wDamage     = figure_;
                        msg.m_dwHP        = summoned->GetHP();
                        player->SendPacketAround((MSG_BASE_FORWARD*)&msg, sizeof(msg));
                    }
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
                }
            }
            break;

        case ACTION_APPLY_HPMPSD_INFO::KMP:
            {
                if (apply_type_ == ACTION_APPLY_HPMPSD_INFO::kIncrease)
                    apply_figure = player->IncreaseMP(figure_);
                else
                    apply_figure = player->DecreaseMP(figure_);
            }
            break;

        case ACTION_APPLY_HPMPSD_INFO::KSD:
            {
                if (apply_type_ == ACTION_APPLY_HPMPSD_INFO::kIncrease)
                    apply_figure = player->IncreaseSD(figure_);
                else
                    apply_figure = player->DecreaseSD(figure_);
            }
            break;
        }
    }

    void operator()(std::pair<DWORD,Player *> pair_type)
    {
        operator()(pair_type.second);
    }

private:
    BYTE point_type_; // HP/MP/SD
    DWORD figure_;    // 수치
    BYTE apply_type_; // 증가/감소
    int team_key_;
    bool is_team_;
};

void
ACTION_APPLY_HPMPSD::OnAction()
{
    const ACTION_APPLY_HPMPSD_INFO* const apply_hpmpsd_info = Get();

    ApplyHPMPSDHandler op(apply_hpmpsd_info->GetPointType(),
                          apply_hpmpsd_info->GetFigure(),
                          apply_hpmpsd_info->GetApplyType(),
                          apply_hpmpsd_info->GetTeamKey());

    const BYTE target_type = apply_hpmpsd_info->GetTargetType();
    switch(target_type)
    {
    case ACTION_APPLY_HPMPSD_INFO::kPerson: // 개인
        {
            Player* player  = NULL;
            Trigger* trigger = getParent();

            DWORD click_player = trigger->GetClickedPlayerKey();
            if (click_player)
                player = PlayerManager::Instance()->FindPlayerByObjKey(click_player);
            else
                player = trigger->GetFirstSatisfiedPlayer();

            if (player)
                op(player);
            else
                SUNLOG(eDEV_LOG, "[TriggerID:%u]:ACTION_APPLY_DAMAGE : Cant not Find Player",
                trigger->TriggerID());
        }
        break;
    case ACTION_APPLY_HPMPSD_INFO::kField: // 전체
        getMGR()->ForeachPlayer(op);
        break;

    case ACTION_APPLY_HPMPSD_INFO::kTeam: // 팀
        getParent()->ForeachSatisfiedPlayer(op);
        break;
    }
}
//==================================================================================================

//-------------------------------------------------------------------------------------------------
// _NA_006658_20130422_ADD_ACTION_TRIGGER_CHANGE_OBJECT_HP
//-------------------------------------------------------------------------------------------------
__IMPL_POOL(ACTION_CHANGE_OBJECT_HP)
void ACTION_CHANGE_OBJECT_HP::OnAction()
{
    const ACTION_CHANGE_OBJECT_HP_INFO* const info = Get();

    const DWORD  object_key = info->GetObjectkey(); // 해당 오브젝트 키
    MapNpc* const map_npc = getMGR()->FindMapNpc(object_key);
    if (NULL == map_npc) {
        return;
    }

    const BYTE calc_type  = info->GetCalcType(); // 계산 타입 (비율/수치)
    const BYTE apply_type = info->GetApplyType(); // 적용 타입 (증가/감소)
    DWORD figure = info->GetFigure(); // 값

    const DWORD max_hp = map_npc->GetMaxHP();
    DWORD change_hp = 0;

    if (calc_type == ACTION_CHANGE_OBJECT_HP_INFO::kPersentage) 
    {
        if (figure > 100) {
            figure = 100;
        }
        change_hp = static_cast<DWORD>(max_hp * (figure / 100.0f));
    }
    else if(calc_type == ACTION_CHANGE_OBJECT_HP_INFO::kValue) {
        change_hp = figure;
    }

    if (apply_type == ACTION_CHANGE_OBJECT_HP_INFO::kIncrease) {
        map_npc->IncreaseHP(change_hp);
    }
    else if(apply_type == ACTION_CHANGE_OBJECT_HP_INFO::kDecrease) {
        map_npc->DecreaseHP(change_hp);
    }

    map_npc->SendHPChange();
}
//=================================================================================================

//-------------------------------------------------------------------------------------------------
// _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
//-------------------------------------------------------------------------------------------------
class ChangeRadarMapHandler
{
public:
    explicit ChangeRadarMapHandler(const WzID sub_map_id_) : sub_map_id_(sub_map_id_)
    {
    }
    ~ChangeRadarMapHandler()
    {
    }
    void operator()(Player* player) 
    {
        MSG_CG_TRIGGER_CHANGE_RADAR_MAP_CMD cmd_msg;
        cmd_msg.sub_map_id = sub_map_id_;
        player->SendPacket(&cmd_msg, sizeof(cmd_msg));
    }
    void operator()(std::pair<DWORD,Player *> pair_type) 
    { 
        operator()(pair_type.second);
    }
private:
    WzID sub_map_id_;
};

__IMPL_POOL(ACTION_CHANGE_RADAR_MAP)
void ACTION_CHANGE_RADAR_MAP::OnAction()
{
    ACTION_CHANGE_RADAR_MAP_INFO* info = Get();
    const WzID sub_map_id = info->GetSubMapID();

    DWORD clicked_player_key = getParent()->GetClickedPlayerKey();
    if(clicked_player_key != 0)
    {
        Player* const player = PlayerManager::Instance()->FindPlayerByObjKey(clicked_player_key);
        if (obj_util::IsValid(player))
        {
            ChangeRadarMapHandler op(sub_map_id);
            op(player);
        }
    }
    else
    {
        ChangeRadarMapHandler op(sub_map_id);
        getParent()->ForeachSatisfiedPlayer(op);
        getParent()->RemoveAllSatisfiedPlayer();
    }
    
}
//=================================================================================================

//-------------------------------------------------------------------------------------------------
// _NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY
//-------------------------------------------------------------------------------------------------
__IMPL_POOL(ACTION_ATTACH_STATE_RANGE)

class AttachStateRangeHandler
{
public:
    AttachStateRangeHandler(DWORD object_key, WzVector* pos, float range, eCHAR_STATE_TYPE state_type, DWORD time_duration, BYTE apply_type)
    {
        object_key_ = object_key;
        object_pos_ = *pos;
        range_ = range * range;
        state_type_ = state_type;
        time_duration_ = time_duration;
        apply_type_ = apply_type;
    }
    ~AttachStateRangeHandler(){}

    void operator()(Player* const player)
    {
        const WzVector& cur_pos = *player->GetPosPtr();

        WzVector distance = object_pos_ - cur_pos;
        float distance2 = VectorLength2(&distance);

        if(distance2 <= range_)
        {
            if (apply_type_ == 0)
            {
                // 상태부여
                StatusManager* const status_manager = player->GetStatusManager();
                ALLOC_STATUS_PAIR steel = 
                    status_manager->AllocStatus(state_type_, time_duration_);
                if (steel.second)
                    steel.second->SendStatusAddBRD();
            }
            else if (apply_type_ == 1)
            {
                // 상태해제
                if (!player->GetStatusManager()->Remove(state_type_))
                {
                    MSG_CG_STATUS_REMOVE_BRD brd_msg;
                    brd_msg.m_dwTargetKey = player->GetObjectKey();
                    brd_msg.m_dwStatusCode = static_cast<DWORD>(state_type_);
                    player->SendPacketAround(&brd_msg, brd_msg.GetSize());
                }
            }
        }
    }

    void operator()(std::pair<DWORD, Player *> pair_type)
    {
        operator()(pair_type.second);
    }
private:
    DWORD object_key_;
    float range_;
    WzVector object_pos_;
    eCHAR_STATE_TYPE state_type_;
    DWORD time_duration_;
    BYTE apply_type_;
};

void ACTION_ATTACH_STATE_RANGE::OnAction()
{
    ACTION_ATTACH_STATE_RANGE_INFO* info = Get();
#ifdef _NA_007266_20140530_BUG_ATTACH_STATE_TRIGGER
    eOBJECTCODE_TYPE object_type = static_cast<eOBJECTCODE_TYPE>(info->GetbyObjectType());

    WzVector object_pos;
    const float range = info->GetfRange();
    switch(object_type)
    {
    case NPCCODE_TYPE:
        // MKS 2014.05.30
        // 초기 구현의도를 알 수 없음. 
        // 해당 몬스터코드를 가진 모든 몬스터가 이 트리거 동작시켜야하나?
        // 구현 보류
        break;
    case UNITID_TYPE:
        {
            NPC* unit = getMGR()->FindNPCForUnitID(info->GetdwObjectKey());
            ASSERT(NULL != unit);
            DEBUG_CODE(if(!unit) return ;);

            object_pos = *unit->GetPosPtr();
        } break;
    case MAPNPC_TYPE:
        {
            MapObject* map_object = getMGR()->FindMapObject(info->GetdwObjectKey());
            ASSERT(NULL != map_object);
            DEBUG_CODE(if(!map_object) return ;);

            object_pos = *map_object->GetPosPtr();
        } break;
    case NON_TYPE:
    default:
        break;
    }
#else
    MapObject* map_object = getMGR()->FindMapObject(info->GetdwObjectKey());
    ASSERT(NULL != map_object);
    DEBUG_CODE(if(!map_object) return ;);

    WzVector object_pos = *map_object->GetPosPtr();
    const float range = info->GetfRange();
#endif //_NA_007266_20140530_BUG_ATTACH_STATE_TRIGGER

    AttachStateRangeHandler handler(info->GetdwObjectKey(), &object_pos, range,
        static_cast<eCHAR_STATE_TYPE>(info->GetdwStateCode()), 
        info->GetdwTimeDuration(), info->GetbyApplyType());

    getMGR()->ForeachPlayer(handler);

}
//=================================================================================================
//-------------------------------------------------------------------------------------------------
// _NA_000000_20130925_ADD_ACTION_TRIGGER_CHANGE_PLAYER_HP
//-------------------------------------------------------------------------------------------------
__IMPL_POOL(ACTION_CHANGE_PLAYER_HP)

class ChangePlayerHPHandler
{
public:
    ChangePlayerHPHandler(const DWORD figure, const BYTE calc_type, const BYTE apply_type) :
      figure_(figure),        // HP 변동시킬 수치
      calc_type_(calc_type),    // 계산 타입 (비율/수치)
      apply_type_(apply_type) // 증가/감소
      {}
      ~ChangePlayerHPHandler(){}

      void operator()(Player* player) 
      { 
          ASSERT(player);

          // 플레이어가 죽었다면 트리거 적용대상에서 제외..
          if (player->IsDead())
              return;

          //비율/수치 여부 판단하고 실제 HP변화시킬 값 구한다
          const DWORD player_max_hp = player->GetMaxHP();
          DWORD change_hp = 0;

          if (calc_type_ == ACTION_CHANGE_PLAYER_HP_INFO::kPersentage) 
          {
              if (figure_ > 100) {
                  figure_ = 100;
              }
              change_hp = static_cast<DWORD>(player_max_hp * (figure_ / 100.0f));
          }
          else if(calc_type_ == ACTION_CHANGE_PLAYER_HP_INFO::kValue) {
              change_hp = figure_;
          }

          DWORD apply_figure = 0;
          if (apply_type_ == ACTION_CHANGE_PLAYER_HP_INFO::kIncrease)
          {
              apply_figure = player->IncreaseHP(change_hp);
          }
          else if (apply_type_ == ACTION_CHANGE_PLAYER_HP_INFO::kDecrease)
          {
              apply_figure = player->DecreaseHP(change_hp);
              MSG_CG_TRIGGER_AREADAMAGE_BRD msg;
              msg.m_dwObjectKey = player->GetObjectKey();
              msg.m_wDamage     = apply_figure;
              msg.m_dwHP        = player->GetHP();
              player->SendPacketAround((MSG_BASE_FORWARD*)&msg, sizeof(msg));
          }
      }

      void operator()(std::pair<DWORD,Player *> pair_type)
      {
          operator()(pair_type.second);
      }

private:
    DWORD figure_;    // 수치
    BYTE calc_type_;   // 계산 타입 (비율/수치)
    BYTE apply_type_; // 증가/감소
};

void ACTION_CHANGE_PLAYER_HP::OnAction()
{
    const ACTION_CHANGE_PLAYER_HP_INFO* const info = Get();

    Player* player  = NULL;
    Trigger* trigger = getParent();
    //value 가져온다
    const BYTE target_type = info->GetTargetType();
    const BYTE calc_type  = info->GetCalcType(); // 계산 타입 (비율/수치)
    const BYTE apply_type = info->GetApplyType(); // 적용 타입 (증가/감소)
    DWORD figure = info->GetFigure(); // 비율/값

    ChangePlayerHPHandler op(figure, calc_type, apply_type);

    switch(target_type)
    {
    case ACTION_CHANGE_PLAYER_HP_INFO::kTargetPerson: // 개인
        {
            DWORD click_player = trigger->GetClickedPlayerKey();
            if (click_player)
                player = PlayerManager::Instance()->FindPlayerByObjKey(click_player);
            else
                trigger->ForeachSatisfiedPlayer(op);

            if (player)
                op(player);
            else
                SUNLOG(eDEV_LOG, "[TriggerID:%u]:ACTION_APPLY_DAMAGE : Cant not Find Player",
                trigger->TriggerID());
        }
        break;

    case ACTION_CHANGE_PLAYER_HP_INFO::kTargetParty: // 파티
        DWORD click_player = trigger->GetClickedPlayerKey();
        if(click_player != 0)
        {
            player = PlayerManager::Instance()->FindPlayerByObjKey(click_player);

            WORD partyKey = player->GetPartyState().GetPartyKey();

            if(!NewGamePartyManager::Instance()->For_eachPlayers(partyKey, op))
                SUNLOG(eCRITICAL_LOG,
                "[ACTION_REWARD_PLAYER] GameParty is NULL! (%s)'s PartyKey (%u)",
                player->GetCharName(), partyKey);
            return;
        }
        else
        {
            getMGR()->ForeachPlayer(op);
        }
        break;
    }
}