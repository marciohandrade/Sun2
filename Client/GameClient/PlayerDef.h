#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define MAX_PLAYER_ATTACHMENT_COUNT 5
#define CHAR_DEL_DAYS_OVER_40_LEVEL		7


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _STAT_BOOST
{
	int m_iStr; // 힘
	int m_iDex; // 민첩
	int m_iVit; // 체력
	int m_iSpr; // 정신력
	int m_iInt; // 지력

	int m_iPhyAttackRate; // 물리공격 성공률
	int m_iPhyAvoidRate;  // 물리공격 회피율

} STAT_BOOST;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum PLAYER_MAKE_TYPE
{
	Player_Make_Type_Member = 0,	// 일반 유저
	Player_Make_Type_Master,		// 방장
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	에테르 관련 구조체
typedef struct _ETHER_INFO
{
	_ETHER_INFO():byEtherChargePos(0), bChange(0), byEtherIndex(0), ChangePos(0), bWait(0), iValue(0)
	{
	}
	POSTYPE		byEtherChargePos;
	BYTE		byEtherIndex;
	int  		iValue;

	//자동장전
	BOOL		bChange;			
	POSTYPE		ChangePos;
	BOOL		bWait;

	//operator
	_ETHER_INFO& operator=(const _ETHER_INFO& rhs)
	{
		this->byEtherChargePos = rhs.byEtherChargePos;
		this->iValue		   = rhs.iValue;
		this->bChange		   = rhs.bChange;
		this->ChangePos		   = rhs.ChangePos;
		this->bWait			   = rhs.bWait;
		this->byEtherIndex	   = rhs.byEtherIndex;
		return *this;
	}
}ETHER_INFO;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	펫 관련 구조체
typedef struct _PET_INFO
{
	_PET_INFO()
		: m_bPetSummoned(FALSE)
		, m_dwPetObjKey(0)
		, m_wPetIndex(0)
	{
	}

	DWORD	m_dwPetObjKey;
	BYTE	m_bPetSummoned;
	WORD	m_wPetIndex;			

	union
	{
		struct
		{
			POSTYPE	m_PetItemPos;			//Hero에만쓰임 
			POSTYPE	m_PetChangePos;			//Hero에만쓰임 
			BOOL	m_bChangePet;			//Hero에만쓰임 

		}HERO_;

		struct
		{
			BYTE	m_byPetFullness;		//Player만쓰임	
			BYTE	m_byPetIntimacy;		//Player만쓰임
			BYTE	m_byPetName;
			TCHAR	m_sPetName[MAX_PET_NAME_LENGTH+1];
		}PLAYER_;
	};

} PET_INFO;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//하이드스킬관련 파라미터구조체
typedef struct _HIDE_SKILL_INFO
{
	enum 
	{
		NONE_HIDE =0,          
		ALL_HIDE =1,          
		HALF_HIDE =2,           
	};

	_HIDE_SKILL_INFO()
		: fRefractRate(1.f)
		, byHideState(0)
		, byHideReserve(0)
		, fRefractAlpha(1.f)
	{
	}

	BYTE  byHideState;
	BYTE  byHideReserve;
	float fRefractRate;
	float fRefractAlpha;
	ENUM_ALTERNATIVE_RENDER eRenderType;
	ENUM_ALPHABLEND	eBlendMode;

} HIDE_SKILL_INFO;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// struct GUILD_RENDER_INFO 의 구조가 바뀌면서 해당 구조체를 멤버로 가지기에는 사이즈가 너무 커짐
// struct GUILD_RENDER_INFO 의 멤버 m_stateTotalInfo 의 사이즈가 1025 바이트
// 클라에서 사용할 구조체 
#pragma pack(push, 1)

struct PLAYER_GUILD_RENDER_INFO
{
	PLAYER_GUILD_RENDER_INFO()
	{
		ZeroMemory(this, sizeof(PLAYER_GUILD_RENDER_INFO));
	}
	DWORD m_dwObjectKey;
	GUILDGUID m_GuildGuid;
	GUILDMARKIDX m_PatternIndex;
	GUILDMARKIDX m_BackGroundIndex;
	eGUILD_RELATION m_RelationType;

	char m_tszGuildName[MAX_GUILDNAME_LENGTH+1];    // NULL포함

#ifdef _NA_008334_20150608_SONNENSCHEIN
    BYTE sonnenschein_phase;
#endif //_NA_008334_20150608_SONNENSCHEIN

	BYTE m_GuildGrade;                              // eGUILD_GRADE
	BYTE m_GuildDuty;                               // eGUILD_DUTY

	bool IsExist() const { return ( m_GuildGuid != 0); }
};

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
struct CLIENT_PLAYER_RANKING_GRADE_INFO
{
	CLIENT_PLAYER_RANKING_GRADE_INFO()
	{
		ZeroMemory(this, sizeof(CLIENT_PLAYER_RANKING_GRADE_INFO));
	}
	eSUN_RANKING_TYPE ranking_type;
	eSUN_RANKING_GRADE_TYPE grade;

	bool IsExist() const { return (grade != eSUN_RANKING_GRADE_NONE); }
};
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
#pragma pack(pop)
// GUILD_RENDER_INFO 구조체가 이후 변경이 되면 변경사항을 같이 적용할 필요가 있다.
BOOST_STATIC_ASSERT(sizeof(PLAYER_GUILD_RENDER_INFO) == (sizeof(GUILD_RENDER_INFO) - sizeof(DOMINATION_STATE_TOTAL_INFO)));

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// -_-
#define INVALID_TEAM		(-1)

struct ClassCodeEx
{
    union 
    {
        struct
        {
            BYTE class_type;
            BYTE gender;
        };
        DWORD class_code;
    };

    ClassCodeEx() :class_code(0) {}
    ClassCodeEx(BYTE _class_type, BYTE _gender) {class_type = _class_type; gender = _gender; }
};
