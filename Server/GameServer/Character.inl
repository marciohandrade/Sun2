#ifndef __GAMESERVER_CHARACTER_INL
    #error "Can't direct include this header file"
#endif

// (CHANGES) (091213) (WAVERIX) move light-weight impl. in header to 'inl' code-file

//==================================================================================================
//  Lifetime control
inline /*virtual*/ void
Character::Reuse() {
    Release();
}

//==================================================================================================
//  Property control
inline void
Character::ClearKillerInfo() {
    m_KillerType = MAX_OBJECT;
    m_dwKillerObjectKey = 0;
}


inline eZONE_STATE
Character::GetGameZoneState() const {
    return m_eGameZoneState;
}

inline GameZone*
Character::GetGameZonePtr() const {
    return m_pGameZone;
}

inline CPathExplorer*
Character::GetPathExplorer() const {
    return m_pPathExplorer;
}


inline /*virtual*/ void
Character::_SetPosVector(const WzVector* const pwzVector) {
    __UNUSED(pwzVector);
    __debugbreak();
} // protector

inline /*virtual*/ int
Character::GetResistBadStatusRatio(WORD wStateID) {
    __UNUSED(wStateID);
    return 0;
}

inline /*virtual*/ Player*
Character::GetPlayerOwner() {
    return NULL;
}



inline /*virtual*/ LEVELTYPE
Character::GetDisplayLevel() const {
    return GetLevel();
}


inline /*virtual*/ int
Character::GetRegenHP() const {
    return 0;
}

inline /*virtual*/ int
Character::GetRegenMP() const {
    return 0;
}

inline /*virtual*/ int
Character::GetRegenSD() const {
    return 0;
}

inline /*virtual*/ BOOL
Character::IsAlive() const {
    return GetHP() > 0;
}

inline /*virtual*/ BOOL
Character::IsDead() const {
    return !IsAlive();
}

//virtual
inline short Character::GetExperty1() const {
    return 0;
}

//virtual
inline short Character::GetExperty2() const {
    return 0;
}

//virtual
inline float Character::GetBodyRadius() const {
    return 0;
}

//virtual
inline int   Character::GetMPSpendIncValue() const {
    return 0;
}

//virtual
inline float Character::GetMPSpendIncRatio() const {
    return 0;
}

inline MoveStateControl*
Character::GetMoveStateControl() const {
    return m_pMoveStateControl;
}


inline /*virtual*/ EXPTYPE
Character::AddExp(EXPTYPE exp, DWORD dwTargetObjKey, float fBonusRatio, BOOL bSendPacket) {
    __UNUSED((exp, dwTargetObjKey, fBonusRatio, bSendPacket));
    return 0;
}

inline void
Character::SetShield(int iShieldHP, int iDecreaseMP, float fRatio, float DecreaseMPRatio) {
    m_iShieldHP = iShieldHP;
    m_iShieldMP = iDecreaseMP;
    decrease_MP_Ratio = DecreaseMPRatio;
    m_fShieldAbsorbRatio = fRatio;
}

inline /*virtual*/ void
Character::UpdateCalcRecover(BOOL bHPUpdate, BOOL bMPUpdate, BOOL bSDUpdate) {
    __UNUSED(bHPUpdate); __UNUSED(bMPUpdate); __UNUSED(bSDUpdate);
}

inline /*virtual*/ void
Character::SendAIMessage(AI_MSG* pMsg, WORD wSize, DWORD dwDelay) {
    __UNUSED(pMsg); __UNUSED(wSize); __UNUSED(dwDelay);
}


inline ObjectList&
Character::GetObservers() {
    return m_Observers;
}

// (CHANGES) (091213) add const interface
inline const ObjectList&
Character::GetObservers() const {
    return m_Observers;
}

// 자신을 소환한 캐릭터 //_NA_000000_20140224_SUMMONER_REFACTORING
inline DWORD
Character::GetSummonerKey() const {
    return m_dwSummonerKey;
}

inline void
Character::SetSummonerKey(DWORD dwSummonerKey) {
    m_dwSummonerKey = dwSummonerKey;
}

// 파티 상위 개념인 팀 구현 2013.01.04 / 김진휘
inline ChaosZoneTeam Character::GetTeamKey() const
{
    return team_key_;
}
inline void Character::SetTeamKey(const ChaosZoneTeam team_key)
{
    team_key_ = team_key;
}

inline PartyState&
Character::GetPartyState() {
    return m_PartyState;
}

inline StatusManager*
Character::GetStatusManager() const {
    return m_pStatusManager;
}

inline SkillManager*
Character::GetSkillManager() const {
    return m_pSkillManager;
}

// CHANGES, f100824.7L
//inline /*virtual*/ bool
//Character::CanApplyPassiveSkill(SLOTCODE skill_code) {
//    __UNUSED(skill_code);
//    return false;
//}

inline void
Character::AddFightingEnergy(short sCount) {
    short sum = m_sFightingEnergyCount + sCount;
    m_sFightingEnergyCount = min(MAX_FIGHTING_ENERGY_NUM, max(0, sum));
}

inline short
Character::GetFightingEnergy() const {
    return m_sFightingEnergyCount;
}

inline /*virtual*/ BOOL
Character::CheckClassDefine(SLOTCODE SkillCode, BOOL bUseActiveSkill) {
    __UNUSED((SkillCode, bUseActiveSkill));
    return TRUE;
}


inline /*virtual*/ void
Character::SetTargetChar(Character* pChar) {
    __UNUSED(pChar);
}

inline /*virtual*/ Character*
Character::GetTargetChar() {
    return NULL;
}

inline /*virtual*/ void
Character::AddBattlePoint(Character* pAttacker, eBATTLE_POINT_TYPE ePointType, int iBattlePoint) {
    __UNUSED(pAttacker); __UNUSED(ePointType); __UNUSED(iBattlePoint);
}

inline /*virtual*/ eUSER_RELATION_TYPE
Character::IsFriend(Character* pTarget) {
    __UNUSED(pTarget);
    return USER_RELATION_NEUTRALIST;
}

inline /*virtual*/ void
Character::ChangeState(DWORD dwStateID, LPARAM param1, LPARAM param2, LPARAM param3) {
    __UNUSED(dwStateID); __UNUSED(param1); __UNUSED(param2); __UNUSED(param3);
}

inline /*virtual*/ void
Character::ForcedAttack(Character* pTarget) {
    __UNUSED(pTarget);
}

inline /*virtual*/ BOOL
Character::CanResurrection(Character* pTarget) {
    __UNUSED(pTarget);
    return TRUE;
}

//적립된 HP
inline void
Character::SetReserveHP(DWORD dwReserveHP) {
    m_dwReserveHP = dwReserveHP;
}

inline DWORD
Character::GetReserveHP() const {
    return m_dwReserveHP;
}

//적립된 HP
inline void
Character::SetDeadReserveHP(DWORD dwDeadReserveHP) {
    m_dwDeadReserveHP = dwDeadReserveHP;
}

inline DWORD
Character::GetDeadReserveHP() const {
    return m_dwDeadReserveHP;
}

inline eCHAR_DEAD_TYPE
Character::GetDeadType() const {
    return m_DeadType;
}

inline void
Character::SetDeadType(eCHAR_DEAD_TYPE DeadType) {
    m_DeadType = DeadType;
}

inline eZONE_STATE
Character::GetGameZone(GameZone*& OUT pZone, MAPCODE& OUT EnterFieldCodeOut,
                       WzID* OUT pAreaID, WzVector* OUT pwvStartPos)
{
    pZone = m_pGameZone;
    EnterFieldCodeOut = m_GameFieldCode;
    if(pAreaID)
        *pAreaID = m_GameAreaID;
    if(pwvStartPos)
        *pwvStartPos = m_GameStartPos;
    return m_eGameZoneState;
}

//virtual
inline BOOL Character::SendPacket(MSG_BASE_FORWARD* pMsg, WORD wSize, BOOL bCrypt)
{
    __UNUSED((pMsg, wSize, bCrypt));
    return true;
}

//virtual
inline BOOL Character::SendExPacket(DWORD dwNumberOfMessages, BYTE **pMsg, WORD *pwSize)
{
    __UNUSED((dwNumberOfMessages, pMsg, pwSize));
    return true;
}


template< typename _MSGSUBTYPE >
inline void
Character::SendPacketAroundForSync(const _MSGSUBTYPE* const pMsgSub, const BOOLEAN bPostAction)
{
    GameField* const pGameField = GetField();
    if(!pGameField)
        return;
    pGameField->SendPacketAroundForSync(this, pMsgSub, bPostAction);
}


//virtual
inline BOOL Character::ExecuteThrust(BOOL bForcedMove, WzVector* pwvDestPos,
                                     WzVector& wvPosAfterThrust, float fMoveDistance,
                                     BOOL bDownAfterThrust)
{
    __UNUSED((bForcedMove, pwvDestPos, &wvPosAfterThrust, fMoveDistance, bDownAfterThrust));
    return false;
}

//virtual
inline void Character::SetActionDelay(DWORD delay)
{
    __UNUSED(delay);
}

//_NA_001231_20081216_ADD_OPTION_KIND
inline DAMAGETYPE  
Character::CalcDecreaseDamage(DAMAGETYPE damage, int resist_value, int resist_ratio)
{
    if (damage == 0) {
        return 0;
    }

    damage -= resist_value;
    if (resist_ratio != 0) {
        damage = static_cast<int>(damage * (1.0f - resist_ratio / 100.0f));
    };
    if (damage < 0) {
        return 0;
    }
    if (damage > DAMAGETYPE_MAX) {
        damage = DAMAGETYPE_MAX;
    }
    return damage;
}

//{__NA_001290_20090525_SHIELD_SYSTEM
inline const BOOL
Character::IsPCObject(const Character* pChar)
{
    if (pChar->IsEqualObjectKind(PLAYER_OBJECT))
    {
        return TRUE;
    }
    if (pChar->IsEqualObjectKind(SSQMONSTER_OBJECT))
    {
        return TRUE;
    }
#ifdef _NA_004910_20120601_SUMMON_PVP_RULE
    if (pChar->IsEqualObjectKind(SUMMON_OBJECT))
    {
        return TRUE;
    }
#endif // _NA_004910_20120601_SUMMON_PVP_RULE
    return FALSE;
}


inline void Character::Damaged(DamageArgs* damage_arg)
{
    return _Damaged(damage_arg);
}

