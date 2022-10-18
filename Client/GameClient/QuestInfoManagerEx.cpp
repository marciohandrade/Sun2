#include "SunClientPrecompiledHeader.h"

#include "questinfomanagerex.h"

QuestInfoManagerEx g_QuestInfoManager;

QuestInfoManagerEx::QuestInfoManagerEx(void)
{
}

QuestInfoManagerEx::~QuestInfoManagerEx(void)
{
}

VOID QuestInfoManagerEx::Release()
{
	Unload();
}

VOID QuestInfoManagerEx::Unload()
{
	ReleaseEx();
}
	
BOOL  QuestInfoManagerEx::Load(eSCRIPT_CODE ScriptCode, const TCHAR* pszFileName, BOOL bBigData, eSEPERATOR_MODE mode, const TCHAR* pszPackFileName, BOOL bReload)
{
	m_ScriptLoader.SetQuestInfoManager(this);
	m_ScriptLoader.Load(ScriptCode, pszFileName, bBigData, mode, pszPackFileName, bReload);
	return TRUE;
}
