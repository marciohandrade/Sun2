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
		SUNLOG( eCRITICAL_LOG, "CollectionStateInfo::Init 스크립트에서 (%s)정보를 찾을수 없습니다.", WzIDToString(ch,pMapInfo->CollectionID) );
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


// 채집중,채집완료에러시 취소.

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

// Active() 호출
// 0-	false리턴	상태값 : collectable (채집가능)
// 1-	false리턴	상태값 : Collecting	 (채집중)
// 2-	true리턴	상태값 : Collecting	 (채집완료)

// Active() 호출
// 3-	false리턴	상태값 : Collected	 (채집했음, 스폰가능)
// 4-	false리턴	상태값 : Respawning  (스폰중) 
// 5-	true리턴	상태값 : Respawning  (스폰완료) 

// 6-	false리턴	상태값 : cancel		 (취소) 

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
    // m_DelayCollectionRespawnTime 사용 이유 :
	// 채집물의 활성화 패킷을 바로 보내면(리스폰 시간 '0' ) 클라에서 무시 될수 있음. (클라에서 채집 완료 모션중)
    // 클라에서 완료 패킷을 늦게 보낼경우(리스폰 시간 '0' ) 채집 가능 패킷이(느낌표) 클라에 갈수 있음.  

	static const DWORD& Delay = ServerOptionParserEx::Instance()->GetServerOption().m_DelayCollectionRespawnTime;

	switch(m_stat)
	{
	case collectable:				// 채집 시작시
		m_Timer.SetTimer( m_pData->m_dwCollectingTime );
		break;

	case Collecting:				// 채집 완료시
		m_Timer.DisableCheckTime();
		break;

	case Collected:					//리스폰 시작시
		m_Timer.SetTimer( m_pData->m_dwRespawnTime + Delay );
		break;

	case Respawning:				//리스폰 완료시
		m_Timer.DisableCheckTime();
		break;

	default:	//case cancel:
		m_Timer.DisableCheckTime();
		m_stat = cancel;
		return false;
	}

	if( spawn == ++m_stat )			//리스폰 완료시 -> 스폰상태==채집가능상태
		m_stat = collectable;

	return true;
}

CollectionInfo* CollectionStateInfo::_FindCollectionInfo( WzID CollectionID ) 
{
	return const_cast<CollectionInfo*>(CollectionParser::Instance()->FindCollectionInfo(CollectionID)); 
}

