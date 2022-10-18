#include "SunClientPrecompiledHeader.h"

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION

#include "SceneBase.h"
#include "GuildMissionScene.h"

#include "Map.h"
#include "GlobalFunc.h"
#include "BGMSoundInfoParser.h"


GuildMissionScene g_GuildMissionScene;


GuildMissionScene::GuildMissionScene()
{
	m_SceneType = SCENE_TYPE_GUILDMISSION;
}

GuildMissionScene::~GuildMissionScene()
{
}

BOOL GuildMissionScene::Init()
{
	if ( NULL == g_pMap )
		return FALSE;

	BOOL bReturn;


	bReturn = BattleScene::Init();
	
	GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_INSTANCE );


	return bReturn;
}

BOOL GuildMissionScene::Process( DWORD dwTick )
{
	BOOL bReturn;

	bReturn = BattleScene::Process(dwTick);

	SendHeroSectorIndex();

	return bReturn;
}


void GuildMissionScene::ParsePacket( MSG_BASE * pMsg )
{
	BattleScene::ParsePacket(pMsg);
}

RELATIONKIND::TYPE GuildMissionScene::GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy )
{
	return GameFunc::GetRelationKindOfPlayerInGuildMission(pObject, bForceEnemy);
}


#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION