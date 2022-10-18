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
    BYTE       m_byEffect; // 공격 결과에 대한 특수 이펙트(크리티컬이 터졌다...)

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
    BYTE m_byEffect; // 공격 결과에 대한 특수 이펙트(크리티컬이 터졌다...)

    void Clear() { ZeroMemory(this, sizeof(*this)); }
};


//==================================================================================================
// base: CG_BATTLE_PLAYER_ATTACK_BRD
WAR_MSG_DECL(BATTLE_PLAYER_ATTACK_BRD)
{
    DWORD      dwAttackerKey;  // 공격자의 오브젝트 키
    BYTE       byAttackType;   // 공격 타입
#ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
    uint8_t    style_presentation; // default(0), client animation presentation, ref: CG_BATTLE_PLAYER_ATTACK_SYN::style_presentation
#endif
    SLOTCODE   StyleCode;      // 스타일 코드
    DWORD      dwClientSerial; // 클라이언트에서 보낸 시리얼을 브로드 캐스트
    WzVector   wvCurPos;       // 공격자의 현재위치
    DWORD      dwTargetKey ;   // 공격 대상의 오브젝트 키
    DAMAGETYPE wDamage;        // 공격 데미지
    DWORD      dwTargetHP;     // 공격 후 타겟의 HP
    BYTE       byEffect;       // 공격 결과에 대한 특수 이펙트(크리티컬이 터졌다...)
    BYTE       byEtherComboCount; // 에테르 탄환 장착 시 콤보
    //
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    DWORD   target_sd; // 공격 후 타겟의 SD
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    void Clear() { ZeroMemory(this, sizeof(*this)); }
};

// base: CG_BATTLE_PLAYER_AREA_ATTACK_BRD
WAR_MSG_DECL(BATTLE_PLAYER_AREA_ATTACK_BRD)
{
    DWORD    m_dwClientSerial;     // 공격 이벤트에 대한 고유 번호
    DWORD    m_dwAttackerKey;      // 공격자의 오브젝트 키
    DWORD    m_dwPrimaryTargetKey; // 주공격 대상 오브젝트 키
    WzVector m_wvCurPos;           // 공격자의 현재 좌표
    SLOTCODE m_StyleCode;          // 스타일 코드
    BYTE     m_byAttackType;       // 공격 타입
    BYTE     m_byNumberOfTargets;  // 공격 대상의 수량
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
    DWORD    dwAttackerKey;  // 공격자의 오브젝트 키
    BYTE     byAttackType;   // 공격 타입
#ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
    uint8_t  style_presentation; // ref:BATTLE_PLAYER_ATTACK_BRD::style_presentation
#endif
    SLOTCODE StyleCode;      // Style Code
    DWORD    dwClientSerial; // 공격 이벤트에 대한 고유 번호
    DWORD    dwPrimaryTargetKey; // 주공격 대상 오브젝트 키
    WzVector wvCurPos;

    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const {
        return sizeof(*this);
    }
};

// base: CG_STYLE_PLAYER_ATTACK_RESULT_BRD
WAR_MSG_DECL(STYLE_PLAYER_ATTACK_RESULT_BRD)
{
    DWORD dwClientSerial;    // 공격 이벤트에 대한 고유 번호
    DWORD dwAttackerKey;     // 공격자의 오브젝트 키
    BYTE  byNumberOfTargets; // 공격 대상의 수량

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
    SLOTCODE m_SkillCode;          // Skill 코드
    DWORD    m_dwClientSerial;     // 공격 이벤트에 대한 고유 번호
    DWORD    m_dwAttackerKey;      // 공격자의 오브젝트 키
    DWORD    m_dwPrimaryTargetKey; // 주공격 대상 오브젝트 키
    WzVector m_wvMainTargetPos;    // 주공격대상의 위치
};

struct SkillActionInstantResult : public SkillActionBase
{
    WzVector m_wvCurPos;      // 공격자의 현재 좌표
    WzVector m_wvDestPos;     // 공격자의 목적지 좌표
    DWORD    m_dwAttackerHP;  // 스킬을 쓰고난 후 공격자의 HP
    DWORD    m_dwAttackerMP;  // 스킬을 쓰고난 후 공격자의 MP
    BYTE     m_byNumberOfTargets     : 6; // 공격 대상의 수량
    BYTE     m_byNumberOfFieldEffect : 2; // 필드에 걸리는 이펙트가 있을 경우 그 수량
};

struct SkillActionDelayStart : public SkillActionBase
{
    WzVector m_wvCurPos;  // 공격자의 현재 좌표
    WzVector m_wvDestPos; // 공격자의 목적지 좌표
};

struct SkillActionDelayResult : public SkillActionBase
{
    DWORD m_dwAttackerHP; // 스킬을 쓰고난 후 공격자의 HP
    DWORD m_dwAttackerMP; // 스킬을 쓰고난 후 공격자의 MP
    BYTE  m_byNumberOfTargets : 6; // 공격 대상의 수량
    BYTE  m_byNumberOfFieldEffect : 2; // 필드에 걸리는 이펙트가 있을 경우 그 수량
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
    DWORD    m_dwObjectKey; // 스킬 소유자 오브젝트키
    SLOTCODE m_SkillCode;   // Skill 코드

    void Clear() { ZeroMemory(this, sizeof(*this)); }
};

WAR_MSG_DECL(SKILL_PERIODIC_DAMAGE_BRD)
{
    DWORD       m_dwAttackerKey; // 주기데미지를 발생시킨 공격자 키
    SLOTCODE    m_SkillCode;     // 주기데미지를 발생시키는 스킬코드
    BYTE        m_byNumberOfTargets; // 공격 대상의 수량
    DAMAGE_INFO m_DamageInfo[MAX_TARGET_NUM]; // 공격 대상의 데미지 정보

    void Clear() { ZeroMemory(this, sizeof(*this)); }
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(m_DamageInfo);
        return static_cast<uint16_t>(kMinSize + sizeof(m_DamageInfo[0]) * m_byNumberOfTargets);
    }

};

WAR_MSG_DECL(SKILL_REFLECT_DAMAGE_BRD)
{
    DWORD      m_dwAttackerKey; // 주기데미지를 발생시킨 공격자 키
    DWORD      m_dwTargetKey;   // 공격 당하는 오브젝트 키
    SLOTCODE   m_SkillCode;     // 피격 스킬코드
    WORD       m_wAbilityCode;  // 피격 어빌리티 코드
    DAMAGETYPE m_wDamage;       // 공격 데미지
    DWORD      m_dwTargetHP;    // 공격 후 타겟의 HP
    DWORD      m_dwTargetResrveHP; // 공격후 타겟의 적립된 HP

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
    DWORD      m_dwAttackerKey; // 흡수를 발생시킨 공격자 키
    SLOTCODE   m_SkillCode;     // 주기데미지를 발생시키는 스킬코드
    DAMAGETYPE m_wTargetDamage; // 흡수 데미지
    DWORD  m_dwTargetHP;   // 타겟의 남은 HP
    DWORD  m_dwTargetMP;   // 타겟의 남은 MP
    DWORD  m_dwAttackerHP; // 공격자의 남은 HP
    DWORD  m_dwAttackerMP; // 공격자의 남은 MP

    void Clear() { ZeroMemory(this, sizeof(*this)); }
};

WAR_MSG_DECL(SKILL_VITAL_SUCTION_BRD)
{
    DWORD m_dwAttackerKey; // 흡수를 발생시킨 공격자 키
    DWORD m_dwTargetKey;   // 타겟의 키.
    DWORD m_dwHP; // 타겟의 남은 HP

    void Clear() { ZeroMemory(this, sizeof(*this)); }
};


//==================================================================================================
#pragma pack(pop)

}; //end of namespace 'nsPacket'

//==================================================================================================

