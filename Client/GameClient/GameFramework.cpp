#include "SunClientPrecompiledHeader.h"
#include "Player.h"
#include "GameFramework.h"
#include "SceneBase.h"
#include "LoginScene.h"
#include "LoadScene.h"
#include "VillageScene.h"
#include "BattleScene.h"
#include "StartScene.h"
#include "MoviePlayScene.h"
#include "characterscene.h"
#include "LobbyScene.h"
#include "MissionScene.h"
#include "InterfaceManager.h"
#include "EventScene.h"
#include "TestUIScene.h"
#include "TestUIScene_Ext00.h"
#include "TestUIScene_Ext01.h"
#include "TestUIScene_Ext02.h"
#include "TestUIScene_Ext03.h"
#include "PVPscene.h"
#include "InstanceDungeonScene.h"
#include "Fieldscene.h"
#include "CompetitionScene.h"
#include "ResourceTest.h"
#include "heroactioninput.h"
#include "MouseCursor.h"
#include "MapEditScene.h"
#include "SsqScene.h"
#include "DominationWarScene.h"
#include "GlobalFunc.h"
#include "PacketRequestManager.h"
#include "BattleGroundScene.h"
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "FreePVPScene.h"
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "GoldRushScene.h"
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
#include "SpaScene.h"
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "SonnenScheinScene.h"
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include "ChaosSystem/GuildMissionScene.h"
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

#include "SkillMain.h"

#ifdef _YMS_AUTOBOT_GAME
    #include "Autobot/Autobot.h"
#endif //_YMS_AUTOBOT_GAME

#include "XignCode/XignCodeApi.h"

SceneBase		*GameFramework::s_pCurrentScene = NULL;
SceneBase		*GameFramework::s_pNextScene = NULL;
SceneBase		*GameFramework::s_pOldNextScene = NULL;
SCENE_TYPE		GameFramework::post_Scene_ = SCENE_TYPE_NONE;

BOOL GameFramework::CommitChangeState( void )
{	
	if( s_pNextScene )
	{
		xxWriteSceneLog("ChangeScene to [%d] Start", s_pNextScene->GetType() );

		if( s_pCurrentScene )
		{
			if (s_pCurrentScene->GetType() != SCENE_TYPE_NONE && 
				s_pCurrentScene->GetType() != SCENE_TYPE_LOAD)
			{
				post_Scene_ = s_pCurrentScene->GetType();
			}

			xxWriteSceneLog("Release LastScene [%d]", (int)s_pCurrentScene->GetType());
			s_pCurrentScene->Release();
			s_pCurrentScene	= NULL;
		}

		xxWriteSceneLog("Init NextScene [%d]", (int)s_pNextScene->GetType());

		if( FALSE == s_pNextScene->Init() )
		{
			xxWriteSceneLog("Init NextScene [%d] Failed -_______________-", (int)s_pNextScene->GetType());
			s_pNextScene->Release();
			return FALSE;
		}

		s_pCurrentScene	= s_pNextScene;

		eZONETYPE eZoneType=GameFunc::ConvertServerZoneType(s_pCurrentScene->GetType());

#ifdef _NHN_USA_CHANNELING
		if(eZoneType!=eZONETYPE_MAX && g_LoadScene.GetRePortZone()!=-1)
		{
			GlobalFunc::SendHanReportPacket(g_LoadScene.GetRePortZone());
		}
#endif//_NHN_USA_CHANNELING

		

		ISecureWorkNode::OnChange::Scene(eZoneType);

		s_pNextScene = NULL;
		g_HeroInput.SetMousePointHide(FALSE);
		MouseCursor::Instance()->ShowCursor( TRUE );

		xxWriteSceneLog("ChangeScene to [%d] Success \n\n\n", (int)s_pCurrentScene->GetType());

	}

	return TRUE;
}

SCENE_TYPE GameFramework::GetCurScene()
{
	if (s_pCurrentScene)
	{
		return s_pCurrentScene->GetType();
	}
	else
	{
		return SCENE_TYPE_NONE;
	}
}

BOOL GameFramework::ChangeScene( SCENE_TYPE eScene )
{
	s_pOldNextScene=s_pNextScene;

	switch( eScene )
	{
	case SCENE_TYPE_MOVIEPLAY:
		s_pNextScene = (SceneBase *)&g_MoviePlayScene;
		break;

	case SCENE_TYPE_START:
		s_pNextScene = (SceneBase *)&g_StartScene;
		break;

	case SCENE_TYPE_CHARSELECT:
		s_pNextScene = (SceneBase *)&g_CharSelectScene;
		break;

	case SCENE_TYPE_LOGIN:
		s_pNextScene = (SceneBase*)&g_LoginScene;
		break;

	case SCENE_TYPE_LOAD:
		s_pNextScene = (SceneBase*)&g_LoadScene;
		break;

	case SCENE_TYPE_LOBBY:
		s_pNextScene = (SceneBase*)&g_LobbyScene;
		break;

	case SCENE_TYPE_VILLAGE:
		s_pNextScene = (SceneBase*)&g_VillageScene;
		break;

	case SCENE_TYPE_MISSION:
		s_pNextScene = (SceneBase*)&g_MissionScene;
		break;

	case SCENE_TYPE_PVP:
		s_pNextScene = (SceneBase*)&g_PVPScene;
		break;


	case SCENE_TYPE_INSTANCE_DUNGEON:
		s_pNextScene = (SceneBase*)&g_InstanceDungeonScene;
		break;	

	case SCENE_TYPE_FIELD:
    	s_pNextScene = (SceneBase*)&g_FieldScene;
		break;

	case SCENE_TYPE_EVENT:
		s_pNextScene = (SceneBase*)&g_EventScene;
		break;

    case SCENE_TYPE_CHUNTING:
        s_pNextScene = (SceneBase*)&g_CompetitionScene;
        break;

	case SCENE_TYPE_MAPEDIT:
		s_pNextScene = (SceneBase*)&g_mapEditScene;
		break;
	
	case SCENE_TYPE_SSQ:
		s_pNextScene = (SceneBase*)&g_SsqScene;
		break;

	case SCENE_TYPE_DOMINATION:
		s_pNextScene = (SceneBase*)&g_DominationWarScene;
		break;

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    case SCENE_TYPE_BATTLEGROUND:
        s_pNextScene = (SceneBase*)&g_BattleGroundScene;
        break;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    case SCENE_TYPE_FREEPVP:
        s_pNextScene = (SceneBase*)&g_FreePVPScene;
        break;
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    case SCENE_TYPE_GOLDRUSH:
        s_pNextScene = static_cast<SceneBase*>(&g_GoldRushScene);
        break;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    case SCENE_TYPE_SPA:
        s_pNextScene = static_cast<SceneBase*>(&g_SpaScene);
        break;
#endif // _NA_008069_20150210_ADD_SPA_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case SCENE_TYPE_SONNENSCHEIN:
        s_pNextScene = static_cast<SceneBase*>(&g_SonnenScheinScene);
        break;
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    case SCENE_TYPE_GUILDMISSION:
        s_pNextScene = static_cast<SceneBase*>(&g_GuildMissionScene);
        break;
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION


	case SCENE_TYPE_TESTUI:
        if (GENERALPARAM->GetTestUISceneID() == 0)
        {
            s_pNextScene = (SceneBase*)&g_TestUIScene;
        }
        else
        if (GENERALPARAM->GetTestUISceneID() == 1)
        {
            s_pNextScene = (SceneBase*)&g_TestUIScene_Ext00;
        }
		else
		if (GENERALPARAM->GetTestUISceneID() == 2)
        {
            s_pNextScene = (SceneBase*)&g_TestUIScene_Ext01;
        }
		else
		if (GENERALPARAM->GetTestUISceneID() == 3)
        {
            s_pNextScene = (SceneBase*)&g_TestUIScene_Ext02;
        }
        else
        if (GENERALPARAM->GetTestUISceneID() == 4)
        {
            s_pNextScene = (SceneBase*)&g_TestUIScene_Ext03;
        }
        else
        {
            assert (!"테스트 UI SCENE 전환 실패");
        }
		break;

    case SCENE_TYPE_RESOURCETEST:
        s_pNextScene = (SceneBase*)&g_ResourceTestScene;
        break;

	default:
        assert (!"SCENE_TYPE 을 정할것!!");
		return FALSE;
	}

	return TRUE;
}

BOOL GameFramework::Create( SCENE_TYPE eScene )
{
    this->Destroy();

    new SkillMain();

	return ChangeScene( eScene );
}

void GameFramework::Destroy( void )
{
    if( SkillMain::Instance() )
    {
        SkillMain::ReleaseInstance();
    }

    SAFE_RELEASE( s_pCurrentScene );
}


BOOL GameFramework::Update( DWORD dwTick )
{
	if( FALSE == CommitChangeState() )
	{
		xxWriteSceneLog("Application Exit Code: 3285 - 1");

		Destroy();
		return FALSE;
	}



	if( s_pCurrentScene )
	{
        PacketRequestManager::Instance()->CheckRequestPacket();

		//	각 씬의 프로세스 함수 호출 전
		if( FALSE == s_pCurrentScene->Process( dwTick ) )
		{
			xxWriteSceneLog("Application Exit Code: 3285 - 2");
			return FALSE;
		}

		//	각 씬의 프로세스 함수 호출 후
	}

	

	return TRUE;
}
void GameFramework::ParsePacket( MSG_BASE * pMsg )
{
    if (pMsg->m_byCategory == CG_CONNECTION)
    {
        if (pMsg->m_byProtocol == CG_CONNECTION_TICK_QUERY_SYN)
        {
            MSG_CG_CONNECTION_TICK_QUERY_SYN* syn = static_cast<MSG_CG_CONNECTION_TICK_QUERY_SYN*>(pMsg);
            MSG_CG_CONNECTION_TICK_QUERY_ACK ack;
            ack.index = syn->index;
            ack.round = syn->round;
            ack.tick_count = clock_function::GetTickCount();
            GlobalFunc::SendPacketToGameServer(&ack, sizeof(ack));
            return;
        }
    }

	if(s_pCurrentScene)
	{
        PacketRequestManager::Instance()->NetworkProc(pMsg);
#ifdef _YMS_AUTOBOT_GAME
        if (Autobot::Instance()->NetworkProc(pMsg) == false)
        {
            //! Autobot에서 패킷을 삼키면 이후는 처리 안하게 한다.
            return;
        }
#endif //_YMS_AUTOBOT_GAME
		s_pCurrentScene->UserInterfaceManagersParsePacket(pMsg);
		s_pCurrentScene->ParsePacket( pMsg );
	}
}


void GameFramework::ParsePacketInChat( MSG_BASE * pMsg )
{
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    __UNUSED(pMsg);
    assert(!"this "__FUNCTION__" is not called anymore");
    return;
#else
	if(s_pCurrentScene)
	{
		s_pCurrentScene->UserInterfaceManagersParsePacketWorld(pMsg);
		s_pCurrentScene->ParsePacketInChat( pMsg );
	}
#endif
}

void GameFramework::SetViewport()
{
	if( s_pCurrentScene )
	{
		s_pCurrentScene->SetViewport();
		
	}


}

BOOL GameFramework::Render( DWORD dwTick )
{
	if( s_pCurrentScene )
	{
		if( FALSE == s_pCurrentScene->Render( dwTick ) )
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL GameFramework::PreInterfaceRender( DWORD dwTick )
{
	if( s_pCurrentScene )
	{
		if( FALSE == s_pCurrentScene->PreInterfaceRender( dwTick ) )
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL GameFramework::PostInterfaceRender( DWORD dwTick )
{
	if( s_pCurrentScene )
	{
		if( FALSE == s_pCurrentScene->PostInterfaceRender( dwTick ) )
		{
			return FALSE;
		}
	}
	return TRUE;
}

void GameFramework::Activate( void )
{
	if( s_pCurrentScene )
	{
		s_pCurrentScene->Activate();
	}
}


void GameFramework::DeleteDeviceObjects( void )
{
}


void GameFramework::InvalidateDeviceObjects( void )
{
	if( s_pCurrentScene )
	{
		s_pCurrentScene->Release();
	}
}

void GameFramework::RestoreDeviceObjects( void )
{
	if( s_pCurrentScene )
	{
		s_pCurrentScene->Init();
	}
}
SceneBase	*GameFramework::GetCurrentScene()
{
	return s_pCurrentScene;

}

void GameFramework::OnDeviceReset()
{
	if( s_pCurrentScene )
	{
		s_pCurrentScene->OnDeviceReset();
	}

}

int GameFramework::OnEndRendering( DWORD dwTick )
{
	if( s_pCurrentScene )
	{
		return s_pCurrentScene->OnEndRendering( dwTick );
	}

	return 0;
}


#ifdef __NA_0_20080415_PROCESS_END_PROECSS
int GameFramework::OnEndProcess( DWORD dwTick )
{
	if( s_pCurrentScene )
	{
		return s_pCurrentScene->OnEndProcess( dwTick );
	}

	return 0;
}

#endif

SCENE_TYPE GameFramework::GetPostSceneType()
{
	return post_Scene_;
}


