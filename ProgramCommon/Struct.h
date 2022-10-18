/** Struct_DB.h : DB의 파라미터
	- Number형은 모두 unsigned int로 통일
	@note
		- 확률, 비율의 데이터 타입을 통일할 필요가 있다. 백분율로 할지, 소수로 할지
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
// 캐릭터 생성시 부여되는 캐릭터 초기수치
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
// 캐릭터 종류(class)에 따른 고정 상수
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
	WORD		m_wMoveSpeedRatio;		//< 이동속도(100을 곱한 %수치)
	WORD		m_wAttSpeedRatio;		//< 공격속도(100을 곱한 %수치)
	//BYTE		m_byLeftSelect;
	INT64		m_StateTime;
	char		m_tszTitleID[MAX_TITLEID_LENGTH];
	INT64		m_TitleTime;
	enum eINVEN_LOCK_MODE { ENABLE = 0, EMPTY_PWD = 0, EXIST_PWD = 1, DISABLED = 2,
		ENABLEnEMPTY = ENABLE|EMPTY_PWD, ENABLEnEXIST = ENABLE|EXIST_PWD, DISABLEDnEXIST = DISABLED|EXIST_PWD, };
	INT			m_iInventoryLock;	// eINVEN_LOCK_MODE의 BitFlags
	SHORT		m_sStrength;
	SHORT		m_sDexterity;
	SHORT		m_sVitality;
	SHORT		m_sInteligence;
	SHORT		m_sSpirit;
	SHORT		m_sSkillStat1;
	SHORT		m_sSkillStat2;
	UPTYPE		m_UserPoint;

	struct { 	
	WORD		m_byGMGrade		: 3;			//< GM 등급( 0 : 일반, 1이상 : GM, 4 : 가상아이디 ) eCHAR_CONDITION
	WORD		m_byPCBangUser	: 1;
	WORD		m_byCondition	: 1;			//< 현재상태(앉기/서기)
	WORD		m_byPKState		: 3;			//< 8
	WORD		m_byPCRoomSts	: 1;			//< PC방 블락상태
	WORD		m_byCharState	: 7;			//<	eDBCHAR_STATE			
	};
	// 2005/12/28(taiyo)
	// 캐릭터 리스트 갈때 받은 값을 사용하라.
	//BYTE		m_byHeight;
	//BYTE		m_byFace;
	//BYTE		m_byHair;
	INT			m_PlayLimitedTime;
    /// [2012-4-20 by i4u4me] 이거 나누어 쓰자. 
    union
    {
        BYTE		m_byInvisibleOptFlag;					//< eINVISIBLE_OPTION_FLAG 참조
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


	// 현재는 이 정보만 추가
	GUILDGUID	m_GuildGuid;
	BYTE		m_GuildDuty;
	char		m_tszGuildName[MAX_GUILDNAME_LENGTH];

	// 카오
	BYTE		m_ChaoState;		// 카오 상태(eCHAO_STATE)
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

// 상태저항
struct RESISTANCE_CONDITION
{
	WORD			m_wStateCode;		// 상태코드
	BYTE			m_byRatio;			// 상태저항 확률
	tagWZID			m_wzidEffectCode;	// 이펙트 코드
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
	BYTE			m_byChangeTargetRatio;			// 타겟변경 확률
	DWORD			m_dwMaxHP;
	DWORD			m_dwMaxMP;
	DWORD			m_dwHP;
    DWORD			m_dwMP;
	DWORD			m_dwMaxSD;
    DWORD			m_dwSD;
	CODETYPE		m_NCode;						// 네임코드
#ifdef _DH_NPC_GROUP_NAME
    DWORD group_name; //그룹네임코드
#endif//_DH_NPC_GROUP_NAME
	DWORD			m_dwICode;						// 이미지 코드
	WORD            m_wNpcImageIndex;               // pc형 몬스터 이미지 인덱스 

	BYTE			m_byGrade;						// 보스/일반
	float			m_fGradeExpRatio;				// 경험치 등급 적용 비율
	WORD			m_wSize;						// 크기

	WORD			m_wAICode;						// AI 타입
	BYTE			m_byAttType;					// 공격타입(근거리, 원거리, 물, 불, ...)
	BYTE			m_byMeleeType;					// 근거리 공격 타입(1:관통, 2:타격, 3:베기)

	BYTE			m_byCriticalRatio;				// 크리티컬 확률
	DWORD			m_dwMinAttackPower;
	DWORD			m_dwMaxAttackPower;
    DWORD           skill_power; // 스킬 공격력 (EP2)
	DWORD			m_dwPhyDef;						// 물리 방어력
	DWORD			m_dwMagicDef;					// 마법 방어력 (EP2: unused)
	BYTE			m_byAttitude;					// 적대 성향(후공, 선공, 조건선공)
	BYTE			m_byMoveAttitude;				// 이동 성향(정지형, 방황형, 감시형)
	DWORD			m_dwMoveAreaID;					// 이동 영역 ID
	WORD			m_wClass;						// 몬스터 계열
    uint16_t element_fire_attack;  // 원소(불) 공격력 (EP2)
    uint16_t element_water_attack; // 원소(물) 공격력 (EP2)
    uint16_t element_wind_attack;  // 원소(바람) 공격력 (EP2)
    uint16_t element_earth_attack; // 원소(대지) 공격력 (EP2)
    uint16_t element_dark_attack;  // 원소(암흑) 공격력 (EP2)
    int16_t element_fire_resist;  // 원소(불) 저항력, EP2: [R] 공격력 %차감률(+/-)
    int16_t element_water_resist; // 원소(물) 저항력, EP2: [R] 공격력 %차감률(+/-)
    int16_t element_wind_resist;  // 원소(바람) 저항력, EP2: [R] 공격력 %차감률(+/-)
    int16_t element_earth_resist; // 원소(대지) 저항력, EP2: [R] 공격력 %차감률(+/-)
    int16_t element_dark_resist;  // 원소(암흑) 저항력, EP2: [R] 공격력 %차감률(+/-)
    NpcElementWeakness element_weakness; // NOTE: f110520.2L, the element weakness of the npc based on the user view
    //
    BYTE            m_byArmorType; // NOTE: EP2 unused... 아머 타입(1:Hard, 2:Medium, 3:Soft, 4:Siege, 5:Unarmer)
    uint16_t        m_wPhyAttRate;      // 물리공격 성공률, EP2:applied, percentage (0.95 -> 95%)
    uint16_t        m_wPhyAttAvoid;     // 물리공격 회피율, EP2:applied, percentage (0.95 -> 95%)
	float			m_fBodyRadius;					// 몸체 반지름
	float			m_fAttRange;					// 공격 거리
	float			m_fViewRange;					// 시야 범위
	float			m_fMoveRange;					// 이동 거리
	float			m_fWalkSpeed;					// 걷는 속도
	float			m_fRunSpeed;					// 달리는 속도
	WORD			m_wAttSpeed;					// 공격 속도(물리, 마법)
	WORD			m_wAttSpeed2;
	WORD			m_wProjectileCode;				// 사용 발사체코드
	WORD			m_wProjectileCode2;
	BYTE			m_byJumpRatio;					// 점프 확률
    //_YJ_ADD_DAMAGE_EFFECT_NPCINFO
	WzID			m_wiDamageEffectID;				//NPC에 지정된 데미지 이펙트가 있다면 출력 없으면 기본 데미지 이펙트 출력
	WzID			m_wiCDamageEffectID;			//NPC에 지정된 크리티컬 데미지 이펙트가 있다면 출력 없으면 기본 데미지 이펙트 출력
	enum {MAX_SPECIAL_NUM = 2,};
	SPECIAL_CONDITION m_SpecialCondition[MAX_SPECIAL_NUM];			// 특수행동
	enum {MAX_RESISTANCE_NUM = 2,};
	RESISTANCE_CONDITION m_ResistanceCondition[MAX_RESISTANCE_NUM];	// 상태저항  (m_wSkillUpdateTime에 따라 상태 저항 발동)
#ifdef _NA_007620_20140915_STATE_RESISTS
    STLX_MAP<WORD, RESISTANCE_CONDITION>* state_resistances_;           // 상태저항2 (                상태가 걸릴 때 저항 발동)
#endif //_NA_007620_20140915_STATE_RESISTS

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    BYTE skill_update_condition;                                    // SkillUpdateTime의 랜덤성을 결정
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
	WORD			m_wSkillUpdateTime;								// 스킬조건을 체크하는 주기시간

	BYTE			m_byReviveCondition;							// 부활 조건
	WORD			m_wReviveConditionParam;						// 부활 조건 파라미터
	BYTE			m_byReviveRate;									// 부활 확률
	SLOTCODE		m_wReviveCode;									// 부활 스킬코드
	WORD			m_wReviveDelay;									// 부활 애니 딜레이시간

	BYTE			m_byHealCondition;								// 힐 조건
	WORD			m_wHealConditionParam;							// 힐 조건 파라미터
	BYTE			m_byHealRate;									// 힐 확률
	SLOTCODE		m_wHealCode;									// 힐 스킬코드
	WORD			m_wHealDelay;									// 힐 애니 딜레이시간

	BYTE			m_bySummonCondition;							// 소환 조건
	WORD			m_wSummonConditionParam;						// 소환 조건 파라미터
	BYTE			m_bySummonRate;									// 소환 확률
	SLOTCODE		m_wSummonCode;									// 소환 스킬코드
	WORD			m_wSummonDelay;									// 소환 애니 딜레이시간
	
	enum {MAX_SKILL_NUM = 10,};
	BYTE			m_bySkillUsePossible;							// 기타 스킬 사용 확률(일반 공격이 아닌)
	SLOTCODE		m_wSkillCode[MAX_SKILL_NUM];					// 기타 스킬 코드
	BYTE			m_bySkillRate[MAX_SKILL_NUM];					// 기타 스킬 확률
	WORD			m_wSkillDelay[MAX_SKILL_NUM];					// 기타 스킬 애니 딜레이시간

	WORD			m_wHateSkill;									// 증오 스킬
	WORD			m_wRevengeSkill;								// 복수 스킬
	BYTE			m_byRevengeRate;								// 복수 확률

	WORD			m_byItemDropRate;								// 아이템 드롭율 : 단위 일만 
	
	BOOL			m_bRegion;
	enum {MAX_DROPRATIO_INDEX_NUM = 10,};
	bool			m_bOwnerShipForDropIndex[MAX_DROPRATIO_INDEX_NUM];
	DWORD			m_dwDropIndex[MAX_DROPRATIO_INDEX_NUM];			//< DropRatioInfo의 index
	DWORD			m_dwFieldDropIndex[MAX_DROPRATIO_INDEX_NUM];	//< bRegion이 True일때 && 몬스터가 필드존일때
	DWORD			m_dwBattleDropIndex[MAX_DROPRATIO_INDEX_NUM];	//< bRegion이 True일때 && 몬스터가 배틀존일때

	enum { 
#ifdef _NA_004509_20120314_QUESTDROPINDEX10
        MAX_QUEST_ITEM_DROP_NUM = 10,
#else
        MAX_QUEST_ITEM_DROP_NUM = 5, 
#endif //_NA_004509_20120314_QUESTDROPINDEX10
    };
	WORD			m_QuestCode[MAX_QUEST_ITEM_DROP_NUM];			//< 진행중인 퀘스트
	DWORD			m_dwQuestDropIndex[MAX_QUEST_ITEM_DROP_NUM];	//< DropRatioInfo의 index
	uint8_t			m_wACCode;										//< AC 코드
	BYTE			m_byACReferenceID;								//< AC 참조ID
	DWORD			m_dwOptionID[2];								// 특수 옵션
	DWORD			m_dwOptionParam[2];								// 특수 옵션 파라미터
	WORD			m_wDest;
	WORD			m_wAct[5];
	WORD			m_wP[5];

	tagWZID			m_wzidSpawnAniCode;								// 스폰 애니메이션 코드
	DWORD           m_dwSpawnTime;									// 스폰시간 0이면 스폰 상태 없음 

	//_YJ_SPAWN_MONSTER_WITH_EFFECT_001250
	DWORD			m_dwSpawnEffectCode;							// 스폰시 출력될 이펙트 코드
    //_NA_0_20100514_NPC_DEAD_STATE_TIME
    DWORD			m_dwDeadWaitingTime;							// 사망 상태 유지 시간(사망 후 - 연기 전)
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
	eROTATE_TYPE	m_eRotateType;	// 대화시 돌아보는지 여부.
	eDISPLAY_TYPE	m_eDisplayType; // 미니멥에 표시 여부.
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
    SLOTCODE m_SkillCode;       // 스타일 혹은 스킬 코드
    SLOTCODE m_SkillClassCode;  // 스타일 혹은 스킬 구현 ID
    BYTE     m_byType;          // 스킬 혹은 스타일 여부 // enum { ABILITY_TYPE, SKILL_TYPE };
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

    char      m_szSkillName[MAX_SKILLNAME_LENGTH]; // 스킬 이름
    CODETYPE  m_dwSkillNcode;         // 네임 코드
    CODETYPE  m_dwSkillDesccode;      // 스킬 설명 코드
    CODETYPE  m_dwSkillIconcode;      // 스킬 아이콘 코드
#ifdef __NA001126_NEW_SKILL_SCREEN_EFFECT
    BYTE      m_bySwayType;           // 흔들림 효과
    DWORD     m_dwSwayTime;           // 흔들림 주기타임
    BYTE      m_bySwayNum;            // 흔들림 횟수
    DWORD     m_dwSwaySize;           // 흔들림 크기
#endif //__NA001126_NEW_SKILL_SCREEN_EFFECT(클라에서만쓰임)
    char      m_szSkillMasterEffectName[MAX_SKILLNAME_LENGTH];
    int       m_iWeaponDefine[4];     // 무기 제한
    tagWZID   m_wzidAnimationCode[4]; // 애니메이션 코드
    WORD      m_CSSyncDelay;          // 스킬 사용시 모든 애니메이션이 완료되는 시간 (애니메이션 처리시 서버/클라 위치 동기화 유지를 위한 컨트롤 수치
    tagWZID   m_wzidCastAniCode;      // 캐스트 애니메이션 코드
    CODETYPE  m_dwFlyingObjectCode;   // 발사체 코드
    WORD      m_wFlyingLifeTime;      // 발사체 라이프 타임
    tagWZID   m_wzidFieldEffectCode;  // 필드이펙트 코드

    CODETYPE  m_dwItemCode;           // 아이템 코드 

    WORD      m_wSkillAttribute;      // 스킬 계열
    LEVELTYPE m_wRequireLv;           // 요구 레벨
    LEVELTYPE m_wSkillLv;             // 해당 레벨
    LEVELTYPE m_wMaxLv;               // 최고 레벨
    LEVELTYPE m_wOverLv;              // 오버 가능 레벨
    WORD      m_wOverStatclass[2];    // 오버스탯 종류
    WORD      m_wOverstat[2];         // 오버 스탯 요구치/스킬 요구 스탯
    BYTE      m_bySkillType;          // 스킬 타입
    BYTE      m_byEtherBulletInfo;    // 에테르 총알 정보
    BYTE      m_bySkillUserType;      // 스킬사용자 타입
    CODETYPE  m_dwClassDefine;        // 클래스 제한
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    CODETYPE  m_UpperSkillClass;       // 스킬시스템 도입시 숙련도 사라짐. 스킬 레벨업할 조건으로 상위 직속 스킬 클래스의 값으로 대체.
#endif
    // _NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
    BYTE      m_RequireUpperSkillLevel;
    BYTE      m_bySkillStatType;      // 숙련도 구분
    WORD      m_wRequireSkillStat[2]; // 요구 숙련도
    BYTE      m_byRequireSkillPoint;  // 요구 스킬포인트
    //IFF_Filter iff_filter; // CHANGES: f110309.1L, 피아 식별 및 대상 선택 조건 집합 (ref:m_byTarget)
    BYTE      m_byTarget;             // 타겟 지정 대상
    BYTE      m_byForbiddenTarget;    // 스킬사용 제한 대상
    WORD      m_wHPSpend;             // HP 소모
    WORD      m_wMPSpend;             // MP 소모
    WORD      m_wSkillCasting;        // 스킬 캐스팅 타임
    DWORD     m_dwCoolTime;           // 쿨타임
    WORD      m_wSkillRange;          // 스킬 사거리
    BYTE      m_byAttRangeform;       // 스킬 범위형태
    WORD      m_wSkillArea;           // 스킬 범위
    WORD      m_wChaseRange;          // 추적 범위
    BYTE      m_byMaxTargetNo;        // 최대 마리수
    BYTE      m_bySkillAcquire;       // 스킬 획득
#ifdef _YJ_ADD_SKILL_SCRIPT_FIELD_4_AUTO_ATTACK_001212
    BYTE      m_btAutoAttack;   // 스킬사용후 자동공격 할지 안할지(0 == 자동공격함, 1 == 자동공격안함)
#endif //_YJ_ADD_SKILL_SCRIPT_FIELD_4_AUTO_ATTACK_001212

    bool IsNonStopSkill() const;  // 사용할때 멈추지 않는 스킬이다.
    bool IsSummonSkill() const;   // 소환체가 사용하는 스킬이다.
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

    BYTE m_byIndex;      // 어빌리티 인덱스
    WORD m_wAbilityID;   // 어빌러티 ID
    IFF_Filter iff_filter; // CHANGES: f110309.1L, 피아 식별 및 대상 선택 조건 집합 (ref:m_byRangeType)
    BYTE m_byRangeType;  // 적용범위 Type
    WORD m_wSuccessRate; // 성공확률 (천분율)
    int  m_iOption1;     // 기능타입1
    int  m_iOption2;     // 기능타입2
    WORD m_wStateID;     // 상태ID
    enum { MAX_ABILITY_PARAM_NUM = 4 };
    int  m_iParam[MAX_ABILITY_PARAM_NUM]; // 각종 어빌러티 수치값
    
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

    //CODETYPE m_dwStyleID;         // 스타일 ID
    //CODETYPE m_dwStyleClassID;    // 스타일 종류 ID
    char      m_szStyleName[MAX_SKILLNAME_LENGTH]; // 스타일 이름
    CODETYPE  m_dwStyleNCode;       // 네임 코드
    CODETYPE  m_dwStyleDescCode;    // 스타일 설명 코드
    WORD      m_wSkillattribute;    // 스타일 속성
    CODETYPE  m_dwStyleImage;       // 이미지 코드

    LEVELTYPE m_wRequireLV;         // 요구 레벨
    LEVELTYPE m_wStyleLV;           // 스타일 레벨
    LEVELTYPE m_wMaxLV;             // 최고 레벨
    LEVELTYPE m_wOverLV;            // 오버 가능 레벨

    WORD   m_wOverStatclass[2];     // 오버 스탯 종류
    WORD   m_wOverstat[2];          // 오버 스탯 요구치 / 스타일 요구 스탯

    BYTE   m_byRequireSkillPoint;   // 요구 스킬 포인트
    BYTE   m_byClassDefine;         // 클래스 제한
    int    m_iWeaponDefine;         // 무기 제한
    BYTE   m_byAttType;             // 공격 타입
    float  m_fAttRange;             // 사정 거리
    BYTE   m_byMoveAttack;          // 이동공격 여부
    BYTE   m_byStyleCheck;          // 스타일 여부

    tagWZID m_wzidCodeFirst;        // 1타 코드
    tagWZID m_wzidCodeSecond;       // 2타 코드
    tagWZID m_wzidCodeThird;        // 3타 코드
    tagWZID m_wzidCodeBack;         // 복귀 코드

    WORD  m_wTimeFirst;             // 1타 시간
    WORD  m_wTimeSecond;            // 2타 시간
    WORD  m_wTimeThird;             // 3타 시간

    int   m_iAddDamage[3];          // 추가 데미지
    float m_fDamagePercent[3];      // 추가 데미지%

    BYTE  m_byAttRangeform;         // 3타 범위 형태
    float m_fStyleArea;             // 3타 범위(반지름)
    float m_fThirdDelay;            // 3타 발동 시간

    int   m_iAttackRate;            // 공격 성공 증가
    int   m_iAvoidRate;             // 블록 성공 증가
    int   m_iAttackSpeed;           // 공격 속도 증가
    int   m_iBonusDefence;          // 방어력 증가
    int   m_iMagicDefenceAdd;       // 마법 방어력 증가( ==> 이거 0814에 들어간 건데? 작업한 사람 누구인고? )
    WORD  m_wCriticalBonus;         // 크리티컬 보너스
    float m_fDefenseIgnore;         // 방어력 무시

    float m_fPierceRate;            // 3타 관통 발생 확률
    float m_fPierceRange;           // 3타 관통 거리
    float m_fStunRate;              // 3타 스턴 발생 확률
    WORD  m_wStunTime;              // 3타 스턴 시간
    float m_fKnockBackRate;         // 3타 녹백 발생 확률
    float m_fKnockBackRange;        // 3타 녹백 거리
    float m_fDownRate;              // 3타 다운 발생 확률
    float m_fDelayReduce;           // 딜레이 발생 확률 감소
    float m_fDelayOccur;            // 3타 딜레이 발생 확률

    WORD  m_wHPAbsorb;              // HP 흡수
    float m_fHPAbsorbPer;           // HP% 흡수
    WORD  m_wMPAbsorb;              // MP 흡수
    float m_fMPAbsorbPer;           // MP% 흡수
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
        MAX_MISSION_MAP_NUM_V1_3 = 6,   //World.txt의 bMmap파라미터 갯수.
        MAX_CHUNTING_MAP_NUM_V1_3 = 6,  //World.txt의 wCmap파라미터 갯수.
        MAX_PVP_MAP_NUM_V1_3 = 6,       //World.txt의 wPmap파라미터 갯수.
    };

    MAPCODE     MapCode;
    MAPCODE     MapKind;
    BYTE        byMKind; //< eZONETYPE 값 참고
    BYTE        byMType; //< (0/1/2) <__NA001226_INSTANCE_DUNGEON_EXTENTION__에 의해 인던 세부타입도 추가 설정
    BYTE        byMinUserNum;
    BYTE        byMaxUserNum;
    LEVELTYPE   minLV;
    LEVELTYPE   maxLV;
    BYTE        byENTCount; // ENT 영역 개수
    BYTE        byFCount;
    eZONE_RULESET Ruleset; // reference: eZONE_RULESET
    char        pszDebugName[MAX_MAPNAME_LENGTH];
    DWORD       dwNCode;
    DWORD       dwFNCode;
    DWORD       dwDCode;
    DWORD       dwANCode; //< 그룹 대표 이름.(클라에서 사용)
    WORD        wTimeLimit; 
    DWORD       wText1; //1.3부터 제거
    DWORD       wText2; //1.3부터 제거
    DWORD       wText3; //1.3부터 제거
    DWORD       wStartAreaID;
    DWORD       wStartAreaID2;
    //_NA_20100307_BETAKEY_SYSTEM
    int         nBetaKey;
    uint8_t     reserved_key_index_; // NOTE: f110520.5L, this support the static field unique index
    BYTE        byClass; //< eCLASS_TYPE 타입을 참조해서 채워넣어라!
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
    DWORD       dwImageCode[MAX_FIELD_NUM]; //dImage1~6까지의 값 저장(성현창)
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
        MOVE_TYPE_FIELD = 0, // 필드 포탈 이동
        MOVE_TYPE_VILLAGE = 1, // 마을 NPC 이동
        MOVE_TYPE_WAYPOINT = 2, // 웨이 포인트
        MOVE_TYPE_WARP_MACHINE = 3, // 순간이동장치
        MOVE_TYPE_UPPER 
    };
    //----------------------------------------------------------------------------------------------
    WORD        wIndex; // 인덱스 값
    BYTE        byMapType; // 맵 타입
    BOOL        bMoveType; // 이동타입(0 : 필드이동, 1 : 마을 NPC 이동, 2 : 웨이 포인트, 3 : 순간이동장치)
    MONSTERCODE npc_id_; // 비공정 이동 NPC ID
    WORD        wWorldFrom;
    WORD        wFieldFrom; // 현재 필드 번호
    char        sAreaFromID[sizeof(WzID) + 1]; // 현재필드의 영역 ID
    DWORD       m_dwFromAreaID; // 현재필드의 영역 ID를 StrToWzID()로 변환한 값
    WORD        wWorldTo;
    WORD        wFieldTo; // 오브젝트 클릭시 이동할 필드 ID
    char        sAreaToID[sizeof(WzID) + 1]; // 넘어가는 필드의 영역 ID
    DWORD       m_dwToAreaID; // 넘어가는 필드의 영역 ID를 StrToWzID()로 변환한 값
    BYTE        byMin_Level;  // 최소 레벨 제한 값
    BYTE        byMax_Level;  // 최대레벨 제한 값
    BYTE        byMissionCode;// 미션제한
    WORD        wQuestCode;   // 퀘스트제한
    WORD        wItemCode;    // 진입시 필요 아이템
    WORD        wItemNum;     // 필요 아이템의 개수
    BOOL        bWaste_Item;  // 맵 진입시 필요 아이템의 소비여부 (0: 소비 안됨, 1:소비 됨)
    DWORD       dwHeim;       // 맵 이동시 금액 만큼 소비, 0인 경우 하임 소비 없음

#ifdef _NA_20100307_BETAKEY_SYSTEM
    INT32       nBeytaKey;
    float       fPayApp;
#endif
};

struct sFIELDINFO_BASE
{
    enum { SERVER_TYPE_NONE = 0, SERVER_TYPE_FIELD = 1, SERVER_TYPE_BATTLE = 2, SERVER_TYPE_BOTH = 3 };
    FIELDCODE   FieldCode;
    BYTE        byServerType;   // 1. 필드서버 2. 배틀서버 3. 둘다 로딩
    BYTE        bNewPathEngine; // 신규 패쓰엔진 적용유무
    BYTE        bWarCastle;     // 공성전용 맵인지
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
	WORD			m_wStateID;								// 상태 ID
	char  			m_szStateName[MAX_SKILLNAME_LENGTH];	// 상태 이름
	CODETYPE  		m_dwNamecode;							// 네임 코드
	CODETYPE	  	m_dwDescCode;							// 스킬설명 코드
	DWORD			m_dwIconCode;							// 아이콘 코드
#ifdef _100506_LTJ_MOD_STATUS_BUFF_IMAGE_RENDER
    BYTE            m_byIconRow;                            // 텍스쳐 행위치(1부터)
    BYTE            m_byIconColumn;                         // 텍스쳐 열위치(1부터)
#endif //_100506_LTJ_MOD_STATUS_BUFF_IMAGE_RENDER
	BYTE			m_byIconKind;							// 아이콘 종류
	BYTE			m_bDelType;								// 삭제여부 ( 유저가 임의로 삭제할 수 있는 버프인가 )
	BYTE			m_byType;								// 상태타입(이상, 악화, 강화, 특수)
    BYTE            m_bySDApply;                            // SD 적용여부
    BOOLEAN  m_bRidingApply; // 탈 것에 적용 여부
	tagWZID         m_wzidEffectID;                         // 상태 이펙트 ID
	BYTE            m_byEffectPos;                          // 이펙트 위치
	WORD			m_wGroupCode;							// Group 코드(같은 계열의 상태Group)

    //!_NA_000587_20100928_DOMINATION_BUFF_ITEM
    WORD m_wBoneIndex;      //! 본 인텍스 m_byEffectPos == eSKILLEFFECT_POS_BONE_INDEX 일때만 사용
    float m_fOffsetX;       //! 해당위치에서의 옵셋값
    float m_fOffsetY;       //! 해당위치에서의 옵셋값
    float m_fOffsetZ;       //! 해당위치에서의 옵셋값
    bool m_bApplyParentRotate; //!< 부모 회전값적용
    bool m_bApplyParentScale; //!< 부모 스케일값적용
    
};
#pragma pack(pop)
// change pack info to support busy access structure, end of pack(8) position
//==================================================================================================

// 총 4 BYTE
struct MISSIONPART
{
	UINT State	:4;				// 미션 클리어 상태 : eMISSION_STATE -=> 실제 사용하는 비트는 3bit
	UINT Point	:12;			// 획득 포인트 ~512 -=> 실제 사용하는 비트는 9bit
	UINT Hidden	:16;			// 히든 미션 클리어 여부
};

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 새로 방 테스트 할때 사용하는 구조체 시작!!!!!
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
struct BASE_ROOMINFO
{
    BYTE m_MinLV;
    BYTE m_MaxLV;
    BYTE m_ClassLimit; // 비트연산 eCLASS_TYPE 참조해라!!!!!!
    BYTE m_SpecialType; // eROOM_SPECIAL_TYPE
    // (CHANGES)(20100318)(hukim)(REQ_0038) can_join_room_ 
    // 방에 입장 가능 여부를 표시하는 멤버 추가

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
	BYTE		m_Difficulty	: 4;		//< eHUNTING_MONSTER_DIFFICULT 의 값
	BYTE		m_Bonus			: 4;		//< eHUNTING_BONUS_TYPE 의 값
};

struct ADDITIONAL_PVPINFO
{
	BYTE		m_Rule			: 4;		//< 룰(10/30/무한) ePVP_RULE_TYPE의 값
	BYTE		m_Mode			: 4;		//< 모드(개인/팀) ePVP_MODE_TYPE의 값
};

struct ADDITIONAL_CHUNTINGINFO
{
	BYTE		m_Difficulty	: 4;		//< eHUNTING_MONSTER_DIFFICULT 의 값
	BYTE		m_Bonus			: 4;		//< eHUNTING_BONUS_TYPE 의 값
	BYTE		m_Rule			: 4;		//< 룰 eCOMPETITION_RULE_TYPE(프리퍼올, 듀얼파티)
	BYTE		m_Mode			: 4;		//< 모드 eCOMPETITION_MODE_TYPE(서바이벌, 타임어택)
};

struct ADDITIONAL_CHUNTINGINFO_EX : ADDITIONAL_CHUNTINGINFO
{
	KEYTYPE		m_LinkHuntingKey;			//< 링크시킬 경쟁헌팅방 키(0이면 최초생성)
};

// CHANGES: f110608.6L
struct PVP_LOBBY_INFO
{
    uint8_t m_Team;  //< 팀전 일경우 팀 번호 ( 1 or 2 )  개인전은 0
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
		BYTE			m_CurUserNum		: 4;			//< 현재 인원
		BYTE			m_MaxLimitUserNum	: 4;			//< 최대 인원(NonBlock Slot 개수)
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
		BYTE			m_CurUserNum		: 4;			//< 현재 인원
		BYTE			m_MaxLimitUserNum	: 4;			//< 최대 인원(NonBlock Slot 개수)
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
		BYTE			m_CurUserNum		: 4;			//< 현재 인원
		BYTE			m_MaxLimitUserNum	: 4;			//< 최대 인원(NonBlock Slot 개수)
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
		BYTE			m_CurUserNum		: 4;			//< 현재 인원
		BYTE			m_MaxLimitUserNum	: 4;			//< 최대 인원(NonBlock Slot 개수)
	};
	BASE_ROOMINFO		m_RoomInfo;
	ADDITIONAL_CHUNTINGINFO	m_AdditionalInfo;
};

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 새로 방 테스트 할때 사용하는 구조체 끝!!!!!
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 길드 관련 구조체
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
    DWORD64 m_RelationApplyTime;//길드 관계 성립 시간
    DWORD64 m_RelationChangeTime;//길드 관계 변경 예정 시간
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
    DWORD       m_remain_time;  //남은 시간(단위 : 초)
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
	bool			m_bRightUse;				//해당 권한의 사용유무
};

//길드 등급별 해당 권한 사용 정보
struct GUILD_GRADE_RIGHT_USE : public GUILD_RIGHT_USE 
{
	BYTE	m_Grade;					//해당 권한을 사용하려는 길드등급
};

//길드 직책별 권한 사용 정보
struct GUILD_DUTY_RIGHT_USE : public GUILD_RIGHT_USE 
{
	eGUILD_DUTY		m_Duty;						//해당 권한을 사용하려는 길드직책
};

//길드 권한별 권한 사용 유무
struct GUILD_RIGHT_USE_INFO : public GUILD_RIGHT_USE 
{
	eGUILD_RIGHTS	m_Right;					//사용하려는 권한
};

//길드 직책 사용 유무
struct GUILD_DUTY_USE_INFO
{
	eGUILD_DUTY		m_Duty;							//해당 직책
	DWORD			m_dwStringCode;					//길드직책별 스트링 코드
	bool			m_bUse;							//사용 유무
};


struct GUILD_RANK_INFO
{
	//versino 1.0
	BYTE		m_byLevel;							//길드레벨
	TCHAR		m_szLevelName[MAX_PATH];			//길드레벨별 이름
	WORD		m_wMaxMember;						//해당 레벨별 최대 길드원 수
	DWORD		m_dwHeim;							//해당 레벨로 랭크업 하는데 소요되는 하임.
	UPTYPE		m_UP;								//해당 레벨로 랭크업 하는데 소요되는 UP.
	GPTYPE		m_GP;								//해당 레벨로 랭크업 하는데 소요되는 GP.

	//version 1.1
	CODETYPE	m_RankItemCode;						// 해당 레벨로 랭코업 하는데 필요한 아이템.
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
	BYTE						m_byClass;								// 클라이언트에서 해당 내용 표시한다.
	LEVELTYPE					m_Level;								// 클라이언트에서 해당 내용 표시한다.
	BYTE						m_byChaoState;							// 클라이언트에서 해당 내용 표시한다.
	BOOL						m_bLogin;								// 로그인 여부 ( 월드서버에서는 현재 접속된 유저만 유지하기 때문에 bLogin이 필요 없다. )
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    DWORD                       guild_coin_;                            // 길드 공헌도
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
};

struct SERVER_GUILD_MEMBER_INFO : public GUILD_MEMBER_INFO
{
	GUILD_RIGHT_USE_INFO	m_DelegateRight[MAX_GUILD_DELEGATE_RIGHT];		//위임된 권한.
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
	BYTE				m_GuildGrade;							//게임서버에서 길드 등급별로 랭크업시 돈 체크 해야 한다.
	char				m_tszGuildName[MAX_GUILDNAME_LENGTH+1];	//길드이름이 BASE에 들어가는 이유는 월드와 게임서버에서 로그를 남기기 때문이다.
#ifdef _NA_008334_20150608_SONNENSCHEIN
    BYTE                sonnenschein_phase_;                    //소넨샤인 진행등급(보상등급)
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
	eGUILD_DELCHK		m_GuildDestroyWaitFlag;		//0: 정상, 1: 삭제, 2: 삭제대기
	INT64				m_nMasterChangeTime;
    WORD                m_DominationMapCount;//점령지역 수
    WORD                m_DominationJoinCount;//점령전 참여 횟수
    WORD                m_DominationDefenseCount;//점령전 방어 성공 횟수
    WORD                m_DominationHuntingCount;//점령전 보스 사냥 횟수
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    DWORD               m_GuildCoin;//기부받아 소유하고 있는 길드 코인 갯수
    DWORD               m_GuildGP;
#endif
};

struct GUILD_CORPS_INFO
{
	BYTE m_bCreate;				//현재 생성된 군단인가?
	BYTE m_byCorpsNum;			//군단번호
	BOOL m_bRearCamp;			//육성캠프 생성 여부.
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
	BYTE		m_byOnline;		// 온라인 여부(0: offline 1: online )
	BYTE		m_byChaoState;	// 카오 상태(eCHAO_STATE)
	eGUILD_DUTY	m_eGuildDuty;
	BYTE		m_byCorpsNum;
};

struct PACKET_FOR_GUILD_RELATION
{
	eGUILD_RELATION m_eRelationType;
	GUILDGUID		m_GuildGuid;
	char			m_GuildName[MAX_GUILDNAME_LENGTH+1];
};

// DB에서 읽어오는 친구 정보
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
		INFO_INVIAT = 5,	//초대 한 캐릭터의 친구 관계
		INFO_BEINVIATED = 6,//초대 받은 캐릭터의 친구 관계
							//A 가 B 를 친구초대 하면 A:5, B:6의 관계가 성립된다.
        INFO_BLOCKED_INVIAT = 7,	//차단된 초대 한 캐릭터의 친구 관계(클라전용)
		INFO_BLOCKED_BEINVIATED = 8,//차단된 초대 받은 캐릭터의 친구 관계(클라전용)
        // }
	};	// Sts 구분
	DWORD		dwFriendGuid;
	char		ptszFriendName[MAX_CHARNAME_LENGTH+1];
	BYTE		byClass;		// 클래스
	LEVELTYPE	Level;			// 레벨
	BYTE		Sts;			// 친구 관계 코드, STRUCT_FRIEND_DB_INFO::enumeration
	BYTE		RevengePoint;	// 리벤지 포인트(INFO_REVENGE_POINT일 때만 유효)
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    BYTE        gender; // 성별
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

//// 클라이언트와 주고받는 친구 정보
//struct STRUCT_FRIEND_PACKET_INFO
//{
//	BYTE	 byClass;								// 클래스
//	LEVELTYPE Level;									// 레벨
//	BOOL bOnline; // 온라인이면 TRUE, 오프라인이면 FALSE
//	BYTE	 byChannel;								// 현재 접속중인 채널(온라인일 때만 사용)
//	char	ptszFriendName[MAX_CHARNAME_LENGTH+1];	// 친구 이름
//};

struct STRUCT_FRIEND_PACKET_INFO
{
	char		ptszFriendName[MAX_CHARNAME_LENGTH+1];	// 친구 이름
	BOOL	    bOnline;								// 온라인이면 TRUE, 오프라인이면 FALSE
	BYTE		byClass;								// 클래스
	LEVELTYPE	Level;									// 레벨
	BYTE		byChannel;								// 현재 접속중인 채널(온라인일 때만 사용)
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    BYTE        gender; // 성별
#endif  // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
};

// 월드서버가 보관하는 친구 정보
struct STRUCT_FRIEND_INFO
{
	DWORD		dwFriendGuid;
	STRUCT_FRIEND_PACKET_INFO	sPacketInfo;

	STRUCT_FRIEND_PACKET_INFO* GetPacketInfo() { return &sPacketInfo; }
};

// 월드서버에서 보관하는 차단 정보
struct STRUCT_BLOCK_INFO
{
	DWORD		dwBlockGuid;
	char		ptszBlockName[MAX_CHARNAME_LENGTH+1];
};

// DB에서 읽어오는 차단 정보
typedef STRUCT_BLOCK_INFO	STRUCT_BLOCK_DB_INFO;

// 클라이언트와 주고받는 차단 정보
struct STRUCT_BLOCK_PACKET_INFO
{
	char		ptszBlockedName[MAX_CHARNAME_LENGTH+1];	// 차단당한 상대 이름
};

// 클라이언트와 주고받는 리벤지 정보
struct STRUCT_REVENGE_PACKET_INFO
{
	char		ptszRevengeName[MAX_CHARNAME_LENGTH+1];	// 리벤지할 놈 이름
	BOOL		bOnline;								// 온라인이면 TRUE, 오프라인이면 FALSE
	BYTE		byClass;								// 클래스
	LEVELTYPE	Level;									// 레벨
	BYTE		byChannelID;							// 접속중인 채널
};

// 월드서버에서 보관하는 리벤지 정보
struct STRUCT_REVENGE_INFO
{
	DWORD						dwRevengeCharGuid;
	STRUCT_REVENGE_PACKET_INFO	sPacketInfo;
};

// DB에서 읽어오는 리벤지 정보
typedef STRUCT_REVENGE_INFO	STRUCT_REVENGE_DB_INFO;

struct EVENT_STATUS
{
	CODETYPE	m_dwEventID;		// 이벤트 인덱스
	WORD		m_wRegisterCnt;		// 이벤트 응모 회수 (하임 이벤트인 경우 모든 라운드를 돈 회수)
	EVENT_STATUS() : m_dwEventID(0), m_wRegisterCnt(0) {}
};


struct EVENT_STATUS_VER2 : EVENT_STATUS
{
	BYTE		m_DonateHeimRewardCnt;	// 단위 구간 하임 기부에 의해 보상된 회수
	BYTE		m_Status;				// 0: (진행중), 1: (완료)
	DWORD		m_CurDonatedHeims;		// 현재까지 누적된 하임 총액 (모든 라운드가 끝나면 자동 클리어)

	EVENT_STATUS_VER2() : EVENT_STATUS(), m_DonateHeimRewardCnt(0), m_Status(0), m_CurDonatedHeims(0) {}
};
typedef struct EVENT_STATUS_VER2		EVENT_STATUS_EX;

//	EventDB를 다목적으로 사용하는 첫 사례
//	Event상태 목록 수신시 event 타입에 따른 filtering이 요구된다.
struct SSQ_RESERVED_REWARD_INFO
{	// 총 5개의 필드 사용 가능
	enum {};
	//struct {	//EventStatusQuery::sQUERY의 구조
	//	INT		__m_EventID;				// m_SSQRewardType
	//	INT		__m_RegisterCnt;
	//	INT		__m_DonateHeimRewardCnt;
	//	INT		__m_DonatedHeims;
	//	INT		__m_Status;					// bit
	//}			__usable_tab_in_DBtable;
	struct SharedField {
		CODETYPE	EventID;
		BYTE		Status;				// used) __m_Status					- 0: (미보상), 1: (보상처리완료) < 클라에서는 미사용
	};
	struct WinnerReward : public SharedField {
		DWORD		Point;				// used) __m_RegisterCnt			- 획득 포인트
		DWORD		Exp;				// used) __m_DonateHeimRewardCnt	- 획득 경험치
		BYTE		MonsterKillNum;		// used) __m_DonatedHeims:8			- 몬스터 킬 수
		BYTE		PlayerKillNum;		// used) __m_DonatedHeims:8			- 플레이어 킬 수
		SLOTCODE	ItemCode;			// used) __m_DonatedHeims:16		- 보상 아이템(판도라) <-
	};
	struct QuestFail : public SharedField {
		DWORD		Point;				// used) __m_RegisterCnt			- 획득 포인트
		DWORD		Exp;				// used) __m_DonateHeimRewardCnt	- 획득 경험치
		BYTE		MonsterKillNum;		// used) __m_DonatedHeims:8			- 몬스터 킬 수
		BYTE		PlayerKillNum;		// used) __m_DonatedHeims:8			- 플레이어 킬 수
	};
	struct AdmissionFeeRefund : public SharedField {
		SLOTCODE	ItemCode;			// used) __m_RegisterCnt			- 복구해줄 아이템
		DWORD		Money;				// used) __m_DonatedHeims			- 복구해줄 Heim
	};

	union {
		SharedField			m_SharedInfo;
		WinnerReward		m_WinnerReward;				// 승자 보상 정보, DB
		QuestFail			m_QuestFail;				// 퀘스트 실패 (정산정보)
		AdmissionFeeRefund	m_AdmissionFeeRefund;		// 입장료 환불 처리
	};
};


// 음성 채팅 상태 전달용 구조체
typedef struct
{
	char		m_ptszCharName[MAX_CHARNAME_LENGTH+1];		// 해당 파티원 이름
	BOOL		m_bVoiceOn;									// TRUE: 켜짐 FALSE: 꺼짐
} STRUCT_VOICE_STATUS, *LP_STRUCT_VOICE_STATUS;

// 쪽지 관련 구조체들
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
	BYTE		m_byRead;		// 읽었으면 1, 아니면 0
	char		m_ptszSubject[MAX_SUBJECT_STRLENGTH];
	char		m_tszCreateDate[21];	// 보낸 날짜
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
	BYTE		m_byMaxLevel;						// 최대레벨
	WORD		m_MaxLevelChangeOfClass1st;			// 전직 후 최대 레벨
	BYTE		m_byNormalShoutLevel;				// 일반 외치기 가능 레벨
	BYTE		m_byWorldShoutPCBANG;				// 월드 외치기 PC방 가능 여부( 0 : 불가능, 1 : 가능 )
    BYTE        m_byFreeStatInitLevel;              // 무료 스텟 초기화 레벨,_NA_0062_20100318_CHANGE_LEVEL_FOR_STAT_INIT_2
#ifdef _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO
    BYTE        m_byDominationChannel;           // 지역 점령전 체널 정보
#endif
#ifdef __NA_0_20080515_BUG_FIX_WORLD_SHOUT_LEVEL
	BYTE		m_byWorldShoutLevel;				// 월드 외치기 가능 레벨
	GAME_OPTION() : m_byMaxLevel(0),
					m_MaxLevelChangeOfClass1st(0),
					m_byNormalShoutLevel(0),
					m_byWorldShoutPCBANG(0),
                    m_byFreeStatInitLevel(0),
					m_byWorldShoutLevel(0) {}
#endif
};


// 길드 창고 로그 정보
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
	char	m_szRecvCharName[MAX_CHARNAME_LENGTH+1];	// 쪽지를 받을 캐릭터명
} USER_INFO, *LPUSER_INFO;

//그룹 시스템 : 그룹정보내 각 그룹 맴버들의 정보 '__KR_001355_20090608_GROUPINGSYSTEM_DATA'
struct STRUCT_GROUP_MEMBER_PACKET_INFO
{
    char		    ptszMemberName[MAX_CHARNAME_LENGTH+1];	// 친구 이름
    BOOL	        bOnline;								// 온라인이면 TRUE, 오프라인이면 FALSE
    BYTE	    	byClass;								// 클래스
    LEVELTYPE	    Level;									// 레벨
    BYTE		    byChannel;								// 현재 접속중인 채널(온라인일 때만 사용)
    INT64		    lastDate;                               // 마지막 접속 시간 
};

struct STRUCT_GROUP_MEMBER_INFO
{
    DWORD			charGUID;		                        //맴버들의 GUID
    DWORD           accumEXP;                               //상위 멤버가 로그 아웃일때 샇이는 EXP
    eGROUP_STAT	    groupSTAT;		                        //그룹원들의 상하위
    INT64		    lastDate;                               //해당 유저의 마지막 접속 시간
    STRUCT_GROUP_MEMBER_PACKET_INFO	GroupMemberInfo;
};

struct STRUCT_GROUP_CHECK_DATE
{
    bool            result;                                 //계산 결과
    DWORD64         accumDate;                              //현제 시간과 비교시간의 차이 값
};

//{ designed by __NA_001290_20090525_SHIELD_SYSTEM

class Character;

// (NOTE) (PROPOSAL) (WAVERIX) 하기와 같은 접근 방식... 좋다고 생각된다.
// 단 data container로만 활용하는 경우는 있으나 마나한 연산자는 별 의미가 없다고 생각된다.
// 물론 일반론은 아닌 관계로, 처음 처리가 기본이 되겠지만,
// 유사 코드 처리 형태가 반복된다고 생각되지 않는가? 중간에 잘못 처리해서 삐꾸가 발생하기
// 보다는 차라리 없는 것이 더 안전하고 가벼워지는 구현이 될 거라 생각된다.
struct ArgDamaged {	      // Damaged()의 인자

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
        eStatus_None,           // 일반 + 미가맹PC방
        eStatus_PcRoom,         // 정액 PC방
        eStatus_PcRoomExpired,  // 정액 PC방 (정액 만료 상태)
    };

    BYTE status_;   //eStatus
};
/************************************************************************/
/*                      지역 점령전 관련 구조체                         */
/************************************************************************/
//_NA_0_20100705_DOMINATION_EVENT
struct DOMINATION_REWARD_EVENT_INFO
{                                  
    enum EventStats{
        all_active = 0,     // 시작 트리거 발동 했으며, 종료 트리거도 발동한 상태.
        all_Inactive,       // 모두 발동 안한 상태.
        event_started       // 시작 트리거만 발동한 상태.
    };

    BYTE  m_byEventStats;                                   //점령이벤트 활성화 여부
    WORD  m_wEventDayOfWeek;                                //점령이벤트 발동 요일(0~6)
    DWORD m_dwEventTime;                                    //점령이벤트 발동 시간(wzTime)
};

//미니맵에 표시하거나 점령전 정보를 나타내기 위한 구조체
struct DOMINATION_GUILD_INFO
{
    GUILDGUID       m_GuildGuid;                            //길드 Guid
    GUILDMARKIDX    m_PatternIndex;                         //길드 마크 패턴
    GUILDMARKIDX    m_BackGroundIndex;                      //길드 마크 배경
    TCHAR  		    m_tszGuildName[MAX_GUILDNAME_LENGTH+1];	//길드명
    TCHAR  		    m_tszGuildMaster[MAX_CHARNAME_LENGTH+1];//길드 마스터명
    MAPCODE         m_MapCode;                              //점령 맵 코드
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    eDOMINATION_PROGRESS_STATE progress_state_;             //점령지역 진행상태
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
};

//길드 정보를 세팅하기 위한 구조체
struct DOMINATION_BASIC_GUILD_INFO
{
    GUILDGUID       m_GuildGuid;                            //길드 Guid
    GUILDMARKIDX    m_PatternIndex;                         //길드 마크 패턴
    GUILDMARKIDX    m_BackGroundIndex;                      //길드 마크 배경
    TCHAR  		    m_tszGuildName[MAX_GUILDNAME_LENGTH+1];	//길드명
    TCHAR  		    m_tszGuildMaster[MAX_CHARNAME_LENGTH+1];//길드 마스터명
    CHARGUID        m_GuildMasterGuid;                      //길드 마스터Guid
};

struct DOMINATION_INFO
{                                                                       
    MAPCODE                     m_FieldCode;                //지역 코드
    DOMINATION_BASIC_GUILD_INFO m_inGuildInfo;              //점령 길드 정보              
    DOMINATION_BASIC_GUILD_INFO m_disGuildInfo;             //공격 길드 정보
    BYTE                        m_DeffenseWallA;            //방벽 A
    BYTE                        m_DeffenseWallB;            //방벽 B
    BYTE                        m_DeffenseWallC;            //방벽 C
    BYTE                        m_TotalPoint;               //방벽에 쓸수 있는 총 데이터
    ULONGLONG                   m_BestCost;                 //공격길드가 입찰한 경매최고가
    DOMINATION_REWARD_EVENT_INFO m_rewardEvent; //_NA_0_20100705_DOMINATION_EVENT
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    eDOMINATION_PROGRESS_STATE progress_state_;             //점령지역 진행상태
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
};



//클라이언트에 전달되어지는 경매 정보
struct DOMINATION_ACTION_INFO
{
    MAPCODE     m_FieldCode;                                //지역 코드  
    GUILDGUID   m_GuilGuid;                                 //입찰 길드
    TCHAR  	    m_tszGuildName[MAX_GUILDNAME_LENGTH+1];		//지역 점령 길드명
    ULONGLONG   m_TenderCost;                               //입찰 금액
    TCHAR		m_tszActionApplyTime[MAX_TIMEDATA_SIZE + 1];//입찰 시간
};

//서버간에 사용되는 경매 정보
struct DominationAuctionInfo
{
    DOMINATION_ACTION_INFO m_ActionInfo;                    //기본 경매 정보
    GUILDMARKIDX    m_PatternIndex;                         //길드 마크 패턴
    GUILDMARKIDX    m_BackGroundIndex;                      //길드 마크 배경
    TCHAR  		    m_tszGuildMaster[MAX_CHARNAME_LENGTH+1];//길드 마스터명
    CHARGUID        m_MasterGuid;                           //길드 마스터 Guid
};

//점령전 맴버 노드 정보
struct DominationMemberInfo
{
    bool guild_type_;//true : 공격 / false : 수비
    CHARGUID member_guid_;
    LEVELTYPE member_level_;
    eCHAR_TYPE member_class_;
    TCHAR member_name_[MAX_CHARNAME_LENGTH+1];
};

struct DOMINATION_RETURNMONEY_INFO
{
    TCHAR  		    m_tszMaster[MAX_CHARNAME_LENGTH+1]; 	//환급 받을 마스터명
    ULONGLONG       m_TenderCost;                           //환급 받을 금액
    eSYSTEM_MEMO_TYPE m_CostType;                           //환급인지 보상인지 구분자
};

struct sDOMINATION_EVENT_INFO
{
    // (CHANGES) (f100604.3L) remove constructors
    //sDOMINATION_EVENT_INFO() {}
    //~sDOMINATION_EVENT_INFO() {}
    //sDOMINATION_EVENT_INFO(const sDOMINATION_EVENT_INFO& rhs);
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    WORD m_wEventIndex;     //각 점령전 이벤트를 구분하는 인덱스
    WORD m_wMapCode;        //해당 점령전 이벤트와 연결된 점령전 정보 인덱스
    BYTE m_bDominationType; //해당 점령전의 종류를 구분하는 인덱스 0: 일반 점령전 1:몬트샤인점령전
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

struct sDOMINATION_CONTINENT_LORD_INFO  //점령전, 대륙의 영주의 정보를 저장
{
    MAPCODE map_code;                           //맵 코드
    BYTE    continent_number;                   //대륙 번호
    char    guild_name[MAX_GUILDNAME_LENGTH+1]; //길드 이름
    BYTE    allience_guild_count;               //동맹 길드 수
    //동맹 길드 정보
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

//친구초대 리스트 구조체
//_KR_0_20091021_FIRENDINVITATION_EXTEND
struct INVITATEFRIENDLIST_INFO
{
    enum FRIEND_ACTION {FRIENDLIST_INIT = 0, FRIENDLIST_ADD, FRIENDLIST_DEL}; //캐릭터 접속후 처음, 친구 추천 목록 추가, 친구 추천 목록 삭제
    DWORD m_dwCharGuid;   // 목록을 소유할 캐릭터 GUID
    BYTE  m_byListCount;  // 목록 개수
    BYTE  m_FriendSts;    // 5: 내가 초대한친구, 6:나를 초대한 친구, ref:STRUCT_FRIEND_DB_INFO::eNodeType
    BYTE  m_FreindAction; // FRIEND_ACTION 참고
    DWORD m_GuidList[MAX_FRIEND_STAT_NUM]; // 캐릭터 GUID 목록
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
    BYTE		byClass;		// 클래스
};

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
/************************************************************************/
/*             (미션|헌팅|PVP) 전투 결과 구조체                         */
/************************************************************************/
struct GAMEROOM_BATTLE_UPDATE_INFO
{
    CHARGUID   CharGuid;
    DWORD   dwTotalDamageToEnemy;   // 적에게 입힌 데미지 총량
    DWORD   dwTotalDamageByEnemy;   // 적에게 당한 데미지 총량
    DWORD   dwTotalHealToParty;     // 자신및 파티에게 사용한 총 힐량
    DWORD   dwMaxDamageToEnemy;     // 적에게 입힌 최대 데미지
    WORD    wKillCount;             // 죽인 횟수
    WORD    wDeadCount;             // 죽은 횟수
    WORD    wLevel;                 // 플레이어 레벨
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BYTE strongpoint_attack;        // 전장 거점 공격
    BYTE strongpoint_defense;       // 전장 거점 방어
    WORD series_kill;               // 전장 연속 킬수
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    WORD assist_count;              // 전장 어시스트 횟수
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    WORD gold_point;                 // 획득 골드포인트(골드러시)
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
};

struct GAMEROOM_BATTLE_ENTER_INFO
{
    CHARGUID   CharGuid;
    char	szName[MAX_CHARNAME_LENGTH+1];
    BYTE	byClass;		// 클래스
    BYTE    byLevel;        // 레벨
    WORD    wTeam;          // 팀(경쟁헌팅/PVP팀전) 그외 0 값
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE    gender;                 // 성별(종종 구분)
#endif // _NA_002050_20110216_ADD_GENDER
};


struct GAMEROOM_BATTLE_LEAVE_INFO
{
    CHARGUID    CharGuid;       
    BYTE        AbNormalDisconnect; // 비정상 종료로 인해 나갔는지 여부 (0 : 일반, 1 : 비정상)
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

struct PartyInfo // 파티 정보
{
    explicit PartyInfo() : 
        party_key_(0), 
        num_of_joined_member_(0), 
        party_master_id_(0), 
        linked_party_finding_key_(0)
    {
    }

    PartyKeyType party_key_; // 파티 키
    PartyFindingNumOfMemberConst::ValueType num_of_joined_member_; // 현재 모집된 인원
    CHARGUID party_master_id_; // 파티장
    PartyFindingKeyType linked_party_finding_key_; // 연결되어 있는 파티 모집 키
}; // PartyInfo

struct PartyFindingInfo // 파티 모집 정보
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

    PartyFindingKeyType party_finding_key_; // 파티 모집 키        
    TCHAR party_finding_title_[PartyFindingTitleConst::kBufferLength]; // 모집 내용
    PartyFindingGoalConst::ValueType party_goal_; // 파티 목적
    PartyFindingNumOfMemberConst::ValueType num_of_max_joinable_member_; // 최대 모집 인원

    TCHAR party_master_name_[kCharNameBufferLength]; // 파티장 이름
    LEVELTYPE party_master_level_; // 파티장 레벨
    MAPCODE party_master_location_; // 파티장 위치
    UserChannelType party_master_channel_; // 파티장 채널

    PartyKeyType linked_party_key_; // 연결되어 있는 파티 키
}; // PartyFindingInfo
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

//_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
struct CharacterAppearanceInfo // 캐릭터 외모 정보
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

    ChangeCharacterAppearanceTypeConst::ValueType appearance_type_; // 사용할 외모 종류 (비트 플래그)
    BYTE face_; // 얼굴
    BYTE hair_; // 헤어
    BYTE height_; // 키
  #ifdef _NA_002050_20110216_ADD_GENDER
    BYTE gender; // 성별
  #endif
}; // CharacterAppearanceInfo

// designed by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
//시스템 메모의 가장 기본적인 정보이다. 시스템 메모 추가 컨텐츠
//구조체 작성시 해당 구조체를 포함 해야 한다.
struct BasicSystemMemoInfo
{
    //-- 케릭터 이름 or Guid를 대상으로 쪽지를 보낼 수 있다
    TCHAR recv_user_name_[MAX_CHARNAME_LENGTH+1];//환급 받을 케릭터명
    CHARGUID recv_user_guid_;//환급 받을 케릭터 Guid
    TCHAR system_memo_contents_[MAX_MEMO_LENGTH + 1];//시스템 메모 내용
    SLOTCODE recv_item_code_;//환급 받을 아이템 코드
    ULONGLONG amount_cost_;//환급 받을 금액
    eSYSTEM_MEMO_TYPE system_memo_type_;//시스템 메모 구분자

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

//지역 점령전 관련 시스템 메모 정보 구조체 메모 내용을 문자열 파싱을 
//하게 되는데 컨텐츠 마다 필요한 정보가 다를 수 있다. 
//컨텐츠별로 아래와 같은 추가 정보 구조체가 필요한 상황이 발생
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

//하임 로또 시스템 메모 구조체
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
    WzID strongpoint_id; // 거점 아이디
    ChaosZoneTeam team_key;
    BattleGroundStrongpointState strongpoint_state;
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND 
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
struct BaseFreePVPRoomInfo
{
    MAPCODE map_code;                                   // 맵 코드
    TCHAR room_title[MAX_ROOMTITLE_LENGTH + 1];         // 룸 제목
    TCHAR room_password[MAX_ROOMPASSWORD_LENGTH + 1];   // 룸 비밀 번호
    BaseFreePVPRoomInfo() : map_code(0)
    {
        ZeroMemory(room_title, sizeof(room_title));
        ZeroMemory(room_password, sizeof(room_password));
    }
};

struct FreePVPRoomInfo
{
    BaseFreePVPRoomInfo base_free_pvp_room_info; // 기본 방 정보
    KEYTYPE room_key; // 룸키(index)
    KEYTYPE zone_key; // 인던 존 키
    BYTE current_user_count; // 현재 인원
    BYTE max_user_count; // 최대 인원

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
    bool     is_equiped_; // 활성화 여부
    POSTYPE  item_pos_; // 인벤토리 위치
    SLOTCODE item_code_; // 아이템 번호

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
            // 빙고 숫자에는 0이 없다
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
