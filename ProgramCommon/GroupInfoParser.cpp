#include "StdAfx.h"
#include "GroupInfoParser.h"

// implemented by __KR_001355_20090608_GROUPINGSYSTEM_PARSER

GroupInfoParser::GroupInfoParser(void)
{
	m_pGroupInfo	= NULL;
}

GroupInfoParser::~GroupInfoParser(void)
{
	Unload();
}

VOID GroupInfoParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );	
}

VOID GroupInfoParser::Reload()
{

}

VOID GroupInfoParser::Unload()
{
	SAFE_DELETE( m_pGroupInfo );
}

BOOL GroupInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_GROUPINFO:
		return _Load( bReload );
		break;
	}

	return FALSE;
}

BOOL GroupInfoParser::_Load( BOOL bReload )
{
	//__UNUSED(const INT nRowSize = GetRowSize(););
	int nRow			= 0;
	
	if( NULL == m_pGroupInfo )
	{
		// 없으면!
		m_pGroupInfo			= new tag_GroupInfo;
		ZeroMemory(m_pGroupInfo, sizeof(tag_GroupInfo));
	}
	else
	{
		if( FALSE == bReload )
		{
			SUNLOG( eCRITICAL_LOG, "Script File Data Error" );
			FASSERT( !"데이터에 오류가 있습니다." );
		}
	}

	m_pGroupInfo->flExpApplyScope		= GetDataBYTE( "ExpApplyScope", nRow);
	m_pGroupInfo->flHeimApplyScope		= GetDataBYTE( "HeimApplyScope", nRow);

	m_pGroupInfo->dwExpRewardRatio		= GetDataDWORD( "ExpRewardRatio", nRow);
	m_pGroupInfo->dwHeimRewardRatio	= GetDataDWORD( "HeimRewardRatio", nRow);

	return TRUE;
}
