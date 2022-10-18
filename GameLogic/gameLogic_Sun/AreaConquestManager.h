#pragma once

#ifndef __AREACONQUESTMANAGER_H__
#define __AREACONQUESTMANAGER_H__

#pragma warning (disable:4786 4244 4101)
#include "../lib/XMLParser/MarkupArchive.h"
using namespace markup;

#pragma warning (disable:4786)
#include <map>
#include <string>
using namespace std;


#ifndef C_NA_0_20100520_ACSYSTEM_CHANGED

#include "AreaConquest.h"
//#include "QuestEx.h"
#include <MissionManager.h>



const int MAX_AC_DIALOG_REWARD_MAX = 20;
const int MAX_AC_DIALOG_ITEM_MAX = 9;
const int MAX_AC_DIALOG_LUCKY_MONSTER_MAX = 4;

const int AC_DIALOG_TEXT_FORMAT_LENGTH = 64;

typedef struct _CONDITION_TEXT
{
	int  iCondition;
	int  iValue;
	int  iMaxValue;
	BOOL bText;

} CONDITION_TEXT;

typedef struct _CONDITION_TEXT2
{
	int  iType;
	int  iCondition;
	int  iValue;
	int  iMaxValue;
	BOOL bText;

} CONDITION_TEXT2;


enum eAC_COLLECTION_CATEGORY
{
	eACC_NONE = 0,
	eACC_STORY,
	eACC_HIDDEN,
	eACC_TREASURE,
	eACC_SPECIAL,
	MAX_AC_COLLECTION_CATEGORY
};

enum eAC_QUEST_CATEGORY
{
	eACQ_NORMAL = 0,
	eACQ_MISSION,
	eACQ_ELITE,
	eACQ_HIDDEN,
	MAX_AC_QUEST_CATEGORY
};

enum eAC_BATTLE_CATEGORY
{
	eACB_COMPETITIVE_HUNTING = 0,
	eACB_HUNTING_EVENT,
	eACB_PVP_POINT,
	eACB_UNLIMITED_ZONE,
	eACB_GUIDANCE_HERO,	
	eACB_ITEM_MASTER,
	eACB_MONSTER_MASTER,
	MAX_AC_BATTLE_CATEGORY

};

enum eAC_MISSION_CATEGORY
{
	eACM_MAIN = 0,
	eACM_SUB,
	eACM_HIDDEN,	
	MAX_AC_MISSION_CATEGORY
};

class QuestManager2;
class QuestInfoManager2;

//////////////////////////////////////////////////////////////////////////
//#ifdef BUILD_KOREA_061211
#define MAX_AC_MAP	5 // for cn
//////////////////////////////////////////////////////////////////////////

class AreaConquestManager : public AreaConquest
{
public:
	AreaConquestManager();
	~AreaConquestManager();

	//	void ApplyMissionData(BYTE * pStream, WORD wSize);
//	void ApplyQuestData(BYTE * pStream, WORD wSize);

	BOOL CompileACStream(BSTR bstrXml);

	void UpdateQuestInfo(QuestManager2* pQuestManager, BOOL bUpdate);
	void UpdateMissionInfo(MissionManager* pMissionMgr, BOOL bUpdate);
	
	void CheckRewardFlag();

	void Init();
	void Release();

	//////////////////////////////////////////////////
	bool    SetAreaIndex(int index);

	void    UpdateAll(BSTR* bstrXml);

	void    UpdateTreasure(); // 보물
	void    UpdateReward();   // 보상
	void    UpdateHunting();  // 사냥
	void    UpdateCollection(); // 수집품
	void    UpdateBoss();       // 보스
	void    UpdateTowerDefense(); // 타워디펜스
	void    UpdateQuest(); // 퀘스트
	void    UpdateBattle(); // 배틀
	void    UpdateMission(); // 미션
	void    UpdateEtc(); // 기타등등	

	CONDITION_TEXT  GetLuckyMonsterConditionIndex(int offset, bool bHidden);
	CONDITION_TEXT2 GetItemConditionIndex(int offset);

	///////////////////////////////////////////////

	int    m_iAreaIndex;

	float  m_fBattleGaugeSize;
	
	TCHAR  m_szRewardFormat[AC_DIALOG_TEXT_FORMAT_LENGTH];
	TCHAR  m_szAreaFormat[AC_DIALOG_TEXT_FORMAT_LENGTH];

	int    m_iLastRewardSendIndex;

	int    m_iMaxLuckyMonsterLineCount;
	int    m_iMaxStoryItemLineCount;
	int    m_iHiddenItemLineCount;
	int    m_iSpecialItem;
	int    m_iTreasureItem;

private:
	
	CMarkupArchive xml;
};

#endif //C_NA_0_20100520_ACSYSTEM_CHANGED
#endif //__AREACONQUESTMANAGER_H__