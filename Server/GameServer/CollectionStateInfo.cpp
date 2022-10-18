#include "stdafx.h"
#include "CollectionStateInfo.h"
#include "CollectionParser.h"
#include "Collection.h"

#include <ServerOptionParserEx.h>

__IMPL_CUSTOMPOOL_PTR( CollectionStateInfo )

CollectionStateInfo::CollectionStateInfo()
	: m_Collector(0)
	, m_stat(collectable)
	, m_pData(NULL)
	, m_pCollection(NULL)
	, m_pMapObjectInfo(NULL)
{
	SetVector(&m_CollectorPos,0,0,0);
	m_Timer.DisableCheckTime();
}

bool CollectionStateInfo::Init( Collection* pCollection, CHARGUID Collector, WzVector& CollectorPos )
{
	MAPOBJECT_INFO* pMapInfo = pCollection->GetMapObjectInfo();
	m_pData	= _FindCollectionInfo(pMapInfo->CollectionID);
	if( NULL == m_pData )
	{
		char ch[5] = {0,};
		SUNLOG( eCRITICAL_LOG, "CollectionStateInfo::Init ��ũ��Ʈ���� (%s)������ ã���� �����ϴ�.", WzIDToString(ch,pMapInfo->CollectionID) );
		return false;
	}

	m_pMapObjectInfo = pMapInfo;
	m_pCollection = pCollection;

	m_Collector = Collector;
	m_CollectorPos = CollectorPos;
	
	return true;
}


void CollectionStateInfo::Release() 
{
	m_pData	= NULL;
	m_pCollection = NULL;
	m_pMapObjectInfo = NULL;

	m_stat = collectable;
	m_Collector = 0;
	SetVector(&m_CollectorPos,0,0,0);

	m_Timer.DisableCheckTime();
}

bool CollectionStateInfo::StartCollect() 
{
	return collectable == m_stat ? _ChangeStat() : false; 
}

bool CollectionStateInfo::StartRespawn() 
{ 
	if( Collected == m_stat )
	{
		m_pCollection = NULL;
		m_Collector = 0;
		SetVector(&m_CollectorPos,0,0,0);
		
		return _ChangeStat();
	}
	return false;
}


// ä����,ä���Ϸῡ���� ���.

bool CollectionStateInfo::CanCancel(CHARGUID Collector)
{
	if( Collecting != m_stat && 
		Collected  != m_stat )
		return false;

	if( Collector != m_Collector )
		return false;

	return true;
}

void CollectionStateInfo::Cancel()
{
	m_stat = cancel;
	_ChangeStat();
}

// Active() ȣ��
// 0-	false����	���°� : collectable (ä������)
// 1-	false����	���°� : Collecting	 (ä����)
// 2-	true����	���°� : Collecting	 (ä���Ϸ�)

// Active() ȣ��
// 3-	false����	���°� : Collected	 (ä������, ��������)
// 4-	false����	���°� : Respawning  (������) 
// 5-	true����	���°� : Respawning  (�����Ϸ�) 

// 6-	false����	���°� : cancel		 (���) 

bool CollectionStateInfo::update(BYTE* stat)
{
	bool retval = false;
	*stat = m_stat;

	if( m_Timer.IsExpired() )
        retval = _ChangeStat();

	return retval;
}


bool CollectionStateInfo::_ChangeStat()
{
    // m_DelayCollectionRespawnTime ��� ���� :
	// ä������ Ȱ��ȭ ��Ŷ�� �ٷ� ������(������ �ð� '0' ) Ŭ�󿡼� ���� �ɼ� ����. (Ŭ�󿡼� ä�� �Ϸ� �����)
    // Ŭ�󿡼� �Ϸ� ��Ŷ�� �ʰ� �������(������ �ð� '0' ) ä�� ���� ��Ŷ��(����ǥ) Ŭ�� ���� ����.  

	static const DWORD& Delay = ServerOptionParserEx::Instance()->GetServerOption().m_DelayCollectionRespawnTime;

	switch(m_stat)
	{
	case collectable:				// ä�� ���۽�
		m_Timer.SetTimer( m_pData->m_dwCollectingTime );
		break;

	case Collecting:				// ä�� �Ϸ��
		m_Timer.DisableCheckTime();
		break;

	case Collected:					//������ ���۽�
		m_Timer.SetTimer( m_pData->m_dwRespawnTime + Delay );
		break;

	case Respawning:				//������ �Ϸ��
		m_Timer.DisableCheckTime();
		break;

	default:	//case cancel:
		m_Timer.DisableCheckTime();
		m_stat = cancel;
		return false;
	}

	if( spawn == ++m_stat )			//������ �Ϸ�� -> ��������==ä�����ɻ���
		m_stat = collectable;

	return true;
}

CollectionInfo* CollectionStateInfo::_FindCollectionInfo( WzID CollectionID ) 
{
	return const_cast<CollectionInfo*>(CollectionParser::Instance()->FindCollectionInfo(CollectionID)); 
}

