#ifndef __GAMESERVER_PLAYER_INL
    #error "Can't direct include this header file"
#endif

//==================================================================================================

//--------------------------------------------------------------------------------------------------

// (UNUSED)
inline BOOL
Player::IsDBCharState(eDBCHAR_STATE stat) {
    BOOLEAN result = char_info_.m_byCharState == stat;
    return result;
}

//--------------------------------------------------------------------------------------------------

inline ePLAYER_BEHAVE_STATE
Player::GetBehave() const {
    return m_BehaveState;
}

//--------------------------------------------------------------------------------------------------
// below interfaces is extracted from the SCPlayer

inline ePLAYER_STATE Player::GetState() const
{
    return player_state_;
}

inline void Player::SetState(ePLAYER_STATE player_state)
{
    player_state_ = player_state;
}

inline const BASE_PLAYERINFO* Player::GetCharInfo() const
{
    return &char_info_;
}

inline BASE_PLAYERINFO* Player::GetCharInfo()
{
    return &char_info_;
}

//--------------------------------------------------------------------------------------------------

inline void
Player::ResumeTimer() {
    m_DBPUpdateTimer.Reset();
}

inline void
Player::SuspendTimer() {
    m_DBPUpdateTimer.DisableCheckTime();
}

//--------------------------------------------------------------------------------------------------

inline DWORD
Player::GetPlayerTitle() const {
    return m_PlayerTitle;
}

//--------------------------------------------------------------------------------------------------

inline void
Player::SetUserKey(DWORD dwUserKey) {
    m_dwUserKey = dwUserKey;
}

inline DWORD
Player::GetUserKey() const {
    return m_dwUserKey;
}

inline const char*
Player::GetUserID() const {
    return m_pszUserID;
}

inline void
Player::SetUserID(const char* pszID) {
    memcpy(m_pszUserID, pszID, sizeof(char)*ID_MAX_LEN);
    m_pszUserID[ID_MAX_LEN]='\0';
}

inline const char*
Player::GetUserIP() const {
    return static_cast<const char*>(m_pszUserIP);
}

inline void
Player::SetUserIP(const char* pszIP) {
    memcpy(m_pszUserIP, pszIP, IP_MAX_LEN); m_pszUserIP[IP_MAX_LEN]='\0';
}

//--------------------------------------------------------------------------------------------------

inline void
Player::SetLoginTime_t(time_t time) {
    m_LoginTime_t = time;
}

inline time_t
Player::GetLoginTime_t() const {
    return m_LoginTime_t;
}

//--------------------------------------------------------------------------------------------------

inline void
Player::SetDebugInfoFlag(BYTE byFlag) {
    m_byDebugInfoFlag = byFlag;
}

inline BYTE
Player::GetDebugInfoFlag() const {
    return m_byDebugInfoFlag;
}

//--------------------------------------------------------------------------------------------------

inline void
Player::SetServerSession(ServerSession* pSession) {
    m_pServerSession = pSession;
}

inline ServerSession*
Player::GetServerSession() const {
    return m_pServerSession;
}

//--------------------------------------------------------------------------------------------------

inline void
Player::SetLastAttackSeq(BYTE byAttackSequence) {
    m_byLastAttackSeq = byAttackSequence;
}

inline BYTE
Player::GetLastAttackSeq() const {
    return m_byLastAttackSeq;
}

//--------------------------------------------------------------------------------------------------

inline MovementChecker*
Player::GetMovementChecker() {
    return m_pMovementChecker;
}

inline BOOLEAN
Player::IsDoingAction() const {
    return m_bDoingAction;
}

inline ItemManager*
Player::GetItemManager() const {
    return m_pItemManager;
}

inline QuickManager*
Player::GetQuickManager() const {
    return m_pQuickManager;
}

inline SkillSlotManager*
Player::GetSkillSlotManager() const {
    return m_pSkillSlotManager;
}

// _NA002217_100728_EP2_PERK_SYSTEM_
inline nsSkill::PerkManager* Player::GetPerkManager() const {
    return perk_manager_;
}

inline Player*
Player::GetPlayerOwner() {
    return this;
}

inline eCHAR_TYPE
Player::GetCharType() const {
    eCHAR_TYPE char_type = static_cast<eCHAR_TYPE>(char_info_.m_byClassCode);
    return char_type;
}

inline LEVELTYPE
Player::GetLevel() const {
    return char_info_.m_LV;
}

#ifdef _NA_002050_20110216_ADD_GENDER
inline BYTE Player::GetGender() const
{
    return char_info_.gender;
}
#endif

//--------------------------------------------------------------------------------------------------

inline EXPTYPE
Player::GetNextExp() const {
    return m_NextExp;
}

//--------------------------------------------------------------------------------------------------

inline DWORD
Player::GetRemainStatPoint() const {
    return char_info_.m_iRemainStat;
}

inline DWORD
Player::GetRemainSkillPoint() const {
    return char_info_.m_iRemainSkill;
}

//--------------------------------------------------------------------------------------------------

inline MONEY
Player::GetMoney() const {
    return char_info_.m_Money;
}

inline void
Player::SetMoney(MONEY money) {
    char_info_.m_Money = money;
}

//--------------------------------------------------------------------------------------------------

inline float
Player::GetBonusMoneyRatio() const {
    int value = m_Attr.GetBonusMoneyRatio();
    return value / 100.f;
}

inline float
Player::GetBonusExpRatio() const {
    int value = m_Attr.GetBonusExpRatio();
    return value / 100.f;
}

inline float
Player::GetReflectDamageRatio() const {
    int value = m_Attr.GetReflectDamageRatio();
    return value / 100.f;
}

//--------------------------------------------------------------------------------------------------

// 제한처리를 위해서 : 캐릭터가 가진 기본 수치가 필요
inline short
Player::GetLimitStrength() const {
    int base = m_Attr.GetBaseSTR();
    int option = m_Attr.GetItemSTR();
    return short(base + option);
}

inline short
Player::GetLimitDexterity() const {
    int base = m_Attr.GetBaseDEX();
    int option = m_Attr.GetItemDEX();
    return short(base + option);
}

inline short
Player::GetLimitVitality() const {
    int base = m_Attr.GetBaseVIT();
    int option = m_Attr.GetItemVIT();
    return short(base + option);
}

inline short
Player::GetLimitSpirit() const {
    int base = m_Attr.GetBaseSPR();
    int option = m_Attr.GetItemSPR();
    return short(base + option);
}

inline short
Player::GetLimitInteligence() const {
    int base = m_Attr.GetBaseINT();
    int option = m_Attr.GetItemINT();
    return short(base + option);
}

inline short
Player::GetLimitExperty1() const {
    int base = m_Attr.GetBaseExperty1();
    int option = m_Attr.GetItemExperty1();
    return short(base + option);
}

inline short
Player::GetLimitExperty2() const {
    int base = m_Attr.GetBaseExperty2();
    int option = m_Attr.GetItemExperty2();
    return short(base + option);
}

//--------------------------------------------------------------------------------------------------

// 스텟 제한 수치 감소
inline short
Player::GetDecreaseLimitStat() const {
    return static_cast<short>(m_Attr.GetDecreaseLimitStat());
}

//--------------------------------------------------------------------------------------------------

inline SLOTCODE
Player::GetBaseSelectedStyle() const {
    return static_cast<SLOTCODE>(char_info_.m_iSelectStyleCode);
}

//--------------------------------------------------------------------------------------------------

// 추가적인 이동속도
inline void
Player::SetAddMoveSpeedRatio(float fRatio) {
    m_fAddMoveSpeedRatio = fRatio;
}

inline float
Player::GetAddMoveSpeedRatio() const {
    return m_fAddMoveSpeedRatio;
}

//--------------------------------------------------------------------------------------------------

// 미션 퀘스트 관련
inline MissionManager*
Player::GetMissionManager() const {
    return m_pMissionManager;
}

inline QuestManager_Concrete*
Player::GetQuestManager() const {
    return m_pQuestManager;
}

inline IQuickStyleManager*
Player::GetStyleManager() const {
    // WARNING | this interface is occurred an implicit fault by a forced type casting,
    // but, this method call, at a using position, hope to calculated valid casting.
    return (IQuickStyleManager*)m_pSkillSlotManager;
}

inline AchievementManager*
Player::GetACManager() const {
    return m_pACManager;
}

//--------------------------------------------------------------------------------------------------
// 거래 
inline void
Player::SetTradeContainer(TradeSlotContainer* pContainer) {
    m_pTradeSlotContainer = pContainer;
}

inline TradeSlotContainer*
Player::GetTradeContainer() const {
    return m_pTradeSlotContainer;
}

//----------------------------------------------------------------------------------------------
// 개인 상점 
inline void
Player::SetVendorContainer(VendorSlotContainer* pContainer) {
    m_pVendorSlotContainer = pContainer;
}

inline VendorSlotContainer*
Player::GetVendorContainer() const {
    return m_pVendorSlotContainer;
}

inline void
Player::SetVendorPage(VendorPage* pVendorPage) {
    m_pVendorPage = pVendorPage;
}

inline VendorPage*
Player::GetVendorPage() const {
    return m_pVendorPage;
}

//--------------------------------------------------------------------------------------------------
// 이벤트 인벤토리
inline EventInventorySlotContainer*
Player::GetEventSlotContainer() const {
    return m_pEventSlotContainer;
}

//--------------------------------------------------------------------------------------------------
// PVP
inline PVPInfo&
Player::GetPVPInfo() const {
    return *m_pPVPInfo;
}

inline CompetitionInfo&
Player::GetCompetitionInfo() const {
    return *m_pCompetitionInfo;
}

//--------------------------------------------------------------------------------------------------
#ifdef _NA_20100307_BETAKEY_SYSTEM
inline BetaKeyInfo* const
Player::GetBetaKeyInfo() {
    return &betakey_info_;
}

inline void 
Player::SetBetaKeyInfo(BETAKEY betakey)
{
    betakey_info_ = betakey;
}
#endif

inline InvitateFriendInfo&
Player::GetInvitateFriend() const {
    return *m_pInvitateFriend;
}

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
inline AccumulatePoint* 
Player::GetAccumulatePoint() const {
    return m_pAccumulatePoint;
}
#endif

inline ChaoState&
Player::GetChaoState() const {
    return *m_pChaoState;
}

inline BOOL
Player::IsExpiredPlayer() const {
    return m_bExpiredPlayer;
}

// PC방 유저인지 체크(GM체크 추가-성현창)
inline void
Player::SetReservedValue(DWORD dwReservedValue) {
    m_dwReservedValue = dwReservedValue;
}

inline DWORD
Player::GetReservedValue() const {
    return m_dwReservedValue;
}

//--------------------------------------------------------------------------------------------------

inline void
Player::SetSts(BYTE bySts)
{
    m_bySts = bySts;
    m_bGMUser = (eGM_GRADE_BASE < m_bySts && m_bySts < eGM_GRADE_MAX);
}

inline BYTE
Player::GetSts() const {
    return m_bySts;
}

inline BOOL
Player::IsGMUser() const {
    return m_bGMUser;
}

inline void
Player::SetPCRoomSts(BYTE bySts) {
    m_PCRoomSts = bySts;
}

inline BYTE
Player::GetPCRoomSts() const {
    return m_PCRoomSts;
}

//--------------------------------------------------------------------------------------------------

// CHARGUID
inline CHARGUID
Player::GetCharGuid() const {
    return char_info_.m_CharGuid;
}

/// USERGUID 
inline USERGUID
Player::GetUserGuid() const {
    return char_info_.m_UserGuid;
}

inline DWORD Player::GetCharNameHash() const { // CHANGES: f110517.3L
    return char_name_hash_;
};
//--------------------------------------------------------------------------------------------------

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
inline GAMECHUUSN
Player::GetGameChuUsn() const {
    return char_info_.gamechu_usn_;
}
#endif

inline GUILDGUID
Player::GetGuildGuid() const {
    return char_info_.m_GuildGuid;
}

inline const char*
Player::GetGuildName() const {
    return char_info_.m_tszGuildName;
}

inline BOOL
Player::IsGuildWarehouseRight() const {
    return m_GuildWarehouseRight;
}

inline void
Player::SetGuildWarehouseRight(BOOL IsRight) {
    m_GuildWarehouseRight = IsRight;
}

//--------------------------------------------------------------------------------------------------

// 클라이언트 렌더링 설정 옵션
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    inline BYTE Player::GetHelmetOption() const 
    {
        return char_info_.VisibleOptionFlag.m_Helmet;
    }
    inline void Player::SetHelmetOption(BYTE byOption) 
    {
        char_info_.VisibleOptionFlag.m_Helmet = byOption;
    }
    inline BYTE Player::GetWingOption() const 
    {
        return char_info_.VisibleOptionFlag.m_Wing;
    }
    inline void Player::SetWingOption(BYTE byOption) 
    {
        char_info_.VisibleOptionFlag.m_Wing = byOption;
    }
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    inline BYTE Player::GetCostumeOption() const
    {
       return char_info_.VisibleOptionFlag.m_Costume;
    }
    inline void Player::SetCostumeOption(BYTE byOption) 
    {
        char_info_.VisibleOptionFlag.m_Costume = byOption;
    }
#endif // _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
#else 
    inline BYTE Player::GetHelmetOption() const 
    {
        return char_info_.m_byInvisibleOptFlag;
    }
    inline void Player::SetHelmetOption(BYTE byOption) 
    {
        char_info_.m_byInvisibleOptFlag = byOption;
    }
    inline BYTE Player::GetWingOption() const 
    {
        return 0;
    }
    inline void Player::SetWingOption(BYTE byOption) 
    {
    }
#endif
//
//--------------------------------------------------------------------------------------------------

// 피도로 관련
inline FatigueManager*
Player::GetFatigueManager() const {
    return m_pFatigueManager;
}

//--------------------------------------------------------------------------------------------------
/*
inline TriggerPlayerLink*
Player::GetTriggerLink() {
    return &m_TriggerLink;
}; //__NA000000_090613_TRIGGER_LOGIC_REARRANGE__
*/
// (CHANGES) (f100614.4L)
inline ns_trigger::TriggerPlayerLinkInGameSlots*
Player::GetTriggerLinks() {
    return &trigger_links_;
}

// CHANGES: f110103.3L
inline ns_object::PlayerZoneTransaction* Player::GetZoneTransactionInfo()
{
    return &zone_transaction_info_;
}

//--------------------------------------------------------------------------------------------------

// 파라미터 관련
inline void
Player::SetExp(EXPTYPE exp) {
    char_info_.m_i64Exp = exp;
}

inline void
Player::SetLevel(LEVELTYPE level) {
    char_info_.m_LV = level;
    m_Attr.UpdateEx();
}

//--------------------------------------------------------------------------------------------------

inline IPlayerEventD&
Player::PlayerEvent() {
    return m_PlayerEvent;
};

inline sTransaction&
Player::EventTransaction() {
    return m_PlayerEvent.EventTransaction();
} // 이벤트 트랜잭션

//--------------------------------------------------------------------------------------------------

inline util::SimpleTimer&
Player::GetSnapShotLogTimer() {
    return m_ShapShotLogTimer;
}

inline util::SimpleTimer&
Player::GetFatigueLogTimer() {
    return m_FatigueLogTimer;
}

inline util::SimpleTimer&
Player::GetLoadingCheckTimer() {
    return m_LoadingCheckTimer;
}

//--------------------------------------------------------------------------------------------------

inline STATE_GM_INFO&
Player::GetGMStateInfo() {
    return m_GMStateInfo;
}

//--------------------------------------------------------------------------------------------------

inline ItemOwnFlag&
Player::GetItemOwnFlag() {
    return m_ItemOwnFlag;
}

//--------------------------------------------------------------------------------------------------

inline WarehouseInventoryProtector&
Player::GetWarehouseInventoryProtector() {
    return m_WarehouseInventoryProtector;
}

inline const WarehouseInventoryProtector& Player::GetWarehouseInventoryProtector() const
{
    return m_WarehouseInventoryProtector;
}

//--------------------------------------------------------------------------------------------------

inline BOOL
Player::GetCheckedStat() const {
    return m_bCheckedStat;
}

//--------------------------------------------------------------------------------------------------

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
inline void
Player::SetPlayTime(WORD wPlayTime) {
    m_wPlayTime = wPlayTime;
}

inline WORD
Player::GetPlayTime() const {
    return m_wPlayTime;
}

inline void
Player::SetOfflineTime(WORD wOfflineTime) {
    m_wOfflineTime = wOfflineTime;
}

inline WORD
Player::GetOfflineTime() const {
    return m_wOfflineTime;
}

inline void
Player::SetAge(BYTE byAge) {
    m_byAge = byAge;
}

inline BYTE
Player::GetAge() const {
    return m_byAge;
}

inline void
Player::SetRealNameAuth(BYTE byRealNameAuth) {
    m_byRealNameAuth = byRealNameAuth;
}

inline BYTE
Player::GetRealNameAuth() const {
    return m_byRealNameAuth;
}
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE

//--------------------------------------------------------------------------------------------------

inline BOOL&
Player::UnexpectedUserEnterRoom() {
    return m_bUCUserEnterRoom;
}

//--------------------------------------------------------------------------------------------------
// 파라미터
// (CHANGES) (091212) (WAVERIX) original code : implicit inline virtual methods in header files
// move to 'inl' file

inline /*virtual*/ Attributes&
Player::GetAttr() {
    return m_Attr;
}

inline /*virtual*/ const Attributes&
Player::GetAttr() const {
    return m_Attr;
}

// CHANGES: f110217.2L, added
inline PlayerAttributes& Player::GetPlayerAttr() {
    return m_Attr;
}

inline const PlayerAttributes& Player::GetPlayerAttr() const {
    return m_Attr;
}

// TODO : 하기 inline virtual method들은 연관관계를 생각해서 재 수정하자.
// Character에 들어가도 충분한 내용도 있고 그렇지 않는 것들도 있다.
inline /*virtual*/ float
Player::GetNormalRangeBonus() const {
    int value = m_Attr.GetNormalRangeBonus();
    return value / 10.0f;
}

inline /*virtual*/ float
Player::GetSkillRangeBonus() const {
    int value = m_Attr.GetSkillRangeBonus();
    return value / 10.0f;
}

inline /*virtual*/ float
Player::GetSightRange() const {
    int value = m_Attr.GetSightRange();
    return value / 10.0f;
}

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
inline /*virtual*/ eARMOR_TYPE
Player::GetArmorType() const {
    return eARMOR_HARD;
}

inline /*virtual*/ eMELEE_TYPE
Player::GetMeleeType() const {
    return eMELEE_TYPE_HIT;
}
#endif

inline /*virtual*/ EXPTYPE
Player::GetExp() const {
    return char_info_.m_i64Exp;
}

inline /*virtual*/ DWORD
Player::GetHP() const {
    return char_info_.m_iHP;
}

inline /*virtual*/ DWORD
Player::GetMP() const {
    return char_info_.m_iMP;
}

//{__NA_001290_20090525_SHIELD_SYSTEM
inline /*virtual*/ DWORD
Player::GetSD() const {
    return char_info_.m_iSD;
}

inline /*virtual*/ void
Player::SetSD(DWORD dwSD) {
    // (CHANGES) remove continuous calls
    // (CHANGES) changes to int type to check boundary
    const int maxSD = GetMaxSD();
    char_info_.m_iSD = BOUND_CHECK(0, ((const int)(dwSD)), maxSD);
}

inline /*virtual*/ DWORD
Player::GetMaxSD() const {
    return m_Attr.GetMaxSD();
}

inline /*virtual*/ int
Player::GetRegenSD() const {
    return m_Attr.GetRecoverSD();
}
//}__NA_001290_20090525_SHIELD_SYSTEM

inline /*virtual*/ DWORD
Player::GetMaxHP() const {
    return m_Attr.GetMaxHP();
}

inline /*virtual*/ DWORD
Player::GetMaxMP() const {
    return m_Attr.GetMaxMP();
}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
inline PLAYERFP Player::GetMaxFP() const
{
    return m_Attr.GetMaxFP();
}

inline PLAYERFP Player::GetFP() const
{
    return char_info_.cur_FP_;
}

inline BOOL Player::ChkNeedFP( PLAYERFP fp_spend )
{
    return GetFP() < abs(fp_spend) ? FALSE:TRUE; 
}
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

inline /*virtual*/ const TCHAR*
Player::GetCharName() const {
    return char_info_.m_tszCharName;
}

inline /*virtual*/ int
Player::GetRegenHP() const {
    return m_Attr.GetRecoverHP();
}

inline /*virtual*/ int
Player::GetRegenMP() const {
    return m_Attr.GetRecoverMP();
}

inline /*virtual*/ int
Player::GetMPSpendIncValue() const {
    return m_Attr.GetMPSpendIncrease();
}

inline /*virtual*/ float
Player::GetMPSpendIncRatio() const {
    int value = m_Attr.GetRatioMPSpendIncrease();
    return value / 100.0f;
}

inline /*virtual*/ short
Player::GetStrength() const {
    return static_cast<short>(m_Attr.GetSTR());
}

inline /*virtual*/ short
Player::GetDexterity() const {
    return static_cast<short>(m_Attr.GetDEX());
}

inline /*virtual*/ short
Player::GetVitality() const {
    return static_cast<short>(m_Attr.GetVIT());
}

inline /*virtual*/ short
Player::GetSpirit() const {
    return static_cast<short>(m_Attr.GetSPR());
}

inline /*virtual*/ short
Player::GetInteligence() const {
    return static_cast<short>(m_Attr.GetINT());
}

inline /*virtual*/ DWORD
Player::GetPhysicalAvoidValue() const {
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    return m_Attr.GetPhysicalAvoidRate();
#else
    LEVELTYPE level = GetLevel();
    int rate = m_Attr.GetPhysicalAvoidRate();

    return DWORD(level / 5 + rate);
#endif
}

inline /*virtual*/ float
Player::GetPhysicalAttackSpeed() const {
    int value = m_Attr.GetAttSpeedRatio();
    return value / 100.0f;
}

inline /*virtual*/ short
Player::GetExperty1() const {
    return static_cast<short>(m_Attr.GetExperty1());
}

inline /*virtual*/ short
Player::GetExperty2() const {
    return static_cast<short>(m_Attr.GetExperty2());
}

inline /*virtual*/ int
Player::GetMoveSpeedRatio() const {
    return m_Attr.GetMoveSpeedRatio();
}

inline /*virtual*/ int
Player::GetAttSpeedRatio() const {
    return m_Attr.GetAttSpeedRatio();
}

inline /*virtual*/ void
Player::SetHP(DWORD dwHP) {
    // (CHANGES) remove continuous calls
    // (CHANGES) changes to int type to check boundary
    const int maxHP = GetMaxHP();
    char_info_.m_iHP = BOUND_CHECK(0, ((const int)(dwHP)), maxHP);
}

inline /*virtual*/ void
Player::SetMP(DWORD dwMP) {
    // (CHANGES) remove continuous calls
    // (CHANGES) changes to int type to check boundary
    const int maxMP = GetMaxMP();
    char_info_.m_iMP = BOUND_CHECK(0, ((const int)(dwMP)), maxMP);
}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
inline void Player::SetFP( PLAYERFP fp )
{
    const PLAYERFP maxfp = GetMaxFP();
    char_info_.cur_FP_ = BOUND_CHECK(0, fp, maxfp);
}
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

//--------------------------------------------------------------------------------------------------

inline void
Player::SetGroupHighMemberInfo(eGROUP_STAT eHighMember ) {
    m_eHighMember = eHighMember;
}

inline void
Player::SetGroupLowMemberInfo(eGROUP_STAT eLowMember ) {
    m_eLowMember = eLowMember;
}

inline eGROUP_STAT
Player::GetGroupHighMemberInfo() const {
    return m_eHighMember;
}

inline eGROUP_STAT
Player::GetGroupLowMemberInfo() const {
    return m_eLowMember;
}

#if defined (_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC) || \
    defined (_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION)
inline PostManager*
Player::GetPostManager() const {
    return m_PostManager;
}
#endif

//--------------------------------------------------------------------------------------------------

template< typename _MSGSUBTYPE >
inline void
Player::SendPacketForSync(const _MSGSUBTYPE* const pMsgSub)
{
    const BYTE _TAG = pMsgSub->value;
    SendPacketForSync_(_TAG,
        static_cast<WAR_TAG_V*>(const_cast<_MSGSUBTYPE*>(pMsgSub)),
        pMsgSub->GetSize());
}

//--------------------------------------------------------------------------------------------------

inline void
Player::NotifyChangedStatus(PlayerStatEvent::eFlag flag)
{
    if(flag != PlayerStatEvent::eFlag_None)
        m_PartyEvent = PlayerStatEvent::eFlag(m_PartyEvent | flag);
    else
        m_PartyEvent = PlayerStatEvent::eFlag_None;
}

// (CHANGES) (f100324.3L)
inline const nsSlot::InventoryConfig&
Player::inventory_config() const
{
    return char_info_.m_InventoryConfig;
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
inline HonorManager* 
Player::GetHonorManager() const
{
    return honor_manager_;
}
#endif //_NA_003027_20111013_HONOR_SYSTEM
