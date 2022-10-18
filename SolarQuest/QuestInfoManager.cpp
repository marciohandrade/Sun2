#include "StdAfx.h"
#include ".\questinfomanager.h"
#include "QuestStruct.h"
#include <algorithm>

QuestInfoManager::QuestInfoManager(void)
{
#if !defined(_NA_004157_20120409_QUEST_BANDWITH_INCREASING)
	m_QuestInfoHash.Initialize( QUESTPART_COMPL::MAX_QUEST_COMPL_NUM );
	m_AutoAcceptQuestInfoHash.Initialize( QUESTPART_COMPL::MAX_QUEST_COMPL_NUM );
#endif
}

QuestInfoManager::~QuestInfoManager(void)
{
	ReleaseEx();
}

VOID QuestInfoManager::ReleaseEx()
{
	QUEST_INFO_HASH_ITR it, end(m_QuestInfoHash.end());
	for( it = m_QuestInfoHash.begin() ; it != end ; ++it )
	{
		QuestInfo * pInfo = 
#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
            it->second;
#else
            (*it);
#endif
		pInfo->Release();
		delete pInfo;
	}
	m_QuestInfoHash.clear();
	m_AutoAcceptQuestInfoHash.clear();
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
void QuestInfoManager::AddGroupQuestCode(WORD quest_group_code, QCODE quest_code)
{
    QUEST_CODE_VECTOR& quest_code_vector = quest_group_map_[quest_group_code];
    QUEST_CODE_VECTOR::iterator found_itr = std::find(quest_code_vector.begin(), 
        quest_code_vector.end(), 
        quest_code);
    
    if (found_itr == quest_code_vector.end())
    {
        quest_code_vector.push_back(quest_code);
    }    
}
#endif //_NA_003027_20111013_HONOR_SYSTEM