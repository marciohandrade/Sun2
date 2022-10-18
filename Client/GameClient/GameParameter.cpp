#include "SunClientPrecompiledHeader.h"
#include "OptionDialog.h"
#include "GlobalFunc.h"


CGameParameter::CGameParameter(void)
{
	ZeroMemory( m_szFullPath, MAX_PATH );
	ZeroMemory( m_szPackFileName, MAX_PATH );
	ZeroMemory( m_szSettingFileNameForPack, MAX_PATH );
	SetCetegoryName( "PARAM" );
}

CGameParameter::CGameParameter(LPCSTR filename)
{
	SetSettingFileName( filename );
	ZeroMemory( m_szPackFileName, MAX_PATH );

	SetCetegoryName( "PARAM" );

}

CGameParameter::~CGameParameter(void)
{
	// TODO : Release 함수를 만들어서 관리해야 함.

	m_IntParam.m_Map.clear();
	m_FloatParam.m_Map.clear();
	m_StringParam.m_Map.clear();
}

void CGameParameter::SetSettingFileName( LPCSTR szSettingFileName )
{
	if( szSettingFileName )
	{
		DWORD dwError = GetCurrentDirectoryA( 512, m_szFullPath );
		if(0 == dwError)
		{
			assert(0);
		}
		strcat(m_szFullPath, "\\");
		strcat(m_szFullPath, szSettingFileName); // 주어지는 파일 이름에 대한 풀 패스를 설정함

		StrnCopy( m_szSettingFileNameForPack, szSettingFileName, MAX_PATH );
	}
	else
	{
		ZeroMemory( m_szFullPath, MAX_PATH );
	}
}

void CGameParameter::SetPackFileName( LPCSTR szPackFileName )
{
	if( szPackFileName )
		strcpy( m_szPackFileName, szPackFileName );
	else
		ZeroMemory( m_szPackFileName, MAX_PATH );
}

void CGameParameter::SetCetegoryName( LPCSTR szCategoryName )
{
	if( szCategoryName )
		StrCopy( m_szCetegoryName, szCategoryName );
	else
		ZeroMemory( m_szCetegoryName, MAX_PATH );
}

LPCTSTR CGameParameter::SelectCetegoryName( LPCTSTR szCategoryName )
{
	if( szCategoryName )
		return szCategoryName;
	else
	{
		return m_szCetegoryName;
	}
}

int CGameParameter::ReadInt( LPCTSTR szKeyName, int nDefaultValue, LPCTSTR szCetegoryName )
{
	util::_tstring	szNumber;
	TCHAR			szDefaultValue[32];

	Sprintf( szDefaultValue, _T("%d"), nDefaultValue );
	Read( szNumber, szKeyName, szDefaultValue, szCetegoryName );

	m_IntParam.m_Map[szKeyName] = (int)_tstof( (TCHAR *)szNumber.c_str() );
	return m_IntParam.m_Map[szKeyName];
}

float CGameParameter::ReadFloat( LPCTSTR szKeyName, float fDefaultValue, LPCTSTR szCetegoryName )
{
	util::_tstring	szNumber;
	TCHAR			szDefaultValue[32];

	Sprintf( szDefaultValue, _T("%f"), fDefaultValue );
	Read( szNumber, szKeyName, szDefaultValue, szCetegoryName );

	m_FloatParam.m_Map[szKeyName] = (float)_tstof( (TCHAR *)szNumber.c_str() );
	return m_FloatParam.m_Map[szKeyName];
}

util::_tstring& CGameParameter::ReadString( LPCTSTR szKeyName, LPCTSTR szDefaultValue, LPCTSTR szCetegoryName )
{
	Read( m_StringParam.m_Map[szKeyName], szKeyName, szDefaultValue, szCetegoryName );
	return m_StringParam.m_Map[szKeyName];
}

void CGameParameter::Read( util::_tstring & szOutput, LPCTSTR szKeyName, LPCTSTR szDefaultValue, LPCTSTR szCetegoryName )
{
	// set default value
	if( szDefaultValue )	szOutput = szDefaultValue;
	else					szOutput.empty();

	// read from pack
	if(IsSystemPackFileName())
	{
		util::SolarFileReader sr;

		BOOL bResult = sr.OpenFileFromPack(m_szPackFileName, m_szSettingFileNameForPack, SEPERATOR_WHITESPACE_ONLY_STRING);
		if( !bResult )
			return;


		while( TRUE )
		{
			if ( TOKEN_END == sr.GetNextTokenType() )
				break;

			if( Strcmp( szKeyName, sr.GetTokenString() ) == 0 )
			{
				sr.GetNextTokenType();	sr.GetTokenString();			// =
				sr.GetNextTokenType();	
				szOutput = sr.GetTokenString();	// real value

				break;;
			}
		}

		sr.CloseFile();
	}
	// read from unpack
	else
	{
		TCHAR szTempBuffer[256];
		GetPrivateProfileString( SelectCetegoryName( szCetegoryName ), szKeyName, szDefaultValue, szTempBuffer, 256, m_szFullPath );
		szOutput = szTempBuffer;
	}
}

BOOL CGameParameter::WriteInt( LPCTSTR szKeyName, int nValue, LPCTSTR szCetegoryName )
{
	TCHAR szValue[MAX_PATH];
	Sprintf( szValue, _T("%d"), nValue );

	BOOL bRet = Write( szKeyName, szValue, szCetegoryName );
	if( bRet )
		m_IntParam.m_Map[szKeyName] = nValue;

	return bRet;
}

BOOL CGameParameter::WriteFloat( LPCTSTR szKeyName, float fValue, LPCTSTR szCetegoryName )
{
	TCHAR szValue[MAX_PATH];
	Sprintf( szValue, _T("%f"), fValue );

	BOOL bRet = Write( szKeyName, szValue, szCetegoryName );
	if( bRet )
		m_FloatParam.m_Map[szKeyName] = fValue;

	return bRet;
}

BOOL CGameParameter::Write( LPCTSTR szKeyName, LPCTSTR szValue, LPCTSTR szCetegoryName )
{
	if(IsSystemPackFileName())
	{
		assert( !"Pack File - Don't support write" );
		return FALSE;
	}
	else
	{
		return WritePrivateProfileString( SelectCetegoryName( szCetegoryName ), szKeyName, szValue, m_szFullPath );
	}
}

//=========================================================================================================
//=========================================================================================================

// GameParameter 들...

CGeneralGameParam::CGeneralGameParam()
{
	
}

void CGeneralGameParam::Load( void )
{
	GlobalFunc::WriteLoadingTimeLog(_T("CGeneralGameParam::Load Start"));
	DWORD dwStartTime = GlobalFunc::StartTimeCheck();


	m_bNet				= ReadInt(_T("NETWORK"), TRUE);
	m_Start_MapID		= ReadInt(_T("START_MAP_ID"), 10001); 
	m_Start_FieldID		= ReadInt(_T("START_FIELD_ID"), 10001);
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    m_CharSelectMapID	= ReadInt(_T("CHARACTER_SELECT_MAP"), 11002);	
#else
	m_CharSelectMapID	= ReadInt(_T("CHARACTER_SELECT_MAP"), 11000);	
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	m_dwVillageMapID	= ReadInt(_T("VILLAGE_MAP_ID"), 10001);
    m_iStartTileIndex   = ReadInt(_T("START_TILE_INDEX"), 0);
	m_iShadowType		= ReadInt(_T("SHADOW_TYPE"),1);
	m_iCameraRotation   = ReadInt(_T("CAMERA_ROTATION"),0);
	m_iScreenShotMode	= ReadInt(_T("SCREENSHOT_MODE"),FALSE);

	float fTemp;

	fTemp = ReadFloat(_T("CAMERA_TO_X"),0.0f);
	m_vCameraTo.x = fTemp;
	fTemp = ReadFloat(_T("CAMERA_TO_Y"),0.0f);
	m_vCameraTo.y = fTemp;
	fTemp = ReadFloat(_T("CAMERA_TO_Z"),0.0f);
	m_vCameraTo.z = fTemp;

	m_bInterpolatePosition = ReadInt(_T("INTERPOLATE_POSITION"),0);
	m_bServerDebug = ReadInt(_T("SERVER_DEBUG"),0);

	//  동영상 -----------------------------------------------
	//	동영상 촬영용 프리 카메라 모드
	m_iIsFreeCameraMode = ReadInt(_T("FREE_CAMERA_MODE"), FALSE);

	//	동영상 촬영용 프리 카메라 제어 속도
	m_fFreeCameraSpeed = ReadFloat(_T("FREE_CAMERA_SPEED"), 0.1f);
	m_iFreeCameraTurnSpeed = ReadInt(_T("FREE_CAMERA_TURN_SPEED"), 2);

	m_bFreeCameraState = FALSE;



	m_szCinematicFileName = ReadString(_T("CINEMATIC_FILENAME"), NULL);
    m_szCharacterSceneCinematicFileName = ReadString(_T("CHARACTER_CINEMATIC_FILENAME"), "");



#if _DEV_VER
	int iSpecialMode = ReadInt(_T("SPECIAL_MODE"),0);		// 개발버전에서만 SPECIAL_MODE 파라미터를 읽어오고
#else
	int iSpecialMode = eSPECIAL_MODE_NONE;					// 유저배포버전에서는 무조건 SPECIAL_MODE를 끄자.
#endif

	m_eSpecialMode = (eSPECIAL_MODE)iSpecialMode;

    if( m_eSpecialMode >= eSPECIAL_MODE_STEP1 )
    {
        // 스페셜 모드가 1보다 클경우 출력창에 로그 표시(개발자버전인경우)
        m_bShowConsole = TRUE;
        m_szConsoleSetting = "ERROR";
    }
    else
    {
        m_bShowConsole = FALSE;
        m_szConsoleSetting = "ERRORR";
    }


	m_bUsePackFile = ReadInt(_T("USE_PACK_FILE"),0);

	m_iMultiThreadLoading = ReadInt(_T("MULTI_THREAD_LOADING"),0);
	m_bMoveNakProcess = ReadInt(_T("MOVE_NAK_PROCESS"), 1);

	m_bStartLuaConsole = ReadInt(_T("START_LUA"),0);
	
	m_engineRoot = ReadString(_T("ENGINE_ROOT"), NULL); 

	// 리소스 테스트 -----------------------------------------------
    // TEST RESOURCE 
    m_bResourceTestScene = ReadInt(_T("TEST_RESOURCE_SCENE"), FALSE);
    m_bTestGraphicResource = ReadInt(_T("TEST_RESOURCE_GRAPHIC_RESOURCE"), FALSE);
    m_bTestMap = ReadInt(_T("TEST_RESOURCE_MAP"), FALSE);
    m_bTestEffect = ReadInt(_T("TEST_RESOURCE_EFFECT"), FALSE);
    m_bTestInterface = ReadInt(_T("TEST_RESOURCE_INTERFACE"), FALSE);
	m_bTestRscFailedOnly = ReadInt(_T("TEST_RESOURCE_FAILED_ONLY_LOG"), FALSE);

    // TEST_UI_SCENE
    this->m_bTestUIScene = ReadInt(_T("TEST_UI_SCENE"), FALSE);
    this->m_iTestUIScene_ExtID = ReadInt(_T("TEST_UI_SCENE_EXT"), 0);

#ifdef _DEV_VER
    show_ui_name_ = !!(ReadInt(_T("SHOW_UI_NAME"), 0));//KeyWord:_DH_SHOW_UI_NAME
    show_string_id_ = !!(ReadInt(_T("SHOW_STRING_ID"), 0));//KeyWord:_DEBUG_STRINGIDEX_SHOW
    show_trigger_ = !!(ReadInt(_T("SHOW_TRIGGER"), 0));//define:_DH_TRIGGER_LOG_MESSAGE
    show_item_id_ = !!(ReadInt(_T("SHOW_ITEM_ID"), 0));//define:_SCJ_SHOW_ITEM_ID
    show_npc_id_ = !!(ReadInt(_T("SHOW_NPC_ID"), 0));//define:_SCJ_SHOW_NPC_ID
#endif//_DEV_VER
	show_ssq_player_name_ = !!(ReadInt(_T("SSQ_PLAYER_NAME"), 0));//define:_NA_004756_120504_SHOW_SSQ_PLAYER_NAME
	/////////////////////////////////////////////////////////////////////
	// about game server
	m_dwUserID			= ReadInt(_T("USER_ID"), NULL); 

	//==============================================================================================
	// 게임 공용 파라미터
    this->m_iIntervalToWorldServerReconnect = 15000;
	
	this->m_bSpringCamera=ReadInt(_T("SPRING_CAMERA_MODE"), FALSE);
	m_springConst=ReadFloat(_T("SPRING_CONST"),0.709996f);
	m_dampConst=ReadFloat(_T("SPRING_DAMP"),6.8f);
	m_springLen=ReadFloat(_T("SPRING_LEN"),0.1f);

	m_iGuildRankUpDisplayCount = ReadInt( _T("GUILD_RANKUP_DISPLAY_COUNT"), 6 );
	m_bSoundLog = ReadInt(_T("MAKE_SOUND_LOG"),0);

	m_fCameraDistance = ReadFloat(_T("CAMERA_DISTANCE"),10.0f);
	m_fMaxCameraDistance = ReadFloat(_T("MAX_CAMERA_DISTANCE"),11.0f);

    wings_down_jump_height_ = ReadFloat(_T("WINGS_DOWN_JUMP_HEIGHT"), 50.0f);

    //크래쉬리포트 전송 타입
    send_dump_type_ = ReadInt(_T("SEND_DUMP_TYPE"), 0);
    //전장 다중 예약 허용
    battle_ground_multi_reservation_ = ReadInt(_T("BATTLE_GROUND_MULTI_RESERVATION"), 0);

    //------------------------------------------------------------------------------ 
    //! 통합샵
    //------------------------------------------------------------------------------ 
    conveniencestore_connect_ip     = ReadString(_T("CONNECT_IP"), _T(""));
    conveniencestore_remote_path    = ReadString(_T("REMOTE_PATH"), _T(""));
    conveniencestore_port_num       = ReadInt(_T("CONNECT_PORT"), 0);
    conveniencestore_always_download= ReadInt(_T("ALWAYS_DOWNLOAD"), 0) != 0;
    //------------------------------------------------------------------------------ 

#ifdef _DEV_VER
    //==============================================================================================
    // 개발모드 옵션을 위한 클라이언트 변수
    m_bEnableAutoKill = false;
    m_bEnableAutoCooltimeReset = false;
#endif


	////==============================================================================================


	GlobalFunc::EndTimeCheck(dwStartTime, true, _T("CGeneralGameParam::Load End"));
	
}
//=========================================================================================================

CClientInfoParam::CClientInfoParam()
{
}

CClientInfoParam::~CClientInfoParam()
{
}

void CClientInfoParam::Load( void )
{
	GlobalFunc::WriteLoadingTimeLog(_T("CClientInfoParam::Load Start"));
	DWORD dwStartTime = GlobalFunc::StartTimeCheck();

	m_ClientVersion		= ReadString( _T("VERSION"), _T("1.0"), _T( "GENERAL" ) );

	GlobalFunc::EndTimeCheck(dwStartTime, true, _T("CClientInfoParam::Load End"));
}

//=========================================================================================================
//=========================================================================================================

CTestGameParam::CTestGameParam()
: CGameParameter("TEST.ini")
{

}

//=========================================================================================================
//=========================================================================================================
CGraphicParam::CGraphicParam()
{

}

CGraphicParam::~CGraphicParam()
{

}

void CGraphicParam::Load()
{
	m_iLoadingText_StartPointX	= ReadInt(_T("LOADTEXT_STARTPOINT_X") , 200 );
	m_iLoadingText_StartPointY	= ReadInt(_T("LOADTEXT_STARTPOINT_Y") , 300);
	m_iLoadingText_CardOut		= ReadInt(_T("LOADTEXT_CARDOUT") , 20);
	m_iFogValue					= ReadInt(_T("FOG_VALUE") , 10);
	m_iBlurValue				= ReadInt(_T("BLUR_VALUE") , 100);
	m_fTerrain_Color_Rate		= ReadFloat(_T("TERRAIN_COLOR_RATE") , 0.6f);


	m_fPkFogBegin		= ReadFloat(_T("PK_FOG_BEGINE"),10.0f);	
	m_fPkFogEnd 		= ReadFloat(_T("PK_FOG_END"),100.0f);
	m_fPkFogRate		= ReadFloat(_T("PK_FOG_RATE"),15.0f);

    TCHAR szKeyWord[1024 + 1] = {0,};
    for(int i = 0; i < MAX_CHARACTER_LIST_NUM; ++i)
    {
        Snprintf(szKeyWord, 1024, _T("CHARACTER_SELECT_SCENE_POSITION%dX"), i);
        m_wvCharScenePos[i].x = ReadFloat(szKeyWord, 0.f);
        Snprintf(szKeyWord, 1024, _T("CHARACTER_SELECT_SCENE_POSITION%dY"), i);
        m_wvCharScenePos[i].y = ReadFloat(szKeyWord, 0.f);
        Snprintf(szKeyWord, 1024, _T("CHARACTER_SELECT_SCENE_POSITION%dZ"), i);
        m_wvCharScenePos[i].z = ReadFloat(szKeyWord, 0.f);
    }

    m_wcCharCreateSceneFogColor = WzColor_RGBA(ReadInt(_T("CHARACTER_CREATE_SCENE_FOG_COLORR"), 255),
                                               ReadInt(_T("CHARACTER_CREATE_SCENE_FOG_COLORG"), 255),
                                               ReadInt(_T("CHARACTER_CREATE_SCENE_FOG_COLORB"), 255),
                                               255);


    //------------------------------------------------------------------------------ 
    //! _YMS_SMOOTH_GAUGE_
    //! 게이지 속도 부드럽게
    //const int kGaugeHpKindCount = 5;
    //const int kGaugeStateKindCount = 2;
    hero_gauge_velocity_[0][0] = ReadFloat(_T("HERO_RESERVEHP_VEL_STAND_1000"), 1.0f);	
    hero_gauge_velocity_[0][1] = ReadFloat(_T("HERO_RESERVEHP_VEL_STAND_2000"), 1.0f);	
    hero_gauge_velocity_[0][2] = ReadFloat(_T("HERO_RESERVEHP_VEL_STAND_3000"), 1.0f);	
    hero_gauge_velocity_[0][3] = ReadFloat(_T("HERO_RESERVEHP_VEL_STAND_4000"), 1.0f);	
    hero_gauge_velocity_[0][4] = ReadFloat(_T("HERO_RESERVEHP_VEL_STAND_5000"), 1.0f);	
    hero_gauge_velocity_[1][0] = ReadFloat(_T("HERO_RESERVEHP_VEL_SIT_1000"), 1.0f);	
    hero_gauge_velocity_[1][1] = ReadFloat(_T("HERO_RESERVEHP_VEL_SIT_2000"), 1.0f);	
    hero_gauge_velocity_[1][2] = ReadFloat(_T("HERO_RESERVEHP_VEL_SIT_3000"), 1.0f);	
    hero_gauge_velocity_[1][3] = ReadFloat(_T("HERO_RESERVEHP_VEL_SIT_4000"), 1.0f);	
    hero_gauge_velocity_[1][4] = ReadFloat(_T("HERO_RESERVEHP_VEL_SIT_5000"), 1.0f);	

    //! 날고 있을때 카메라 눈높이.
    camera_eye_height_wings_spread_ = ReadFloat(_T("CAMERA_EYE_HEIGHT_WINGS_SPREAD"), 2.5f);	
    camera_eye_height_rider_spread_ = ReadFloat(_T("CAMERA_EYE_HEIGHT_RIDER_SPREAD"), 6.0f);	

}
//------------------------------------------------------------------------------ 
float CGraphicParam::hero_gauge_velocity(DWORD max_hp, bool sit)
{
    int state_kind = sit ? 1 : 0;
    int hp_kind = 0;
    if (max_hp < 1000)       hp_kind = 0;
    else if (max_hp < 2000)  hp_kind = 1;
    else if (max_hp < 3000)  hp_kind = 2;
    else if (max_hp < 4000)  hp_kind = 3;
    else                     hp_kind = 4;
 
    return hero_gauge_velocity_[state_kind][hp_kind];
} 
//=========================================================================================================
//=========================================================================================================
CProgramParam::CProgramParam()
{
    is_sound_caching_ = true;

    nation_type_ = kLanguageInvalid;
    is_block_cash_shop_ = false;
}

CProgramParam::~CProgramParam()
{

}

void CProgramParam::Load()
{
	m_bCheckBlockChatRepeat		= ReadInt(_T("CHECK_CHAT_REPEAT") , TRUE);

	m_iBlockChatRepeatTime		= ReadInt(_T("BLOCK_CHAT_REPEAT_TIME"), 5000);
	m_bEffectBatch				= ReadInt(_T("EFFECTBATCH"),1);
    m_iAutoEffectRate           = ReadInt(_T("AUTO_EFFECT_RATE"),1);  // 자동 이펙트 생성 비율 조절: 기본값은 활성화(1)

#ifdef _GS_ADD_BIGEVENT_MSG_1122
	m_bBigEventTimer			= ReadInt(_T("BIG_EVENT_TIMER"),0);	
	m_wBigEventQuestCode		= ReadInt(_T("BIG_EVENT_QUEST_CODE"),0);
	m_iBigEventStringCode		= ReadInt(_T("BIG_EVENT_STRING_CODE"),0);
	m_iBigEventDisplayTime		= ReadInt(_T("BIG_EVENT_DISPLAY_TIME"),1000);
#endif//_GS_ADD_BIGEVENT_MSG_1122

	m_NewServerSet.insert(ReadInt( _T("NEW_SERVER_ID1"), 0 ));
	m_NewServerSet.insert(ReadInt( _T("NEW_SERVER_ID2"), 0 ));
	m_NewServerSet.insert(ReadInt( _T("NEW_SERVER_ID3"), 0 ));
	m_NewServerSet.insert(ReadInt( _T("NEW_SERVER_ID4"), 0 ));
	m_NewServerSet.insert(ReadInt( _T("NEW_SERVER_ID5"), 0 ));
	m_NewServerSet.erase(0);

    m_IconWidth = ReadInt( _T("NEW_ICON_WIDTH"), 63 );
    m_IconHeight = ReadInt( _T("NEW_ICON_HEIGHT"), 16 );
	m_DominationChannel			= ReadInt(_T("DOMINATION_CHANNEL"),1);

#ifdef _DEV_VER
    search_frame_per_update_time_	 = ReadInt(_T("SEARCH_FRAME_UPDATE_TIME"),100);
    number_of_searching_per_a_frame_ = ReadInt(_T("NUMBER_OF_SEARCHING_PER_A_FRAME"),200);
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
    replace_string_ = ReadString(_T("REPLACE_WORD_FOR_SLANG"), "*");
    replace_type_ = ReadInt(_T("REPLACE_TYPE_FOR_SLANG"), 0);
    is_enable_perk_setting_ = (ReadInt(_T("ENABLE_PERK_SETTING_FREE"), 1) == 1) ? true : false;

    is_sound_caching_ = (ReadInt(_T("SOUND_CACHING"), 1) != 0);

    // 캐릭터 삭제대기 레벨(디폴트:40)
    delete_level_for_wait_ = static_cast<DWORD>(ReadInt(_T("DELETE_LEVEL_FOR_WAIT"), 40));

    int temp_type = ReadInt(_T("NATION_TYPE"), -1);
    nation_type_ = static_cast<LanguageType>(temp_type);

    int cashshop_block_value = ReadInt(_T("CASHSHOP_BLOCK"), 0);
    is_block_cash_shop_ = (cashshop_block_value == 1) ? true : false;
}
//////////////////////////////////////////////////////////////////////////

//=========================================================================================================
CTimerParam::CTimerParam()
{
}

CTimerParam::~CTimerParam()
{
}

void CTimerParam::Load()
{
    is_show_event_timer = ReadInt(_T("EVENT_TIMER") , FALSE);
    max_event_time = ReadInt(_T("MAX_EVENT_TIME"), 1800);
    show_tired_system = ReadInt(_T("TIRED_SYSTEM") , 1);
    max_time_tired_system = ReadInt(_T("TIRED_TIME") , 720);
}

