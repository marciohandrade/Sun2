#pragma once
//=======================================================================================================================
/// RewardInfoList class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 4. 7
	@remarks
			- 보상 정보 스크립트 파서
	@purpose
			- 보상 처리 통합용 스크립트 ( 이벤트, 미션, 경쟁헌팅, 퀘스트 )
	@history
			- 
*/
//=======================================================================================================================
#include <SolarHashTable.h>
#include <hash_map>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

class RewardInfo
{
public:
	enum 
    { 
        MAX_DESC_LENGTH = 128,
        MAX_DROP_INDEX_NUM = 5, // increased by _NA_0_20101130_REWARD_ITEM_COUNT_INCREASE
    };

	enum 
    { 
        MIN_ITEM_TYPECODE = 10000 
    };
	
    // 선태과정이 있다.
	// RefType = 0 : 아이템코드, TypeCode => ItemCode, SelectNum만큼만 준다.
	// RefType = 1 : 랜덤방식, TypeCode => DropIndex, SelectNum만큼만 준다.
	// 선태과정이 없다 : SeletNum은 사용하지 않는다.
	// RefType = 2 : 아이템코드
	// RefType = 3 : 랜덤방식
    // RefType = 4 : 기간제 설정방식
	enum 
    { 
        REF_TYPE_SELECT = 0, 
        REF_TYPE_RANDOM = 1, 
        REF_TYPE_EVENT_ITEMCODE = 2, 
        REF_TYPE_EVENT_DROPINFO = 3,
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        REF_TYPE_DATETIME = 4,
        REF_TYPE_MAX
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    };

	enum
	{
		ITEM1_SELECTED = (1<<0),
		ITEM2_SELECTED = (1<<1),
		ITEM3_SELECTED = (1<<2),
		ITEM4_SELECTED = (1<<3),
        ITEM5_SELECTED = (1<<4), //_NA_0_20101130_REWARD_ITEM_COUNT_INCREASE
	};

	__PROPERTY( DWORD, Index );
	__PROPERTY( DWORD, DCode );
	__PROPERTY( EXPTYPE, Exp );
	__PROPERTY( MONEY, Hime );
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    __PROPERTY( WORD, Coin );
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    __PROPERTY( WORD, Title );
    __PROPERTY( WORD, ReputeCode );
    __PROPERTY( WORD, ReputePoint );
#endif //_NA_003027_20111013_HONOR_SYSTEM
	__PROPERTY( DWORD, ChaoTime );
	__PROPERTY( SLOTCODE, ItemCode );
	__PROPERTY( DWORD, ItemTypeIndex );
	__PROPERTY( WORD, SkillCode );
	__PROPERTY( BYTE, SelectNum );
	__PROPERTY( BYTE, RefType );
	__ARRAY_PROPERTY( DWORD, MAX_DROP_INDEX_NUM, TypeCode );
	__ARRAY_PROPERTY( DWORD, MAX_DROP_INDEX_NUM, TypeIndex );	//< ItemTypeIndex : IsOverlaped()이면 개수
																//< REF_TYPE_RANDOM, REF_TYPE_EVENT_DROPINFO : TypeIndex사용안함
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    __ARRAY_PROPERTY( DWORD, MAX_DROP_INDEX_NUM, TimeLimit);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
	__PROPERTY( WORD, StringCode ); 
#ifdef _NA_20100307_BETAKEY_SYSTEM
    __PROPERTY( INT32, BetaKey );
#endif

public:	
    CHAR    m_pszDesc[MAX_DESC_LENGTH];
};



class RewardInfoList : public util::Singleton<RewardInfoList>, public IParser
{
private:	
    typedef stdext::hash_map<DWORD, RewardInfo*>	REWARD_HASH;
	typedef std::pair<DWORD, RewardInfo*>			REWARD_HASH_PAIR;

public:
    RewardInfoList(VOID);
	~RewardInfoList(VOID);

	VOID Unload();
	VOID Init( CHAR* pszPackFile);
    virtual	VOID	Release();
	BOOL Load( CHAR* pszFileName, BOOL bReload = FALSE );
	VOID Reload();

	RewardInfo*	FindRewardInfo( DWORD Index );

    BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	BOOL _Load( BOOL bReload );
	BOOL IsExistItemInScript(SLOTCODE IN ItemCode, DWORD dwItemTypeIndex);

private:
    REWARD_HASH	m_RewardInfoHash;
	CHAR		m_pszFileName[MAX_PATH];
};
