#include "SunClientPrecompiledHeader.h"

#include "uiRadarMan/uiRadarMan.h"
#include "Hero.h"
#include "npc.h"
#include "baseRadarDialog.h"
#include "ObjectManager.h"
#include "uiPartyMan/uiPartyMan.h"
#include "GameFramework.h"
#include "GameFunc.h"
#include "InterfaceManager.h"
#include "HeroActionInput.h"
#include "uitooltipman/uiToolTipMan.h"
#include "SceneBase.h"
#include "Application.h"
#include "HeadMeshInfoParser.h"
#include "Monster.h"
#include "Mouse.h"
#include "TargetDialog.h"
#include "uiTargetMan/uiTargetMan.h"
#include "uiQuestMan/uiQuestMan.h"
#include "uiQuestListDlg/uiQuestListDlg.h"
#include "QuestManager_Concrete.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "ChaosSystem/ChaosSystemData.h"
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "Map.h"

// 하드코딩 시러.. - _-; //TODO: elflee
#define	_MONSTER_RADAR_ABS_VALUE	5	//  몬스터가 인던(층수있는 맵)에서 레이더에 나올때는 5미터 안에있을때만 나온다

#define NAVI_POINT_WIDTH 64

#include "GlobalFunc.h"
//------------------------------------------------------------------------------
/**
*/
BaseRadarDialog::BaseRadarDialog(InterfaceManager *pUIMan):
uiBase(pUIMan),
m_bResizefullmap(false)
{
	memset(&m_PortalOffset,0,sizeof(m_PortalOffset));
}
//------------------------------------------------------------------------------
/**
*/
BaseRadarDialog::~BaseRadarDialog(void)
{
	Release();
}

//------------------------------------------------------------------------------
/**
*/
void BaseRadarDialog::Init(CDrawBase * pDrawBase )
{
	uiBase::Init( pDrawBase );

	ResetRadarState();
}

//------------------------------------------------------------------------------
/**
*/
void BaseRadarDialog::Release()
{
}

//------------------------------------------------------------------------------
/**
//*/
void BaseRadarDialog::Wrap_loadMapImage(CControlWZ * pControl, DWORD fieldID, BYTE btImgType  /*= 0*/, TCHAR* file_name /*= NULL*/)
{
    //	인터페이스 엔진에 이미지 파일들 들어있는 경로가 설정되기 때문에
    //	경로 빼고 이미지 파일 이름만 넣어준다
    char pszpath[MAX_PATH] = {0,};

#ifdef _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
    if (file_name != NULL)
    {
        _snprintf( pszpath, MAX_PATH-1, "%s.dds", file_name);
    }
    else
    {
        _snprintf( pszpath, MAX_PATH-1, "map_%05d.dds", fieldID );
    }
#else
    _snprintf( pszpath, MAX_PATH-1, "map_%05d.dds", fieldID );
#endif //_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP

#ifdef _DEBUG
    char   szFile[MAX_PATH] = {0,};
    _snprintf (szFile, MAX_PATH-1, "Data\\Interface\\%s", pszpath);
    FILE* fp = fopen (szFile, "r");
    if (!fp)
    {
        //assert (fp && "loadMapImage - File not found");
    }
    else        
        fclose(fp);
#endif

	SI_TFORMAT sImageFormat;
    sImageFormat.m_sImageState[E_BASE].m_blIs			= TRUE;
    sImageFormat.m_sImageState[E_BASE].m_sImageName		= pszpath;
	sImageFormat.m_sImageState[E_BASE].m_sFRect.fTopX	= 0;
	sImageFormat.m_sImageState[E_BASE].m_sFRect.fTopY	= 0;
	sImageFormat.m_sImageState[E_BASE].m_sFRect.fWidth	= m_wRadarImageWidth;
	sImageFormat.m_sImageState[E_BASE].m_sFRect.fHeight	= m_wRadarImageHeight;
	sImageFormat.m_sImageState[E_BASE].m_nType			= 1;

    if(btImgType == 1)
    {
        sImageFormat.m_sImageState[E_BASE].m_color			= RGBA(255,255,255,150);
    }

	pControl->SetCtrlImageFormat(&sImageFormat);
}

void BaseRadarDialog::Wrap_render(CCtrlPictureWZ * pCtrlPicture)
{
	if (g_pHero == NULL)
	{
		return;
	}

#ifdef _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR

    // 블랜딩을 위한 알파값을 구함 (0.0 ~ 1.0 구간)
    DWORD current_tick = g_pSunRenderer->x_Clock.GetCurrentTick();
    static DWORD last_time = current_tick;
    static float alpha_blend_rate = 0.0f;
    static bool alpha_blend_inc = true;


    static float s_rate = 0.003f;
    float delta_tick = (float)(current_tick - last_time) * 0.003f;
    last_time = current_tick;

    if( alpha_blend_inc == true )
    {
        alpha_blend_rate += delta_tick;
        if( alpha_blend_rate >= 1.0f )
        {
            alpha_blend_inc = false;
            alpha_blend_rate = 1.0f;
        }
    }
    else
    {
        alpha_blend_rate -= delta_tick;
        if( alpha_blend_rate <= 0.0f )
        {
            alpha_blend_inc = true;
            alpha_blend_rate = 0.0f;
        }
    }
#endif


    uiRadarMan* ui_radar_manager = static_cast<uiRadarMan*>(GetMan());
    if (ui_radar_manager == NULL)
    {
        return;
    }

	POINT		pt = g_pApplication->Get2DModeMapping();
	POINT		*pScreenSize = g_pApplication->GetScreenSize();
	float		fWidth = (float)pScreenSize->x / (float)pt.x;
	float		fHeight = (float)pScreenSize->y / (float)pt.y;

	// 렌더 스테이트 백업
	BOOL bIsEnableAlphaTest = GetDrawBase()->IsEnableAlphaTest();
	ENUM_ALPHABLEND eAlphablendMode = GetDrawBase()->GetAlphaBlendMode();
	BOOL bZWrite = GetDrawBase()->IsEnableZWrite();
	BOOL bZWriteTest = GetDrawBase()->IsEnableZTest();

	// 0. z 버퍼 가동	
	GetDrawBase()->SetZBufferTest(FALSE );
	GetDrawBase()->SetZBufferWrite( FALSE );
	GetDrawBase()->SetAlphaBlendMode( AB_NEW_ALPHA, FALSE );
	GetDrawBase()->SetAlphaBlendMode( AB_ALPHA, FALSE );
	GetDrawBase()->EnableAlphaTest( TRUE );

	//------------------------------------------------------------------------------------------------------------
	//		커스텀 텍스쳐 : 각종 포인트 텍스쳐 랜더링
	//------------------------------------------------------------------------------------------------------------
	RECT rect = pCtrlPicture->GetSizeRect();
	DWORD dwAtt=g_pHero->GetCurrentTileAtt(FALSE);

	if ( !GENERALPARAM->IsNet() )
	{
		if(BIT_CHECK(dwAtt,PTA_FREE_CONFLICT_TILE))
		{
			WzColor color =WzColor_RGBA(255,0,0,128);
			GetDrawBase()->ColorFill(color,(float)rect.left,(float)rect.top,(float)rect.right-rect.left,(float)rect.bottom-rect.top);
		}
		else if(BIT_CHECK(dwAtt,PTA_CONFLICT_TILE))
		{
			WzColor color =WzColor_RGBA(255,128,0,128);
			GetDrawBase()->ColorFill(color,(float)rect.left,(float)rect.top,(float)rect.right-rect.left,(float)rect.bottom-rect.top);

		}
		else if(BIT_CHECK(dwAtt,PTA_PK_TILE))
		{
			WzColor color =WzColor_RGBA( 128, 0, 255, 128 );
			GetDrawBase()->ColorFill(color,(float)rect.left,(float)rect.top,(float)rect.right-rect.left,(float)rect.bottom-rect.top);
		}

	}

	float		fImageCtrlCenterX , fImageCtrlCenterY;
	fImageCtrlCenterX = (float)rect.left + (float)(rect.right - rect.left) * 0.5f;
	fImageCtrlCenterY = (float)rect.top + (float)(rect.bottom - rect.top) * 0.5f;
	RECT	*pRect;

	//---------------------------------------------------------------------------------------------
	//
	//		1. 리스트에 등록되어 있는 점들 모두 찍는다,
	//		2. 하드 코딩 되어 있는 숫자 정리하자!!!!!!!
	//		3. 화살표, <회전> 시키자!!!!!!!
	//		4. RadarFullDialog 여기에도 같은 코드 추가하여 똑같이 잘 나오도록 하자!!!!!
	//
	//---------------------------------------------------------------------------------------------

	int			iMouseX = Mouse::Instance()->GetMouseX();
	int			iMouseY = Mouse::Instance()->GetMouseY();

	const WzID	FontID_8 = StrToWzID("s210");
	const WzID	FontID_10 = StrToWzID("m212");
	const WzID	FontID_12 = StrToWzID("m212");
	const WzID	FontID_14 = StrToWzID("m216");

	const WzColor	NPCTextColor = WzColor_RGBA(2,236,195,255);
	const WzColor	QuestNPCTextColor = WzColor_RGBA(75, 255, 45, 255);
	const WzColor	QuestNPCAcceptTextColor = WzColor_RGBA(255, 208, 45, 255);
    const WzColor	DailyQuestNPCAcceptTextColor = WzColor_RGB(197, 47, 47);
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
    const WzColor   ChallengeDailyQuestNPCAcceptTextColor = WzColor_RGB(61, 229, 255);
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST
    const WzColor	GuildQuestNPCAcceptTextColor = WzColor_RGBA( 176, 118, 242, 255 );
	const WzColor	PlayerTextColor = WzColor_RGBA(231,162,2,255);
	const WzColor	PartyTextColor = WzColor_RGBA(0,253,13,255);
	const WzColor	TimeAttackTextColor = WzColor_RGBA(165,74,255,255);
	const WzColor   DeActiveTextColor = WzColor_RGBA(150,150,150,255);
	const WzColor   RepeatTextColor = WzColor_RGBA(75, 255, 45, 255);
	WzColor TextColor = NPCTextColor;

	const int	iNameLineHeight = 25 + 2;
	const int	iTitleLineHeight = 14 + 2;
	const int	iNormalLineHeight = 12 + 2;
	const int	iSpaceLineHeight = 12;

	WzColor wcRadarPointColor[4];
	for (int a = 0; a < 4; a++)
	{
		wcRadarPointColor[a] = WzColor_RGBA(255,255,255,255);
	}

	if(GetRadarState() == eFULL_RADAR_STATE_ALPHA)
	{
		for (int a = 0; a < 4; a++)
		{
			wcRadarPointColor[a] = WzColor_RGBA(255,255,255,150);
		}
	}

	DWORD dwNaviSamePosObjID[MAX_NAVI_CNT] = {0,};
	RADAR_POINT* pHeroRadarPoint = NULL;

    uiRadarMapData& map_data = ui_radar_manager->radar_map_data();
    uiRadarPointCollecter& point_collecter = ui_radar_manager->radar_point_collecter();
    uiRadarClickAnimation& click_animation = ui_radar_manager->radar_click_animation();

    float hero_position_x;
    float hero_position_y;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    if (is_full_map() == true)
    {
#ifdef _NA_000000_20130422_SCALE_FULLMAP
        if(m_bResizefullmap)
        {
            scaleX = 0.5f;
            scaleY = 0.5f;
        }
#endif

        hero_position_x = -(((map_data.map_image_width()*scaleX) / 2.0f) - GetRadarImageWidth() * scaleX);
        hero_position_y = -(((map_data.map_image_height()*scaleY) / 2.0f) - GetRadarImageHeight() * scaleY);
    }
    else
    {
        hero_position_x = map_data.transform_position().x;
        hero_position_y = map_data.transform_position().y;
    }

	ListNpcRect_.clear();

    RADAR_POINT* radar_point = NULL;
    RADAR_POINT_DEQUE& radar_point_container = point_collecter.radar_point_container();
	RADAR_POINT_DEQUE_ITR it = radar_point_container.begin();
	RADAR_POINT_DEQUE_ITR Endit = radar_point_container.end();
	for( ; it != Endit; ++it)
	{
		//	주인공 인덱스 건너뛴다
		if( (*it)->dwObjectKey == g_pHero->GetObjectKey() )
		{
			pHeroRadarPoint = (*it);
			continue;
		}

		eRADAR_POINT_COLOR	eRadarPointColor = (*it)->eRadarPointColor;

		if(GetRadarState() == eFULL_RADAR_STATE_ALPHA)
		{
            if ((eRadarPointColor == eRADAR_MONSTER_POINT) || 
                (eRadarPointColor == eRADAR_MONSTER_DEAD_POINT) || 
                (eRadarPointColor == eRADAR_BOSS_MONSTER_POINT) || 
                (eRadarPointColor == eRADAR_BOSS_MONSTER_DEAD_POINT))
			{
				continue;
			}
		}

		if(eRADAR_NOT_FOUND_OBJECT == eRadarPointColor)
			continue;

        // 블라인드/화이트 블라인드 상태에서는 패스
        if( g_pHero->IsBlind() || g_pHero->IsBlind2() )
            continue;

		//	동그라미 크기는 다 같으므로 하나만 정해서 좌표를 계산하자
		pRect = RadarInfoParser::Instance()->GetRadarImgInfo( eRadarPointColor );
		if( pRect == NULL ) 
		{
			assert(pRect);
			return;
		}

        radar_point = (*it);

		float fPointSizeX = ((float)pRect->right * 0.5f);
		float fPointSizeY	= ((float)pRect->bottom * 0.5f);
		float fIconCenterX = fImageCtrlCenterX  + (radar_point->fPosX*scaleX) - fPointSizeX - hero_position_x;
		float fIconCenterY = fImageCtrlCenterY  + (radar_point->fPosY* scaleY) - fPointSizeY - hero_position_y ;

        float depth_x = 0.f;
        float depth_y = 0.f;
        float offset_x = 0.f;
        float offset_y = 0.f;

#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
		bool isNpcPos = FALSE;
		if(is_full_map())
		{
			if( (eRADAR_NPC_START <= eRadarPointColor)  && (eRadarPointColor <= eRADAR_NPC_END ) )
			{
				isNpcPos = TRUE;
			}
			else if(eRADAR_DEACTIVE_QUEST_NPC == eRadarPointColor)
			{
				isNpcPos = TRUE;
			}
			else if((eRADAR_REPEAT_QUEST_ACCEPT == eRadarPointColor) ||
				(eRADAR_REPEAT_QUEST_REWARD) == eRadarPointColor)
			{
				isNpcPos = TRUE;
			}
			else if ((eRadarPointColor == eRADAR_NORMAL_QUEST_ACCEPT) || 
				(eRadarPointColor == eRADAR_NORMAL_QUEST_REWARD))
			{
				isNpcPos = TRUE;
			}
			else if ((eRadarPointColor == eRADAR_DAILY_QUEST_ACCEPT) || 
				(eRadarPointColor == eRADAR_DAILY_QUEST_REWARD)) //_NA_003027_20111013_HONOR_SYSTEM
			{
				isNpcPos = TRUE;
			}
			else if ((eRadarPointColor == eRADAR_GUILD_QUEST_ACCEPT) || 
				(eRadarPointColor == eRADAR_GUILD_QUEST_REWARD)) //_NA_003027_20111013_HONOR_SYSTEM
			{
				isNpcPos = TRUE;
			}
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
            else if ((eRadarPointColor == eRADAR_CHALLENGE_DAILY_QUEST_ACCEPT) || 
                (eRadarPointColor == eRADAR_CHALLENGE_DAILY_QUEST_REWARD))
            {
                isNpcPos = TRUE;
            }
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST

			if(isNpcPos)
			{
				RECT UnitRect;
				UnitRect.top = (int)fIconCenterY;
				UnitRect.left = (int)fIconCenterX;
				UnitRect.bottom = (int)fIconCenterY + (int)pRect->bottom;
				UnitRect.right = (int)fIconCenterX + (int)pRect->right;
				ListNpcRect_.push_back(UnitRect);
			}
		}
#endif //#ifdef _NA_000000_20120911_FULL_MISSION_HELPER

#ifdef _NA_004644_20110506_QUEST_NAVIGATION
        if ((fIconCenterX + pRect->right) > rect.right)
        {//잘라서 그리기
            depth_x = fIconCenterX + pRect->right - rect.right;
        }
        else if (fIconCenterX < rect.left && (fIconCenterX + pRect->right) > rect.left)
        {
            offset_x = rect.left - fIconCenterX;
            depth_x = offset_x;
        }

        if ((fIconCenterY + pRect->bottom) > rect.bottom)
        {
            depth_y = fIconCenterY + pRect->bottom - rect.bottom;
        }
        else if (fIconCenterY < rect.top && (fIconCenterY + pRect->bottom) > rect.top)
        {
            offset_y = rect.top - fIconCenterY;
            depth_y = offset_y;
        }
#endif// _NA_004644_20110506_QUEST_NAVIGATION

		//	현재 화면 밖으로 나갔는지 검사해서 출력 유무를 결정하는데
		//	아예 리스트에 등록하기 전에 검사해서 등록을 안하게 바꾸자
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
		if( fIconCenterX > (rect.left-pRect->right) && fIconCenterX < (rect.right/* - pRect->right*/) && 
			fIconCenterY > (rect.top-pRect->bottom) && fIconCenterY < (rect.bottom/* - pRect->bottom*/) )
#else
        if( fIconCenterX > (rect.left) && fIconCenterX < (rect.right - pRect->right) && 
            fIconCenterY > (rect.top) && fIconCenterY < (rect.bottom - pRect->bottom) )
#endif//
		{
			//	마우스 좌표 검사, 이름 툴팁, NPC, Player, Party Player
			BOOL	bRender = FALSE;
			BOOL	bIsNPC = FALSE;

			pRect = RadarInfoParser::Instance()->GetRadarImgInfo( eRadarPointColor );
			if( pRect == NULL) 
			{
                assert(pRect);
#ifdef _DEV_VER
                if (pRect == NULL)
                {
                    GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, "No RadarInfoRect(%d)", eRadarPointColor);
                }    
#endif //_DEV_VER
				continue;
			}

			if( iMouseX > (fIconCenterX) && iMouseX < (fIconCenterX + (float)pRect->right) && 
				iMouseY > (fIconCenterY) && iMouseY < (fIconCenterY + (float)pRect->bottom) )
			{

				if( (eRADAR_NPC_START <= eRadarPointColor)  && (eRadarPointColor <= eRADAR_NPC_END ) )
				{
					bRender = TRUE;
					TextColor = NPCTextColor;
					bIsNPC = TRUE;
				}
				else if( (eRADAR_NPC_START2 <= eRadarPointColor)  && (eRadarPointColor <= eRADAR_NPC_END2 ) )
				{
					bRender = TRUE;
					TextColor = NPCTextColor;
					bIsNPC = TRUE;
				}
				else if(eRADAR_DEACTIVE_QUEST_NPC == eRadarPointColor)
				{
					bRender = TRUE;
					TextColor = DeActiveTextColor;
					bIsNPC = TRUE;
				}
				else if((eRADAR_REPEAT_QUEST_ACCEPT == eRadarPointColor) ||
                    (eRADAR_REPEAT_QUEST_REWARD) == eRadarPointColor)
				{
					bRender = TRUE;
					TextColor = RepeatTextColor;
					bIsNPC = TRUE;
				}
				else if ((eRadarPointColor == eRADAR_NORMAL_QUEST_ACCEPT) || 
                    (eRadarPointColor == eRADAR_NORMAL_QUEST_REWARD))
				{
					bRender = TRUE;
					TextColor = QuestNPCAcceptTextColor;
					bIsNPC = TRUE;
				}
                else if ((eRadarPointColor == eRADAR_DAILY_QUEST_ACCEPT) || 
                    (eRadarPointColor == eRADAR_DAILY_QUEST_REWARD)) //_NA_003027_20111013_HONOR_SYSTEM
                {
                    bRender = TRUE;
                    TextColor = DailyQuestNPCAcceptTextColor;
                    bIsNPC = TRUE;
                }
                else if ((eRadarPointColor == eRADAR_GUILD_QUEST_ACCEPT) || 
                    (eRadarPointColor == eRADAR_GUILD_QUEST_REWARD)) //_NA_003027_20111013_HONOR_SYSTEM
                {
                    bRender = TRUE;
                    TextColor = GuildQuestNPCAcceptTextColor;
                    bIsNPC = TRUE;
                }
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
                else if ((eRadarPointColor == eRADAR_CHALLENGE_DAILY_QUEST_ACCEPT) || 
                    (eRadarPointColor == eRADAR_CHALLENGE_DAILY_QUEST_REWARD))
                {
                    bRender = TRUE;
                    TextColor = ChallengeDailyQuestNPCAcceptTextColor;
                    bIsNPC = TRUE;
                }
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST

				if( eRadarPointColor == eRADAR_PC_POINT )
				{
					bRender = TRUE;
					TextColor = PlayerTextColor;
				}
				else if( eRadarPointColor == eRADAR_PARTY_POINT || eRadarPointColor >= eRADAR_PARTY_POINT_01 && eRadarPointColor <= eRADAR_PARTY_POINT_10 )
				{
					bRender = TRUE;
					TextColor = PartyTextColor;
				}
				else if( eRadarPointColor == eRADAR_TIMEATTACK )
				{
					bRender = TRUE;
					TextColor = TimeAttackTextColor;
				}
                else if (eRadarPointColor == eRADAR_AIRSHIP_PORTAL)
                {
                    bRender = TRUE;
                    TextColor = QuestNPCTextColor;
                }
				else if(eRadarPointColor >= eRADAR_NAVI_POINT_YELLOW && eRadarPointColor <= eRADAR_NAVI_ARROW_PERPLE)
				{
					bRender = TRUE;
					TextColor = QuestNPCTextColor;
				}
				else if(eRadarPointColor >= eRADAR_NORMAL_PORTAL && eRadarPointColor <= eRADAR_WAY_POINT)
				{
					bRender = TRUE;
					TextColor = QuestNPCTextColor;
				}
				else if(eRADAR_CRYSTAL_WARP == eRadarPointColor)
				{
					bRender = TRUE;
					TextColor = QuestNPCTextColor;
				}
				else if(eRADAR_BLAST == eRadarPointColor)
				{
					bRender = TRUE;
					TextColor = QuestNPCTextColor;
				}
				else if(eRADAR_AIRSHIP == eRadarPointColor)
				{
					bRender = TRUE;
					TextColor = QuestNPCTextColor;
				}
                else if (eRadarPointColor >= eRADAR_BATTLE_GROUND_STRONGPOINT_NORMAL &&
                    eRadarPointColor <= eRADAR_BATTLE_GROUND_STRONGPOINT_BLUE)
                {
                    bRender = TRUE;
                    TextColor = QuestNPCTextColor;
                }
			}

			if( bRender )
			{
				uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
				if(pUIMan)
				{

					DWORD sp_tooltip = eSPECIAL_TOOLTIP_NONE;
					sp_tooltip = eSPECIAL_TOOLTIP_NOMAL_ITEM;
					pUIMan->RegisterTooltip(radar_point->szName, -1,  22, FontID_8, TextColor, TOOLTIP_BG_WZCOLOR, c_dwTOOLTIP_EXTRA_INFO_DT_CENTER, sp_tooltip);
					pUIMan->SetTooltipDialogID( GetDialogKey() );
					
				}

			}

			//	동그란 점
			pRect = RadarInfoParser::Instance()->GetRadarImgInfo( eRadarPointColor );
			if(NULL == pRect)
			{
				assert(!"BaseRadarDialog::Wrap_render::NULL == pRect");
				return;
			}

            // _YJ_NAVI_SYSTEM_001277
			if(eRADAR_NAVI_POINT_YELLOW <= eRadarPointColor && eRADAR_NAVI_POINT_PERPLE >= eRadarPointColor)
			{
				int iSamePos = -1;
				if(0 != radar_point->dwObjectKey)
				{
					if(eRADAR_NAVI_POINT_RED == eRadarPointColor)
						dwNaviSamePosObjID[0] = radar_point->dwObjectKey;
					else if(eRADAR_NAVI_POINT_GREEN == eRadarPointColor)
						dwNaviSamePosObjID[1] = radar_point->dwObjectKey;
					else if(eRADAR_NAVI_POINT_BLUE == eRadarPointColor)
						dwNaviSamePosObjID[2] = radar_point->dwObjectKey;
					else if(eRADAR_NAVI_POINT_PERPLE == eRadarPointColor)
						dwNaviSamePosObjID[3] = radar_point->dwObjectKey;

					for(int i = 0; i < MAX_NAVI_CNT; ++i)
					{
						if(radar_point->dwObjectKey == dwNaviSamePosObjID[i])
							iSamePos++;
					}
				}
				else
				{
					iSamePos = 0;
				}

				if(0 != iSamePos)
				{
					//회전한 이후에 Dlg영역 안에 들어오는지 체크
					if((fIconCenterY + (float)pRect->bottom) < (rect.top) || (fIconCenterY > rect.bottom))
					{
						continue;
					}
                    if((fIconCenterX + (float)pRect->right) < (rect.left) || (fIconCenterX > rect.right))
                    {
                        continue;
                    }
				}

				float fRot = (36.f*WZ_PI*iSamePos) / 180.f;
	
                GetDrawBase()->RenderTexture( RadarInfoParser::Instance()->GetTexture(),
                    fIconCenterX + offset_x,
                    //fIconCenterX - 2.f,   //!< 2010.8.12 / i4u4me / 중심위치 수정
                    fIconCenterY + offset_y,
                    (float)pRect->right - depth_x * scaleX,
                    (float)pRect->bottom - depth_y * scaleY,
                    (float)pRect->left + offset_x,
                    (float)pRect->top + offset_y,
                    (float)pRect->right - depth_x,
                    (float)pRect->bottom - depth_y,
                    wcRadarPointColor,
                    0.0f, 
                    RTO_DEFAULT,
                    fRot);
			}
			else
			{
				GetDrawBase()->RenderTexture( RadarInfoParser::Instance()->GetTexture(),
                                        fIconCenterX + offset_x,
                                        fIconCenterY + offset_y,
                                        (float)pRect->right - depth_x * scaleX,
                                        (float)pRect->bottom - depth_y * scaleY,
                                        (float)pRect->left + offset_x,
                                        (float)pRect->top + offset_y,
                                        (float)pRect->right - depth_x,
                                        (float)pRect->bottom - depth_y,
					                    wcRadarPointColor,
					                    0.0f);

                //! 타겟팅된것 애니메이션 
                bool is_ani_rader = false;
                bool is_monster_radar = ((eRadarPointColor == eRADAR_MONSTER_POINT) || 
                                         (eRadarPointColor == eRADAR_MONSTER_DEAD_POINT) || 
                                         (eRadarPointColor == eRADAR_BOSS_MONSTER_POINT) || 
                                         (eRadarPointColor == eRADAR_BOSS_MONSTER_DEAD_POINT) || 
                                         (eRadarPointColor == eRADAR_PC_POINT) ||
                                         (eRadarPointColor == eRADAR_TEAM_PLAYER_DEAD_POINT));
                if(is_monster_radar)
                {
                    TargetDialog* target_dialog = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
                    if ((target_dialog != NULL) && 
                        (target_dialog->IsVisible() == TRUE) && 
                        (target_dialog->GetTargetKey() == radar_point->dwObjectKey))
                    {
                            is_ani_rader = true;
                    }
                }

#ifdef _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR
                if (eRadarPointColor == eRADAR_COLLECT_TARGET )
                {
                    if( pRect = RadarInfoParser::Instance()->GetRadarImgInfo(eRADAR_COLLECT_TARGET_ANIMATION) )
                    {
                        fPointSizeX = ((float)pRect->right * 0.5f);
                        fPointSizeY	= ((float)pRect->bottom * 0.5f);
                        fIconCenterX = fImageCtrlCenterX + radar_point->fPosX - fPointSizeX - hero_position_x;
                        fIconCenterY = fImageCtrlCenterY + radar_point->fPosY - fPointSizeY - hero_position_y;

                        float max_alpha = 255.f;
                        if(GetRadarState() == eFULL_RADAR_STATE_ALPHA)
                        {
                            max_alpha = 150.f;
                        }

                        WzColor render_color = WzColor_RGBA(255,255,255,(BYTE)(max_alpha*alpha_blend_rate));
                        
                        GetDrawBase()->RenderTexture( RadarInfoParser::Instance()->GetTexture(),
                            fIconCenterX + offset_x,
                            fIconCenterY + offset_y,
                            (float)pRect->right - depth_x * scaleX,
                            (float)pRect->bottom - depth_y * scaleY,
                            (float)pRect->left + offset_x,
                            (float)pRect->top + offset_y,
                            (float)pRect->right - depth_x,
                            (float)pRect->bottom - depth_y,
                            &render_color,
                            0.0f);
                    }
                }
#endif

                if (is_ani_rader)
                {
                    bool is_party_member = false;
                    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
                    if ((party_manager != NULL) && 
                        (party_manager->GetPartyMemberInfo(radar_point->dwObjectKey) != NULL))
                    {
                        is_party_member = true;
                    }

                    bool is_monster_or_cao = false;
                    Object* object_ptr = g_ObjectManager.GetObject(radar_point->dwObjectKey);
                    if ((object_ptr != NULL) && 
                         ((object_ptr->IsKindOfObject(MONSTER_OBJECT) == TRUE) || 
                          (object_ptr->IsKindOfObject(PLAYER_OBJECT) == TRUE))
                        )
                    {
                        is_monster_or_cao = true;
                    }

                    if (is_party_member || is_monster_or_cao)
                    {
                        static int radar_ani_count = eRADAR_TARGET_NORMAL_MONSTER_ANI_POINT_5 - eRADAR_TARGET_NORMAL_MONSTER_ANI_POINT_0;
                        static int radar_ani_index = 0;
                        static DWORD radar_ani_prev_tick = clock_function::GetTickCount();
                        if (radar_ani_prev_tick + 100 < clock_function::GetTickCount())
                        {
                            radar_ani_index ++;
                            if (radar_ani_index > radar_ani_count)
                            {
                                radar_ani_index = 0;
                            }

                            radar_ani_prev_tick = clock_function::GetTickCount();
                        }

                        int start_ani_radar = static_cast<int>(eRADAR_TARGET_NORMAL_MONSTER_ANI_POINT_0);
                        if ((eRadarPointColor == eRADAR_BOSS_MONSTER_POINT) || 
                            (eRadarPointColor == eRADAR_BOSS_MONSTER_DEAD_POINT) || 
                            (eRadarPointColor == eRADAR_PC_POINT)|| 
                            (eRadarPointColor == eRADAR_TEAM_PLAYER_DEAD_POINT))
                        {
                            start_ani_radar = static_cast<int>(eRADAR_TARGET_BOSS_MONSTER_ANI_POINT_0);
                        }

                        pRect = RadarInfoParser::Instance()->GetRadarImgInfo(start_ani_radar + radar_ani_index);
                        if( pRect == NULL ) 
                        {
                            assert(pRect);
                            return;
                        }

                        fPointSizeX = ((float)pRect->right * 0.5f);
                        fPointSizeY	= ((float)pRect->bottom * 0.5f);
                        fIconCenterX = fImageCtrlCenterX  + (radar_point->fPosX*scaleX) - fPointSizeX - hero_position_x;
                        fIconCenterY = fImageCtrlCenterY  + (radar_point->fPosY* scaleY) - fPointSizeY - hero_position_y ;

                        GetDrawBase()->RenderTexture( RadarInfoParser::Instance()->GetTexture(),
                                                fIconCenterX + offset_x,
                                                fIconCenterY + offset_y,
                                                (float)pRect->right - depth_x * scaleX,
                                                (float)pRect->bottom - depth_y * scaleY,
                                                (float)pRect->left + offset_x,
                                                (float)pRect->top + offset_y,
                                                (float)pRect->right - depth_x,
                                                (float)pRect->bottom - depth_y,
                                                wcRadarPointColor,
                                                0.0f);
                    }
                }
			}
		}

#ifdef _NA_004644_20110506_QUEST_NAVIGATION
        if( fIconCenterX > (rect.left - pRect->right + 5) && fIconCenterX < (rect.right - 5) && 
            fIconCenterY > (rect.top - pRect->bottom + 5) && fIconCenterY < (rect.bottom - 5))
        {

        }
#endif//_NA_004644_20110506_QUEST_NAVIGATION
        // _YJ_NAVI_SYSTEM_001277
		else if(eRADAR_NAVI_POINT_YELLOW <= eRadarPointColor && eRADAR_NAVI_POINT_PERPLE >= eRadarPointColor)
		{
			pRect = RadarInfoParser::Instance()->GetRadarImgInfo( eRadarPointColor + 5);

#ifdef _NA_004644_20110506_QUEST_NAVIGATION
            //45도 기울어져 있기 때문에 +- 6.f;
            if (fIconCenterY > (rect.bottom - pRect->bottom))
            {
                //fIconCenterX += 6.f;
                fIconCenterY = (float)rect.bottom - (float)pRect->bottom; 
            }
            else if(fIconCenterY < rect.top)
            {
                //fIconCenterX += 6.f;
                fIconCenterY = (float)rect.top;// - 6.f; 
            }

            if (fIconCenterX > (rect.right - pRect->right))
            {
                fIconCenterX = (float)rect.right - (float)pRect->right; 
            }
            else if (fIconCenterX < rect.left)
            {
                fIconCenterX = (float)rect.left;
            }
#else
			//45도 기울어져 있기 때문에 +- 6.f;
			if(fIconCenterY > rect.bottom - pRect->bottom)
			{
				fIconCenterX += 6.f;
				fIconCenterY = (float)rect.bottom - (float)pRect->bottom + 6.f; 
			}
			else if(fIconCenterY < rect.top)
			{
				fIconCenterX += 6.f;
				fIconCenterY = (float)rect.top - 6.f; 
			}

			if( fIconCenterX >  rect.right - pRect->right)
				fIconCenterX = (float)rect.right - (float)pRect->right; 
			else if(fIconCenterX < rect.left)
				fIconCenterX = (float)rect.left;
#endif//_NA_004644_20110506_QUEST_NAVIGATION

			float fRot = 0.f;
			float fMod = 1.f;
            bool is_visible = true;
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
            int rect_width = ((rect.right - rect.left - pRect->right)/3);
            int rect_height = ((rect.bottom - rect.top - pRect->bottom)/3);

            
            int position = 0;

            int x = static_cast<int>(fIconCenterX - rect.left);
            int y = static_cast<int>(fIconCenterY - rect.top);

            if (x > 0)
            {
                x = x / rect_width;
                x += 1;
            }
            else
            {
                x = 0;
            }

            if (y > 0)
            {
                y = y / rect_height;
                y += 1;
            }
            else
            {
                y = 0;
            }

            if (x > 4)
            {
                x = 4;
            }
            if (y > 4)
            {
                y = 4;
            }
            y = y * 5;

            /*
                 0  1  2  3  4
                 5  6  7  8  9
                 10 11 12 13 14
                 15 16 17 18 19
                 20 21 22 23 24

                 12는 내위치
            */
            switch (x+y)
            {
            case 0:
            case 6:
                fRot = -315.f; // 좌상단
                fIconCenterY -= 4.f;
                break;
            case 1:
                fRot = -337.5f; // 좌좌상단
                fIconCenterY -= 2.f;
                break;
            case 2:
            case 7:
                fRot = 0.f; // 상단
                break;
            case 3:
                fRot = -22.5f; // 우우상단
                fIconCenterY -= 2.f;
                break;
            case 4:
            case 8:
                fRot = -45.f; // 우상단
                fIconCenterY -= 4.f;
                break;
            case 5:
                fRot = -292.5f; // 
                fIconCenterX += 2.f;
                break;
            case 9:
                fRot = -67.5f; //
                fIconCenterX -= 3.f;
                break;
            case 10:
            case 11:
                fRot = -270.f; // 좌
                fIconCenterX += 4.f;
                break;
            case 14:
            case 13:
                fRot = -90.f; // 우
                fIconCenterX -= 4.f;
                break;
            case 15:
                fRot = -247.5f; //
                fIconCenterX += 2.f;
                break;
            case 19:
                fRot = -112.5f; //
                fIconCenterX -= 3.f;
                break;
            case 20:
            case 16:
                fRot = -225.f; // 좌하단
                fIconCenterY += 4.f;
                break;
            case 21:
                fRot = -202.5f; //
                fIconCenterY += 2.f;
                break;
            case 22:
            case 17:
                fRot = 180.f; // 하단
                break;
            case 23:
                fRot = -157.5f; //
                fIconCenterY += 2.f;
                break;
            case 24:
            case 18:
                fRot = -135.f; // 우하단
                fIconCenterY += 4.f;
                break;
            case 12:
            default://
                fRot = 0.f;
                break;
            }

            fRot = (fRot * WZ_PI) /  180.f;
            fMod = -1.f;
#else
			//미니맵의 좌하단에 있다면
			if( fIconCenterX < ( rect.left + ((rect.right - rect.left)/2) ) && fIconCenterY > ( rect.top + ((rect.bottom - rect.top)/2) ) )  
			{
				fRot = (-225.f * WZ_PI) / 180.f;
				fMod = -1.f;
			}
			//우하단
			else if( fIconCenterX > ( rect.left + ((rect.right - rect.left)/2) ) && fIconCenterY > ( rect.top + ((rect.bottom - rect.top)/2) ) )
			{
				fRot = (-135.f * WZ_PI) / 180.f;
			}
			//좌상단
			else if( fIconCenterX < ( rect.left + ((rect.right - rect.left)/2) ) && fIconCenterY < ( rect.top + ((rect.bottom - rect.top)/2) ) )
			{
				fRot = (-315.f * WZ_PI) / 180.f;
				fMod = -1.f;
			}
			//우상단
			else if( fIconCenterX > ( rect.left + ((rect.right - rect.left)/2) ) && fIconCenterY < ( rect.top + ((rect.bottom - rect.top)/2) ) )
			{
				fRot = (-45.f * WZ_PI) / 180.f;
			}
#endif//_NA_004644_20110506_QUEST_NAVIGATION

			int iSamePos = -1;
			if(0 != radar_point->dwObjectKey)
			{
				if(eRADAR_NAVI_POINT_RED == eRadarPointColor)
					dwNaviSamePosObjID[0] = radar_point->dwObjectKey;
				else if(eRADAR_NAVI_POINT_GREEN == eRadarPointColor)
					dwNaviSamePosObjID[1] = radar_point->dwObjectKey;
				else if(eRADAR_NAVI_POINT_BLUE == eRadarPointColor)
					dwNaviSamePosObjID[2] = radar_point->dwObjectKey;
				else if(eRADAR_NAVI_POINT_PERPLE == eRadarPointColor)
					dwNaviSamePosObjID[3] = radar_point->dwObjectKey;

				for(int i = 0; i < MAX_NAVI_CNT; ++i)
				{
					if(radar_point->dwObjectKey == dwNaviSamePosObjID[i])
						iSamePos++;
				}
			}
			else
			{
				iSamePos = 0;
			}

			if(1 == iSamePos)
			{
				fIconCenterX += 1.25f*fMod;
				fIconCenterY -= 1.25f*fMod;
				fRot -= (15.f*WZ_PI) / 180.f;
			}
			else if(2 == iSamePos)
			{
				fIconCenterX += 2.5f*fMod;
				fIconCenterY -= 2.5f*fMod;
				fRot -= (30.f*WZ_PI) / 180.f;
			}
			else if(3 == iSamePos)
			{
				fIconCenterX -= 1.25f*fMod;
				fIconCenterY += 1.25f*fMod;
				fRot += (15.f*WZ_PI) / 180.f;
				
			}
			else if(4 == iSamePos)
			{
				fIconCenterX -= 2.5f*fMod;
				fIconCenterY += 2.5f*fMod;
				fRot += (30.f*WZ_PI) / 180.f;
			}

            if (is_visible)
            {
			    GetDrawBase()->RenderTexture( RadarInfoParser::Instance()->GetTexture(),
				    fIconCenterX,
				    fIconCenterY,
				    (float)pRect->right,
				    (float)pRect->bottom,
				    (float)pRect->left,
				    (float)pRect->top,
				    (float)pRect->right,
				    (float)pRect->bottom,
				    wcRadarPointColor,
				    0.0f, 
				    RTO_DEFAULT,
				    fRot);
            }

			if( iMouseX > (fIconCenterX) && iMouseX < (fIconCenterX + (float)pRect->right) && 
				iMouseY > (fIconCenterY) && iMouseY < (fIconCenterY + (float)pRect->bottom) )
			{
				uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
				if(pUIMan)
				{
					DWORD sp_tooltip = eSPECIAL_TOOLTIP_NONE;
					sp_tooltip = eSPECIAL_TOOLTIP_NOMAL_ITEM;

					pUIMan->RegisterTooltip( radar_point->szName, 
											 -1,  
											 22, 
											 FontID_8, 
											 QuestNPCTextColor, 
											 TOOLTIP_BG_WZCOLOR, 
											 c_dwTOOLTIP_EXTRA_INFO_DT_CENTER,
											 sp_tooltip);
					pUIMan->SetTooltipDialogID( GetDialogKey() );

				}
			}
		}
	}

	//	주인공 방향 화살표 랜더링
	//	RenderSprite() 시리즈는 나중에 모아서 소팅해서 랜더링하지만
	//	RealizeRenderSprite() 시리즈는 지금 현재 바로 랜더링을 걸어서 소팅이 안된다
	WzVector	wvDir, wvNormal;
	SetVector( &wvDir, 0.0f, 0.0f, 1.0f );

	if( g_pHero )
	{
		wvDir = g_pHero->GetDirection();
	}
	VectorNormalize( &wvNormal, &wvDir );

	POINT	TextureSize = RadarInfoParser::Instance()->GetTextureSize();

	pRect = RadarInfoParser::Instance()->GetRadarImgInfo( eRADAR_HERO_ARROW );
	if(pRect)
	{
		float fArrowDistance = ((float)pRect->bottom * 0.4f);
		wvNormal = wvNormal * fArrowDistance;
	}

	if( pHeroRadarPoint != NULL )
	{
		const float		fUV_REVISION = 0.025f;

		g_pSunRenderer->Set2DMode( FALSE );
		WzVector	wvPos;
		WzRay		wrRay;
		POINT		point;

        point.x = (LONG)((fImageCtrlCenterX + (pHeroRadarPoint->fPosX*scaleX) - hero_position_x) * fWidth + 0.5f);
        point.y = (LONG)((fImageCtrlCenterY + (pHeroRadarPoint->fPosY*scaleY) - hero_position_y) * fHeight + 0.5f);

        g_pSunRenderer->GetMouseCursorRay( &wrRay, &point );
		wvPos = wrRay.m_wvOrigin + ( wrRay.m_wvDirection * ARROW_DISTANCE );

		pRect = RadarInfoParser::Instance()->GetRadarImgInfo( eRADAR_HERO_ARROW );
        POINT radar_texture_size = RadarInfoParser::Instance()->GetTextureSize();

		WzUVVert	UVVert[2];
		UVVert[0].u = ( (float)(pRect->left) / (float)radar_texture_size.x);
		UVVert[0].v = ( (float)(pRect->top) / (float)radar_texture_size.y);
		UVVert[1].u = ( (float)(pRect->left + pRect->right) / (float)radar_texture_size.x);
		UVVert[1].v = ( (float)(pRect->top + pRect->bottom) / (float)radar_texture_size.y);
		
		float	fDirection = WZ_PI + Math_GetAngleFromVector( &wvNormal );

		GetDrawBase()->RealizeRenderSprite( RadarInfoParser::Instance()->GetTexture(), wvPos, ARROW_SCALE, fDirection, 0.0f, UVVert, EBBT_SPRITE, NULL );

		g_pApplication->Set2DMode();
	}
	//클릭 지점 랜더링
	if(click_animation.display_time() > 0)
	{
		RECT	*pRect;

		float fX2d,fY2d;

        fX2d = click_animation.click_position_x() - hero_position_x;
        fY2d = click_animation.click_position_y() - hero_position_y;

		//	동그라미 크기를 구해서 좌표를 계산하자
        int ani_frame_index = 
            (click_animation.display_time() / uiRadarClickAnimation::kTimePerFrame) % uiRadarClickAnimation::kMaxClickAniNum;
	
		DWORD pos = ani_frame_index + eRADAR_CLICK_POINT_01;
		pRect = RadarInfoParser::Instance()->GetRadarImgInfo( pos );

		if(pRect)
		{
			float		fPointSizeX = (float)pRect->right * 0.5f;
			float		fPointSizeY = (float)pRect->bottom * 0.5f;
			float		fPosX = fImageCtrlCenterX + fX2d  - 32;
			float		fPosY = fImageCtrlCenterY + fY2d  - 32;

			//	아예 리스트에 등록하기 전에 검사해서 등록을 안하게 바꾸자
			if( fPosX > (rect.left - fPointSizeX ) && fPosX < (rect.right - fPointSizeX ) && 
				fPosY > (rect.top - fPointSizeY ) && fPosY < (rect.bottom - fPointSizeY ) )
			{
				GetDrawBase()->RenderTexture( RadarInfoParser::Instance()->GetTexture(),
					fPosX,
					fPosY,
					(float)pRect->right,
					(float)pRect->bottom,
					(float)pRect->left,
					(float)pRect->top,
					(float)pRect->right,
					(float)pRect->bottom,
					wcRadarPointColor,
					0.0f
					);
			}
		}
	}

	if(TRUE == g_pMap->IsField() || TRUE == g_pMap->IsVill())
	{
 		TCHAR szID[5] = {0,};
		
		PORTAL_OFFSET_PARAM Param;
		Param.bInPut = TRUE;

		for(int i = 0; i < 10; ++i)
		{
			//오프셋정보
			Param.INPUT_INFO.nCol = i;
			Param.INPUT_INFO.nRow = 0;
			Sprintf(szID, _T("PRT%d"), i);//포탈1
			RenderPortalName(szID, pCtrlPicture,&Param);
			Param.INPUT_INFO.nRow = 1;
			Sprintf (szID, _T("OUT%d"), i);//포탈2
			RenderPortalName(szID, pCtrlPicture,&Param);
			Param.INPUT_INFO.nRow = 2;
			Sprintf (szID, _T("MP0%d"), i);//포탈3
			RenderPortalName(szID, pCtrlPicture,&Param);
		}
	}

	//	랜더 스테이트 복구
	GetDrawBase()->SetZBufferTest(bZWriteTest);
	GetDrawBase()->SetZBufferWrite( bZWrite );
	GetDrawBase()->SetAlphaBlendMode( eAlphablendMode, FALSE );
	GetDrawBase()->EnableAlphaTest( bIsEnableAlphaTest );

}

//------------------------------------------------------------------------------
/**  북미버전에서 포탈이름이 너무길어서 다른 포탈이름과 텍스트가 겹쳐버린다
	 이함수가 호출될때 겹쳐지는 포탈명에대해서 Offset값을 계산해주는함수
	 포탈인덱스(PRT,OUT,MPO)가 추가되면 여기도 수정해야됨
*/
void BaseRadarDialog::CalcPortalOffset(CCtrlPictureWZ * pCtrlPicture)
{
	const int c_iPortalType =3;
	const int c_iMaxPortal  =10;

	memset(&m_PortalOffset,0,sizeof(m_PortalOffset));
	
	PORTAL_OFFSET_PARAM Param;
	RECT rTempRect[c_iPortalType*c_iMaxPortal];
	TCHAR szID[5] = {0,};
	int i =0;
	memset(&rTempRect,0,sizeof(rTempRect));

	//STEP1: 현3가지포탈인덱스에 대해 RenderPortalName호출해 랜더링시 뿌려질영역 추출
	for(; i < c_iMaxPortal; ++i)
	{
		memset(&Param,0,sizeof(Param));
		Sprintf(szID, _T("PRT%d"), i);//포탈1
		if(RenderPortalName(szID, pCtrlPicture,&Param))
			rTempRect[i] = Param.OUTPUT_INFO.nRect;
	}
	
	for(i=0; i < c_iMaxPortal; ++i)
	{
		memset(&Param,0,sizeof(Param));
		Sprintf (szID, _T("OUT%d"), i);//포탈2
		if(RenderPortalName(szID, pCtrlPicture,&Param))
			rTempRect[i+10] = Param.OUTPUT_INFO.nRect;

	}
	for(i=0; i < c_iMaxPortal; ++i)
	{
		memset(&Param,0,sizeof(Param));
		Sprintf (szID, _T("MP0%d"), i);//포탈3
		if(RenderPortalName(szID, pCtrlPicture,&Param))
			rTempRect[i+20] = Param.OUTPUT_INFO.nRect;
	}

	//STEP2: rTempRect안에 모든포탈의 영역이 담겨있다 배열[30]중의 순번이 앞서있는것이 우선순위
	//		 예를들어 1번과 2번이 겹칠경우 2번을 내려찍어준다

#ifdef _NA_000000_20151015_CALC_PORTAL_OFFSET_PROBLEM_MODIFY
    int try_count = 0;
#endif //_NA_000000_20151015_CALC_PORTAL_OFFSET_PROBLEM_MODIFY

	for(int a=0; a<c_iPortalType*c_iMaxPortal; ++a)
	{
		for(int b=0; b<a; ++b)
		{
			//자신과는 비교하지않는다.
			if(b==a) continue;
	
			//기본체크(값없는것들은 continue)
			if((rTempRect[a].right==0 && rTempRect[a].bottom==0) || 
				(rTempRect[b].right==0 && rTempRect[b].bottom==0)) continue;
			
			POINT pt[4]={{rTempRect[a].left,rTempRect[a].top},
						 {rTempRect[a].right,rTempRect[a].top},
						 {rTempRect[a].left,rTempRect[a].bottom},
						 {rTempRect[a].right,rTempRect[a].bottom}};
							
			for(int cnt=0;cnt<4;++cnt)
			{
				//겹치는지체크 시작점(top,left)이 rect영역 안에들어가있는지체크
				if(rTempRect[b].left <= pt[cnt].x && rTempRect[b].right >= pt[cnt].x)
				{
					if (rTempRect[b].top <= pt[cnt].y && rTempRect[b].bottom >= pt[cnt].y)
					{ 
						int row =a/c_iMaxPortal;
						int col =a%c_iMaxPortal;
						int gap= 0;
						//차이값구하기
						gap  = (rTempRect[a].top>rTempRect[b].top) ? (rTempRect[b].bottom-rTempRect[a].top):(rTempRect[b].top-rTempRect[a].bottom);
						gap = (gap>=0)?(gap+2):(gap-2);
						//텍스트가 겹쳤다. 약간내려준다.
						m_PortalOffset[row][col] +=gap; 
						rTempRect[a].top+=gap;
						rTempRect[a].bottom+=gap;

#ifdef _NA_000000_20151015_CALC_PORTAL_OFFSET_PROBLEM_MODIFY
                        if (try_count < 10)
                        {
                            ++try_count;
                            b = 0; //다시한번더 돌린다. 딴거와 충돌될수도있기때문에
                        }
#else
                        b= 0; //다시한번더 돌린다. 딴거와 충돌될수도있기때문에
#endif //_NA_000000_20151015_CALC_PORTAL_OFFSET_PROBLEM_MODIFY
					}
				}
			}
		}
	}
}

BOOL BaseRadarDialog::RenderPortalName(char* PortalID, CCtrlPictureWZ * pCtrlPicture,void *pParm)
{
	if(NULL == pCtrlPicture)
		return FALSE;

    if (g_pHero == NULL)
    {
        return FALSE;
    }

    uiRadarMan* ui_radar_manager = static_cast<uiRadarMan*>(GetMan());
    if (ui_radar_manager == NULL)
    {
        return FALSE;
    }

    uiRadarMapData& map_data = ui_radar_manager->radar_map_data();
    float hero_position_x;
    float hero_position_y;
    float scaleX = 1.0f;
    float scaleY = 1.0f;

    if (is_full_map() == true)
    {
#ifdef _NA_000000_20130422_SCALE_FULLMAP
        if(m_bResizefullmap)
        {
            scaleX = 0.5f;
            scaleY = 0.5f;
        }
#endif

        hero_position_x = -(((map_data.map_image_width()*scaleX) / 2.0f) - GetRadarImageWidth() * scaleX);
        hero_position_y = -(((map_data.map_image_height()*scaleY) / 2.0f) - GetRadarImageHeight() * scaleY);
    }
    else
    {
        hero_position_x = map_data.transform_position().x;
        hero_position_y = map_data.transform_position().y;
    }

	TCHAR szFieldName[INTERFACE_STRING_LENGTH + 1] = {0,};
	WzVector wvPos;

	RECT rect = pCtrlPicture->GetSizeRect();
	float fImageCtrlCenterX = (float)rect.left + (float)(rect.right - rect.left) * 0.5f;
	float fImageCtrlCenterY = (float)rect.top + (float)(rect.bottom - rect.top) * 0.5f;

	BOOL bVal = g_pMap->GetAreaPosition(&wvPos, StrToWzID(PortalID), FALSE);
	if(FALSE == bVal)
		return FALSE;

	WORD wIndex = MapInfoParser::Instance()->GetCurrentMapPortalIndex(PortalID, g_pMap->GetCurrentFieldID());
	if (0 == wIndex)
		return FALSE;

	sMAP_ENTERENCE_PORTAL* entPortal = MapInfoParser::Instance()->GetMapPortal( wIndex );
	if(NULL == entPortal)
		return FALSE;

	const sMAPINFO* pMI = MapInfoParser::Instance()->FindMapInfo(entPortal->wWorldTo);
	if(NULL == pMI)
		return FALSE;

	//////////////////////////////////////////////////////////////////////////
    wvPos = CalculatePointPosition(wvPos.x, wvPos.y, wvPos.z);

    float fIconCenterX = fImageCtrlCenterX + (wvPos.x*scaleX)  - hero_position_x;
    float fIconCenterY = fImageCtrlCenterY + (wvPos.z*scaleY)  - hero_position_y;

	WzColor wcText = WzColor_RGBA(255,255,255,200);
	if(eMINI_MAP != GetRadarState())
	{
		if(eFULL_RADAR_STATE_ALPHA == GetRadarState())
		{
			wcText = WzColor_RGBA(255,255,255,150);
		}
	}

	//////////////////////////////////////////////////////////////////////////

	g_InterfaceManager.GetInterfaceString(pMI->dwNCode, szFieldName);

#ifdef _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG
	SIZE siText;
	int length = strlen(szFieldName);
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("mn12") );	
	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPointA( szFieldName, length, &siText);
#endif // _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG

	RECT DlgRect = pCtrlPicture->GetSizeRect();
#ifdef _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG
	float real_cx = (float)g_pSunRenderer->RealXToStdX((siText.cx) / 2.0f);
	if(fIconCenterY - 32 > DlgRect.top && fIconCenterX - real_cx > DlgRect.left)
#else
	if(fIconCenterY > DlgRect.top && fIconCenterX > DlgRect.left)
#endif // _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG
	{
		RECT drawRect;
		memset(&drawRect,0,sizeof(drawRect));

#ifdef _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG
#else
		int length = strlen(szFieldName);

		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("mn12") );	
#endif // _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG
		drawRect.top = (LONG)fIconCenterY;
		drawRect.left = (LONG)fIconCenterX;

#ifdef _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG
#else
		SIZE siText;
		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPointA( szFieldName, length, &siText);
#endif // _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG

        LONG isLeftSize = (LONG)g_pSunRenderer->RealXToStdX((float)siText.cx) / 2;
        drawRect.left -= isLeftSize;
        drawRect.top -= 32;

		//북미버전에서 포탈이름이 겹쳐서 나오는버그때문에 이를 방지하기위한 작업디파인
		//pDrawRect가 NULL이 아닐때는 Rect정보를 얻는다는뜻
		if(pParm)
		{
			PORTAL_OFFSET_PARAM* pInfo  = (PORTAL_OFFSET_PARAM*)pParm;
			
			//true이면 옵셋값을 그대로 적용하고 false일때는 값을 그릴영역값을 얻는다.
			if(pInfo->bInPut)
			{
				drawRect.top    += m_PortalOffset[pInfo->INPUT_INFO.nRow][pInfo->INPUT_INFO.nCol];
				drawRect.bottom += m_PortalOffset[pInfo->INPUT_INFO.nRow][pInfo->INPUT_INFO.nCol];

			}
			else
			{
				//값만 추출하고 리턴한다.
				SIZE textsize;
				g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szFieldName,lstrlen(szFieldName),&textsize);
				pInfo->OUTPUT_INFO.nRect.left	= drawRect.left;
				pInfo->OUTPUT_INFO.nRect.top	= drawRect.top;
				pInfo->OUTPUT_INFO.nRect.bottom	= drawRect.top+textsize.cy;
				pInfo->OUTPUT_INFO.nRect.right	= drawRect.left+textsize.cx;
				g_pSunRenderer->x_pManagerTextOut->EndUseFont();
				return TRUE;
			}
		}

		drawRect.bottom = DlgRect.bottom;
		drawRect.right = DlgRect.right;
		g_pSunRenderer->x_pManagerTextOut->DrawText(szFieldName, &drawRect, wcText);
#ifdef _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG
#else
		g_pSunRenderer->x_pManagerTextOut->EndUseFont();
#endif // _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG
	}

#ifdef _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
#endif // _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG

	return TRUE;
}


#ifdef _NA_0_20111013_RADAR_PATHFINDING
bool BaseRadarDialog::PathFindMove( CControlWZ* pPictureControl )
{
    SCENE_TYPE SceneType = GameFramework::GetCurScene();
    if( SceneType == SCENE_TYPE_VILLAGE ||
        SceneType == SCENE_TYPE_FIELD ||
        SceneType == SCENE_TYPE_EVENT )
    {
        // NPC 길찾기 처리
        if( uiRadarMan* ui_radar_manager = static_cast<uiRadarMan*>(GetMan()) )
        {
            RECT picture_rect = pPictureControl->GetSizeRect();

            float fImageCtrlCenterX;
            float fImageCtrlCenterY;
            fImageCtrlCenterX = (float)picture_rect.left + (float)(picture_rect.right - picture_rect.left) * 0.5f;
            fImageCtrlCenterY = (float)picture_rect.top + (float)(picture_rect.bottom - picture_rect.top) * 0.5f;


            uiRadarMapData& map_data = ui_radar_manager->radar_map_data();

            float hero_position_x;
            float hero_position_y;
            float scaleX = 1.0f;
            float scaleY = 1.0f;

            if (is_full_map() == true)
            {
#ifdef _NA_000000_20130422_SCALE_FULLMAP
                if(m_bResizefullmap)
                {
                    scaleX = 0.5f;
                    scaleY = 0.5f;
                }
#endif

                hero_position_x = -((map_data.map_image_width()*scaleX) / 2.0f - (GetRadarImageWidth()*scaleX));
                hero_position_y = -((map_data.map_image_height()*scaleY) / 2.0f - (GetRadarImageHeight()*scaleY));
            }
            else
            {
                hero_position_x = map_data.transform_position().x;
                hero_position_y = map_data.transform_position().y;
            }


            uiRadarPointCollecter& point_collecter = ui_radar_manager->radar_point_collecter();
            uiRadarClickAnimation& click_animation = ui_radar_manager->radar_click_animation();

            RADAR_POINT_DEQUE& radar_point_container = point_collecter.radar_point_container();

            for( RADAR_POINT_DEQUE::const_iterator it(radar_point_container.begin());
                it != radar_point_container.end(); ++it )
            {
                RADAR_POINT* point = (*it);

                // 히어로는 패스
                if( point->dwObjectKey == g_pHero->GetObjectKey() )
                    continue;


                eRADAR_POINT_COLOR eRadarPointColor = point->eRadarPointColor;

                if(GetRadarState() == eFULL_RADAR_STATE_ALPHA)
                {
                    if ((eRadarPointColor == eRADAR_MONSTER_POINT) || 
                        (eRadarPointColor == eRADAR_MONSTER_DEAD_POINT) || 
                        (eRadarPointColor == eRADAR_BOSS_MONSTER_POINT) || 
                        (eRadarPointColor == eRADAR_BOSS_MONSTER_DEAD_POINT))
                    {
                        continue;
                    }
                }

                if(eRADAR_NOT_FOUND_OBJECT == eRadarPointColor)
                    continue;


                if( RECT* pRect = RadarInfoParser::Instance()->GetRadarImgInfo( eRadarPointColor ) )
                {
                    float fPointSizeX = ((float)pRect->right * 0.5f);
                    float fPointSizeY	= ((float)pRect->bottom * 0.5f);
                    float fIconCenterX = fImageCtrlCenterX + (point->fPosX*scaleX) - fPointSizeX - hero_position_x;
                    float fIconCenterY = fImageCtrlCenterY + (point->fPosY*scaleY) - fPointSizeY - hero_position_y;


                    int iMouseX = Mouse::Instance()->GetMouseX();
                    int iMouseY = Mouse::Instance()->GetMouseY();

                    if( iMouseX > (fIconCenterX) && iMouseX < (fIconCenterX + (float)pRect->right) && 
                        iMouseY > (fIconCenterY) && iMouseY < (fIconCenterY + (float)pRect->bottom) )
                    {

                        if( Object* clicked_object = g_ObjectManager.GetObject( point->dwObjectKey ) )
                        {
                            return PathFindMoveToObject( clicked_object );
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool BaseRadarDialog::PathFindMoveToObject( Object* object )
{
    if (object->IsKindOfObject(MAP_OBJECT) == TRUE)
    {
        if( g_HeroInput.GetAutoRun() )
        {
            g_HeroInput.SetAutoRun(FALSE);
            g_HeroInput.SetFollowState( FALSE );

            g_pHero->SetNextState(STATE::IDLE,g_CurTime);
            g_pHero->CommitChangeState( g_CurTime );
        }

        g_HeroInput.MoveToMapObjectWithAction(static_cast<MapObject*>(object));
        g_HeroInput.ProcessQueueAction();

        // 자동이동을 시작합니다.
        LPCTSTR msg = g_InterfaceManager.GetInterfaceString(11480);
        GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, msg );
        WzColor msg_color = WzColor_RGBA(129,254,82,255);
        g_InterfaceManager.ShowCenterMessageBox(msg, 5000, &msg_color, FALSE);

        return true;
    }
    else if (object->IsKindOfObject(CHARACTER_OBJECT) == TRUE)
    {
        if( g_HeroInput.GetAutoRun() )
        {
            g_HeroInput.SetAutoRun(FALSE);
            g_HeroInput.SetFollowState( FALSE );

            g_pHero->SetNextState(STATE::IDLE,g_CurTime);
            g_pHero->CommitChangeState( g_CurTime );
        }

        Character* character = static_cast<Character*>(object);
        RELATIONKIND::TYPE relation_type = GameFunc::GetRelationKindOfHero(character);
        g_HeroInput.OnLClickedCharacter(character, relation_type, true);
        g_HeroInput.ProcessQueueAction();

        // 자동이동을 시작합니다.
        LPCTSTR msg = g_InterfaceManager.GetInterfaceString(11480);
        GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, msg );
        WzColor msg_color = WzColor_RGBA(129,254,82,255);
        g_InterfaceManager.ShowCenterMessageBox(msg, 5000, &msg_color, FALSE);

        return true;
    }

    return false;

}
#endif//_NA_0_20111013_RADAR_PATHFINDING

BOOL BaseRadarDialog::Wrap_CheckClickedPosition(CCtrlPictureWZ * pCtrlPicture)
{
    if (pCtrlPicture == NULL)
    {
        return FALSE;
    }

    uiRadarMan* ui_radar_manager = static_cast<uiRadarMan*>(GetMan());
    if (ui_radar_manager == NULL)
    {
        return FALSE;
    }

    //	동그라미 크기를 구해서 좌표를 계산하자
    RECT* pRect = NULL;
    pRect = RadarInfoParser::Instance()->GetRadarImgInfo( eRADAR_MONSTER_DEAD_POINT);
    if(pRect == NULL)
    {
        return FALSE;
    }

    uiRadarMapData& map_data = ui_radar_manager->radar_map_data();

    float hero_position_x;
    float hero_position_y;
    float scaleX = 1.0f;
    float scaleY = 1.0f;

    if (is_full_map() == true)
    {
#ifdef _NA_000000_20130422_SCALE_FULLMAP
        if(m_bResizefullmap)
        {
            scaleX = 0.5f;
            scaleY = 0.5f;
        }
#endif

        hero_position_x = -((map_data.map_image_width()*scaleX) / 2.0f - (GetRadarImageWidth()*scaleX));
        hero_position_y = -((map_data.map_image_height()*scaleY) / 2.0f - (GetRadarImageHeight()*scaleY));
    }
    else
    {
        hero_position_x = map_data.transform_position().x;
        hero_position_y = map_data.transform_position().y;
    }

	POINT pt = g_pApplication->Get2DModeMapping();
	RECT rect = pCtrlPicture->GetSizeRect();
	//---------------------------------------------------------------------------------------------
	POINT* pScreenSize = g_pApplication->GetScreenSize();
	float fWidth = (float)pScreenSize->x / pt.x;
	float fHeight = (float)pScreenSize->y / pt.y;

	float fMouseX = (float)Mouse::Instance()->GetMouseX();
	float fMouseY = (float)Mouse::Instance()->GetMouseY();

	float fx = rect.left + (rect.right - rect.left) * 0.5f;
	float fy = rect.top + (rect.bottom - rect.top) * 0.5f;

	float fPointSizeX = (float)pRect->right * 0.5f;
	float fPointSizeY = (float)pRect->bottom * 0.5f;

	if( fMouseX > (rect.left ) && fMouseX < (rect.right ) && 
		fMouseY > (rect.top ) && fMouseY < (rect.bottom ) )
	{
		//Todo:마우스 좌표를 맵좌표로 변경하자.

		RECT rect = pCtrlPicture->GetSizeRect();

		//	주인공 위치( 점 ) 계산부터 하고 나중에 찍는다
		fMouseX-=fx;
		fMouseY-=fy;

        fMouseX += hero_position_x;
        fMouseY += hero_position_y;

        float fRealX = fMouseX;
        float fRealY = fMouseY;

		if( GENERALPARAM->IsNet()==FALSE)// 싱글 모드 일경우 
		{
            uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
            if (ui_radar_manager != NULL)
            {
                ui_radar_manager->SetClickPosition(fRealX, fRealY, 5000);
            }	
		}
		else
		{
			MSG_CG_PARTY_MINIMAP_POINTING_SYN Packet;
			Packet.m_byCategory = CG_PARTY;
			Packet.m_byProtocol = CG_PARTY_MINIMAP_POINTING_SYN;
			Packet.m_fx=fRealX;
			Packet.m_fy=fRealY;

			if(g_pHero)
				g_pHero->SendPacket(&Packet,sizeof(Packet));
		}
	}
	return TRUE;
}

void BaseRadarDialog::NetworkProc(MSG_BASE * pMsg )
{

}

VOID BaseRadarDialog::Process(DWORD dwTick)
{
}

void BaseRadarDialog::SetMapName(CCtrlStaticWZ* pControl, TCHAR* pszMapName)
{
    if (!pControl || !pszMapName)
    {
        return;
    }

    //
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    if (GlobalFunc::IsFreePVPScene() == TRUE)
    {
        //ChaosSystem::GetFreePVPData().my_room_info()->base_free_pvp_room_info.room_title;
        int room_number = ChaosSystem::GetFreePVPData().my_room_info()->room_key - free_pvp_base_room_key; // n번 방
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR number_string[INTERFACE_STRING_LENGTH] = {0,};
        if (room_number > 0)
        {
            _itot(room_number, number_string, 10);
        }
        else
        {
            Snprintf(number_string, INTERFACE_STRING_LENGTH, "?");
        }
        g_InterfaceManager.GetInterfaceString(2613, temp_string, INTERFACE_STRING_LENGTH);
        Snprintf(pszMapName, INTERFACE_STRING_LENGTH, temp_string, number_string);
    }
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    //
	// 컨트롤의 너비 구함(텍스트가 출력될 수 있을 만한 너비)
	// 스트링의 길이와 비교하여 전부 출력 가능한지 체크
	// 전부 출력 가능하면 그대로 세팅(tooltip off)
	// 컨트롤의 너비가 부족하면 줄임 처리 뒤 세팅(tooltip on)
	RECT rtSize = pControl->GetSize();
	int nControlWidth = rtSize.right - 8;
	int nTextWidth = GlobalFunc::GetTextWidth(pszMapName, pControl->GetFontID());

	if (nTextWidth > nControlWidth)
	{
		std::vector<util::_tstring> vstr;
		int nWidth = GlobalFunc::GetTextWidth(_T("..."), pControl->GetFontID());

		GlobalFunc::ProcessWordWrapByFontSize( &vstr, pszMapName, pControl->GetFontID(), _tcslen(pszMapName) , nControlWidth-nWidth );
		vstr[0]+=_T("...");
		GlobalFunc::SetCaption( pControl, const_cast<TCHAR *>(vstr[0].c_str()) );

		m_bShowTooltipForMapName = true;
	}
	else
	{		
		GlobalFunc::SetCaption( pControl, pszMapName );
		m_bShowTooltipForMapName = false;
	}
}

void BaseRadarDialog::ShowToolTipMapName(CCtrlStaticWZ* pControl, TCHAR* pszMapName)
{
	uiToolTipMan* pUIMan = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
	if (pUIMan)
	{
		int iMouseX = Mouse::Instance()->GetMouseX();
		int iMouseY = Mouse::Instance()->GetMouseY();
		
		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX
			&& rc.top <= iMouseY && rc.bottom >= iMouseY)
		{
			pUIMan->RegisterTooltipMapName(pControl, pszMapName);
			pUIMan->SetTooltipDialogID(GetDialogKey());
		}			
	}	
}

void BaseRadarDialog::DrawControlImage(CControlWZ* control_ptr, void* parameter)
{
    if ((control_ptr == NULL) || 
        (parameter == NULL) || 
        (g_pSunRenderer == NULL))
    {
        return;
    }

    SI_TFORMAT* image_format = control_ptr->GetCtrlImageFormat();
    if (image_format->m_sImageState[E_BASE].m_blIs == false)
    {
        return;
    }

    RECT control_rect = control_ptr->GetSizeRect();
    HANDLE texture_handle = reinterpret_cast<HANDLE>(image_format->m_sImageState[E_BASE].m_pVoid);
    ENUM_ALPHABLEND old_alpha_blend_mode = g_pSunRenderer->GetAlphaBlendMode();
    BOOL old_ztest = g_pSunRenderer->IsEnableZTest();

    g_pSunRenderer->SetZBufferTest(FALSE);
    g_pSunRenderer->SetAlphaBlendMode(AB_NONE);

    float draw_width = image_format->m_sImageState[E_BASE].m_sFRect.fWidth;
    float draw_height = image_format->m_sImageState[E_BASE].m_sFRect.fHeight;
    float src_x = image_format->m_sImageState[E_BASE].m_sFRect.fTopX;
    float src_y = image_format->m_sImageState[E_BASE].m_sFRect.fTopY;

#ifdef _NA_000000_20130422_SCALE_FULLMAP
    BaseRadarDialog* pdialog = (BaseRadarDialog*)parameter;
    float isdraw = pdialog->GetShowResize() ? 0.5f :  1.0f;

    g_pSunRenderer->RenderTexture(texture_handle, 
        static_cast<float>(control_rect.left), 
        static_cast<float>(control_rect.top), 
        draw_width * isdraw,
        draw_height * isdraw, 
        src_x, 
        src_y, 
        draw_width, 
        draw_height);
#else
	g_pSunRenderer->RenderTexture(texture_handle, 
        static_cast<float>(control_rect.left), 
        static_cast<float>(control_rect.top), 
        draw_width, 
        draw_height, 
        src_x, 
        src_y, 
        draw_width, 
        draw_height);
#endif

    g_pSunRenderer->SetAlphaBlendMode(old_alpha_blend_mode);
    g_pSunRenderer->SetZBufferTest(old_ztest);
}

WzVector BaseRadarDialog::CalculatePointPosition(float x,float y, float z)
{
    WzVector Position;
    Position.x = x;
    Position.y = y;
    Position.z = 0.0;

    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager == NULL)
    {
        return Position;
    }

    uiRadarMapData& radar_map_data = ui_radar_manager->radar_map_data();

    D3DXVECTOR4 transfirm_position(Position.x, Position.z, Position.y , 1.0f);
    WzMatrix& transfirm_matrix = radar_map_data.transform_matrix();
    D3DXVec4Transform(&transfirm_position, &transfirm_position, (D3DXMATRIX*)(&transfirm_matrix));

    Position.x = (((transfirm_position.x * 0.5f) + 0.5f) * radar_map_data.map_image_width());
    Position.z = (((transfirm_position.y * (-0.5f)) + 0.5f) * radar_map_data.map_image_height());

    return Position;
}
