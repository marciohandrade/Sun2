#ifndef __GAMESERVER_TARGET_MANAGER_H__
#define __GAMESERVER_TARGET_MANAGER_H__
#pragma once

class NPC;
class Character;
class BattleRecord;

//==================================================================================================

class TargetManager
{
public:
    enum {
        TRACK_SLOT_NONE = 255
    };

    TargetManager(NPC* pNPC);
    ~TargetManager();

    void Init();
    void Update();

    void SetTrackSlot(BYTE bySlot);
    BYTE GetTrackSlot() const;

    void SetCurTarget(Character* pTarget);
    Character* GetCurTarget() const;

    BattleRecord* GetBattleRecord() const;

    void AddBattlePoint(Character* pAttacker, eBATTLE_POINT_TYPE ePointType, int iBattlePoint);
    void RemoveTarget(DWORD leaveEnemyKey);

    void RemoveBattleRecord(DWORD ObjKey);
    void ClearBattleRecord();

#ifdef _NA_0_20100819_AGGRO_RENEWAL
	void AddAggroDisplayUser(const DWORD object_key);
	void NotifyAggroValue();
    inline bool IsEmptyAggroDisplayUser() { return aggro_moitor_users_.empty(); };
#endif

private:
    void SelectBestTarget();
    void UpdateTargetList(); // 주변의 타겟을 찾아서 BattleRecord에 등록해준다.

private:
    NPC* const m_pNPC;

    BYTE m_byTrackSlot;      // 내가 추적하려는 타겟의 빈자리 인덱스(추적하려는 적이 없으면 TRACK_SLOT_NONE로 셋팅한다.)
    Character* m_pCurTarget;
    BattleRecord* m_pBattleRecord;
    ITimerBase m_pSearchTimer[1];
    ITimerBase m_pAggroTimer[1];
    __DISABLE_COPY(TargetManager);

#ifdef _NA_0_20100819_AGGRO_RENEWAL
	typedef STLX_VECTOR<DWORD>				AGGRO_MONITOR_USER;
	typedef AGGRO_MONITOR_USER::iterator	AGGRO_MONITOR_USER_ITER;
	AGGRO_MONITOR_USER aggro_moitor_users_;
#endif
};

//==================================================================================================

inline void
TargetManager::SetTrackSlot(BYTE bySlot) {
    m_byTrackSlot = bySlot;
}

inline BYTE
TargetManager::GetTrackSlot() const {
    return m_byTrackSlot;
}

inline Character* TargetManager::GetCurTarget() const 
{
//    if (obj_util::IsValid(m_pCurTarget, __FUNCTION__, __LINE__) == false)
//    {
//        return NULL;
//    }
    return m_pCurTarget;
}

inline BattleRecord*
TargetManager::GetBattleRecord() const {
    return m_pBattleRecord;
}

#endif //__GAMESERVER_TARGET_MANAGER_H__
