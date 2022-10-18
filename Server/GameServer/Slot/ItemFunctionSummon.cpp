#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "NPCInfoParser.h"
#include "MonsterSummonParser.h"

#include "InventorySlotContainer.h"
#include "GameZone.h"
#include "MovingPlanner.h"

#include "Monster.h"

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionSummon) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
//Use패킷
RC::eITEM_RESULT nsSlot::ItemFunctionSummon::CreateTotemNPC(
    SCItemSlot & rItemSlot, WzVector* pwvDestPos)
{
    if (!root_->player_->GetField())
        return RC::RC_ITEM_FIELD_IS_NULL;

    const BASE_ITEMINFO* const pInfo = rItemSlot.GetItemInfo();

    BASE_NPCINFO *pNPCBaseInfo = NPCInfoParser::Instance()->GetNPCInfo(pInfo->m_wSkillCode);
    if (!pNPCBaseInfo)
        return RC::RC_ITEM_CANNOT_CREATE_TOTEMNPC;

    // 몬스터코드는 ItemInfo의 스킬코드 값을 사용한다.

    TotemNpc * pTotemNpc = NULL;
    if (pwvDestPos)
    {
        pTotemNpc = root_->player_->GetField()->CreateTotemNPC(
            pInfo->m_wSkillCode, pwvDestPos, root_->player_, rItemSlot);
    }
    else
    {
        WzVector vCurPos;   root_->player_->GetPos(&vCurPos);
        pTotemNpc = root_->player_->GetField()->CreateTotemNPC(
            pInfo->m_wSkillCode, &vCurPos, root_->player_, rItemSlot);
    }

    if (!pTotemNpc) 
        return RC::RC_ITEM_CANNOT_CREATE_TOTEMNPC;

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
//Use패킷
RC::eITEM_RESULT nsSlot::ItemFunctionSummon::SummonMonsterItemUse(SCItemSlot & rItemSlot)
{
    if (NULL == root_->player_)
        return RC::RC_ITEM_INVALIDSTATE;

    GameZone *pZone = root_->player_->GetGameZonePtr();
    if (NULL == pZone) 
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;

    GameField* pField = root_->player_->GetField();
    if (NULL == pField) 
        return RC::RC_ITEM_INVALIDSTATE;

    if (pZone->GetZoneType() == eZONETYPE_VILLAGE || pZone->GetZoneType() == eZONETYPE_LOBBY ||
        pZone->GetZoneType() == eZONETYPE_SPA_FIELD){    //_NA_008069_20150210_ADD_SPA_SYSTEM
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;
    }

    const MonsterSummonScriptInfo* pInfo = \
        MonsterSummonParser::Instance()->GetMonsterSummonScriptInfo(\
            rItemSlot.GetItemInfo()->m_wSkillCode);
    if (NULL == pInfo)
        return RC::RC_ITEM_NOTEXIST_SUMMON_MONSTERINFO;

    if (pInfo->m_LimitedLV > root_->player_->GetLevel())
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CURR_LEVEL;

    CODETYPE    MonsterCode = 0;
    WORD        MonsterNum = 0;
    WORD        SuccessNum = 0;
    WORD        FailedNum = 0;

    WzVector    vPlayerPos;
    root_->player_->GetPos(&vPlayerPos);

    // 플레이어가 위치한 타일
    int iTileIdx = root_->player_->GetPathExplorer()->GetTile();
    if (-1 == iTileIdx) return RC::RC_ITEM_INVALIDSTATE;

    //소환해야 할 몬스터 수
    MonsterNum = pInfo->m_wMonsterNum;
    if (MonsterNum == 0)
        return RC::RC_ITEM_INVALIDSTATE;

    //랜덤으로 소환
    for (DWORD i = 0; i < MonsterNum; i++)
    {
        CODETYPE MonsterCode = MonsterSummonParser::Instance()->GetRandomMonsterCode(\
                rItemSlot.GetItemInfo()->m_wSkillCode);
        if (MonsterCode != 0)
        {
            // 플레이어가 위치한 타일위에서 랜덤한 위치
            nsAI::MovingPlanner* const pMovingPlanner = pField->GetMovingPlanner();
            WzVector vRandomPos = pMovingPlanner->GetRandomPosInTile(iTileIdx);
            BASE_NPCINFO *pBaseNPCInfo = NPCInfoParser::Instance()->GetNPCInfo(MonsterCode);
            if (NULL == pBaseNPCInfo)
                continue;

            Monster* pMonster = pField->CreateMonster((WORD)MonsterCode, &vPlayerPos);
            if (NULL ==  pMonster)
                continue;

            pMonster->StartSummonedTimer(pInfo->m_LiveTime);
            SuccessNum++;
        }       
    }   

    //몬스터 소환 아이템을 사용실패
    if (SuccessNum == 0)
        return RC::RC_ITEM_FAILED;

    return RC::RC_ITEM_SUCCESS; 
}
