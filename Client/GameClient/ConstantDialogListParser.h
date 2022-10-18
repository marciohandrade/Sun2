#pragma once

#include <Singleton.h>
#include <ScriptCode.IParser.h>

class ConstantDialogListParser	: public Singleton<ConstantDialogListParser> ,public IParser
{
	typedef std::set<DWORD>				DIALOG_LIST;
	typedef std::set<DWORD>::iterator	DIALOG_LIST_ITR;
public:
	ConstantDialogListParser(void);
	~ConstantDialogListParser(void);

	void Init(char * pszPackFileName);
	void Release(){ Unload(); };

	void Unload();

public:
	BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );	

	BOOL IsFixedDialog( DWORD dwDlgID );
private:
	BOOL _Load( BOOL bReload );

	DIALOG_LIST m_FixedDialogList;
};
