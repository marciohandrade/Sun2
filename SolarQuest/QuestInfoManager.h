#pragma once
//=======================================================================================================================
/// QuestInfoManager class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 3. 23
	@remarks
			- 
	@history
			- 2006.5.19 : for_each,find_if 관련 functor 추가
*/
//=======================================================================================================================
#include "QuestInfo.h"

class QuestInfoManager
{
public:
	QuestInfoManager(void);
	~QuestInfoManager(void);

	// loader에서 등록해서 release에서 메모리 해제
	VOID ReleaseEx();

	inline QuestInfo * FindQuestInfo( QCODE QCode ) { return m_QuestInfoHash.GetData( QCode ); }

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    QUEST_GROUP_MAP& quest_group_map() { return quest_group_map_; }
#endif //_NA_003027_20111013_HONOR_SYSTEM

	template< class Operator >
		void for_each( Operator & Opr );
	template< class Operator >
		void for_each_auto( Operator & Opr );

	template< class Operator >
		bool find_if( Operator & Opr );
	template< class Operator >
		bool find_if_auto( Operator & Opr );

protected:
	inline void AddQuestInfo( QuestInfo * pInfo, QCODE QCode ) { m_QuestInfoHash.Add( pInfo, QCode ); }
	inline void AddAutoQuestInfo( QuestInfo * pInfo, QCODE QCode ) { m_AutoAcceptQuestInfoHash.Add( pInfo, QCode ); }

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    void AddGroupQuestCode(WORD quest_group_code, QCODE quest_code);
#endif //_NA_003027_20111013_HONOR_SYSTEM  

private:
	
	friend class QuestScriptLoader;
	QUEST_INFO_HASH m_QuestInfoHash;
	QUEST_INFO_HASH m_AutoAcceptQuestInfoHash;

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    QUEST_GROUP_MAP quest_group_map_;
#endif //_NA_003027_20111013_HONOR_SYSTEM    
};

template< class Operator >
void QuestInfoManager::for_each( Operator & Opr )
{
	QUEST_INFO_HASH::iterator it, end( m_QuestInfoHash.end() );
	for( it = m_QuestInfoHash.begin() ; it != end ; ++it )
		Opr(*it);
}
template< class Operator >
void QuestInfoManager::for_each_auto( Operator & Opr )
{
	QUEST_INFO_HASH::iterator it, end( m_AutoAcceptQuestInfoHash.end() );
    for( it = m_AutoAcceptQuestInfoHash.begin() ; it != end ; ++it ) {
        QuestInfo* quest_info = 
#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
            it->second;
#else
            *it;
#endif
		Opr(quest_info);
    }
}
template< class Operator >
bool QuestInfoManager::find_if( Operator & Opr )
{
	QUEST_INFO_HASH::iterator it, end( m_QuestInfoHash.end() );
	for( it = m_QuestInfoHash.begin() ; it != end ; ++it )
	{
		if( Opr(*it) ) return true;
	}

	return false;
}
template< class Operator >
bool QuestInfoManager::find_if_auto( Operator & Opr )
{
	QUEST_INFO_HASH::iterator it, end( m_AutoAcceptQuestInfoHash.end() );
	for( it = m_AutoAcceptQuestInfoHash.begin() ; it != end ; ++it )
	{
		if( Opr(*it) ) return true;
	}

	return false;
}
