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
	m_GameOption.m_byMaxLevel = GetINIDataInt( "GAME_OPTION", "MAX_LEVEL" );					                //유저의 맥스 레벨
	m_GameOption.m_MaxLevelChangeOfClass1st = GetINIDataInt( "GAME_OPTION", "MAX_LEVEL_CHANGE_OF_CLASS_1st" );  //유저 1차 Changeup의 Max Level
	m_GameOption.m_byNormalShoutLevel = GetINIDataInt( "GAME_OPTION", "NORMAL_SHOUT_LEVEL" );	                //일반 외치기 레벨
	m_GameOption.m_byWorldShoutPCBANG = GetINIDataInt( "GAME_OPTION", "WORLD_SHOUT_PCBANG" );	                //PC방에서 월드 외치기를 허용할 것인가?
#ifdef __NA_0_20080515_BUG_FIX_WORLD_SHOUT_LEVEL
	m_GameOption.m_byWorldShoutLevel = GetINIDataInt( "GAME_OPTION", "WORLD_SHOUT_LEVEL" );	                    //월드 외치기 레벨
#endif
    //_NA_0062_20100318_CHANGE_LEVEL_FOR_STAT_INIT_2
    m_GameOption.m_byFreeStatInitLevel = GetINIDataInt("GAME_OPTION", "MAX_LEVEL_FOR_FREE_STAT_INIT");          //무료 스텟 초기화 레벨
#ifdef _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO
    m_GameOption.m_byDominationChannel = GetINIDataInt( "GAME_OPTION", "DOMINATION_CHANNEL");                   //지역 점령전 체널 정보

    /// [2012-6-7 by i4u4me] 기본값을 1채널로 고정 
    if (m_GameOption.m_byDominationChannel == 0)
    {
        m_GameOption.m_byDominationChannel = 1;
    }
#endif
	return TRUE;
}

