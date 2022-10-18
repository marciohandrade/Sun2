#ifndef __GAMESERVER_NPC_INL
    #error "Can't direct include this header file"
#endif


inline /*virtual*/ VOID
NPC::Reuse() {
    Release();
}


inline BASE_NPCINFO*
NPC::GetBaseInfo() const {
    return m_pBaseInfo;
}



inline /*virtual*/ Attributes&
NPC::GetAttr() {
    return m_Attr;
}

inline /*virtual*/ const Attributes&
NPC::GetAttr() const {
    return m_Attr;
}

inline /*virtual*/ LEVELTYPE
NPC::GetLevel() const {
    return m_pBaseInfo->m_Level;
}

inline /*virtual*/ LEVELTYPE
NPC::GetDisplayLevel() const {
    return m_pBaseInfo->m_DisplayLevel;
}

inline BYTE
NPC::GetGrade() const {
    return m_pBaseInfo->m_byGrade;
}

inline FLOAT
NPC::GetGradeExpRatio() {
    return m_pBaseInfo->m_fGradeExpRatio;
}

inline /*virtual*/ FLOAT
NPC::GetNormalRangeBonus() const {
    int value = m_Attr.GetNormalRangeBonus();
    return value / 10.0f;
}

inline /*virtual*/ FLOAT
NPC::GetSkillRangeBonus() const {
    int value = m_Attr.GetSkillRangeBonus();
    return value / 10.0f;
}

inline /*virtual*/ FLOAT
NPC::GetSightRange() const {
    int value = m_Attr.GetSightRange();
    return value / 10.0f;
}

inline /*virtual*/ eARMOR_TYPE
NPC::GetArmorType() const {
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    return eARMOR_HARD;
#else
    return (eARMOR_TYPE)m_pBaseInfo->m_byArmorType;
#endif
}

inline /*virtual*/ eMELEE_TYPE
NPC::GetMeleeType() const {
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    return eMELEE_TYPE_SLASH;
#else
    return (eMELEE_TYPE)m_pBaseInfo->m_byMeleeType;
#endif
}

inline /*virtual*/ eATTACK_TYPE
NPC::GetWeaponBaseAttType() const {
    return (eATTACK_TYPE)m_pBaseInfo->m_byAttType;
}

inline /*virtual*/ eATTACK_TYPE
NPC::GetWeaponMagicalAttType() const {
    return (eATTACK_TYPE)m_pBaseInfo->m_byAttType;
}

inline /*virtual*/ DWORD
NPC::GetHP() const {
    return m_dwHP;
}

inline /*virtual*/ DWORD
NPC::GetMP() const {
    return m_dwMP;
}

inline /*virtual*/ DWORD
NPC::GetMaxHP() const {
    return m_Attr.GetMaxHP();
}

inline /*virtual*/ DWORD
NPC::GetMaxMP() const {
    return m_Attr.GetMaxMP();
}

inline /*virtual*/ const TCHAR*
NPC::GetCharName() const {
    return m_pBaseInfo->m_pszName;
}

inline /*virtual*/ INT
NPC::GetRegenHP() const {
    return m_Attr.GetRecoverHP();
}

inline /*virtual*/ INT
NPC::GetRegenMP() const {
    return m_Attr.GetRecoverMP();
}

inline /*virtual*/ DWORD
NPC::GetPhysicalAvoidValue() const {
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    return m_Attr.GetPhysicalAvoidRate();
#else
    LEVELTYPE level = GetLevel();
    int rate = m_Attr.GetPhysicalAvoidRate();
    return DWORD(level / 2.5f + rate);
#endif
}

inline /*virtual*/ FLOAT
NPC::GetPhysicalAttackSpeed() const {
    int value = m_Attr.GetAttSpeedRatio();
    return value / 100.0f;
}

inline /*virtual*/ INT
NPC::GetMoveSpeedRatio() const {
    return (INT)m_Attr.GetMoveSpeedRatio();
}

inline /*virtual*/ INT
NPC::GetAttSpeedRatio() const {
    return m_Attr.GetAttSpeedRatio();
}

inline /*virtual*/ FLOAT
NPC::GetBodyRadius() const {
    return m_pBaseInfo->m_fBodyRadius;
}

//{__NA_001290_20090525_SHIELD_SYSTEM
inline /*virtual*/ DWORD
NPC::GetMaxSD() const {
    return m_Attr.GetMaxSD();
}

inline /*virtual*/ INT
NPC::GetRegenSD() const {
    return m_Attr.GetRecoverSD();
}

inline /*virtual*/ VOID
NPC::SetSD( DWORD dwSD ) {
    // (CHANGES) remove continuous calls
    // (CHANGES) changes to int type to check boundary
    const int maxSD = GetMaxSD();
    m_dwSD = BOUND_CHECK(0, ((const int)(dwSD)), maxSD);
}

inline /*virtual*/ DWORD
NPC::GetSD() const {
    return m_dwSD;
}
//}

inline /*virtual*/ VOID
NPC::SetMP( DWORD dwMP ) {
    // (CHANGES) remove continuous calls
    // (CHANGES) changes to int type to check boundary
    const int maxMP = GetMaxMP();
    m_dwMP = BOUND_CHECK(0, ((const int)(dwMP)), maxMP);
}

// 일단은 무시. 렙업하는 NPC도 있겠지?; ...없다.
inline /*virtual*/ EXPTYPE
NPC::GetExp() const {
    return 1;
}



inline NPCStateManager*
NPC::GetStateManager() const {
    return m_pNPCStatusManager;
}

inline TargetManager*
NPC::GetTargetManager() const {
    return m_pTargetManager;
}

inline NPC::SkillSelector&
NPC::GetSkillSelector() {
    return m_SkillSelector;
}

inline VOID
NPC::ChangeNpcDependencyChain( const INpcLinkActionD& rNewAction ) {
    m_LinkedAction = rNewAction;
}



inline SLOTCODE
NPC::GetSelectedSkill() const {
    return m_SelectedSkillCode;
}

inline WORD
NPC::GetSelectedSkillDelay() const {
    return m_wSelectedSkillDelay;
}


inline /*virtual*/ BOOL
NPC::IsOutOfWanderRadius() {
    return FALSE;
}

inline /*virtual*/ BOOL
NPC::IsOutOfRegenLocationLimit( WzVector& vTargetPos ) {
    __UNUSED(vTargetPos);
    return FALSE;
}

inline /*virtual*/ VOID
NPC::SetRegenPos( const WzVector& regenPos ) {
    __UNUSED(regenPos);
}



inline WzVector&
NPC::GetJumpPos() {
    return m_vJumpPos;
}

inline REGENID
NPC::GetRegenID() {
    if (m_pUnitRegenInfo != NULL)
        return m_pUnitRegenInfo->GetID();

    return 0;   // 4자리의 wzid가 dword로 변환된것 툴을 통해서 들어올 경우 0이 될 수 없다.
}

// CHANGES: f110311.4L add a slot type for a trigger area linker to support a multi-area link
// declared by _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
inline ns_trigger::TriggerNpcLinkInGameSlots*
NPC::GetTriggerLinks() {
    return &trigger_links_;
}

//_NA_0_20100817_HELLON_LAST_TRIGGER
inline CODETYPE 
NPC::GetTriggerAnimationCode() {
    return trigger_animation_code_;
}

inline void 
NPC::SetTriggerAnimationCode(CODETYPE code) {
    trigger_animation_code_ = code;
}

inline bool
NPC::IsFollower() {
    // 똘마니... 정도로 표현할 수 있을까?
    BOOLEAN is_member = !!IsGroupMember();
    BOOLEAN is_leader = !!IsLeader();

    return is_member && is_leader == false;
}

inline bool
NPC::IsGroupMember() {
    if(m_pUnitGroupInfo != 0) {
        // (NOTE) 옮기기 전... 이 그룹 id를 얻기 위해 얼마나 귀찮은 작업을 했던가...
        UNITGROUPID group_id = m_pUnitGroupInfo->GetID();
        return group_id != 0;
    }
    return false;
}

inline void
NPC::SetUnitGroupInfo( UnitGroupInfo* p ) {
    m_pUnitGroupInfo = p;
}

inline void
NPC::SetUnitRegenInfo( UnitRegenInfo* p ) {
    m_pUnitRegenInfo = p;
}  // 임시

inline UnitRegenInfo*
NPC::GetUnitRegenInfo() {
    return m_pUnitRegenInfo;
}

inline VOID
NPC::SetGMIdleOn( BOOL bIdleOn ) {
    m_bGMIdleOn = bIdleOn;
}

inline BOOL
NPC::ISGMIdleOn() const {
    return m_bGMIdleOn;
}
//_NA001385_20090924_DOMINATION_MAPNPC_AI_BUG
inline eTARGET_SEARCH_TYPE  
NPC::GetSearchType() const {
    return search_type_;
}

