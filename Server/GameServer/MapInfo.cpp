#include "stdafx.h"
#include "MapInfo.h"
#include "MovingPlanner.h"

namespace nsAI
{


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


	//class PlanEventObject
	//{
	//public:		PlanEventObject(
	//				DWORD&				rEventField
	//				, CPathExplorer*&	pPathExplorer
	//				, SkillManager*&	pSkillManager
	//				);

	//public:		VOID					Init( Character* const pCharacter );
	//			VOID					Release();

	//public:
	//private:	DWORD&					m_rEventField;
	//			CPathExplorer*&			m_pPathExplorer;
	//			SkillManager*&			m_pSkillManager;
	//			TileEvent				m_TileEventInfo;
	//};


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式







//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

IPlanEventObject::IPlanEventObject(
					CPathExplorer*&		pPathExplorer
					, SkillManager*&	pSkillManager
					, StatusManager*&	pStatusManager
					)
	: EventField(m_EventField)
	, m_EventField(DISABLED_ALL)
	, m_pHandler( new PlanEventObject( m_EventField, pPathExplorer, pSkillManager, pStatusManager ) )
{
}

IPlanEventObject::~IPlanEventObject()
{
	SAFE_DELETE( m_pHandler );
}


VOID
IPlanEventObject::Init( Character* const pCharacter )
{
	m_EventField = DISABLED_ALL;
	m_pHandler->Init( pCharacter );
}


VOID
IPlanEventObject::Release()
{
	m_pHandler->Release();
	m_EventField = DISABLED_ALL;
}


}; // end namespace nsAI

