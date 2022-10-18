#ifndef PROGRAMCOMMON_STRUCT_BASE_PLAYER_INFO_H
#define PROGRAMCOMMON_STRUCT_BASE_PLAYER_INFO_H
#pragma once

#pragma pack(push,1)

//==================================================================================================
// (CHANGES) moved from Struct.h to solve dependency problem, need more generic solving method.
//_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_
namespace nsSlot {

struct InventoryConfig
{
    typedef uint16_t SetType;
    static const uint8_t kConfigVersion = 1;
    // all checked state test pseudo code
    //  set[n] |= (any field : eInsertRule)
    //  if (configures[n] == eInsertRule_All)
    //      all checked state -> clear all check box and toggle 'all select' check box
    enum eInsertRule {
        eInsertRule_Equip   = 1 << 0, // equip items
        eInsertRule_Quest   = 1 << 1, // quest items
        eInsertRule_AC      = 1 << 2, // AC items
        eInsertRule_Material= 1 << 3, // composite, enchant materials, etc...
        eInsertRule_Normal  = 1 << 4, // other items = ~(eInsertRule_All & ~eInsertRule_Normal)
        eInsertRule_Waste   = 1 << 5, // waste items = { potion, elixir, etc... }
        eInsertRule_All     = eInsertRule_Equip    | eInsertRule_Quest  | eInsertRule_AC    |
                              eInsertRule_Material | eInsertRule_Normal | eInsertRule_Waste,
    };
    static const int kMaxNumberOfInsertRules = 7;
    static const int kMaxNumberOfConfigurations = 10;
    SetType configures[kMaxNumberOfConfigurations]; // number of inventory tabs
};

//--------------------------------------------------------------------------------------------------
// to support the client toggle switch for UI
struct InventoryConfigSwitcher : InventoryConfig
{
    typedef InventoryConfig base_type;
    enum eSwitchValue {
        eSwitch_Equip   = util::_BIT_SHIFT<base_type::eInsertRule_Equip>::BITSHIFT,     // 0
        eSwitch_Quest   = util::_BIT_SHIFT<base_type::eInsertRule_Quest>::BITSHIFT,     // 1
        eSwitch_AC      = util::_BIT_SHIFT<base_type::eInsertRule_AC>::BITSHIFT,        // 2
        eSwitch_Material= util::_BIT_SHIFT<base_type::eInsertRule_Material>::BITSHIFT,  // 3
        eSwitch_Normal  = util::_BIT_SHIFT<base_type::eInsertRule_Normal>::BITSHIFT,    // 4
        eSwitch_Waste   = util::_BIT_SHIFT<base_type::eInsertRule_Waste>::BITSHIFT,     // 5
        eSwitch_All     = util::bit_count_<base_type::eInsertRule_All>::value,          // 6
        eSwitch_Upperbound
    };
    BOOST_STATIC_ASSERT(eSwitch_All == 6 &&
                        eSwitch_Upperbound == kMaxNumberOfInsertRules);

    struct Switches {
        bool checked[eSwitch_Upperbound];
    };
    struct Tabs {
        bool changed_tabs[kMaxNumberOfConfigurations];
        Switches tabs[kMaxNumberOfConfigurations];
    };

    bool changed() const;
    void Clear();
    void SetConfigures(const InventoryConfig& configure); // set a value from the game server
    void SyncFromConfigure(); // convert to information of tabs from configures of bit flags
    // if the result is succeed, changed the pointed value.
    bool Toggle(int tab_index, eSwitchValue switch_value);
    // should be called after the action for sending to the game server,
    // this action has cleared the changed field
    bool Updated();
    //
    // data fields
    bool changed_;
    Tabs table_;
};

inline bool InventoryConfigSwitcher::changed() const {
    return changed_;
}

inline void InventoryConfigSwitcher::Clear() {
    ZeroMemory(this, sizeof(*this));
}

inline void InventoryConfigSwitcher::SetConfigures(const InventoryConfig& configure)
{
    Clear();
    static_cast<InventoryConfig&>(*this) = configure;
    SyncFromConfigure();
}

// should be called after the action for sending to the game server,
// this action has cleared the changed field
inline bool InventoryConfigSwitcher::Updated()
{
    changed_ = false;
    ZeroMemory(table_.changed_tabs, sizeof(table_.changed_tabs));
}
#ifdef _NA_006372_20130108_BADGE_ITEM
struct TOGGLE_ITEMS
{
    bool     is_equiped_; // Ȱ��ȭ ����
    POSTYPE  item_pos_; // �κ��丮 ��ġ
    SLOTCODE item_code_; // ������ ��ȣ
    ToggleItemType toggle_item_type_;

    TOGGLE_ITEMS()
    {
        clear();
    };

    void clear()
    {
        is_equiped_ = false;
        item_pos_ = INVALID_POSTYPE_VALUE;
        item_code_ = 0;
        toggle_item_type_ = kToggleItemMax;
    };

    ToggleItemType type() { return toggle_item_type_; };
};
#endif //_NA_006372_20130108_BADGE_ITEM
}; //end of namespace

//==================================================================================================

// ���� ���εǴ� ����
// ����� ������ Ÿ�԰� ��ġ��Ų��
struct BASE_PLAYERINFO_LOG
{
	USERGUID	m_UserGuid;
	BYTE		m_bySlot;
	BYTE		m_byClassCode;
	CHARGUID	m_CharGuid;
	char		m_tszCharName[MAX_CHARNAME_LENGTH+1];	//< ���Ӽ������� �����ϰ� ����ϱ� ���� : �Ź� memcpy���ϰ� �ϱ� ����
	BYTE		m_byHeight;
	BYTE		m_byFace;
	BYTE		m_byHair;
	SHORT		m_LV;
	SHORT		m_sStrength;
	SHORT		m_sDexterity;
	SHORT		m_sVitality;
	SHORT		m_sInteligence;
	SHORT		m_sSpirit;
	SHORT		m_sSkillStat1;
	SHORT		m_sSkillStat2;
	UPTYPE		m_UserPoint;
	INT			m_iExpForLog;				// �α� ������ ���� ��¿ �� ���� ����
	INT			m_iMaxHP;
	INT			m_iHP;
	INT			m_iMaxMP;
	INT			m_iMP;
	INT64		m_Money;
	INT			m_iRemainStat;				//<	���� ����
	INT			m_iRemainSkill;
	INT			m_iSelectStyleCode;
	BYTE		m_byPKState;
	BYTE		m_byCharState;
	INT64		m_StateTime;
	INT			m_iRegion;
	SHORT		m_sLocationX;
	SHORT		m_sLocationY;
	SHORT		m_sLocationZ;
	INT			m_iPreRegion;
	char		m_tszTitleID[MAX_TITLEID_LENGTH+1];
	INT64		m_TitleTime;
	INT			m_iInventoryLock;

	// ī��
	BYTE		m_ChaoState;		// ī�� ����(eCHAO_STATE)
	INT64		m_ChaoEndTime;		// ī�� ���� �ð�

	INT			m_PlayLimitedTime;

	// PVP
	INT			m_iPVPPoint;
	INT			m_iPVPScore;
	BYTE		m_byPVPGrade;
	//DWORD		m_dwPVPRanking;
	INT			m_iPVPMaxSeries;
	INT			m_iPVPMaxKill;
	INT			m_iPVPMaxDie;
	INT			m_iPVPTotalKill;
	INT			m_iPVPTotalDie;
	INT			m_iPVPTotalDraw;

    /// [2012-4-20 by i4u4me] �̰� ������ ����. 
    union
    {
	    BYTE		m_byInvisibleOptFlag;					//< eINVISIBLE_OPTION_FLAG ����
        struct
        {
            BYTE m_Helmet   : 1;
            BYTE m_Cape     : 1;
            BYTE m_Wing	    : 1;
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
            BYTE m_Costume  : 1; 
            BYTE m_Temp	    : 4;
#else
            BYTE m_Temp	    : 5;
#endif // _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
            
        }VisibleOptionFlag;
    };

	// ���
	GUILDGUID		m_GuildGuid;
	GUILDMARKIDX	m_GuildMarkIdx;
	char			m_tszGuildName[MAX_GUILDNAME_LENGTH+1];		//< ��� �̸�, NULL����
	BYTE			m_GuildPosition;							//< ��� ������... ==> �������� �Ѵ�...
	char			m_tszGuildNickName[MAX_CHARNAME_LENGTH+1];	//< ��峻 ȣĪ, NULL����

	// �������� ����
	INT			m_iCHuntingDisCount;	//< �������� Dis Ƚ��
	INT			m_iCHuntingVCount;		//< �������� �¸�Ƚ��
	INT			m_iCHuntingLCount;		//< �������� �й�Ƚ��
	INT			m_iCHuntingTotCount;	//< �������� ����Ƚ��

	INT64		m_CreateTime;
	INT			m_Version;					//< T_Character�� ������ ��Ʈ�� ����

	BYTE		m_ExtraInventoryTabCount	: 6;
	BYTE		m_ExtraEquipCashSlotCount	: 2;

	CHAR		m_InventoryPwd[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH];	

	BYTE		m_byFreeInitCount;

	eGUILD_DUTY		m_eGuildDuty;								

	EXPTYPE		m_i64Exp;

//-------------------------------------------V3-------------------------------------------------
	//ToDo: Log�� ���ܾ� �ϴ� ������ ���⿡ �����Ͻÿ�.
	// ��, üũ�� ��Ʋ���� â���̿��� �뺸�Ͻÿ�.
	// ���⿡ ������ ���� ���ÿ�. ���� �ٲ��� ���ÿ�!!!!!!!!!
//-----------------------------------------Function----------------------------------------------
/*
	BOOL PlayerInfoConverter( int iFromVersion IN, BYTE * IN pFromStream IN,
		int & iToVersion OUT, BYTE * pToStream OUT );
*/
};

//==================================================================================================

struct BASE_PLAYERINFO : public BASE_PLAYERINFO_LOG
{
	//Log�� �ȳ��ܵ� �Ǵ� ������ ���⿡ �����Ͻÿ�.	
	BOOL			m_bWhisperFlag;
	BOOL			m_bTradeFlag;

    eGUILD_PENALTY	m_ePenaltyType;
	INT64			m_GuildPenaltyTime;			// ��� ���Ƽ ���� �ð�
	PROPERTY<BYTE>	m_ChangeOfClassStep;	// eCHAREX_TYPE : ������ �ڵ� 0�̸� ���� ����.

	inline DWORD			GetLogSize()
	{ return (DWORD)( sizeof(BASE_PLAYERINFO_LOG) ); }

	// �����ϱ� ���� (=ClassCode), ���� ������ Ŭ���� eCHAREX_TYPE
	inline eCHAREX_TYPE		GetChangeOfClass()	{ return (eCHAREX_TYPE)(BYTE)m_ChangeOfClassStep; }
	inline VOID				SetChangeOfClass( eCHAREX_TYPE changedClass ) { m_ChangeOfClassStep.Set( (BYTE)changedClass ); }

	BYTE		m_bSummonedPet			: 4;
	BYTE		m_bMAXIntimacy			: 4;
	POSTYPE		m_PetItemPos;
	DWORD		m_bMAXOptionRemainTime;
	BYTE		m_bActiveEtherDevice;
	BYTE		m_byEtherBulletPos;
#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
	INT			m_iACVersion;
#endif
    //{__NA_001290_20090525_SHIELD_SYSTEM
	INT			m_iMaxSD;
	INT			m_iSD;
    //}
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    WORD        m_wAccumulateYear;      // �÷��� �ð� ���� �⵵.
    DWORD       m_dwAccumulateTime;     // ���� �ð� (��:0000000��)
    WORD        m_wAccumulatePoint;     // ���� ����Ʈ
#endif
    nsSlot::InventoryConfig m_InventoryConfig;
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN  gamechu_usn_;
#endif
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE gender; // ����
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BETAKEY beta_key_;
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
	WORD		honor_title_;
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA_004035_20120227_COSTUME_ITEM
    bool     is_equipped_costume_; // �ڽ�Ƭ Ȱ��ȭ ����
    POSTYPE  costume_item_pos_; // �ڽ�Ƭ �κ��丮 ��ġ
    SLOTCODE costume_item_code_; // �ڽ�Ƭ ������ ��ȣ

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    bool     is_equipped_decorate_costume_; // ���� �ڽ�Ƭ Ȱ��ȭ ����
    POSTYPE  decorate_costume_item_pos_; // ���� �ڽ�Ƭ �κ��丮 ��ġ
    SLOTCODE decorate_costume_item_code_; // ���� �ڽ�Ƭ ������ ��ȣ
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

#endif // _NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    PLAYERFP max_FP_;
    PLAYERFP cur_FP_;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
};

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
struct ChaosZoneRecord 
{
    MAPCODE map_code; //< �ش� �� �ڵ�
    WORD total_count; //< ����Ƚ��
    WORD win_count; //< �¸�Ƚ��
    WORD draw_count; //< ���º�Ƚ��
    WORD lose_count; //< �й�Ƚ��
    WORD dis_count; //< Dis Ƚ��
    WORD kill_count; //< Kill Ƚ��
    WORD death_count; //< Death Ƚ��
    WORD max_series_kill; //< �ִ� ���� ų��
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    WORD assist_count; // < ��ý�Ʈ�� Ƚ��
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

    ChaosZoneRecord()
    {
        Clear();
    }

    void Clear()
    {
        map_code = 0;
        total_count = 0;
        win_count = 0;
        draw_count = 0;
        lose_count = 0;
        dis_count = 0;
        kill_count = 0;
        death_count = 0;
        max_series_kill = 0;
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        assist_count = 0;
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    }
    const MAPCODE GetMapCode() const { return map_code; }
};

struct BattleGroundRecord : public ChaosZoneRecord
{
    WORD strongpoint_attack;   //< ���� ���� ����
    WORD strongpoint_defense;  //< ���� ���� ���

    BattleGroundRecord()
    {
        Clear();
    }

    void Clear()
    {
        ChaosZoneRecord::Clear();
        strongpoint_attack = 0;
        strongpoint_defense = 0;
    }
};
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
struct GoldRushRecord : public ChaosZoneRecord
{
    WORD gold_point;   //������ �������Ʈ
    GOLDGRADE gold_grade;

    GoldRushRecord() {
        Clear();
    }
    void Clear()
    {
        ChaosZoneRecord::Clear();
        gold_point = 0;
        gold_grade = eGOLDGRADE0;
    }
};
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
//==================================================================================================

#pragma pack(pop)
#endif //PROGRAMCOMMON_STRUCT_BASE_PLAYER_INFO_H