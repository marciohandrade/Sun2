#pragma once
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION

//#include "scenebase.h"
#include "BattleScene.h"

class GuildMissionScene : public BattleScene
{
public:
	GuildMissionScene();
	virtual ~GuildMissionScene();

	virtual BOOL		Init();
	virtual BOOL		Process( DWORD dwTick );		

	virtual void		ParsePacket( MSG_BASE * pMsg );

	virtual RELATIONKIND::TYPE GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy );

};

extern GuildMissionScene g_GuildMissionScene;


#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION