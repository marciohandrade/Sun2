#include "SunClientPrecompiledHeader.h"
#include "gamefunc.h"
#include "Application.h"
#include "RadarInfoParser.h"
#include "radarplusdialog.h"
#include "ResourceManager.h"
#include "map.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "Map.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "NPC.h"
#include "Mouse.h"
#include "SoundEffect.h"
#include "radardialog.h"
#include "HeadMeshInfoParser.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiHelpMan/uiHelpMan.h"
#include "globalfunc.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "HeroActionInput.h"
#include "uiRadarMan/uiRadarMan.h"
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "TiredSystemDialog.h"
#include "Interface/uiTiredSystemMan/uiTiredSystemMan.h"

#ifdef _NA_000000_20130114_RENEWER_UI
	#include "EnduranceDialog.h"
#endif

WzID RadarPlusDialog::m_wzId[RADAR_PLUS_MAX] = 
{ 
	StrToWzID("PI00"),//RADAR_PLUS_MAP_IMAGE,
	StrToWzID("T001"),//RADAR_PLUS_MAP_NAME,
	StrToWzID("T002"),//RADAR_PLUS_MAP_POS,
	StrToWzID("C001"),//RADAR_PLUS_MINUS,
	StrToWzID("B000"),//RADAR_PLUS_EXIT,
	StrToWzID("PI01"),//RADAR_PLUS_PIC_LAYER_BACKSIZE,
	StrToWzID("PI02"),//RADAR_PLUS_PIC_LAYER_SIZE,
	StrToWzID("PI03"),//RADAR_PLUS_PIC_LAYER_COUNT,
	StrToWzID("ST00"),//RADAR_PLUS_TXT_LAYER_COUNT,

#ifdef _NA_000000_20130114_RENEWER_UI
	StrToWzID("B001"),
	StrToWzID("P002"),
	StrToWzID("P000"),
	StrToWzID("PRAD"),
	StrToWzID("P001"),

#endif

};	


RadarPlusDialog::RadarPlusDialog(InterfaceManager *pUIMan):
	BaseRadarDialog(pUIMan)
		,show_hide_check_(false)
{
}

RadarPlusDialog::~RadarPlusDialog(void)
{
}


VOID RadarPlusDialog::Release()
{
}

void RadarPlusDialog::UpdatePosition()
{
	// window size
	RECT* rcWindow = g_InterfaceManager.GetWindowRC();

	long wWidth, wHeight;

	POINT pt = g_pApplication->Get2DModeMapping();

	wWidth = pt.x;
	wHeight = pt.y;
	
	// dialog size
	RECT rcDialog;
	GetDialogWZ()->GetDlgSize(&rcDialog);

	long dWidth, dHeight;
	dWidth = rcDialog.right;
	dHeight = rcDialog.bottom;

	// 상대적인 레이더 위치설정
	MoveWindowWZ( (float)(wWidth - dWidth) + 2.0f, 0.0f );
}

VOID RadarPlusDialog::Init( CDrawBase * pDrawBase )
{
	BaseRadarDialog::Init(pDrawBase);

	UpdatePosition();

	m_iCurLayer		= 0;
	m_fLayerSize	= 0.0f;


    /// 레이더 컨트롤의 크기
    CControlWZ* pCtrl = getControl(RADAR_PLUS_MAP_IMG);
    if (pCtrl != NULL)
    {
        m_wRadarImageWidth	= (WORD)pCtrl->GetSize().right;
        m_wRadarImageHeight = (WORD)pCtrl->GetSize().bottom;
        pCtrl->SetCallbackDraw(DrawControlImage, this);
    }
    else
    {
        m_wRadarImageWidth = 1;
        m_wRadarImageHeight = 1;
    }

	InitControls();

	m_btRadarState = eMINI_MAP;
}




VOID RadarPlusDialog::updateMap()
{
    uiRadarMan* ui_radar_manager = static_cast<uiRadarMan*>(GetMan());
    if (ui_radar_manager == NULL)
    {
        return;
    }
    uiRadarMapData& map_data = ui_radar_manager->radar_map_data();

	CCtrlPictureWZ * pPicture = (CCtrlPictureWZ *)getControl( RADAR_PLUS_MAP_IMG );
	SI_TFORMAT * pFormat = pPicture->GetCtrlImageFormat();
	pFormat->m_sImageState[E_BASE].m_sFRect.fTopX = map_data.transform_position().x - ( (m_wRadarImageWidth>>1) );
	pFormat->m_sImageState[E_BASE].m_sFRect.fTopY = map_data.transform_position().y - ( (m_wRadarImageHeight>>1) );

    if (g_pHero != NULL)
    {
        // 현재 좌표 출력
        TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
        WzVector hero_position = g_pHero->GetVisiblePos();
#ifdef	_K2P_HERO_POSITION
        Snprintf(szMessage, MAX_PATH-1, _T("%d | %d"), 
            static_cast<int>(hero_position.x), 
            static_cast<int>(hero_position.y));
#else
        Snprintf(szMessage, MAX_PATH-1, _T("%4.2f | %4.2f"), hero_position.x, hero_position.y);
#endif

        CCtrlStaticWZ* pStaticPos = static_cast<CCtrlStaticWZ*>(getControl(RADAR_PLUS_MAP_POS));
        if (pStaticPos != NULL)
        {
            GlobalFunc::SetCaption(pStaticPos, szMessage);
        }
    }    
}


VOID RadarPlusDialog::OnUpdateSolarDialog()
{
	//if( m_bChangedXY )
	{
		updateMap();
	}
}


VOID RadarPlusDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
#ifdef _NA_0_20111013_RADAR_PATHFINDING
    case RT_MSG_RBUTTONCLICK:
        CheckClickedPosition();
        break;
#endif //_NA_0_20111013_RADAR_PATHFINDING

	case RT_MSG_LBUTTONCLICK:
		{
#ifdef _NA_0_20111013_RADAR_PATHFINDING
            if( PathFindMove( getControl( RADAR_PLUS_MAP_IMG ) ) == true )
                break;
#else 
            CheckClickedPosition();
#endif //_NA_0_20111013_RADAR_PATHFINDING

			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case RADAR_PLUS_MINUS:
				{
					// 확대된 레이더창을 열어라.
					ShowWindow(FALSE);

					RadarDialog* radar_dialog = 
						(RadarDialog*)GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG);

					if(radar_dialog)
					{
						radar_dialog->ShowWindow(true);
						radar_dialog->HideAllControl(!show_hide_check_);
					}

                    // 배틀존 아이콘 위치 조정
                    uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
                    if (ui_battlezone2_manager_ != NULL)
                    {
                        ui_battlezone2_manager_->MoveBattleZoneIcon();
                    }
				}
				break;
			case RADAR_MAP_B001:
				{
					HideAllControl(true);
				}
				break;
			case RADAR_PLUS_EXIT:
				{
					HideAllControl(FALSE);
					//ShowDialog(FALSE);

                    // 배틀존 아이콘 위치 조정
                    uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
                    if (ui_battlezone2_manager_ != NULL)
                    {
                        ui_battlezone2_manager_->MoveBattleZoneIcon();
                    }
				}
				break;
			}
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}

	EnduranceDIalog* pEnduranceDlg = static_cast<EnduranceDIalog *>
		(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ENDURANCE));

	if(pEnduranceDlg)
	{
		SendMesaageUI(pEnduranceDlg,WM_USER +2,0,0);
	}
}

VOID RadarPlusDialog::NetworkProc( MSG_BASE * pMsg )
{
}

POSTYPE RadarPlusDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < RADAR_PLUS_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return RADAR_PLUS_MAX;
}

CControlWZ * RadarPlusDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < RADAR_PLUS_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

BOOL RadarPlusDialog::IsVisibleWindow()
{
	return GetDialogWZ()->IsState( WZ_SHOW );
}


void RadarPlusDialog::InitControls()
{
	m_pPicCtrlLayer = static_cast<CCtrlPictureWZ *>(getControl(RADAR_PLUS_PIC_LAYER_COUNT));
	assert(m_pPicCtrlLayer);

	m_pTxtCtrlLayer = static_cast<CCtrlStaticWZ *>(getControl(RADAR_PLUS_TXT_LAYER_COUNT));
	assert(m_pTxtCtrlLayer);

	m_pPicCtrlLayerSize = static_cast<CCtrlPictureWZ *>(getControl(RADAR_PLUS_PIC_LAYER_SIZE));
	assert(m_pPicCtrlLayerSize);

	m_pPicCtrlLayerBackSize = static_cast<CCtrlPictureWZ *>(getControl(RADAR_PLUS_PIC_LAYER_BACKSIZE));
	assert(m_pPicCtrlLayerBackSize);

#ifdef _NA_000000_20130114_RENEWER_UI
	show_on_radar_iamge2_ = static_cast<CCtrlPictureWZ *>(getControl(RADAR_MAP_P002));
	show_on_radar_iamge3_ = static_cast<CCtrlPictureWZ *>(getControl(RADAR_MAP_P003));
	show_on_radar_button_ = static_cast<CCtrlButtonWZ *>(getControl(RADAR_MAP_B001));

	if (show_on_radar_iamge2_)
	{
		show_on_radar_iamge2_->ShowWindowWZ(WZ_HIDE);
	}
	if (show_on_radar_iamge3_)
	{
		show_on_radar_iamge3_->ShowWindowWZ(WZ_HIDE);
	}
	if (show_on_radar_button_)
	{
		show_on_radar_button_->ShowWindowWZ(WZ_HIDE);
	}
#endif
}


VOID RadarPlusDialog::OnShowWindow( BOOL val )
{
	if( val == TRUE )
	{
		InitLayerShowNHide();
	}

	ENUM_STATEWZ v = ( val == TRUE ? WZ_SHOW : WZ_HIDE );

	GetDialogWZ()->ShowWindowWZ( v );

	if(val)	
	{
		uiHelpMan* helpMan =
					static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

		if(helpMan)
		{
			helpMan->SetPosition();
		}
	}
}



BOOL RadarPlusDialog::InterSectRect( POINT point )
{
	RECT rc;

	GetDialogWZ()->GetClientRect(&rc);

	if(DEF_IntersectRect(rc,point))
	{
		return TRUE;	
	}
	return FALSE;
}


void RadarPlusDialog::ChangeMap(TCHAR* file_name /*= NULL*/)
{
	CControlWZ* pCtrl = getControl(RADAR_PLUS_MAP_IMG);
    if (pCtrl != NULL)
    {
        DWORD FieldID = g_pMap->GetCurMMCode();
        if (FieldID != 0)
        {
            Wrap_loadMapImage(pCtrl , FieldID, 0, file_name);
        }        
    }
	
    CCtrlStaticWZ* pStatic = static_cast<CCtrlStaticWZ*>(getControl(RADAR_PLUS_MAP_NAME));
    SetMapName(pStatic, g_pMap->GetFieldName());

    CCtrlPictureWZ* pPicture = static_cast<CCtrlPictureWZ*>(getControl(RADAR_PLUS_MAP_IMG));
    CalcPortalOffset(pPicture);
}

void RadarPlusDialog::OnRenderSolarDialog()
{
	if(show_hide_check_)
	{
		return;
	}

	if(!g_pHero)
		return;
/*
	if( g_pHero->IsShowFullMap() )
	{
		return;
	}
*/
	CCtrlPictureWZ * pPicture = (CCtrlPictureWZ *)getControl( RADAR_PLUS_MAP_IMG );
	if ( pPicture)
	{
		Wrap_render(pPicture);
	}
	if (IsShowToolTipForMapName())
	{
		CCtrlStaticWZ * pStatic = (CCtrlStaticWZ *)getControl( RADAR_PLUS_MAP_NAME );
		ShowToolTipMapName(pStatic, g_pMap->GetFieldName());		
	}
}

BOOL RadarPlusDialog::CheckClickedPosition()
{
	CCtrlPictureWZ * pPicture = (CCtrlPictureWZ *)getControl( RADAR_PLUS_MAP_IMG );
	BOOL ret = BaseRadarDialog::Wrap_CheckClickedPosition(pPicture);

	return ret;
}

void RadarPlusDialog::SetMaxLayer( int iMaxLayer )
{
	if( iMaxLayer <= 1)
	{
		m_fLayerSize = 0.0f;
	}
	else
	{
		m_fLayerSize = ((float)( m_pPicCtrlLayerSize->GetSize().bottom - m_pPicCtrlLayer->GetSize().bottom )
			/ (float)(iMaxLayer - 1));
	}
}

void RadarPlusDialog::SetCurLayer( int iCurLayer )
{
	if(m_iCurLayer == iCurLayer || iCurLayer < 1)
		return;

	m_iCurLayer = iCurLayer;

	if(m_pPicCtrlLayerSize && m_pPicCtrlLayer && m_pTxtCtrlLayer)
	{
		float	fMoveHeightPos;
		RECT	rtLayer, rtTxt;
		TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

		rtLayer		= m_pPicCtrlLayer->GetSizeRect();
		float fLayerStartPos = (float)(m_pPicCtrlLayerSize->GetSizeRect().bottom - m_pPicCtrlLayer->GetSize().bottom);
		fMoveHeightPos = fLayerStartPos - ( m_fLayerSize * (float)(m_iCurLayer - 1));
		m_pPicCtrlLayer->MoveWindowWZ( (float)rtLayer.left , fMoveHeightPos,
			(float)(rtLayer.right - rtLayer.left), (float)(rtLayer.bottom - rtLayer.top));

		rtTxt = m_pTxtCtrlLayer->GetSizeRect();
#ifdef _JAPAN
        m_pTxtCtrlLayer->MoveWindowWZ( (float)rtTxt.left , (float)(fMoveHeightPos + 7.5f), 
            (float)(rtTxt.right - rtTxt.left), (float)(rtTxt.bottom - rtTxt.top));
#else
		m_pTxtCtrlLayer->MoveWindowWZ( (float)rtTxt.left , (float)(fMoveHeightPos + 5.5f), 
			(float)(rtTxt.right - rtTxt.left), (float)(rtTxt.bottom - rtTxt.top));
#endif //_JAPAN

		Sprintf(szMessage, _T("%d"), m_iCurLayer);
		GlobalFunc::SetCaption(m_pTxtCtrlLayer, szMessage);
	}
}

void RadarPlusDialog::InitLayerShowNHide( void )
{
#ifdef _NA_007477_20140805_RADAR_RAYER_MODIFY
    if (m_fLayerSize > 0.0f)
#else
    if(GameFramework::GetCurrentScene() && GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_INSTANCE_DUNGEON  && m_fLayerSize > 0.0f)
#endif //_NA_007477_20140805_RADAR_RAYER_MODIFY
	{
		if(m_pPicCtrlLayerBackSize)
			m_pPicCtrlLayerBackSize->ShowWindowWZ(WZ_SHOW);
		if(m_pPicCtrlLayerSize)
			m_pPicCtrlLayerSize->ShowWindowWZ(WZ_SHOW);
		if(m_pPicCtrlLayer)
			m_pPicCtrlLayer->ShowWindowWZ(WZ_SHOW);
		if(m_pTxtCtrlLayer)
			m_pTxtCtrlLayer->ShowWindowWZ(WZ_SHOW);
	}
	else
	{
		if(m_pPicCtrlLayerBackSize)
			m_pPicCtrlLayerBackSize->ShowWindowWZ(WZ_HIDE);
		if(m_pPicCtrlLayerSize)
			m_pPicCtrlLayerSize->ShowWindowWZ(WZ_HIDE);
		if(m_pPicCtrlLayer)
			m_pPicCtrlLayer->ShowWindowWZ(WZ_HIDE);
		if(m_pTxtCtrlLayer)
			m_pTxtCtrlLayer->ShowWindowWZ(WZ_HIDE);
	}
}

void RadarPlusDialog::HideAllControl( bool value )
{
	show_hide_check_ = !value;

	for (int i = RADAR_PLUS_MAP_IMG ; i < RADAR_PLUS_MAX ; ++i)
	{
		CControlWZ* control = getControl(i);

		if(control)
		{
			control->ShowWindowWZ(value ? WZ_SHOW : WZ_HIDE);
		}
	}

	if(value)
	{
		InitLayerShowNHide();
	}

	if (show_on_radar_iamge2_)
	{
		show_on_radar_iamge2_->ShowWindowWZ(value ? WZ_HIDE : WZ_SHOW);
	}
	if (show_on_radar_iamge3_)
	{
		show_on_radar_iamge3_->ShowWindowWZ(WZ_SHOW);
	}
	if (show_on_radar_button_)
	{
		show_on_radar_button_->ShowWindowWZ(value ? WZ_HIDE : WZ_SHOW);
	}

	CControlWZ* control = getControl(RADAR_PLUS_MAP_NAME);
	if(control)
	{
		control->ShowWindowWZ(WZ_SHOW);
	}

	control = getControl(RADAR_PLUS_MAP_POS);
	if(control)
	{
		control->ShowWindowWZ(WZ_SHOW);
	}

	control = getControl(RADAR_PLUS_MINUS);
	if(control)
	{
		control->ShowWindowWZ(WZ_SHOW);
	}

	control = getControl(value ? RADAR_MAP_BACK : RADAR_MAP_P002);

	TiredSystemDialog* pTiredDlg = GET_CAST_DIALOG(TiredSystemDialog, IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM);

	if(pTiredDlg && control)
	{
		pTiredDlg->MoveuiForce(0,control->GetSizeRect().bottom);
	}

	//DisablePosInWindow( value );
}

void RadarPlusDialog::DisablePosInWindow( bool value )
{
	if(value == false)
	{
		CControlWZ* control = getControl(RADAR_MAP_P003);
		if(control)
		{
			DisableInterSectRect(control->GetSizeRect());
			control->SetDiscardMessage(true);
		}

		control = getControl(RADAR_MAP_BACK);
		if(control)
		{
			control->SetDiscardMessage(true);
		}

		control = getControl(RADAR_MAP_BACK2);
		if(control)
		{
			control->SetDiscardMessage(true);
		}

		control = getControl(RADAR_PLUS_MAP_IMG);
		if(control)
		{
			control->SetDiscardMessage(true);
		}
	}
	else
	{
		RECT rect = {0,0,0,0};
		//의미없는좌표로
		DisableInterSectRect(rect);

		CControlWZ* control = getControl(RADAR_MAP_P003);
		if(control)
		{
			control->SetDiscardMessage(false);
		}

		control = getControl(RADAR_MAP_BACK);
		if(control)
		{
			control->SetDiscardMessage(false);
		}

		control = getControl(RADAR_MAP_BACK2);
		if(control)
		{
			control->SetDiscardMessage(false);
		}

		control = getControl(RADAR_PLUS_MAP_IMG);
		if(control)
		{
			control->SetDiscardMessage(false);
		}
	}
}