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
#include "MissionScene.h"
#include "InterfaceManager.h"
#include "uiMissionRankMan/uiMissionRankMan.h"

void	
MissionScene::ParsePacket( MSG_BASE * pMsg )
{
	switch (pMsg->m_byCategory ) 
	{
	case CG_STATUS:
		OnRecvCG_STATUS( pMsg );
		break;
	case CG_MISSION:
		OnRecvCG_MISSION( pMsg );
		break;
	default:
		BattleScene::ParsePacket(pMsg);
		break;
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
MissionScene::OnRecvCG_STATUS( MSG_BASE * pMsg )
{
	switch ( pMsg->m_byProtocol )
	{
	case CG_STATUS_RESURRECTION_BRD:
		{
			BattleScene::OnRecvCG_STATUS( pMsg );
		}
		break;

	default:
		BattleScene::OnRecvCG_STATUS( pMsg );
		break;
	}
}

void 
MissionScene::OnRecvCG_MISSION( MSG_BASE * pMsg )
{
	BattleScene::OnRecvCG_MISSION( pMsg );
}

