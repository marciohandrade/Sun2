#pragma once

#include "Interface/uiBaseMan_def.h"


class SkillData
    : private uicSkillInfo
{
public:
    SkillData();

    void SetSlotPosition( int pos, int tab );
    void SetCurSkill( int skill_Code );
    
    int GetLineIndex() const                    { return GetSlotPosition() / 6; }
    int GetSlotPosition() const                 { return uicSkillInfo::m_SlotIndex; }
    int GetTabIndex() const                     { return uicSkillInfo::m_TabIndex; }
    SkillScriptInfo* GetCurSkillInfo() const    { return uicSkillInfo::m_pCurrentSkillInfo; }
    SkillScriptInfo* GetNextSkillInfo() const   { return uicSkillInfo::m_pNextSkillInfo; }

    int IsEnableLevelup()  const                { return uicSkillInfo::m_bEnableLevelup; }
    void SetEnableLevelup( bool enable )        { uicSkillInfo::m_bEnableLevelup = enable; }
    bool IsMaxLevel()  const                    { return uicSkillInfo::m_IsMaxLevel; }
    void SetMaxLevel( bool v )                  { uicSkillInfo::m_IsMaxLevel = v; }

    bool IsActive() const                       { return m_IsActive; }
    void SetActive( bool active )               { m_IsActive = active; }


    eSLOT_RENDER_STATE GetState()  const        { return uicSkillInfo::m_State; }
    eSLOT_RENDER_STATE GetBlockState() const    { return uicSkillInfo::m_BlockState; }

    int GetRequiredSkillClassCode() const;
    int GetRequiredSkillLevel() const;

    bool IsActiveState() const;
    void SetActiveState();

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    bool IsLeftSide() const;
    void SetDeactiveState();
#endif

    bool IsBlockedState() const;
    void SetBlockState( bool bBlock );

    int GetSkillCode() const;
    int GetSkillClassCode() const;
    int GetClassDefine() const;

    bool IsUseableClass(DWORD character_class);

    DWORD GetSkillNameCode() const;
    int GetSkillLevel() const;
    WORD GetSkillRange() const;

    bool IsPassiveSkill() const;


    bool LevelUp();
};


class SkillDataSet
    : public std::vector<SkillData>
{
public:
    SkillDataSet() {}

    SkillData* GetSkillDataBySlotPosition(int position);
    SkillData* GetSkillDataBySkillClassCode(CODETYPE skill_class_code);
    SkillData* GetSkillDataBySkillCode(CODETYPE skill_code);
    int FindPosBySkillClassCode(int skill_class_code);

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    size_t GetTotalUsedPoint(bool left) const;
#else
    size_t GetTotalUsedPoint() const;
#endif
    bool CheckSkillLevel(DWORD skill_class, int skill_level, DWORD* skill_name_code);

private:
    std::vector<SkillData>::reference Get( size_t _pos ) { return (*(begin() + _pos)); }
    std::vector<SkillData>::const_reference Get( size_t _pos ) const { return (*(begin() + _pos)); }


};

#define INVALID_REQUIRED_SKILL_LEVEL    0x00000000
#define INVALID_SKILL_LEVEL             0x00000000
