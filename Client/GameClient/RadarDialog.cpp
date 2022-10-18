#include "SunClientPrecompiledHeader.h"
#include "gamefunc.h"
#include "Application.h"
#include "RadarInfoParser.h"
#include "radardialog.h"
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
#include "radarplusdialog.h"
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

WzID RadarDialog::m_wzId[RADAR_MAX] = 
{ 
	StrToWzID("PI00"),//RADAR_MAP_IMAGE,
	StrToWzID("T001"),//RADAR_MAP_NAME,
	StrToWzID("C001"),//RADAR_PLUS,
	StrToWzID("B000"),//RADAR_EXIT,
	StrToWzID("PI01"),//RADAR_PIC_LAYER_BACKSIZE,
	StrToWzID("PI02"),//RADAR_PIC_LAYER_SIZE,
	StrToWzID("PI03"),//RADAR_PIC_LAYER_COUNT,
	StrToWzID("ST00"),//RADAR_TXT_LAYER_COUNT,

#ifdef _NA_000000_20130114_RENEWER_UI
	StrToWzID("B001"),
	StrToWzID("P002"),
	StrToWzID("P003"),
	StrToWzID("PRAD"),
	StrToWzID("P000"),
#endif

};


RadarDialog::RadarDialog(InterfaceManager *pUIMan):
	BaseRadarDialog(pUIMan)
	,show_hide_check_(false)
{

}


RadarDialog::~RadarDialog()
{
	_asm nop;
}

VOID RadarDialog::Release()
{
}

VOID RadarDialog::Init( CDrawBase * pDrawBase )
{
	BaseRadarDialog::Init(pDrawBase);

	/**
	사이즈 가변형에 적합하도록 레이더 위치 우측으로 정렬
	*/

	// window size
	RECT* rcWindow = g_InterfaceManager.GetWindowRC();

	POINT	*pScreenSize = g_pApplication->GetScreenSize();

	POINT pt = g_pApplication->Get2DModeMapping();

	// dialog size
	RECT rcDialog;
	GetDialogWZ()->GetDlgSize(&rcDialog);

	long dWidth, dHeight;
	dWidth = rcDialog.right;
	dHeight = rcDialog.bottom;

	// 상대적인 레이더 위치설정
	MoveWindowWZ( (float)(pt.x - dWidth) + 2.0f, -4.0f );

	m_iCurLayer		= 0;
	m_fLayerSize	= 0.0f;

	/// 레이더 컨트롤의 크기
	CControlWZ* pCtrl = getControl(RADAR_MAP_IMG);
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

void RadarDialog::InitControls()
{
	m_pPicCtrlLayer = static_cast<CCtrlPictureWZ *>(getControl(RADAR_PIC_LAYER_COUNT));
	assert(m_pPicCtrlLayer);

	m_pTxtCtrlLayer = static_cast<CCtrlStaticWZ *>(getControl(RADAR_TXT_LAYER_COUNT));
	assert(m_pTxtCtrlLayer);

	m_pPicCtrlLayerSize = static_cast<CCtrlPictureWZ *>(getControl(RADAR_PIC_LAYER_SIZE));
	assert(m_pPicCtrlLayerSize);

	m_pPicCtrlLayerBackSize = static_cast<CCtrlPictureWZ *>(getControl(RADAR_PIC_LAYER_BACKSIZE));
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


VOID RadarDialog::updateMap()
{
    uiRadarMan* ui_radar_manager = static_cast<uiRadarMan*>(GetMan());
    if (ui_radar_manager == NULL)
    {
        return;
    }
    uiRadarMapData& map_data = ui_radar_manager->radar_map_data();
	CCtrlPictureWZ * pPicture = (CCtrlPictureWZ *)getControl( RADAR_MAP_IMG );
	SI_TFORMAT * pFormat = pPicture->GetCtrlImageFormat();
	pFormat->m_sImageState[E_BASE].m_sFRect.fTopX = map_data.transform_position().x - ( (m_wRadarImageWidth>>1) );
	pFormat->m_sImageState[E_BASE].m_sFRect.fTopY = map_data.transform_position().y - ( (m_wRadarImageHeight>>1) );
}




VOID RadarDialog::OnUpdateSolarDialog()
{
	updateMap();
}


VOID RadarDialog::MessageProc( SI_MESSAGE * pMessage )
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
            if( PathFindMove( getControl( RADAR_MAP_IMG ) ) == true )
                break;
#else 
            CheckClickedPosition();
#endif //_NA_0_20111013_RADAR_PATHFINDING

			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case RADAR_PLUS:
				{
					// 확대된 레이더창을 열어라.
					ShowWindow(FALSE);

					//					g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_RADAR_PLUS, TRUE);
					RadarPlusDialog* radar_plus = 
						(RadarPlusDialog*)GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
					
					if(radar_plus)
					{
						radar_plus->ShowWindow(true);
						radar_plus->HideAllControl(!show_hide_check_);
					}

					g_SoundEffect.PlayUI( UI_SOUND_MAP_ZOOM_IN );

                    // 배틀존 아이콘 위치 조정
                    uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
                    if (ui_battlezone2_manager_ != NULL)
                    {
                        ui_battlezone2_manager_->MoveBattleZoneIcon();
                    }
				}
				break;

			case RADAR_MAP_IMG:
				{
				}
				break;
			case RADAR_MAP_B001:
				{
					HideAllControl(true);
				}
				break;
			case RADAR_EXIT:
				{
					HideAllControl(FALSE);

#ifdef _NA_000000_20130114_RENEWER_UI
                    // 배틀존 아이콘 위치 조정
                    uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
                    if (ui_battlezone2_manager_ != NULL)
                    {
                        ui_battlezone2_manager_->MoveBattleZoneIcon();
                    }
#endif
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




VOID RadarDialog::NetworkProc( MSG_BASE * pMsg )
{
}

POSTYPE RadarDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < RADAR_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return RADAR_MAX;
}

CControlWZ * RadarDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < RADAR_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

BOOL RadarDialog::IsVisibleWindow()
{
	return GetDialogWZ()->IsState( WZ_SHOW );
}

VOID RadarDialog::OnShowWindow( BOOL val )
{
	if( val == TRUE )
	{
		InitLayerShowNHide();

		CControlWZ* control;
		control = getControl( RADAR_MAP_BACK );

		TiredSystemDialog* pTiredDlg = GET_CAST_DIALOG(TiredSystemDialog, IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM);

		if(pTiredDlg && control)
		{
			pTiredDlg->MoveuiForce(0,control->GetSizeRect().bottom);
		}
	}

	ENUM_STATEWZ v = ( val == TRUE ? WZ_SHOW: WZ_HIDE );

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
BOOL RadarDialog::InterSectRect( POINT point )
{
	RECT rc;

	GetDialogWZ()->GetClientRect(&rc);

	if(DEF_IntersectRect(rc,point))
	{
		return TRUE;	
	}
	return FALSE;
}

void RadarDialog::ChangeMap(TCHAR* file_name /*= NULL*/)
{
	CControlWZ* pCtrl = getControl(RADAR_MAP_IMG);
	if (pCtrl != NULL)
	{
		DWORD FieldID = g_pMap->GetCurMMCode();
        if (FieldID != 0)
        {
            Wrap_loadMapImage(pCtrl , FieldID, 0, file_name);
        }
	}

	CCtrlStaticWZ* pStatic = static_cast<CCtrlStaticWZ*>(getControl(RADAR_MAP_NAME));
	SetMapName(pStatic, g_pMap->GetFieldName());

	CCtrlPictureWZ* pPicture = static_cast<CCtrlPictureWZ*>(getControl(RADAR_MAP_IMG));
	CalcPortalOffset(pPicture);
}


BOOL RadarDialog::CheckClickedPosition()
{
	CCtrlPictureWZ * pPicture = (CCtrlPictureWZ *)getControl( RADAR_MAP_IMG );
	BOOL ret = FALSE;
	if( pPicture )
	{
		ret =	BaseRadarDialog::Wrap_CheckClickedPosition(pPicture);
	}
	return ret;
}

void RadarDialog::OnRenderSolarDialog()
{
	if ( !g_pHero )
		return;
/*
	if( g_pHero->IsShowFullMap() )
	{
		return;
	}
*/
	if(show_hide_check_)
	{
		return;
	}

	CCtrlPictureWZ * pPicture = (CCtrlPictureWZ *)getControl( RADAR_MAP_IMG );
	if( pPicture)
	{
		BaseRadarDialog::Wrap_render(pPicture);
	}
	if (IsShowToolTipForMapName())
	{
		CCtrlStaticWZ * pStatic = (CCtrlStaticWZ *)getControl( RADAR_MAP_NAME );
		ShowToolTipMapName(pStatic, g_pMap->GetFieldName());							
	}
}

void RadarDialog::SetMaxLayer( int iMaxLayer )
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

void RadarDialog::SetCurLayer( int iCurLayer )
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

void RadarDialog::InitLayerShowNHide( void )
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

#ifdef _NA_000000_20130114_RENEWER_UI
void RadarDialog::HideAllControl(bool value)
{
	show_hide_check_ = !value;

	for (int i = RADAR_MAP_IMG ; i < RADAR_MAX ; ++i)
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

	CControlWZ* control = getControl(RADAR_MAP_NAME);
	if(control)
	{
		control->ShowWindowWZ(WZ_SHOW);
	}

	control = getControl(RADAR_PLUS);
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
	
	DisablePosInWindow(value);

}

void RadarDialog::DisablePosInWindow(bool value)
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

		control = getControl(RADAR_MAP_IMG);
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

		control = getControl(RADAR_MAP_IMG);
		if(control)
		{
			control->SetDiscardMessage(false);
		}
	}
	
}
#endif //_NA_000000_20130114_RENEWER_UI
//========================================================================================

