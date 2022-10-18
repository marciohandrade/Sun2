//=============================================================================================================================
/// UnitRegenInfo class
/*
@author	Kim Min Wook < taiyo@webzen.com >
@since	2006. 7. 28
@note
- WUT에 읽은 UnitRegen정보, 공유 정보 
- 동시간 여러 유닛이 공유하는 정보가 아닌, 단일 유닛의 리젠 정보
@history

*/
//=============================================================================================================================

#ifndef SUN_GAMESERVER_UNITREGENINFO_H_
#define SUN_GAMESERVER_UNITREGENINFO_H_
#pragma once

class UnitGroupInfo;


class UnitRegenInfo
{
public:
    UnitRegenInfo(void);
    ~UnitRegenInfo(void);

    static const BYTE MAX_WUT_FILENAME_LENGTH = 128;

    __inline void CopyDebugName(char* p) 
    {
        strncpy(m_pszDebugName,p,MAX_MONSTERNAME_LENGTH);
        m_pszDebugName[MAX_MONSTERNAME_LENGTH]='\0'; 
    }

    __inline void CopyFileName(char* p) 
    { 
        strncpy(m_pszWutFileName,p,MAX_WUT_FILENAME_LENGTH); 
        m_pszWutFileName[MAX_WUT_FILENAME_LENGTH]='\0'; 
    }

    char* GetDebugName() { return m_pszDebugName; }
    char* GetWutFileName() { return m_pszWutFileName; }


    DWORD GetRegenTime() const { return m_RegenTime; }
    DWORD GetNpcCodeArray(MONSTERCODE* OUT npc_code_array, int size);

    WORD  GetNumOfPos() const { return m_NumOfPos; }     //_NA_0_20100222_UNIT_TRIGGERS_PATHLIST
    WzVector& GetStartPos(const WORD idx) const { return m_StartPos[min(idx, m_NumOfPos-1)]; }
    WzVector& GetDestPos(const WORD idx)  const { return m_DestPos[min(idx, m_NumOfPos-1)]; }
    eCHAR_MOVE_STATE GetWalkState(const WORD idx) const { return walk_state_[min(idx, m_NumOfPos-1)]; } 

    int GetRandomNPCCode();                 //  _NA_0_20100706_RANDOM_REGEN
    WORD GetRandomPosIndex();
 
    // 로딩시 초기화
    void Init(BYTE NumOfNPC, WORD NumOfPos);
    //_NA_0_20100222_UNIT_TRIGGERS_REGEN
    bool IsRegenTypeTrigger() const { return (regen_type_ & REGEN_TYPE_TRIGGER) == REGEN_TYPE_TRIGGER; }

    void Release();

    void SetRegenTime(DWORD RegenTime);
    void SetStartPos(WORD idx, WzVector* pos);
    void SetDestPos(WORD idx, WzVector* pos);
    void SetWalkState(WORD idx, eCHAR_MOVE_STATE walk_state);  //_NA_0_20100222_UNIT_TRIGGERS_PATHLIST
    void SetReservedData2(DWORD v) { m_ReservedData2 = v; }

    //_NA_0_20100222_UNIT_TRIGGERS_REGEN
    void SetRegenTypeTrigger(BYTE regen_type) { regen_type_ = regen_type; }

    //_NA_0_20100706_RANDOM_REGEN
    BOOL AddRandomRegenMonster(MONSTERCODE regen_npc_code, DWORD select_rate) {
        return regen_monster_selector_.Add(regen_npc_code, select_rate);
    }

    const WzVector& GetRandomStartPos();
    const WzVector& GetRandomDestPos();
    const WzVector& GetStartPos() { return m_StartPos[0]; }

    const int* const GetNpcCodeArray(DWORD& OUT result_number_of_npcs);
    void CalcRatio();
    void SetNPCCodes(BYTE idx, int value);
    void SetSelectRates(BYTE idx, int value);
    BOOL IsRandomRegen();

private:
    char m_pszDebugName[MAX_MONSTERNAME_LENGTH+1];
    char m_pszWutFileName[MAX_WUT_FILENAME_LENGTH+1];

    __PROPERTY( int, Category );				//< not used
    __PROPERTY( REGENID, ID );
    __PROPERTY( float, Dir );
    __PROPERTY( int, UnitType );
    __PROPERTY( int, AIType );					//< do
    __PROPERTY( int, RegenRandDist );
    __PROPERTY( UNITGROUPID, GroupID );
    __PROPERTY( DWORD, SectorIndex );          


    DWORD m_ReservedData2;
    WORD m_NumOfPos;
    WzVector* m_StartPos;
    WzVector* m_DestPos;

    DWORD m_RegenTime;

    BYTE regen_type_;               // _NA_0_20100222_UNIT_TRIGGERS_REGEN
    eCHAR_MOVE_STATE* walk_state_;  // _NA_0_20100222_UNIT_TRIGGERS_PATHLIST
    
    RatioSelector<MONSTERCODE> regen_monster_selector_; //_NA_0_20100706_RANDOM_REGEN


    BYTE m_bTrigger;
    WORD m_SelectedPos;
    int* m_pNPCCodes;
    int m_SelectedNPC;
    BYTE m_NumOfNPC;
    int* m_pSelectRates;
    BYTE m_RegenType;
};

////////////////////////////////////////////////////////////////////////////////////////////////

//_NA_0_20100706_RANDOM_REGEN 삭제
inline const int* const
UnitRegenInfo::GetNpcCodeArray( DWORD& OUT rNpcCount )
{
    rNpcCount = 0;
    if( m_pNPCCodes )
    {
        rNpcCount = m_NumOfNPC;
        return m_pNPCCodes;
    }
    return NULL;
}


#endif  //SUN_GAMESERVER_UNITREGENINFO_H_
