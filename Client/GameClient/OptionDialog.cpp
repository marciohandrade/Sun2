#include "SunClientPrecompiledHeader.h"
#include "Application.h"
#include "OptionDialog.h"
#include "ResourceManager.h"
#include "InterfaceManager.h"
#include "Map.h"
#include "Sound.h"
#include "SoundBGM.h"
#include "globalfunc.h"
#include "GameFunc.h"
#include "uiQuestMan/uiQuestMan.h"
#include "Camera.h"
#pragma warning ( push )
#pragma warning(disable : 4800)

#ifdef _NA_000000_20130114_RENEWER_UI
#else

const int	c_iGammaScrollMin = 64;
const int	c_iGammaScrollMax = 255;

const float c_fCullScrollMin  = 5000.0f;
const float c_fCullScrollLength  = 1400.0f;
const float c_fCullScrollMax	= c_fCullScrollMin + c_fCullScrollLength;

const float c_fGammaMin = 64.0f;
const float c_fGammaMax = 255.0f;

const int	c_iOptionMaxNum = 2;
const int   c_IEffectOptionMaxNum = 3;
const int	c_iMaxSelectList = 4;

int		iOption[c_iMaxSelectList] = { 70311, 70312, 70313, 70314 };				//	높음, 중간, 낮음,끔




WzID OptionDialog::m_wzId[OPTION_MAX] = 
{
	StrToWzID("B999"),				//OPTION_BT_EXIT			//	버튼 - 종료
	StrToWzID("TI06"),				//OPTION_BT_INIT			//	초기화 버튼
	StrToWzID("TI02"),				//OPTION_BT_AUTO_INIT		//	자동설정 버튼 - 사용 안함
	StrToWzID("TI07"),				//OPTION_BT_OK				//	OK버튼

	StrToWzID("B000"),				//OPTION_BT_SCREEN			//	해상도 버튼
	StrToWzID("B001"),				//OPTION_BT_TEXTURE_LEFT,	//	택스쳐 품질 버튼 (좌)
	StrToWzID("B002"),				//OPTION_BT_TEXTURE_RIGHT,	//	택스쳐 품질 버튼 (우)
	StrToWzID("B003"),				//OPTION_BT_LOD_LEFT,		//	LOD (좌)
	StrToWzID("B004"),				//OPTION_BT_LOD_RIGHT,		//	LOD (좌)
	StrToWzID("B005"),				//OPTION_BT_EFFECT_LEFT,	//  이펙트 품질 (좌)
	StrToWzID("B006"),				//OPTION_BT_EFFECT_RIGHT,	//  이펙트 품질 (우)

	StrToWzID("C001"),				//OPTION_CHECKBOX_SHADOW,		//  그림자 체크박스
	StrToWzID("C002"),				//OPTION_CHECKBOX_GLOW,			//	뽀샤시 체크박스
	StrToWzID("C004"),				//OPTION_CHECKBOX_MAP_SPECULAR,	//	배경 스펙큘러 효과 체크박스
	StrToWzID("C003"),				//OPTION_CHECKBOX_FOG,			//	안개효과 체크박스

	StrToWzID("T021"),				//OPTION_STATIC_SCREEN,			//	해상도 
	StrToWzID("T022"),				//OPTION_STATIC_TEXTURE,		//	택스쳐 품질
	StrToWzID("T023"),				//OPTION_STATIC_LOD,			//	LOD
	StrToWzID("T024"),				//OPTION_STATIC_EFFECT,			//	이펙트 품질
	
	StrToWzID("L000"),				//OPTION_LIST_SCREEN,			//	해상도 리스트
	StrToWzID("HS00"),				//OPTION_HSCROLL_GAMMA,			// 감마조절 스크롤
	StrToWzID("HS01"),
	StrToWzID("VS00"),				//OPTION_LIST_SCREEN_VSCR		// 해상도 스크롤 
	StrToWzID("PI00"),				//OPTION_LIST_SCREEN_PICTURE    // 스크롤 백판
#ifdef _YMS_WINDOW_MAXSCREEN
#else
	StrToWzID("PI04"),				//OPTION_LIST_SCREEN_BG    // 리스트 백판			
#endif //_YMS_WINDOW_MAXSCREEN
	StrToWzID("CT01"),				//OPTION_CHECKBOX_TILEBLENDING	// 타일블렌딩
    StrToWzID("C000"),				//OPTION_CHECKBOX_VOLUME_LIGHT 태양효과
#ifdef _YMS_WINDOW_MAXSCREEN
    StrToWzID("L001"),				//OPTION_SCREEN_MODE_LISTBOX
    StrToWzID("B007"),				//OPTION_SCREEN_MODE_BUTTON, 
    StrToWzID("T020"),				//OPTION_SCREEN_MODE_STATIC, 
#endif //_YMS_WINDOW_MAXSCREEN
};


OptionDialog::OptionDialog( InterfaceManager * pUIMan )
:uiBase( pUIMan )
{
	int		i;
	for( i = 0 ; i < OCB_MAX ; ++i )
	{
		m_pCheckBox[i] = NULL;
	}

	for( i = 0 ; i < OST_MAX ; ++i )
	{
		m_pStatic[i] = NULL;
	}

	m_pHScrollGamma		= NULL;
	m_pListScreen		= NULL;	
	m_pHScrollCull		= NULL;
}

OptionDialog::~OptionDialog( void )
{
}

//----------------------------------------------------------------------------
/**
*/
VOID 
OptionDialog::OnUpdateSolarDialog()
{

	if(m_pScreenVScrollCtrl && m_pListScreen)
	{
		UpdateMouseWheel();
        
		m_pListScreen->SetBeginDrawIndex( m_pScreenVScrollCtrl->GetScrollPosWZ() );
	}
}

//해상도 스크롤추가
void OptionDialog::UpScroll()
{
	int pos = m_pScreenVScrollCtrl->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
	int mmin = 0 , mmax = 0;
	m_pScreenVScrollCtrl->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

	if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
	{
		pos--;//한줄 감소 시키고 
		m_pScreenVScrollCtrl->SetScrollPosWZ(pos);
	}
}
void OptionDialog::DownScroll()
{
	//.아랫쪽 화살표를 눌렀다
	int pos = m_pScreenVScrollCtrl->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
	int mmin = 0 , mmax = 0;
	m_pScreenVScrollCtrl->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

	if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
	{
		pos++;//한줄 증가 시키고 
		m_pScreenVScrollCtrl->SetScrollPosWZ(pos);
	}
}
void OptionDialog::UpdateMouseWheel()
{
	//상단블럭에서 널체크하구있씀
	if(!m_bListScreenShow) return;

	RECT rc = m_pListScreen->GetSizeRect();

	switch(GetMouseWheelStatus(&rc))
	{
	case MOUSE_WHEEL_UP:	UpScroll();  break;
	case MOUSE_WHEEL_DOWN:  DownScroll();break;
	}
}

VOID OptionDialog::Init( CDrawBase * pDrawBase )
{
	/**
	사이즈 가변형에 적합하도록 레이더 위치 우측으로 정렬
	*/

	//UpdatePosition();

	//-------------------------------------------------------------------------
	//		옵션 데이터 설정
	//-------------------------------------------------------------------------
	m_OptionParam = GAMEOPTION->GetGameOption();
	m_InitOptionParam = m_OptionParam;

	DWORD	dwTextColor	= RGBA( 235, 192, 38, 255 );
	DWORD	dwWideTextColor	= RGBA( 245, 166, 46, 255 );
    DWORD	dwWideTextColor2	= RGBA( 240, 179, 42, 255 );
	DWORD	dwSelectColor = RGBA( 0, 0, 255,255 );
	DWORD	dwBGColor = RGBA( 32, 32, 32, 0 );
	DWORD	dwSetColorSelBarOnText = RGBA( 255, 255, 255, 255 );

	int index;
	int	i;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH]	= {0,};
	TCHAR	szText[INTERFACE_STRING_LENGTH]		= {0,};

	m_pListScreen = static_cast<CCtrlListWZ *>(getControl(OPTION_LIST_SCREEN));

	m_pScreenVScrollCtrl= (CCtrlVScrollWZ *)getControl( OPTION_LIST_SCREEN_VSCR );

	m_pScrollBg=static_cast<CCtrlPictureWZ *> (getControl(OPTION_LIST_SCREEN_PICTURE) );
#ifdef _YMS_WINDOW_MAXSCREEN
    m_pScreenBg = NULL;
#else
	m_pScreenBg=static_cast<CCtrlPictureWZ *> (getControl(OPTION_LIST_SCREEN_BG) );
#endif //_YMS_WINDOW_MAXSCREEN

	//	리스트 컨트롤 감추기
	if (m_pListScreen)
    {
        m_pListScreen->ShowWindowWZ( WZ_HIDE );
    }

	if(m_pScreenBg)
	{
		m_pScreenBg->ShowWindowWZ( WZ_HIDE );
	}

	if(m_pScreenVScrollCtrl)
		m_pScreenVScrollCtrl->ShowWindowWZ( WZ_HIDE );


	if(m_pScrollBg)
		m_pScrollBg->ShowWindowWZ( WZ_HIDE );



	m_bListScreenShow = FALSE;

	//	스테틱 택스트 컨트롤 얻어오기
	m_pStatic[OST_SCREEN] = static_cast<CCtrlStaticWZ *>(getControl( OPTION_STATIC_SCREEN ));
	m_pStatic[OST_TEXTURE] = static_cast<CCtrlStaticWZ *>(getControl( OPTION_STATIC_TEXTURE ));
	m_pStatic[OST_LOD] = static_cast<CCtrlStaticWZ *>(getControl( OPTION_STATIC_LOD ));
	m_pStatic[OST_EFFECT] = static_cast<CCtrlStaticWZ *>(getControl( OPTION_STATIC_EFFECT ));

	//	체크 박스 얻어오기( 효과 끄고 켜기 )
	m_pCheckBox[OCB_SHADOW]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_CHECKBOX_SHADOW));
	m_pCheckBox[OCB_GLOW]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_CHECKBOX_GLOW));
	m_pCheckBox[OCB_MAP_SPECULAR]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_CHECKBOX_MAP_SPECULAR));
	m_pCheckBox[OCB_FOG]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_CHECKBOX_FOG));
	
	m_pCheckBox[OCB_BLEND]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_CHECKBOX_TILEBLENDING));

    m_pCheckBox[OCB_VOLUME_LIGHT]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_CHECKBOX_VOLUME_LIGHT));

#ifdef _YMS_WINDOW_MAXSCREEN
    CControlWZ* control_ptr = getControl(OPTION_SCREEN_MODE_LISTBOX);
    if (control_ptr != NULL && control_ptr->IsKind(CTRL_KIND_LIST))
    {
        CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(control_ptr);

        list_control->ShowWindowWZ(WZ_HIDE);
        list_control->SetStartLineGapTop(3);
        list_control->SetSellHeight(15);
        list_control->SetColorSelBar(dwSelectColor);

        //2431 해상도 구분
        //2432 전체화면
        //2433 창모드
        //2434 창모드(전체)
        const static int string_code[OptionParam::kScreenMode_Count] = {2432, 2433, 2434};
        for (int i = 0; i < (int)(OptionParam::kScreenMode_Count); ++i)
        {
            int index = list_control->InsertItemBack();

            GlobalFunc::SetItem(list_control, index, 0, 
                                (TCHAR*)(g_InterfaceManager.GetInterfaceString(string_code[i])), 
                                StrToWzID("mn12"), DT_VCENTER |  DT_CENTER, 
                                dwBGColor, dwTextColor, dwSetColorSelBarOnText);

            list_control->SetItemData(index, (void*)string_code[i]);
        }
        CControlWZ* static_control = getControl(OPTION_SCREEN_MODE_STATIC);
        if (static_control != NULL)
        {
            static_control->SetTextWZ(g_InterfaceManager.GetInterfaceString(string_code[m_OptionParam.GetScreenMode()]));
        }
    }
    if (m_pListScreen != NULL)
    {
        m_pListScreen->SetSellHeight(18);  
        m_pListScreen->SetStartLineGapTop(0);
    }

#endif //_YMS_WINDOW_MAXSCREEN

	// 감마 스크롤바
	m_pHScrollGamma = static_cast<CCtrlHScrollWZ *>(getControl(OPTION_HSCROLL_GAMMA));
    if (m_pHScrollGamma)
    {
        m_pHScrollGamma->SetScrollRangeWZ( c_iGammaScrollMin, c_iGammaScrollMax );
        m_pHScrollGamma->SetScrollPosWZ( m_OptionParam.GetOptionValueFloat(nOptionType::GAMMA_VALUE));
    }

	m_pHScrollCull	= static_cast<CCtrlHScrollWZ *>(getControl(OPTION_HSCROLL_CULL));
	if( m_pHScrollCull )
	{
		m_pHScrollCull->SetScrollRangeWZ(0.f , 100.f );
        m_pHScrollCull->SetScrollPosWZ( m_OptionParam.GetOptionValueFloat(nOptionType::CULLING_DISTANCE_VALUE));
	}


	// 해상도
    if (m_pListScreen)
    {
#ifdef _YMS_WINDOW_MAXSCREEN
#else
        int		iSellHeight = m_pListScreen->GetSellheight();
		RECT	rc = m_pListScreen->GetSizeRect();

		m_pListScreen->MoveWindowWZ((float)rc.left,(float)rc.top,(float)(rc.right - rc.left),(float)(iSellHeight *5 ));
#endif //_YMS_WINDOW_MAXSCREEN
 
        //	화면 해상도 리스트 설정
        for( i = 0 ; i <  GAMEOPTION->GetAvailableScreenSettingCount(); ++i )
        {
            St_ScreenSetting ss = GAMEOPTION->GetAvailableScreenSetting(i).ScreenSetting;

            index = m_pListScreen->InsertItemBack();
            Sprintf( szMessage, _T("%d x %d x %d"), ss.m_iWidth, ss.m_iHeight, ss.m_iBpp );
            //GlobalFunc::SetItemText( m_pListScreen, index, szMessage );
            ////		m_pList[OLC_SCREEN]->SetItemText( index, szMessage );
            //m_pListScreen->SetItemFontID( index, 0, StrToWzID("mn12") );


			float fRate = (float)ss.m_iWidth/ (float)ss.m_iHeight;
			DWORD dwTempTextColor;

            if (g_pApplication->screen_ratio() == SR_16_9)   
            {
                dwTempTextColor = dwWideTextColor;
            }
            else if (g_pApplication->screen_ratio() == SR_16_10)   
            {
                dwTempTextColor = dwWideTextColor2;
            }
            else
            {
                dwTempTextColor=dwTextColor;
            }

            m_pListScreen->SetColorSelBar( dwSelectColor );
            m_pListScreen->SetColorSelBarOnText( dwSetColorSelBarOnText );
            m_pListScreen->SetItemBKColor( index, 0, dwBGColor );
            GlobalFunc::SetItem( m_pListScreen, index, 0, szMessage, StrToWzID("mn12"), DT_VCENTER |  DT_CENTER, dwBGColor, dwTempTextColor, dwSetColorSelBarOnText );

			if(m_pScreenVScrollCtrl)
			{
				int iRange = 0;
				if(i > (5-1))
				{
					iRange = i - 5 + 1;
				}

				m_pScreenVScrollCtrl->SetScrollRangeWZ(0,iRange);	//스크롤 범위는 입력된 라인수로 한다.
				m_pScreenVScrollCtrl->SetScrollPosWZ(0);

			}

        }
    }

	// 스크롤 처리


	//	컨트롤에 적용
	ApplyControl();
}


VOID OptionDialog::Release()
{
}

VOID OptionDialog::MessageProc( SI_MESSAGE * pMessage )
{
	int		iIndex = -1;
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	*pszTRUE = _T("TRUE");
	TCHAR	*pszFALSE = _T("FALSE");

	int		iSelect;
	float	fGammaStep = 1.0f;

	switch( pMessage->eResultMsg ) 
	{
		//-----------------------------------------------------------------------------------------
		//		감마조절 스크롤 바 마우스로 끌어땡기기
		//-----------------------------------------------------------------------------------------
	case RT_MSG_SB_THUMBTRACK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case OPTION_HSCROLL_GAMMA:
				{
					float fGamma = c_fGammaMin;
					int iScrollPos = m_pHScrollGamma->GetScrollPosWZ();
					if(iScrollPos > 0)
					{
						fGamma = (float)iScrollPos;
					}

                    m_OptionParam.SetOptionValue( nOptionType::GAMMA_VALUE, (int)fGamma * 100 );

					SetGamma(DWORD(fGamma));

				}
				break;
			}
		}
		break;

	case RT_MSG_SB_THUMBPOSITION:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case OPTION_HSCROLL_GAMMA:
				{
					float fGamma = c_fGammaMin;
					int iScrollPos = m_pHScrollGamma->GetScrollPosWZ();
					if(iScrollPos > 0)
					{
						fGamma = (float)iScrollPos;
					}

                    m_OptionParam.SetOptionValue( nOptionType::GAMMA_VALUE, (int)fGamma * 100 );
					
					SetGamma(DWORD(fGamma));
					
				}
				break;


			case OPTION_HSCROLL_CULL:
				{
					float fCull = 0;
					int iScrollPos = m_pHScrollCull->GetScrollPosWZ();
					if( iScrollPos > 0)
					{
						fCull =  (float) iScrollPos;
					}

                    m_OptionParam.SetOptionValue( nOptionType::CULLING_DISTANCE_VALUE, (int)fCull * 100 );
				}
				break;

			}
		}
		break;

		//-----------------------------------------------------------------------------------------
		//		감마조절 스크롤 바 왼쪽 버튼 클릭
		//-----------------------------------------------------------------------------------------
	case RT_MSG_SB_LINELEFT:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case OPTION_HSCROLL_GAMMA:
				{
                    float fOldGamma = m_OptionParam.GetOptionValueFloat(nOptionType::GAMMA_VALUE);
					float fGamma = fOldGamma - fGammaStep;
					if(fGamma <= c_fGammaMin)
					{
						fGamma = c_fGammaMin;
					}
					m_pHScrollGamma->SetScrollPosWZ( (int)fGamma);

                    m_OptionParam.SetOptionValue( nOptionType::GAMMA_VALUE, (int)fGamma * 100 );
					SetGamma(DWORD(fGamma));
				}
				break;

			case OPTION_HSCROLL_CULL:
				{
                    float fOldCullLength = m_OptionParam.GetOptionValueFloat(nOptionType::CULLING_DISTANCE_VALUE);
					float fCullLength = fOldCullLength - 10.f;
					if( fCullLength <= 0.f )
					{
						fCullLength = 0.f;
					}
					m_pHScrollCull->SetScrollPosWZ((int)fCullLength );
                    m_OptionParam.SetOptionValue( nOptionType::CULLING_DISTANCE_VALUE, (int)fCullLength * 100 );
				}	
				break;

			}

		}
		break;


		//-----------------------------------------------------------------------------------------
		//		감마조절 스크롤 바 오른쪽 버튼 클릭
		//-----------------------------------------------------------------------------------------
	case RT_MSG_SB_LINERIGHT:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case OPTION_HSCROLL_GAMMA:
				{
                    float fOldGamma = m_OptionParam.GetOptionValueFloat(nOptionType::GAMMA_VALUE);
					float fGamma = fOldGamma + fGammaStep;
					if(fGamma >= c_fGammaMax)
					{
						fGamma = c_fGammaMax;
					}
					m_pHScrollGamma->SetScrollPosWZ( (int)fGamma);

                    m_OptionParam.SetOptionValue( nOptionType::GAMMA_VALUE, (int)fGamma * 100 );
					SetGamma(DWORD(fGamma));
				}
				break;


			case OPTION_HSCROLL_CULL:
				{
                    float fOldCullLength = m_OptionParam.GetOptionValueFloat(nOptionType::CULLING_DISTANCE_VALUE);
					float fCullLength = fOldCullLength + 10.f;
					if( fCullLength >= 100.0f)
					{
						fCullLength = 100.0f;
					}
					m_pHScrollCull->SetScrollPosWZ((int)fCullLength);
                    m_OptionParam.SetOptionValue( nOptionType::CULLING_DISTANCE_VALUE, (int)fCullLength * 100 );
				}
				break;

			}
		}
		break;

	case RT_MSG_SB_LINETOP:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{

			case OPTION_LIST_SCREEN_VSCR:
				{
					
					UpScroll();
					
				}
				break;
			}

		}
		break;
	case RT_MSG_SB_LINEBOTTOM:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{

			case OPTION_LIST_SCREEN_VSCR:
				{
					DownScroll();
				}
				break;
			}

		}
		break;



		//-----------------------------------------------------------------------------------------
		//		마우스 왼쪽 클릭 이벤트가 일어났을 때, 어디냐?
		//-----------------------------------------------------------------------------------------
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
				//---------------------------------------------------------------------------------
				//		리스트 박스를 보여주는 버튼들~
				//---------------------------------------------------------------------------------
			case OPTION_STATIC_SCREEN:
			case OPTION_BT_SCREEN:				//	해상도 버튼
				{
					if( m_bListScreenShow == FALSE )
					{
						if (m_pListScreen)
						{
						    m_pListScreen->ShowWindowWZ( WZ_SHOW );

							if(m_pScreenVScrollCtrl)
								m_pScreenVScrollCtrl->ShowWindowWZ( WZ_SHOW );
							
							if(m_pScrollBg)
								m_pScrollBg->ShowWindowWZ( WZ_SHOW );

							if(m_pScreenBg)
								m_pScreenBg->ShowWindowWZ( WZ_SHOW );
							

							m_bListScreenShow = TRUE;
						}
						    
					}
					else
					{
                        if (m_pListScreen)
						{
    						m_pListScreen->ShowWindowWZ( WZ_HIDE );

							if(m_pScreenBg)
								m_pScreenBg->ShowWindowWZ( WZ_HIDE );

							if(m_pScreenVScrollCtrl)
								m_pScreenVScrollCtrl->ShowWindowWZ( WZ_HIDE );
							
							if(m_pScrollBg)
								m_pScrollBg->ShowWindowWZ( WZ_HIDE );
							
							m_bListScreenShow = FALSE;
						}
					}
#ifdef _YMS_WINDOW_MAXSCREEN
                    if (getControl(OPTION_SCREEN_MODE_LISTBOX))
                    {
                        getControl(OPTION_SCREEN_MODE_LISTBOX)->ShowWindowWZ(WZ_HIDE);
                    }
#endif // _YMS_WINDOW_MAXSCREEN
				}
				break;

			//	택스쳐 품질 버튼
			case OPTION_BT_TEXTURE_LEFT:
				{
                    int iSelect = m_OptionParam.GetOptionValue(nOptionType::TEXTURE_QUALITY);
					if(iSelect < c_iOptionMaxNum)
					{
						++iSelect;
					}
					else
					{
						iSelect = 0;
					}

                    m_OptionParam.SetOptionValue( nOptionType::TEXTURE_QUALITY, iSelect );
					g_InterfaceManager.GetInterfaceString( iOption[iSelect], szText, INTERFACE_STRING_LENGTH );
                    GlobalFunc::SetCaption( m_pStatic[OST_TEXTURE], szText );
				}
				break;

			case OPTION_BT_TEXTURE_RIGHT:
				{
                    int iSelect = m_OptionParam.GetOptionValue(nOptionType::TEXTURE_QUALITY);
					if(iSelect > 0)
					{
						--iSelect;
					}
					else
					{
						iSelect = c_iOptionMaxNum;
					}

                    m_OptionParam.SetOptionValue( nOptionType::TEXTURE_QUALITY, iSelect );
					g_InterfaceManager.GetInterfaceString( iOption[iSelect], szText, INTERFACE_STRING_LENGTH );
                    GlobalFunc::SetCaption( m_pStatic[OST_TEXTURE], szText );
				}
				break;

			// LOD 버튼
			case OPTION_BT_LOD_LEFT:
				{
                    int iSelect = m_OptionParam.GetOptionValue(nOptionType::LOD_LEVEL);
					if(iSelect < c_iOptionMaxNum)
					{
						++iSelect;
					}
					else
					{
						iSelect = 0;
					}
					
                    m_OptionParam.SetOptionValue(nOptionType::LOD_LEVEL, iSelect );
					g_InterfaceManager.GetInterfaceString( iOption[iSelect], szText, INTERFACE_STRING_LENGTH );
                    GlobalFunc::SetCaption( m_pStatic[OST_LOD], szText );
				}
				break;

			case OPTION_BT_LOD_RIGHT:
				{
                    int iSelect = m_OptionParam.GetOptionValue(nOptionType::LOD_LEVEL);
					if(iSelect > 0)
					{
						--iSelect;
					}
					else
					{
						iSelect = c_iOptionMaxNum;
					}

                    m_OptionParam.SetOptionValue(nOptionType::LOD_LEVEL, iSelect );
                    g_InterfaceManager.GetInterfaceString( iOption[iSelect], szText, INTERFACE_STRING_LENGTH );
                    GlobalFunc::SetCaption( m_pStatic[OST_LOD], szText );
				}
				break;

			// 이펙트 품질 버튼
			case OPTION_BT_EFFECT_LEFT:
				{
                    int iSelect = m_OptionParam.GetOptionValue(nOptionType::EFFECT_QUALITY);
					if(iSelect < c_IEffectOptionMaxNum)
					{
						++iSelect;
					}
					else
					{
						iSelect = 0;
					}
					
                    m_OptionParam.SetOptionValue(nOptionType::EFFECT_QUALITY, iSelect );
					g_InterfaceManager.GetInterfaceString( iOption[iSelect], szText, INTERFACE_STRING_LENGTH );
                    GlobalFunc::SetCaption( m_pStatic[OST_EFFECT], szText );
				}
				break;

			case OPTION_BT_EFFECT_RIGHT:
				{
                    int iSelect = m_OptionParam.GetOptionValue(nOptionType::EFFECT_QUALITY);
					if(iSelect > 0)
					{
						--iSelect;
					}
					else
					{
						iSelect = c_iMaxSelectList-1;
					}

                    m_OptionParam.SetOptionValue(nOptionType::EFFECT_QUALITY, iSelect );
                    g_InterfaceManager.GetInterfaceString( iOption[iSelect], szText, INTERFACE_STRING_LENGTH );
                    GlobalFunc::SetCaption( m_pStatic[OST_EFFECT], szText );
				}
				break;

			
			//---------------------------------------------------------------------------------
			//		리스트 박스
			//---------------------------------------------------------------------------------
			case OPTION_LIST_SCREEN:
				{
                    if (m_pListScreen)
                    {
                        iSelect = m_pListScreen->GetCurSel();
                        if( iSelect > -1 && iSelect < GAMEOPTION->GetAvailableScreenSettingCount() )
                        {
                            m_OptionParam.SetScreenIndex( iSelect );

                            SCREEN_SETTING ss = GAMEOPTION->GetAvailableScreenSetting(iSelect);

                            m_pListScreen->ShowWindowWZ( WZ_HIDE );

							if(m_pScreenBg)
								m_pScreenBg->ShowWindowWZ( WZ_HIDE );


							if(m_pScreenVScrollCtrl)
								m_pScreenVScrollCtrl->ShowWindowWZ( WZ_HIDE );

							if(m_pScrollBg)
								m_pScrollBg->ShowWindowWZ( WZ_HIDE );

                            m_bListScreenShow = FALSE;
                            Sprintf( szMessage, _T("%d x %d x %d"),	ss.ScreenSetting.m_iWidth,
                                ss.ScreenSetting.m_iHeight,
                                ss.ScreenSetting.m_iBpp );
                            GlobalFunc::SetCaption( m_pStatic[OST_SCREEN], szMessage );
                            
                        }
                    }
				}
				break;

			//---------------------------------------------------------------------------------
			//		체크 박스들~
			//---------------------------------------------------------------------------------
			case OPTION_CHECKBOX_SHADOW:
				{
					if (m_pCheckBox[OCB_SHADOW])
                        m_OptionParam.SetOptionValue(nOptionType::SHOW_SHADOW, m_pCheckBox[OCB_SHADOW]->GetCheckState() );
				}
				break;

			case OPTION_CHECKBOX_GLOW:
				{
                    if (m_pCheckBox[OCB_GLOW])
                        m_OptionParam.SetOptionValue(nOptionType::USE_BLUR, m_pCheckBox[OCB_GLOW]->GetCheckState() );
				}
				break;

			case OPTION_CHECKBOX_FOG:
				{
                    if (m_pCheckBox[OCB_FOG])
                        m_OptionParam.SetOptionValue(nOptionType::USE_FOG, m_pCheckBox[OCB_FOG]->GetCheckState() );
				}
				break;

			case OPTION_CHECKBOX_MAP_SPECULAR:
				{
					if (m_pCheckBox[OCB_MAP_SPECULAR])
                        m_OptionParam.SetOptionValue(nOptionType::USE_MAP_DISTANCE_VIEW, m_pCheckBox[OCB_MAP_SPECULAR]->GetCheckState() );
				}
				break;
			case OPTION_CHECKBOX_TILEBLENDING:
				{
					if (m_pCheckBox[OCB_BLEND])
                        m_OptionParam.SetOptionValue(nOptionType::USE_TILE_BLENDING, m_pCheckBox[OCB_BLEND]->GetCheckState() );
				}
				break;			
			
            case OPTION_CHECKBOX_VOLUME_LIGHT:
                {
                   if (m_pCheckBox[OCB_VOLUME_LIGHT])
                   {
                       m_OptionParam.SetOptionValue(nOptionType::USE_VOLUME_LIGHT, m_pCheckBox[OCB_VOLUME_LIGHT]->GetCheckState() );
                   }
                }
                break;
			//---------------------------------------------------------------------------------
			//		아래 세개 버튼과 X 종료 버튼
			//---------------------------------------------------------------------------------
			case OPTION_BT_INIT:	//	초기화 - 처음 창 열었을 때로 적용
				{
					m_OptionParam = m_InitOptionParam;
					ApplyControl(true);
				}
				break;

			case OPTION_BT_AUTO_INIT:
				{
					//자동 설정 - 현재 기능 없음
				}
				break;

			case OPTION_BT_OK:		//	OK 버튼 - 적용의 의미
				{
					ApplyScroll();
					ApplyControl();
					Apply();
					ShowDialog(FALSE);
				}
				break;

			case OPTION_BT_EXIT:	//	종료 버튼 - 취소의 의미
				{
					m_OptionParam = m_InitOptionParam;
					ApplyControl();
					ShowDialog(FALSE);
				}
				break;

#ifdef _YMS_WINDOW_MAXSCREEN
            case OPTION_SCREEN_MODE_LISTBOX:
                {
                    CControlWZ* control_ptr = GetControlWZ(pMessage->dwCtrlID);
                    if (control_ptr != NULL && control_ptr->IsKind(CTRL_KIND_LIST))
                    {
                        SCREEN_SETTING screen_setting = GAMEOPTION->GetAvailableScreenSetting(m_OptionParam.GetScreenIndex());

                        CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(control_ptr);
                        int select_index = list_control->GetCurSel();
                        if (select_index >= 0)
                        {
                            int string_code = (int)(list_control->GetItemData(select_index));

                            CControlWZ* static_control = getControl(OPTION_SCREEN_MODE_STATIC);
                            if (static_control != NULL)
                            {
                                static_control->SetTextWZ(g_InterfaceManager.GetInterfaceString(string_code));
                            }

                            m_OptionParam.SetScreenMode((OptionParam::OptionScreenMode)select_index );
                            if( select_index == OptionParam::kScreenMode_MaxWindow)
                            {
                                if (getControl(OPTION_BT_SCREEN))   
                                {
                                    getControl(OPTION_BT_SCREEN)->ShowWindowWZ(WZ_HIDE);
                                }
                                if (m_pStatic[OST_SCREEN])   
                                {
                                    m_pStatic[OST_SCREEN]->SetTextColor(RGBA(128, 128, 128, 255));

                                    Sprintf(szMessage, _T("%d x %d x %d"), g_pApplication->desktop_screen_size().x,
                                                                            g_pApplication->desktop_screen_size().y,
                                                                            screen_setting.ScreenSetting.m_iBpp);

                                    m_pStatic[OST_SCREEN]->SetTextWZ(szMessage);
                                }
                            }
                            else
                            {
                                if (getControl(OPTION_BT_SCREEN))   
                                {
                                    getControl(OPTION_BT_SCREEN)->ShowWindowWZ(WZ_SHOW);
                                }
                                if (m_pStatic[OST_SCREEN])   
                                {
                                    m_pStatic[OST_SCREEN]->SetTextColor(RGBA(255, 255, 255, 255));
                                }

                                m_bListScreenShow = FALSE;
                                Sprintf(szMessage, _T("%d x %d x %d"),	screen_setting.ScreenSetting.m_iWidth,
                                                                        screen_setting.ScreenSetting.m_iHeight,
                                                                        screen_setting.ScreenSetting.m_iBpp);
                                m_pStatic[OST_SCREEN]->SetTextWZ(szMessage);
                            }
                        }
                        list_control->ShowWindowWZ(WZ_HIDE);
                    }
                }
                break;
            case OPTION_SCREEN_MODE_BUTTON:
                {
                    CControlWZ* control_ptr = getControl(OPTION_SCREEN_MODE_LISTBOX);  
                    if (control_ptr != NULL)
                    {
                        if (control_ptr->GetState() == WZ_SHOW)
                        {
                            control_ptr->ShowWindowWZ(WZ_HIDE);
                        }
                        else
                        {
                            control_ptr->ShowWindowWZ(WZ_SHOW);
                        }
                        if (m_pListScreen)          m_pListScreen->ShowWindowWZ(WZ_HIDE);
                        if (m_pScreenBg)            m_pScreenBg->ShowWindowWZ(WZ_HIDE);
                        if (m_pScreenVScrollCtrl)   m_pScreenVScrollCtrl->ShowWindowWZ(WZ_HIDE);
                        if (m_pScrollBg)            m_pScrollBg->ShowWindowWZ(WZ_HIDE);
                        m_bListScreenShow = FALSE;
                    }
                }
                break;             
#endif //_YMS_WINDOW_MAXSCREEN

			default:
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

}


VOID OptionDialog::NetworkProc( MSG_BASE * pMsg )
{
}


CControlWZ*  OptionDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < OPTION_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}


POSTYPE OptionDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < OPTION_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return OPTION_MAX;
}


VOID OptionDialog::OnShowWindow( BOOL val )
{

	if( val == TRUE )
	{
		if (m_pListScreen)
			m_pListScreen->ShowWindowWZ( WZ_HIDE );

		if(m_pScreenBg)
			m_pScreenBg->ShowWindowWZ( WZ_HIDE );


		if(m_pScreenVScrollCtrl)
			m_pScreenVScrollCtrl->ShowWindowWZ( WZ_HIDE );

		if(m_pScrollBg)
			m_pScrollBg->ShowWindowWZ( WZ_HIDE );

        if (m_bListScreenShow)
			m_bListScreenShow = FALSE;

		ApplyControl(true);

#ifdef _YMS_WINDOW_MAXSCREEN
        CControlWZ* control_ptr = getControl(OPTION_SCREEN_MODE_LISTBOX);
        if (control_ptr != NULL && control_ptr->IsKind(CTRL_KIND_LIST))
        {
            CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(control_ptr);
            list_control->SetCurSel(m_OptionParam.GetScreenMode());

            CControlWZ* static_control = getControl(OPTION_SCREEN_MODE_STATIC);
            if (static_control != NULL)
            {
                int string_code = (int)(list_control->GetItemData(m_OptionParam.GetScreenMode()));
                static_control->SetTextWZ(g_InterfaceManager.GetInterfaceString(string_code));
            }
        }
#endif //_YMS_WINDOW_MAXSCREEN

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Option_System2;
		msg.wParam=InterfaceManager::DIALOG_OPTION_SYSTEM;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

	}
	else
	{
		ApplyControl();
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Option_System2);
	}

}

//-------------------------------------------------------------------------------------------
void OptionDialog::ApplyControl(bool is_check_mode)
{
	//-------------------------------------------------------------------------
	//		이펙트 체크 버튼들 값 설정
	//-------------------------------------------------------------------------
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};

#ifdef _YMS_WINDOW_MAXSCREEN
    St_ScreenSetting st = GAMEOPTION->GetAvailableScreenSetting(m_OptionParam.GetScreenIndex()).ScreenSetting;

    if (is_check_mode)
    {
        //! control key + enter 로 변경되었을수가 있다.
        St_ScreenFormat screen_format;
        g_pSunRenderer->GetScreenFormat(&screen_format); 
        if ((screen_format.m_bFullScreen == TRUE) && (m_OptionParam.GetScreenMode() != OptionParam::kScreenMode_Full))
        {
            m_OptionParam.SetScreenMode(OptionParam::kScreenMode_Full);
        }
        else if ((screen_format.m_bFullScreen == FALSE) && (m_OptionParam.GetScreenMode() == OptionParam::kScreenMode_Full))
        {
            m_OptionParam.SetScreenMode(OptionParam::kScreenMode_Window);
        }
    }

    if ((m_OptionParam.GetScreenMode() == OptionParam::kScreenMode_Window) && 
        (st.m_iWidth == g_pApplication->desktop_screen_size().x) && 
        (st.m_iHeight == g_pApplication->desktop_screen_size().y))
    {
        m_OptionParam.SetScreenMode( OptionParam::kScreenMode_MaxWindow );
    }
    if (m_OptionParam.GetScreenMode() == OptionParam::kScreenMode_MaxWindow)
    {
        int screen_index = CGameOption::Instance()->GetAvailableScreenSettingIndex(g_pApplication->desktop_screen_size().x,
                                                                                g_pApplication->desktop_screen_size().y, 
                                                                                st.m_iBpp);
        if (screen_index >= 0)
        {
            m_OptionParam.SetScreenIndex(screen_index);
        }

        if (getControl(OPTION_BT_SCREEN))
        {
            getControl(OPTION_BT_SCREEN)->ShowWindowWZ(WZ_HIDE);
        }
        if (m_pStatic[OST_SCREEN])   
        {
            m_pStatic[OST_SCREEN]->SetTextColor(RGBA(128, 128, 128, 255));
        }
    }
    else
    {
        if (getControl(OPTION_BT_SCREEN))
        {
            getControl(OPTION_BT_SCREEN)->ShowWindowWZ(WZ_SHOW);
        }
        if (m_pStatic[OST_SCREEN])   
        {
            m_pStatic[OST_SCREEN]->SetTextColor(RGBA(255, 255, 255, 255));
        }
    }
    CControlWZ* control_ptr = NULL;
    control_ptr = getControl(OPTION_SCREEN_MODE_STATIC);
    if (control_ptr != NULL)
    {
        const static int string_code[OptionParam::kScreenMode_Count] = {2432, 2433, 2434};
        control_ptr->SetTextWZ(g_InterfaceManager.GetInterfaceString(string_code[m_OptionParam.GetScreenMode()]));
    }
    control_ptr = getControl(OPTION_SCREEN_MODE_LISTBOX);
    if (control_ptr != NULL && control_ptr->IsKind(CTRL_KIND_LIST))
    {
        static_cast<CCtrlListWZ*>(control_ptr)->SetCurSel(m_OptionParam.GetScreenMode());
    }

    const SCREEN_SETTING& ScreenSetting = GAMEOPTION->GetAvailableScreenSetting(m_OptionParam.GetScreenIndex());
    GlobalFunc::SetCaption(m_pStatic[OST_SCREEN], ScreenSetting.SettingText.c_str());
    if (m_pListScreen)
    {
        m_pListScreen->SetCurSel(m_OptionParam.GetScreenIndex());
    }

#else
	const SCREEN_SETTING& ScreenSetting = GAMEOPTION->GetAvailableScreenSetting(m_OptionParam.GetScreenIndex());
	GlobalFunc::SetCaption( m_pStatic[OST_SCREEN], ScreenSetting.SettingText.c_str() );
#endif //_YMS_WINDOW_MAXSCREEN

    int TextureQuality = m_OptionParam.GetOptionValue(nOptionType::TEXTURE_QUALITY);
    int LODLevel = m_OptionParam.GetOptionValue(nOptionType::LOD_LEVEL);
    int EffectQuality = m_OptionParam.GetOptionValue(nOptionType::EFFECT_QUALITY);

	g_InterfaceManager.GetInterfaceString( iOption[TextureQuality], szText, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption( m_pStatic[OST_TEXTURE], szText );

	g_InterfaceManager.GetInterfaceString( iOption[LODLevel], szText, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption( m_pStatic[OST_LOD], szText );

	g_InterfaceManager.GetInterfaceString( iOption[EffectQuality], szText, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption( m_pStatic[OST_EFFECT], szText );

	//-------------------------------------------------------------------------
	//		이펙트 체크 버튼들 값 설정
	//-------------------------------------------------------------------------
	if (m_pCheckBox[OCB_SHADOW])
        m_pCheckBox[OCB_SHADOW]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::SHOW_SHADOW));
    if (m_pCheckBox[OCB_GLOW])
        m_pCheckBox[OCB_GLOW]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_BLUR));
    if (m_pCheckBox[OCB_FOG])
        m_pCheckBox[OCB_FOG]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_FOG));
    if (m_pCheckBox[OCB_MAP_SPECULAR])
        m_pCheckBox[OCB_MAP_SPECULAR]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_MAP_DISTANCE_VIEW));
	if (m_pCheckBox[OCB_BLEND])
        m_pCheckBox[OCB_BLEND]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_TILE_BLENDING));

    if (m_pCheckBox[OCB_VOLUME_LIGHT])
    {
        if( m_OptionParam.GetOptionValue(nOptionType::USE_VOLUME_LIGHT) )
        {
            m_pCheckBox[OCB_VOLUME_LIGHT]->SetCheckState(true);
            g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect(PET_VOLUME_LIGHT);
        }
        else
        {   
            m_pCheckBox[OCB_VOLUME_LIGHT]->SetCheckState(false);
            g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect(PET_VOLUME_LIGHT);
        }
    }

	//-------------------------------------------------------------------------
	//		감마 스크롤 조절 바 초기화
	//-------------------------------------------------------------------------

	if (m_pHScrollGamma)
	{
        m_pHScrollGamma->SetScrollPosWZ( (int)m_OptionParam.GetOptionValueFloat(nOptionType::GAMMA_VALUE) );
	}


	if(m_pHScrollCull)
	{
        m_pHScrollCull->SetScrollPosWZ(m_OptionParam.GetOptionValueFloat(nOptionType::CULLING_DISTANCE_VALUE) );
	}


}

void OptionDialog::ApplyScroll()
{
	if (m_pHScrollGamma)
	{
        float fGamma = (float)m_pHScrollGamma->GetScrollPosWZ();
        m_OptionParam.SetOptionValue(nOptionType::GAMMA_VALUE, (int)(fGamma*100.0f));
        m_pHScrollGamma->SetScrollPosWZ((int)fGamma);
	}


	if(m_pHScrollCull)
	{
        float fCullLength = (float)m_pHScrollCull->GetScrollPosWZ();
        m_OptionParam.SetOptionValue(nOptionType::CULLING_DISTANCE_VALUE, (int)(fCullLength*100.0f));
        m_pHScrollCull->SetScrollPosWZ(fCullLength);
	}

}

//-------------------------------------------------------------------------------------------
BOOL OptionDialog::Apply( BOOL bCheckApply ,BOOL bProgramInit)
{
	//---------------------------------------------------------------------------------------------
	//		이 아래로 각종 설정 기능을 작업해 주시오.
	//---------------------------------------------------------------------------------------------

	BOOL	bApply = FALSE;


	//	해상도

	const int	c_iBPP_24	= 24;
	const int	c_iBPP_16	= 16;

	St_ScreenFormat ScreenFormat;
	g_pSunRenderer->GetScreenFormat( &ScreenFormat );

    St_ScreenFormat backup_screen_format = ScreenFormat;

	// 프로그램이 처음에 뜰때에는 3dsetup에서 설정하지 못하는 옵션만 적용한다.
	if (!bProgramInit)
	{
		bApply = FALSE;
		if( bCheckApply )
		{
			if( m_OptionParam.GetScreenIndex() != m_InitOptionParam.GetScreenIndex() )
			{
				bApply = TRUE;
			}
#ifdef _YMS_WINDOW_MAXSCREEN
            else if (m_OptionParam.GetScreenMode() != m_InitOptionParam.GetScreenMode())
            {
                bApply = TRUE;
            }
            else
            {
                if (ScreenFormat.m_bFullScreen == TRUE)
                {
                    if ( m_OptionParam.GetScreenMode() != OptionParam::kScreenMode_Full)
                    {
                        bApply = TRUE;
                    }
                }
                else if (ScreenFormat.m_bMaximizeScreen == true)
                {
                    if (m_OptionParam.GetScreenMode() != OptionParam::kScreenMode_MaxWindow)
                    {
                        bApply = TRUE;
                    }
                }
                else
                {
                     if (m_OptionParam.GetScreenMode() != OptionParam::kScreenMode_Window)
                     {
                         bApply = TRUE;
                     }
                }

                //------------------------------------------------------------------------------ 
                SCREEN_SETTING ss = GAMEOPTION->GetAvailableScreenSetting(m_OptionParam.GetScreenIndex());
                if ((ss.ScreenSetting.m_iWidth != ScreenFormat.m_dwWidth) ||
                    (ss.ScreenSetting.m_iHeight != ScreenFormat.m_dwHeight))
                {
                    bApply = TRUE;
                }
            }
#endif //_YMS_WINDOW_MAXSCREEN
		}
		else
		{
			bApply = TRUE;
		}

		if( bApply )
		{
            GAMEOPTION->ApplyScreenOption();
        }

		// 텍스쳐 퀄리티~
		bApply = FALSE;
		if( bCheckApply )
		{
			if( m_OptionParam.GetTextureQuality() != m_InitOptionParam.GetTextureQuality() )
			{
                GAMEOPTION->ApplyOptionValue( nOptionType::TEXTURE_QUALITY, m_OptionParam.GetTextureQuality() );
			}
		}
		else
		{
            GAMEOPTION->ApplyOptionValue( nOptionType::TEXTURE_QUALITY, m_OptionParam.GetTextureQuality() );
		}
	}

	// 그림자 퀄리티
	St_RenderOption Option;

	ENUM_SHADOWMAP_MODE eType = g_pSunRenderer->x_pManagerShadowMap->ConfirmDevice();
	ENUM_SHADOW_TYPE eTypeToSet;
	// 그림자 퀄리티
	if( m_OptionParam.IsRenderShadow()) 
        eTypeToSet = SWT_SHADOWMAP_NO_SELFSHADOW;
	else 
        eTypeToSet = SWT_DISABLED;

	switch (eType)
	{
		// 최고사양임 어떠한것도 필요없음
	case ESM_DEPTHSTENCIL:
	case ESM_R32F:
		break;

		// 프로젝션 쉐도우밖에 지원안함
	case ESM_PROTECTED_TEXTURE:
		{
			// 쉐도우맵을 버퍼로 변환
			if (eTypeToSet == SWT_SHADOWMAP_NO_SELFSHADOW || eTypeToSet == SWT_SHADOWMAP)
			{
				eTypeToSet = SWT_CIRCLESHADOW;
			}
		}
		break;

		// 다 지원안함.
	case ESM_NONE:
		{
			switch (m_OptionParam.GetShadowQuality())
			{
			case eOPTION_LEVEL_HIGH:			
			case eOPTION_LEVEL_MIDDLE:				
				eTypeToSet = SWT_CIRCLESHADOW;
				break;

			case eOPTION_LEVEL_LOW:
			default:
				eTypeToSet = SWT_DISABLED;
				break;

			}
		}
		break;
	}


	g_pSunRenderer->GetRenderOption(&Option);
	Option.m_eShadowType = eTypeToSet;
    Option.m_bRenderShadow = m_OptionParam.IsRenderShadow();

	g_pSunRenderer->SetRenderOption(Option);
    g_pSunRenderer->x_pManagerShadowMap->ResetShadowMode();

	// LOD
    GameFunc::ApplyLODDistance(m_OptionParam.GetLODLevel());
    GameFunc::ApplyEffectLODDistance(m_OptionParam.GetLODLevel());


	//	블러 효과 끄고 켜기
	g_pMap->EnableBlur( m_OptionParam.IsRenderBlur() );

	//  포그
	g_pSunRenderer->EnableFog(m_OptionParam.IsRenderFog());

	// dof
	if (m_OptionParam.IsUseMapDistanceView())
    {
		g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect(PET_DEPTH_OF_FIELD);
    }
	else
    {
		g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect(PET_DEPTH_OF_FIELD);
    }
	
	if( bProgramInit)
	{
		St_RenderOption Option;
		switch(m_OptionParam.GetTextureQuality())
		{
		case eOPTION_LEVEL_HIGH:
			{
				g_pSunRenderer->GetRenderOption(&Option);
				Option.m_fTextureRate = 1.0f;
				g_pSunRenderer->SetRenderOption(Option);
				g_pSunRenderer->x_pManagerTexture->ResizeAllTexture();
			}
			break;

		case eOPTION_LEVEL_MIDDLE:

			{
				g_pSunRenderer->GetRenderOption(&Option);
				Option.m_fTextureRate = 0.5f;
				g_pSunRenderer->SetRenderOption(Option);
				g_pSunRenderer->x_pManagerTexture->ResizeAllTexture();
			}
			break;

		case eOPTION_LEVEL_LOW:

			{
				g_pSunRenderer->GetRenderOption(&Option);
				Option.m_fTextureRate = 0.25f;
				g_pSunRenderer->SetRenderOption(Option);
				g_pSunRenderer->x_pManagerTexture->ResizeAllTexture();
			}
			break;
		}	//	switch(m_OptionParam.iTextureQuality)
	}

    // Copy
	OptionParam tempOption = GAMEOPTION->GetGameOption();

	if (tempOption.GetEffectQuality() != m_OptionParam.GetEffectQuality())
	{
        GAMEOPTION->ApplyOptionValue( nOptionType::EFFECT_QUALITY, m_OptionParam.GetEffectQuality() );

		if(tempOption.GetEffectQuality() > m_OptionParam.GetEffectQuality())
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
			//6035	이펙트 품질 변경은 맵 이동 후에 적용됩니다. 
			g_InterfaceManager.GetInterfaceString(6035, szMessage);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
		}
	}

    tempOption.SetOptionValue( nOptionType::USE_BLUR,               m_OptionParam.GetOptionValue( nOptionType::USE_BLUR ) );               
    tempOption.SetOptionValue( nOptionType::USE_FOG,                m_OptionParam.GetOptionValue( nOptionType::USE_FOG ) );                
    tempOption.SetOptionValue( nOptionType::USE_MAP_DISTANCE_VIEW,  m_OptionParam.GetOptionValue( nOptionType::USE_MAP_DISTANCE_VIEW ) );  
    tempOption.SetOptionValue( nOptionType::SHOW_SHADOW,            m_OptionParam.GetOptionValue( nOptionType::SHOW_SHADOW ) );            
    tempOption.SetOptionValue( nOptionType::GAMMA_VALUE,            m_OptionParam.GetOptionValue( nOptionType::GAMMA_VALUE ) );            
    tempOption.SetOptionValue( nOptionType::EFFECT_QUALITY,         m_OptionParam.GetOptionValue( nOptionType::EFFECT_QUALITY ) );         
    tempOption.SetOptionValue( nOptionType::NORMAL_MAP_QUALITY,     m_OptionParam.GetOptionValue( nOptionType::NORMAL_MAP_QUALITY ) );     
    tempOption.SetOptionValue( nOptionType::SHADOW_QUALITY,         m_OptionParam.GetOptionValue( nOptionType::SHADOW_QUALITY ) );         
    tempOption.SetOptionValue( nOptionType::TEXTURE_QUALITY,        m_OptionParam.GetOptionValue( nOptionType::TEXTURE_QUALITY ) );        
    tempOption.SetOptionValue( nOptionType::LOD_LEVEL,              m_OptionParam.GetOptionValue( nOptionType::LOD_LEVEL ) );              
    tempOption.SetOptionValue( nOptionType::CULLING_DISTANCE_VALUE, m_OptionParam.GetOptionValue( nOptionType::CULLING_DISTANCE_VALUE ) ); 
    tempOption.SetOptionValue( nOptionType::USE_TILE_BLENDING,      m_OptionParam.GetOptionValue( nOptionType::USE_TILE_BLENDING ) );      
    tempOption.SetOptionValue( nOptionType::USE_VOLUME_LIGHT,       m_OptionParam.GetOptionValue( nOptionType::USE_VOLUME_LIGHT ) );       

    tempOption.SetScreenIndex(m_OptionParam.GetScreenIndex());

	m_InitOptionParam = tempOption;
	GAMEOPTION->SetGameOption( tempOption );
	if( bProgramInit == FALSE )
	{
		GAMEOPTION->Save();
	}

	return TRUE;
}
//
//void OptionDialog::UpdatePosition()
//{
//	//POINT pt = g_pApplication->Get2DModeMapping(); 
//	//
//	//// dialog size
//	//RECT rcDialog;
//	//GetDialogWZ()->GetDlgSize(&rcDialog);
//
//	//long dWidth, dHeight;
//	//dWidth = rcDialog.right / 2;
//	//dHeight = rcDialog.bottom / 2;
//
//
//	////	화면 가운데 설정
//	//MoveWindowWZ( (float)((pt.x / 2) - dWidth), (float)((pt.y / 2) - dHeight) );
//
//
//}

#endif
#pragma warning ( pop )
