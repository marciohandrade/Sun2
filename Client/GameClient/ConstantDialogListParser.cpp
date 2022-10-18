#include "SunClientPrecompiledHeader.h"
#include "constantdialoglistparser.h"

ConstantDialogListParser::ConstantDialogListParser(void)
{
}

ConstantDialogListParser::~ConstantDialogListParser(void)
{
}

void ConstantDialogListParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );
	m_FixedDialogList.clear();
}

void ConstantDialogListParser::Unload()
{
	m_FixedDialogList.clear();
}

BOOL ConstantDialogListParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_CONSTANT_DIALOG:
		return _Load( bReload );
		break;
	}

	return FALSE;
}

BOOL ConstantDialogListParser::_Load( BOOL bReload )
{
	const INT nRowSize = GetRowSize();
	char szWZID[5] = {0, };
	DWORD dwDlgID = 0;
	for( INT nRow=0; nRow<nRowSize ; ++nRow )
	{
		strncpy( szWZID, GetDataString( "DlgID", nRow ), 5 );
		dwDlgID = StrToWzID(szWZID);
		m_FixedDialogList.insert(dwDlgID);
	}

	return TRUE;
}

BOOL ConstantDialogListParser::IsFixedDialog( DWORD dwDlgID )
{
	DIALOG_LIST_ITR itr = m_FixedDialogList.find(dwDlgID);
	if ( itr == m_FixedDialogList.end() )
	{
		return FALSE;
	}
	return TRUE;
}
