
#include "stdafx.h"
#include "MultiplyExpFormula.h"
/*
#include "GameServerEx.h"


MultiplyExpFormula::MultiplyExpFormula()
{
	m_pStatusObject		= NULL;
}


MultiplyExpFormula::~MultiplyExpFormula()
{

}

BOOL MultiplyExpFormula::Init()
{
	return NumericEngine::Init();
}

VOID MultiplyExpFormula::Release()
{
	g_pGameServer->m_fnContactNumerical( CN_DEL_STATUS, m_pStatusObject );
	NumericEngine::Release();
}



BOOL MultiplyExpFormula::Load(char* pFilePath)
{
//	char* pFilePath = "data\\Formula\\MultiplyExpFormula.txt";

	// 수치엔진 스크립트 로딩
	if( FALSE == m_pStatusEngine->ReadFile(pFilePath) )
	{
		SUNLOG( eCRITICAL_LOG, "[MultiplyExpFormula::Load] Script File Load Failed!!");
		return FALSE;
	}

	// *.txt파일을 읽어 *.wzn파일로 변환
	m_pStatusEngine->Convert(pFilePath);

	m_pStatusObject = (CStatusObject*)g_pGameServer->m_fnContactNumerical(CN_NEW_STATUS, m_pStatusEngine);


	return TRUE;
}


BOOL MultiplyExpFormula::OnExcute()
{
	m_pStatusEngine->SetVariable(m_pStatusObject, "Exp", &m_iExp, NUMERICAL_INT);

	m_pStatusEngine->GetVariable_Calc( m_pStatusObject, "FinalExp", &m_iFinalExp, NUMERICAL_INT );

	return TRUE;
}


*/

