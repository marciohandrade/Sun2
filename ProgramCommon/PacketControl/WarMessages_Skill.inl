#ifndef PROGRAMCOMMON_WARMESSAGES_INTER_INL
    #error "can't direct include this file"
#endif

//==================================================================================================

namespace nsSync {
#pragma pack(push, 1)
;

// NOTE: remove _NA_0_20100211_SD_EFFECT_MODIFY
//              REMOVE_ATTACK_MOVE
// NOTE:
//  effect : composition of SKILL_EFFECT_~~

//==================================================================================================
// base: CG_BATTLE_MONSTER_ATTACK_CMD
WAR_MSG_DECL(BATTLE_MONSTER_ATTACK_BRD)
{
    DWORD      m_dwObjectKey;
    DWORD      m_dwTargetObjectKey;
    BYTE       m_byAttackType;
    WzVector   m_wvCurPos;
    DAMAGETYPE m_wDamage;
    DWORD      m_wTargetHP;
    WORD       m_wTargetReserveHP;
    BYTE       m_byEffect; // ���� ����� ���� Ư�� ����Ʈ(ũ��Ƽ���� ������...)

    void Clear() { ZeroMemory(this, sizeof(*this)); }
};

// base: CG_SYNC_NPC_JUMPRESULT_BRD
WAR_MSG_DECL(BATTLE_MONSTER_JUMP_ATTACK_BRD)
{
    DWORD    m_dwObjectKey;
    DWORD    m_dwTargetObjectKey;
    BYTE     m_byAttackType;
    WzVector m_wvCurPos;
    DAMAGETYPE m_wDamage;
    DWORD m_wTargetHP;
    BYTE m_byEffect; // ���� ����� ���� Ư�� ����Ʈ(ũ��Ƽ���� ������...)

    void Clear() { ZeroMemory(this, sizeof(*this)); }
};


//==================================================================================================
// base: CG_BATTLE_PLAYER_ATTACK_BRD
WAR_MSG_DECL(BATTLE_PLAYER_ATTACK_BRD)
{
    DWORD      dwAttackerKey;  // �������� ������Ʈ Ű
    BYTE       byAttackType;   // ���� Ÿ��
#ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
    uint8_t    style_presentation; // default(0), client animation presentation, ref: CG_BATTLE_PLAYER_ATTACK_SYN::style_presentation
#endif
    SLOTCODE   StyleCode;      // ��Ÿ�� �ڵ�
    DWORD      dwClientSerial; // Ŭ���̾�Ʈ���� ���� �ø����� ��ε� ĳ��Ʈ
    WzVector   wvCurPos;       // �������� ������ġ
    DWORD      dwTargetKey ;   // ���� ����� ������Ʈ Ű
    DAMAGETYPE wDamage;        // ���� ������
    DWORD      dwTargetHP;     // ���� �� Ÿ���� HP
    BYTE       byEffect;       // ���� ����� ���� Ư�� ����Ʈ(ũ��Ƽ���� ������...)
    BYTE       byEtherComboCount; // ���׸� źȯ ���� �� �޺�
    //
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    DWORD   target_sd; // ���� �� Ÿ���� SD
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    void Clear() { ZeroMemory(this, sizeof(*this)); }
};

// base: CG_BATTLE_PLAYER_AREA_ATTACK_BRD
WAR_MSG_DECL(BATTLE_PLAYER_AREA_ATTACK_BRD)
{
    DWORD    m_dwClientSerial;     // ���� �̺�Ʈ�� ���� ���� ��ȣ
    DWORD    m_dwAttackerKey;      // �������� ������Ʈ Ű
    DWORD    m_dwPrimaryTargetKey; // �ְ��� ��� ������Ʈ Ű
    WzVector m_wvCurPos;           // �������� ���� ��ǥ
    SLOTCODE m_StyleCode;          // ��Ÿ�� �ڵ�
    BYTE     m_byAttackType;       // ���� Ÿ��
    BYTE     m_byNumberOfTargets;  // ���� ����� ����
    AREA_ATTACK_RESULT_INFO AttackInfo[MAX_TARGET_NUM];

    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(AttackInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(AttackInfo[0]) * m_byNumberOfTargets);
    }
};


//==================================================================================================
// base: CG_STYLE_PLAYER_ATTACK_BRD
WAR_MSG_DECL(STYLE_PLAYER_ATTACK_BRD)
{
    DWORD    dwAttackerKey;  // �������� ������Ʈ Ű
    BYTE     byAttackType;   // ���� Ÿ��
#ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
    uint8_t  style_presentation; // ref:BATTLE_PLAYER_ATTACK_BRD::style_presentation
#endif
    SLOTCODE StyleCode;      // Style Code
    DWORD    dwClientSerial; // ���� �̺�Ʈ�� ���� ���� ��ȣ
    DWORD    dwPrimaryTargetKey; // �ְ��� ��� ������Ʈ Ű
    WzVector wvCurPos;

    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        return sizeof(*this);
    }
};

// base: CG_STYLE_PLAYER_ATTACK_RESULT_BRD
WAR_MSG_DECL(STYLE_PLAYER_ATTACK_RESULT_BRD)
{
    DWORD dwClientSerial;    // ���� �̺�Ʈ�� ���� ���� ��ȣ
    DWORD dwAttackerKey;     // �������� ������Ʈ Ű
    BYTE  byNumberOfTargets; // ���� ����� ����

    STYLE_ATTACK_RESULT_INFO AttackInfo[MAX_TARGET_NUM];

    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(AttackInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(AttackInfo[0]) * byNumberOfTargets);
    }
};


//==================================================================================================

struct SkillActionBase
{
    SLOTCODE m_SkillCode;          // Skill �ڵ�
    DWORD    m_dwClientSerial;     // ���� �̺�Ʈ�� ���� ���� ��ȣ
    DWORD    m_dwAttackerKey;      // �������� ������Ʈ Ű
    DWORD    m_dwPrimaryTargetKey; // �ְ��� ��� ������Ʈ Ű
    WzVector m_wvMainTargetPos;    // �ְ��ݴ���� ��ġ
};

struct SkillActionInstantResult : public SkillActionBase
{
    WzVector m_wvCurPos;      // �������� ���� ��ǥ
    WzVector m_wvDestPos;     // �������� ������ ��ǥ
    DWORD    m_dwAttackerHP;  // ��ų�� ���� �� �������� HP
    DWORD    m_dwAttackerMP;  // ��ų�� ���� �� �������� MP
    BYTE     m_byNumberOfTargets     : 6; // ���� ����� ����
    BYTE     m_byNumberOfFieldEffect : 2; // �ʵ忡 �ɸ��� ����Ʈ�� ���� ��� �� ����
};

struct SkillActionDelayStart : public SkillActionBase
{
    WzVector m_wvCurPos;  // �������� ���� ��ǥ
    WzVector m_wvDestPos; // �������� ������ ��ǥ
};

struct SkillActionDelayResult : public SkillActionBase
{
    DWORD m_dwAttackerHP; // ��ų�� ���� �� �������� HP
    DWORD m_dwAttackerMP; // ��ų�� ���� �� �������� MP
    BYTE  m_byNumberOfTargets : 6; // ���� ����� ����
    BYTE  m_byNumberOfFieldEffect : 2; // �ʵ忡 �ɸ��� ����Ʈ�� ���� ��� �� ����
};

WAR_MSG_DECL(SKILL_ACTION_INSTANT_RESULT_BRD)
{
    SkillActionInstantResult result;
    uint16 sizeof_additions;

    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        return static_cast<uint16_t>(sizeof(*this) + sizeof_additions);
    };
};

WAR_MSG_DECL(SKILL_ACTION_DELAY_START_BRD)
{
    SkillActionDelayStart result;

    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        return sizeof(*this);
    };
};

WAR_MSG_DECL(SKILL_ACTION_DELAY_RESULT_BRD)
{
    SkillActionDelayResult result;
    uint16 sizeof_additions;

    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        return static_cast<uint16_t>(sizeof(*this) + sizeof_additions);
    };
};


//==================================================================================================

WAR_MSG_DECL(SKILL_CASTING_CANCEL_BRD)
{
    DWORD    m_dwObjectKey; // ��ų ������ ������ƮŰ
    SLOTCODE m_SkillCode;   // Skill �ڵ�

    void Clear() { ZeroMemory(this, sizeof(*this)); }
};

WAR_MSG_DECL(SKILL_PERIODIC_DAMAGE_BRD)
{
    DWORD       m_dwAttackerKey; // �ֱⵥ������ �߻���Ų ������ Ű
    SLOTCODE    m_SkillCode;     // �ֱⵥ������ �߻���Ű�� ��ų�ڵ�
    BYTE        m_byNumberOfTargets; // ���� ����� ����
    DAMAGE_INFO m_DamageInfo[MAX_TARGET_NUM]; // ���� ����� ������ ����

    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_DamageInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_DamageInfo[0]) * m_byNumberOfTargets);
    }

};

WAR_MSG_DECL(SKILL_REFLECT_DAMAGE_BRD)
{
    DWORD      m_dwAttackerKey; // �ֱⵥ������ �߻���Ų ������ Ű
    DWORD      m_dwTargetKey;   // ���� ���ϴ� ������Ʈ Ű
    SLOTCODE   m_SkillCode;     // �ǰ� ��ų�ڵ�
    WORD       m_wAbilityCode;  // �ǰ� �����Ƽ �ڵ�
    DAMAGETYPE m_wDamage;       // ���� ������
    DWORD      m_dwTargetHP;    // ���� �� Ÿ���� HP
    DWORD      m_dwTargetResrveHP; // ������ Ÿ���� ������ HP

    void Clear() { ZeroMemory(this, sizeof(*this)); }
};

WAR_MSG_DECL(SKILL_EFFECT_BOMB_BRD)
{
    DWORD    m_dwAttackerKey;
    SLOTCODE m_SkillCode;
    BYTE     m_byNumberOfTargets;
    EFFECT_BOMB_INFO m_DamageInfo[MAX_TARGET_NUM];

    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_DamageInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_DamageInfo[0]) * m_byNumberOfTargets);
    }
};

WAR_MSG_DECL(SKILL_SELF_DESTRUCTION_BRD)
{
    DWORD    m_dwAttackerKey;
    SLOTCODE m_SkillCode;
    BYTE     m_byNumberOfTargets;
    DAMAGE_INFO m_DamageInfo[MAX_TARGET_NUM];

    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_DamageInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_DamageInfo[0]) * m_byNumberOfTargets);
    }

};

WAR_MSG_DECL(SKILL_TRANSPARENT_BRD)
{
    DWORD m_dwObjectKey;

    void Clear() { ZeroMemory(this, sizeof(*this)); }
};

WAR_MSG_DECL(SKILL_SUCTION_BRD)
{
    DWORD      m_dwAttackerKey; // ����� �߻���Ų ������ Ű
    SLOTCODE   m_SkillCode;     // �ֱⵥ������ �߻���Ű�� ��ų�ڵ�
    DAMAGETYPE m_wTargetDamage; // ��� ������
    DWORD  m_dwTargetHP;   // Ÿ���� ���� HP
    DWORD  m_dwTargetMP;   // Ÿ���� ���� MP
    DWORD  m_dwAttackerHP; // �������� ���� HP
    DWORD  m_dwAttackerMP; // �������� ���� MP

    void Clear() { ZeroMemory(this, sizeof(*this)); }
};

WAR_MSG_DECL(SKILL_VITAL_SUCTION_BRD)
{
    DWORD m_dwAttackerKey; // ����� �߻���Ų ������ Ű
    DWORD m_dwTargetKey;   // Ÿ���� Ű.
    DWORD m_dwHP; // Ÿ���� ���� HP

    void Clear() { ZeroMemory(this, sizeof(*this)); }
};


//==================================================================================================
#pragma pack(pop)

}; //end of namespace 'nsPacket'

//==================================================================================================

