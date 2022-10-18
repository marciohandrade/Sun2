#pragma once

/*
// (CHANGES) (100220) (WAVERIX) remove unused section

#include <map>

class Guidance;

class GuidanceManager
{
public:
	typedef std::map<WORD,Guidance*>	GUIDANCE_MAP;

	GuidanceManager();
	virtual ~GuidanceManager();

	virtual VOID Initialize( DWORD GuidancePoolSize );
	virtual VOID Release();

	Guidance *					FindGuidance( WORD GuidanceKey );
	virtual Guidance *			CreateGuidance( eGUIDANCE_TYPE eGuidanceType, WORD GuidanceKey, DWORD MasterKey );
	virtual RC::ePARTY_RESULT	DestroyGuidance( WORD GuidanceKey );
	virtual RC::ePARTY_RESULT	JoinGuidance( WORD GuidanceKey, WORD PartyKey, _PARTY_MEMBER_INFO & MemberInfo );
	virtual RC::ePARTY_RESULT	LeaveGuidance( WORD GuidanceKey, WORD PartyKey, DWORD MemberKey );
	virtual RC::ePARTY_RESULT	ChangeMaster( WORD GuidanceKey, WORD PartyKey, DWORD NextMasterKey );
	virtual RC::ePARTY_RESULT	ChangeGuidanceMaster( WORD GuidanceKey, WORD PartyKey, DWORD NextMasterKey );

protected:
	PoolRouter<eGUIDANCE_MAX> 	m_GuidancePoolRouter;
	GUIDANCE_MAP				m_mapGuidances;
};

*/
