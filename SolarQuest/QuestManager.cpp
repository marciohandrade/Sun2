#include "StdAfx.h"
#include ".\questmanager.h"
#include ".\Quest.h"
#include ".\QuestInfoManager.h"
#include <algorithm>
#include <functional>
#include ".\QuestCondition.h"
#include ".\QuestAction.h"
#include "SolarDateTime.h"

#include <atltime.h>

//==================================================================================================
// CHANGES: f110527.2L
namespace util {
;
struct QuestPool
{
    typedef util::CMemoryPoolFactory<Quest> PoolType;
    QuestPool()
    {
        pool_.Initialize(QUESTPART_PROGR::MAX_QUEST_PROGR_NUM,
                         QUESTPART_PROGR::MAX_QUEST_PROGR_NUM,
                         "Quest");
    };
    ~QuestPool()
    {
        pool_.Release();
    };
    //
    PoolType pool_;
    //
    __DISABLE_COPY(QuestPool);
};
}; //end of namespace
//==================================================================================================

QuestManager::QuestManager(QuestInfoManager * pmgr)
:	m_pQuestInfoManager(pmgr)
{
    // NOTE: f110527.3L, memory usage optimization
    static util::QuestPool quest_pool_static;
    Quest::SetPool(&quest_pool_static.pool_);
    m_QuestHash.Initialize(QUESTPART_PROGR::MAX_QUEST_PROGR_NUM);
    DestroyAll(); // �ʱ�ȭ�� ���ؼ� �ѹ� ���ش�. // j0 // Ȯ���ϰ� �ּ� �����ּ���.
}

QuestManager::~QuestManager(void)
{
	DestroyAll();
}

void QuestManager::DestroyAll()
{
    //(lst1024)(060915) �÷��̾� ��ü Ǯ���� Release�� ���� �ʱ�ȭ.
    m_ProgressTimeQCode = 0;
    ClearProgrStream();
	ClearComplStream();
}

void QuestManager::Update()
{
	QUEST_HASH_ITR it, end(m_QuestHash.end());
	for( it = m_QuestHash.begin() ; it != end ; ++it )
	{
		if( FALSE == (*it)->Update() )
			OnEvent( QUEST_EVENT_TIME, NULL, (*it) );
	}
}

void QuestManager::Event( eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
	QUEST_HASH_ITR it, end(m_QuestHash.end());
	for( it = m_QuestHash.begin() ; it != end ; ++it )
	{
		if( FALSE != (*it)->Event( evt, pMsg ) )
		{
			OnEvent( evt, pMsg, (*it) );
		}
	}
}
BOOL QuestManager::Event( QCODE QCode, eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
	Quest * pQuest = FindQuest( QCode );
	if( !pQuest ) return FALSE;

	if( FALSE != pQuest->Event( evt, pMsg ) )
	{
		OnEvent( evt, pMsg, pQuest );
		return TRUE;
	}
	return FALSE;
}

RC::eQUEST_RESULT QuestManager::CanAccept( QCODE qCode, void * pvParam )
{
	// ��� ������ ������ �� : and ����
	Quest * pQuest = m_QuestHash.GetData(qCode);
	if( !pQuest ) return RC::RC_QUEST_EMPTY;
	return pQuest->CanAccept( pvParam );
}

RC::eQUEST_RESULT QuestManager::CanComplete( QCODE qCode, void * pvParam )
{
	// ��� ������ ������ �� : and ����
	Quest * pQuest = m_QuestHash.GetData(qCode);
	if( !pQuest ) return RC::RC_QUEST_EMPTY;
	return pQuest->CanComplete( pvParam );
}

DWORD QuestManager::GetChildQuestNum(QCODE qCode)
{
#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
    return 0;
#else
    Quest* pQuest = m_QuestHash.GetData(qCode);
    if (!pQuest)
        return 0;

    QuestInfo* const questInfo = pQuest->GetQuestInfo();
    QUEST_INFO_HASH& childQuestInfo = questInfo->GetChildQuestInfoHash();
    const DWORD childQuestDataNum = childQuestInfo.GetDataNum();

    return childQuestDataNum;
#endif
}

RC::eQUEST_RESULT QuestManager::CanDoAccept( QCODE qCode, void * pvParam )
{
	Quest * pQuest = m_QuestHash.GetData(qCode);
	if( !pQuest ) return RC::RC_QUEST_EMPTY;
	RC::eQUEST_RESULT rt;
	rt = pQuest->CanAccept( pvParam );
	if( rt != RC::RC_QUEST_SUCCESS ) return rt;

	pQuest->DoAccept( pvParam );

#if !defined(_NA_004157_20120409_QUEST_BANDWITH_INCREASING)
	// �ڽ� ����Ʈ ���� ó��
	Quest * pChild = NULL;
	QUEST_INFO_HASH_ITR it(pQuest->GetQuestInfo()->m_ChildQuestInfoHash.begin()), 
						end(pQuest->GetQuestInfo()->m_ChildQuestInfoHash.end());
	for( ; it != end ; ++it )
	{
	    //__NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM
        // �θ� ����Ʈ�� ���� ��ȹ���� ������� �ʴ´�.
		if( IsQuestMax() )
		{
            // DoAccept ���� FALSE ���Ͻ� ���� �߻��Ѵ�.
            // ����Ʈ���� handler���� ��ü ���� ������ üũ.
            break;
		};
		if( pChild = CreateQuest( (*it)->GetQuestCode() ) )
		{
			assert( pChild->CanAccept(pvParam) == RC::RC_QUEST_SUCCESS );	
			pChild->DoAccept(pvParam);
		}
	}
#endif

	return RC::RC_QUEST_SUCCESS;
}

RC::eQUEST_RESULT QuestManager::CanDoComplete( QCODE qCode, void * pvParam )
{
	Quest * pQuest = m_QuestHash.GetData(qCode);

	if( !pQuest ) return RC::RC_QUEST_EMPTY;

	RC::eQUEST_RESULT rt = pQuest->CanComplete( pvParam );
	if( rt != RC::RC_QUEST_SUCCESS )
		return rt;

	pQuest->DoComplete( pvParam );

	return RC::RC_QUEST_SUCCESS;
}

Quest * QuestManager::CreateQuest( QCODE QuestCode )
{
	QuestInfo * pInfo = m_pQuestInfoManager->FindQuestInfo( QuestCode );
	if( !pInfo ) return NULL;

	Quest * pQuest = FindQuest( QuestCode );
	if( pQuest ) return NULL;

	pQuest = Quest::ALLOC();
	pQuest->Init( m_pQuestInfoManager, this, &m_pCompletedQuest[QuestCode] );
	pQuest->SetQuestInfo( pInfo );
	pQuest->Build( NULL );

	AddQuest( pQuest, pQuest->GetQuestCode() );

	return pQuest;
}

void QuestManager::DestroyQuest(Quest* quest)
{
#if !defined(_NA_004157_20120409_QUEST_BANDWITH_INCREASING)
    // �ڽ� ����Ʈ ���� ó��
    QUEST_INFO_HASH& child_quest_list = quest->GetQuestInfo()->m_ChildQuestInfoHash;
    QUEST_INFO_HASH_ITR it(child_quest_list.begin()),
                        end(child_quest_list.end());
    for ( ; it != end; ++it)
    {
        const QuestInfo* child_quest_info = *it;
        const QCODE child_quest_code = child_quest_info->GetQuestCode();
        if (Quest* child_quest = FindQuest(child_quest_code))
        {
            RemoveQuest(child_quest_code);
            child_quest->Destroy();
        }
    }
#endif

    const QCODE quest_code = quest->GetQuestCode();
    RemoveQuest(quest_code);
    quest->Destroy();
    Quest::FREE(quest);
}

void QuestManager::DestroyQuest( QCODE QuestCode )
{
	Quest * pQuest = FindQuest(QuestCode);
	if( !pQuest ) return ;

	DestroyQuest( pQuest );
}
VOID QuestManager::SerializeProgrStream( BYTE * pProgrStream, WORD wSize, eQUEST_SERIALIZE eType )
{
	if( eType == QSERIALIZE_SAVE )
	{
		ClearProgrStream();

		QUESTPART_PROGR * pProgr = (QUESTPART_PROGR *)pProgrStream;
		for( int idx = 0 ; idx < QUESTPART_PROGR::MAX_QUEST_PROGR_NUM ; ++idx )
		{
			assert( pProgr[idx].m_QCode < QUESTPART_COMPL::MAX_QUEST_COMPL_NUM );
			if( pProgr[idx].m_QCode == 0 || pProgr[idx].m_QCode >= QUESTPART_COMPL::MAX_QUEST_COMPL_NUM )
                continue;

            if( !m_pQuestInfoManager->FindQuestInfo( pProgr[idx].m_QCode ) )
                continue;
			Quest * pQuest = Quest::ALLOC();
			pQuest->Init( m_pQuestInfoManager, this, &m_pCompletedQuest[pProgr[idx].m_QCode] );
			pQuest->Serialize( pProgr[idx], eType );
			assert( NULL == FindQuest( pQuest->GetQuestCode() ) );
			AddQuest( pQuest, pQuest->GetQuestCode() );
		}
	}
	else if( eType == QSERIALIZE_LOAD )
	{
		memset( pProgrStream, 0, wSize );
		if( m_QuestHash.size() <= QUESTPART_PROGR::MAX_QUEST_PROGR_NUM )
		{
			QUESTPART_PROGR * pProgr = (QUESTPART_PROGR *)pProgrStream;
			BYTE idx = 0;
			QUEST_HASH_ITR it, end(m_QuestHash.end());
			for( it = m_QuestHash.begin() ; it != end ; ++it, ++idx )
			{
				(*it)->Serialize( pProgr[idx], eType );
			}
		}
	}
}

#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
VOID QuestManager::SerializeComplStream( BYTE * pComplStream, WORD wSize, eQUEST_SERIALIZE eType )
{
    if (eType == QSERIALIZE_SAVE)
    {
        QUESTPART_COMPL_EX* completed_quests = reinterpret_cast<QUESTPART_COMPL_EX*>(pComplStream);

        WORD daily_quest  = 0;
        WORD normal_quest = 0;

        for(QUESTPART_COMPL_EX* it = &completed_quests[0]; it != &completed_quests[wSize]; ++it)
        {
            QUESTPART_COMPL_EX part_data;
            part_data.m_QCode = it->m_QCode;
            part_data.m_Num   = it->m_Num;
            m_pCompletedQuest.insert(COMPLETED_QUEST_MAP::value_type(it->m_QCode, part_data));

            if (part_data.m_Num > 0)
            {
                QuestInfo* quest_info = m_pQuestInfoManager->FindQuestInfo(part_data.m_QCode);
                if (quest_info) 
                {
                    if (quest_info->GetDailyQuest() > 0) {
                        daily_quest += part_data.m_Num;
                    }
                    else if(quest_info->GetACMaxRepeatNum() == 1) {
                        ++normal_quest;
                    }
                }
            }
        }

        SetCompleteQuestCount(normal_quest, daily_quest);
    }
    else if (eType == QSERIALIZE_LOAD)
    {
        QUESTPART_COMPL_EX* completed_quests = reinterpret_cast<QUESTPART_COMPL_EX*>(pComplStream);
        for(COMPLETED_QUEST_MAP::iterator it2 = m_pCompletedQuest.begin();
            it2 != m_pCompletedQuest.end(); ++it2)
        {
            memcpy(&completed_quests[wSize], &it2->second, sizeof(QUESTPART_COMPL_EX));
        }
    };
}
#else
VOID QuestManager::SerializeComplStream( BYTE * pComplStream, WORD wSize, eQUEST_SERIALIZE eType )
{
    BOOST_STATIC_ASSERT(_countof(m_pCompletedQuest) == QUESTPART_COMPL::MAX_QUEST_COMPL_NUM);
    const size_t kDesiredMessageSize = sizeof(m_pCompletedQuest);
    assert(kDesiredMessageSize == wSize);

    if (eType == QSERIALIZE_SAVE)
    {
        CopyMemory(m_pCompletedQuest, pComplStream, kDesiredMessageSize);

#ifdef _NA_003027_20111013_HONOR_SYSTEM
        WORD daily_quest  = 0;
        WORD normal_quest = 0;

        for (QCODE idx = 1; idx < _countof(m_pCompletedQuest); ++idx)
        {
            if (m_pCompletedQuest[idx].m_Num > 0)
            {
                QuestInfo* quest_info = m_pQuestInfoManager->FindQuestInfo(idx);// idx => questcode
                if (quest_info) 
                {
                    if (quest_info->GetDailyQuest() > 0) {
                        daily_quest += m_pCompletedQuest[idx].m_Num;
                    }
                    else if(quest_info->GetACMaxRepeatNum() == 1) {
                        ++normal_quest;
                    }
                }
            }
        };

        SetCompleteQuestCount(normal_quest, daily_quest);
#endif//_NA_003027_20111013_HONOR_SYSTEM
    }
    else if (eType == QSERIALIZE_LOAD)
    {
        CopyMemory(pComplStream, m_pCompletedQuest, wSize);
    };
}
#endif

VOID QuestManager::SerializeStream( QUEST_PROGR_TOTAL_INFO & IN rTotalInfo, eQUEST_SERIALIZE eType )
{
	if( eType == QSERIALIZE_SAVE )
	{
		ClearProgrStream();
		for( int idx = 0 ; idx < rTotalInfo.m_Count ; ++idx )
		{
			assert( rTotalInfo.m_Info[idx].m_QCode < QUESTPART_COMPL::MAX_QUEST_COMPL_NUM );
			if( rTotalInfo.m_Info[idx].m_QCode >= QUESTPART_COMPL::MAX_QUEST_COMPL_NUM )
                continue;
			Quest * pQuest = Quest::ALLOC();
			pQuest->Init( m_pQuestInfoManager, this, &m_pCompletedQuest[rTotalInfo.m_Info[idx].m_QCode] );
			pQuest->Serialize( rTotalInfo.m_Info[idx], eType );
			assert( NULL == FindQuest( pQuest->GetQuestCode() ) );
			AddQuest( pQuest, pQuest->GetQuestCode() );
		}
	}
	else
	{
		BYTE idx = 0;
		QUEST_HASH_ITR it, end(m_QuestHash.end());
		for( it = m_QuestHash.begin() ; it != end ; ++it, ++idx )
		{
			(*it)->Serialize( rTotalInfo.m_Info[idx], eType );
		}
		assert( SAFE_NUMERIC_TYPECAST( DWORD, m_QuestHash.size(), BYTE ) );
		rTotalInfo.m_Count = (BYTE)m_QuestHash.size();
	}
}
VOID QuestManager::ClearProgrStream()
{
	QUEST_HASH_ITR it, end(m_QuestHash.end());
	for( it = m_QuestHash.begin() ; it != end ; ++it )
	{
		(*it)->Destroy();
		Quest::FREE((*it));
	}
	m_QuestHash.clear();
}
VOID QuestManager::ClearComplStream()
{
#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
    m_pCompletedQuest.clear();
#else
	memset( m_pCompletedQuest, 0, (QUESTPART_COMPL::MAX_QUEST_COMPL_NUM)*sizeof(QUESTPART_COMPL) );
#endif

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    completed_quest_count.daily_quest = 0;
    completed_quest_count.normal_quest = 0;
#endif//_NA_003027_20111013_HONOR_SYSTEM
}
VOID QuestManager::SerializeStream( QUEST_COMPL_TOTAL_INFO & IN rTotalInfo, eQUEST_SERIALIZE eType )
{
	if( eType == QSERIALIZE_SAVE )
	{
		ClearComplStream();

		assert( rTotalInfo.m_Count <= QUESTPART_COMPL::MAX_QUEST_COMPL_NUM );
		if( rTotalInfo.m_Count > QUESTPART_COMPL::MAX_QUEST_COMPL_NUM )
            return;

		for( int idx = 0 ; idx < rTotalInfo.m_Count ; ++idx )
		{
			QCODE tmpQCode = rTotalInfo.m_Info[idx].m_QCode;
			assert( tmpQCode < QUESTPART_COMPL::MAX_QUEST_COMPL_NUM );
			if( tmpQCode >= QUESTPART_COMPL::MAX_QUEST_COMPL_NUM )
                continue;

			m_pCompletedQuest[tmpQCode].m_Num = rTotalInfo.m_Info[idx].m_Num;
			//memcpy( &m_pCompletedQuest[rTotalInfo.m_Info[idx].m_QCode], &rTotalInfo.m_Info[idx], sizeof(QUESTPART_COMPL) );
		}
	}
	else if( eType == QSERIALIZE_LOAD )
	{
		rTotalInfo.m_Count = 0;

		for( QCODE idx = 1 ; idx < QUESTPART_COMPL::MAX_QUEST_COMPL_NUM ; ++idx )
		{
			if( m_pCompletedQuest[idx].m_Num > 0 )
			{
				//memcpy( &rTotalInfo.m_Info[rTotalInfo.m_Count], &m_pCompletedQuest[idx], sizeof(QUESTPART_COMPL) );
				rTotalInfo.m_Info[rTotalInfo.m_Count].m_Num = m_pCompletedQuest[idx].m_Num;
				rTotalInfo.m_Info[rTotalInfo.m_Count].m_QCode = idx;
				++rTotalInfo.m_Count;
			}
		}
	}
}

// ������ �ް� ���� �Ϸ� ����Ʈ ���� ���
void QuestManager::DoReward( QCODE QCode/*������������Ʈ�ڵ�*/, void * pvParam )
{
	Quest * pQuest = FindQuest(QCode);
	if( !pQuest ) return;

	pQuest->DoReward( pvParam );

	IncComplStream( QCode );

	DestroyQuest(pQuest);
}
void QuestManager::DecComplStream( QCODE QCode )
{
	ASSERT( QCode < QUESTPART_COMPL::MAX_QUEST_COMPL_NUM );
	ASSERT( m_pCompletedQuest[QCode].m_Num != 0 );

	if( m_pCompletedQuest[QCode].m_Num != 0 )
		--m_pCompletedQuest[QCode].m_Num;

	//if( QCode > m_MaxComplQuestCount )
	//	m_MaxComplQuestCount = QCode;
}
void QuestManager::IncComplStream( QCODE QCode )
{
	ASSERT( QCode < QUESTPART_COMPL::MAX_QUEST_COMPL_NUM );
    //(lst1024)(090611) ���� �ݺ����� 255ȸ �̻��ϴ� ������ ��������.
//	ASSERT( m_pCompletedQuest[QCode].m_Num != 0xFF );

	if( m_pCompletedQuest[QCode].m_Num != 0xFF )
		++m_pCompletedQuest[QCode].m_Num;
}

//{__NA001282_20090206_MODIFY_QUEST_TIME
void QuestManager::DoFailed( QCODE QCode )
{
	Quest * pQuest = FindQuest(QCode);
	if( !pQuest ) return;

	if( m_ProgressTimeQCode == QCode )
		m_ProgressTimeQCode = 0;
}

void QuestManager::DoAbandon( QCODE QCode )
{
	Quest * pQuest = FindQuest(QCode);
	if( !pQuest ) return;

	if( m_ProgressTimeQCode == QCode )
		m_ProgressTimeQCode = 0;
}

// �ð��� �����ϴ� ��츸 TimeQuest.
BOOL QuestManager::IsTimeQuest(QCODE QCode)
{
	QuestInfo * pInfo = m_pQuestInfoManager->FindQuestInfo( QCode );
	if( !pInfo ) 
		return FALSE;

	return !!(pInfo->GetExpiredTime());
}
//}__NA001282_20090206_MODIFY_QUEST_TIME

BOOL QuestManager::GmAccept( QCODE qCode, void * pvParam )
{
	Quest * pQuest = m_QuestHash.GetData(qCode);
	if( !pQuest ) 
		return FALSE;

	pQuest->DoAccept( pvParam );

#if !defined(_NA_004157_20120409_QUEST_BANDWITH_INCREASING)
	// �ڽ� ����Ʈ ���� ó��
	Quest * pChild = NULL;
	QUEST_INFO_HASH_ITR it(pQuest->GetQuestInfo()->m_ChildQuestInfoHash.begin()), 
		end(pQuest->GetQuestInfo()->m_ChildQuestInfoHash.end());
	for( ; it != end ; ++it )
		if( pChild = CreateQuest( (*it)->GetQuestCode() ) )
			pChild->DoAccept(pvParam);
#endif

	return TRUE;
}

BOOL QuestManager::GmComplete( QCODE qCode, void * pvParam )
{
	Quest * pQuest = m_QuestHash.GetData(qCode);
	if( !pQuest ) 
		return FALSE;

	pQuest->DoComplete( pvParam );

	return TRUE;
}

BOOL QuestManager::GmReset( QCODE QCode, void * pvParam )
{
    __UNUSED(pvParam);
	if( QCode >= QUESTPART_COMPL::MAX_QUEST_COMPL_NUM )
		return FALSE;

	if( 0 == QCode )
    {
#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
        m_pCompletedQuest.clear();
#else
		ZeroMemory( m_pCompletedQuest, QUESTPART_COMPL::MAX_QUEST_COMPL_NUM );
#endif
    }
	else
		m_pCompletedQuest[QCode].m_Num = 0;

	return TRUE;
}

#ifdef _NA_20100307_BETAKEY_SYSTEM
INT32 QuestManager::GetBetakey(QCODE quest_code)
{
    Quest * pQuest = m_QuestHash.GetData(quest_code);
    if( !pQuest ) 
        return 0;

    return pQuest->GetQuestInfo()->GetBetaKey();
}
#endif

#ifdef _NA_003027_20111013_HONOR_SYSTEM
BOOL QuestManager::IncCompleteQuestCount(QCODE questcode)
{
    QuestInfo* quest_info = m_pQuestInfoManager->FindQuestInfo( questcode );
    if(!quest_info) 
        return FALSE;

    if (quest_info->GetDailyQuest() > 0)
    {
        ++completed_quest_count.daily_quest;
        return TRUE;
    }
    else if(quest_info->GetACMaxRepeatNum() == 1)
    {
        ++completed_quest_count.normal_quest;
        return TRUE;
    }

    return FALSE;
}

BOOL QuestManager::GetCompleteQuestCount(WORD& OUT normal_quest, WORD& OUT daily_quest)
{
    normal_quest    = completed_quest_count.normal_quest;
    daily_quest     = completed_quest_count.daily_quest;

    return TRUE;
}

BOOL QuestManager::SetCompleteQuestCount(WORD normal_quest, WORD daily_quest)
{
    completed_quest_count.normal_quest  = normal_quest;
    completed_quest_count.daily_quest   = daily_quest;

    return TRUE;
}

void QuestManager::SerializeDailyQuestStream(QUEST_DAILY_INFO* stream, WORD& count, eSERIALIZE eType)
{
    if( eType == SERIALIZE_STORE )
    {
        daily_quest_data_.clear();

        for(int i = 0; i < count; ++i)
        {
            QUEST_DAILY_INFO* src = &stream[i];
            daily_quest_data_.insert(DAILY_QUEST_MAP::value_type((QCODE)src->code_, (*src)));

            //!~ ���� ����Ʈ ������ ����Ʈ ������ ��ġ ���� ������ ���� ����Ʈ ������ ��ȿȭ 
            // arycoat 2011.12.23
            if(src->status_ == QS_NORMAL) 
            {
                Quest* quest = FindQuest(src->code_);
#ifdef _NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION
                if(!quest || quest->GetQState() == QS_FAILED) {
                    DoAbandonDailyQuest(src->code_);
                }
#else
                if(!quest || quest->GetQState() != QS_NORMAL) {
                    DoAbandonDailyQuest(src->code_);
                }
#endif //_NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION
            }
            //~!
        }
    }
    else if( eType == SERIALIZE_LOAD )
    {
        count = 0;

        for(DAILY_QUEST_MAP::iterator it = daily_quest_data_.begin(); it != daily_quest_data_.end(); ++it)
        {
            memcpy(&stream[count++], &(it->second), sizeof(QUEST_DAILY_INFO));
            
            if(count >= MAX_DAILYQUEST_INFO_COUNT)
                break;
        }
    }
}


const QUEST_DAILY_INFO* const QuestManager::GetDailyQuestData( QCODE code )
{
    DAILY_QUEST_MAP::iterator it = daily_quest_data_.find(code);
    if(it == daily_quest_data_.end())
        return NULL;

    return &(it->second);
}

void QuestManager::SetDailyQuestData(QUEST_DAILY_INFO* daily_quest_info)
{
    if (daily_quest_info == NULL)
        return;

    DAILY_QUEST_MAP::iterator it = daily_quest_data_.find(daily_quest_info->code_);
    if(it == daily_quest_data_.end())
    {
        daily_quest_data_.insert(DAILY_QUEST_MAP::value_type(daily_quest_info->code_, (*daily_quest_info)));
    }
    else
    {
        QUEST_DAILY_INFO& quest_data = (it->second);
        quest_data = (*daily_quest_info);
    }
}

RC::eQUEST_RESULT QuestManager::CanDoAcceptDailyQuest(QCODE QCode, void * pvParam)
{
    QuestInfo* quest_info = GetQuestInfoManager()->FindQuestInfo(QCode);
    if (quest_info == NULL)
        return RC::RC_QUEST_EMPTY; // quest info empty

    // 1.��������Ʈ �Ϸ� ���� ���� üũ
    BYTE daily_quest_type = quest_info->GetDailyQuest();
    int daily_quest_count = GetDailyQuestCompleteCountToday(daily_quest_type);

    // �Ϲ� ��������Ʈ (type : 1)
    if (daily_quest_type == DAILYQUEST_NORMAL && \
        daily_quest_count >= MAX_DAILYQUEST_PROGRESS_COUNT)
    {
        // �Ϸ� ��������Ʈ ���� �ѱ�
        return RC::RC_QUEST_MAX_DAILY_QUEST_COUNT;
    }

    // ��� ��������Ʈ (type : 2)
    if (daily_quest_type == DAILYQUEST_GUILD && \
        daily_quest_count >= MAX_GUILD_DAILYQUEST_PROGRESS_COUNT)
    {
        // �Ϸ� ��� ��������Ʈ ���� �ѱ�
        return RC::RC_QUEST_MAX_GUILD_DAILY_QUEST_COUNT;
    }

#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
    // ���� ��������Ʈ (type : 3)
    if (daily_quest_type == DAILYQUEST_CHALLENGE && \
        daily_quest_count >= MAX_CHALLENGE_DAILYQUEST_PROGRESS_COUNT)
    {
        // �Ϸ� ��� ��������Ʈ ���� �ѱ�
        return RC::RC_QUEST_MAX_CHALLENGE_DAILY_QUEST_COUNT;
    } 
#endif //_NA_007407_20140710_CHALLENGE_DAILY_QUEST

    // 2.�� ����Ʈ�� �������� �ʱ�ȭ �ð� üũ
    BOOL is_completed_daily_quest = IsCompletedDailyQuest(QCode, quest_info);
    if (is_completed_daily_quest == TRUE)
    {
        return RC::RC_QUEST_COMPLETED_DAILY_QUEST;
    }

    // 3. ����Ʈ �׷� üũ
    if (IsCompletedDailyQuestGroup(quest_info->GetSquadCode()) == TRUE)
    {
        return RC::RC_QUEST_COMPLETED_DAILY_QUEST;
    }

    return RC::RC_QUEST_SUCCESS;
}

void QuestManager::DoAcceptDailyQuest( QCODE questcode )
{
    DAILY_QUEST_MAP::iterator it = daily_quest_data_.find(questcode);
    if(it == daily_quest_data_.end())
    {
        QUEST_DAILY_INFO quest_data;
        quest_data.code_ = questcode;
        quest_data.status_ = QS_NORMAL;
        util::GetDateTime_YYYYMMDDHHMMSS(quest_data.start_datetime_);
        quest_data.end_datetime_ = quest_data.start_datetime_;

        daily_quest_data_.insert(DAILY_QUEST_MAP::value_type(questcode, quest_data));
    }
    else
    {
        QUEST_DAILY_INFO& quest_data = (it->second);
        quest_data.status_ = QS_NORMAL;
        util::GetDateTime_YYYYMMDDHHMMSS(quest_data.start_datetime_);
        //quest_data.end_datetime_ = 0;
    }
}

void QuestManager::DoCompleteDailyQuest( QCODE questcode )
{
    DAILY_QUEST_MAP::iterator it = daily_quest_data_.find(questcode);
    if(it == daily_quest_data_.end())
    {
        // what????
    }
    else
    {
        QUEST_DAILY_INFO& quest_data = (it->second);
        quest_data.status_ = QS_COMPLETED;
        //quest_data.start_datetime_ = 0; // do not clear previous start time.
        util::GetDateTime_YYYYMMDDHHMMSS(quest_data.end_datetime_);
    }
}

void QuestManager::DoAbandonDailyQuest( QCODE questcode )
{
    DAILY_QUEST_MAP::iterator it = daily_quest_data_.find(questcode);
    if(it == daily_quest_data_.end())
    {
        // what????
    }
    else
    {
        QUEST_DAILY_INFO& quest_data = (it->second);
#ifdef _NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION
        quest_data.status_ = QS_NONE;
#else
        quest_data.status_ = QS_COMPLETED;
#endif //_NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION
        quest_data.start_datetime_ = 20110101000000;
        quest_data.end_datetime_   = 20110101000000;
    }
}

DWORD QuestManager::GetDailyQuestCompleteCountToday( \
    BYTE daily_quest_type, DWORD* OUT today_complete_count/* = NULL*/)
{
    int daily_quest_count = 0;
    if (today_complete_count != NULL)
    {
        *today_complete_count = 0;
    }

    CTime current_time = util::TimeSync::_time64(NULL);
    CTime last_daily_init_time = CTime(current_time.GetYear(), 
        current_time.GetMonth(), 
        current_time.GetDay(), 
        4, 
        0, 
        0);
    if (last_daily_init_time > current_time)
    {
        last_daily_init_time -= CTimeSpan(1, 0, 0, 0);
    }

    DAILY_QUEST_MAP::iterator begin = daily_quest_data_.begin();
    DAILY_QUEST_MAP::iterator end = daily_quest_data_.end();
    for ( ; begin != end; ++begin)
    {
        QUEST_DAILY_INFO& daily_info = (begin->second);

        QuestInfo* quest_info = GetQuestInfoManager()->FindQuestInfo(daily_info.code_);
        if (!quest_info || quest_info->GetDailyQuest() != daily_quest_type)
            continue;

        if (daily_info.status_ == QS_NORMAL)
            ++daily_quest_count; // �������� ��������Ʈ ī��Ʈ
        else if (daily_info.status_ == QS_COMPLETED)
        {
            SYSTEMTIME end_time_st;
            util::YYYYMMDDHHMMSSToSYSTEMTIME(daily_info.end_datetime_, end_time_st);
            CTime end_time = end_time_st;
            if (end_time > last_daily_init_time)
            {
                ++daily_quest_count; // ���� �Ϸ��� ��������Ʈ ī��Ʈ
                if (today_complete_count != NULL)
                {
                    ++(*today_complete_count);
                }
            }
        }
    }

    return daily_quest_count;
}

BOOL QuestManager::IsCompletedDailyQuest(QCODE questcode, QuestInfo* quest_info)
{
    DAILY_QUEST_MAP::iterator it = daily_quest_data_.find(questcode);
    if(it == daily_quest_data_.end())
    {
        // it's ok... can accept. - ó�� �����ϴ� ����Ʈ
        return FALSE;
    }

    QUEST_DAILY_INFO& quest_daily_info = it->second;
    if(quest_daily_info.status_ == QS_NORMAL)
        return TRUE; // ����Ʈ�� ���� �� ����.

    int daily_quest_init_day = quest_info->GetSetBackDay();
    if (daily_quest_init_day == 0)
        return RC::RC_QUEST_COMPLETED_DAILY_QUEST; // �ʱ�ȭ�� ���� �ʴ� ��������Ʈ

    CTime current_time = util::TimeSync::_time64(NULL);
    CTime last_daily_init_time = CTime(current_time.GetYear(), 
        current_time.GetMonth(), 
        current_time.GetDay(), 
        quest_info->GetSetBackTime() / 100, 
        quest_info->GetSetBackTime() % 100, 
        0);

    if (last_daily_init_time > current_time)
    {
        last_daily_init_time -= CTimeSpan(1, 0, 0, 0);
    }

    if ((daily_quest_init_day >= 2) &&
        (daily_quest_init_day <= 8))
    {
        daily_quest_init_day = (daily_quest_init_day == 8) ? 1 :daily_quest_init_day;
        daily_quest_init_day = last_daily_init_time.GetDayOfWeek() - daily_quest_init_day;
        if (daily_quest_init_day < 0)
            daily_quest_init_day += 7;

        last_daily_init_time -= CTimeSpan(daily_quest_init_day, 0, 0, 0);
    }

    
    SYSTEMTIME end_time_st;
    util::YYYYMMDDHHMMSSToSYSTEMTIME(quest_daily_info.end_datetime_, end_time_st);
    CTime end_time = end_time_st;
    if (end_time > last_daily_init_time)
        return TRUE; // �Ϸ� �� �ʱ�ȭ�� ���� ���� ��������Ʈ

    return FALSE;
}

BOOL QuestManager::IsCompletedDailyQuestGroup(WORD quest_group_code)
{
    DAILY_QUEST_MAP::iterator begin = daily_quest_data_.begin();
    DAILY_QUEST_MAP::iterator end   = daily_quest_data_.end();
    for ( ; begin != end; ++begin)
    {
        QUEST_DAILY_INFO& daily_info = (begin->second);
        QuestInfo* quest_info2 = GetQuestInfoManager()->FindQuestInfo(daily_info.code_);
        if ((quest_info2 != NULL) && 
            (quest_info2->GetSquadCode() == quest_group_code))
        {
            if (daily_info.status_ == QS_NORMAL)
                return TRUE;

            if ((daily_info.status_ == QS_COMPLETED) && 
                (IsCompletedDailyQuest(daily_info.code_,quest_info2) == TRUE))
                return TRUE;
        }
    }

    return FALSE;
}

// ��������Ʈ�� �� ���� �ִ°� ���°�...
// today : 0, yesterday : -1;
BOOL QuestManager::HaveDoneDailyQuest(int day)
{
    CTime current_time = util::TimeSync::_time64(NULL);
    CTime daily_init_time = CTime(current_time.GetYear(), current_time.GetMonth(), \
        current_time.GetDay(), 4, 0, 0); // ���ؽð� - 04:00

    if (daily_init_time > current_time)
        daily_init_time -= CTimeSpan(1, 0, 0, 0);

    CTime begin_t, end_t;
    begin_t = daily_init_time + CTimeSpan(day, 0, 0, 0);
    end_t   = begin_t + CTimeSpan(1, 0, 0, 0);

    DWORD64 begin64_t, end64_t;
    begin64_t = util::GetDateTime_YYYYMMDDHHMMSS(begin_t.GetTime());
    end64_t   = util::GetDateTime_YYYYMMDDHHMMSS(end_t.GetTime());

    DAILY_QUEST_MAP::iterator it  = daily_quest_data_.begin();
    DAILY_QUEST_MAP::iterator end = daily_quest_data_.end();
    for ( ; it != end; ++it)
    {
        QUEST_DAILY_INFO& daily_info = (it->second);
        if (daily_info.status_ != QS_COMPLETED)
            continue;

        if(daily_info.end_datetime_ >= begin64_t && daily_info.end_datetime_ < end64_t)
            return true;
    }

    return false;
}

//_NA_007909_20141218_SUNLOG_QUESTLIST
VOID QuestManager::PrintLogProgressQuestList()
{
    std::string quest_code_list;
    std::ostringstream ost;

    quest_code_list.clear();
    QUEST_HASH_ITR it, end(m_QuestHash.end());
    for( it = m_QuestHash.begin() ; it != end ; ++it )
    {
        ost.str("");
        ost << (*it)->GetQuestCode();
        quest_code_list += ost.str();
        quest_code_list += "/";
    }
    SUNLOG(eCRITICAL_LOG, "Player QuestList: %s",quest_code_list.c_str());
}

#endif //_NA_003027_20111013_HONOR_SYSTEM
