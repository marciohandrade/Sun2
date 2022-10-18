// Player.h: interface for the Player class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __GAMESERVER_PLAYER_H
#define __GAMESERVER_PLAYER_H
#pragma once

//==================================================================================================
//	<INCLUDEs>
//
#include <Const.h>
#include <Struct.h>
#include <PacketStruct_CG.h>
#include <SCSlotContainer.h>
#include <SCPlayer.h>
#include <Timer.h>
#include <misc.h>

#include "../../SolarTrigger/TriggerMsg.h"
//
#include "Character.h"

#include "PlayerAttrForUpdate.h"
#include "VarMsg.h"
#include "ItemOwnFlag.h"
#include "PlayerEventManager.h"		//Event������ ����
#include "WareInvenProtector.h"
#include "./FriendInvitation/InvitateFriendInfo.h" //ģ�� �ʴ� �̺�Ʈ
#include "CPetManager.h"
#include "EtherEmissionDevice.h"

#ifdef _NA_20100307_BETAKEY_SYSTEM
#include <BetaKey.h>
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "HonorSystem/HonorManager.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "PointSystem/PointWalletManager.h"
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <World/ChaosZoneSystem/BattleGroundPlayerInfo.h>
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "World/ChaosZoneSystem/GoldRushPlayerInfo.h"
#include "World/ChaosZoneSystem/ChaosZoneManager.h"
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "CharacterInfoExManager.h"
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#include "World/ChaosZoneSystem/PlayerAssistMeter.h"
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#include "RatioManager.h" //_NA_000000_20130612_RANDOMIZER_CHANGE
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
#include "PlayerSpaInfo.h"
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
//==================================================================================================
//
//
#define __GAMESERVER_PLAYER_INL
#include "Player_NestedInfo.hxx"
#undef __GAMESERVER_PLAYER_INL
//
//
//==================================================================================================

class ChaosZone;
#ifdef _NA_008334_20150608_SONNENSCHEIN
class GameGuild;
#endif //_NA_008334_20150608_SONNENSCHEIN
//==================================================================================================
//	<DECLARATIONs>
struct PLAYER_SUMMON_INFO
{
    MONSTERCODE monster_code;
    DWORD hp;
    DWORD mp;
    DWORD remained_time;
    eSUMMON_COMMAND command;
    SLOTCODE skill_code;
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    eSUMMON_TYPE summoned_type;
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    
    PLAYER_SUMMON_INFO() : 
        monster_code(0), hp(0), mp(0), remained_time(0), command(eSUMMON_COMMAND_FOLLOW), skill_code(0)
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        ,summoned_type(eSUMMON_NORMAL)
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    {
    }
};

typedef std::list<PLAYER_SUMMON_INFO*>      PLAYER_SUMMON_INFO_LIST;
typedef PLAYER_SUMMON_INFO_LIST::iterator   PLAYER_SUMMON_INFO_LIST_ITER;

//_KR_0_20091021_FIRENDINVITATION_EXTEND = {
typedef STLX_MAP<DWORD, DWORD64>					RECALL_MSGBOX_TIME_MAP;
typedef RECALL_MSGBOX_TIME_MAP::iterator			RECALL_MSGBOX_TIME_MAP_ITER;
typedef std::pair< DWORD, DWORD64>					RECALL_MSGBOX_TIME_MAP_PAIR;
//}

//==================================================================================================
// <TYPE DEFINITIONS>
//
#define SNAPSHOT_LOG_CYCLE  (1000*60*10) // 10�� ���� ���� �� �����..
#define FATIGUE_LOG_CYCLE   (1000*60*60) // 1�ð� ���� �Ƿε� �����..

//==================================================================================================
//  <PLAYER CLASS DEFINITION>
//
class Player : public Character//,
               //public SCPlayer
{
private:
    friend class PlayerManager;
    friend class Village;
    friend class Zone;
    //friend class View; // (WAVERIX)(080528) - ���� ���� �������� �ӽ� ���� ���
    friend class PlayerOperator;
    enum 
    {
        _DBP_UPDATE_CYCLE_DELAY             = 1000*60*3,    //< �ӽ� : 5�� ������ => 3�� �����̷� ����
        REBIRTH_STEEL_MODE_TIME             = 1000*5,       // ��Ȱ�� 5�ʰ� ��ö���..
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        DOMINATION_REBIRTH_STEEL_MODE_TIME  = 0,     // ������ ��Ȱ�� 0�ʰ� ��ö��� 
#else
        DOMINATION_REBIRTH_STEEL_MODE_TIME  = 1000*2,       // ������ ��Ȱ�� 2�ʰ� ��ö���  
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
        PLAYER_SKILL_COOLTIME_INIT_NUM      = 30,
        LOADING_STEEL_MODE_TIME             = 1000*15,      // �ε��� 15�ʰ� ��ö���
        DOMINATION_LOADING_STEEL_MODE_TIME  = 1000*7,       // ������ �ε��� 7�ʰ� ��ö���   
    };

    //----------------------------------------------------------------------------------------------
    //
//_NA_004035_20120227_COSTUME_ITEM
public:
    // �ڽ�Ƭ ó����
    class CostumeHandler
    {
        friend class Player;
    private:
        explicit CostumeHandler(Player* const owner) :
            owner_(owner),
            is_applyed_option_(false)
#ifdef _RU_006860_20130806_COSTUME_PCBANG_EFFECT
            ,is_applyed_pcbang_option_(false)
#endif //_RU_006860_20130806_COSTUME_PCBANG_EFFECT
        {            
        }
        ~CostumeHandler()
        {
        }
    
    public:
        // �ڽ�Ƭ �������� ����Ѵ�.
        bool Use(const SLOTCODE item_code, const POSTYPE item_pos);
        bool Equipped(SCItemSlot& item_slot, const bool is_equipped);
        
        // ĳ���� ���� ������ �ڽ�Ƭ ������ �����Ѵ�.
        void SetCostumeInfo();
        
        // �ɼ� ���� ���� �÷���
        inline bool GetIsApplyedOption() { return is_applyed_option_; }
#ifdef _RU_006860_20130806_COSTUME_PCBANG_EFFECT
        // �ǽù�ɼ��� �������� �������� Ȯ���Ѵ�.
        inline bool GetIsApplyedPCBANGOption() { return is_applyed_pcbang_option_;}
#endif //_RU_006860_20130806_COSTUME_PCBANG_EFFECT
    
    private:
        CostumeHandler(const CostumeHandler&);
        void operator=(const CostumeHandler&);

    private:
        void ApplyOption(SCItemSlot& item_slot, const bool is_add);
        
    private:
        Player* owner_;
        bool is_applyed_option_;
#ifdef _RU_006860_20130806_COSTUME_PCBANG_EFFECT
        bool is_applyed_pcbang_option_;
#endif //_RU_006860_20130806_COSTUME_PCBANG_EFFECT
    };

public:
    Player();
    virtual ~Player();

    virtual BOOL Init();
    virtual void Release();
    void Create(ServerSession* pServerSession, DWORD UserKey,
                const TCHAR* pszID, const CHAR* pszIP, time_t LoginTime_t);
    //void Destroy();
    // CHANGES: f101115.3L, make an independent method
    // for stable player closing process using shared data.
    void CloseSharedTransactions();

    //virtual BOOL Update(DWORD dwDeltaTick);
    virtual BOOL Update(const util::ServerTimeInfoPerFrame* time_info_per_frame, BOOL bSectorUpdate);
    virtual void Reuse() { Release(); }

    // GM����
    // BOOL IsSuperPlayer();
    BOOL IsDBCharState(eDBCHAR_STATE stat); // (UNUSED)
    void SetDBCharState(eDBCHAR_STATE stat, WORD wMinutes = 0); //< wMinutes ���� �ð�

    // �ֽ� ���� ������
    //virtual BASE_PLAYERINFO* GetCharInfo();
    // returns : encoded size
    int GetPlayerRenderInfo(BOOLEAN is_player_render, PLAYER_RENDER_INFO_EX* OUT pack_info);
    //----------------------------------------------------------------------------------------------
    // below interfaces is extracted from the SCPlayer
    ePLAYER_STATE GetState() const;
    void SetState(ePLAYER_STATE player_state);

    const BASE_PLAYERINFO* GetCharInfo() const;
    BASE_PLAYERINFO* GetCharInfo();
    void OnSetCharInfo();
    void SetCharInfo(BASE_PLAYERINFO* char_info);
    // removed an unused method because of the kind of a weapon can acquire from an equip container
    //void SetWeaponKind(DWORD weapon_kind) { weapon_kind; }
    DWORD GetWeaponKind() const;
    // Item Cool Timer
    void SetItemCoolTime(BOOL bPoolUse, DWORD cool_time_margin = 0);
    void FreeItemCoolTime(BOOL bPoolUse);
    util::Timer* GetItemCoolTimer(WORD wType); //{ return m_pItemCoolTimeHash->GetData(wType); };
    void InitializeItemCooltime();
    //
    //----------------------------------------------------------------------------------------------
    void SetBehave(ePLAYER_BEHAVE_STATE state);
    ePLAYER_BEHAVE_STATE GetBehave() const;

    void ResumeTimer();
    void SuspendTimer();
    //----------------------------------------------------------------------------------------------
    // �÷��̾� Title
    void AddPlayerTitle(ePLAYER_TITLE title);
    void RemovePlayerTitle(ePLAYER_TITLE title);
    DWORD GetPlayerTitle() const;

    //----------------------------------------------------------------------------------------------
    // ���弳�� �� ����Ű
    void SetStaticWorld(CWorldBase* pWorld);
    void SetUserKey(DWORD dwUserKey);
    DWORD GetUserKey() const;
    const char* GetUserID() const;
    void SetUserID(const char* pszID);
    const char* GetUserIP() const;
    void SetUserIP(const char* pszIP);

    inline void SetLoginTime_t(time_t time);
    inline time_t GetLoginTime_t() const;
    inline void SetDebugInfoFlag(BYTE byFlag);
    inline BYTE GetDebugInfoFlag() const;

    //----------------------------------------------------------------------------------------------
    // ��Ŷ Send �� ���� ����
    virtual BOOL SendPacket(MSG_BASE_FORWARD* pMsg, WORD wSize, BOOL bCompress = FALSE);
    virtual BOOL SendExPacket(DWORD dwNumberOfMessages, BYTE** pMsg, WORD* pwSize);
    virtual void SendPacketAround(MSG_BASE_FORWARD* pMsg, WORD wSize, BOOL bSendToMe = TRUE, GUILDGUID OtherGuildGuid = INVALID_GUILDGUID);
    virtual void SendExPacketAround(DWORD dwNumberOfMessages, BYTE** pMsg, WORD* pwSize, BOOL bSendToMe = TRUE);

    template< typename _MSGSUBTYPE >
    inline void SendPacketForSync(const _MSGSUBTYPE* const pMsgSub);
private:
    void SendPacketForSync_(const BYTE _TAG, WAR_TAG_V* const IN pMsg, const WORD wSize);
public:
    BOOL SendToGameDBPServer(MSG_BASE_FORWARD* pMsg, WORD wSize);
    BOOL SendToGuildServer(MSG_BASE_FORWARD* pMsg, WORD wSize);
    void SetServerSession(ServerSession* pSession);
    ServerSession* GetServerSession() const;
    void ForceDisconnect(BYTE reason);

    //----------------------------------------------------------------------------------------------
    // ��������
    // CHANGES, f100830.4L, replace an arguement block for a skill to a SkillInfo
    RC::eBATTLE_RESULT CanNormalAttack(SkillInfo* preparing_skill_info_data);
    RC::eBATTLE_RESULT CanStyleAttack(SkillInfo* preparing_skill_info_data);
    virtual BOOL CanAttack();
    virtual BOOL CanBeAttacked() const;
    void SetAttackDelay(BYTE byAttackSequence, SLOTCODE StyleCode);
    virtual BOOL OnResurrection(float fRecoverExp, float fRecoverHP, float fRecoverMP, Player *pHealer=NULL);
    virtual void OnDead(); // �׾��� ��
    virtual void OnAttack(Character *pTarget, SLOTCODE skillcode, DAMAGETYPE damage); // ������ ��

protected:
    virtual void _Damaged(DamageArgs* damage_arg);
#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
    virtual VOID _WriteMondschienLog(Character* attacker, SLOTCODE skill_code, DAMAGETYPE damage_value);
    VOID _WriteMondschienLog_StatusList(Character* user);
    BOOL _IsLogTargetUser(Character* user);
    BOOL _IsLogTargetGuildUser(GUILDGUID guild_guid);
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG

#ifdef _NA_008706_20160229_TEXTLOG_PK
    virtual VOID _WriteBattleMonitorLog(Character* attacker, SLOTCODE skill_code, DAMAGETYPE damage_value);
#endif //_NA_008706_20160229_TEXTLOG_PK

public:

    virtual void ApplyPenalty();
    void SetLastAttackSeq(BYTE byAttackSequence);
    BYTE GetLastAttackSeq() const;

    virtual eUSER_RELATION_TYPE IsFriend(Character* pTarget);
    MovementChecker* GetMovementChecker();
    nsSync::CSSyncTuner* GetSyncTuner();

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    VOID OnRecoverFP(PLAYERFP recoverFP);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    virtual	VOID OnRecover( INT recoverHP, INT recoverMP, INT recoverSD = 0, eRECOVER_TYPE recoverType = eRECOVER_TYPE_NORMAL, Character* pAttacker = NULL );

    virtual void ForcedAttack(Character *pTarget);
    virtual BOOL CanResurrection(Character *pTarget);

    //----------------------------------------------------------------------------------------------
    // ����
    virtual void SetActionDelay(DWORD delay);
    DWORD GetActionDelay();
    BOOL IsActionExpired();
    BOOLEAN IsDoingAction() const;

    //----------------------------------------------------------------------------------------------
    // ��ġ ��ȭ
    virtual BOOL ExecuteThrust(BOOL bForcedMove, WzVector *pwvMoveDistance, WzVector &wvPosAfterThrust, float fMoveDistance, BOOL bDownAfterThrust=FALSE);
    RC::ePORTAL_RESULT Portal(ePORTAL_TYPE ePortalType, FIELDCODE ToFieldCode, WzVector& vDestPos, POSTYPE atItemPos);
    RC::ePORTAL_RESULT CanPortal();
    BOOL ReCallPlayer(char const character_name[MAX_CHARNAME_LENGTH], const KEYTYPE room_key, FIELDCODE field_code, WzVector dest_pos, 
                      ePORTAL_TYPE portal_type, POSTYPE item_pos, SLOTCODE item_code);
    //----------------------------------------------------------------------------------------------
    // ������ ����
    RC::eITEM_RESULT CanUseItem(const BASE_ITEMINFO* pItemInfo); // (CHANGES) const
    void ResetUseItem(WORD wCoolTimeType);
    ItemManager* GetItemManager() const;
    QuickManager* GetQuickManager() const;
    BOOL CanUsePotion();
    void UseHPPotion(int recoverHP, int iApplicationTime, int iPeriodicTime);
    void UseMPPotion(int recoverMP, int iApplicationTime, int iPeriodicTime);

    void StatPointUpItemUse();
    SkillSlotManager* GetSkillSlotManager() const;
    // _NA002217_100728_EP2_PERK_SYSTEM_
    nsSkill::PerkManager* GetPerkManager() const;

    // �Ķ����(���� ���� �� ���)
#if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
    bool IsRenderEquipItem(POSTYPE pos) const;
#endif
    POSTYPE SerializeRenderEquipItemBaseInfo(EQUIP_ITEM_BASEINFO::data_type* const OUT result_slot_list) const;
    void SerializeSkillTotalInfo(SKILL_TOTAL_INFO& rSkillInfo, eSERIALIZE eType );
    BOOL SerializeInfoToDBProxy(eGUILDWAREHOUSE_UPDATE_FACTOR UpdateFactor = UPDATE_FACTOR_COMMON);

    void CopyToCharInfo(PLAYERINFO_FOR_PACKET& rCharInfoOut);
    const BASE_ITEMINFO* GetEquipItemInfo(eEQUIPCONTAINER_POS pos) const;
    SCItemSlot*  GetEquipItemSlot(eEQUIPCONTAINER_POS pos);
    //_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
    RC::eCHANGE_CHARACTER_APPEARANCE_RESULT ChangeCharacterAppearance(
        CharacterAppearanceInfo& INOUT character_appearance); // ĳ���� �ܸ� �����Ѵ�.

    //==================================================================================================
    // �Ķ����
    virtual Attributes& GetAttr();
    virtual const Attributes& GetAttr() const;
    // CHANGES: f110217.2L, added
    PlayerAttributes& GetPlayerAttr();
    const PlayerAttributes& GetPlayerAttr() const;

    Player* GetPlayerOwner();

    eCHAR_TYPE GetCharType() const; // extracted from the SCPlayer
    LEVELTYPE GetLevel() const;     // extracted from the SCPlayer
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE GetGender() const;
#endif

    // TODO : �ϱ� inline virtual method���� �������踦 �����ؼ� �� ��������.
    // Character�� ���� ����� ���뵵 �ְ� �׷��� �ʴ� �͵鵵 �ִ�.
    virtual float GetNormalRangeBonus() const;
    virtual float GetSkillRangeBonus() const;
    virtual float GetSightRange() const;
    virtual eARMOR_TYPE GetArmorType() const;
    virtual eMELEE_TYPE GetMeleeType() const;
    //virtual DWORD GetWeaponKind() const;
    virtual eATTACK_TYPE GetWeaponBaseAttType() const;
    virtual eATTACK_TYPE GetWeaponMagicalAttType() const;
    virtual float GetAttackRange() const;

    virtual EXPTYPE GetExp() const;
    EXPTYPE GetNextExp() const;
    EXPTYPE GetAccumExp(LEVELTYPE LV);
    virtual DWORD GetHP() const;
    virtual DWORD GetMP() const;
    virtual DWORD GetSD() const;
    virtual void SetSD(DWORD dwSD);
    virtual float GetSDRatio() const; 
    virtual DWORD GetMaxSD() const;
    virtual int GetRegenSD() const;
    virtual float GetHPRatio() const; 
    virtual float GetMPRatio() const; 
    virtual DWORD GetMaxHP() const;
    virtual DWORD GetMaxMP() const;
    virtual const TCHAR* GetCharName() const;
    virtual int GetRegenHP() const;
    virtual int GetRegenMP() const;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    PLAYERFP GetMaxFP() const;
    PLAYERFP GetFP() const;
    virtual BOOL ChkNeedFP(PLAYERFP fp_spend);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    DWORD GetRemainStatPoint() const;
    DWORD GetRemainSkillPoint() const;
    MONEY GetMoney() const;

    virtual int GetMPSpendIncValue() const;
    virtual float GetMPSpendIncRatio() const;

    float GetBonusMoneyRatio() const;
    float GetBonusExpRatio() const;
    float GetReflectDamageRatio() const;

    // ����ó���� ���ؼ� : ĳ���Ͱ� ���� �⺻ ��ġ�� �ʿ�
    short GetLimitStrength() const;
    short GetLimitDexterity() const;
    short GetLimitVitality() const;
    short GetLimitSpirit() const;
    short GetLimitInteligence() const;
    short GetLimitExperty1() const;
    short GetLimitExperty2() const;

    // ���� ���� ��ġ ����
    short GetDecreaseLimitStat() const;

    virtual short GetStrength() const;
    virtual short GetDexterity() const;
    virtual short GetVitality() const;
    virtual short GetSpirit() const;
    virtual short GetInteligence() const;
    virtual DWORD GetPhysicalAvoidValue() const;
    virtual float GetPhysicalAttackSpeed() const;
    virtual short GetExperty1() const;
    virtual short GetExperty2() const;

    SLOTCODE GetBaseSelectedStyle() const;
    SLOTCODE GetSelectedStyle(); // ������ ����� ��Ÿ��
    SLOTCODE SynchronizeStyleBasedOnWeapon(); // CHANGES: f110704.3L

    virtual int GetMoveSpeedRatio() const;
    virtual int GetAttSpeedRatio() const;

    virtual void SetAttr(eATTR_TYPE eAttrType, eATTR_KIND eAttrKind, int iValue);
    virtual void SetHP(DWORD dwHP);
    virtual void SetMP(DWORD dwMP);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    void SetFP(PLAYERFP fp);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    //_NA_001231_20081216_ADD_OPTION_KIND
    virtual int GetResistBadStatusRatio(WORD wStateID = eCHAR_STATE_INVALID);

    virtual EXPTYPE AddExp(EXPTYPE exp, DWORD dwTargetObjKey=0, int iBonusRatio=0, BOOL bSendPacket=TRUE, eGAME_CONTENT_TYPE systemType = GAME_CONTENT_NORMAL);

    virtual void UpdateCalcRecover(BOOL bHPUpdate, BOOL bMPUpdate, BOOL bSDUpdate);
    //
    enum 
    { 
        eMONEY_GENERNAL, 
        eMONEY_CIRCULATING, 
        eMONEY_GROUP,//�׷� ���� �����ϰ�� �߰�
    };
    BOOL CanPlusMoney(MONEY plus_money);
    BOOL CanMinusMoney(MONEY minus_money);
    BOOL PlusMoney(MONEY plus_money, int type = eMONEY_GENERNAL);
    BOOL MinusMoney(MONEY minus_money, int type = eMONEY_GENERNAL);
    // implemented by _NA_002399_20110323_MONEY_DISTRIBUTE_METHOD_CHANGE
    BOOL PickMoney(MONEY nMoney, DWORD dwMonsterKey, Player* pOwner);
private:
    void SetMoney(MONEY money);

public:
    void SetBaseSelectedStyle(WORD wStyleCode);
    // (REMOVE) (UNUSED)
    //inline void SetVkrReloadCount(int iCount) { m_byVkrReloadCount = (BYTE)(max(0, iCount)); }
    //inline BYTE GetVkrReloadCount() { return m_byVkrReloadCount; }
    virtual DWORD IncreaseMP(DWORD dwIncrement);
    virtual DWORD DecreaseMP(DWORD dwDecrement);
    virtual DWORD IncreaseHP(DWORD dwIncrement);
    virtual DWORD DecreaseHP(DWORD dwDecrement, int fLimitHP = 0);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    PLAYERFP IncreaseFP(PLAYERFP valueFP);
    PLAYERFP DecreaseFP(PLAYERFP valueFP);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    void SaveStartLocation();
    //WzVector GetStartVector(GameField* pField);

    // �Ķ����(��ȭ)
    BOOL CanSelectStat(eATTR_TYPE attrType) const;
    DWORD SelectStat(eATTR_TYPE attrType, int iIncValue=1);

#ifdef _NA_0_20110216_STATUS_STAT_POINT_SIMULATION    
    // ��,��ø,ü��,����,���ŷ�
    RC::eSTAT_POINT_RESULT ApplyStat(ushort* INOUT stat_point_array, uint array_count);
#endif

    // �Ķ����(GM ��ɾ� ó��)
    void LevelUp(DWORD dwPlusLevel);
    void LevelDown(int nDownLevel);
    void StatPointUp(int iBonusStat);
    int GetStatPoint();
    void SkillPointUp(int iBonusSkill);
    int GetRemainSkillPoint();
    // �߰����� �̵��ӵ�
    void SetAddMoveSpeedRatio(float fRatio);
    float GetAddMoveSpeedRatio() const;

    // ��ų ����
    BOOL CheckRequireLv(SkillScriptInfo *pBaseSkillInfo);
    bool CheckWeaponDefine(const SkillScriptInfo* const skill_script_info) const;
    BOOL CheckRequireSkillStat(SkillScriptInfo *pBaseSkillInfo);
    BOOL CheckNullSkillSlot(SCSkillSlot* pCurSkillSlot, SLOTCODE SkillCode);
    RC::eSKILL_RESULT UseSkill(SkillInfo* pSInfo, BYTE byAttackPropensity);
    BOOL CorrectPosition(WzVector& wvClientCurPos, WzVector& vResultCurPos);

    virtual RC::eSKILL_RESULT CanUseSkillItem(SLOTCODE SkillCode, DWORD dwTargetKey = 0, BOOL bCoolTimerReset = TRUE);
    RC::eSKILL_RESULT CanUseSummonSkill(SLOTCODE SkillCode, DWORD dwTargetKey = 0, BOOL bCoolTimerReset = TRUE);

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    RC::eSKILL_RESULT CanUseSkill(const SkillInfo* skill_info, BOOL bCoolTimerReset = TRUE);

    void ProcessRisingForce();
#else
    RC::eSKILL_RESULT CanUseSkill(SLOTCODE SkillCode, DWORD dwTargetKey = 0, BOOL bCoolTimerReset = TRUE);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
private:
    RC::eSKILL_RESULT CanUseSkilltoTarget(SkillScriptInfo *pBaseSkillInfo, DWORD TargetKey);
public:
    // CHANGES, f100824.7L, change an interface specification which is not inherited from a Character
    bool CanApplyPassiveSkill(SLOTCODE skill_code);
    SLOTCODE GetDefaultStyle(const CODETYPE weapon_code) const;
    SLOTCODE GetPunchStyle() const;
    RC::eSKILL_RESULT CheckSkillCondition(const SLOTCODE skill_code) const;
    virtual BOOL CheckClassDefine(SLOTCODE SkillCode, BOOL bUseActiveSkill);

    // �̼� ����Ʈ ����
    MissionManager* GetMissionManager() const;
    QuestManager_Concrete* GetQuestManager() const;
    IQuickStyleManager* GetStyleManager() const;
	AchievementManager* GetACManager() const;
	

    //----------------------------------------------------------------------------------------------
    // �ŷ� 
    void SetTradeContainer(TradeSlotContainer* pContainer);
    TradeSlotContainer* GetTradeContainer() const;
    //----------------------------------------------------------------------------------------------
    // ���� ���� 
    void SetVendorContainer(VendorSlotContainer* pContainer);
    VendorSlotContainer* GetVendorContainer() const;
    void GetVendorRenderInfo(VENDOR_RENDER_INFO* OUT pRenderInfo);

    void SetVendorPage(VendorPage* pVendorPage);
    VendorPage* GetVendorPage() const;

    //----------------------------------------------------------------------------------------------
    // â��
    CWarehouseSlotContainer* GetWarehouseContainer() const;
    CGuildWarehouseSlotContainer* GetGuildWarehouseContainer();

    //----------------------------------------------------------------------------------------------
    // �̺�Ʈ �κ��丮
    EventInventorySlotContainer* GetEventSlotContainer() const;
    //----------------------------------------------------------------------------------------------
    // PVP
    PVPInfo& GetPVPInfo() const;
    CompetitionInfo& GetCompetitionInfo() const;
    ChaoState& GetChaoState() const;
    BOOL IsExpiredPlayer() const;
    //ģ�� �ʴ� �̺�Ʈ, _KR_0_20091021_FIRENDINVITATION_EXTEND
    InvitateFriendInfo& GetInvitateFriend() const;
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    AccumulatePoint* GetAccumulatePoint() const;
#endif

#ifdef _NA_20100307_BETAKEY_SYSTEM
    BetaKeyInfo* const GetBetaKeyInfo();
    void SetBetaKeyInfo(BETAKEY betakey);
#endif

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    HonorManager* GetHonorManager() const;
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    PointWalletManager* GetPointWalletManager() const
    {
        return point_wallet_manager_;
    };
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

    //_NA_004035_20120227_COSTUME_ITEM
    Player::CostumeHandler& GetCostumeHandler()
    {
        return costume_handler_;
    }
    inline bool GetIsEquippedCostume() const    
    {
        return char_info_.is_equipped_costume_;
    }
    inline SLOTCODE GetCostumeItemCode() const
    {
        return char_info_.costume_item_code_;
    }
    inline POSTYPE GetCostumeItemPos() const
    {
        return char_info_.costume_item_pos_;
    }

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    inline bool GetIsEquippedDecoCostume() const    
    {
        return char_info_.is_equipped_decorate_costume_;
    }
    inline SLOTCODE GetDecoCostumeItemCode() const
    {
        return char_info_.decorate_costume_item_pos_;
    }
    inline POSTYPE GetDecoCostumeItemPos() const
    {
        return char_info_.decorate_costume_item_pos_;
    } 
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

    //----------------------------------------------------------------------------------------------
    // Battleground
    bool IsThereBattleGroundZone() const;     // �÷��̾ ���� �ִ� Zone�� ��Ʋ�׶���(����)���� üũ (true/false)
    ChaosZoneType GetChaosZoneType() const;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    inline BattleGroundPlayerInfo& GetBattleGroundPlayerInfo() const {
        return *battle_ground_player_info_;
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    inline GoldRushPlayerInfo& GetGoldRushPlayerInfo() const {
        return *goldrush_playerinfo_;
    }
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    CharacterInfoExManager& GetExtraInfoManager() { 
        return extra_info_manager_; 
    };
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    inline PlayerAssistMeter* GetPlayerAssistMeter()
    {
        return player_assist_meter;
    }
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_007086_20140318_MONSTERBOOK
    MonsterBook& GetMonsterBook() {
        return monster_book_;
    }
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    inline PlayerSpaInfo* GetPlayerSpaInfo()
    {
        return player_spa_info_;
    }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    //----------------------------------------------------------------------------------------------
    // �÷��̾ ���� ī�������� �����Ѵٸ�, ���� ��ġ�ϰ��ִ� Zone�� ����
    // ���� ī���� ���� �ƴ϶��, NULL ����
    ChaosZone* const GetCurrentChaosZone() const;

    RandomManager* const GetRandomManager() const {
        return random_manager_;
    };

    // PC�� �������� üũ(GMüũ �߰�-����â)
    void SetReservedValue(DWORD dwReservedValue);
    DWORD GetReservedValue() const;
    BOOL CanApplyPCBangBenefit(BOOL bNationCheck);

    void SetSts(BYTE bySts);
    inline BYTE GetSts() const;
    inline BOOL IsGMUser() const;

    void SetPCRoomSts(BYTE bySts);
    BYTE GetPCRoomSts() const;

    CHARGUID GetCharGuid() const;
    USERGUID GetUserGuid() const;
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN GetGameChuUsn() const;
#endif
    DWORD GetCharNameHash() const; // CHANGES: f110517.3L
    GUILDGUID GetGuildGuid() const;
#ifdef _NA_008334_20150608_SONNENSCHEIN
    GameGuild* GetGuild() const;
#endif //_NA_008334_20150608_SONNENSCHEIN
    const char* GetGuildName() const;

    BOOL IsGuildWarehouseRight() const;
    void SetGuildWarehouseRight(BOOL IsRight);
    BOOL GetGuildRenderInfo(GUILD_RENDER_INFO* OUT pRenderInfo, GUILDGUID other_guildIndex = 0) const;

    // Ŭ���̾�Ʈ ������ ���� �ɼ�
    BYTE GetHelmetOption() const;
    void SetHelmetOption(BYTE byOption /* 1 bit ���*/);
    BYTE GetWingOption() const;
    void SetWingOption(BYTE byOption /* 1 bit ���*/);
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    BYTE GetCostumeOption() const;
    void SetCostumeOption(BYTE byOption /* 1 bit ���*/);
#endif // _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME

    // �ǵ��� ����
    FatigueManager* GetFatigueManager() const;

    //----------------------------------------------------------------------------------------------
    // for GameField/Sector grid
    // �ʵ� ���� �� ������
    virtual void OnEnterField(GameField& IN rField, const WzVector& IN rwzVec, int iAngle=-1);
    virtual void OnLeaveField();

    virtual void OnEnterSector();
    virtual void OnEnterObject(const Sector& IN rSector);
    virtual void OnEnterNeighbor(const Sector& IN rSector);
    virtual void OnLeaveObject(const Sector& IN rSector);
    virtual void OnLeaveNeighbor(const Sector& IN rSector);
    // (CHANGES) (f100614.4L)
    ns_trigger::TriggerPlayerLinkInGameSlots* GetTriggerLinks();
    void _EnterEtcStatus();
    void _LeaveEtcStatus();
    void FirstGuildMemberEnter();
    // �Ķ���� ����
    void SetExp(EXPTYPE exp);
    void SetLevel(LEVELTYPE level);
    void SetNextExp();
    void FixLevel();
    void OnLevelUp();
    void OnLevelDown();

    void OnAddExp(DWORD dwTargetObjKey, int iBonusRatio, BOOL bSendPacket, eGAME_CONTENT_TYPE systemType = GAME_CONTENT_NORMAL);
    void NotifyGroupMoneyToPlayer(MONEY amountMoney);

    IPlayerEventD& PlayerEvent();
    sTransaction& EventTransaction(); // �̺�Ʈ Ʈ�����

    // �������� ��ġ ����.
    void SetPreVillageMapCode(MAPCODE mapCode);
    MAPCODE GetPreVillageMapCode();

    //_NA_0_20100722_AUTO_SYSTEM
    VOID SetlastReturnScroll();
    RC::ePORTAL_RESULT MovelastReturnScroll();

    // ���۽� ��Ÿ�� �� ����.
    void SetStartMapCode(MAPCODE mapCode);
    MAPCODE GetStartMapCode();

    util::SimpleTimer& GetSnapShotLogTimer();
    util::SimpleTimer& GetFatigueLogTimer();
    util::SimpleTimer& GetLoadingCheckTimer();

    STATE_GM_INFO& GetGMStateInfo();
    void SerialzieStatusGMInfo(STATE_GM_INFO& INOUT rStateGMInfo, eSERIALIZE se);
    void ApplyGMState();

    ItemOwnFlag& GetItemOwnFlag();

    WarehouseInventoryProtector& GetWarehouseInventoryProtector();

    // (ADD)(20100318)(hukim)(STYLE) ���� ���� ��� GetWarehouseInventoryProtector()=xxxx;
    // �̷� �ڵ尡 ������ �ۼ��Ǿ� �ִ� ��� ��ȿ�ϰ� �ȴ�. ���� ȣȯ���� ����
    // ��� ������ �߰��Ѵ�.
    const WarehouseInventoryProtector& GetWarehouseInventoryProtector() const;

    //Stat Init
    void UpdateCharStat();
    BOOL InitStat(BYTE eInitStat, WORD& OUT rDecreaseValue); 
protected:
    BOOL InitStatAKind(BYTE eInitStat, WORD& OUT DecreaseValue);
    BOOL InitStatAll(); 
public:
#ifdef __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP
    BOOL InitAllStatByChangeUp();
#endif
protected:
    void InitBasicStatAll();
    BOOL CanInitBasicStatAll();
    void InitExpertyStatAll();
    BOOL CanInitExpertyStatAll();
    void InitPassiveStatAll();
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    void InitSkillStatAll();
#endif
public:

    BOOL CheckValidStatPoint();
    void RecoveryStatPoint();
    BOOL GetCheckedStat() const;
private:
    BOOL m_bCheckedStat;
public:
    void SetFallDead(WzVector BeforeDeadPos);
    RC::eSTATUS_RESULT RebirthFromFallDead();
    //----------------------------------------------------------------------------------------------
protected:
    void OnSendAllPlayersRenderInfoAllKinds(const Sector& IN rSector);
    //----------------------------------------------------------------------------------------------
protected:
    void OnSendAllMonsterInfo(const Sector& IN rSector);
    void OnSendAllFieldItemInfo(const Sector& IN rSector);
    //__NA001283_20090225_COLLECTION_USE
    void OnSendAllFieldCollectionInfo(const Sector& IN rSector);
public:
    void SendAllFieldCollectionInfo();
protected:
//{__NA_0_20090720_GUILD_RENDER_INFO_MODIFY
public:
    void OnSendAllPlayersGuildRenderInfo(const Sector& IN rSector);
    void OnSendAllPlayersGuildRenderInfoToNeighbor(const Sector& IN rSector);
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
public:
    void OnSendAllPlayersSunRankingGradeInfo(const Sector& IN rSector);
    void OnSendAllPlayersSunRankingGradeInfoToNeighbor(const Sector& IN rSector);
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
protected:
//}
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
public:
    void SetPlayTime(WORD wPlayTime);
    WORD GetPlayTime() const;
    void SetOfflineTime(WORD wOfflineTime);
    WORD GetOfflineTime() const;
    void SetAge(BYTE byAge);
    BYTE GetAge() const;
    void SetRealNameAuth(BYTE byRealNameAuth);
    BYTE GetRealNameAuth() const;
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE

public:
    enum eVERIFY_MAGICCODE { eVERIFY_MAGICCODE_INIT = 0xAABBCCDD, eVERIFY_MAGICCODE_RELEASE = 0xDEADBEEF, };
    static Player* VerifyNRestoreTHIS(Player* pPlayer);

    //----------------------------------------------------------------------
    // ���� ����
    //----------------------------------------------------------------------
    BOOL SetInterceptionOption(BYTE byType, BOOL bFlag);
    BOOL GetInterceptionOption(BYTE byType);
    BOOL& UnexpectedUserEnterRoom();
    //__NA_S00015_20080828_NEW_HOPE_SHADOW
    void InsertSummonInfo(PLAYER_SUMMON_INFO* pInfo);
    void DestroySummonInfo();
    void CallSummonMonster();
    //end of shadow
    //{__NA001336_090514_PARTY_NAVI_SUPPORT__
    inline void NotifyChangedStatus(PlayerStatEvent::eFlag flag);
    PlayerStatEvent::eFlag  CheckChangedStatus(PlayerStatEvent::eFlag skip_flag);
    void SendChangedStatus();

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    VOID RemoveDragonTransformStatus();
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    //}
    // __KR_001355_20090608_GROUPINGSYSTEM_DATA = {
    void SetGroupHighMemberInfo(eGROUP_STAT eHighMember );
    void SetGroupLowMemberInfo(eGROUP_STAT eLowMember );
    eGROUP_STAT GetGroupHighMemberInfo() const;
    eGROUP_STAT GetGroupLowMemberInfo() const;
    void GroupBonusAcceptForMoney(MONEY bonusMoney, MONEY addRatioMoney, eHEIM_ACQUISITION_TYPE eType);
    void GroupBonusAcceptForEXP(EXPTYPE bonusEXP, EXPTYPE addRatioEXP, eEXP_ACQUISITION_TYPE eType);
    // end of '__KR_001355_20090608_GROUPINGSYSTEM_DATA'

#if defined (_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC) || \
    defined (_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION)
    PostManager* GetPostManager() const;
#endif

    void Player_TransOff(); //_NA001385_20090924_DOMINATION_SHADOW ���̵� ����, �Լ��� ����

    // (CHANGES) (f100324.3L)
    const nsSlot::InventoryConfig& inventory_config() const;
    // update all inventory insert rule configures
    // returns : if the result is succeeds, the requester has reflected inventory configurations
    bool UpdateInventoryConfig(const nsSlot::InventoryConfig& configure);
    // this has a different presentation from a complex riding casting
    bool SetupEtherealizer(nsSlot::EtheriaImpl* result_instance);
    // CHANGES: f110103.3L
    ns_object::PlayerZoneTransaction* GetZoneTransactionInfo();

#pragma warning(push)
#pragma warning(disable : 4201)
    //==============================================================================================
    //
public:
    enum
    {
        SENDINFO_ENTER_OBJECT           = 0,

        SENDINFO_PLAYER_RENDER          = 0,
        SENDINFO_PLAYER_EQUIP           = 1,
        SENDINFO_PLAYER_GUILDRENERINFO  = 2,
        SENDINFO_PLAYER_SPECIAL_RENDER  = 3,

        SENDINFO_ITEM_RENDER            = 0,

        SENDINFO_NPCS_MONSTER           = 0,
        SENDINFO_NPCS_SUMMONED          = 1,
        SENDINFO_NPCS_MAPNPC            = 2,
        SENDINFO_NPCS_TOTEMNPC          = 3,
        SENDINFO_NPCS_MERCHANTNPC       = 4,    //_NA_0_20100222_UNIT_TRIGGERS_REGEN

        SENDINFO_GUILD_RENDER           = 0,

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        SENDINFO_SUN_RANKING_GRADE      = 0,
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

        SENDINFO_MAX_BUFFER_SIZE        = 5
    };
    static VarMsg2 m_VarMsg2[SENDINFO_MAX_BUFFER_SIZE];

    //==============================================================================================
    //  <FIELD SECTION>
    //
    //  -   �÷��̾��� �޸𸮰� �̻��� �������� üũ�ϱ� ���� �ڵ�
private:
    int m_StartCheckCode;
    int m_StartInitCode;

private:
    //==============================================================================================
    //  Player Game Data
    struct {
		AchievementManager*   m_pACManager;
        QuestManager_Concrete* m_pQuestManager;
        MissionManager*        m_pMissionManager;
        SlotManager*           m_pSlotManager;
        ItemManager*           m_pItemManager;
        QuickManager*          m_pQuickManager;
        //QuickStyleManager*     m_pStyleManager;
        FatigueManager*        m_pFatigueManager;
        SkillSlotManager *     m_pSkillSlotManager;
        nsSkill::PerkManager*  perk_manager_; // _NA002217_100728_EP2_PERK_SYSTEM_
        //----------------------------------------------------------------------
        // �ŷ� 
        //----------------------------------------------------------------------
        TradeSlotContainer*    m_pTradeSlotContainer;
        //----------------------------------------------------------------------
        // ���� ���� 
        //----------------------------------------------------------------------
        VendorSlotContainer*   m_pVendorSlotContainer;
        VendorPage*            m_pVendorPage;

        //----------------------------------------------------------------------
        // �̺�Ʈ �κ��丮
        //----------------------------------------------------------------------
        EventInventorySlotContainer* m_pEventSlotContainer;
        //----------------------------------------------------------------------
        // PVP
        //----------------------------------------------------------------------
        PVPInfo*         m_pPVPInfo;
        CompetitionInfo* m_pCompetitionInfo;
        // ī������ ����
        ChaoState*  m_pChaoState;
        //ģ�� �ʴ� ��� ����, _KR_0_20091021_FIRENDINVITATION_EXTEND
        InvitateFriendInfo* m_pInvitateFriend;
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
        AccumulatePoint* m_pAccumulatePoint;
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
        BetaKeyInfo betakey_info_;
#endif

        //�ý��� ���� ������
#if defined (_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC) || \
    defined (_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION)
        PostManager* m_PostManager;
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        HonorManager* honor_manager_;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
        PointWalletManager* point_wallet_manager_;
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

        //_NA_004035_20120227_COSTUME_ITEM
        CostumeHandler costume_handler_;

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        BattleGroundPlayerInfo* battle_ground_player_info_;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        GoldRushPlayerInfo* goldrush_playerinfo_;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
        CharacterInfoExManager extra_info_manager_;
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        PlayerAssistMeter* player_assist_meter;
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        RandomManager* random_manager_;
#ifdef _NA_007086_20140318_MONSTERBOOK
        MonsterBook monster_book_;
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        PlayerSpaInfo* player_spa_info_;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    };

public:
    __BOOL_PROPERTY(ForceAttack);
public:
    //_NA_008706_20160229_TEXTLOG_PK
    void               KillPlayer(Player* pDeadPlayer, BOOL bSummoned = FALSE, DAMAGETYPE damage_value = 0);
    RC::eBATTLE_RESULT ChkAttackPvP(Character* pTargetChar);
    RC::eSKILL_RESULT  ChkSkillPvP(Character* pTargetChar);
    BOOL               IsPvPAtt(WORD PTATroubld);

private:
    util::Timer m_DBPUpdateTimer; ///< DBProxy�� �����͸� �ֱ������� Update�� �ϱ����� Ÿ�̸�
    BYTE  m_byLastAttackSeq;
    MovementChecker* m_pMovementChecker;

public:
#ifdef _NA_008012_20150130_RANKING_SYSTEM
    __PTR_PROPERTY(RankingSystemCtl, RankingSystemCtl);
    __ARRAY_PROPERTY(int, (MAX_MISSION_NUM+1), BestMissionTime); // �̼� Ÿ�� ��� ����
#endif // _NA_008012_20150130_RANKING_SYSTEM
    __PTR_PROPERTY(WayPoint, WayPoint); // Waypoint
    __PTR_PROPERTY(CharWayPoint, CharWayPoint); // CharWayPoint
    // for debugging
private:
    struct
    {
        DWORD m_dwLastSendTick;
        DWORD m_dwSentSize;
        BYTE  m_byDebugInfoFlag; // ��������� �÷���(������� ���� Ŭ���̾�Ʈ�� ��������� ���ۿ���)
    };

    //////////////////////////////////////////////////////////////////////////
    // Player Session Info
private:
    struct {
        ServerSession* m_pServerSession;
        TCHAR  m_pszUserID[ID_MAX_LEN+1];
        CHAR   m_pszUserIP[IP_MAX_LEN+1];
        DWORD  m_dwUserKey;
        DWORD  char_name_hash_; // CHANGES: f110517.3L
        time_t m_LoginTime_t;
        DWORD  m_dwReservedValue; // ���� (0:�Ϲݻ����, ���������� PC�� ������ UID)
        BYTE   m_bySts; // DB�� ����Ǵ� ���� ����(100�̻��� GM���)
        BOOL   m_bGMUser; // GM ����(GM ���� ����� m_bySts ����-100����)
        BYTE   m_PCRoomSts; // PC�� �������
    };


private:
    EXPTYPE m_NextExp; // ���� ������ ���۰���ġ : ������ üũ������ ����Ѵ�.

    BOOL m_bExpiredPlayer; //< Release �Ǿ����� ���� (�ı���,Ǯ����ȯ��) - not used
    struct {    // Action Delay Control fields for C/S Synchronize
        util::Timer m_ActionTimer; // ��ų�̳� �Ϲ� ������ ��Ÿ�� ���� Ÿ�̸�
        BOOLEAN     m_bDoingAction; // �׼������� (��ų �׼�)
    };

    ePLAYER_BEHAVE_STATE m_BehaveState;
    ePLAYER_STATE player_state_; // f100827.4L
    DWORD   m_PlayerTitle;
    float   m_fAddMoveSpeedRatio; // �߰����� �̵��ӵ� ��ȭ.. 1�̸� �����Ȳ ����...
    MAPCODE m_PreVillageMapCode; // ������ �� �ڵ�, ���� ������ ������ ���� ���� �ڵ尡 ��ϵȴ�.
    // f100827.4L, a detach inheritance relation from a SCPlayer
    ns_object::PlayerAttributeLinker attributes_linker_;
    BASE_PLAYERINFO char_info_; // f100827.4L

    //_NA_0_20100722_AUTO_SYSTEM
    MAPCODE  m_lastReturnScrollMapCode; // ������ ��ȯ ��ũ�� ��� ��.
    WzVector m_lastReturnScrollPos;     // ������ ��ȯ ��ũ�� ��� ��ġ.

    util::SimpleTimer m_ShapShotLogTimer; // ������ �α� Ÿ�̸�.

    util::SimpleTimer m_FatigueLogTimer; // �Ƿε� �α� Ÿ�̸�.
    util::SimpleTimer m_LoadingCheckTimer; // Ŭ���̾�Ʈ�� �ʵ��� �ε��ð��� üũ�Ѵ�.(�� �ð��� ������������ ª���� ���¡���� ����)
    //  Attributes
private:
    PlayerAttrForUpdate m_Attr;

    // ���
    PLAYERKEY m_InvitationHostKey;

    // ��Ű�� �������� ����ȸ��(�ִ�20��)
    BYTE m_byVkrReloadCount;

    IPlayerEventD m_PlayerEvent;
    STATE_GM_INFO m_GMStateInfo; // GM ����(����, ����, �ӵ�)
    ItemOwnFlag   m_ItemOwnFlag;

    WarehouseInventoryProtector m_WarehouseInventoryProtector;
public:
    struct { //���� ����
        BOOL     m_bIsFallDead; 
        WzVector m_dwBeforeFallDeadPos;
    };
private:

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    struct {    //�Ƿε� ���� ����
        WORD m_wPlayTime;
        WORD m_wOfflineTime;
        BYTE m_byAge;
        BYTE m_byRealNameAuth;
    };
#endif

    BOOL  m_bUCUserEnterRoom;
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
public:
    bool GetTotalEquipmentScore(int& out_total_item_score);
    SunRankingPlayerInfo GetSunRankingPlayerInfo();

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

public:
    CPetManager m_PetManager;
    CEtherEmissionDevice m_EtherManager;
    ns_object::SPECIAL_RENDER_EFFECT RENDER_EFFECT; // PROPERTY
private:
    eSPECIAL_RENDER_EFFECT m_RenderEffect;

    // ��� â�� ������ ������ ����ɼ� �����Ƿ�
    // ���� Player�� BaseInfo�� ���� �ʾ���
    BOOL  m_GuildWarehouseRight;
    // _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    // this has a different presentation from a complex riding casting
    nsSlot::EtheriaControlOption etheria_control_option_;
public:
    ISSQHandlerInGame SSQHandler; // PROPERTY
    IPlayerRiderD Rider; // Property : __NA001390_090915_RIDING_SYSTEM__
    IPlayerWingsD Wings; // CHANGES: f110315.2L, property '_NA001955_110210_WING_COSTUME'

    // implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND = {
    //      m_RecallMsgBoxTimeMap,
    //      SetInvitatedFriendRecallRequestTime,
    //      GetInvitatedFriendRecallRequestTime
private:
    //��õģ������ ��ȯ ��û�� �ð��� �����Ѵ�.
    RECALL_MSGBOX_TIME_MAP  m_RecallMsgBoxTimeMap;    
public:
    VOID SetInvitatedFriendRecallRequestTime(DWORD FriendGUID, DWORD64 Recalltime);
    DWORD64 GetInvitatedFriendRecallRequestTime(DWORD FriendGUID);

private:
    PLAYER_SUMMON_INFO_LIST m_listPlayerSummonInfo;
    // (WAVERIX) (NOTE) �̳༮...protected�� �����ϴ�. -.-;;;
    __PTR_PROPERTY( CollectedList, CollectedList);
private:
    PlayerStatEvent::eFlag m_PartyEvent;
    ViewBoxCubic m_LastUpdatedPartyNaviPosBox; // 5x5

    eGROUP_STAT m_eHighMember; //�÷��̾��� �׷� ���� ��� ����
    eGROUP_STAT m_eLowMember; //�÷��̾��� �׷� ���� ��� ����

    //ns_trigger::TriggerPlayerLinkInGame m_TriggerLink; //__NA000000_090613_TRIGGER_LOGIC_REARRANGE__
    ns_trigger::TriggerPlayerLinkInGameSlots trigger_links_;
    ns_object::PlayerZoneTransaction zone_transaction_info_; // CHANGES: f110103.3L
    //
    COOL_TIME_TABLE item_cool_time_table_;
    //
    // <WARNING> �� �ʵ�� �׻� �������� ���� ��.
    // �÷��̾��� �޸𸮰� �̻��� �������� üũ�ϱ� ���� �ڵ�
    int m_EndCheckCode; 
    int m_EndInitCode;

#pragma warning(pop) // 4201
    //
    //
    __DISABLE_COPY(Player);
    friend class ns_object::PlayerAttributeLinker;
}; // 
//
//  <end Player>
//
//==================================================================================================


#define __GAMESERVER_PLAYER_INL
#include "./Player.inl"
#undef __GAMESERVER_PLAYER_INL

#endif //__GAMESERVER_PLAYER_H

