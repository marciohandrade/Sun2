#include "StdAfx.h"
#include ".\questinfomanagerex.h"
#include "AppliedNpcDependencyChain.h"

QuestInfoManagerEx g_QuestInfoManager;

QuestInfoManagerEx::QuestInfoManagerEx(void)
{
}

QuestInfoManagerEx::~QuestInfoManagerEx(void)
{
}

VOID		QuestInfoManagerEx::Unload()
{
	ReleaseEx();
}




BOOL		QuestInfoManagerEx::Load( eSCRIPT_CODE ScriptCode, const TCHAR* pszFileName, BOOL bBigData, eSEPERATOR_MODE mode, const TCHAR* pszPackFileName, BOOL bReload )
{
	m_ScriptLoader.SetQuestInfoManager( this );
	m_ScriptLoader.SetEventRegister( AppliedNpcDependencyChain::Instance()->GetIEventRegisterForNpcKillByQuest() );
	return m_ScriptLoader.Load( ScriptCode, pszFileName, bBigData, mode, pszPackFileName, bReload );
}

BOOL		QuestInfoManagerEx::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	// 이곳은 안 들어와야 한다.
	return TRUE;
}

BOOL	QuestInfoManagerEx::SolarLoad( eSCRIPT_CODE ScriptCode, BYTE* pSTREAM, DWORD dwSize, BOOL bBigData )
{
	m_ScriptLoader.SetQuestInfoManager( this );
	m_ScriptLoader.SetEventRegister( AppliedNpcDependencyChain::Instance()->GetIEventRegisterForNpcKillByQuest() );	

	const char* pszFileName = FindFileName( ScriptCode );
	m_ScriptLoader.InsertFileName( ScriptCode,  pszFileName );
	return m_ScriptLoader.SolarLoad( ScriptCode, pSTREAM, dwSize, bBigData );
}

VOID		QuestInfoManagerEx::Release()
{
	Unload();
}
