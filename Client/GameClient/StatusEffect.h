#pragma once

class Object;

class SKILL_EFFECT
{
private:
    BASE_STATEINFO* status_info;    // �׻� ��ȿ�ؾ���

public:
    DWORD       dwStatusID;	
    int         iRemainTime;

    //BYTE        m_byIconKind;                           // ������ ����
    //BYTE        m_bDelType;                             // �������� ( ������ ���Ƿ� ������ �� �ִ� �����ΰ� )

    DWORD       dwSkillID;
    eABILITY    dwAbilityID;
    BASE_ABILITYINFO AbilityInfo;

    WORD        wGroupId;

    int         iEffectID;
    HANDLE      hEffect;
    eSKILLEFFECT_POS    effect_position_type;
    WORD                effect_position_bone_index;         //!< �� ���ؽ� m_byEffectPos == eSKILLEFFECT_POS_BONE_INDEX �϶��� ���
    WzVector            effect_position_offset;             //!< ���� ��ġ���� �̵���
    bool bApplyParentRotate;    //!< �θ� ȸ������ ����������ΰ�
    bool bApplyParentScale;     //!< �θ� �����ϰ��� ����������ΰ�
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