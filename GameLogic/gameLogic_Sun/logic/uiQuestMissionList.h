#ifndef     __UIQUESTMISSIONLIST_H__
#define     __UIQUESTMISSIONLIST_H__


#define SET_ARRAY_VECTOR(fOutArrVec, x, y, z)		{ fOutArrVec[0] = x; fOutArrVec[1] = y; fOutArrVec[2] = z; }

//---------------------------------------------------------------------------------------
//		스트링 관련
//---------------------------------------------------------------------------------------
#define		StrnCopy				_tcsncpy
#define		StrCopy					_tcscpy
#define		Strcmp					_tcscmp
// unicode 관련
#ifdef Sprintf
#undef Sprintf
#endif
#define	Sprintf						_stprintf
#define Snprintf					_sntprintf


#ifndef C_NA_0_20100520_ACSYSTEM_CHANGED
//------------------------------------------------------------------------------
/**
    @class uiQuestMissionList

    미션 정보 리스트

    (C) 2005 y2jinc 
*/

#include "MapInfoParser.h"



//-----------------------------------------------------------
#define TYPEDEF_MAP(Name, KEY_TYPE, DATA_TYPE)			\
	typedef std::map<KEY_TYPE, DATA_TYPE>	Name##_MAP;	\
	typedef Name##_MAP::iterator			Name##_ITR;	\
	typedef pair<KEY_TYPE, DATA_TYPE>		Name##_PAIR; 


class uiQuestMissionList
{
	enum eDIALOG_POS
	{
		eDLGPOS_CHK_MISSION,
		eDLGPOS_CHK_QUEST,	
		eDLGPOS_LST_MISSION_LIST,
		eDLGPOS_BTN_SELECT_MISSION,
		eDLGPOS_BTN_EXIT,
		eDLGPOS_BTN_MAIN,
		eDLGPOS_BTN_SUB1,
		eDLGPOS_BTN_SUB2,
		eDLGPOS_BTN_SUB3,
		eDLGPOS_TXT_REWARD_1,
		eDLGPOS_TXT_REWARD_2,
		eDLGPOS_TXT_REWARD_3,

		DIALOG_MAX,
	};

	enum
	{
		QUEST_CATEGORY_BEGIN= eDLGPOS_CHK_MISSION,
		QUEST_CATEGORY_SIZE	= eDLGPOS_CHK_QUEST-QUEST_CATEGORY_BEGIN+1,    

		BEGIN_MISSION_TYPE = eDLGPOS_BTN_MAIN,
		MAX_MISSION_TYPE = eDLGPOS_BTN_SUB3-eDLGPOS_BTN_MAIN+1,

		BEGIN_REWARD = eDLGPOS_TXT_REWARD_1,
		MAX_REWARD_COUNT = eDLGPOS_TXT_REWARD_3-BEGIN_REWARD+1,
	};

public:
	uiQuestMissionList();
	virtual ~uiQuestMissionList();

	TYPEDEF_MAP(MISSION_GROUP, CODETYPE, MapGroup * );

	//--------------------------------------------------------------------------
	// functions
	//
public:
	void ClearInfo();

	// 메니저에서 공동으로 호출하는 함수.
	void UpdateMissionInfo(MissionManager* pMissionMgr, BSTR* bstrXml);	

protected:

	void					initMissionGroupMap();
	
	//--------------------------------------------------------------------------
	// member vars
private:
	TCHAR					m_szText[INTERFACE_STRING_LENGTH];
	TCHAR					m_szResult[INTERFACE_STRING_LENGTH];

	MISSION_GROUP_MAP		m_MissionMapGroupMap;
	
	CMarkupArchive xml;
};
#endif //#ifndef C_NA_0_20100520_ACSYSTEM_CHANGED
//------------------------------------------------------------------------------
#endif  //  __UIQUESTMISSIONLIST_H__