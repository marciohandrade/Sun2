#include "StdAfx.h"
#include "PKScriptParser.h"
#include <Struct.h>
#include <SolarFileReader.h>

#include <iostream>


#define PARSER_END_ROW_KEY	7777

PKScriptParser::PKScriptParser()
{
}

PKScriptParser::~PKScriptParser()
{
}

VOID PKScriptParser::Reload()
{
//	Load( m_pszFileName, TRUE );
}


#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL PKScriptParser::_Load( BOOL bReload )
{
    __UNUSED(bReload);
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		m_PKOptionInfo.m_PKAllowableLV = GetDataNumber( "PK_Use", nRow );
		m_PKOptionInfo.m_PKProtectLV   = GetDataNumber( "PK_Protect", nRow );

		m_PKOptionInfo.m_BadFriendDropRatio = GetDataNumber( "NItemDrop", nRow );
		m_PKOptionInfo.m_PreChaoDropRatio	= GetDataNumber( "PrecaoItemDrop", nRow );
		m_PKOptionInfo.m_ChaoDropRatio		= GetDataNumber( "CaoItemDrop", nRow );
		m_PKOptionInfo.m_EquipDropRatio		= GetDataNumber( "EquipDropRatio", nRow );

		m_PKOptionInfo.m_PreChaoTime = GetDataNumber( "TimeA", nRow );
		m_PKOptionInfo.m_ChaoTime	 = GetDataNumber( "TimeB", nRow );
		m_PKOptionInfo.m_AddChaoTime = GetDataNumber( "TimeC", nRow );

		m_PKOptionInfo.m_BoradoriAttackTime  = GetDataNumber( "Attack_Time", nRow );
		m_PKOptionInfo.m_BoradoriAttackTime *= 1000;	// Tick ������ ȯ���Ѵ�.
		m_PKOptionInfo.m_BoradoriNormalTime  = GetDataNumber( "Normal_Time", nRow );
		m_PKOptionInfo.m_BoradoriNormalTime *= 1000;	// Tick ������ ȯ���Ѵ�.

        m_PKOptionInfo.m_IgnorePKTile = GetDataNumber( "Ignore_PKTile", nRow );
        //__NA_0_POSSESSION_ITEM_DROP
        m_PKOptionInfo.m_ePossessionDropOption = (PKOptionInfo::ePoessessionDropOption)GetDataInt( "Drop_SBItems", nRow );

		// ������ ���� ǥ���ϴ� Ű
		m_PKOptionInfo.m_wEND_ROW_KEY = GetDataNumber( "End_Row_Key", nRow );
		if( m_PKOptionInfo.m_wEND_ROW_KEY != PARSER_END_ROW_KEY )
		{
			SUNLOG(  eCRITICAL_LOG, "[PKScriptParser::Load] ��ũ��Ʈ �� ������ ��������!" );
			return FALSE;
		}
	}

	if( !CheckValidity() )
		return FALSE;

	return TRUE;
}

BOOL PKScriptParser::CheckValidity()
{
	BOOL bSuccess = TRUE;

	// ��ȣ������ 10�̸��̸� Ȥ�� ��ġ�Է� �Ǽ�?
	if( m_PKOptionInfo.m_PKProtectLV < 10 )
	{
		SUNLOG( eCRITICAL_LOG, "[PKScriptParser] m_PKProtectLV[%d] is very low! \n", m_PKOptionInfo.m_PKProtectLV );
		bSuccess = FALSE;
	}

	// ������ ���Ȯ���� 0~100 ���� ���̴�.
	if( m_PKOptionInfo.m_BadFriendDropRatio <= 0 || m_PKOptionInfo.m_BadFriendDropRatio > 100 )
	{
		SUNLOG( eCRITICAL_LOG, "[PKScriptParser] m_BadFriendDropRatio[%d] is Invalid! \n", m_PKOptionInfo.m_BadFriendDropRatio );
		bSuccess = FALSE;
	}
	if( m_PKOptionInfo.m_PreChaoDropRatio <= 0 || m_PKOptionInfo.m_PreChaoDropRatio > 100 )
	{
		SUNLOG( eCRITICAL_LOG, "[PKScriptParser] m_PreChaoDropRatio[%d] is Invalid! \n", m_PKOptionInfo.m_PreChaoDropRatio );
		bSuccess = FALSE;
	}
	if( m_PKOptionInfo.m_ChaoDropRatio <= 0 || m_PKOptionInfo.m_ChaoDropRatio > 100 )
	{
		SUNLOG( eCRITICAL_LOG, "[PKScriptParser] m_ChaoDropRatio[%d] is Invalid! \n", m_PKOptionInfo.m_ChaoDropRatio );
		bSuccess = FALSE;
	}

	// ī�� �����ð��� 10�ð� �̸�, 100�ð� �̻��̸� Ȥ�� ��ġ�Է� �Ǽ�?
	//if( m_PKOptionInfo.m_PreChaoTime < 10*3600 || m_PKOptionInfo.m_PreChaoTime >= 100*3600 )
	//{
	//	SUNLOG( eCRITICAL_LOG, "[PKScriptParser] m_PreChaoTime[%d] is Invalid! \n", m_PKOptionInfo.m_PreChaoTime );
	//	bSuccess = FALSE;
	//}
	//if( m_PKOptionInfo.m_ChaoTime < 10*3600 || m_PKOptionInfo.m_ChaoTime >= 100*3600 )
	//{
	//	SUNLOG( eCRITICAL_LOG, "[PKScriptParser] m_ChaoTime[%d] is Invalid! \n", m_PKOptionInfo.m_ChaoTime );
	//	bSuccess = FALSE;
	//}
	//if( m_PKOptionInfo.m_AddChaoTime >= 50*3600 )
	//{
	//	SUNLOG( eCRITICAL_LOG, "[PKScriptParser] m_AddChaoTime[%d] is Invalid! \n", m_PKOptionInfo.m_AddChaoTime );
	//	bSuccess = FALSE;
	//}

	return bSuccess;
}

#pragma warning ( pop )

VOID	PKScriptParser::Release()
{
	Unload();
}

VOID	PKScriptParser::Unload()
{

}


BOOL	PKScriptParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_PKSCRIPT:
		return _Load( bReload );
	}

	return FALSE; 
}

