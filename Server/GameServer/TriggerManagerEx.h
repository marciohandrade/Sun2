#ifndef _GAMESERVER_TRIGGERMANAGER_H
#define _GAMESERVER_TRIGGERMANAGER_H
#pragma once

#include "..\\..\\SolarTrigger\\TriggerManager.h"
#include "TriggerLastedState.h"

class Trigger;

//==================================================================================================
//
class TriggerManagerEx : public TriggerManager
{
public:
    enum { MAX_SWITCH_NUM = 255, };

    TriggerManagerEx();
    virtual ~TriggerManagerEx();

    // 각종 셋팅!!
    void Init(TriggerGroupInfo& IN rInfo, GameField* pField);
    void Release();
    void Update();

    inline GameField*  GetField() const { return m_pField; }
    inline BOOL IsCanProcess() { return m_pField && m_pField->IsReady(); }

    inline BYTE GetSwitchValue(BYTE index)             { return SWITCH.Get(index); }
    inline void SetSwitchValue(BYTE index, BYTE value) { SWITCH.Set(index, value); }
    //----------------------------------------------------------//

    void SetAttribute(const INT iTileID, const WORD attribute);
    void UnsetAttribute(const INT iTileID, const WORD attribute);

#ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
    const TriggerAreaManagerForNpc* GetAreaManagerForNpc() const;
    bool CreateAreaForNpc(int area_id, CODETYPE unit_id, TriggerAreaLocalStorageForNpc* area_item);
    bool OnEnterAreaForNpc(TRIGGER_NPC_ENTER_AREA* msg);
    bool OnLeaveAreaForNpc(TRIGGER_NPC_LEAVE_AREA* msg);
#endif
    bool CreateAreaForMonster(int area_id, CODETYPE monster_code, TriggerAreaLocalStorageForNpc* area_item);

    //------------- GameLogic -------------------------------//
    // NOTE: f110928.3L, prevent the client enter area request hacking loophole.
    // USAGES: CheckAreaIdBasedOnPos(id, NULL, &tile_no);CheckAreaIdBasedOnPos(id, &position, NULL);
    bool CheckAreaIdBasedOnPos(const int area_id, const WzVector* check_pos, const int* or_tile_no);
    WzSpecialArea* GetAreaInfo(INT index);   //< 트리거에서는 인덱스방식의 영역찾기만을 사용!!!!!
    int    CreateNPC(WORD monsterCode, DWORD numberOfNpc,
                      WzVector* pvPos, DWORD areaID, WORD wGroupID, int iAngle = -1);

    void    CreateMapNPC(WORD NPCCode, const WzVector* pvPos, MAPOBJECT_INFO* pMapInfo);

    WzVector GetRandomPosInTile(int iTile);
    BOOL    KillMonster(MONSTERCODE monsterCode, Player* pPlayer, BOOL IsExpItemDrop);
    BOOL    KillOneMonster(NPC* pNpc, Player* pPlayer, BOOL IsExpItemDrop); //_NA_0_20100222_UNIT_TRIGGERS_ADD

    void    AddExpAllMember(DWORD64 Exp);
    MapObject* FindMapObject(const DWORD object_key);

    //__NA_276_20070213_UNIT_TRIGGER {
    NPC* CreateNPCForUnitID(const DWORD UnitID);
    void DestroyNPCForUnitID(const DWORD UnitID);
    NPC* FindNPCForUnitID(const DWORD unitID);
    //}
    MAPOBJECT_INFO* FindMapObjectInfo(const DWORD objectKey);

    MapNpc* FindMapNpc(const DWORD objectKey);
    NPC*    FindNPC(const DWORD object_key);
    DWORD   GetPlayerNum();
    void    ClearMission();
    void    SendToAll(MSG_BASE_FORWARD* pMsg, WORD wSize);

    template<typename _OPERATOR>
    void    ForeachPlayer(_OPERATOR& Opr);

    // _NA_0_20100222_UNIT_TRIGGERS_ADD
    template<typename _OPERATOR>
    void    ForeachNPC(_OPERATOR& Opr);

    static BOOL     Check_RewardWindow(Player* pPlayer, BOOL bGMSpecial = FALSE);
    static BOOL     Process_RewardWindow(Player* pPlayer);

protected:
    virtual BaseTrigger* AllocTrigger();
    virtual void         FreeTrigger(BaseTrigger* pTrigger);

    //////////////////////////////////////////////////////////////////////////
    //  <FIELDs>
private:
    GameField*  m_pField;
    BOOL        m_bInitCompleted;   // 초기화를 했는가?
#ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
    TriggerAreaManagerForNpc trigger_area_manager_for_npc_;
#endif
public:
    TriggerLastedStateDelegator  MAP_STATE;
    typedef PROPERTY_ARRAY<BYTE, INVALID_SWITCH_ID_VALUE, MAX_SWITCH_NUM>   SWITCH_CONTROL;
    SWITCH_CONTROL  SWITCH;
};

//==================================================================================================

template<typename _OPERATOR>
void
TriggerManagerEx::ForeachPlayer(_OPERATOR& Opr)
{
    if(m_pField)
        m_pField->ForAllPlayer(Opr);
}

//_NA_0_20100222_UNIT_TRIGGERS_ADD
template<typename _OPERATOR>
void
TriggerManagerEx::ForeachNPC(_OPERATOR& Opr)
{
    if(m_pField)
        m_pField->ForAllNPC(Opr);
}

#ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
inline const TriggerAreaManagerForNpc*
TriggerManagerEx::GetAreaManagerForNpc() const {
    return &trigger_area_manager_for_npc_;
}
#endif

#endif //_GAMESERVER_TRIGGERMANAGER_H
