#include "stdafx.h"
#include "QuestEx.h"
#include "logic/logic.h"
#include "util/util.h"

extern QuestInfoManager2* g_pQuestInfoManager;

void enumQuests(QuestInfo* pQuest)
{
	QUEST_INFO questINFO = {0};
	
	GetStringByCode(pQuest->GetQNCode(), questINFO.pszName,256);
	GetStringByCode(pQuest->GetGroupCode(), questINFO.pszCategoryName,256);
	questINFO.m_ExpireTime = pQuest->GetExpiredTime();
	questINFO.m_QState = QS_NORMAL;
	questINFO.m_Num = 0;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    questINFO.is_daily = pQuest->GetDailyQuest();
    questINFO.group_code = pQuest->GetSquadCode();
#endif

	g_pQuestInfoManager->m_QuestPreset.insert(std::pair<QCODE, QUEST_INFO>(pQuest->GetQuestCode(), questINFO));
//	_LOG("QUEST [%d] %s", pQuest->GetQuestCode(), questINFO.pszName);
}

BOOL QuestInfoManager2::Load( char * pszFileName )
{
#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
	m_ScriptLoader.SetQuestInfoManager( this );
	m_ScriptLoader.Load( SCRIPT_QUESTINFO, pszFileName, FALSE, SEPERATOR_WHITESPACE, NULL, FALSE );

#else
	// 파일 열기
	util::SolarFileReader sr;
	if( !sr.OpenFile( pszFileName, SEPERATOR_WHITESPACE ) )
		return FALSE;
	m_ScriptLoader.Load( sr, *this );
	sr.CloseFile();
#endif
	for_each(enumQuests);

	return TRUE;	
}
