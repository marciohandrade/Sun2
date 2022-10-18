#include "stdafx.h"

#ifndef __NA_000000_20071106_SERVER_OPTION_CHANGE

//
#include "OptLogicByNation.h"

//////////////////////////////////////////////////////////////////////////
// <MODULE VARIABLEs>
OptLogicByNation*	OptLogicByNation::ms_pInstance = NULL;

//////////////////////////////////////////////////////////////////////////
// <METHOD IMPLEMENTATION>
OptLogicByNation::OptLogicByNation()
{
}

OptLogicByNation::~OptLogicByNation()
{
}

VOID	OptLogicByNation::CreateInstance( eNATION_CODE eNationCode )
{
	if( !ms_pInstance )
	{
		ms_pInstance = new OptLogicByNation;
		ms_pInstance->_InitializeByNationCode( eNationCode );
	}
}

VOID	OptLogicByNation::DestroyInstance()
{
	SAFE_DELETE( ms_pInstance );
}

typedef VOID (OptLogicByNation::*__INITIALIZER)();
VOID	OptLogicByNation::_InitializeByNationCode( eNATION_CODE eNationCode )
{
	memset( &AGENT.m_INFO, 0, sizeof(AGENT.m_INFO) );
	memset( &GAME.m_INFO, 0, sizeof(GAME.m_INFO) );
	memset( &WORLD.m_INFO, 0, sizeof(WORLD.m_INFO) );
	memset( &DBPROXY.m_INFO, 0, sizeof(DBPROXY.m_INFO) );
	memset( &MASTER.m_INFO, 0, sizeof(MASTER.m_INFO) );

	__INITIALIZER	INITIALIZER = NULL;

	switch( eNationCode )
	{
	case eNATION_KOREA:
		INITIALIZER = __Initialize__KR;
		break;
	case eNATION_TAIWAN:
		INITIALIZER = __Initialize__TW;
		break;
	case eNATION_CHINA:
		INITIALIZER = __Initialize__CN;
		break;
	case eNATION_JAPAN:
		INITIALIZER = __Initialize__JP;
		break;
	}

	if( INITIALIZER == NULL )
	{
		ASSERT( !"Invalid NationCode" );
		return;
	}

	// 공통 로직
	__Initialize__KR();

	if( __Initialize__KR != INITIALIZER )
		(this->*INITIALIZER)();
}

VOID	OptLogicByNation::__Initialize__KR()
{
	m_eNationCode = eNATION_KOREA;
	// AGENT
	do {
		OptLogicAgent::sOptInfo& rINFO = AGENT.m_INFO;
		rINFO.m_bCanApplyPCBangBenefit = TRUE;
	} while(0);

	// GAME
	do {
		OptLogicGame::sOptInfo& rINFO = GAME.m_INFO;
		rINFO.m_bCanApplyPCBangBenefit = TRUE;
		rINFO.m_fMasterBonusExp = 0.2f;
		rINFO.m_fNoMasterBonusExp = 0.1f;
		rINFO.m_bCheckLevelOwnership = TRUE;
	} while(0);
}

VOID	OptLogicByNation::__Initialize__JP()
{
	m_eNationCode = eNATION_JAPAN;
}

VOID	OptLogicByNation::__Initialize__TW()
{
	m_eNationCode = eNATION_TAIWAN;
}

VOID	OptLogicByNation::__Initialize__CN()
{
	m_eNationCode = eNATION_CHINA;
	// AGENT
	{

		OptLogicAgent::sOptInfo& rINFO = AGENT.m_INFO;
		rINFO.m_bCanApplyPCBangBenefit = FALSE;

	}

	// GAME
	{
	
		OptLogicGame::sOptInfo& rINFO = GAME.m_INFO;
		rINFO.m_bCanApplyPCBangBenefit = FALSE;
		rINFO.m_fMasterBonusExp = 0.5f;
		rINFO.m_fNoMasterBonusExp = 0.2f;
		rINFO.m_bCheckLevelOwnership = FALSE;
	}
}

VOID	OptLogicByNation::__Initialize__UK()
{
	m_eNationCode = eNATION_KOREA;	// eNATION_CODE에 값이 없다(070506)
}

#endif	//__NA_000000_20071106_SERVER_OPTION_CHANGE


