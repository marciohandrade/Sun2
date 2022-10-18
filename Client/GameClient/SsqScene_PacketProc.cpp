#include "SunClientPrecompiledHeader.h"
#include "PVPScene.h"
#include "NPC.h"
#include "ObjectManager.h"
#include "hero.h"
#include "NPCInfoParser.h"
#include "BattleLobby.h"
#include "LoadScene.h"
#include "uiBattleZoneList/uiBattleZoneList.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "ChatDialog.h"
#include "UserSlot.h"
#include "ProjectileManagerEx.h"
#include "PVPManager.h"
#include "HeroActionInput.h"
#include "BattleScene.h"
#include "SsqScene.h"

//-------------------------------------------------------------------------------------------
/**
*/
void	
SsqScene::ParsePacket( MSG_BASE * pMsg )
{
	BattleScene::ParsePacket(pMsg);
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
SsqScene::OnRecvCG_STATUS( MSG_BASE * pMsg )
{
	BattleScene::OnRecvCG_STATUS( pMsg );
}

