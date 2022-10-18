#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define MAX_PLAYER_ATTACHMENT_COUNT 5
#define CHAR_DEL_DAYS_OVER_40_LEVEL		7


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _STAT_BOOST
{
	int m_iStr; // ��
	int m_iDex; // ��ø
	int m_iVit; // ü��
	int m_iSpr; // ���ŷ�
	int m_iInt; // ����

	int m_iPhyAttackRate; // �������� ������
	int m_iPhyAvoidRate;  // �������� ȸ����

} STAT_BOOST;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum PLAYER_MAKE_TYPE
{
	Player_Make_Type_Member = 0,	// �Ϲ� ����
	Player_Make_Type_Master,		// ����
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���׸� ���� ����ü
typedef struct _ETHER_INFO
{
	_ETHER_INFO():byEtherChargePos(0), bChange(0), byEtherIndex(0), ChangePos(0), bWait(0), iValue(0)
	{
	}
	POSTYPE		byEtherChargePos;
	BYTE		byEtherIndex;
	int  		iValue;

	//�ڵ�����
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
//	�� ���� ����ü
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
			POSTYPE	m_PetItemPos;			//Hero�������� 
			POSTYPE	m_PetChangePos;			//Hero�������� 
			BOOL	m_bChangePet;			//Hero�������� 

		}HERO_;

		struct
		{
			BYTE	m_byPetFullness;		//Player������	
			BYTE	m_byPetIntimacy;		//Player������
			BYTE	m_byPetName;
			TCHAR	m_sPetName[MAX_PET_NAME_LENGTH+1];
		}PLAYER_;
	};

} PET_INFO;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���̵彺ų���� �Ķ���ͱ���ü
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
// struct GUILD_RENDER_INFO �� ������ �ٲ�鼭 �ش� ����ü�� ����� �����⿡�� ����� �ʹ� Ŀ��
// struct GUILD_RENDER_INFO �� ��� m_stateTotalInfo �� ����� 1025 ����Ʈ
// Ŭ�󿡼� ����� ����ü 
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

	char m_tszGuildName[MAX_GUILDNAME_LENGTH+1];    // NULL����

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
// GUILD_RENDER_INFO ����ü�� ���� ������ �Ǹ� ��������� ���� ������ �ʿ䰡 �ִ�.
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
