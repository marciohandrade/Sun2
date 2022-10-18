#include "stdafx.h"
#include <PacketStruct_AG.h>
#include <GameInstanceDungeon.h>

#include <Status/StatusManager.h>
#include "GameWarControlManager.h"

// for debugging control
#undef WAVERIX_DEBUGGING_OUTPUTn
#undef WAVERIX_DEBUGGING_OUTPUTnV

#define WAVERIX_DEBUGGING_OUTPUTn( msg )	\
	SUNLOG( eCRITICAL_LOG, "[%s][%u][%u] %s\n", __FUNCTION__, __LINE__,	\
	(DWORD)((GetTickCount()-tickvalue)/(1000)), msg );					\
	tickvalue = GetTickCount()

#define WAVERIX_DEBUGGING_OUTPUTnV( value )	\
	{ CHAR _message[10];					\
	WAVERIX_DEBUGGING_OUTPUTn( itoa( value, _message, sizeof(_message) ) ); }

static DWORD tickvalue = GetTickCount();

#include "./SSQ/GameSSQControlInfos.hxx"
#include "./SSQ/GameSSQPacketProcess.hxx"
#include "./SSQ/GameSSQUpdateCtrl.hxx"
#include "./SSQ/GameSSQControlManagerImpl.hxx"
#include "./SSQ/SSQHandlerInGame.h"


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<DEFINITION #2>
//

#define MGR_SSQIMPL			((GameSSQControlManagerImpl&)*m_pSSQImpl)
#define MGR_SSQIMPLS(this_)	((GameSSQControlManagerImpl&)*((this_)->m_pSSQImpl))

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<GameWarControlManager>
WORK_KEY				GameWarControlManager::ms_WorkKey = 0;
GameWarControlManager*	GameWarControlManager::ms_pSSQControlManager = NULL;

GameWarControlManager::GameWarControlManager()
{
	ms_pSSQControlManager = this;
	m_pSSQImpl = new GameSSQControlManagerImpl();
}

GameWarControlManager::~GameWarControlManager()
{
	SAFE_DELETE( m_pSSQImpl );
	ms_pSSQControlManager = NULL;
}

//─────────────────────────────────────────
//	<IWorkNode Interface>

BOOL
GameWarControlManager::DoInit()
{
	MGR_SSQIMPL.ScheduleUpdateTimer.SetTimer(INTERVALofUPDATE);
	MGR_SSQIMPL._InitializeUpdateCtrl();

	return TRUE;
}

BOOL
GameWarControlManager::DoProcess()
{
	ITimerBase& rTimeScheduler = MGR_SSQIMPL.ScheduleUpdateTimer;
	if( rTimeScheduler.IsExpired() )
	{
		sTIME_INFO timeInfo;
		sTIME_INFO::SetTimeValue( timeInfo );

		SSQ_UPDATERS& rList = MGR_SSQIMPL.UpdateList;
		FOREACH_CONTAINER( const SSQ_UPDATER_NODE& rCtrlNode, rList, SSQ_UPDATERS )
		{
			GameSSQUpdateCtrl* const pCtrl = rCtrlNode.pUpdater;
			pCtrl->UpdateMsg( timeInfo );
			pCtrl->Update( timeInfo );
		}
	}
	return TRUE;
}

BOOL
GameWarControlManager::IsComplete()
{
	return FALSE;
}

BOOL
GameWarControlManager::DoRelease()
{
	MGR_SSQIMPL._ReleaseUpdateCtrl();
	return TRUE;
}

// Control Interface
VOID
GameWarControlManager::OnMsg( sSIG_MSG* pSIG_MSG )
{
}


//─────────────────────────────────────────
//	<INTERFACEs>
VOID
GameWarControlManager::Update()
{
}


#define SSQ_PACKET_HANDLER_CALL( c, p )	\
	case MAKEWORD( c, p ):					\
		MGR_SSQIMPLS(this_).OnRecv_##p( static_cast<const MSG_##p*>( pMsg ), wSize, pSession.m_pSession );	\
		break

VOID
GameWarControlManager::OnRecv( const MSG_BASE* pMsg, const WORD wSize, const GameSSQSessionCmd::FromClient& pSession )
{
	if( !ms_pSSQControlManager )
		return;
	GameWarControlManager* const this_ = ms_pSSQControlManager;

	const WORD protocol = MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol );
	//switch( protocol )
	//{
	//default:
		FlowControl::FCAssert( !"Unexpected Status" );
	//}

	return;
}

VOID
GameWarControlManager::OnRecv( const MSG_BASE* pMsg, const WORD wSize, const GameSSQSessionCmd::FromAgent& pSession )
{
	if( !ms_pSSQControlManager )
		return;
	GameWarControlManager* const this_ = ms_pSSQControlManager;

	const WORD protocol = MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol );
	switch( protocol )
	{
	SSQ_PACKET_HANDLER_CALL( AG_ZONE,		AG_ZONE_SSQ_JOIN_INFO_SYN );
	SSQ_PACKET_HANDLER_CALL( AG_ZONE,		AG_ZONE_SSQ_JOIN_SYN );
	SSQ_PACKET_HANDLER_CALL( AG_ZONE,		AG_ZONE_SSQ_ROOM_CTRL_CMD );
	default:
		FlowControl::FCAssert( !"Unexpected Status" );
	}

	return;
}


VOID
GameWarControlManager::Serialize()
{
}

BOOL
GameWarControlManager::SendPacket( const MSG_BASE* pMsg, const WORD wSize )
{
	return TRUE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<GameWarControlManager><SendPacket>

VOID
GameWarControlManager::OnMsg( const GameSSQCtrlCmd::Created&, GameInstanceDungeon* const pIndun )
{
	if( !ms_pSSQControlManager )
		return;
	GameWarControlManager* const this_ = ms_pSSQControlManager;

	//	이후 해제될 때 해제하는 루틴이 요구된다.
	const MAPCODE mapCode = pIndun->GetMapCode();
	SSQ_UPDATERS_IT itFind = MGR_SSQIMPLS(this_)._FindCodeOfSSQ( mapCode );
	if( itFind == MGR_SSQIMPLS(this_).UpdateList.end() )
		return;

	SSQ_UPDATER_NODE& rNode = *itFind;
	const KEYTYPE keySSQField = pIndun->GetKey();
	rNode.SetKey( keySSQField );
	rNode.pUpdater->SetDungeon( pIndun );
}

VOID
GameWarControlManager::OnMsg( const GameSSQCtrlCmd::Destroy&, GameInstanceDungeon* const pIndun )
{
	if( !ms_pSSQControlManager )
		return;
	GameWarControlManager* const this_ = ms_pSSQControlManager;

	const MAPCODE mapCode	= pIndun->GetMapCode();
	const SSQCODE CodeOfSSQ	= mapCode;
	SSQ_UPDATERS_IT itFind	= MGR_SSQIMPLS(this_)._FindCodeOfSSQ( CodeOfSSQ );
	if( itFind == MGR_SSQIMPLS(this_).UpdateList.end() )
		return;

	SSQ_UPDATER_NODE& rNode		= *itFind;
	const KEYTYPE keySSQField	= pIndun->GetKey();

	SSQMsgRouterInGame* const pMsgRouter = rNode.pUpdater->GetMsgRouter();
	BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = pMsgRouter->GetCMD_AG();
	WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
	{
		MSGSUB_SSQ_S2S_FIELD_DESTORYED msg;
		msg.CodeOfSSQ	= mapCode;
		msg.m_ZoneKey	= keySSQField;
		msg.SerializeTo( pHeader );
	}
	pMsgRouter->Update( CodeOfSSQ );	// side-effect : flush buffer
	GameSSQUpdateCtrl::ChangeToNextUpdateCtrl( rNode.pUpdater, GameSSQControlInfo::TT_NONE );

	rNode.SetKey( 0 );
	rNode.pUpdater->SetDungeon( 0 );
}


VOID
GameWarControlManager::OnMsg( const GameSSQCtrlCmd::CharacterState& rInfo, GameInstanceDungeon* const pIndun )
{
	if( !ms_pSSQControlManager )
		return;
	GameWarControlManager* const this_ = ms_pSSQControlManager;

	const MAPCODE mapCode	= pIndun->GetMapCode();
	const SSQCODE CodeOfSSQ	= mapCode;
	SSQ_UPDATERS_IT itFind	= MGR_SSQIMPLS(this_)._FindCodeOfSSQ( CodeOfSSQ );
	if( itFind == MGR_SSQIMPLS(this_).UpdateList.end() )
		return;

	SSQ_UPDATER_NODE& rNode			= *itFind;
	GameSSQUpdateCtrl* const pCtrl	= rNode.pUpdater;

	Character* const pCharacter = rInfo.pCharacter;
	if( rInfo.isEnter )
	{
		if( MGR_SSQIMPLS(this_)._OnEnterEvent( pCtrl, pCharacter ) )
		{
		}
	}
	else
	{
		if( MGR_SSQIMPLS(this_)._OnLeaveEvent( pCtrl, pCharacter ) )
		{
			//	(080913) - nothing...
		}
	}
}


VOID
GameWarControlManager::OnMsg( const GameSSQCtrlCmd::CharacterEvent& rInfo, GameInstanceDungeon* const pIndun )
{
	if( !ms_pSSQControlManager )
		return;
	GameWarControlManager* const this_ = ms_pSSQControlManager;

	const MAPCODE mapCode	= pIndun->GetMapCode();
	const SSQCODE CodeOfSSQ	= mapCode;
	SSQ_UPDATERS_IT itFind	= MGR_SSQIMPLS(this_)._FindCodeOfSSQ( CodeOfSSQ );
	if( itFind == MGR_SSQIMPLS(this_).UpdateList.end() )
		return;

	SSQ_UPDATER_NODE& rNode			= *itFind;
	GameSSQUpdateCtrl* const pCtrl	= rNode.pUpdater;

	//rInfo.pCharacter;
	const GameSSQCtrlCmd::eCharacterEvent event = rInfo.event;
	switch( event )
	{
	case GameSSQCtrlCmd::On_Dead:
		{
			typedef GameSSQCtrlCmd::CharacterEvent::OnDead	CmdOnDead;
			const CmdOnDead& rCmd = static_cast<const CmdOnDead&>(rInfo);
			MGR_SSQIMPLS(this_)._OnDead( pCtrl, rCmd.pDest, rCmd.pSrc );
		}
		break;
	default:
		SUNLOG( eCRITICAL_LOG, "Invalid CharacterEvent (%u)", event );
	}
}


namespace GameSSQControlInfo
{

static const GM_Spec::Category _GM_Categoriess[GM_Spec::eCategory_upperbound]
= {
	{ GM_Spec::eCategory_SSQ,		"ssq",		},
};

};

BOOLEAN
GameWarControlManager::OnMsg( const GameSSQCtrlCmd::GMCmd& rInfo, Player* const pGmPlayer )
{
	if( !ms_pSSQControlManager )
		return FALSE;

	//	Type Control
	GM_Spec::eCategory cate_type = GM_Spec::eCategory_upperbound;
	{
		typedef util::ARRAY_TYPEINFO< const GM_Spec::Category,
            0, GM_Spec::eCategory_upperbound > array_info;
		FOREACH_ARRAY( const GM_Spec::Category& rCate, _GM_Categoriess, array_info )
		{
			const INT iRet = memcmp( rCate.CateString, rInfo.Arguments[0], sizeof(GM_Spec::String) );
			if( iRet )
				continue;
			cate_type = rCate.Cate;
			break;
		}
	};

	switch( cate_type )
	{
	case GM_Spec::eCategory_SSQ:
		{
			GameWarControlManager* const this_ = ms_pSSQControlManager;
			return MGR_SSQIMPLS(this_).GM_Command_Process( rInfo, pGmPlayer );
		}
		break;
	}

	return FALSE;
}

