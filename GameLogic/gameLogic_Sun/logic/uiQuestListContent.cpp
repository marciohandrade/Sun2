//------------------------------------------------------------------------------
//  uiQuestListContent.cpp
//  (C) 2005 SUN ONLINE 
//------------------------------------------------------------------------------


#include "stdafx.h"
#include <assert.h>

#include "util/util.h"
#include "QuestEx.h"
#include <Struct.h>
#include <SolarFileReader.h>
#include "CSunOnline.h"
#include "logic.h"

#include "SCItemSlot.h"
#include "uiQuestListContent.h"

#pragma warning (disable:4786 4244 4101)

#include <map>
#include <string>
using namespace std;

#include "../lib/XMLParser/MarkupArchive.h"
using namespace markup;


extern QuestInfoManager2* g_pQuestInfoManager;

uiQuestListContent::uiQuestListContent()
{
}

uiQuestListContent::~uiQuestListContent()
{
	this->ClearInfo();
}

//initInfo();
//UpdateQuestInfo();

void
uiQuestListContent::ClearInfo()
{
	// 초기화 
	QUEST_GROUP_ITR itr = m_QuestGroup.begin();
	while (itr != m_QuestGroup.end()) 
	{
		itr->second.clear();
		++itr;
	}

	m_QuestGroup.clear();
}

DWORD uiQuestListContent::CompileQuestStream(BSTR bstrXml, BYTE* ProgrQStream, BYTE* ComplQStream)
{
	USES_CONVERSION;

	std::string tmpStr;
	ConvertBSTRToString(bstrXml,tmpStr);	

	CMarkupArchive xml(CMarkupArchive::ArchiveModeNotZipped | CMarkupArchive::ArchiveModeString);
	xml.OpenString(tmpStr.c_str());

	

	//////////////////////////////////////
	QUESTPART_PROGR* pProgrQuest = (QUESTPART_PROGR*)ProgrQStream;
	QUESTPART_COMPL* pComplQuest = (QUESTPART_COMPL*)ComplQStream;
	int nQuestOffSet =0;

	CHECK(xml.FindElem(_T("QuestList")),"QuestList");

    int nIndex = 0;

	while(TRUE)
	{
		std::string str;
		if(!xml.FindChildElem(_T("Quest")) )
			break;

		xml.IntoElem();

		DWORD QCode = 0;
		CHECK(xml.FindGetChildData(_T("Code"), QCode) ,"Code");
		
		DWORD dwExpireTime = 0;
		CHECK(xml.FindGetChildData(_T("ExpireTime"), dwExpireTime),"ExpireTime");

		DWORD QState = 0;
		CHECK(xml.FindGetChildData(_T("State"), QState),"State");

		DWORD dwNum = 0;
		CHECK(xml.FindGetChildData(_T("Num"), dwNum) ,"Num");

		if(dwNum > 0 )
		{
			pComplQuest[QCode].m_Num = dwNum;
		}
		else
		{
            

			pProgrQuest[nQuestOffSet].m_QCode = QCode;
			pProgrQuest[nQuestOffSet].m_ExpireTime =dwExpireTime;
			pProgrQuest[nQuestOffSet].m_QState = QState;

            //퀘스트에서 몹잡는 수치 표시 2009-11-17 문상현
            char sMonTag[64];
            for(int j = 0 ; j < /*QUESTPART_PROGR_V1::MAX_QUEST_MONSTER_KILL_COUNT*/5 ; j++)
            {
                Sprintf(sMonTag,"KillMonster%d",j);
                CHECK(xml.FindGetChildData(sMonTag, dwNum),sMonTag );
                pProgrQuest[nQuestOffSet].m_KCount[j] = (BYTE)dwNum;
            }
            
			nQuestOffSet++;

#ifdef __NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM
            if(nQuestOffSet >= QUESTPART_PROGR::MAX_QUEST_PROGR_NUM)
#else
            if(nQuestOffSet >= MAX_QUEST_PROGR_NUM)
#endif
            {
                xml.OutOfElem();
                return GM_ERROR_MAX_QUEST;
            }           
            
		}
		
		xml.OutOfElem();		
	}

	return GM_SUCCEEDED;
}

//-------------------------------------------------------------------------------------------
/**
*/
//void
//uiQuestListContent::UpdateQuestInfo(QuestManager2* pQuestMgr, BSTR* bstrXml)
void uiQuestListContent::DecompileQuestInfo(BYTE * ProgrQStream, BYTE * ComplQStream, BSTR* bstrXml)
{
    CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate |
                       CMarkupArchive::ArchiveModeStoring |
                       CMarkupArchive::ArchiveModeNotZipped);
	
	SetXMLCharSet(xml);

	xml.Open("C:\\WOPSConfig\\SUNGMS\\decodeQuestList.xml");
	
	xml.AddElem(_T("QuestList"));

	
	TCHAR pszCategoryName [256] = {0};
	TCHAR pszName[256] = {0};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0};

	QUESTPART_PROGR * pProgrQStream = reinterpret_cast<QUESTPART_PROGR *>(ProgrQStream);

	////////////////////////
	// 퀘스트 진행 목록(진행,완료,실패)
#ifdef __NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM
    for( int i = 0 ; i < QUESTPART_PROGR::MAX_QUEST_PROGR_NUM ; ++i )
#else
    for( int i = 0 ; i < MAX_QUEST_PROGR_NUM ; ++i )
#endif
	{
		if( 0 != pProgrQStream[i].m_QCode )
		{
			// 진행중인 퀘스트
			//pProgrQStream[i].m_QState		//< 퀘스트 상태
			//pProgrQStream[i].m_ExpireTime	//< 퀘스트 완료 제한시간
			//pProgrQStream[i].m_QCode		

			//	pProgrQStream[i].m_KCount[MAX_QUEST_MONSTER_KILL_COUNT];

			QuestInfo* pQuestInfo = g_pQuestInfoManager->FindQuestInfo(pProgrQStream[i].m_QCode);
			if(!pQuestInfo)
				continue;

			GetStringByCode(pQuestInfo->GetGroupCode(), pszCategoryName,256);
			GetStringByCode(pQuestInfo->GetQNCode(), pszName,256);

			switch ( pProgrQStream[i].m_QState )
			{
			case QS_COMPLETED:
				{
					// (완료) ( 9051 )
					GetStringByCode(9051, szMessage,256);
					_tcscat(pszName, szMessage);					
				}
				break;
			case QS_FAILED:
				{
					// (실패) ( 9052 )
					GetStringByCode(9052, szMessage,256);
					_tcscat(pszName, szMessage );
				}
				break;
			case QS_NORMAL:
				{						
				}				
			default:
				break;
			}

			xml.AddChildElem(_T("Quest"));
			xml.IntoElem();

			xml.AddChildElem(_T("Name"), pszName);	
			xml.AddChildElem(_T("Category"), pszCategoryName);

			xml.AddChildElemEx(_T("Code"), pProgrQStream[i].m_QCode );
			xml.AddChildElemEx(_T("ExpireTime"), pProgrQStream[i].m_ExpireTime );

			xml.AddChildElemEx(_T("State"), pProgrQStream[i].m_QState );

			xml.AddChildElemEx(_T("Num"), 0 );

#ifdef _NA_003027_20111013_HONOR_SYSTEM
            xml.AddChildElemEx(_T("IsDailyQuest"), pQuestInfo->IsDailyQuest() ? 1 : 0);
            xml.AddChildElemEx(_T("GroupCode"), pQuestInfo->GetSquadCode());
#endif

            //퀘스트에서 몹잡는 수치 표시 2009-11-17 문상현
            char sMonTag[64];
            for(int j = 0 ; j < /*QUESTPART_PROGR_V1::MAX_QUEST_MONSTER_KILL_COUNT*/5 ; j++)
            {
                Sprintf(sMonTag,"KillMonster%d",j);
                xml.AddChildElemEx(sMonTag, pProgrQStream[i].m_KCount[j] );
            }
			xml.OutOfElem();
		}
	}

	QUESTPART_COMPL * pComplQStream = reinterpret_cast<QUESTPART_COMPL *>(ComplQStream);

	////////////////////////
	// 퀘스트 완료 목록
#ifdef __NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM
    for( int QCode = 0 ; QCode < QUESTPART_COMPL::MAX_QUEST_COMPL_NUM ; ++QCode )
#else
    for( int QCode = 0 ; QCode < MAX_QUEST_COMPL_NUM ; ++QCode )
#endif		
	{
		if( pComplQStream[QCode].m_Num > 0 )
		{
			// 완료한 퀘스트, 완료한 횟수(0 ~255)
			// QCode, pComplQStream[i].m_Num	

			QuestInfo* pQuestInfo = g_pQuestInfoManager->FindQuestInfo(QCode);
			if(!pQuestInfo)
				continue;

			GetStringByCode(pQuestInfo->GetGroupCode(), pszCategoryName,256);
			GetStringByCode(pQuestInfo->GetQNCode(), pszName,256);

			// (완료) ( 9051 )
			GetStringByCode(9051, szMessage,256);
			_tcscat(pszName, szMessage);			

			xml.AddChildElem(_T("Quest"));
			xml.IntoElem();

			xml.AddChildElem(_T("Name"), pszName);	
			xml.AddChildElem(_T("Category"), pszCategoryName);

			xml.AddChildElemEx(_T("Code"), QCode );
			xml.AddChildElemEx(_T("ExpireTime"), 0 );

			xml.AddChildElemEx(_T("State"), QS_COMPLETED );

			xml.AddChildElemEx(_T("Num"), pComplQStream[QCode].m_Num );
#ifdef _NA_003027_20111013_HONOR_SYSTEM            
            xml.AddChildElemEx(_T("IsDailyQuest"), pQuestInfo->IsDailyQuest() ? 1 : 0);
            xml.AddChildElemEx(_T("GroupCode"), pQuestInfo->GetSquadCode());
#endif

			xml.OutOfElem();			
		}
	}

	//////////////////
	/*	QuestManager2* pQuestMgr = new QuestManager2(  g_pQuestInfoManager);
	pQuestMgr->SerializeProgrStream( ProgrQStream, MAX_PROGR_QUESTSTREAM_SIZE, QSERIALIZE_SAVE );
	pQuestMgr->SerializeComplStream( ComplQStream, MAX_COMPL_QUESTSTREAM_SIZE, QSERIALIZE_SAVE );

	uiQuestListContent QuestList;
	QuestList.UpdateQuestInfo(pQuestMgr, bstrXml);
	delete pQuestMgr;
	*/
	//////////////////
	//	MissionManager missionMgr;
	//	missionMgr.SerializeStream(Mission, MAX_MISSIONSTREAM_SIZE, SERIALIZE_STORE);
	//
	//	uiQuestMissionList MissionList;
	//	MissionList.UpdateMissionInfo(&missionMgr, bstrXml);

	*bstrXml = _bstr_t(xml.GetDoc().c_str() ).copy();
	xml.Close();
}
