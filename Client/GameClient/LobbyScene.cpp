#include "SunClientPrecompiledHeader.h"
#include "lobbyscene.h"
#include "InterfaceManager.h"
#include "Player.h"
#include "Hero.h"
#include "objectmanager.h"

LobbyScene		g_LobbyScene;


LobbyScene::LobbyScene(void) 
{
	m_SceneType = SCENE_TYPE_LOBBY;
}

LobbyScene::~LobbyScene(void)
{
}


BOOL LobbyScene::Init( void )
{
	return BattleScene::Init();
}


void LobbyScene::Release( void )
{
	BattleScene::Release();
}

void LobbyScene::OnRecvCG_SYNC( MSG_BASE * pMsg )
{
	BattleScene::OnRecvCG_SYNC( pMsg );
}