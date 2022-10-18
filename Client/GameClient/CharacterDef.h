#pragma once

#include "WeaponSoundInfoParser.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Form Character.h
const DWORD ATTACK_ADDITIONAL_EFFECT_STUN				= 0x00000001;
const DWORD ATTACK_ADDITIONAL_EFFECT_KNOCKBACK			= 0x00000002;
const DWORD ATTACK_ADDITIONAL_EFFECT_FREEZE				= 0x00000004;
const DWORD ATTACK_ADDITIONAL_EFFECT_DOWN				= 0x00000008;
const DWORD ATTACK_ADDITIONAL_EFFECT_AIR				= 0x00000010;
const DWORD ATTACK_ADDITIONAL_EFFECT_TOGROUND			= 0x00000020;
const DWORD ATTACK_ADDITIONAL_EFFECT_REBIRTH			= 0x00000040;
const DWORD ATTACK_ADDITIONAL_EFFECT_CRITICAL			= 0x00000080;
const DWORD ATTACK_ADDITIONAL_EFFECT_DONOTPLAYANIMATION = 0x00000100;
const DWORD ATTACK_ADDITIONAL_EFFECT_DOUBLE				= 0x00000200;
const DWORD ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_METAL	= 0x00000400;
const DWORD ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_ETHER	= 0x00000800;
const DWORD ATTACK_ADDITIONAL_EFFECT_KNOCKBACK2			= 0x00001000;
const DWORD ATTACK_ADDITIONAL_EFFECT_KNOCKBACK3			= 0x00002000;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum eForceTurn
{
	FORCE_TURN_NONE = 0,
	FORCE_TURN_LEFT = 1,
	FORCE_TURN_RIGHT = 2
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_DAMAGE_EFFECT_CNT 5
#define MAX_DAMAGE_EFFECT_CNT_IN_DOMINATION 3

//BP_PANTS(바지)까지는 캐릭몸관련 파츠이며 무기및 악세사리는 Attach형식입니다.
enum BodyPart
{
	BP_BONE,	
	BP_HAIR,// 머리카락
	BP_FACE,// 얼굴
	BP_BOOTS,// 신발
	BP_ARMOR,// 가슴파츠
	BP_HELM,// 투구	
	BP_GLOVE,// 장갑
	BP_PANTS,// 바지
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	BP_DECORATE_COSTUME, // 데코 코스튬
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
	BP_WEAPON,// 무기(Attach)
	BP_SPECIAL_ACCESSORY1,//특수 악세사리 1(Attach)
	BP_SPECIAL_ACCESSORY2,//특수 악세사리 2(Attach)
	BP_SPECIAL_ACCESSORY3,//특수 악세사리 3(Attach)
	BP_COUNT,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum BodyPartNum
{
	kBeginBodyParts = BP_BONE + 1,              // 장비 파츠 시작인덱스
	kBeginAccessory = BP_SPECIAL_ACCESSORY1,    // 악세사리 시작인덱스
	kEndBodyParts   = BP_WEAPON,                // 장비 파츠 갯수들

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum eActionID
{
	ACTION_IDLE			= 0,
	ACTION_ATTACK		= 1,
	ACTION_STYLE_ATTACK	= 2,
	ACTION_SKILL		= 3,
	ACTION_MOVE			= 4,
	ACTION_KEYBOARDMOVE	= 5,
	ACTION_KNOCKBACK	= 6,
	ACTION_TOAIR		= 7,
	ACTION_DOWN			= 8,
	ACTION_GETUP		= 9,
	ACTION_NPCMEET		= 10,
	ACTION_GETITEM		= 11,
	ACTION_JUMP			= 12,
	ACTION_EVENTJUMP    = 13,
	ACTION_STOP         = 14,
	ACTION_VALKYRIESHOT = 15,
	ACTION_VENDORMEET   = 16,
	ACTION_PLAYERMEET	= 17,
	ACTION_SIT          = 18,	
	ACTION_SPECIAL_MOVE = 19,	
	ACTION_USE_OBJECT   = 20,
	ACTION_PLAYERFOLLOW	= 21,
	ACTION_KEYBOARDJUMP = 22,
	ACTION_TELEPORT     = 23,
	ACTION_ATTACKJUMP   = 24,
	ACTION_RUNAWAY      = 25,
	ACTION_WAYPOINT_MEET= 26,
	ACTION_TRANSFORM    = 27,
	ACTION_FALLDOWN     = 28,
	ACTION_KNOCKBACK2	= 29,
	ACTION_AIR_FALLDOWN	= 30,
	ACTION_FIRECRACKER = 31,
	ACTION_COLLECTION_OBJECT = 32,
	ACTION_SUMMON_RIDER= 33,
	ACTION_RIDER_TRUN = 34,
	ACTION_DOMINATION_OBJECT = 35,
	ACTION_ETHERIA_EXTRACT = 36,
	ACTION_TRIGGER_ANIMATION_CONTROL = 37,
	ACTION_AIRSHIP_MEET = 38,

    //! 2011.2.28 / i4u4me / 날개
    ACTION_WINGS_SPREAD = 39,
    ACTION_WINGS_FURL = 40,
    ACTION_RIDER_SPREAD = 41,
    ACTION_RIDER_FURL = 42,

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	ACTION_STATUE_PLAYER_MEET = 43,
	ACTION_STATUE_STAND = 44,
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

	ACTION_INVALID
};

class cAttackResultData
{
public:
	cAttackResultData()
	{
		Init();

	}

	void Init()
	{

		m_TargetObjectKey = INVALID_OBJECT_KEY;

		m_CurrentPosition.wvPos = c_ZeroVector;
		m_CurrentPosition.wTile = -1;
		m_DestPosition.wvPos = c_ZeroVector;
		m_DestPosition.wTile = -1;

		m_IsHeal = FALSE;
		m_IsMiss = FALSE;
		m_IsNoDamage = FALSE;
		m_ForceSetLatestAttack = FALSE;
		m_Damage = 0;

		m_TargetHP = 0;
		m_AdditionalEffect = 0;
		m_ComboCount = 0;
		m_AirTime = 0;
		m_GetupTime = 0;
		m_EndDownTime = 0;
		m_fForce = 0.0f;

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        m_TargetSD = 0;
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
	}

	void SetTargetObjectKey( DWORD TargetObjectKey ) { m_TargetObjectKey = TargetObjectKey; }
	void SetTargetHP( DWORD TargetHP ) { m_TargetHP = TargetHP; }
	void SetDamage( DWORD TargetDamage ) { m_Damage = TargetDamage; }
	void SetMiss( BOOL IsMiss ) { m_IsMiss = IsMiss; }

	void SetAdditionalEffect( DWORD Effect ) { m_AdditionalEffect = Effect; }
	void AddAdditionalEffect( DWORD Effect ) { m_AdditionalEffect |= Effect; }
	BOOL CheckAdditionalEffect( DWORD Effect) { return m_AdditionalEffect & Effect ? TRUE : FALSE; }

	void SetHeal( BOOL IsHeal ) { m_IsHeal = IsHeal; }
	void SetNoDamage( BOOL IsNoDamage ) { m_IsNoDamage = IsNoDamage; }
	void SetComboCount( DWORD ComboCount ) { m_ComboCount = ComboCount; }
	void SetForceSetLastestAttack( BOOL ForceSetLastestAttack ) { m_ForceSetLatestAttack = ForceSetLastestAttack; }

	void SetPosition( const WzVector& CurrentPosition, const WzVector& DestPosition ) 
	{
		m_CurrentPosition.wvPos = CurrentPosition; m_DestPosition.wvPos = DestPosition; 
		m_CurrentPosition.wTile = m_DestPosition.wTile = -1;
	}

	DWORD GetTargetObjectKey() { return m_TargetObjectKey; }
	BOOL IsMiss() { return m_IsMiss; }
	BOOL IsForceSetLastestAttack() { return m_ForceSetLatestAttack; }
	BOOL IsHeal() { return m_IsHeal; }
	BOOL IsNoDamage() { return m_IsNoDamage; }
	BOOL IsDead() { return m_TargetHP <= 0; }
	BOOL IsLive() { return m_TargetHP > 0; }
	DWORD GetComboCount() { return m_ComboCount; }

	DWORD GetTargetHP() { return m_TargetHP; }
	DWORD GetDamage() { return m_Damage; }
	DWORD GetAdditionalEffect() { return m_AdditionalEffect; }

	const HONGPOS& GetCurrentPosition() const { return m_CurrentPosition; }
	const HONGPOS& GetDestPosition() const { return m_DestPosition; }

	// 상태이상 처리를 위한 기타 변수
	DWORD GetEndDownTime() { return m_EndDownTime; }
	DWORD GetGetUpTime() { return m_GetupTime; }
	DWORD GetAirTime() { return m_AirTime; }
	void SetEndDownTime( DWORD EndTime ) { m_EndDownTime = EndTime; }

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    void SetTargetSD(DWORD TargetSD) { m_TargetSD = TargetSD; }
    DWORD GetTargetSD() { return m_TargetSD; }
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

private:
	DWORD		m_TargetObjectKey;

	HONGPOS		m_CurrentPosition;
	HONGPOS		m_DestPosition;

	BOOL		m_IsHeal;
	BOOL		m_IsMiss;
	BOOL        m_IsNoDamage;
	BOOL        m_ForceSetLatestAttack;

	DAMAGETYPE	m_Damage;
	DWORD		m_TargetHP;
	DWORD		m_AdditionalEffect;
	DWORD		m_ComboCount;

	DWORD		m_AirTime;
	DWORD		m_GetupTime;
	DWORD       m_EndDownTime;
	float		m_fForce;

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    DWORD       m_TargetSD;
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ATTACK_RESULT
{
	DWORD m_SkillCode;
	DWORD m_AttackSerial;
	DWORD m_TimeSerial;
	DWORD m_GroupID;
	eWEAPONSOUNDKIND m_WeaponSound;
	cAttackResultData m_TargetData;

public:
	ATTACK_RESULT() 
	{
		m_SkillCode = INVALID_SKILL_CODE; // 0
		m_AttackSerial = 0;
		m_TimeSerial = 0;
		m_GroupID = 0;
		m_WeaponSound = eWEAPONSOUNDKIND_MAX;
	}

	cAttackResultData& GetTargetData() { return m_TargetData; }

	void SetSkillCode( DWORD SkillCode ) { m_SkillCode = SkillCode; }
	void SetGroupID( DWORD GroupID ) { m_GroupID = GroupID; }
	void SetAttackSerial( DWORD AttackSerial ) { m_AttackSerial = AttackSerial; }
	void SetTimeSerial( DWORD TimeSerial ) {m_TimeSerial = TimeSerial; }
	void SetWeaponSound( eWEAPONSOUNDKIND Kind ) { m_WeaponSound = Kind; }

	DWORD GetSkillCode() { return m_SkillCode; }
	DWORD GetAttackSerial() { return m_AttackSerial; }
	DWORD GetTimeSerial() { return m_TimeSerial; }
	DWORD GetGroupID() { return m_GroupID; }
	eWEAPONSOUNDKIND GetWeaponSound() { return m_WeaponSound; }
};


typedef STLX_LIST<ATTACK_RESULT> ATTACK_RESULT_LIST; 

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//데미지 파라미터
typedef struct _DAMAGE_PRAM
{
	_DAMAGE_PRAM(ATTACK_RESULT* pResultPram,BOOL bRightPram,DWORD dwObjKeyPram)
		:pResult(pResultPram),bRight(bRightPram),dwObjKey(dwObjKeyPram)
	{
	}

	ATTACK_RESULT *pResult;
	BOOL		   bRight;
	DWORD		   dwObjKey;

}DAMAGE_PRAM;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _HPINFO
{
	PLAYERHP m_HP;
	PLAYERHP m_MaxHP;
	DWORD    m_LV;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    PLAYERSD m_SD;
    PLAYERSD m_MaxSD;
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
} HPINFO;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StatusEffect.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _ATTACK_JUMP_INFO
{
	DWORD		m_dwTotalTime;		// 목적지까지 도착하는데 걸리는 전체 시간
	float		m_fDestHeight;		// 목적지 높이
	WzVector	m_vDist;			// 목적지 좌표와 현재 좌표간의 차이
	float		m_fVelocity_X;		// 점프를 시작할 때의 초기 속도
	float		m_fVelocity_Y; 
} ATTACK_JUMP_INFO;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 나중에 union으로
typedef struct _PLAYER_ACTION
{
	eActionID ActionID;	
	DWORD dwTimeSerial;

	union
	{
		struct 
		{
			WzVector	vCurPos;
		} IDLE;

		struct 
		{			
			BOOL		bThrust;
			BYTE		byState;
			HONGPOS		vCurPos;
			HONGPOS		vDestPos;
		} MOVE;

		struct 
		{
			HONGPOS		vKeyboardCurPos;
			float		fKeyboardAngle;	
			BYTE		byKeyboardState;
		} KEYBOARDMOVE;

		struct 
		{
			DWORD		dwAttackSerial;	
			DWORD		dwAttackStyle;
			BYTE		byAttackType;
			DWORD		dwTargetID;
			BOOL		bAttackMove;
			HONGPOS		vCurPos;
			HONGPOS		vDestPos;
			BOOL        bForceAttack;
            //_NA003109_STYLE_PRESENTATION_IMPROVEMENT
            uint8_t     style_presentation;
		} ATTACK;

		struct
		{
			DWORD		dwEmoteID;
			WzVector	vEmoteCurPos;
		} EMOTE;

		struct
		{
			DWORD		dwAttackSerial;
			SLOTCODE	dwSkillID;
			DWORD		dwTargetID;			
			HONGPOS		vCurPos;
			HONGPOS		vDestPos;
			WzVector    vTargePos;//발사체 타겟 포스
			BOOL        bForceAttack;
		} SKILL;

		//struct
		//{
		//	float		fInitialSpeed;
		//	HONGPOS		vCurPos;
		//	HONGPOS		vDestPos;
		//	DWORD       dwAttackId;
		//	DWORD		dwEndTime; // 반드시 이시간안에 동작이 끝나야함
		//} KNOCKBACK;

		struct
		{			
			float		fInitialSpeed;
			DWORD		dwDelay;
			HONGPOS		vCurPos;
			HONGPOS		vDestPos;
			BOOL		bFly;
			DWORD		dwEndTime;   // 반드시 이시간안에 동작이 끝나야함
			DWORD		dwGetupTime; // 이시간에 일어남.
		} DOWN;

		struct
		{
			float		fDownForceLimit;
			float		fGravity;
			float		fHeightLimit;
			float		fUpperForce;
			DWORD		dwAirTime;     // 떠있을시간
			DWORD		dwGetupTime;   // 일어날시간
			BOOL        bForceDown;   // 최대 높이 까지 가면 강제로 내릴것인가.
		} TOAIR;

		struct
		{
			DWORD		dwEndTime;
		} GETUP;

		struct
		{
			DWORD		dwEndTime;
		} SPAWN;

		struct
		{
			DWORD		dwNPCKey;
		} NPCMEET;

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		struct
		{
			DWORD       dwStatuePlayerKey;
		} STATUEMEET;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

		struct 
		{
			DWORD		dwItemKey;
		} GETITEM;

		struct 
		{
			HONGPOS		vCurPos;
			HONGPOS		vDestPos;
			int			iDestTileNum;
			DWORD		dwStartTime;
			DWORD		dwEndTime;
		} EVENT_JUMP;

		struct 
		{
			DWORD		dwAttackSerial;	
			DWORD		dwTargetID;
			HONGPOS		vCurPos;
			HONGPOS		vDestPos;
			DWORD		dwStartTime;
			DWORD		dwEndTime;
		} ATTACKJUMP;

		struct 
		{
			HONGPOS		vCurPos;
		} STOP;

		struct 
		{
			DWORD    dwTargetID;
		} VALKYRIESHOT;

		struct 
		{
			DWORD    dwTargetID;
		} VENDORMEET;

		struct 
		{
			DWORD    dwTargetID;
		} PLAYERMEET;

		struct 
		{
			DWORD    dwTargetID;
		} USEOBJECT;

		struct 
		{
			HONGPOS		vCurPos;
			float		fAngle;	
			BYTE		bType;
		} SPECIALMOVE;


		struct 
		{
			WzVector	vDest;
			int         iJumpFlag;
		} KEYBAORDJUMP;

		struct 
		{
			WzVector	vDest;
		} TELEPORT;

		struct 
		{
			DWORD target_object;
		} AIRSHIP;

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        struct 
        {
            DWORD target_object;
        } BATTLEGROUND_RESURRECTION;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

		struct 
		{
			DWORD    dwTargetID;
		} WAYPOINTMEET;

		struct 
		{
			RESOURCECODE		rcCode;
			eTRANSFORM_TYPE     transFormType;
			eTRANSFORM_PROCESS  transFormProcess;
			BYTE				appearKind;
		} TRANSFORM;

		struct
		{
			WzVector	vSrcRot;	
			WzVector	vDestRot;
			DWORD       dwRotInterporateTime;
			int         iHp;

		} FALLDOWN;

		struct 
		{
			DWORD		dwItemCode;
			WzVector	vTargetPos;
		} FIRECRACKER;

		struct 
		{
			DWORD    dwTargetID;
		} COLLECTOBJECT;

		struct 
		{
			POSTYPE	ItemPos;
		} RIDEROBJECT;


		struct 
		{
			DWORD    dwTargetID;
		} DOMINATIONOBJECT;

		struct 
		{
			POSTYPE	slot_pos;           //extract item    
			POSTYPE	etherealizer_pos;   //use item
		}ETHERIA;
		struct 
		{
			WzID start_animation_id;    // 시작 애니메이션ID
			WzID next_animation_id;     // 다음 애니메이션ID
			BYTE start_animation_loop;  // 시작 애니메이션의 루프여부
			BYTE next_animation_loop;   // 다음 애니메이션의 루프여부
		}TRIGGERANIMATIONCONTROL;
	};

}PLAYER_ACTION;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _ITEM_EFFECT_PARAM
{
    int				    iBoneIndex;
    WzVector		    vScale;
    HANDLE			    hEffect;
    eSKILLEFFECT_POS    bPositionType;
    BOOL			    bRotateWithBone;
    DWORD               dwItemCode;
    _ITEM_EFFECT_PARAM()
    {
        hEffect = INVALID_HANDLE_VALUE;
    }
} ITEM_EFFECT_PARAM;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum PLAYER_VARIATION_PART
{
    PLAYER_VARIATION_HAIR = 1,
    PLAYER_VARIATION_FACE = 2,
    PLAYER_VARIATION_MAX = 3

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum eKIND_OF_EQUIPED_SETITEM;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct sAttachEffectForSetItem
{
    eKIND_OF_EQUIPED_SETITEM	eSetType;
    HANDLE						m_hEffectHandle;
    BYTE						m_btBoneIdx;
    WzID                        m_EffectID;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct sFullSetEffectInfo
{
    DWORD	m_dwEffectID;
    BYTE	m_btBoneIdx;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct sFullSetChangeItemInfo
{
    BOOL	     m_IsFullSetItem;
    BOOL	     m_bAlreadyLoad;
    DWORD	     m_dwItemCode;
    RESOURCECODE m_SkinCode;
    RESOURCECODE m_NewSkinCode;
    int          m_SubType;
    POSTYPE	     m_PartIndex;        //m_ppRenderPart의 인덱스
    CWzUnitDraw* m_pChangePart;
};

typedef ::std::queue<PLAYER_ACTION, STLX_DEQUE<PLAYER_ACTION> > PLAYER_ACTION_QUEUE;


