#include "SunClientPrecompiledHeader.h"
#include "gamefunc.h"
#include "Application.h"
#include "RadarInfoParser.h"
#include "RadarFullDialog.h"
#include "ResourceManager.h"
#include "map.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "Map.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "NPC.h"
#include "Mouse.h"
#include "HeroActionInput.h"
#include "HeadMeshInfoParser.h"
#include "uiPartyMan/uiPartyMan.h"
#include "CursorChangeTransact.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "GlobalFunc.h"
#include "uiRadarMan/uiRadarMan.h"
#include "BattlezoneMissionTreeParser.h"
#include "GameFramework.h"
#include "RadarFullMissionInfo.h"

#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
#include "HotKeyManager.h"
#include "Input/KeyCommands.h"
#include "input/HotKeyInfoParser.h"
#endif

WzID RadarFullDialog::m_wzId[RADAR_FULL_MAX] = 
{ 
	StrToWzID("PI00"),//RADAR_FULL_MAP_IMG,
		StrToWzID("PI01"),//RADAR_FULL_FRAME,
		StrToWzID("B999"),//RADAR_FULL_EXIT,
		StrToWzID("T001"),//RADAR_FULL_MAP_NAME,
		StrToWzID("PI02"), //RADAR_FULL_ALPHA_MAP,
		StrToWzID("ST00"), //RADAR_FULL_TEXT,
		StrToWzID("S000"), //RADAR_FULL_TEXT2,
		StrToWzID("C000"), //RADAR_FULL_MISSIONHELP,
		StrToWzID("S001"), //RADAR_FULL_MISSIONTEXT,
        StrToWzID("B000"), //RADAR_FULL_UNEXTEND,
        StrToWzID("B001"), //RADAR_FULL_EXTEND,

        StrToWzID("P000"), //RADAR_FULL_UNEXTEND,
        StrToWzID("P001"), //RADAR_FULL_EXTEND,
};					  


RadarFullDialog::RadarFullDialog(InterfaceManager *pUIMan):
	BaseRadarDialog(pUIMan)
	
	{
#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
		showMissionController_ = false;
		showMissionInfo_ = TRUE;
#endif //#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
	}

	RadarFullDialog::~RadarFullDialog(void)
	{
	}


	VOID RadarFullDialog::Release()
	{
	}

	VOID RadarFullDialog::Init( CDrawBase * pDrawBase )
	{
		BaseRadarDialog::Init(pDrawBase);

		/**
		사이즈 가변형에 적합하도록 레이더 위치 우측으로 정렬
		*/

		//MissionInfo_.InitMissionData("Data\\MissionHelp.txt");
		// window size
#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
		m_listMissionHelper_.clear();
#endif //#ifdef _NA_000000_20120911_FULL_MISSION_HELPER

		RECT* rcWindow = g_InterfaceManager.GetWindowRC();

		long wWidth, wHeight;
		POINT	*pScreenSize = g_pApplication->GetScreenSize();

		wWidth = rcWindow->right;
		wHeight = rcWindow->bottom;

		// dialog size
		RECT rcDialog;
		GetDialogWZ()->GetDlgSize(&rcDialog);

		long dWidth, dHeight;
		dWidth = rcDialog.right;
		dHeight = rcDialog.bottom;

		// 상대적인 레이더 위치설정
		MoveWindowWZ( (float)((wWidth >> 1) - (dWidth >> 1)), (float)((wHeight >> 1) - (dHeight >> 1)) );

		/// 레이더 컨트롤의 크기
		CControlWZ* pCtrl = getControl(RADAR_FULL_MAP_IMG);
#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
		m_CheckBoxCtrl = getControl(RADAR_FULL_CHECKSHOWMISSION);
#endif //_NA_000000_20120911_FULL_MISSION_HELPER


		if (pCtrl != NULL)
		{
#ifdef _NA_000000_20130422_SCALE_FULLMAP
            pCtrl->SetDiscardMessage(true);
#endif

			Offsettops_ = (WORD)pCtrl->GetSize().top;
			Offsetlefts_ = (WORD)pCtrl->GetSize().left;

			m_wRadarImageWidth	= (WORD)pCtrl->GetSize().right;
			m_wRadarImageHeight = (WORD)pCtrl->GetSize().bottom;
			pCtrl->SetCallbackDraw(DrawControlImage, this);
#ifdef _NA_000000_20130422_SCALE_FULLMAP
            RECT rc;
            GetDialogWZ()->GetDlgSize(&rc);
            rc.left = 0;
            rc.top = 0;

            GetDialogWZ()->SetMoveRECT(rc);
#endif
  
		}
		else
		{
			m_wRadarImageWidth = 1;
			m_wRadarImageHeight = 1;
		}
#ifdef _NA_000000_20130422_SCALE_FULLMAP
        pCtrl = getControl(RADAR_FULL_FRAME);
        if(pCtrl)
        {
            pCtrl->SetDiscardMessage(true);
        }

        pCtrl = getControl(RADAR_FULL_ALPHA_MAP);
        if(pCtrl)
        {
            pCtrl->SetDiscardMessage(true);
        }

        GetDialogWZ()->SetType(DLG_PMOVE);

        pCtrl = getControl(RADAR_FULL_EXTEND);
        if(pCtrl)
        {
            pCtrl->ShowWindowWZ(WZ_HIDE);
        }
#endif

		m_btRadarState = eFULL_RADAR_STATE_MAX;        
	}

	VOID RadarFullDialog::updateMap()
	{
#ifdef _NA_000000_20130422_SCALE_FULLMAP
#else
		MoveCenter();
#endif	
    }


	VOID RadarFullDialog::OnUpdateSolarDialog()
	{
#ifdef _NA_000000_20130422_SCALE_FULLMAP
#else
		updateMap();
#endif
	}

	VOID RadarFullDialog::MessageProc( SI_MESSAGE * pMessage )
	{
        if(pMessage->lnReceiveMsg == 256)
        {
            return;
        }

		switch( pMessage->eResultMsg ) 
		{
        case RT_MSG_RBUTTONCLICK:
            {
                CheckClickedPosition();
            }
            break;
		case RT_MSG_LBUTTONCLICK:
			{
#ifdef _NA_0_20111013_RADAR_PATHFINDING
				if( PathFindMove( getControl( RADAR_FULL_MAP_IMG ) ) == true )
				{
					IncreaseRadarState();
					IncreaseRadarState();

					OnShowWindow(false);
					break;
				}
#endif//_NA_0_20111013_RADAR_PATHFINDING

				switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
				{
                case RADAR_FULL_UNEXTEND:
                    {
#ifdef  _NA_000000_20130422_SCALE_FULLMAP
                        m_bResizefullmap = true;
                        
                        GetDialogWZ()->SetScale(0.5f,0.5f);
                        
                        doActionSize();

                        RECT rc;
                        GetDialogWZ()->GetDlgSize(&rc);
                        rc.left = 0;
                        rc.top = 0;

                        GetDialogWZ()->SetMoveRECT(rc);

                        MoveCenter();
                        GetDialogWZ()->OnFocusOut();
#endif
                    }
                    break;
                case RADAR_FULL_EXTEND:
                    {
#ifdef  _NA_000000_20130422_SCALE_FULLMAP
                        m_bResizefullmap = false;
                       
                        GetDialogWZ()->SetScale(1.0f,1.0f);
                        
                        doActionSize();

                        RECT rc;
                        GetDialogWZ()->GetDlgSize(&rc);
                        
                        rc.left = 0;
                        rc.top = 0;

                        GetDialogWZ()->SetMoveRECT(rc);

                        MoveCenter();
                        GetDialogWZ()->OnFocusOut();
#endif
                    }
                    break;
				case RADAR_FULL_EXIT:
					{
						ResetRadarState();
						ShowDialog(FALSE);
					}
					break;
#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
				case RADAR_FULL_CHECKSHOWMISSION:
					{
						CCtrlButtonCheckWZ* checkBoxFullMap = static_cast<CCtrlButtonCheckWZ *>(m_CheckBoxCtrl);
						showMissionInfo_ = checkBoxFullMap->GetCheckState();
					}
					break;
#endif //_NA_000000_20120911_FULL_MISSION_HELPER
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

	VOID RadarFullDialog::NetworkProc( MSG_BASE * pMsg )
	{
	}

	POSTYPE RadarFullDialog::getCtrlIDToPos( WzID wzId )
	{
		for( POSTYPE i = 0 ; i < RADAR_FULL_MAX ; ++i )
		{
			if( m_wzId[i] == wzId )
			{
				return i;
			}
		}
		return RADAR_FULL_MAX;
	}

	CControlWZ * RadarFullDialog::getControl( POSTYPE AtPos )
	{
		assert( AtPos < RADAR_FULL_MAX );

		return GetControlWZ( m_wzId[AtPos] );
	}

	BOOL RadarFullDialog::IsVisibleWindow()
	{
		return GetDialogWZ()->IsState( WZ_SHOW );
	}

	VOID RadarFullDialog::OnShowWindow( BOOL val )
	{
		if( val == TRUE )
		{
			ShowMapImageControl();
		}
#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
		if(showMissionController_ == false)
		{
			CCtrlButtonCheckWZ* checkBoxFullMap = static_cast<CCtrlButtonCheckWZ *>(m_CheckBoxCtrl);
			if(checkBoxFullMap)
			{
				checkBoxFullMap->ShowWindowWZ(WZ_HIDE);
			}

			CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(getControl(RADAR_FULL_TEXT3));
			if (static_control)
			{
				static_control->ShowWindowWZ(WZ_HIDE);
			}
		}
#ifdef  _NA_000000_20130422_SCALE_FULLMAP
        else
        {
            CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(getControl(RADAR_FULL_TEXT3));
            if (static_control)
            {
                if (m_bResizefullmap)
                {
                    static_control->ShowWindowWZ(WZ_HIDE);
                }
                else
                {
                    CCtrlStaticWZ* frame_control = static_cast<CCtrlStaticWZ*>(getControl(RADAR_FULL_FRAME));
                    if (frame_control)
                    {
                        static_control->ShowWindowWZ(frame_control->IsState(WZ_HIDE) ? WZ_HIDE : WZ_SHOW);
                    }
                }
            }            
        }
#endif // _NA_000000_20130422_SCALE_FULLMAP
        
#endif //#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
		ENUM_STATEWZ v = ( val == TRUE ? WZ_SHOW : WZ_HIDE );

		GetDialogWZ()->ShowWindowWZ(v);

		if (g_pHero)
		{
			if (m_btRadarState == eFULL_RADAR_STATE_ALL)
			{
				g_pHero->ShowFullMapWindow(val);
			}
			else
			{
				g_pHero->ShowFullMapWindow(FALSE);
			}
		}
#if WZENGINEVER >= 590
        if( v == WZ_SHOW && m_btRadarState == eFULL_RADAR_STATE_ALPHA )
        {
            CDialogManager* pDialogManager = g_InterfaceManager.GetDialogManager();
            pDialogManager->MoveHead( GetDialogKey() );
        }
#endif // WZENGINEVER >= 590

		//	오른쪽 하단 메인 메뉴 닫기
		if( g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_MAIN_OPTION) )
		{
			g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_MAIN_OPTION, FALSE);
		}

		if(val)
		{
            if(m_btRadarState != eFULL_RADAR_STATE_ALPHA)
            {
                if(m_bResizefullmap)
                {
                    CControlWZ* pCtrl = getControl(RADAR_FULL_TEXT2);
                    if(pCtrl)
                    {
                        pCtrl->ShowWindowWZ(WZ_HIDE);
                    }
                    pCtrl = getControl(RADAR_FULL_TEXT);
                    if(pCtrl)
                    {
                        pCtrl->ShowWindowWZ(WZ_HIDE);
                    }

                    pCtrl = getControl(RADAR_FULL_EXTEND);
                    if(pCtrl)
                    {
                        pCtrl->ShowWindowWZ(WZ_SHOW);
                    }
                    pCtrl = getControl(RADAR_FULL_UNEXTEND);
                    if(pCtrl)
                    {
                        pCtrl->ShowWindowWZ(WZ_HIDE);
                    }
                }
                else
                {
                    CControlWZ* pCtrl = getControl(RADAR_FULL_TEXT2);
                    if(pCtrl)
                    {
                        pCtrl->ShowWindowWZ(WZ_SHOW);
                    }
                    pCtrl = getControl(RADAR_FULL_TEXT);
                    if(pCtrl)
                    {
                        pCtrl->ShowWindowWZ(WZ_SHOW);
                    }
                    pCtrl = getControl(RADAR_FULL_EXTEND);
                    if(pCtrl)
                    {
                        pCtrl->ShowWindowWZ(WZ_HIDE);
                    }
                    pCtrl = getControl(RADAR_FULL_UNEXTEND);
                    if(pCtrl)
                    {
                        pCtrl->ShowWindowWZ(WZ_SHOW);
                    }
                }
            }
           

			keyMsg msg;
			ZeroMemory(&msg,sizeof(msg));
			msg.dwType=eDoType_dialog_radar_full;
			msg.wParam=InterfaceManager::DIALOG_RADAR_FULL;
			msg.DoSomething=GlobalFunc::DoRadarFullDialog;
			g_KeyQueueManager.PushBack(msg);

			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
		}
		else
		{

			g_KeyQueueManager.DeleteMsg(eDoType_dialog_radar_full);

		}
	}


	void RadarFullDialog::OnRenderSolarDialog()
	{
		CCtrlPictureWZ * pPicture = (CCtrlPictureWZ *)getControl( RADAR_FULL_MAP_IMG );
		if ( pPicture)
		{
			Wrap_render(pPicture);


#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
			if(m_listMissionHelper_.size() && showMissionInfo_ == TRUE)
			{
				for (int i = 0 ; i < (int)m_listMissionHelper_.size() ; ++i)
				{
					bool     IsInMouse = false;

					uiRadarMan* ui_radar_manager = static_cast<uiRadarMan*>(GetMan());

					int			iMouseX = Mouse::Instance()->GetMouseX();
					int			iMouseY = Mouse::Instance()->GetMouseY();

					uiRadarMapData& map_data = ui_radar_manager->radar_map_data();

					float hero_position_x;
					float hero_position_y;

                    float scaleX = 1.0f;
                    float scaleY = 1.0f;
                    if(m_bResizefullmap)
                    {
                        scaleX = 0.5f;
                        scaleY = 0.5f;
                    }
					
                    hero_position_x = -((map_data.map_image_width()*scaleX) / 2.0f - (GetRadarImageWidth()*scaleX));
                    hero_position_y = -((map_data.map_image_height()*scaleY) / 2.0f - (GetRadarImageHeight()*scaleY));

					RECT Rect = RadarFullMissionInfoParser::Instance()->GetInputReturnRealSize(m_listMissionHelper_[i]->IconID_,FALSE);;
					
					RECT rect = pPicture->GetSizeRect();
					float		fImageCtrlCenterX , fImageCtrlCenterY;
					fImageCtrlCenterX = (float)rect.left + (float)(rect.right - rect.left) * 0.5f;
					fImageCtrlCenterY = (float)rect.top + (float)(rect.bottom - rect.top) * 0.5f;

					float fPointSizeX = ((float)Rect.right * 0.5f);
					float fPointSizeY	= ((float)Rect.bottom * 0.5f);

					WzVector pos = CalculatePointPosition(m_listMissionHelper_[i]->IconPosX,m_listMissionHelper_[i]->IconPosY, m_listMissionHelper_[i]->IconPosZ);

					float fIconCenterX = fImageCtrlCenterX + (pos.x*scaleX) - fPointSizeX - hero_position_x;
					float fIconCenterY = fImageCtrlCenterY + (pos.z*scaleY) - fPointSizeY - hero_position_y;

					if( iMouseX > (fIconCenterX) && iMouseX < (fIconCenterX  + (float)Rect.right) && 
						iMouseY > (fIconCenterY ) &&  iMouseY < (fIconCenterY + (float)Rect.bottom) )
					{
						uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
						if(pUIMan)
						{
							const WzID	FontID_8 = StrToWzID("s210");

							DWORD sp_tooltip = eSPECIAL_TOOLTIP_NONE;

							DWORD isInAllColor = GetHexValue(m_listMissionHelper_[i]->color_);
							DWORD Red = ((isInAllColor >> 16) & 0xff);
							DWORD Green = ((isInAllColor >> 8) & 0xff);
							DWORD Blue = (isInAllColor & 0xff);
							DWORD Alpha = ((isInAllColor >> 24) & 0xff);

							const WzColor	NPCTextColor = WzColor_RGBA(Red,Green,Blue,255);

							TCHAR temp_buffer[INTERFACE_STRING_LENGTH] = {'\0', };
							g_InterfaceManager.GetInterfaceString(m_listMissionHelper_[i]->StringID, temp_buffer, INTERFACE_STRING_LENGTH);

							pUIMan->RegisterTooltip(temp_buffer, -1,  22, FontID_8, NPCTextColor, TOOLTIP_BG_WZCOLOR, c_dwTOOLTIP_EXTRA_INFO_DT_CENTER, sp_tooltip);
							pUIMan->SetTooltipDialogID( GetDialogKey() );

						}

						IsInMouse = true;
					}

					WzColor wcRadarPointColor[4];
					for (int a = 0; a < 4; a++)
					{
						if(GetRadarState() == eFULL_RADAR_STATE_ALPHA)
							wcRadarPointColor[a] = WzColor_RGBA(255,255,255,150);
						else
							wcRadarPointColor[a] = WzColor_RGBA(255,255,255,255);
					}

					

					RECT ScrRect = RadarFullMissionInfoParser::Instance()->GetInputReturnRealSize(m_listMissionHelper_[i]->IconID_,IsInMouse);
					GetDrawBase()->RenderTexture( RadarFullMissionInfoParser::Instance()->GetMissionTexture(),
						(float)fIconCenterX,
						(float)fIconCenterY,
						(float)ScrRect.right,
						(float)ScrRect.bottom,
						(float)ScrRect.left,
						(float)ScrRect.top,
						(float)ScrRect.right,
						(float)ScrRect.bottom,
						wcRadarPointColor);

				}

			}
#endif //_NA_000000_20120911_FULL_MISSION_HELPER

		}
	}

	VOID RadarFullDialog::ChangeMap(TCHAR* file_name /*= NULL*/)
	{
#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
		m_listMissionHelper_.clear();
#endif //#ifdef _NA_000000_20120911_FULL_MISSION_HELPER

#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
		showMissionController_ = FALSE;
#endif // _NA_000000_20120911_FULL_MISSION_HELPER


		DWORD FieldID = g_pMap->GetCurMMCode();
		const sFIELDINFO* fieldino = g_pMap->GetFieldInfo();

		if (FieldID != 0)    {
			if( CControlWZ* pCtrl = getControl(RADAR_FULL_MAP_IMG) )
			{
				Wrap_loadMapImage(pCtrl , FieldID, 0, file_name);
			}

			if( CControlWZ* pCtrl = getControl(RADAR_FULL_ALPHA_MAP) )
			{
				Wrap_loadMapImage(pCtrl , FieldID, 1, file_name);
			}
		}    

		CCtrlStaticWZ* pStatic = static_cast<CCtrlStaticWZ*>(getControl(RADAR_FULL_MAP_NAME));
		SetMapName(pStatic, g_pMap->GetFieldName());

		CCtrlPictureWZ* pPicture = static_cast<CCtrlPictureWZ*>(getControl(RADAR_FULL_MAP_IMG));
		CalcPortalOffset(pPicture);

#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
		if ((GameFramework::GetCurScene() == SCENE_TYPE_MISSION)
#ifdef _NA_008334_20150608_SONNENSCHEIN
            || (GameFramework::GetCurScene() == SCENE_TYPE_SONNENSCHEIN) // 소넨샤인
#endif //_NA_008334_20150608_SONNENSCHEIN
            )
		{
			showMissionController_= TRUE;
			CCtrlButtonCheckWZ* checkBoxFullMap = static_cast<CCtrlButtonCheckWZ *>(m_CheckBoxCtrl);
			checkBoxFullMap->SetCheckState(TRUE);

			RadarFullMissionInfoParser::Instance()->FindData(&m_listMissionHelper_,g_pMap->GetMapID(),g_pMap->GetFieldID());
		}
#endif //_NA_000000_20120911_FULL_MISSION_HELPER

	}
	//------------------------------------------------------------------------------
	bool RadarFullDialog::GetPictureSize(RECT& rect)
	{
		CControlWZ* controlwz = getControl(RADAR_FULL_MAP_IMG);
		if (controlwz != NULL)
		{
			rect = controlwz->GetSizeRect();
			return true;
		}

		return false;
	}

	void RadarFullDialog::ShowMapImageControl()
	{
		switch(m_btRadarState)
		{
		case eFULL_RADAR_STATE_ALL:
			{

				for(int i = 0; i < RADAR_FULL_MAX; ++i)
				{
					CControlWZ* pCtrl = getControl(i);
					if(pCtrl)
					{
						if(RADAR_FULL_ALPHA_MAP == i)
							pCtrl->ShowWindowWZ(WZ_HIDE);
						else
							pCtrl->ShowWindowWZ(WZ_SHOW);
					}


				}
			}
			break;
		case eFULL_RADAR_STATE_ALPHA:
			{
				for(int i = 0; i < RADAR_FULL_MAX; ++i)
				{
					GetDialogWZ()->SetTransparency(1.f);

					CControlWZ* pCtrl = getControl(i);
					if(NULL == pCtrl)
						break;

					if(RADAR_FULL_ALPHA_MAP == i)
						pCtrl->ShowWindowWZ(WZ_SHOW);
					else
						pCtrl->ShowWindowWZ(WZ_HIDE);

				}
			}
			break;
		}
	}

	__forceinline DWORD RadarFullDialog::GetHexValue( const TCHAR* Value )
	{
		DWORD sum = 0;
		size_t StringCount = strlen(Value);

		int count = static_cast<int>(StringCount);

		for (size_t index = 0; index < StringCount ; ++index)
		{
			sum +=  GetHexConvertor(Value[index]) * GetHexMulti(--count);
		}

		return sum;
	}

	__forceinline int RadarFullDialog::GetHexMulti( int Value )
	{
		if( Value == 0)
			return 1;

		if(Value >= 1)
			return 16 * GetHexMulti(Value - 1);

		return 0;
	}

	__forceinline int RadarFullDialog::GetHexConvertor( char charset )
	{
		if ('a' <= charset && charset <= 'f') //소문자인가?
			charset = charset - 32; //대문자로 변화

		if ('A' <= charset && charset <= 'F')
			return charset - 'A' + 10; //대문자면 16진수에 해당되는 값으로 변환.

		if ('0' <= charset && charset <= '9') //숫자 문자인가?
			return charset - '0'; //숫자로 변환

		return 0;
	}

    BOOL RadarFullDialog::CheckClickedPosition()
    {
        if (m_btRadarState != eFULL_RADAR_STATE_ALL)
        {
            return FALSE;
        }

        CCtrlPictureWZ* picture = static_cast<CCtrlPictureWZ*>(getControl(RADAR_FULL_MAP_IMG));
        BOOL ret = FALSE;
        if (picture != NULL)
        {
            ret = BaseRadarDialog::Wrap_CheckClickedPosition(picture);
        }
        return ret;
    }
#ifdef _NA_006643_20130417_UI_OUT_WINDOW_BREAK
    void RadarFullDialog::CheckModifyOption()
    {

        KEY_BIND_PAIR_MAP::iterator found_itr = 
            HotKeyManager::Instance()->current_using_key_bind_info().key_bind_pair_map().find(kOpen_ContinentMap_UI);

        KEY_BIND_PAIR temp_key_bind_pair = found_itr->second;

        STLX_STRING hot_key_string;
        DWORD string_code = 0;

        HOT_KEY_CODE hot_ket_code(temp_key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey));
        if (hot_ket_code.key_code_ != 0)
        {
            if (hot_ket_code.ctrl_key_ == 1)
            {
                hot_key_string += g_InterfaceManager.GetInterfaceString(8218);
                hot_key_string += " + ";
            }

            if (hot_ket_code.alt_key_ == 1)
            {
                hot_key_string += g_InterfaceManager.GetInterfaceString(8219);
                hot_key_string += " + ";
            }

            if (hot_ket_code.shift_key_ == 1)
            {
                hot_key_string += g_InterfaceManager.GetInterfaceString(8217);
                hot_key_string += " + ";
            }

            string_code = HotKeyInfoParser::Instance()->GetStringCodeByKeyCode(hot_ket_code.key_code_);

            hot_key_string += g_InterfaceManager.GetInterfaceString(string_code);
        }

        CControlWZ* pCtrl = getControl(RADAR_FULL_TEXT);

        if(pCtrl)
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
            DWORD textID = pCtrl->GetTextID();
            g_InterfaceManager.GetInterfaceStringFormat(message,INTERFACE_STRING_LENGTH,textID,hot_key_string.c_str());
            pCtrl->SetTextWZA(message);
        }
        

    }

    VOID RadarFullDialog::doActionSize()
    {
        CControlWZ* pCtrl = getControl(RADAR_FULL_TEXT2);
        if(pCtrl)
        {
            pCtrl->ShowWindowWZ(m_bResizefullmap ? WZ_HIDE : WZ_SHOW);
        }
        pCtrl = getControl(RADAR_FULL_TEXT);
        if(pCtrl)
        {
            pCtrl->ShowWindowWZ(m_bResizefullmap ? WZ_HIDE : WZ_SHOW);
        }

        pCtrl = getControl(RADAR_FULL_TEXT3);
        if(pCtrl)
        {
            pCtrl->ShowWindowWZ(m_bResizefullmap ? WZ_HIDE : WZ_SHOW);
        }

        if(m_bResizefullmap)
        {
            pCtrl = getControl(RADAR_FULL_EXTEND);
            if(pCtrl)
            {
                RECT base = pCtrl->GetSizeRect();
                RECT Origin = pCtrl->GetOriginRect();
                pCtrl->ShowWindowWZ(WZ_SHOW);

                pCtrl->MoveWindowWZ((float)base.left+2,(float)base.top+2,
                    (float)Origin.right,(float)Origin.bottom);
            }
            pCtrl = getControl(RADAR_FULL_UNEXTEND);
            if(pCtrl)
            {
                pCtrl->ShowWindowWZ(WZ_HIDE);

                RECT base = pCtrl->GetSizeRect();
                RECT Origin = pCtrl->GetOriginRect();
                pCtrl->MoveWindowWZ((float)base.left,(float)base.top,
                    (float)Origin.right,(float)Origin.bottom);
            }
            pCtrl = getControl(RADAR_FULL_EXIT);
            if(pCtrl)
            {
                RECT base = pCtrl->GetSizeRect();
                RECT Origin = pCtrl->GetOriginRect();
                pCtrl->MoveWindowWZ((float)base.left-10,(float)base.top+2,
                    (float)Origin.right,(float)Origin.bottom);
            }

            pCtrl = getControl(RADAR_BASE_IMAGE_P000);
            if(pCtrl)
            {
                RECT base = pCtrl->GetSizeRect();
                RECT Origin = pCtrl->GetOriginRect();
                pCtrl->MoveWindowWZ((float)base.left,(float)base.top,
                    (float)Origin.right,(float)Origin.bottom);
            }

            pCtrl = getControl(RADAR_BASE_IMAGE_P001);
            if(pCtrl)
            {
                RECT base = pCtrl->GetSizeRect();
                RECT Origin = pCtrl->GetOriginRect();

                pCtrl->MoveWindowWZ((float)base.left - 12,(float)base.top,
                    (float)Origin.right,(float)Origin.bottom);
            }

            pCtrl = getControl(RADAR_FULL_CHECKSHOWMISSION);
            if(pCtrl)
            {
                RECT base = pCtrl->GetSizeRect();
                RECT Origin = pCtrl->GetOriginRect();

                pCtrl->MoveWindowWZ((float)base.left-4,(float)base.top-2,
                    (float)Origin.right,(float)Origin.bottom);
            }

            

        }
        else
        {
            pCtrl = getControl(RADAR_FULL_EXTEND);
            if(pCtrl)
            {
                pCtrl->ShowWindowWZ(WZ_HIDE);

            }
            pCtrl = getControl(RADAR_FULL_UNEXTEND);
            if(pCtrl)
            {
                pCtrl->ShowWindowWZ(WZ_SHOW);
            }
            pCtrl = getControl(RADAR_FULL_TEXT3);
            if(pCtrl)
            {
                pCtrl->ShowWindowWZ(showMissionController_ ? WZ_SHOW : WZ_HIDE);
            }            
        }
        


    }
#endif