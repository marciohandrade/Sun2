#include "StdAfx.h"
#include "Guidance.h"
#include "GuidanceManager.h"

/*
// (CHANGES) (100220) (WAVERIX) remove unused section

GuidanceManager::GuidanceManager()
{
}

GuidanceManager::~GuidanceManager()
{
}

VOID GuidanceManager::Initialize( DWORD GuidancePoolSize )
{
	m_GuidancePoolRouter.Register<Guidance>(eGUIDANCE_NORMAL);
	m_GuidancePoolRouter.Initialize( eGUIDANCE_NORMAL, GuidancePoolSize, GuidancePoolSize/4 );
}

VOID GuidanceManager::Release()
{
	for( GUIDANCE_MAP::iterator it = m_mapGuidances.begin(); it != m_mapGuidances.end(); ++it )
	{
		Guidance *pGuidance = it->second;
		m_GuidancePoolRouter.Free( pGuidance->GetType(), pGuidance );
		pGuidance->Release();
	}

	m_mapGuidances.clear();
}

Guidance * GuidanceManager::FindGuidance( WORD GuidanceKey )
{
	GUIDANCE_MAP::iterator it = m_mapGuidances.find(GuidanceKey);
	if( it == m_mapGuidances.end() ) 
		return NULL;
	return it->second;
}

Guidance *	GuidanceManager::CreateGuidance( eGUIDANCE_TYPE eGuidanceType, WORD GuidanceKey, DWORD MasterKey )
{
	if( FindGuidance( GuidanceKey ) )	return NULL;

	Guidance *pGuidance = (Guidance*)m_GuidancePoolRouter.Alloc( eGuidanceType );
	pGuidance->Init( GuidanceKey, MasterKey );

	m_mapGuidances.insert( GUIDANCE_MAP::value_type(GuidanceKey, pGuidance) );

	return pGuidance;
}

RC::ePARTY_RESULT GuidanceManager::DestroyGuidance( WORD GuidanceKey )
{
	Guidance *pGuidance = FindGuidance( GuidanceKey );
	if( !pGuidance )
		return RC::RC_PARTY_GUIDANCE_NOTEXIST;

	m_mapGuidances.erase( GuidanceKey );
	m_GuidancePoolRouter.Free( pGuidance->GetType(), pGuidance );

	pGuidance->Release();

	return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT GuidanceManager::JoinGuidance( WORD GuidanceKey, WORD PartyKey, _PARTY_MEMBER_INFO & MemberInfo )
{
	Guidance *pGuidance = FindGuidance( GuidanceKey );
	if( !pGuidance )
		return RC::RC_PARTY_GUIDANCE_NOTEXIST;

	return pGuidance->JoinParty( PartyKey, MemberInfo );
}

RC::ePARTY_RESULT GuidanceManager::LeaveGuidance( WORD GuidanceKey, WORD PartyKey, DWORD MemberKey )
{
	Guidance *pGuidance = FindGuidance( GuidanceKey );
	if( !pGuidance )
		return RC::RC_PARTY_GUIDANCE_NOTEXIST;

	return pGuidance->LeaveParty( PartyKey, MemberKey );
}

RC::ePARTY_RESULT GuidanceManager::ChangeMaster( WORD GuidanceKey, WORD PartyKey, DWORD NextMasterKey )
{
	Guidance *pGuidance = FindGuidance( GuidanceKey );
	if( !pGuidance )
		return RC::RC_PARTY_GUIDANCE_NOTEXIST;

	return pGuidance->ChangeMaster( PartyKey, NextMasterKey );
}

RC::ePARTY_RESULT GuidanceManager::ChangeGuidanceMaster( WORD GuidanceKey, WORD PartyKey, DWORD NextMasterKey )
{
	Guidance *pGuidance = FindGuidance( GuidanceKey );
	if( !pGuidance )
		return RC::RC_PARTY_GUIDANCE_NOTEXIST;

	return pGuidance->ChangeGuidanceMaster( PartyKey, NextMasterKey );
}

*/
