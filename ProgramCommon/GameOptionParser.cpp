#include "StdAfx.h"
#include ".\gameoptionparser.h"

GameOptionParser::GameOptionParser(void)
{
	ZeroMemory( &m_GameOption, sizeof( m_GameOption ) );
}

GameOptionParser::~GameOptionParser(void)
{
}

VOID	GameOptionParser::Release()
{
	ZeroMemory( &m_GameOption, sizeof( m_GameOption ) );
}

BOOL GameOptionParser::CheckMaxLevel(LEVELTYPE nLevel)
{
	if( m_GameOption.m_byMaxLevel < nLevel )
		return FALSE;

	return TRUE;
}

BOOL GameOptionParser::ShoutLevelCheck(const GAME_OPTION& opt, LEVELTYPE charLev)
{
	return !(opt.m_byNormalShoutLevel == 0 || opt.m_byNormalShoutLevel > charLev );
}

BOOL	GameOptionParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_GAME_OPTION:
		return _Load( bReload );
	}

	return FALSE;
}

BOOL	GameOptionParser::_Load( BOOL bReload )
{
    __UNUSED(bReload);
	m_GameOption.m_byMaxLevel = GetINIDataInt( "GAME_OPTION", "MAX_LEVEL" );					                //������ �ƽ� ����
	m_GameOption.m_MaxLevelChangeOfClass1st = GetINIDataInt( "GAME_OPTION", "MAX_LEVEL_CHANGE_OF_CLASS_1st" );  //���� 1�� Changeup�� Max Level
	m_GameOption.m_byNormalShoutLevel = GetINIDataInt( "GAME_OPTION", "NORMAL_SHOUT_LEVEL" );	                //�Ϲ� ��ġ�� ����
	m_GameOption.m_byWorldShoutPCBANG = GetINIDataInt( "GAME_OPTION", "WORLD_SHOUT_PCBANG" );	                //PC�濡�� ���� ��ġ�⸦ ����� ���ΰ�?
#ifdef __NA_0_20080515_BUG_FIX_WORLD_SHOUT_LEVEL
	m_GameOption.m_byWorldShoutLevel = GetINIDataInt( "GAME_OPTION", "WORLD_SHOUT_LEVEL" );	                    //���� ��ġ�� ����
#endif
    //_NA_0062_20100318_CHANGE_LEVEL_FOR_STAT_INIT_2
    m_GameOption.m_byFreeStatInitLevel = GetINIDataInt("GAME_OPTION", "MAX_LEVEL_FOR_FREE_STAT_INIT");          //���� ���� �ʱ�ȭ ����
#ifdef _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO
    m_GameOption.m_byDominationChannel = GetINIDataInt( "GAME_OPTION", "DOMINATION_CHANNEL");                   //���� ������ ü�� ����

    /// [2012-6-7 by i4u4me] �⺻���� 1ä�η� ���� 
    if (m_GameOption.m_byDominationChannel == 0)
    {
        m_GameOption.m_byDominationChannel = 1;
    }
#endif
	return TRUE;
}

