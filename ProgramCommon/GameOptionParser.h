#pragma once

#include <Singleton.h>
#include "Struct.h"

enum eGAME_OPTION_SECTION_MODE
{
	GAME_SECTION_NONE,				
	GAME_SECTION_GAME_OPTION,		
	GAME_SECTION_MAX,
};


class GameOptionParser : public util::Singleton<GameOptionParser>, public IParser
{
public:
	GameOptionParser(void);
	virtual ~GameOptionParser(void);

public:


	GAME_OPTION&			GetGameOption(){ return m_GameOption; }

	virtual	VOID			Release();


			BOOL			CheckMaxLevel( LEVELTYPE nLevel );
			BOOL			ShoutLevelCheck(const GAME_OPTION& opt, LEVELTYPE level);
			WORD			GetMaxLevel(){ return m_GameOption.m_byMaxLevel; }
			void			SetMaxLevel(BYTE byMaxLevel) { m_GameOption.m_byMaxLevel = byMaxLevel; }

			void			SetNormalShoutLevel(BYTE byNormalShoutLevel) { m_GameOption.m_byNormalShoutLevel = byNormalShoutLevel; }
			void			SetWorldShoutPCBANG(BYTE byWorldShoutPCBANG) { m_GameOption.m_byWorldShoutPCBANG = byWorldShoutPCBANG; }
#ifdef __NA_0_20080515_BUG_FIX_WORLD_SHOUT_LEVEL
			void			SetWorldShoutLevel(BYTE byWorldShoutLevel) { m_GameOption.m_byWorldShoutLevel = byWorldShoutLevel; }
#endif

			LEVELTYPE		GetMaxLevelChangeOfClass1st() { return m_GameOption.m_MaxLevelChangeOfClass1st; }
			void			SetMaxLevelChangeOfClass1st(WORD MaxLevelChangeOfClass1st) { m_GameOption.m_MaxLevelChangeOfClass1st = MaxLevelChangeOfClass1st; }
            //_NA_0062_20100318_CHANGE_LEVEL_FOR_STAT_INIT_2
            BYTE			GetFreeStatInitLevel() { return m_GameOption.m_byFreeStatInitLevel; }
            void			SetFreeStatInitLevel(BYTE byFreeStatInitLevel) { m_GameOption.m_byFreeStatInitLevel = byFreeStatInitLevel; }
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
			BOOL			_Load( BOOL bReload );

private:
			GAME_OPTION		m_GameOption;

};


