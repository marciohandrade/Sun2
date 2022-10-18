#include "StdAfx.h"
#include ".\zonefactory.h"
#include "BaseZone.h"
#include <assert.h>

ChatZoneFactory::ChatZoneFactory(void)
{
	m_pZonePool = NULL;
}

ChatZoneFactory::~ChatZoneFactory(void)
{
	assert( m_pZonePool == NULL );
}

VOID ChatZoneFactory::Init()
{
	m_pZonePool			= new util::CMemoryPoolFactory<ChatZone>;
	//m_pZonePool->Initialize( 5000, 5000 );	// �ִ� 1�δ� 1������
	m_pZonePool->Initialize( 50, 50 );	// �ִ� 1�δ� 1������
}

VOID ChatZoneFactory::Release()
{
	SAFE_DELETE( m_pZonePool );
}

ChatZone* ChatZoneFactory::AllocZone()
{
	ChatZone *pZone = (ChatZone *)m_pZonePool->Alloc();
	pZone->Init();

	return pZone;
}

VOID ChatZoneFactory::FreeZone( ChatZone *pZone )
{
	pZone->Release();

	m_pZonePool->Free( pZone );
}

ViewPortZoneFactory::ViewPortZoneFactory(void)
{
	m_pZonePool = NULL;
}

ViewPortZoneFactory::~ViewPortZoneFactory(void)
{
	assert( m_pZonePool == NULL );
}

VOID ViewPortZoneFactory::Init()
{
	m_pZonePool			= new util::CMemoryPoolFactory<ViewPortZone>;
	//m_pZonePool->Initialize( 100, 10 );	// ������ ��� �ְڳ�?
	m_pZonePool->Initialize( 100, 100 );	// ������ ��� �ְڳ�?
}

VOID ViewPortZoneFactory::Release()
{
	SAFE_DELETE( m_pZonePool );
}

ViewPortZone* ViewPortZoneFactory::AllocZone()
{
	ViewPortZone *pZone = (ViewPortZone *)m_pZonePool->Alloc();
	pZone->Init();

	return pZone;
}

VOID ViewPortZoneFactory::FreeZone( ViewPortZone *pZone )
{
	pZone->Release();

	m_pZonePool->Free( pZone );
}

/////////////////////////////////
//VillageZone
VillageZoneFactory::VillageZoneFactory(void)
{
	m_pZonePool = NULL;
}

VillageZoneFactory::~VillageZoneFactory(void)
{
	assert( m_pZonePool == NULL );
}

VOID VillageZoneFactory::Init()
{
	m_pZonePool			= new util::CMemoryPoolFactory<VillageZone>;
	//m_pZonePool->Initialize( 100, 10 );	// ������ ��� �ְڳ�?
	m_pZonePool->Initialize( 20, 10 );	// ������ ��� �ְڳ�?
}

VOID VillageZoneFactory::Release()
{
	SAFE_DELETE( m_pZonePool );
}

VillageZone* VillageZoneFactory::AllocZone()
{
	VillageZone *pZone = (VillageZone *)m_pZonePool->Alloc();

	return pZone;
}

VOID VillageZoneFactory::FreeZone( VillageZone *pZone )
{
	pZone->Release();

	m_pZonePool->Free( pZone );
}
