#include "StdAfx.h"
#include "Collection.h"
#include "CollectionStateInfo.h"

Collection::Collection()
	: m_pStateInfo(NULL)
	, m_pMapObjectInfo(NULL)
    , is_active_(true)
{
	SetObjectType(COLLECTION_OBJECT);
}

void Collection::Release()
{
	m_pStateInfo = NULL;
	m_pMapObjectInfo = NULL;
    is_active_ = true;
	NonCharacter::Release();
}

BOOL Collection::InitCollection( MAPOBJECT_INFO *pMapInfo )
{
	if( NULL == pMapInfo )
		return FALSE;

	m_pMapObjectInfo = pMapInfo;
	return NonCharacter::Init();
}

void Collection::GetRenderInfo( COLLECT_RENDER_INFO & OUT RenderInfo )
{
	RenderInfo.m_dwObjectKey  = GetObjectKey();
	RenderInfo.m_CollectionID = m_pMapObjectInfo->CollectionID;
}

void Collection::GetRenderInfo( COLLECT_PLAYER_RENDER_INFO & OUT RenderInfo )
{
	RenderInfo.m_bUseCollectRanderInfo = 1;

	RenderInfo.m_dwObjectKey  = GetObjectKey();
	RenderInfo.m_CollectionID = m_pMapObjectInfo->CollectionID;

	if( NULL == m_pStateInfo )
		RenderInfo.m_dwRemainTime = 0;
	else
		RenderInfo.m_dwRemainTime = m_pStateInfo->GetRemainedTime();
}
