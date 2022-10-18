#include "stdafx.h"
#include "ServerOptionParser.h"
#include <SolarFileReader.h>
#include <iostream>


ServerOptionParser::ServerOptionParser()
{
	memset(&m_ServerPoolOption, NULL, sizeof(m_ServerPoolOption));
}

ServerOptionParser::~ServerOptionParser()
{
}

VOID ServerOptionParser::Release()
{
}

VOID ServerOptionParser::Reload()
{
	LoadPoolInfo( m_pszPoolFileName, m_ServerType, TRUE );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL ServerOptionParser::LoadPoolInfo( char * pszFileName, BYTE byServerType, BOOL bReload )
{
	if( !bReload ) 
	{
		strncpy( m_pszPoolFileName, pszFileName, MAX_PATH );
		m_ServerType = (eSERVER_OPTION_TYPE)byServerType;
	}

	memset(&m_ServerPoolOption, NULL, sizeof(m_ServerPoolOption));

	// ���Ͽ���
	util::SolarFileReader sr;
	if ( !sr.OpenFile( m_pszPoolFileName, SEPERATOR_WHITESPACE ) )
	{
		ASSERT( !"File Open Error : ServerPoolOption.ini" );
		return (FALSE);
	}

	//������������������������������������������������������������������������������
	// ���� üũ
	//������������������������������������������������������������������������������
	const double SERVER_POOL_OPTION_PARSER_VERSION = 1.1;

	sr.GetNextTokenType(TOKEN_NUMBER);
	double FileVer = sr.GetTokenNumber();

	if( FileVer != SERVER_POOL_OPTION_PARSER_VERSION )
	{
		std::cout << sr.GetFileName() << " Version is different! ParserVer:" << SERVER_POOL_OPTION_PARSER_VERSION << ",ScriptVer:" << FileVer << std::endl;
	}
	//������������������������������������������������������������������������������

	while( TRUE )
	{
		eTOKEN_TYPE ettType = sr.GetNextTokenType(); 
		if ( TOKEN_END == ettType )
		{
			break;
		}
		else if ( TOKEN_NUMBER != ettType )
		{
			ASSERT(!"�߸��� ��ũ��Ʈ");
			return FALSE;
		}

		// �Ϲ� Ǯ
		m_ServerPoolOption.m_byIndex = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_STRING); strncpy( m_ServerPoolOption.m_szServerName, sr.GetTokenString(), 16 );
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wPlayerPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wNPCPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wItemPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wMapObjectPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wMapNPCPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wMerchantPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wPlayerSkillPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wNPCSkillPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wAbilityPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wPartyPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wSummonPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wStatusPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wCoolTimerCharPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wMapPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wSectorPool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wTradePool = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wObjGroupPool = sr.GetTokenNumber();

		if( FileVer >= 1.1 )
		{
			sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wItemSlotPool = sr.GetTokenNumber();
			sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wSkillSlotPool = sr.GetTokenNumber();
			sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wQuickSlotPool = sr.GetTokenNumber();
			sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wQuickStyleSlotPool = sr.GetTokenNumber();
			sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wTradeSlotPool = sr.GetTokenNumber();
			sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wVendorSlotPool = sr.GetTokenNumber();
			sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wEventSlotPool = sr.GetTokenNumber();
		}
		else
		{
			m_ServerPoolOption.m_wItemSlotPool = 10000;
			m_ServerPoolOption.m_wSkillSlotPool = 10000;
			m_ServerPoolOption.m_wQuickSlotPool = 5000;
			m_ServerPoolOption.m_wQuickStyleSlotPool = 2000;
			m_ServerPoolOption.m_wTradeSlotPool = 100;
			m_ServerPoolOption.m_wVendorSlotPool = 100;
			m_ServerPoolOption.m_wEventSlotPool = 50;
		}

		// �ļ� Ǯ
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wItemInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wNPCInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wMapInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wShopInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wSkillInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wRegenInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wGroupInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wStateInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wItemCompInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wItemCompResultInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wMissionRewardInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wFormulaRatioInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wStyleQuickInfo = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); m_ServerPoolOption.m_wAITypeInfo = sr.GetTokenNumber();

		if( m_ServerPoolOption.m_byIndex == byServerType )	break;
	}

	sr.CloseFile();

	return TRUE;

}




#pragma warning ( pop )




