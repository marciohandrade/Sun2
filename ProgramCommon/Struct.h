/** Struct_DB.h : DB�� �Ķ����
	- Number���� ��� unsigned int�� ����
	@note
		- Ȯ��, ������ ������ Ÿ���� ������ �ʿ䰡 �ִ�. ������� ����, �Ҽ��� ����
*/

#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <tchar.h>
#include <vector>

#include <ProgramCommon/WzDataType.h>

#include <BitSetOperation.h>
#include <SafeStringBufferControl.h>
#include <SolarFileReader.h>
#include <ResultCode.h>
#include <Struct_BasePlayerInfo.h>

#pragma pack(push,1)
#pragma warning(push)
#pragma warning(disable : 4201)

union tagWZID
{
	WzID id;
	char code[4];
};

//----------------------------------------------------------------------------------------------------------------------------------------
// ĳ���� ������ �ο��Ǵ� ĳ���� �ʱ��ġ
//----------------------------------------------------------------------------------------------------------------------------------------


struct INIT_CHARINFO_BASE
{
	BYTE		m_ClassCode;
	CHAR		m_pszClassName[MAX_CHARNAME_LENGTH];
	LEVELTYPE	m_LV;
	DWORD		m_dwMaxHP;
	DWORD		m_dwMaxMP;
	EXPTYPE		m_i64Exp;
	WORD		m_dwRemainStat;
	WORD		m_dwRemainSkill;
	MAPCODE		m_dwRegion;
	SHORT		m_sLocationX;
	SHORT		m_sLocationY;
	SHORT		m_sLocationZ;
	SHORT		m_sStrength;
	SHORT		m_sDexterity;
	SHORT		m_sVitality;
	SHORT		m_sInteligence;
	SHORT		m_sSpirit;
	SHORT		m_sSkillStat1;
	SHORT		m_sSkillStat2;
	CODETYPE	m_ClassNameCode;
    DWORD       m_RiderSummonCode;
#ifdef _NA_20100307_BETAKEY_SYSTEM
    INT32       m_MenBeytaKey;
    INT32       m_WomanBeytaKey;
#endif
};

//----------------------------------------------------------------------------------------------------------------------------------------
// ĳ���� ����(class)�� ���� ���� ���
//----------------------------------------------------------------------------------------------------------------------------------------
struct CALC_CHARINFO
{
	CODETYPE		m_ClassCode;
	DAMAGETYPE		m_wMeleeDamage;
	DAMAGETYPE		m_wRangeDamage;
	DAMAGETYPE		m_wMagicDamage;
	BYTE			m_byHPPerStat;
	BYTE			m_byHPPerLevel;
	BYTE			m_byHPRecover;
	BYTE			m_byMPPerStat;
	BYTE			m_byMPPerLevel;
	BYTE			m_byMPRecover;
	BYTE			m_byStatPerLevel;
	BYTE			m_bySkillPerLevel;
	BYTE			m_byPhyDef;
	BYTE			m_byMagicDef;
	BYTE			m_byPhyCritical;
	BYTE			m_byMagicCritical;

	BYTE			m_byPhyAttRate;
	BYTE			m_byPhyAvoidRate;
	BYTE			m_byMagicAttRate;
	BYTE			m_byMagicAvoidRate;

	BYTE			m_byAttSpeed;
	BYTE			m_byMagicSpeed;
	BYTE			m_byMoveSpeed;
	BYTE			m_byLimitCarry;
	BYTE			m_byFastRecovery;


};

struct PLAYERINFO_FOR_PACKET
{
	EXPTYPE		m_i64Exp;
	DWORD		m_dwRemainStat;
	DWORD		m_dwRemainSkill;
	MONEY		m_Money;
	SLOTCODE	m_SelectStyleCode;
	PLAYERHP	m_MaxHP;
	PLAYERHP	m_HP;
	MPTYPE		m_MaxMP;
	MPTYPE		m_MP;
	WORD		m_wMoveSpeedRatio;		//< �̵��ӵ�(100�� ���� %��ġ)
	WORD		m_wAttSpeedRatio;		//< ���ݼӵ�(100�� ���� %��ġ)
	//BYTE		m_byLeftSelect;
	INT64		m_StateTime;
	char		m_tszTitleID[MAX_TITLEID_LENGTH];
	INT64		m_TitleTime;
	enum eINVEN_LOCK_MODE { ENABLE = 0, EMPTY_PWD = 0, EXIST_PWD = 1, DISABLED = 2,
		ENABLEnEMPTY = ENABLE|EMPTY_PWD, ENABLEnEXIST = ENABLE|EXIST_PWD, DISABLEDnEXIST = DISABLED|EXIST_PWD, };
	INT			m_iInventoryLock;	// eINVEN_LOCK_MODE�� BitFlags
	SHORT		m_sStrength;
	SHORT		m_sDexterity;
	SHORT		m_sVitality;
	SHORT		m_sInteligence;
	SHORT		m_sSpirit;
	SHORT		m_sSkillStat1;
	SHORT		m_sSkillStat2;
	UPTYPE		m_UserPoint;

	struct { 	
	WORD		m_byGMGrade		: 3;			//< GM ���( 0 : �Ϲ�, 1�̻� : GM, 4 : ������̵� ) eCHAR_CONDITION
	WORD		m_byPCBangUser	: 1;
	WORD		m_byCondition	: 1;			//< �������(�ɱ�/����)
	WORD		m_byPKState		: 3;			//< 8
	WORD		m_byPCRoomSts	: 1;			//< PC�� �������
	WORD		m_byCharState	: 7;			//<	eDBCHAR_STATE			
	};
	// 2005/12/28(taiyo)
	// ĳ���� ����Ʈ ���� ���� ���� ����϶�.
	//BYTE		m_byHeight;
	//BYTE		m_byFace;
	//BYTE		m_byHair;
	INT			m_PlayLimitedTime;
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


	// ����� �� ������ �߰�
	GUILDGUID	m_GuildGuid;
	BYTE		m_GuildDuty;
	char		m_tszGuildName[MAX_GUILDNAME_LENGTH];

	// ī��
	BYTE		m_ChaoState;		// ī�� ����(eCHAO_STATE)
	BYTE		m_ExtraInventoryTabCount	: 6;
	BYTE		m_ExtraEquipCashSlotCount	: 2;

	BOOL		m_bWhisperFlag;
	BOOL		m_bTradeFlag;

	BYTE		m_byFreeInitCount;

	BYTE		m_bSummonedPet			: 4;
	BYTE		m_bMAXIntimacy			: 4;
	POSTYPE		m_PetItemPos;
	DWORD		m_bMAXOptionRemainTime;
	BYTE		m_bActiveEtherDevice;
	BYTE		m_byEtherBulletPos;
    //
    PLAYERSD    m_MaxSD;
    PLAYERSD    m_SD;	
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    WORD        m_wHonorTitle;
#endif
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    PLAYERFP    max_FP;
    PLAYERFP    cur_FP;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
};

struct SPECIAL_CONDITION
{
	BYTE			byCondition;
	WORD			wConditionParam;
	WORD			wActionType;
	WORD			wActionParam;
	BYTE			byActionRate;			
};

// ��������
struct RESISTANCE_CONDITION
{
	WORD			m_wStateCode;		// �����ڵ�
	BYTE			m_byRatio;			// �������� Ȯ��
	tagWZID			m_wzidEffectCode;	// ����Ʈ �ڵ�
};

//--------------------------------------------------------------------------------------------------
// (CHANGES) change pack info to support busy access structure, \
// target = { BASE_NPCINFO, EXTRA_NPCINFO,
//            ROOT_SKILLINFO, BASE_SKILLINFO, BASE_ABILITYINFO, BASE_STYLEINFO }
#pragma pack(push)
#pragma pack()

// NOTE: f110520.2L, the element weakness of the npc based on the user view
struct NpcElementWeakness
{
    enum {
        eElementWeakness_Fire,
        eElementWeakness_Water,
        eElementWeakness_Wind,
        eElementWeakness_Earth,
        eElementWeakness_Dark,
        eElementWeakness_Default,
        eElementWeakness_Counts = eElementWeakness_Default
    };
    typedef uint8_t weakness_type;
    weakness_type value;
};

struct BASE_NPCINFO
{
	MONSTERCODE		m_MonsterCode;
	char			m_pszName[MAX_MONSTERNAME_LENGTH];
	LEVELTYPE		m_Level;
	LEVELTYPE		m_DisplayLevel;
	BYTE			m_byChangeTargetRatio;			// Ÿ�ٺ��� Ȯ��
	DWORD			m_dwMaxHP;
	DWORD			m_dwMaxMP;
	DWORD			m_dwHP;
    DWORD			m_dwMP;
	DWORD			m_dwMaxSD;
    DWORD			m_dwSD;
	CODETYPE		m_NCode;						// �����ڵ�
#ifdef _DH_NPC_GROUP_NAME
    DWORD group_name; //�׷�����ڵ�
#endif//_DH_NPC_GROUP_NAME
	DWORD			m_dwICode;						// �̹��� �ڵ�
	WORD            m_wNpcImageIndex;               // pc�� ���� �̹��� �ε��� 

	BYTE			m_byGrade;						// ����/�Ϲ�
	float			m_fGradeExpRatio;				// ����ġ ��� ���� ����
	WORD			m_wSize;						// ũ��

	WORD			m_wAICode;						// AI Ÿ��
	BYTE			m_byAttType;					// ����Ÿ��(�ٰŸ�, ���Ÿ�, ��, ��, ...)
	BYTE			m_byMeleeType;					// �ٰŸ� ���� Ÿ��(1:����, 2:Ÿ��, 3:����)

	BYTE			m_byCriticalRatio;				// ũ��Ƽ�� Ȯ��
	DWORD			m_dwMinAttackPower;
	DWORD			m_dwMaxAttackPower;
    DWORD           skill_power; // ��ų ���ݷ� (EP2)
	DWORD			m_dwPhyDef;						// ���� ����
	DWORD			m_dwMagicDef;					// ���� ���� (EP2: unused)
	BYTE			m_byAttitude;					// ���� ����(�İ�, ����, ���Ǽ���)
	BYTE			m_byMoveAttitude;				// �̵� ����(������, ��Ȳ��, ������)
	DWORD			m_dwMoveAreaID;					// �̵� ���� ID
	WORD			m_wClass;						// ���� �迭
    uint16_t element_fire_attack;  // ����(��) ���ݷ� (EP2)
    uint16_t element_water_attack; // ����(��) ���ݷ� (EP2)
    uint16_t element_wind_attack;  // ����(�ٶ�) ���ݷ� (EP2)
    uint16_t element_earth_attack; // ����(����) ���ݷ� (EP2)
    uint16_t element_dark_attack;  // ����(����) ���ݷ� (EP2)
    int16_t element_fire_resist;  // ����(��) ���׷�, EP2: [R] ���ݷ� %������(+/-)
    int16_t element_water_resist; // ����(��) ���׷�, EP2: [R] ���ݷ� %������(+/-)
    int16_t element_wind_resist;  // ����(�ٶ�) ���׷�, EP2: [R] ���ݷ� %������(+/-)
    int16_t element_earth_resist; // ����(����) ���׷�, EP2: [R] ���ݷ� %������(+/-)
    int16_t element_dark_resist;  // ����(����) ���׷�, EP2: [R] ���ݷ� %������(+/-)
    NpcElementWeakness element_weakness; // NOTE: f110520.2L, the element weakness of the npc based on the user view
    //
    BYTE            m_byArmorType; // NOTE: EP2 unused... �Ƹ� Ÿ��(1:Hard, 2:Medium, 3:Soft, 4:Siege, 5:Unarmer)
    uint16_t        m_wPhyAttRate;      // �������� ������, EP2:applied, percentage (0.95 -> 95%)
    uint16_t        m_wPhyAttAvoid;     // �������� ȸ����, EP2:applied, percentage (0.95 -> 95%)
	float			m_fBodyRadius;					// ��ü ������
	float			m_fAttRange;					// ���� �Ÿ�
	float			m_fViewRange;					// �þ� ����
	float			m_fMoveRange;					// �̵� �Ÿ�
	float			m_fWalkSpeed;					// �ȴ� �ӵ�
	float			m_fRunSpeed;					// �޸��� �ӵ�
	WORD			m_wAttSpeed;					// ���� �ӵ�(����, ����)
	WORD			m_wAttSpeed2;
	WORD			m_wProjectileCode;				// ��� �߻�ü�ڵ�
	WORD			m_wProjectileCode2;
	BYTE			m_byJumpRatio;					// ���� Ȯ��
    //_YJ_ADD_DAMAGE_EFFECT_NPCINFO
	WzID			m_wiDamageEffectID;				//NPC�� ������ ������ ����Ʈ�� �ִٸ� ��� ������ �⺻ ������ ����Ʈ ���
	WzID			m_wiCDamageEffectID;			//NPC�� ������ ũ��Ƽ�� ������ ����Ʈ�� �ִٸ� ��� ������ �⺻ ������ ����Ʈ ���
	enum {MAX_SPECIAL_NUM = 2,};
	SPECIAL_CONDITION m_SpecialCondition[MAX_SPECIAL_NUM];			// Ư���ൿ
	enum {MAX_RESISTANCE_NUM = 2,};
	RESISTANCE_CONDITION m_ResistanceCondition[MAX_RESISTANCE_NUM];	// ��������  (m_wSkillUpdateTime�� ���� ���� ���� �ߵ�)
#ifdef _NA_007620_20140915_STATE_RESISTS
    STLX_MAP<WORD, RESISTANCE_CONDITION>* state_resistances_;           // ��������2 (                ���°� �ɸ� �� ���� �ߵ�)
#endif //_NA_007620_20140915_STATE_RESISTS

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    BYTE skill_update_condition;                                    // SkillUpdateTime�� �������� ����
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
	WORD			m_wSkillUpdateTime;								// ��ų������ üũ�ϴ� �ֱ�ð�

	BYTE			m_byReviveCondition;							// ��Ȱ ����
	WORD			m_wReviveConditionParam;						// ��Ȱ ���� �Ķ����
	BYTE			m_byReviveRate;									// ��Ȱ Ȯ��
	SLOTCODE		m_wReviveCode;									// ��Ȱ ��ų�ڵ�
	WORD			m_wReviveDelay;									// ��Ȱ �ִ� �����̽ð�

	BYTE			m_byHealCondition;								// �� ����
	WORD			m_wHealConditionParam;							// �� ���� �Ķ����
	BYTE			m_byHealRate;									// �� Ȯ��
	SLOTCODE		m_wHealCode;									// �� ��ų�ڵ�
	WORD			m_wHealDelay;									// �� �ִ� �����̽ð�

	BYTE			m_bySummonCondition;							// ��ȯ ����
	WORD			m_wSummonConditionParam;						// ��ȯ ���� �Ķ����
	BYTE			m_bySummonRate;									// ��ȯ Ȯ��
	SLOTCODE		m_wSummonCode;									// ��ȯ ��ų�ڵ�
	WORD			m_wSummonDelay;									// ��ȯ �ִ� �����̽ð�
	
	enum {MAX_SKILL_NUM = 10,};
	BYTE			m_bySkillUsePossible;							// ��Ÿ ��ų ��� Ȯ��(�Ϲ� ������ �ƴ�)
	SLOTCODE		m_wSkillCode[MAX_SKILL_NUM];					// ��Ÿ ��ų �ڵ�
	BYTE			m_bySkillRate[MAX_SKILL_NUM];					// ��Ÿ ��ų Ȯ��
	WORD			m_wSkillDelay[MAX_SKILL_NUM];					// ��Ÿ ��ų �ִ� �����̽ð�

	WORD			m_wHateSkill;									// ���� ��ų
	WORD			m_wRevengeSkill;								// ���� ��ų
	BYTE			m_byRevengeRate;								// ���� Ȯ��

	WORD			m_byItemDropRate;								// ������ ����� : ���� �ϸ� 
	
	BOOL			m_bRegion;
	enum {MAX_DROPRATIO_INDEX_NUM = 10,};
	bool			m_bOwnerShipForDropIndex[MAX_DROPRATIO_INDEX_NUM];
	DWORD			m_dwDropIndex[MAX_DROPRATIO_INDEX_NUM];			//< DropRatioInfo�� index
	DWORD			m_dwFieldDropIndex[MAX_DROPRATIO_INDEX_NUM];	//< bRegion�� True�϶� && ���Ͱ� �ʵ����϶�
	DWORD			m_dwBattleDropIndex[MAX_DROPRATIO_INDEX_NUM];	//< bRegion�� True�϶� && ���Ͱ� ��Ʋ���϶�

	enum { 
#ifdef _NA_004509_20120314_QUESTDROPINDEX10
        MAX_QUEST_ITEM_DROP_NUM = 10,
#else
        MAX_QUEST_ITEM_DROP_NUM = 5, 
#endif //_NA_004509_20120314_QUESTDROPINDEX10
    };
	WORD			m_QuestCode[MAX_QUEST_ITEM_DROP_NUM];			//< �������� ����Ʈ
	DWORD			m_dwQuestDropIndex[MAX_QUEST_ITEM_DROP_NUM];	//< DropRatioInfo�� index
	uint8_t			m_wACCode;										//< AC �ڵ�
	BYTE			m_byACReferenceID;								//< AC ����ID
	DWORD			m_dwOptionID[2];								// Ư�� �ɼ�
	DWORD			m_dwOptionParam[2];								// Ư�� �ɼ� �Ķ����
	WORD			m_wDest;
	WORD			m_wAct[5];
	WORD			m_wP[5];

	tagWZID			m_wzidSpawnAniCode;								// ���� �ִϸ��̼� �ڵ�
	DWORD           m_dwSpawnTime;									// �����ð� 0�̸� ���� ���� ���� 

	//_YJ_SPAWN_MONSTER_WITH_EFFECT_001250
	DWORD			m_dwSpawnEffectCode;							// ������ ��µ� ����Ʈ �ڵ�
    //_NA_0_20100514_NPC_DEAD_STATE_TIME
    DWORD			m_dwDeadWaitingTime;							// ��� ���� ���� �ð�(��� �� - ���� ��)
#ifdef _NA_20100307_BETAKEY_SYSTEM
    INT32           m_BeytaKey;
#endif
};


struct EXTRA_NPCINFO
{
	enum eROTATE_TYPE 
	{
		eROTATE_ROTATE = 0,
		eROTATE_FIX,
	};

	enum eDISPLAY_TYPE 
	{
		eDISPLAY_HIDE		= 0x00,
		eDISPLAY_MINIMAP	= 0x01,
		eDISPLAY_FULLMAP	= 0x02,
		eDISPLAY_BOTH		= eDISPLAY_MINIMAP | eDISPLAY_FULLMAP,
	};

	MAPCODE         m_MapCode;
	FIELDID			m_FieldID;
	eNPC_TYPE		m_eNPCTYPE;
	CODETYPE		m_NPCCODE;
	eROTATE_TYPE	m_eRotateType;	// ��ȭ�� ���ƺ����� ����.
	eDISPLAY_TYPE	m_eDisplayType; // �̴ϸ㿡 ǥ�� ����.
	float			m_fNPCPos[3];
	float			m_fNPCDir[3];
	int             m_iMoveType;
	float			m_fMovePos[3];
	float           m_fRange;
	DWORD			m_dwJobStrID;
	int				m_iNPC_IconID;
    DWORD           m_unitID;
};

//--------------------------------------------------------------------------------------------------
// (CHANGES) change method type to const in 'ROOT_SKILLINFO' and 'BASE_SKILLINFO'
struct ROOT_SKILLINFO
{
    ROOT_SKILLINFO() : 
        m_SkillCode(0), m_SkillClassCode(0), m_byType(0)
    {
    }
    enum { SKILL_TYPE, STYLE_TYPE };
    SLOTCODE m_SkillCode;       // ��Ÿ�� Ȥ�� ��ų �ڵ�
    SLOTCODE m_SkillClassCode;  // ��Ÿ�� Ȥ�� ��ų ���� ID
    BYTE     m_byType;          // ��ų Ȥ�� ��Ÿ�� ���� // enum { ABILITY_TYPE, SKILL_TYPE };
    bool IsStyle() const { return m_byType == STYLE_TYPE; }
    bool IsSkill() const { return m_byType == SKILL_TYPE; }
};

//--------------------------------------------------------------------------------------------------
// NOTE: f110309.1L, the IFF_Filter object support target select filtering in ability
// this is implemented by _NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
struct BASE_SKILLINFO;
struct BASE_ABILITYINFO;
//
struct IFF_Filter
{
    IFF_Filter() : 
        value(0), 
        validated(false)
    {
    }

    enum eIFF_Check
    {
        eIFF_None            = 0,
        //
        eIFF_CheckSkip       = 1 << 0,
        //
        eIFF_One             = 1 << 3,
        eIFF_Player          = 1 << 4,
        //
        eIFF_Corpse          = 1 << 7,
        eIFF_Resurrection    = 1 << 8,
        //
        eIFF_Enemy           = 1 << 11,
        eIFF_Friend          = 1 << 12,
        //
        eIFF_AttackerOnly    = 1 << 18,
        eIFF_MainTargetOnly  = 1 << 19,
        //
    };
#if SUN_CODE_BACKUP
    enum eIFF_Relation
    {
        eIFF_RelationCheckAttackerOnly
            = (1 << SKILL_TARGET_ME)
            | (1 << SKILL_TARGET_REACHABLE_ME),
        eIFF_RelationCheckFriend
            = (1 << SKILL_TARGET_FRIEND)
            | (1 << SKILL_TARGET_REACHABLE_FRIEND),
        eIFF_RelationCheckEnemy
            = (1 << SKILL_TARGET_ENEMY)
            | (1 << SKILL_TARGET_REACHABLE_ENEMY)
            | (1 << SKILL_TARGET_AREA),
        eIFF_RelationCheckFriendCorpse
            = (1 << SKILL_TARGET_FRIEND_CORPSE),
    };
#endif
    enum eIFF_AbilityCondition
    {
        eIFF_AbilityConditionActorOnly
            = (1 << SKILL_ABILITY_ME),

        eIFF_AbilityConditionMainTargetOnly
            = (1 << SKILL_ABILITY_FRIEND)
            | (1 << SKILL_ABILITY_ENEMY),

        eIFF_AbilityConditionCorpseAbilities
            = (1 << SKILL_ABILITY_CORPSE_RESURRECTION)
            | (1 << SKILL_ABILITY_CORPSE_FRIEND)
            | (1 << SKILL_ABILITY_MYAREA_CORPSE_ENEMY)
            | (1 << SKILL_ABILITY_CORPSE_ENEMY),

        eIFF_AbilityConditionEnabledResurrection
            = (1 << SKILL_ABILITY_CORPSE_RESURRECTION),

        eIFF_AbilityConditionPlayer
            = (1 << SKILL_ABILITY_PLAYER_ENEMY),

        eIFF_AbilityConditionSkipAbilities
            = (1 << SKILL_ABILITY_FIELD)
            | (1 << SKILL_ABILITY_SUMMON)
            | (1 << SKILL_ABILITY_SUMMONED_MONSTER),

        eIFF_AbilityConditionFriendAbilities
            = (1 << SKILL_ABILITY_FRIEND)
            | (1 << SKILL_ABILITY_TARGETAREA_FRIEND)
            | (1 << SKILL_ABILITY_MYAREA_FRIEND)
            | (1 << SKILL_ABILITY_CORPSE_FRIEND),

        eIFF_AbilityConditionEnemyAbilities
            = (1 << SKILL_ABILITY_ENEMY)
            | (1 << SKILL_ABILITY_TARGETAREA_ENEMY)
            | (1 << SKILL_ABILITY_MYAREA_ENEMY)
            | (1 << SKILL_ABILITY_MYAREA_CORPSE_ENEMY)
            | (1 << SKILL_ABILITY_CORPSE_ENEMY),
    };

    typedef ulong value_type;
    //
    //static bool ConfigureIFF_Check(IFF_Filter* result_iff, const eSKILL_TARGET_TYPE target_type);
    static bool ConfigureIFF_Check(IFF_Filter* result_iff, const BASE_ABILITYINFO& ability_info);
    //
    value_type value;
    bool validated;
};

//--------------------------------------------------------------------------------------------------

struct BASE_SKILLINFO : public ROOT_SKILLINFO
{
    BASE_SKILLINFO()
    {
        Clear();
    }

    char      m_szSkillName[MAX_SKILLNAME_LENGTH]; // ��ų �̸�
    CODETYPE  m_dwSkillNcode;         // ���� �ڵ�
    CODETYPE  m_dwSkillDesccode;      // ��ų ���� �ڵ�
    CODETYPE  m_dwSkillIconcode;      // ��ų ������ �ڵ�
#ifdef __NA001126_NEW_SKILL_SCREEN_EFFECT
    BYTE      m_bySwayType;           // ��鸲 ȿ��
    DWORD     m_dwSwayTime;           // ��鸲 �ֱ�Ÿ��
    BYTE      m_bySwayNum;            // ��鸲 Ƚ��
    DWORD     m_dwSwaySize;           // ��鸲 ũ��
#endif //__NA001126_NEW_SKILL_SCREEN_EFFECT(Ŭ�󿡼�������)
    char      m_szSkillMasterEffectName[MAX_SKILLNAME_LENGTH];
    int       m_iWeaponDefine[4];     // ���� ����
    tagWZID   m_wzidAnimationCode[4]; // �ִϸ��̼� �ڵ�
    WORD      m_CSSyncDelay;          // ��ų ���� ��� �ִϸ��̼��� �Ϸ�Ǵ� �ð� (�ִϸ��̼� ó���� ����/Ŭ�� ��ġ ����ȭ ������ ���� ��Ʈ�� ��ġ
    tagWZID   m_wzidCastAniCode;      // ĳ��Ʈ �ִϸ��̼� �ڵ�
    CODETYPE  m_dwFlyingObjectCode;   // �߻�ü �ڵ�
    WORD      m_wFlyingLifeTime;      // �߻�ü ������ Ÿ��
    tagWZID   m_wzidFieldEffectCode;  // �ʵ�����Ʈ �ڵ�

    CODETYPE  m_dwItemCode;           // ������ �ڵ� 

    WORD      m_wSkillAttribute;      // ��ų �迭
    LEVELTYPE m_wRequireLv;           // �䱸 ����
    LEVELTYPE m_wSkillLv;             // �ش� ����
    LEVELTYPE m_wMaxLv;               // �ְ� ����
    LEVELTYPE m_wOverLv;              // ���� ���� ����
    WORD      m_wOverStatclass[2];    // �������� ����
    WORD      m_wOverstat[2];         // ���� ���� �䱸ġ/��ų �䱸 ����
    BYTE      m_bySkillType;          // ��ų Ÿ��
    BYTE      m_byEtherBulletInfo;    // ���׸� �Ѿ� ����
    BYTE      m_bySkillUserType;      // ��ų����� Ÿ��
    CODETYPE  m_dwClassDefine;        // Ŭ���� ����
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    CODETYPE  m_UpperSkillClass;       // ��ų�ý��� ���Խ� ���õ� �����. ��ų �������� �������� ���� ���� ��ų Ŭ������ ������ ��ü.
#endif
    // _NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
    BYTE      m_RequireUpperSkillLevel;
    BYTE      m_bySkillStatType;      // ���õ� ����
    WORD      m_wRequireSkillStat[2]; // �䱸 ���õ�
    BYTE      m_byRequireSkillPoint;  // �䱸 ��ų����Ʈ
    //IFF_Filter iff_filter; // CHANGES: f110309.1L, �Ǿ� �ĺ� �� ��� ���� ���� ���� (ref:m_byTarget)
    BYTE      m_byTarget;             // Ÿ�� ���� ���
    BYTE      m_byForbiddenTarget;    // ��ų��� ���� ���
    WORD      m_wHPSpend;             // HP �Ҹ�
    WORD      m_wMPSpend;             // MP �Ҹ�
    WORD      m_wSkillCasting;        // ��ų ĳ���� Ÿ��
    DWORD     m_dwCoolTime;           // ��Ÿ��
    WORD      m_wSkillRange;          // ��ų ��Ÿ�
    BYTE      m_byAttRangeform;       // ��ų ��������
    WORD      m_wSkillArea;           // ��ų ����
    WORD      m_wChaseRange;          // ���� ����
    BYTE      m_byMaxTargetNo;        // �ִ� ������
    BYTE      m_bySkillAcquire;       // ��ų ȹ��
#ifdef _YJ_ADD_SKILL_SCRIPT_FIELD_4_AUTO_ATTACK_001212
    BYTE      m_btAutoAttack;   // ��ų����� �ڵ����� ���� ������(0 == �ڵ�������, 1 == �ڵ����ݾ���)
#endif //_YJ_ADD_SKILL_SCRIPT_FIELD_4_AUTO_ATTACK_001212

    bool IsNonStopSkill() const;  // ����Ҷ� ������ �ʴ� ��ų�̴�.
    bool IsSummonSkill() const;   // ��ȯü�� ����ϴ� ��ų�̴�.
    bool IsActionSkill() const;
    bool IsEmoticonSkill() const;
    bool IsRangedSkill() const;
    void Clear() { ZeroMemory(this, sizeof(*this)); }
};

// (CHANGES) (100307) change to inlining function in BASE_SKILLINFO

inline bool BASE_SKILLINFO::IsNonStopSkill() const
{
    return m_wzidAnimationCode[0].id == StrToWzID("NULL");
}

inline bool BASE_SKILLINFO::IsSummonSkill() const
{
    return m_bySkillUserType == eSKILL_USER_SUMMONED;
}

inline bool BASE_SKILLINFO::IsRangedSkill() const
{
    const WORD RANGE_SKILL_MIN_RANGE = 40;
    return m_wSkillRange >= RANGE_SKILL_MIN_RANGE;
}

inline bool BASE_SKILLINFO::IsActionSkill() const
{
    return m_bySkillUserType == eSKILL_USER_ACTION;
}

inline bool BASE_SKILLINFO::IsEmoticonSkill() const
{
    return m_bySkillUserType == eSKILL_USER_EMOTICON;
}

//--------------------------------------------------------------------------------------------------

struct BASE_ABILITYINFO
{
    BASE_ABILITYINFO() : 
        m_byIndex(0), 
        m_wAbilityID(0), 
        m_byRangeType(0), 
        m_wSuccessRate(0), 
        m_iOption1(0), 
        m_iOption2(0), 
        m_wStateID(0)
    {
        ::ZeroMemory(m_iParam, sizeof(m_iParam));
    }

    BYTE m_byIndex;      // �����Ƽ �ε���
    WORD m_wAbilityID;   // �����Ƽ ID
    IFF_Filter iff_filter; // CHANGES: f110309.1L, �Ǿ� �ĺ� �� ��� ���� ���� ���� (ref:m_byRangeType)
    BYTE m_byRangeType;  // ������� Type
    WORD m_wSuccessRate; // ����Ȯ�� (õ����)
    int  m_iOption1;     // ���Ÿ��1
    int  m_iOption2;     // ���Ÿ��2
    WORD m_wStateID;     // ����ID
    enum { MAX_ABILITY_PARAM_NUM = 4 };
    int  m_iParam[MAX_ABILITY_PARAM_NUM]; // ���� �����Ƽ ��ġ��
    
    //----------------------------------------------------------------------------------------------
    // CHANGES: f110214.3L, added debugging facility
    struct DetailLogging;
    //
    eATTR_TYPE GetAttrType() const;
    eATTR_TYPE GetAttrTypeDetail(bool* valid_check) const;
    eATTR_TYPE GetAttrTypeForAttackpower(eATTACK_TYPE attack_type, DetailLogging* logging = NULL) const;
    eATTR_TYPE GetAttrTypeForDefence(eATTACK_TYPE attack_type, DetailLogging* logging = NULL) const;
    eATTR_TYPE GetAttrTypeForStat(eSTAT_TYPE stat_type, DetailLogging* logging = NULL) const;
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    eATTR_TYPE GetAttrTypeForElementalResist(eATTACK_TYPE attack_type, DetailLogging* logging = NULL) const;
#else
    eATTR_TYPE GetAttrTypeForBonusDamage(eARMOR_TYPE armor_type, int value_type_value, DetailLogging* logging = NULL) const;
    eATTR_TYPE GetAttrTypeForBonusDefence(eATTACK_TYPE attack_type, DetailLogging* logging = NULL) const;
#endif
    eATTR_TYPE GetAttrTypeForReduceDamage(eATTACK_TYPE attack_type, DetailLogging* logging = NULL) const;
    eATTR_TYPE GetAttrTypeForAttackRange(eRANGE_TYPE range_type, DetailLogging* logging = NULL) const;
#ifdef _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
    eATTR_TYPE GetAttrTypeForCriticalRatio(eATTR_APPLY_TYPE state_type, DetailLogging* logging = NULL) const;
    eATTR_TYPE GetAttrTypeForCriticalDamage(eATTR_APPLY_TYPE state_type, DetailLogging* logging = NULL) const;
#endif //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
    //_NA001956_110210_PERK_SWITCHABLE_REGION
    eATTR_EXTRA_OPTION GetAttrExtraOption() const;
};

inline eATTR_TYPE BASE_ABILITYINFO::GetAttrType() const {
    return GetAttrTypeDetail(NULL);
}

struct BASE_STYLEINFO : public ROOT_SKILLINFO
{
    BASE_STYLEINFO()
    {
        ::ZeroMemory(this, sizeof(this));
    }

    //CODETYPE m_dwStyleID;         // ��Ÿ�� ID
    //CODETYPE m_dwStyleClassID;    // ��Ÿ�� ���� ID
    char      m_szStyleName[MAX_SKILLNAME_LENGTH]; // ��Ÿ�� �̸�
    CODETYPE  m_dwStyleNCode;       // ���� �ڵ�
    CODETYPE  m_dwStyleDescCode;    // ��Ÿ�� ���� �ڵ�
    WORD      m_wSkillattribute;    // ��Ÿ�� �Ӽ�
    CODETYPE  m_dwStyleImage;       // �̹��� �ڵ�

    LEVELTYPE m_wRequireLV;         // �䱸 ����
    LEVELTYPE m_wStyleLV;           // ��Ÿ�� ����
    LEVELTYPE m_wMaxLV;             // �ְ� ����
    LEVELTYPE m_wOverLV;            // ���� ���� ����

    WORD   m_wOverStatclass[2];     // ���� ���� ����
    WORD   m_wOverstat[2];          // ���� ���� �䱸ġ / ��Ÿ�� �䱸 ����

    BYTE   m_byRequireSkillPoint;   // �䱸 ��ų ����Ʈ
    BYTE   m_byClassDefine;         // Ŭ���� ����
    int    m_iWeaponDefine;         // ���� ����
    BYTE   m_byAttType;             // ���� Ÿ��
    float  m_fAttRange;             // ���� �Ÿ�
    BYTE   m_byMoveAttack;          // �̵����� ����
    BYTE   m_byStyleCheck;          // ��Ÿ�� ����

    tagWZID m_wzidCodeFirst;        // 1Ÿ �ڵ�
    tagWZID m_wzidCodeSecond;       // 2Ÿ �ڵ�
    tagWZID m_wzidCodeThird;        // 3Ÿ �ڵ�
    tagWZID m_wzidCodeBack;         // ���� �ڵ�

    WORD  m_wTimeFirst;             // 1Ÿ �ð�
    WORD  m_wTimeSecond;            // 2Ÿ �ð�
    WORD  m_wTimeThird;             // 3Ÿ �ð�

    int   m_iAddDamage[3];          // �߰� ������
    float m_fDamagePercent[3];      // �߰� ������%

    BYTE  m_byAttRangeform;         // 3Ÿ ���� ����
    float m_fStyleArea;             // 3Ÿ ����(������)
    float m_fThirdDelay;            // 3Ÿ �ߵ� �ð�

    int   m_iAttackRate;            // ���� ���� ����
    int   m_iAvoidRate;             // ��� ���� ����
    int   m_iAttackSpeed;           // ���� �ӵ� ����
    int   m_iBonusDefence;          // ���� ����
    int   m_iMagicDefenceAdd;       // ���� ���� ����( ==> �̰� 0814�� �� �ǵ�? �۾��� ��� �����ΰ�? )
    WORD  m_wCriticalBonus;         // ũ��Ƽ�� ���ʽ�
    float m_fDefenseIgnore;         // ���� ����

    float m_fPierceRate;            // 3Ÿ ���� �߻� Ȯ��
    float m_fPierceRange;           // 3Ÿ ���� �Ÿ�
    float m_fStunRate;              // 3Ÿ ���� �߻� Ȯ��
    WORD  m_wStunTime;              // 3Ÿ ���� �ð�
    float m_fKnockBackRate;         // 3Ÿ ��� �߻� Ȯ��
    float m_fKnockBackRange;        // 3Ÿ ��� �Ÿ�
    float m_fDownRate;              // 3Ÿ �ٿ� �߻� Ȯ��
    float m_fDelayReduce;           // ������ �߻� Ȯ�� ����
    float m_fDelayOccur;            // 3Ÿ ������ �߻� Ȯ��

    WORD  m_wHPAbsorb;              // HP ���
    float m_fHPAbsorbPer;           // HP% ���
    WORD  m_wMPAbsorb;              // MP ���
    float m_fMPAbsorbPer;           // MP% ���
    WORD  m_wMaxTargetNo;
};

#pragma pack(pop)
// (CHANGES) change pack info to support busy access structure, end of pack(8) position
//==================================================================================================
// CHANGES: f110503.7L, change pack info to support busy access structure, \
// target = { sMAPSUBINFO, sMAPINFO, sMAP_ENTERENCE_PORTAL, sFIELDINFO, sWMOINFO, sWUTINFO,
//            BASE_STATEINFO }
#pragma pack(push)
#pragma pack()

// related to MapInfoParser data structures
struct sMAPINFO_BASE;
struct sMAP_ENTERENCE_PORTAL_BASE;
struct sFIELDINFO_BASE;
struct sWMOINFO_BASE;
struct sWUTINFO_BASE;

typedef const sMAPINFO_BASE sMAPINFO;
typedef const sMAP_ENTERENCE_PORTAL_BASE sMAP_ENTERENCE_PORTAL;
typedef const sFIELDINFO_BASE sFIELDINFO;
typedef const sWMOINFO_BASE sWMOINFO;
typedef const sWUTINFO_BASE sWUTINFO;


struct sMAPINFO_BASE
{
    enum
    {
        MAX_FIELD_NUM = 20,
        MAX_MISION_MAP_NUM = 15,
        MAX_HUNTING_MAP_NUM = 15,
        MAX_PVP_MAP_NUM = 10,
        MAX_QUEST_MAP_NUM = 15,
        MAX_CHUNTING_MAP_NUM = 15,
        //
        MAX_MISSION_MAP_NUM_V1_3 = 6,   //World.txt�� bMmap�Ķ���� ����.
        MAX_CHUNTING_MAP_NUM_V1_3 = 6,  //World.txt�� wCmap�Ķ���� ����.
        MAX_PVP_MAP_NUM_V1_3 = 6,       //World.txt�� wPmap�Ķ���� ����.
    };

    MAPCODE     MapCode;
    MAPCODE     MapKind;
    BYTE        byMKind; //< eZONETYPE �� ����
    BYTE        byMType; //< (0/1/2) <__NA001226_INSTANCE_DUNGEON_EXTENTION__�� ���� �δ� ����Ÿ�Ե� �߰� ����
    BYTE        byMinUserNum;
    BYTE        byMaxUserNum;
    LEVELTYPE   minLV;
    LEVELTYPE   maxLV;
    BYTE        byENTCount; // ENT ���� ����
    BYTE        byFCount;
    eZONE_RULESET Ruleset; // reference: eZONE_RULESET
    char        pszDebugName[MAX_MAPNAME_LENGTH];
    DWORD       dwNCode;
    DWORD       dwFNCode;
    DWORD       dwDCode;
    DWORD       dwANCode; //< �׷� ��ǥ �̸�.(Ŭ�󿡼� ���)
    WORD        wTimeLimit; 
    DWORD       wText1; //1.3���� ����
    DWORD       wText2; //1.3���� ����
    DWORD       wText3; //1.3���� ����
    DWORD       wStartAreaID;
    DWORD       wStartAreaID2;
    //_NA_20100307_BETAKEY_SYSTEM
    int         nBetaKey;
    uint8_t     reserved_key_index_; // NOTE: f110520.5L, this support the static field unique index
    BYTE        byClass; //< eCLASS_TYPE Ÿ���� �����ؼ� ä���־��!
    //__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
    WORD        completeQCode; // QCODE
    MAPCODE     completeMCode;
    LEVELTYPE   freePassLV;

    WORD        wContinentCode;
    int ContienetNumber; //_DH_EP2_FULL_MAP
    //__NA_000000_20070528_TAIWAN_GUILD_PVP = {
    SLOTCODE    EntranceItemCode;
    BYTE        EntranceItemCount;
    //};
    //----------------------------------------------------------------------------------------------
    // enterable MapKind list of the map_info
    MAPCODE byMissionMapVec[MAX_MISSION_MAP_NUM_V1_3];
    MAPCODE wPVPMapVec[MAX_PVP_MAP_NUM_V1_3];
    MAPCODE wCHuntingMapVec[MAX_CHUNTING_MAP_NUM_V1_3];
    //----------------------------------------------------------------------------------------------
    MAPCODE     FCode[MAX_FIELD_NUM];
    // f110504.1L, changes size 'MAX_MAPNAME_LENGTH' to 5
    char        FGCode[MAX_FIELD_NUM][sizeof(WzID) + 1];
    FIELDCODE   FWMOCode[MAX_FIELD_NUM];
    FIELDCODE   FWUTCode[MAX_FIELD_NUM];
    FIELDCODE   MMCode[MAX_FIELD_NUM];
    DWORD       dwEnvironmentCode[MAX_FIELD_NUM];
    DWORD       dwImageCode[MAX_FIELD_NUM]; //dImage1~6������ �� ����(����â)
    //----------------------------------------------------------------------------------------------
    bool CheckValidMapKind() const; // CHANGES: f110504.2L, internal check utility
    eZONETYPE GetZoneType4SharedCode() const;
};

inline eZONETYPE sMAPINFO_BASE::GetZoneType4SharedCode() const
{
    eZONETYPE zone_type = eZONETYPE(byMKind);
    if (zone_type == eZONETYPE_TUTORIAL_FIELD || 
        zone_type == eZONETYPE_AIRSHIP_FIELD)
    {
        return eZONETYPE_FIELD;
    }
    return zone_type;
}

//--------------------------------------------------------------------------------------------------

struct sMAP_ENTERENCE_PORTAL_BASE
{
    enum { MAP_T_FIELD = 0, MAP_T_HUNTING, MAP_T_MISSION, MAP_T_UPPERBOUND };
    enum { LEVEL_UPPERBOUND = 256 };
    enum { MISSION_CODE_UPPERBOUND = 256 };
    enum { QUEST_CODE_UPPERBOUND = USHRT_MAX };
    enum { DONTWAST_ITEM = 0, WASTE_ITEM, WASTE_UPPERBOUND };
    enum 
    { 
        MOVE_TYPE_FIELD = 0, // �ʵ� ��Ż �̵�
        MOVE_TYPE_VILLAGE = 1, // ���� NPC �̵�
        MOVE_TYPE_WAYPOINT = 2, // ���� ����Ʈ
        MOVE_TYPE_WARP_MACHINE = 3, // �����̵���ġ
        MOVE_TYPE_UPPER 
    };
    //----------------------------------------------------------------------------------------------
    WORD        wIndex; // �ε��� ��
    BYTE        byMapType; // �� Ÿ��
    BOOL        bMoveType; // �̵�Ÿ��(0 : �ʵ��̵�, 1 : ���� NPC �̵�, 2 : ���� ����Ʈ, 3 : �����̵���ġ)
    MONSTERCODE npc_id_; // ����� �̵� NPC ID
    WORD        wWorldFrom;
    WORD        wFieldFrom; // ���� �ʵ� ��ȣ
    char        sAreaFromID[sizeof(WzID) + 1]; // �����ʵ��� ���� ID
    DWORD       m_dwFromAreaID; // �����ʵ��� ���� ID�� StrToWzID()�� ��ȯ�� ��
    WORD        wWorldTo;
    WORD        wFieldTo; // ������Ʈ Ŭ���� �̵��� �ʵ� ID
    char        sAreaToID[sizeof(WzID) + 1]; // �Ѿ�� �ʵ��� ���� ID
    DWORD       m_dwToAreaID; // �Ѿ�� �ʵ��� ���� ID�� StrToWzID()�� ��ȯ�� ��
    BYTE        byMin_Level;  // �ּ� ���� ���� ��
    BYTE        byMax_Level;  // �ִ뷹�� ���� ��
    BYTE        byMissionCode;// �̼�����
    WORD        wQuestCode;   // ����Ʈ����
    WORD        wItemCode;    // ���Խ� �ʿ� ������
    WORD        wItemNum;     // �ʿ� �������� ����
    BOOL        bWaste_Item;  // �� ���Խ� �ʿ� �������� �Һ񿩺� (0: �Һ� �ȵ�, 1:�Һ� ��)
    DWORD       dwHeim;       // �� �̵��� �ݾ� ��ŭ �Һ�, 0�� ��� ���� �Һ� ����

#ifdef _NA_20100307_BETAKEY_SYSTEM
    INT32       nBeytaKey;
    float       fPayApp;
#endif
};

struct sFIELDINFO_BASE
{
    enum { SERVER_TYPE_NONE = 0, SERVER_TYPE_FIELD = 1, SERVER_TYPE_BATTLE = 2, SERVER_TYPE_BOTH = 3 };
    FIELDCODE   FieldCode;
    BYTE        byServerType;   // 1. �ʵ弭�� 2. ��Ʋ���� 3. �Ѵ� �ε�
    BYTE        bNewPathEngine; // �ű� �о����� ��������
    BYTE        bWarCastle;     // �������� ������
    char        pszPath[MAX_PATH];
};

struct sWMOINFO_BASE
{
    FIELDCODE Code;
    char      pszPath[MAX_PATH];
};

struct sWUTINFO_BASE
{
    FIELDCODE Code;
    char      pszPath[MAX_PATH];
};

#pragma pack(pop)
// (CHANGES) change pack info to support busy access structure, end of pack(8) position
//==================================================================================================
// CHANGES: f110503.7L, change pack info to support busy access structure, \
// target = { BASE_STATEINFO }
#pragma pack(push)
#pragma pack()

struct BASE_STATEINFO
{
	WORD			m_wStateID;								// ���� ID
	char  			m_szStateName[MAX_SKILLNAME_LENGTH];	// ���� �̸�
	CODETYPE  		m_dwNamecode;							// ���� �ڵ�
	CODETYPE	  	m_dwDescCode;							// ��ų���� �ڵ�
	DWORD			m_dwIconCode;							// ������ �ڵ�
#ifdef _100506_LTJ_MOD_STATUS_BUFF_IMAGE_RENDER
    BYTE            m_byIconRow;                            // �ؽ��� ����ġ(1����)
    BYTE            m_byIconColumn;                         // �ؽ��� ����ġ(1����)
#endif //_100506_LTJ_MOD_STATUS_BUFF_IMAGE_RENDER
	BYTE			m_byIconKind;							// ������ ����
	BYTE			m_bDelType;								// �������� ( ������ ���Ƿ� ������ �� �ִ� �����ΰ� )
	BYTE			m_byType;								// ����Ÿ��(�̻�, ��ȭ, ��ȭ, Ư��)
    BYTE            m_bySDApply;                            // SD ���뿩��
    BOOLEAN  m_bRidingApply; // Ż �Ϳ� ���� ����
	tagWZID         m_wzidEffectID;                         // ���� ����Ʈ ID
	BYTE            m_byEffectPos;                          // ����Ʈ ��ġ
	WORD			m_wGroupCode;							// Group �ڵ�(���� �迭�� ����Group)

    //!_NA_000587_20100928_DOMINATION_BUFF_ITEM
    WORD m_wBoneIndex;      //! �� ���ؽ� m_byEffectPos == eSKILLEFFECT_POS_BONE_INDEX �϶��� ���
    float m_fOffsetX;       //! �ش���ġ������ �ɼ°�
    float m_fOffsetY;       //! �ش���ġ������ �ɼ°�
    float m_fOffsetZ;       //! �ش���ġ������ �ɼ°�
    bool m_bApplyParentRotate; //!< �θ� ȸ��������
    bool m_bApplyParentScale; //!< �θ� �����ϰ�����
    
};
#pragma pack(pop)
// change pack info to support busy access structure, end of pack(8) position
//==================================================================================================

// �� 4 BYTE
struct MISSIONPART
{
	UINT State	:4;				// �̼� Ŭ���� ���� : eMISSION_STATE -=> ���� ����ϴ� ��Ʈ�� 3bit
	UINT Point	:12;			// ȹ�� ����Ʈ ~512 -=> ���� ����ϴ� ��Ʈ�� 9bit
	UINT Hidden	:16;			// ���� �̼� Ŭ���� ����
};

// ������������������������������������������������������������������������������
// ���� �� �׽�Ʈ �Ҷ� ����ϴ� ����ü ����!!!!!
// ������������������������������������������������������������������������������
struct BASE_ROOMINFO
{
    BYTE m_MinLV;
    BYTE m_MaxLV;
    BYTE m_ClassLimit; // ��Ʈ���� eCLASS_TYPE �����ض�!!!!!!
    BYTE m_SpecialType; // eROOM_SPECIAL_TYPE
    // (CHANGES)(20100318)(hukim)(REQ_0038) can_join_room_ 
    // �濡 ���� ���� ���θ� ǥ���ϴ� ��� �߰�

    bool can_join_room_;

    BASE_ROOMINFO() : m_MinLV(0), 
                      m_MaxLV(0), 
                      m_ClassLimit(0), 
                      m_SpecialType(0), 
                      can_join_room_(false)
    {
    }

    BOOL IsSpecialType(eROOM_SPECIAL_TYPE room_type)
    {
        if (m_SpecialType & room_type)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        } 
    }
};

struct ADDITIONAL_ROOMINFO
{
	BYTE		m_Difficulty	: 4;		//< eHUNTING_MONSTER_DIFFICULT �� ��
	BYTE		m_Bonus			: 4;		//< eHUNTING_BONUS_TYPE �� ��
};

struct ADDITIONAL_PVPINFO
{
	BYTE		m_Rule			: 4;		//< ��(10/30/����) ePVP_RULE_TYPE�� ��
	BYTE		m_Mode			: 4;		//< ���(����/��) ePVP_MODE_TYPE�� ��
};

struct ADDITIONAL_CHUNTINGINFO
{
	BYTE		m_Difficulty	: 4;		//< eHUNTING_MONSTER_DIFFICULT �� ��
	BYTE		m_Bonus			: 4;		//< eHUNTING_BONUS_TYPE �� ��
	BYTE		m_Rule			: 4;		//< �� eCOMPETITION_RULE_TYPE(�����ۿ�, �����Ƽ)
	BYTE		m_Mode			: 4;		//< ��� eCOMPETITION_MODE_TYPE(�����̹�, Ÿ�Ӿ���)
};

struct ADDITIONAL_CHUNTINGINFO_EX : ADDITIONAL_CHUNTINGINFO
{
	KEYTYPE		m_LinkHuntingKey;			//< ��ũ��ų �������ù� Ű(0�̸� ���ʻ���)
};

// CHANGES: f110608.6L
struct PVP_LOBBY_INFO
{
    uint8_t m_Team;  //< ���� �ϰ�� �� ��ȣ ( 1 or 2 )  �������� 0
    uint8_t m_Ready;
    uint16_t m_dwPlayerKey;
};

struct MISSION_ROOMINFO
{
	KEYTYPE				m_Key;
	CODETYPE			m_MapCode;
	char				m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	BYTE				m_byRoomPublic;			//< eZONEPUBLIC
	struct {
		BYTE			m_CurUserNum		: 4;			//< ���� �ο�
		BYTE			m_MaxLimitUserNum	: 4;			//< �ִ� �ο�(NonBlock Slot ����)
	};
	BASE_ROOMINFO		m_RoomInfo;
};

struct HUNTING_ROOMINFO
{
	KEYTYPE				m_Key;
	CODETYPE			m_MapCode;
	char				m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	BYTE				m_byRoomPublic;			//< eZONEPUBLIC
	struct {
		BYTE			m_CurUserNum		: 4;			//< ���� �ο�
		BYTE			m_MaxLimitUserNum	: 4;			//< �ִ� �ο�(NonBlock Slot ����)
	};
	BASE_ROOMINFO		m_RoomInfo;
	ADDITIONAL_ROOMINFO m_AdditionalInfo;
};

struct PVP_ROOMINFO
{
	KEYTYPE				m_Key;
	CODETYPE			m_MapCode;
	char				m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	BYTE				m_byRoomPublic;		//< eZONEPUBLIC
	struct {
		BYTE			m_CurUserNum		: 4;			//< ���� �ο�
		BYTE			m_MaxLimitUserNum	: 4;			//< �ִ� �ο�(NonBlock Slot ����)
	};
	BASE_ROOMINFO		m_RoomInfo;
	ADDITIONAL_PVPINFO	m_AdditionalPVPInfo;
};

struct CHUNTING_ROOMINFO
{
	KEYTYPE				m_Key;
	CODETYPE			m_MapCode;
	char				m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	BYTE				m_byRoomPublic;		//< eZONEPUBLIC
	struct {
		BYTE			m_CurUserNum		: 4;			//< ���� �ο�
		BYTE			m_MaxLimitUserNum	: 4;			//< �ִ� �ο�(NonBlock Slot ����)
	};
	BASE_ROOMINFO		m_RoomInfo;
	ADDITIONAL_CHUNTINGINFO	m_AdditionalInfo;
};

// ������������������������������������������������������������������������������
// ���� �� �׽�Ʈ �Ҷ� ����ϴ� ����ü ��!!!!!
// ������������������������������������������������������������������������������

// ������������������������������������������������������������������������������
// ��� ���� ����ü
// ������������������������������������������������������������������������������
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	struct CHUNTING_PVP_INFO
	{
		INT			m_CHuntingDisCount;
		INT			m_CHuntingVCount;
		INT			m_CHuntingLCount;
		INT			m_CHuntingTotCount;
		INT			m_CHuntingPVPPoint;
		BYTE		m_ChuntingRanking;
	};
#endif

struct GUILD_RELATION
{
	GUILDGUID m_GuildGuid;	
	ePROCLAIM_GUILD m_eProClaim;
	eGUILD_RELATION m_eRelationType;
	TCHAR m_GuildName[MAX_GUILDNAME_LENGTH+1];
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    DWORD64 m_RelationApplyTime;//��� ���� ���� �ð�
    DWORD64 m_RelationChangeTime;//��� ���� ���� ���� �ð�
#endif

	GUILD_RELATION() : m_GuildGuid(INVALID_GUILDGUID), 
		m_eProClaim(NONE_PROCLAIM_GUILD),
		m_eRelationType(eGUILD_RELATION_NONE)
	{
		ZeroMemory(m_GuildName, MAX_GUILDNAME_LENGTH+1); 
	}

}; /*GUILD_RELATION, *LP_GUILD_RELATION;*/

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

struct GUILD_JOIN_REQUEST_INFO
{
    time_t      m_request_time;
    DWORD       m_remain_time;  //���� �ð�(���� : ��)
    GUILDGUID   m_guild_guid;
    CHARGUID    m_char_guid;
    LEVELTYPE   m_char_level;
    BYTE        m_char_class;
    TCHAR       m_char_name[MAX_CHARNAME_LENGTH + 1];

    GUILD_JOIN_REQUEST_INFO()
    {
        m_remain_time   = 0;
        m_guild_guid    = 0;
        m_request_time  = 0;
        m_char_guid     = 0;
        m_char_level    = 0;
        m_char_class    = 0;
        memset(m_char_name, 0, sizeof(m_char_name));
    }
};

#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

struct BASE_GUILD_ADD_INFO
{
    static const size_t kMaxNumberOfRelations = MAX_ALLIENCE_GUILD_NUM +
        MAX_HOSTILITY_GUILD_NUM + MAX_ONE_SIDE_HOSTILITY_NUM;
    //
	GUILDGUID m_GuildGuid;
	BYTE		m_byRelationGuildCnt;
	BYTE		m_byAllienceGuildCnt;	
	BYTE		m_byHostilityGuildCnt;
	BYTE		m_byOneSideGuildCnt;
	GUILD_RELATION m_stRelationGuild[kMaxNumberOfRelations];
	
	BASE_GUILD_ADD_INFO() : m_GuildGuid(0),
		m_byRelationGuildCnt(0),m_byAllienceGuildCnt(0),
		m_byHostilityGuildCnt(0),m_byOneSideGuildCnt(0) {}

	BYTE GetTotalSize() { return m_byRelationGuildCnt; }
	void SetSize(BYTE allience, BYTE aginst, BYTE oneside)
	{
		m_byAllienceGuildCnt = allience;
		m_byHostilityGuildCnt = aginst;
		m_byOneSideGuildCnt = oneside;
		m_byRelationGuildCnt = allience+ aginst+ oneside;
	}
};

struct GUILD_RIGHT_USE
{
	bool			m_bRightUse;				//�ش� ������ �������
};

//��� ��޺� �ش� ���� ��� ����
struct GUILD_GRADE_RIGHT_USE : public GUILD_RIGHT_USE 
{
	BYTE	m_Grade;					//�ش� ������ ����Ϸ��� �����
};

//��� ��å�� ���� ��� ����
struct GUILD_DUTY_RIGHT_USE : public GUILD_RIGHT_USE 
{
	eGUILD_DUTY		m_Duty;						//�ش� ������ ����Ϸ��� �����å
};

//��� ���Ѻ� ���� ��� ����
struct GUILD_RIGHT_USE_INFO : public GUILD_RIGHT_USE 
{
	eGUILD_RIGHTS	m_Right;					//����Ϸ��� ����
};

//��� ��å ��� ����
struct GUILD_DUTY_USE_INFO
{
	eGUILD_DUTY		m_Duty;							//�ش� ��å
	DWORD			m_dwStringCode;					//�����å�� ��Ʈ�� �ڵ�
	bool			m_bUse;							//��� ����
};


struct GUILD_RANK_INFO
{
	//versino 1.0
	BYTE		m_byLevel;							//��巹��
	TCHAR		m_szLevelName[MAX_PATH];			//��巹���� �̸�
	WORD		m_wMaxMember;						//�ش� ������ �ִ� ���� ��
	DWORD		m_dwHeim;							//�ش� ������ ��ũ�� �ϴµ� �ҿ�Ǵ� ����.
	UPTYPE		m_UP;								//�ش� ������ ��ũ�� �ϴµ� �ҿ�Ǵ� UP.
	GPTYPE		m_GP;								//�ش� ������ ��ũ�� �ϴµ� �ҿ�Ǵ� GP.

	//version 1.1
	CODETYPE	m_RankItemCode;						// �ش� ������ ���ھ� �ϴµ� �ʿ��� ������.
};


struct BASE_GUILD_MEMBER_INFO
{
	CHARGUID	 m_CharGuid;
	char m_tszCharName[MAX_CHARNAME_LENGTH+1];
	eGUILD_DUTY m_eGuildDuty;
	BYTE	 m_byCorpsNum;
};

struct BASE_GUILD_MEMBER_TOTAL_INFO
{
	BYTE					m_byMemberCount;
	BASE_GUILD_MEMBER_INFO	m_MemberInfo[MAX_GUILD_MEMBER_NUM];
	int						GetSize()
	{
		return sizeof(BASE_GUILD_MEMBER_TOTAL_INFO)-(MAX_GUILD_MEMBER_NUM-m_byMemberCount)*sizeof(BASE_GUILD_MEMBER_INFO);
	}
};

struct GUILD_MEMBER_INFO : public BASE_GUILD_MEMBER_INFO
{
	BYTE						m_byClass;								// Ŭ���̾�Ʈ���� �ش� ���� ǥ���Ѵ�.
	LEVELTYPE					m_Level;								// Ŭ���̾�Ʈ���� �ش� ���� ǥ���Ѵ�.
	BYTE						m_byChaoState;							// Ŭ���̾�Ʈ���� �ش� ���� ǥ���Ѵ�.
	BOOL						m_bLogin;								// �α��� ���� ( ���弭�������� ���� ���ӵ� ������ �����ϱ� ������ bLogin�� �ʿ� ����. )
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    DWORD                       guild_coin_;                            // ��� ���嵵
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
};

struct SERVER_GUILD_MEMBER_INFO : public GUILD_MEMBER_INFO
{
	GUILD_RIGHT_USE_INFO	m_DelegateRight[MAX_GUILD_DELEGATE_RIGHT];		//���ӵ� ����.
};

struct GUILD_MEMBER_TOTAL_INFO
{
	BYTE				m_byMemberCount;
	GUILD_MEMBER_INFO	m_MemberInfo[MAX_GUILD_MEMBER_NUM];
	int					GetSize()
	{
		return sizeof(GUILD_MEMBER_TOTAL_INFO)-(MAX_GUILD_MEMBER_NUM-m_byMemberCount)*sizeof(GUILD_MEMBER_INFO);
	}
};

struct SERVER_GUILD_MEMBER_TOTAL_INFO
{
	BYTE						m_byMemberCount;
	SERVER_GUILD_MEMBER_INFO	m_MemberInfo[MAX_GUILD_MEMBER_NUM];
	int							GetSize()
	{
		return sizeof(SERVER_GUILD_MEMBER_TOTAL_INFO)-(MAX_GUILD_MEMBER_NUM-m_byMemberCount)*sizeof(SERVER_GUILD_MEMBER_INFO);
	}
};


struct BASE_GUILD_INFO
{
	GUILDGUID			m_GuildGuid;
	BYTE				m_GuildGrade;							//���Ӽ������� ��� ��޺��� ��ũ���� �� üũ �ؾ� �Ѵ�.
	char				m_tszGuildName[MAX_GUILDNAME_LENGTH+1];	//����̸��� BASE�� ���� ������ ����� ���Ӽ������� �α׸� ����� �����̴�.
#ifdef _NA_008334_20150608_SONNENSCHEIN
    BYTE                sonnenschein_phase_;                    //�ҳٻ��� ������(������)
#endif //_NA_008334_20150608_SONNENSCHEIN
};

struct GAME_GUILD_INFO : public BASE_GUILD_INFO
{
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	CHUNTING_PVP_INFO	m_ChuntingPVPInfo;
#endif
	GUILDMARKIDX		m_BackGroundIdx;
	GUILDMARKIDX		m_PatternIdx;
};

struct WORLD_GUILD_INFO : public BASE_GUILD_INFO
{
    TCHAR m_szGuildNotice[MAX_GUILDNOTICE_LENGTH + 1]; // CHANGES: f110915.1L, with null
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	CHUNTING_PVP_INFO	m_ChuntingPVPInfo;
#endif
};

struct GUILD_INFO : public GAME_GUILD_INFO
{
	GPTYPE				m_GuildPoint;
	UPTYPE				m_UserPoint;
	INT64				m_GuildPenaltyTime;	
	INT64				m_GuildDestroyTime;
	eGUILD_DELCHK		m_GuildDestroyWaitFlag;		//0: ����, 1: ����, 2: �������
	INT64				m_nMasterChangeTime;
    WORD                m_DominationMapCount;//�������� ��
    WORD                m_DominationJoinCount;//������ ���� Ƚ��
    WORD                m_DominationDefenseCount;//������ ��� ���� Ƚ��
    WORD                m_DominationHuntingCount;//������ ���� ��� Ƚ��
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    DWORD               m_GuildCoin;//��ι޾� �����ϰ� �ִ� ��� ���� ����
    DWORD               m_GuildGP;
#endif
};

struct GUILD_CORPS_INFO
{
	BYTE m_bCreate;				//���� ������ �����ΰ�?
	BYTE m_byCorpsNum;			//���ܹ�ȣ
	BOOL m_bRearCamp;			//����ķ�� ���� ����.
};

struct GUILD_CORPS_TOTAL_INFO
{
//		BYTE				m_byCorpsCount;
	GUILD_CORPS_INFO	m_CorpsInfo[MAX_GUILD_CORPS_NUM];
};

struct BASE_GUILD_TOTAL_INFO
{
	BASE_GUILD_INFO					m_BaseGuildInfo;
	BASE_GUILD_MEMBER_TOTAL_INFO	m_BaseMemberTotalInfo;
	int					GetSize()
	{
		return sizeof(m_BaseGuildInfo) + m_BaseMemberTotalInfo.GetSize();
	}
};

struct GUILD_TOTAL_INFO
{
	GUILD_INFO					m_GuildInfo;
	GUILD_MEMBER_TOTAL_INFO		m_MemberTotalInfo;
	int					GetSize()
	{
		return sizeof(m_GuildInfo) + m_MemberTotalInfo.GetSize();
	}
};

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
struct GUILD_INFO_EXTRA
{
    WORD ranking_;
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    CHAR introduction_strings_[MAX_GUILD_INTRODUCTION_LENGTH+1];
#endif

    GUILD_INFO_EXTRA()
    {
        ranking_ = 0;
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
        memset(introduction_strings_, 0, sizeof(introduction_strings_));
#endif
    }
};
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
struct GUILD_JOIN_REQUEST_TOTAL_INFO
{
    GUILD_JOIN_REQUEST_INFO     m_GuildJoinRequestInfo[MAX_GUILD_JOIN_REQUEST_NUM];
};

struct GUILD_FACILITY_INFO
{
    FCODE  code;
    FLEVEL level;
    GUILD_FACILITY_INFO()
    {
        code = 0;
        level = 0;
    }
};

struct GUILD_FACILITY_TOTAL_INFO
{
    GUILD_FACILITY_INFO m_GuildFacilityInfo[MAX_GUILD_FACILITY];
    int m_count;
    GUILD_FACILITY_TOTAL_INFO()
    {
        m_count = 0;
    }
};
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

struct PACKET_FOR_GUILD
{
	char		m_tszGuildName[MAX_GUILDNAME_LENGTH+1];
	BYTE		m_GuildGrade;
	//BYTE		m_MarkBinary[MAX_GUILDMARKSTREAM_SIZE];
	INT			m_MasterCharGuid;
	char		m_tszMasterCharName[MAX_CHARNAME_LENGTH+1];
	GPTYPE		m_GuildPoint;
	UPTYPE		m_UserPoint;
	INT			m_RestrictedRight;
	INT64		m_GuildMoney;
	BYTE		m_GuildMemberNum;
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	CHUNTING_PVP_INFO	m_ChuntingPVPInfo;
#endif
	eGUILD_DELCHK	m_eDelChk;
	INT64			m_GuildPenaltyTime;
	GUILDMARKIDX m_BackGroundIndex ;
	GUILDMARKIDX m_PatternIndex;
};

struct PACKET_FOR_GUILD_ADD_INFO
{
	GUILDGUID m_GuildGuid;
	BYTE	 m_byRelationGuildCnt;
	BYTE	 m_byAllienceGuildCnt;	
	BYTE	 m_byAgainstGuildCnt;
	BYTE	 m_byOneSideGuildCnt;
	GUILD_RELATION m_stRelationGuild[MAX_ALLIENCE_GUILD_NUM + 
		MAX_HOSTILITY_GUILD_NUM + 
		MAX_ONE_SIDE_HOSTILITY_NUM];

	void SetSize(int allience, int aginst, int oneside)
	{
		m_byAllienceGuildCnt = (BYTE)allience;
		m_byAgainstGuildCnt = (BYTE)aginst;
		m_byOneSideGuildCnt = (BYTE)oneside;
		m_byRelationGuildCnt = (BYTE)(allience + aginst + oneside);
	}
};

struct PACKET_FOR_GUILD_MEMBER
{
	char		m_tszCharName[MAX_CHARNAME_LENGTH+1];
	BYTE		m_byClass;
	LEVELTYPE	m_LV;
	BYTE		m_byOnline;		// �¶��� ����(0: offline 1: online )
	BYTE		m_byChaoState;	// ī�� ����(eCHAO_STATE)
	eGUILD_DUTY	m_eGuildDuty;
	BYTE		m_byCorpsNum;
};

struct PACKET_FOR_GUILD_RELATION
{
	eGUILD_RELATION m_eRelationType;
	GUILDGUID		m_GuildGuid;
	char			m_GuildName[MAX_GUILDNAME_LENGTH+1];
};

// DB���� �о���� ģ�� ����
struct STRUCT_FRIEND_DB_INFO
{
	enum eNodeType
    {
        UNDEFINED = 0, // default, unused
        INFO_FRIEND = 1,
        INFO_BLOCK = 2,
        INFO_REVENGE = 3,
        INFO_REVENGE_POINT = 4, 
        // expanded by _KR_0_20091021_FIRENDINVITATION_EXTEND = {
		INFO_INVIAT = 5,	//�ʴ� �� ĳ������ ģ�� ����
		INFO_BEINVIATED = 6,//�ʴ� ���� ĳ������ ģ�� ����
							//A �� B �� ģ���ʴ� �ϸ� A:5, B:6�� ���谡 �����ȴ�.
        INFO_BLOCKED_INVIAT = 7,	//���ܵ� �ʴ� �� ĳ������ ģ�� ����(Ŭ������)
		INFO_BLOCKED_BEINVIATED = 8,//���ܵ� �ʴ� ���� ĳ������ ģ�� ����(Ŭ������)
        // }
	};	// Sts ����
	DWORD		dwFriendGuid;
	char		ptszFriendName[MAX_CHARNAME_LENGTH+1];
	BYTE		byClass;		// Ŭ����
	LEVELTYPE	Level;			// ����
	BYTE		Sts;			// ģ�� ���� �ڵ�, STRUCT_FRIEND_DB_INFO::enumeration
	BYTE		RevengePoint;	// ������ ����Ʈ(INFO_REVENGE_POINT�� ���� ��ȿ)
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    BYTE        gender; // ����
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
};

struct COMMUNITY_DB_INFO
{
	enum { INFO_FRIEND = 1, INFO_BLOCK = 2, 
		INFO_REVENGE = 3, INFO_REVENGE_POINT = 4, };	 // friendRelationType

	BYTE	 friendRelationType;
	CHARGUID	 friendGuid;
	BYTE	 friendClass;
	LEVELTYPE friendLevel;
	TCHAR friendName[MAX_CHARNAME_LENGTH+1];
	COMMUNITY_DB_INFO() : friendRelationType(INFO_FRIEND),
		friendGuid(0), friendClass(0), friendLevel(0)
	{
		ZeroMemory(friendName, MAX_CHARNAME_LENGTH+1);
	}
};

//// Ŭ���̾�Ʈ�� �ְ�޴� ģ�� ����
//struct STRUCT_FRIEND_PACKET_INFO
//{
//	BYTE	 byClass;								// Ŭ����
//	LEVELTYPE Level;									// ����
//	BOOL bOnline; // �¶����̸� TRUE, ���������̸� FALSE
//	BYTE	 byChannel;								// ���� �������� ä��(�¶����� ���� ���)
//	char	ptszFriendName[MAX_CHARNAME_LENGTH+1];	// ģ�� �̸�
//};

struct STRUCT_FRIEND_PACKET_INFO
{
	char		ptszFriendName[MAX_CHARNAME_LENGTH+1];	// ģ�� �̸�
	BOOL	    bOnline;								// �¶����̸� TRUE, ���������̸� FALSE
	BYTE		byClass;								// Ŭ����
	LEVELTYPE	Level;									// ����
	BYTE		byChannel;								// ���� �������� ä��(�¶����� ���� ���)
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    BYTE        gender; // ����
#endif  // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
};

// ���弭���� �����ϴ� ģ�� ����
struct STRUCT_FRIEND_INFO
{
	DWORD		dwFriendGuid;
	STRUCT_FRIEND_PACKET_INFO	sPacketInfo;

	STRUCT_FRIEND_PACKET_INFO* GetPacketInfo() { return &sPacketInfo; }
};

// ���弭������ �����ϴ� ���� ����
struct STRUCT_BLOCK_INFO
{
	DWORD		dwBlockGuid;
	char		ptszBlockName[MAX_CHARNAME_LENGTH+1];
};

// DB���� �о���� ���� ����
typedef STRUCT_BLOCK_INFO	STRUCT_BLOCK_DB_INFO;

// Ŭ���̾�Ʈ�� �ְ�޴� ���� ����
struct STRUCT_BLOCK_PACKET_INFO
{
	char		ptszBlockedName[MAX_CHARNAME_LENGTH+1];	// ���ܴ��� ��� �̸�
};

// Ŭ���̾�Ʈ�� �ְ�޴� ������ ����
struct STRUCT_REVENGE_PACKET_INFO
{
	char		ptszRevengeName[MAX_CHARNAME_LENGTH+1];	// �������� �� �̸�
	BOOL		bOnline;								// �¶����̸� TRUE, ���������̸� FALSE
	BYTE		byClass;								// Ŭ����
	LEVELTYPE	Level;									// ����
	BYTE		byChannelID;							// �������� ä��
};

// ���弭������ �����ϴ� ������ ����
struct STRUCT_REVENGE_INFO
{
	DWORD						dwRevengeCharGuid;
	STRUCT_REVENGE_PACKET_INFO	sPacketInfo;
};

// DB���� �о���� ������ ����
typedef STRUCT_REVENGE_INFO	STRUCT_REVENGE_DB_INFO;

struct EVENT_STATUS
{
	CODETYPE	m_dwEventID;		// �̺�Ʈ �ε���
	WORD		m_wRegisterCnt;		// �̺�Ʈ ���� ȸ�� (���� �̺�Ʈ�� ��� ��� ���带 �� ȸ��)
	EVENT_STATUS() : m_dwEventID(0), m_wRegisterCnt(0) {}
};


struct EVENT_STATUS_VER2 : EVENT_STATUS
{
	BYTE		m_DonateHeimRewardCnt;	// ���� ���� ���� ��ο� ���� ����� ȸ��
	BYTE		m_Status;				// 0: (������), 1: (�Ϸ�)
	DWORD		m_CurDonatedHeims;		// ������� ������ ���� �Ѿ� (��� ���尡 ������ �ڵ� Ŭ����)

	EVENT_STATUS_VER2() : EVENT_STATUS(), m_DonateHeimRewardCnt(0), m_Status(0), m_CurDonatedHeims(0) {}
};
typedef struct EVENT_STATUS_VER2		EVENT_STATUS_EX;

//	EventDB�� �ٸ������� ����ϴ� ù ���
//	Event���� ��� ���Ž� event Ÿ�Կ� ���� filtering�� �䱸�ȴ�.
struct SSQ_RESERVED_REWARD_INFO
{	// �� 5���� �ʵ� ��� ����
	enum {};
	//struct {	//EventStatusQuery::sQUERY�� ����
	//	INT		__m_EventID;				// m_SSQRewardType
	//	INT		__m_RegisterCnt;
	//	INT		__m_DonateHeimRewardCnt;
	//	INT		__m_DonatedHeims;
	//	INT		__m_Status;					// bit
	//}			__usable_tab_in_DBtable;
	struct SharedField {
		CODETYPE	EventID;
		BYTE		Status;				// used) __m_Status					- 0: (�̺���), 1: (����ó���Ϸ�) < Ŭ�󿡼��� �̻��
	};
	struct WinnerReward : public SharedField {
		DWORD		Point;				// used) __m_RegisterCnt			- ȹ�� ����Ʈ
		DWORD		Exp;				// used) __m_DonateHeimRewardCnt	- ȹ�� ����ġ
		BYTE		MonsterKillNum;		// used) __m_DonatedHeims:8			- ���� ų ��
		BYTE		PlayerKillNum;		// used) __m_DonatedHeims:8			- �÷��̾� ų ��
		SLOTCODE	ItemCode;			// used) __m_DonatedHeims:16		- ���� ������(�ǵ���) <-
	};
	struct QuestFail : public SharedField {
		DWORD		Point;				// used) __m_RegisterCnt			- ȹ�� ����Ʈ
		DWORD		Exp;				// used) __m_DonateHeimRewardCnt	- ȹ�� ����ġ
		BYTE		MonsterKillNum;		// used) __m_DonatedHeims:8			- ���� ų ��
		BYTE		PlayerKillNum;		// used) __m_DonatedHeims:8			- �÷��̾� ų ��
	};
	struct AdmissionFeeRefund : public SharedField {
		SLOTCODE	ItemCode;			// used) __m_RegisterCnt			- �������� ������
		DWORD		Money;				// used) __m_DonatedHeims			- �������� Heim
	};

	union {
		SharedField			m_SharedInfo;
		WinnerReward		m_WinnerReward;				// ���� ���� ����, DB
		QuestFail			m_QuestFail;				// ����Ʈ ���� (��������)
		AdmissionFeeRefund	m_AdmissionFeeRefund;		// ����� ȯ�� ó��
	};
};


// ���� ä�� ���� ���޿� ����ü
typedef struct
{
	char		m_ptszCharName[MAX_CHARNAME_LENGTH+1];		// �ش� ��Ƽ�� �̸�
	BOOL		m_bVoiceOn;									// TRUE: ���� FALSE: ����
} STRUCT_VOICE_STATUS, *LP_STRUCT_VOICE_STATUS;

// ���� ���� ����ü��
typedef struct
{
    enum eMEMOTYPE {
        NORMAL_MEMO,
        SYSTEM_MEMO
    };
	enum eSYSTEMTYPE 
	{
        NORMAL_TYPE,
        LOTTO_TYPE,
		GROUP_TYPE,
        //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
		FRINVATE_START_TYPE			= SYSMEMO_FRIENDINVITATION_INVATES_TART,
		FRINVATE_END_TYPE			= SYSMEMO_FRIENDINVITATION_INVATE_END,
		FRINVATE_PRESENT_TYPE		= SYSMEMO_FRIENDINVITATION_INVATE_PRESENT,
		FRBEINVATED_START_TYPE		= SYSMEMO_FRIENDINVITATION_BEINVATED_START,
		FRBEINVATED_END_TYPE		= SYSMEMO_FRIENDINVITATION_BEINVATED_END,
		FRBEINVATED_PRESENT_TYPE	= SYSMEMO_FRIENDINVITATION_BEINVATED_PRESENT,
        //}
        DOMINATION_TYPE_RETURN = 9,
        DOMINATION_TYPE_REWARD = 10,
        LOTTO_TYPE_NEW              = SYSMEMO_LOTTO_WIN_NEW,
        DOMINATION_TYPE_RETURN_NEW  = SYSMEMO_DOMINATION_RETURN_MONEY_NEW,
        DOMINATION_TYPE_REWARD_NEW  = SYSMEMO_DOMINATION_REWARD_MONEY_NEW,
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        WORLDRANKING_TYPE_REWARD_ALARAM = SYSMEMO_WORLD_RANKING_GRADE,
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        GUILD_JOIN_REQUEST          = SYSMEMO_GUILD_JOIN_REQUEST,
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        SUN_RANKING_SETTLE_RESULT_TYPE = SYSMEMO_SUN_RANKING_SETTLE_RESULT,
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

    };
    MEMOGUID	m_MemoGuid;
    eMEMOTYPE   m_MemoType;
	char		m_SendName[MAX_CHARNAME_LENGTH+1];
	BYTE		m_byRead;		// �о����� 1, �ƴϸ� 0
	char		m_ptszSubject[MAX_SUBJECT_STRLENGTH];
	char		m_tszCreateDate[21];	// ���� ��¥
} STRUCT_SUBJECT_PACKETINFO, *LP_STRUCT_SUBJECT_PACKETINFO;


class IParser;

struct sSCRIPT_DATA_INFO_NODE
{
	eSCRIPT_CODE		m_ScriptCode;
	IParser*			m_pIParser;
	BOOL			m_bBigData;

	sSCRIPT_DATA_INFO_NODE()
	{
		m_ScriptCode = SCRIPT_EMPTY;
		m_pIParser = NULL;
		m_bBigData = FALSE;
	}
};

struct SCRIPT_DATA : public sSCRIPT_DATA_INFO_NODE
{
    typedef void (*DestroyParserFunction)();

	TCHAR				m_szFileName[MAX_PATH+1];
	TCHAR				m_szVersion[MAX_PATH+1];
	eSEPERATOR_MODE		m_SeperatorMode;
	TCHAR				m_szPackFileName[MAX_PATH+1];
    DestroyParserFunction DestroyParser;

    SCRIPT_DATA() : DestroyParser(NULL)
	{
		m_szFileName[0] = '\0';
		m_szVersion[0] = '\0';
		m_SeperatorMode = SEPERATOR_WHITESPACE;
		m_szPackFileName[0] = '\0';
	}
};

struct st_RANKING_POINT
{
	RANKINGKEY	DBKey;
	USERGUID	UserGUID;
	RANKINGNO	RankingNum;
	DWORD		Point;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
};

struct GAME_OPTION
{
	BYTE		m_byMaxLevel;						// �ִ뷹��
	WORD		m_MaxLevelChangeOfClass1st;			// ���� �� �ִ� ����
	BYTE		m_byNormalShoutLevel;				// �Ϲ� ��ġ�� ���� ����
	BYTE		m_byWorldShoutPCBANG;				// ���� ��ġ�� PC�� ���� ����( 0 : �Ұ���, 1 : ���� )
    BYTE        m_byFreeStatInitLevel;              // ���� ���� �ʱ�ȭ ����,_NA_0062_20100318_CHANGE_LEVEL_FOR_STAT_INIT_2
#ifdef _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO
    BYTE        m_byDominationChannel;           // ���� ������ ü�� ����
#endif
#ifdef __NA_0_20080515_BUG_FIX_WORLD_SHOUT_LEVEL
	BYTE		m_byWorldShoutLevel;				// ���� ��ġ�� ���� ����
	GAME_OPTION() : m_byMaxLevel(0),
					m_MaxLevelChangeOfClass1st(0),
					m_byNormalShoutLevel(0),
					m_byWorldShoutPCBANG(0),
                    m_byFreeStatInitLevel(0),
					m_byWorldShoutLevel(0) {}
#endif
};


// ��� â�� �α� ����
typedef struct
{
	INT m_ItemCode;
	INT m_LimiteCode;
	WORD m_ItemNum;
	MONEY m_Money;
	INT64 m_UsedTime;
	GuildWarehouseLogType m_LogType;
	TCHAR m_UsedCharName[MAX_CHARNAME_LENGTH+1];

} STRUCT_GUILDWAREHOUSE_LOGINFO, *LP_STRUCT_GUILDWAREHOUSE_LOGINFO;

typedef util::cSTRING_BUFFER<MAX_SSN_ENCODED_STREAM_LENGTH+1>	sSSN_ENCODED_STREAM;
typedef util::cSTRING_BUFFER<MAX_CHARNAME_LENGTH+1>				sSTRING_BUFFER_CHAR_NAME;

struct CHANNEL_USERCOUNT
{
	DWORD	dwChannelID;
	DWORD	dwVillageCount;
	DWORD	dwVillagePCRoomCount;
	DWORD	dwBattleZoneCount;
	DWORD	dwBattleZonePCRoomCount;
	CHANNEL_USERCOUNT() 
		: dwChannelID(0), dwVillageCount(0), dwVillagePCRoomCount(0), 
		dwBattleZoneCount(0), dwBattleZonePCRoomCount(0) {}
};

//__NA001254_20090109_RTTG_FUNC_ADD
typedef struct tag_USER_INFO
{
	tag_USER_INFO() {
		ZeroMemory(m_szRecvCharName, MAX_CHARNAME_LENGTH+1);
	}
	char	m_szRecvCharName[MAX_CHARNAME_LENGTH+1];	// ������ ���� ĳ���͸�
} USER_INFO, *LPUSER_INFO;

//�׷� �ý��� : �׷������� �� �׷� �ɹ����� ���� '__KR_001355_20090608_GROUPINGSYSTEM_DATA'
struct STRUCT_GROUP_MEMBER_PACKET_INFO
{
    char		    ptszMemberName[MAX_CHARNAME_LENGTH+1];	// ģ�� �̸�
    BOOL	        bOnline;								// �¶����̸� TRUE, ���������̸� FALSE
    BYTE	    	byClass;								// Ŭ����
    LEVELTYPE	    Level;									// ����
    BYTE		    byChannel;								// ���� �������� ä��(�¶����� ���� ���)
    INT64		    lastDate;                               // ������ ���� �ð� 
};

struct STRUCT_GROUP_MEMBER_INFO
{
    DWORD			charGUID;		                        //�ɹ����� GUID
    DWORD           accumEXP;                               //���� ����� �α� �ƿ��϶� ���̴� EXP
    eGROUP_STAT	    groupSTAT;		                        //�׷������ ������
    INT64		    lastDate;                               //�ش� ������ ������ ���� �ð�
    STRUCT_GROUP_MEMBER_PACKET_INFO	GroupMemberInfo;
};

struct STRUCT_GROUP_CHECK_DATE
{
    bool            result;                                 //��� ���
    DWORD64         accumDate;                              //���� �ð��� �񱳽ð��� ���� ��
};

//{ designed by __NA_001290_20090525_SHIELD_SYSTEM

class Character;

// (NOTE) (PROPOSAL) (WAVERIX) �ϱ�� ���� ���� ���... ���ٰ� �����ȴ�.
// �� data container�θ� Ȱ���ϴ� ���� ������ ������ �����ڴ� �� �ǹ̰� ���ٰ� �����ȴ�.
// ���� �Ϲݷ��� �ƴ� �����, ó�� ó���� �⺻�� �ǰ�����,
// ���� �ڵ� ó�� ���°� �ݺ��ȴٰ� �������� �ʴ°�? �߰��� �߸� ó���ؼ� �ٰ߲� �߻��ϱ�
// ���ٴ� ���� ���� ���� �� �����ϰ� ���������� ������ �� �Ŷ� �����ȴ�.
struct ArgDamaged {	      // Damaged()�� ����

    ArgDamaged() :
        pAttacker(NULL),
        attackType(eATTACK_TYPE_INVALID),
        damage(0),
        bCritical(FALSE),
        bMirror(FALSE),
        fLimitHP(0),
        attackKind(eATTACK_RESIST_NOMAL),
        SDApply(eSD_APPLY_EMPTY)
    {
    }

    /*
    // (CHANGES) (WAVERIX) remove meaningless code block
    ~ArgDamaged() {
        pAttacker   = NULL;
        attackType  = eATTACK_TYPE_INVALID;
        damage      = 0;
        bCritical   = FALSE;
        bMirror     = FALSE;
        fLimitHP    = 0;
        attackKind  = eATTACK_RESIST_NOMAL;
        SDApply     = eSD_APPLY_EMPTY;
    }

    const ArgDamaged& operator = ( const ArgDamaged& rhs ) {
        this->pAttacker  = rhs.pAttacker;
        this->attackType = rhs.attackType;
        this->damage     = rhs.damage;
        this->bCritical  = rhs.bCritical;
        this->bMirror    = rhs.bMirror;
        this->fLimitHP   = rhs.fLimitHP;
        this->attackKind = rhs.attackKind;
        this->SDApply   = rhs.SDApply;

        return *this;
    }
    */

    Character*      pAttacker;
    eATTACK_TYPE    attackType;
    DAMAGETYPE      damage;
    BOOLEAN         bCritical; // (CHANGES) (WAVERIX) changes boundary alignment by change types
    BOOLEAN         bMirror;
    INT             fLimitHP;  // (NOTE) prefix f~ : int room_type ?
    eATTACK_RESIST  attackKind;
    eSD_APPLY       SDApply;
};
//}__NA_001290_20090525_SHIELD_SYSTEM

// (WAVERIX) (090819) Added by _PCBANG_POLICY_CHANGE_POST_EXPIRED
struct CHANGE_PCROOM_STATUS
{
    enum eStatus {
        eStatus_None,           // �Ϲ� + �̰���PC��
        eStatus_PcRoom,         // ���� PC��
        eStatus_PcRoomExpired,  // ���� PC�� (���� ���� ����)
    };

    BYTE status_;   //eStatus
};
/************************************************************************/
/*                      ���� ������ ���� ����ü                         */
/************************************************************************/
//_NA_0_20100705_DOMINATION_EVENT
struct DOMINATION_REWARD_EVENT_INFO
{                                  
    enum EventStats{
        all_active = 0,     // ���� Ʈ���� �ߵ� ������, ���� Ʈ���ŵ� �ߵ��� ����.
        all_Inactive,       // ��� �ߵ� ���� ����.
        event_started       // ���� Ʈ���Ÿ� �ߵ��� ����.
    };

    BYTE  m_byEventStats;                                   //�����̺�Ʈ Ȱ��ȭ ����
    WORD  m_wEventDayOfWeek;                                //�����̺�Ʈ �ߵ� ����(0~6)
    DWORD m_dwEventTime;                                    //�����̺�Ʈ �ߵ� �ð�(wzTime)
};

//�̴ϸʿ� ǥ���ϰų� ������ ������ ��Ÿ���� ���� ����ü
struct DOMINATION_GUILD_INFO
{
    GUILDGUID       m_GuildGuid;                            //��� Guid
    GUILDMARKIDX    m_PatternIndex;                         //��� ��ũ ����
    GUILDMARKIDX    m_BackGroundIndex;                      //��� ��ũ ���
    TCHAR  		    m_tszGuildName[MAX_GUILDNAME_LENGTH+1];	//����
    TCHAR  		    m_tszGuildMaster[MAX_CHARNAME_LENGTH+1];//��� �����͸�
    MAPCODE         m_MapCode;                              //���� �� �ڵ�
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    eDOMINATION_PROGRESS_STATE progress_state_;             //�������� �������
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
};

//��� ������ �����ϱ� ���� ����ü
struct DOMINATION_BASIC_GUILD_INFO
{
    GUILDGUID       m_GuildGuid;                            //��� Guid
    GUILDMARKIDX    m_PatternIndex;                         //��� ��ũ ����
    GUILDMARKIDX    m_BackGroundIndex;                      //��� ��ũ ���
    TCHAR  		    m_tszGuildName[MAX_GUILDNAME_LENGTH+1];	//����
    TCHAR  		    m_tszGuildMaster[MAX_CHARNAME_LENGTH+1];//��� �����͸�
    CHARGUID        m_GuildMasterGuid;                      //��� ������Guid
};

struct DOMINATION_INFO
{                                                                       
    MAPCODE                     m_FieldCode;                //���� �ڵ�
    DOMINATION_BASIC_GUILD_INFO m_inGuildInfo;              //���� ��� ����              
    DOMINATION_BASIC_GUILD_INFO m_disGuildInfo;             //���� ��� ����
    BYTE                        m_DeffenseWallA;            //�溮 A
    BYTE                        m_DeffenseWallB;            //�溮 B
    BYTE                        m_DeffenseWallC;            //�溮 C
    BYTE                        m_TotalPoint;               //�溮�� ���� �ִ� �� ������
    ULONGLONG                   m_BestCost;                 //���ݱ�尡 ������ ����ְ�
    DOMINATION_REWARD_EVENT_INFO m_rewardEvent; //_NA_0_20100705_DOMINATION_EVENT
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    eDOMINATION_PROGRESS_STATE progress_state_;             //�������� �������
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
};



//Ŭ���̾�Ʈ�� ���޵Ǿ����� ��� ����
struct DOMINATION_ACTION_INFO
{
    MAPCODE     m_FieldCode;                                //���� �ڵ�  
    GUILDGUID   m_GuilGuid;                                 //���� ���
    TCHAR  	    m_tszGuildName[MAX_GUILDNAME_LENGTH+1];		//���� ���� ����
    ULONGLONG   m_TenderCost;                               //���� �ݾ�
    TCHAR		m_tszActionApplyTime[MAX_TIMEDATA_SIZE + 1];//���� �ð�
};

//�������� ���Ǵ� ��� ����
struct DominationAuctionInfo
{
    DOMINATION_ACTION_INFO m_ActionInfo;                    //�⺻ ��� ����
    GUILDMARKIDX    m_PatternIndex;                         //��� ��ũ ����
    GUILDMARKIDX    m_BackGroundIndex;                      //��� ��ũ ���
    TCHAR  		    m_tszGuildMaster[MAX_CHARNAME_LENGTH+1];//��� �����͸�
    CHARGUID        m_MasterGuid;                           //��� ������ Guid
};

//������ �ɹ� ��� ����
struct DominationMemberInfo
{
    bool guild_type_;//true : ���� / false : ����
    CHARGUID member_guid_;
    LEVELTYPE member_level_;
    eCHAR_TYPE member_class_;
    TCHAR member_name_[MAX_CHARNAME_LENGTH+1];
};

struct DOMINATION_RETURNMONEY_INFO
{
    TCHAR  		    m_tszMaster[MAX_CHARNAME_LENGTH+1]; 	//ȯ�� ���� �����͸�
    ULONGLONG       m_TenderCost;                           //ȯ�� ���� �ݾ�
    eSYSTEM_MEMO_TYPE m_CostType;                           //ȯ������ �������� ������
};

struct sDOMINATION_EVENT_INFO
{
    // (CHANGES) (f100604.3L) remove constructors
    //sDOMINATION_EVENT_INFO() {}
    //~sDOMINATION_EVENT_INFO() {}
    //sDOMINATION_EVENT_INFO(const sDOMINATION_EVENT_INFO& rhs);
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    WORD m_wEventIndex;     //�� ������ �̺�Ʈ�� �����ϴ� �ε���
    WORD m_wMapCode;        //�ش� ������ �̺�Ʈ�� ����� ������ ���� �ε���
    BYTE m_bDominationType; //�ش� �������� ������ �����ϴ� �ε��� 0: �Ϲ� ������ 1:��Ʈ����������
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    WORD  m_wDayOfWeek;                     //Sunday = 0, Monday = 1
    DWORD m_dwTime[DOMINATION_EVENT_MAX];

    sDOMINATION_EVENT_INFO()
    {
        ::ZeroMemory(this, sizeof(*this));
    }
};

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
struct sDOMINATION_CONTINENT_LORD_ALLIENCE_INFO
{
    char allience_guild_name[MAX_GUILDNAME_LENGTH+1];

    sDOMINATION_CONTINENT_LORD_ALLIENCE_INFO()
    {
        memset(allience_guild_name, 0, sizeof(allience_guild_name));
    }
};

struct sDOMINATION_CONTINENT_LORD_INFO  //������, ����� ������ ������ ����
{
    MAPCODE map_code;                           //�� �ڵ�
    BYTE    continent_number;                   //��� ��ȣ
    char    guild_name[MAX_GUILDNAME_LENGTH+1]; //��� �̸�
    BYTE    allience_guild_count;               //���� ��� ��
    //���� ��� ����
    sDOMINATION_CONTINENT_LORD_ALLIENCE_INFO    allience_guild_infos[MAX_ALLIENCE_GUILD_NUM];

    sDOMINATION_CONTINENT_LORD_INFO()
    {
        map_code = 0;
        continent_number = 0;
        memset(guild_name, 0, sizeof(guild_name));
        allience_guild_count = 0;
    }
};
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

/************************************************************************/

//ģ���ʴ� ����Ʈ ����ü
//_KR_0_20091021_FIRENDINVITATION_EXTEND
struct INVITATEFRIENDLIST_INFO
{
    enum FRIEND_ACTION {FRIENDLIST_INIT = 0, FRIENDLIST_ADD, FRIENDLIST_DEL}; //ĳ���� ������ ó��, ģ�� ��õ ��� �߰�, ģ�� ��õ ��� ����
    DWORD m_dwCharGuid;   // ����� ������ ĳ���� GUID
    BYTE  m_byListCount;  // ��� ����
    BYTE  m_FriendSts;    // 5: ���� �ʴ���ģ��, 6:���� �ʴ��� ģ��, ref:STRUCT_FRIEND_DB_INFO::eNodeType
    BYTE  m_FreindAction; // FRIEND_ACTION ����
    DWORD m_GuidList[MAX_FRIEND_STAT_NUM]; // ĳ���� GUID ���
    WORD GetSize() const
    {   // (f100624.2L)
        const size_t kMinSize = sizeof(*this) - sizeof(m_GuidList);
        return static_cast<WORD>(kMinSize + (sizeof(m_GuidList[0]) * m_byListCount));
    }
};

//_KR_0_20091021_FIRENDINVITATION_EXTEND
struct STRUCT_FRIEND_INVITATE_INFO
{
    DWORD       dwGuid;
    DWORD		dwFriendGuid;
    char		ptszFriendName[MAX_CHARNAME_LENGTH+1];
    BYTE		byClass;		// Ŭ����
};

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
/************************************************************************/
/*             (�̼�|����|PVP) ���� ��� ����ü                         */
/************************************************************************/
struct GAMEROOM_BATTLE_UPDATE_INFO
{
    CHARGUID   CharGuid;
    DWORD   dwTotalDamageToEnemy;   // ������ ���� ������ �ѷ�
    DWORD   dwTotalDamageByEnemy;   // ������ ���� ������ �ѷ�
    DWORD   dwTotalHealToParty;     // �ڽŹ� ��Ƽ���� ����� �� ����
    DWORD   dwMaxDamageToEnemy;     // ������ ���� �ִ� ������
    WORD    wKillCount;             // ���� Ƚ��
    WORD    wDeadCount;             // ���� Ƚ��
    WORD    wLevel;                 // �÷��̾� ����
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BYTE strongpoint_attack;        // ���� ���� ����
    BYTE strongpoint_defense;       // ���� ���� ���
    WORD series_kill;               // ���� ���� ų��
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    WORD assist_count;              // ���� ��ý�Ʈ Ƚ��
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    WORD gold_point;                 // ȹ�� �������Ʈ(��巯��)
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
};

struct GAMEROOM_BATTLE_ENTER_INFO
{
    CHARGUID   CharGuid;
    char	szName[MAX_CHARNAME_LENGTH+1];
    BYTE	byClass;		// Ŭ����
    BYTE    byLevel;        // ����
    WORD    wTeam;          // ��(��������/PVP����) �׿� 0 ��
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE    gender;                 // ����(���� ����)
#endif // _NA_002050_20110216_ADD_GENDER
};


struct GAMEROOM_BATTLE_LEAVE_INFO
{
    CHARGUID    CharGuid;       
    BYTE        AbNormalDisconnect; // ������ ����� ���� �������� ���� (0 : �Ϲ�, 1 : ������)
};

/************************************************************************/

#if !defined(__SHOP_CASH_DATA_DECLARATION__)
#define __SHOP_CASH_DATA_DECLARATION__
// presentation by _NA00XXXX_090111_CASH_TYPE_SHARED_
struct CashData
{
    CASH cash_;     // default cash
    CASH pp_cash_;  // pp card cash
    DWORD mileage_; // china mileage service
};
#endif

// _NA_0_20100211_SD_EFFECT_MODIFY
struct PAIR_DAMAGE
{
    PAIR_DAMAGE() 
        : damageHP(0), damageSD(0) {}

    PAIR_DAMAGE(DAMAGETYPE hp, DAMAGETYPE sd) 
        : damageHP(hp), damageSD(sd) {}

    DAMAGETYPE damageHP;
    DAMAGETYPE damageSD;
};

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
typedef DWORD PartyFindingKeyType;
typedef WORD PartyKeyType;
typedef DWORD UserChannelType;

struct PartyInfo // ��Ƽ ����
{
    explicit PartyInfo() : 
        party_key_(0), 
        num_of_joined_member_(0), 
        party_master_id_(0), 
        linked_party_finding_key_(0)
    {
    }

    PartyKeyType party_key_; // ��Ƽ Ű
    PartyFindingNumOfMemberConst::ValueType num_of_joined_member_; // ���� ������ �ο�
    CHARGUID party_master_id_; // ��Ƽ��
    PartyFindingKeyType linked_party_finding_key_; // ����Ǿ� �ִ� ��Ƽ ���� Ű
}; // PartyInfo

struct PartyFindingInfo // ��Ƽ ���� ����
{
    enum { NULL_PARTY_FINDING = 0 };
    enum { NULL_PARTY = 0 };
    enum { kCharNameBufferLength = MAX_CHARNAME_LENGTH + 1 };

    explicit PartyFindingInfo() : 
        party_finding_key_(NULL_PARTY_FINDING), 
        party_goal_(PartyFindingGoalConst::GetDefaultValue()), 
        num_of_max_joinable_member_(PartyFindingNumOfMemberConst::GetDefaultValue()), 
        party_master_level_(0), 
        party_master_location_(0), 
        party_master_channel_(0), 
        linked_party_key_(NULL_PARTY)
    {
        ::ZeroMemory(party_finding_title_, sizeof(party_finding_title_));
        ::ZeroMemory(party_master_name_, sizeof(party_master_name_));
    }

    PartyFindingKeyType party_finding_key_; // ��Ƽ ���� Ű        
    TCHAR party_finding_title_[PartyFindingTitleConst::kBufferLength]; // ���� ����
    PartyFindingGoalConst::ValueType party_goal_; // ��Ƽ ����
    PartyFindingNumOfMemberConst::ValueType num_of_max_joinable_member_; // �ִ� ���� �ο�

    TCHAR party_master_name_[kCharNameBufferLength]; // ��Ƽ�� �̸�
    LEVELTYPE party_master_level_; // ��Ƽ�� ����
    MAPCODE party_master_location_; // ��Ƽ�� ��ġ
    UserChannelType party_master_channel_; // ��Ƽ�� ä��

    PartyKeyType linked_party_key_; // ����Ǿ� �ִ� ��Ƽ Ű
}; // PartyFindingInfo
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

//_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
struct CharacterAppearanceInfo // ĳ���� �ܸ� ����
{
    explicit CharacterAppearanceInfo() : 
        appearance_type_(
            ChangeCharacterAppearanceTypeConst::EnumToValue(ChangeCharacterAppearanceTypeConst::kNone)), 
        face_(0), 
        hair_(0), 
        height_(0)
  #ifdef _NA_002050_20110216_ADD_GENDER
        , gender(0)
  #endif
    {
    }

    ChangeCharacterAppearanceTypeConst::ValueType appearance_type_; // ����� �ܸ� ���� (��Ʈ �÷���)
    BYTE face_; // ��
    BYTE hair_; // ���
    BYTE height_; // Ű
  #ifdef _NA_002050_20110216_ADD_GENDER
    BYTE gender; // ����
  #endif
}; // CharacterAppearanceInfo

// designed by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
//�ý��� �޸��� ���� �⺻���� �����̴�. �ý��� �޸� �߰� ������
//����ü �ۼ��� �ش� ����ü�� ���� �ؾ� �Ѵ�.
struct BasicSystemMemoInfo
{
    //-- �ɸ��� �̸� or Guid�� ������� ������ ���� �� �ִ�
    TCHAR recv_user_name_[MAX_CHARNAME_LENGTH+1];//ȯ�� ���� �ɸ��͸�
    CHARGUID recv_user_guid_;//ȯ�� ���� �ɸ��� Guid
    TCHAR system_memo_contents_[MAX_MEMO_LENGTH + 1];//�ý��� �޸� ����
    SLOTCODE recv_item_code_;//ȯ�� ���� ������ �ڵ�
    ULONGLONG amount_cost_;//ȯ�� ���� �ݾ�
    eSYSTEM_MEMO_TYPE system_memo_type_;//�ý��� �޸� ������

    explicit BasicSystemMemoInfo():
    recv_user_guid_(0),
    recv_item_code_(0),
    amount_cost_(0),
    system_memo_type_(NORMAL_MEMO)
    {
        ZeroMemory(recv_user_name_, sizeof(recv_user_name_));
        ZeroMemory(system_memo_contents_, sizeof(system_memo_contents_));
    }
};

//���� ������ ���� �ý��� �޸� ���� ����ü �޸� ������ ���ڿ� �Ľ��� 
//�ϰ� �Ǵµ� ������ ���� �ʿ��� ������ �ٸ� �� �ִ�. 
//���������� �Ʒ��� ���� �߰� ���� ����ü�� �ʿ��� ��Ȳ�� �߻�
struct SystemMemoInfoOfDomination : public BasicSystemMemoInfo
{
    MAPCODE map_code_;
    TCHAR guild_name_[MAX_GUILDNAME_LENGTH+1];

    explicit SystemMemoInfoOfDomination():
    map_code_(0)
    {
        ZeroMemory(guild_name_, sizeof(guild_name_));
    }
        
};

//���� �ζ� �ý��� �޸� ����ü
struct SystemMemoInfoOfLotto : public BasicSystemMemoInfo
{
    INT64 i64Idx_;
    WORD draw_num_;
    BYTE status_;
};

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
struct SystemMemoInfoOfWorldRanking : public BasicSystemMemoInfo
{
    RANKINGID ranking_id;
    BYTE reward_ranking;
    
    SystemMemoInfoOfWorldRanking() : ranking_id(0), reward_ranking(0) {}
};
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

#ifdef _NA_003923_20120130_GUILD_RENEWAL
struct GuildInGameLogData
{
    BYTE log_type_;//GuildInGameLogType
    DWORD param1_;
    DWORD param2_;
    DWORD param3_;
    INT64 log_time_;
    TCHAR string_param_[MAX_IN_GAME_LOG_PARAM_STR];

    GuildInGameLogData()
    {
        param1_ = 0;
        param2_ = 0;
        param3_ = 0;
        log_time_ = 0;
        ZeroMemory(string_param_, sizeof(string_param_));
    }
};
#endif//_NA_003923_20120130_GUILD_RENEWAL

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
struct BATTLE_GROUND_TEAM_MEMBER_INFO
{
    ChaosZoneTeam team_key;
    TCHAR character_name[MAX_CLIENT_CHARNAME_LENGTH];

    BATTLE_GROUND_TEAM_MEMBER_INFO()
    {
        team_key = kChaosZoneTeamNone;
        ZeroMemory(character_name, sizeof(character_name));
    }
};

struct BATTLE_GROUND_TEAM_SCORE_INFO
{
    ChaosZoneTeam team_key;
    BYTE strongpoint_count;
    WORD victory_point;
};
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
struct GOLDRUSH_TEAM_MEMBER_INFO
{
    ChaosZoneTeam team_key;
    BYTE gold_grade;
    TCHAR character_name[MAX_CLIENT_CHARNAME_LENGTH];

    GOLDRUSH_TEAM_MEMBER_INFO()
    {
        gold_grade = 0;
        team_key = kChaosZoneTeamNone;
        ZeroMemory(character_name, sizeof(character_name));
    }
};

struct GOLDRUSH_TEAM_SCORE_INFO
{
    struct GoldRushUserInfo
    {
        TCHAR character_name[MAX_CLIENT_CHARNAME_LENGTH];
        WORD gold_grade;

        GoldRushUserInfo() : gold_grade(0) {
            ZeroMemory(character_name, sizeof(character_name));
        }
    };

    ChaosZoneTeam team_key;
    WORD team_gold_point;
    BYTE team_member_count;
    GoldRushUserInfo team_player_info[10];

    GOLDRUSH_TEAM_SCORE_INFO() : team_member_count(0), team_gold_point(0) {
        ZeroMemory(team_player_info, sizeof(team_player_info));
    }
};
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

struct BATTLE_GROUND_STRONGPOINT_INFO
{
    WzID strongpoint_id; // ���� ���̵�
    ChaosZoneTeam team_key;
    BattleGroundStrongpointState strongpoint_state;
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND 
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
struct BaseFreePVPRoomInfo
{
    MAPCODE map_code;                                   // �� �ڵ�
    TCHAR room_title[MAX_ROOMTITLE_LENGTH + 1];         // �� ����
    TCHAR room_password[MAX_ROOMPASSWORD_LENGTH + 1];   // �� ��� ��ȣ
    BaseFreePVPRoomInfo() : map_code(0)
    {
        ZeroMemory(room_title, sizeof(room_title));
        ZeroMemory(room_password, sizeof(room_password));
    }
};

struct FreePVPRoomInfo
{
    BaseFreePVPRoomInfo base_free_pvp_room_info; // �⺻ �� ����
    KEYTYPE room_key; // ��Ű(index)
    KEYTYPE zone_key; // �δ� �� Ű
    BYTE current_user_count; // ���� �ο�
    BYTE max_user_count; // �ִ� �ο�

    FreePVPRoomInfo() : 
        room_key(0),
        current_user_count(0),
        max_user_count(0)
    {
    }
};

#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_006372_20130108_BADGE_ITEM
struct TOGGLE_ITEMS
{
    bool     is_equiped_; // Ȱ��ȭ ����
    POSTYPE  item_pos_; // �κ��丮 ��ġ
    SLOTCODE item_code_; // ������ ��ȣ

    TOGGLE_ITEMS()
    {
        clear();
    };

    void clear()
    {
        is_equiped_ = false;
        item_pos_ = INVALID_POSTYPE_VALUE;
        item_code_ = 0;
    };
};
#endif //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
struct CHAOS_ZONE_RECORD_INFOS 
{
    enum { kRecordMaxCount = 5 };
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    GoldRushRecord goldrush_record[kRecordMaxCount+1];
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    BattleGroundRecord battle_ground_record[kRecordMaxCount+1];
    CHAOS_ZONE_RECORD_INFOS()
    {
        Clear();
    }
    void Clear()
    {
        ZeroMemory(battle_ground_record, sizeof(battle_ground_record));
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        ZeroMemory(goldrush_record, sizeof(goldrush_record));
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    }
};

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
struct ATTENDANCE
{
    enum { kDayMaxCount = 31, };
    DWORD               year;
    BYTE                month;
    eATTENDANCE_TYPE    type;
    int                 title;
    int                 explanation;
    BYTE                day_emphasis[kDayMaxCount];
    CODETYPE            day_reward_index[kDayMaxCount];
};

class AttendanceInfo
{
public:
    enum { kRecordMaxCount = 31 };

    AttendanceInfo();
    eATTENDANCE_INFO* begin() { return &m_info[0]; };
    eATTENDANCE_INFO* end() { return &m_info[kRecordMaxCount]; }
    eATTENDANCE_INFO* GetAttendance(int day);
    bool  SetAttendance(int day, eATTENDANCE_INFO info);
    void  init(DWORD year, BYTE month, eATTENDANCE_TYPE type, eATTENDANCE_INFO info);
    bool  IsEqualAttendanceInfo(int day, eATTENDANCE_INFO type);
    int   GetAttendanceInfoCount( eATTENDANCE_INFO type );
    DWORD GetYear() const { return m_year; };
    BYTE  GetMonth() const { return m_month; };
    eATTENDANCE_TYPE GetType() const { return m_type; };

private:
    eATTENDANCE_INFO m_info[kRecordMaxCount + 1];
    eATTENDANCE_TYPE m_type;
    DWORD            m_year;
    BYTE             m_month;
};
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
struct SunRankingInfoForClient
{
    SunRankingInfoForClient()
    {
        memset(char_name, 0, MAX_CHARNAME_LENGTH+1);
        score = 0;
        class_code = eCHAR_NONE;
    }

    char       char_name[MAX_CHARNAME_LENGTH+1];
    SCORE      score;
    eCHAR_TYPE class_code;
};
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
struct NotificationInfo
{
    NotificationInfo()
    {
        Clear();
    }

    void Clear();

    void SetInfo(const TCHAR* char_name, MAPCODE map_code, DWORD time,
        NotificationType notification_type, SLOTCODE item_code, int option_value1 = 0);

    TCHAR               m_char_name[MAX_CHARNAME_LENGTH + 1];
    MAPCODE             m_map_code;
    SLOTCODE            m_item_code;
    DWORD               m_time;
    NotificationType    m_notification_type;
    int                 m_optione_value1;
};
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
struct BingoInfo
{
    BYTE bingo_number;
    eMINIGAME_BINGO_STATE bingo_state;
};

struct MiniGameBingoInfos
{
    MiniGameBingoInfos()
    {
        Clear();
    }

    void Clear()
    {
        for (int i = 0; i < MINIGAME_BINGO_LENGTH; ++i)
        {
            m_bingo_infos[i].bingo_number = 0;
            m_bingo_infos[i].bingo_state = eMINIGAME_BINGO_STATE_EMPTY;
        }

        bingo_event_index = 0;
    }

    bool IsGenerated()
    {
        for (int i = 0; i < MINIGAME_BINGO_LENGTH; ++i)
        {
            // ���� ���ڿ��� 0�� ����
            if (m_bingo_infos[i].bingo_number == 0)
            {
                return false;
            }
        }
        return true;
    }

    int bingo_event_index;
    BingoInfo m_bingo_infos[MINIGAME_BINGO_LENGTH];
};
#endif // _NA_008404_20150712_MINIGAME_SYSTEM

struct AttrOptionInfo
{
    WORD m_wOptionKind;
    BYTE m_byOptionType;
    INT  m_iOptionValue;

    AttrOptionInfo() : 
    m_wOptionKind(0), m_byOptionType(0), m_iOptionValue(0)
    {
    }
    AttrOptionInfo(WORD kind, BYTE type, INT value) :
    m_wOptionKind(kind), m_byOptionType(type), m_iOptionValue(value)
    {}
};

#pragma warning(pop)
#pragma pack(pop)

#endif // __STRUCT_H__
