#pragma once

//---------------------------------------------------------------------------------------------
// will be deprecated!!!!!!!!!
//---------------------------------------------------------------------------------------------
//#include <SolarHashTable.h>
#include <Singleton.h>
//#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>


using namespace util;


#ifndef MAKEDWORD
#define MAKEDWORD(a, b)		((DWORD)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#endif

#define REWARD_DESC_LENGTH 64

#define REWARD_ARRAY_SIZE  eCHAR_TYPE_MAX


//struct REWARD1_ITEM_INFO
//{
//	enum { MAX_REWARD1_ITEM_NUM=4, };
//	BYTE		m_bySelectableRewardItemNum;
//	BYTE		m_bRewardItemCode;						// 보상 아이템코드(1: 디바인, 0: 일반 아이템)
//	SLOTCODE	m_RewardItemCode[MAX_REWARD1_ITEM_NUM];
//	BYTE		m_Quantity[MAX_REWARD1_ITEM_NUM];
//
//	MONEY		m_RewardMoney;
//	DWORD		m_RewardEXP;
//	DWORD		m_RewardSpecial;
//};
//
//struct REWARD2_ITEM_INFO
//{
//	enum { MAX_REWARD2_ITEM_NUM=2, };
//	SLOTCODE	m_RewardItemCode[MAX_REWARD2_ITEM_NUM];
//	BYTE		m_Quantity[MAX_REWARD2_ITEM_NUM];
//	MONEY		m_RewardMoney;
//	DWORD		m_RewardEXP;
//};
//
//struct RANDOM_REWARD_ITEM_INFO
//{
//	enum { MAX_RANDOM_REWARD_ITEM_NUM=3, };
//	SLOTCODE	m_RandomRewradItemCode[MAX_RANDOM_REWARD_ITEM_NUM];	// 랜덤한 아이템
//	BYTE		m_byDropRewardrate[MAX_RANDOM_REWARD_ITEM_NUM];		// 아이템 드롭 확률(소수 아님. 퍼센티지)
//};
//
//struct BASE_MissionReward 
//{
//	DWORD				m_Num;
//	char				m_pszDesc[REWARD_DESC_LENGTH];
//	BYTE				bMcode;									// 미션 코드(0:메인 미션, 1, 2: 서브 미션)
//	REWARD1_ITEM_INFO	m_Item1Info[REWARD_ARRAY_SIZE];
//	REWARD2_ITEM_INFO	m_Item2Info[REWARD_ARRAY_SIZE];
//	RANDOM_REWARD_ITEM_INFO	m_RandItemInfo[REWARD_ARRAY_SIZE];	// 랜덤하게 나오는 아이템
//};

struct MissionRewardInfo
{
	static const DWORD	CLASS_CODE_LOWER = 0;
	static const DWORD	CLASS_CODE_UPPER = eCHAR_TYPE_MAX;
	static const DWORD	MISSION_REWARD_MAX_SIZE = 3;
	BYTE			m_byMissionNo;
	BYTE			GetMissionNo() { return m_byMissionNo; }

	struct sCLASS	// 클래스 코드, 캐릭터 종류
	{
		BYTE			m_bClassCode;
		struct sREWARD	// 미션 완료 회수에 따른 보상 체계 정보 3단계
		{
			BYTE			m_bMissionNum;
			WORD			m_wRewardCode;
		} REWARDs[MISSION_REWARD_MAX_SIZE];
	} CLASSs[CLASS_CODE_UPPER];	// index(0)은 사용하지 않는다. 인덱스 차이로 인한 문제발생을 피하는 편이 더 나을 듯.

	CHAR				m_pszDesc[REWARD_DESC_LENGTH];
};



class MissionRewardParser : public Singleton<MissionRewardParser>, public IParser
{
public:
	MissionRewardParser();
	~MissionRewardParser();


	VOID           Init(DWORD dwPoolSize);
	VOID		   Init(DWORD dwPoolSize, char * pszPackFileName);
	VOID           Release();
	BOOL           Load(char *pszFilename, BOOL bReload = FALSE);
	VOID		   Reload();
	BOOL		   _LoadEx( eSCRIPT_CODE ScriptCode, util::SolarFileReader& sr, BOOL bReload ){return TRUE;}
	BOOL		  LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload ) {return TRUE;};
	MissionRewardInfo * GetMissionReward( BYTE byMissionNo );

private:
	char		   m_pszFileName[MAX_PATH];

	VOID		   Unload();

	

	// KEY: "미션 번호", "미션 코드"의 조합 -- MAKEDWORD(dwMissionNum, bMcode)
	util::SolarHashTable<MissionRewardInfo *>		*m_pDataTable;
};

