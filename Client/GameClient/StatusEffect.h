#pragma once

class Object;

class SKILL_EFFECT
{
private:
    BASE_STATEINFO* status_info;    // 항상 유효해야함

public:
    DWORD       dwStatusID;	
    int         iRemainTime;

    //BYTE        m_byIconKind;                           // 아이콘 종류
    //BYTE        m_bDelType;                             // 삭제여부 ( 유저가 임의로 삭제할 수 있는 버프인가 )

    DWORD       dwSkillID;
    eABILITY    dwAbilityID;
    BASE_ABILITYINFO AbilityInfo;

    WORD        wGroupId;

    int         iEffectID;
    HANDLE      hEffect;
    eSKILLEFFECT_POS    effect_position_type;
    WORD                effect_position_bone_index;         //!< 본 인텍스 m_byEffectPos == eSKILLEFFECT_POS_BONE_INDEX 일때만 사용
    WzVector            effect_position_offset;             //!< 계산된 위치에서 이동값
    bool bApplyParentRotate;    //!< 부모 회전값을 적용받을것인가
    bool bApplyParentScale;     //!< 부모 스케일값을 적용받을것인가
    float fEffectHeight;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    WORD         overlap_status_count_;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE


public:
    SKILL_EFFECT();

public:
    static SKILL_EFFECT* CreateStatusEffect( DWORD status_code, DWORD remain_time );
    static SKILL_EFFECT* CreateSkillStatusEffect( DWORD status_code, DWORD skill_code, DWORD remain_time, BASE_ABILITYINFO* ability_info );


    //const BASE_STATEINFO* GetStateInfo() const { return status_info; }
    LPCSTR GetStateName() { return status_info->m_szStateName; }

    void CreateEffect( Character* character, DWORD target_object_key );
    void UpdateEffect( Character* character );

    WzID GetSkillEffectID() { return status_info->m_wzidEffectID.id; }

    BYTE GetIconKind() { return status_info->m_byIconKind; }
    BYTE GetDelType() { return status_info->m_bDelType; }




public:
    ~SKILL_EFFECT();

public:
    BOOL Process(Object *pObject,DWORD dwTick);

};