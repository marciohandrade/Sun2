#pragma once

//=======================================================================================================================
/// QuestInfoManagerEx class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 3. 29
	@remarks
			- QuestInfo를 관리하고, QuestScriptLoader를 가진 싱글 객체
	@history
			- 
*/
//=======================================================================================================================

#include <questinfomanager.h>
#include <QuestScriptLoader.h>

class QuestInfoManagerEx :
	public QuestInfoManager, public IParser
{
public:
	QuestInfoManagerEx(void);
	virtual ~QuestInfoManagerEx(void);

	virtual BOOL	LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload ){return TRUE;}
	virtual BOOL	Load(eSCRIPT_CODE ScriptCode, const TCHAR* pszFileName, BOOL bBigData, eSEPERATOR_MODE mode, const TCHAR* pszPackFileName, BOOL bReload);
	virtual VOID	Release();

	VOID Unload();

private:
	QuestScriptLoader m_ScriptLoader;
};

extern QuestInfoManagerEx g_QuestInfoManager;
