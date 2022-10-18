#include "SunClientPrecompiledHeader.h"

#include "InstanceDungeonScene.h"
#include "ObjectManager.h"
#include "hero.h"
#include "BattleLobby.h"
#include "LoadScene.h"
#include "ChatDialog.h"
#include "UserSlot.h"
#include "HeroActionInput.h"
#include "BattleScene.h"
#include "interfacemanager.h"
#include "globalfunc.h"

//----------------------------------------------------------------------------
/**
*/
void 
InstanceDungeonScene::ParsePacket( MSG_BASE * pMsg )
{
	switch ( pMsg->m_byCategory )
	{
	case CG_SYNC:
		{
			OnRecvCG_SYNC( pMsg );
		}
		break;

	case CG_STATUS:
		{
			OnRecvCG_STATUS( pMsg );
		}
		break;

	case CG_PVP:
		{
			OnRecvCG_InstanceDungeon( pMsg );
		}
		break;

	default:
		{
			BattleScene::ParsePacket( pMsg );
		}
		break;
	}
}




//----------------------------------------------------------------------------
/** 
@Remark 
- !! 단, PVP 진행중에 유저가 들어올 수 없다.
기획팀과 논의된 내용.
*/
void	
InstanceDungeonScene::OnRecvCG_SYNC( MSG_BASE * pMsg )
{
	BattleScene::OnRecvCG_SYNC( pMsg );	

	switch ( pMsg->m_byProtocol )
	{
	case CG_SYNC_PLAYER_ENTER_ACK:
		{
			MSG_CG_SYNC_PLAYER_ENTER_ACK * pRecvMsg = (MSG_CG_SYNC_PLAYER_ENTER_ACK *)pMsg;
			pRecvMsg->Decode();
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
void InstanceDungeonScene::OnRecvCG_STATUS( MSG_BASE * pMsg )
{
	BattleScene::OnRecvCG_STATUS( pMsg );

	switch ( pMsg->m_byProtocol )
	{
	case CG_STATUS_RESURRECTION_BRD:
		{
			MSG_CG_STATUS_RESURRECTION_BRD * pRecvMsg = (MSG_CG_STATUS_RESURRECTION_BRD *)pMsg;
		}
		break;

	case CG_STATUS_DEAD_BRD :
		{
			MSG_CG_STATUS_DEAD_BRD *pRecvPacket = (MSG_CG_STATUS_DEAD_BRD *)pMsg;
		}
		break;

	default:
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
void InstanceDungeonScene::OnRecvCG_InstanceDungeon( MSG_BASE * pMsg )
{
}
