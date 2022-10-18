
#include "stdafx.h"
#include "NumericEngine.h"
/*
#include <Library\CalcEngine\Numeric.h>

#include "GameServerEx.h"


NumericEngine::NumericEngine()
{
	m_pParameterEngine	= NULL;
	m_pStatusEngine		= NULL;

}

NumericEngine::~NumericEngine()
{



}


BOOL NumericEngine::Init()
{
	m_pStatusEngine		= (CStatusEngine*)g_pGameServer->m_fnContactNumerical( CN_NEW_STATUS_ENGINE, NULL );
	if( NULL == m_pStatusEngine )
	{
		SUNLOG( eCRITICAL_LOG, "[NumericEngine::Init] [MultiplyExpFormula] CStatusEngine Init Failed!!" ); 
		return FALSE;
	}

	m_pParameterEngine	= (CParameterEngine*)g_pGameServer->m_fnContactNumerical( CN_NEW_PARAMETER_ENGINE, NULL );
	if( NULL == m_pParameterEngine )
	{
		SUNLOG( eCRITICAL_LOG, "[NumericEngine::Init] [MultiplyExpFormula] CParameterEngine Init Failed!!" ); 
		return FALSE;
	}

	return TRUE;
}

VOID NumericEngine::Release()
{
	g_pGameServer->m_fnContactNumerical( CN_DEL_STATUS_ENGINE,	NULL );
	g_pGameServer->m_fnContactNumerical( CN_DEL_PARAMETER_ENGINE, NULL );
}

*/
