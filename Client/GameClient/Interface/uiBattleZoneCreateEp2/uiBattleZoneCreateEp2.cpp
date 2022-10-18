//------------------------------------------------------------------------------
//  uiBattleZoneCreate_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiBattleZoneCreateEp2.h"
#include "HeroActionInput.h"
#include "Map.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "uiPartyMan/uiPartyMan.h"
#include "Application.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Mouse.h"

#include "uiToolTipMan/uiToolTipMan.h"
#include "ChatDialog.h"
#include "uiChatMan/uiChatMan_def.h"

#include "MapInfoParser.h"
#include "RequireFieldLevelInfoParser.h"
#include "hero.h"
#include "ItemManager.h"
#include "GameConst_Color.h"

//------------------------------------------------------------------------------
WzID uiBattleZoneCreateEp2::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("B999"),  // DLGPOS_BTN_EXIT=0,
    StrToWzID("TI02"),  // DLGPOS_BTN_CREATE,
    StrToWzID("TI03"),  // DLGPOS_BTN_CANCEL,
    StrToWzID("B000"),  // DLGPOS_BTN_PEOPLE_LIMIT,
    StrToWzID("B001"),  // DLGPOS_BTN_ROOM_TYPE,

    StrToWzID("C001"),  // DLGPOS_CHKBTN_CLASS_B,  // 버서커
    StrToWzID("C002"),  // DLGPOS_CHKBTN_CLASS_D,  // 드래곤 나이트
    StrToWzID("C003"),  // DLGPOS_CHKBTN_CLASS_V,  // 발키리
    StrToWzID("C004"),  // DLGPOS_CHKBTN_CLASS_E,  // 엘리멘탈 리스트
	StrToWzID("C005"),	// DLGPOS_CHKBTN_CLASS_S,  // 쉐도우

    StrToWzID("C008"),  //  DLGPOS_CHKBTN_MAP_THE_EARTH_OF_TREES_SHADOW,	// 녹음의 대지
    StrToWzID("C007"),  //  DLGPOS_CHKBTN_MAP_REST_AREA_OF_SPIRITS,			// 정령들의 쉼터로
    StrToWzID("C000"),  //  DLGPOS_CHKBTN_MAP_BOALESS_MOUNTAIN_CHAIN,		// 보아레스 산맥
    StrToWzID("C006"),  //  DLGPOS_CHKBTN_MAP_VALLY_OF_LIFE,	            // 생명의 골짜기

    StrToWzID("T009"),  // DLGPOS_TXT_PEOPLE_LIMIT,
    StrToWzID("T011"),  // DLGPOS_TXT_ROOM_TYPE,
    StrToWzID("T012"),  // DLGPOS_TXT_ROOM_TYPE_DESC,
    StrToWzID("T005"),  // DLGPOS_TXT_MIN_LIMIT_LEVEL,
    StrToWzID("T006"),  // DLGPOS_TXT_MAX_LIMIT_LEVEL,

    StrToWzID("E011"),  // DLGPOS_EDT_MIN_LEVEL,
    StrToWzID("E012"),  // DLGPOS_EDT_MAX_LEVEL,
    StrToWzID("E001"),  // DLGPOS_EDT_TITLE,
    StrToWzID("E002"),  // DLGPOS_EDT_PASSWORD,

    StrToWzID("L001"),  // DLGPOS_LST_ROOM_TYPE,
    StrToWzID("L002"),  // DLGPOS_LST_PEOPLE_LIMIT

	StrToWzID("TI00"),	// DLGPOS_TI_SUPER_ROOM_NAME
	StrToWzID("BT00"),	// DLGPOS_BTN_SUPER_ROOM_SELECT
	StrToWzID("LS00"),	// DLGPOS_LIST_SUPER_ROOM

	StrToWzID("P001"),	//DLGPOS_CHKBTN_MAPALL,

	StrToWzID("CT20"),//DLGPOS_CHKBTN_MAP_0,
	StrToWzID("CT19"),//DLGPOS_CHKBTN_MAP_1,
	StrToWzID("CT17"),//DLGPOS_CHKBTN_MAP_2,
	StrToWzID("CT16"),//DLGPOS_CHKBTN_MAP_3,
	StrToWzID("CT14"),//DLGPOS_CHKBTN_MAP_4,

	StrToWzID("CT13"),//DLGPOS_CHKBTN_MAP_5,
	StrToWzID("CT11"),//DLGPOS_CHKBTN_MAP_6,
	StrToWzID("CT12"),//DLGPOS_CHKBTN_MAP_7,
	StrToWzID("CT10"),//DLGPOS_CHKBTN_MAP_8,
	StrToWzID("CT06"),//DLGPOS_CHKBTN_MAP_9,

	StrToWzID("CT18"),//DLGPOS_CHKBTN_MAP_10,
	StrToWzID("CT08"),//DLGPOS_CHKBTN_MAP_11,
	StrToWzID("CT07"),//DLGPOS_CHKBTN_MAP_12,
	StrToWzID("CT05"),//DLGPOS_CHKBTN_MAP_13,
	StrToWzID("CT04"),//DLGPOS_CHKBTN_MAP_14,
	StrToWzID("CT03"),//DLGPOS_CHKBTN_MAP_15,

	StrToWzID("CT02"),//DLGPOS_CHKBTN_MAP_16,
	StrToWzID("CT01"),//DLGPOS_CHKBTN_MAP_17,
	StrToWzID("CT00"),//DLGPOS_CHKBTN_MAP_18,
	StrToWzID("M001"),//DLGPOS_CHKBTN_MAP_19,
};

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneCreateEp2::uiBattleZoneCreateEp2(InterfaceManager *pUIMan) :
uiBase(pUIMan),
    m_bInitControls(false)
{
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneCreateEp2::~uiBattleZoneCreateEp2()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
    uiBattleZoneCreateEp2::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
    uiBattleZoneCreateEp2::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void
    uiBattleZoneCreateEp2::InitControls()
{
    int i;

    for (i=0; i<NUM_POS_BTN; ++i)
    {
        m_pBtnControls[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(POS_BTN_BEGIN + i));
        assert (m_pBtnControls[i]);
    }

    for (i=0; i<NUM_POS_CLASS_CHKBTN; ++i)
    {
        m_pChkBtnClassControls[i] =
            static_cast<CCtrlButtonCheckWZ *>(this->getControl(POS_CHKBTN_CLASS_BEGIN + i));
        assert (m_pChkBtnClassControls[i]);
    }

    m_pChkBtnMapAll = static_cast<CCtrlButtonCheckWZ *>(this->getControl(DLGPOS_CHKBTN_MAPALL));
    assert (m_pChkBtnMapAll);

    for (i=0; i<NUM_POS_MAP_CHKBTN; ++i)
    {
        m_pChkBtnMapControls[i] =
            static_cast<CCtrlButtonCheckWZ *>(this->getControl(POS_CHKBTN_MAP_BEGIN + i));
        assert (m_pChkBtnMapControls[i]);
    }

    for (i=0; i<NUM_POS_TXT; ++i)
    {
        m_pTxtControls[i] =
            static_cast<CCtrlStaticWZ *>(this->getControl(POS_TXT_BEGIN + i));
        assert (m_pTxtControls[i]);
        GlobalFunc::SetCaption (m_pTxtControls[i], _T(""));
    }

    for (i=0; i<NUM_POS_EDT; ++i)
    {
        m_pEdtControls[i] =
            static_cast<CCtrlDxIMEEditBox *>(this->getControl(POS_EDT_BEGIN + i));
        assert (m_pEdtControls[i]);
    }

    for (i=0; i<NUM_POS_LST; ++i)
    {
        m_pLstControls[i] =
            static_cast<CCtrlListWZ *>(this->getControl(POS_LST_BEGIN + i));
        assert (m_pLstControls[i]);
    }

    m_pListSuperMaster = static_cast<CCtrlListWZ *>( getControl(DLGPOS_LIST_SUPER_ROOM) );
    assert(m_pListSuperMaster);


    m_pButtonSuperMaster = static_cast<CCtrlButtonWZ *>(getControl(DLGPOS_BTN_SUPER_ROOM_SELECT) );
    assert(m_pButtonSuperMaster);

    m_pTextSuperMaster = static_cast<CCtrlImageStaticWZ *> ( getControl( DLGPOS_TI_SUPER_ROOM_NAME ) );
    assert(m_pTextSuperMaster);
    m_pTextSuperMaster->SetFontID(StrToWzID("mn12"));

    m_bInitControls = true;
}

//------------------------------------------------------------------------------
/**
called to show request
*/
void uiBattleZoneCreateEp2::OnShowWindow( BOOL val )
{
	if (val)
    {
        if (!this->CheckControls())
        {
            ShowInterface(FALSE);
            return;
        }

        this->Clear();
		SetDefaultSuperMasterList();
        //UpdatePosition();

        uiPartyMan* partyMan =
            static_cast<uiPartyMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_PARTY));
        if (partyMan)
            partyMan->uiClearAll();

		GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST )->ShowInterface(FALSE);
        
        m_bCheckDistance_NPCMEET = false;

        // TODO: 이거 제대로 동작하는가?
        g_HeroInput.ClearPickingTarget();

        // 베틀 존 목록 UI 활성화 할 경우 맵 렌더링을 OFF
        if (g_pMap)
            g_pMap->SetRenderFlag(FALSE);

        //
        // support widemode
        //
        RECT rcDlgSize;
        this->GetDialogWZ()->GetDlgSize(&rcDlgSize);

        POINT pt = g_pApplication->Get2DModeMapping();

        RECT rcInterface;
        rcInterface.left = rcInterface.top = 0;
        rcInterface.right = pt.x;
        rcInterface.bottom = pt.y;

        int x = (rcInterface.right / 2) - (rcDlgSize.right / 2);
        int y = (rcInterface.bottom / 2) - (rcDlgSize.bottom / 2);

        MoveWindowWZ( (float)x, (float)y );

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_dialog_battle_create;
        msg.wParam=InterfaceManager::DIALOG_BATTLEZONE_CREATE;
        msg.DoSomething=GlobalFunc::DoBattleCreate;
        g_KeyQueueManager.PushBack(msg);

		m_pListSuperMaster->ShowWindowWZ(WZ_HIDE);

        ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
        if (pChatDlg)
        {
            RECT rcClient;
            this->GetDialogWZ()->GetClientRect(&rcClient);
            pChatDlg->MoveBattleZonePosition(rcClient);
            pChatDlg->ShowWindow(FALSE);
        }
    }
    else
    {
        uiPartyMan* partyMan =
			static_cast<uiPartyMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_PARTY));
        if (partyMan)
            partyMan->uiShowAll();

        // 맵 렌더링 On
        if (g_pMap)
        {
            g_pMap->SetRenderFlag(TRUE);
        }

        g_KeyQueueManager.DeleteMsg(eDoType_dialog_battle_create);

		this->GetUIMan()->ShowCenterMessageBox( NULL, 1, NULL, FALSE );
		
    }
}

//------------------------------------------------------------------------------
/**
*/
//void     
//uiBattleZoneCreate::UpdatePosition()
//{
//    //MoveWindowWZ(748, 180);
//}
//=====================================================================================
//	DESC: 컨트롤러 인덱스통해 해당맵정보를 리턴한다.
//=====================================================================================
POSTYPE
uiBattleZoneCreateEp2::getMapAreaInfo(int nPos)
{
	POSTYPE MapArea = eCHKMAP_EP2_MAX;
	switch (nPos)
	{
	case DLGPOS_CHKBTN_MAP_0: // 녹음의 대지
	case DLGPOS_CHKBTN_MAP_1:
	case DLGPOS_CHKBTN_MAP_2:
	case DLGPOS_CHKBTN_MAP_3: 
    case DLGPOS_CHKBTN_MAP_4:
        MapArea = eCHKMAP_THE_EARTH_OF_TREES_SHADOW;break;

	case DLGPOS_CHKBTN_MAP_5: // 정령들의 쉼터
	case DLGPOS_CHKBTN_MAP_6:
	case DLGPOS_CHKBTN_MAP_7: 
	case DLGPOS_CHKBTN_MAP_8: 
	case DLGPOS_CHKBTN_MAP_9:
        MapArea = eCHKMAP_REST_AREA_OF_SPIRITS;break;

	case DLGPOS_CHKBTN_MAP_10: // 보아레스 산맥
	case DLGPOS_CHKBTN_MAP_11:
	case DLGPOS_CHKBTN_MAP_12:
	case DLGPOS_CHKBTN_MAP_13:
	case DLGPOS_CHKBTN_MAP_14:
	case DLGPOS_CHKBTN_MAP_15:
        MapArea = eCHKMAP_BOALESS_MOUNTAIN_CHAIN;break;

	case DLGPOS_CHKBTN_MAP_16: // 생명의 골짜기
	case DLGPOS_CHKBTN_MAP_17:
	case DLGPOS_CHKBTN_MAP_18:
	case DLGPOS_CHKBTN_MAP_19: 
        MapArea = eCHKMAP_VALLY_OF_LIFE;break;
	}

	return MapArea; 
}
//=====================================================================================
//	DESC: 배틀존 생성창에서 해당지역 클릭시 그지역을 활성화시킨다.
//=====================================================================================
void
uiBattleZoneCreateEp2::clickMapArea(POSTYPE pos)
{
	POSTYPE MapArea = getMapAreaInfo(pos);

	if(MapArea==eCHKMAP_EP2_MAX)//맵체크버튼이 아니다
		return;

	bool canChoice = true;
	
	if (!setMapType(MapArea))
	{
		canChoice = false;

		for (int i=eCHKMAP_THE_EARTH_OF_TREES_SHADOW; i<eCHKMAP_EP2_MAX; ++i)
		{
			if (setMapType(i))
			{
				canChoice = true;
				break;
			}
		}
	}

	if (!canChoice)
	{
		m_cfgs.mapType = eCHKMAP_EP2_MAX;
	}
}

//=====================================================================================
//	DESC: 배틀존 맵영역창에 있을때는 툴팁스트링을 제거해준다.
//=====================================================================================
void
uiBattleZoneCreateEp2::checkMapMouseOver()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	RECT rc;
	rc = m_pChkBtnMapAll->GetSizeRect();

	BOOL bMouseOver = FALSE;
	if (rc.left <= iMouseX && rc.right >= iMouseX)
	{
		if (rc.top <= iMouseY && rc.bottom >= iMouseY)
		{
			bMouseOver =TRUE;
		}
	}

	if(bMouseOver==FALSE)
		m_vMapDesc.clear();

}
//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    switch( pMessage->eResultMsg ) 
    {
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			refreshToolTip(getMapAreaInfo(pos));
		}
		break;

    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			
			clickMapArea(pos);//해당지역활성화

            switch( pos )
            {
			case DLGPOS_LST_ROOM_TYPE:	
				{
					int iSel = m_pLstControls[0]->GetCurSel();
					if(iSel >= 0)
					{
						setDefaultRoomName(iSel);
						m_pLstControls[0]->ShowWindowWZ(WZ_HIDE);
					}
				}
				break;


            case DLGPOS_CHKBTN_CLASS_B:
            case DLGPOS_CHKBTN_CLASS_D:
            case DLGPOS_CHKBTN_CLASS_E:
            case DLGPOS_CHKBTN_CLASS_V:
			case DLGPOS_CHKBTN_CLASS_S:
                {
                    this->setClassType(pos);
                }
                break;

            case DLGPOS_BTN_ROOM_TYPE:
                {
                    if (m_pLstControls[0]->GetState() == WZ_SHOW)
                        m_pLstControls[0]->ShowWindowWZ(WZ_HIDE);
                    else
                    {
                        m_pLstControls[0]->ShowWindowWZ(WZ_SHOW);
                        m_pLstControls[0]->SetCurSel(-1);
                    }
                }
                break;

            case DLGPOS_BTN_PEOPLE_LIMIT:
                {
                    if (m_pLstControls[1]->GetState() == WZ_SHOW)
                        m_pLstControls[1]->ShowWindowWZ(WZ_HIDE);
                    else
                    {
                        m_pLstControls[1]->ShowWindowWZ(WZ_SHOW);
                        m_pLstControls[1]->SetCurSel(-1);
                    }
                }
                break;

            case DLGPOS_BTN_CREATE:
                {
#ifdef	__NA_000000_20070528_TAIWAN_GUILD_PVP
					if( GetManager()->IsSpecialType( eROOM_SPECIAL_GUILD_PVP ) )
					{
						if( g_pHero != NULL )
						{
							char str[MAX_ROOMTITLE_LENGTH];
							sprintf( str, "%s VS", g_pHero->GetGuildName() );
							strncpy( m_cfgs.strTitle, str, MAX_ROOMTITLE_LENGTH-1 );
						}
					}
					else
#endif
					{
						// 방제목의 최대 사이즈(20 byte)를 넘었습니다.
						if(!GlobalFunc::CompareLimitText(m_pEdtControls[EDT_TITLE], m_cfgs.strTitle, MAX_ROOMTITLE_LENGTH-1, 5791))
							break;
					}

					// 비밀번호의 최대 사이즈(7 byte)를 넘었습니다.
					if(!GlobalFunc::CompareLimitText(m_pEdtControls[EDT_PASSWORD], m_cfgs.strPassword, MAX_ROOMPASSWORD_LENGTH-1, 5792))
						break;


                    if (!lstrcmp(m_cfgs.strTitle, _T("")))
                    {
                        this->GetUIMan()->GetInterfaceString(eST_INPUT_ROOM_NAME, szMessage, INTERFACE_STRING_LENGTH);

						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_VerifyLock(szMessage);

                    }
                    else
					{
						// 금지단어 check
						if(GlobalFunc::IsSlang(m_cfgs.strTitle, false) == TRUE)
						{
							this->GetUIMan()->GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, MAX_MESSAGE_LENGTH );

							uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
							if(pSystemMan)
								pSystemMan->Sys_VerifyLock(szMessage);

							break;
						}
                        //! 배틀존 생성시 날개 접음
                        g_pHero->SendWingsControlByRider(false, false);
                        g_pHero->SendWingsControlByAccessory(false, false);

						m_pEdtControls[EDT_TITLE]->SetFocusWZ();
						this->checkEditMinMaxLevel();
						this->GetManager()->NET_SEND_MSG_CG_ZONE_LOBBY_CREATE_SYN();
					}
                }
                break;

            case DLGPOS_BTN_EXIT:
            case DLGPOS_BTN_CANCEL:
                {
                    this->ExitZoneCreate();
                }
                break;

			case DLGPOS_EDT_TITLE:
				{
					GlobalFunc::SetText(m_pEdtControls[EDT_TITLE], _T(""));
				}
				break;

			
			case DLGPOS_BTN_SUPER_ROOM_SELECT:
				{
					if( m_pListSuperMaster->GetState() == WZ_SHOW )
					{
						m_pListSuperMaster->ShowWindowWZ(WZ_HIDE);
					}
					else
					{
						m_pListSuperMaster->ShowWindowWZ(WZ_SHOW);
					}
				}
				break;

			case DLGPOS_LIST_SUPER_ROOM:
				{
					int iSel = m_pListSuperMaster->GetCurSel();
					if( iSel >= 0 )
					{
						SetTextCurrentSuperMasterType(iSel);
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
						setMapType(m_cfgs.mapType);
#endif
						m_pListSuperMaster->ShowWindowWZ(WZ_HIDE);
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
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::OnUpdateSolarDialog()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	if (m_cfgs.mapType == eCHKMAP_EP2_MAX)
	{
		// 레벨제한에 의해 선택할 수 있는 맵이 존재하지 않습니다.\n다른 모드를 선택하시거나 마을 이동을 통해 자신의 레벨에 맞는 맵을 선택해주세요 : 5623
		g_InterfaceManager.GetInterfaceString(eST_LIMIT_LEVEL_AND_NO_SELET_MAP , szMessage, INTERFACE_STRING_LENGTH ); 
		this->m_pBtnControls[1]->ShowWindowWZ(WZ_HIDE);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
	}
	else
	{
		this->m_pBtnControls[1]->ShowWindowWZ(WZ_SHOW);
		this->GetUIMan()->ShowCenterMessageBox( NULL, 1, NULL, FALSE );
	}
    this->updateCheckBtns();
    this->updateRoomType();

	
	this->updatePeopleLimitType();
    this->updateLevelLimit();

    this->checkEditMinMaxLevel();
	this->checkMapMouseOver();

}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::OnRenderSolarDialog()
{
    this->renderRoomTypeDescription();

	renderTooltip();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::renderTooltip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

    // 스킵처리 
    // 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
    BOOL bMouseOver = FALSE;
	int iTooltipStringCode = 0;

	for (POSTYPE a = DLGPOS_CHKBTN_CLASS_B; a < POS_CHKBTN_CLASS_END; a++)
    {
        CControlWZ * pControl = getControl(a);

		if( pControl == NULL )
			continue;

        RECT rc = pControl->GetSizeRect();

        if (rc.left <= iMouseX && rc.right >= iMouseX)
        {
            if (rc.top <= iMouseY && rc.bottom >= iMouseY)
            {
                bMouseOver = TRUE;

				switch(a)
				{
				case DLGPOS_CHKBTN_CLASS_B: iTooltipStringCode = eST_CHARACTER_CLASS_NAME_BERSERKER; 
					break;
				case DLGPOS_CHKBTN_CLASS_D: iTooltipStringCode = eST_CHARACTER_CLASS_NAME_DRAGON;
					break;
				case DLGPOS_CHKBTN_CLASS_E: iTooltipStringCode = eST_CHARACTER_CLASS_NAME_MAGICIAN;
					break;
				case DLGPOS_CHKBTN_CLASS_V: iTooltipStringCode = eST_CHARACTER_CLASS_NAME_VALKYRIE;
					break;
				case DLGPOS_CHKBTN_CLASS_S: iTooltipStringCode = eST_CHARACTER_CLASS_NAME_SHADOW;
					break;
				default: 
					return;
				}
            }
        }
    }

    if ( !bMouseOver )
    {
		iTooltipStringCode = 0;
        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
		return;
    }
	
	if(iTooltipStringCode)
	{
		uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
		if(pUIMan)
		{
			pUIMan->RegisterTooltipWithStringCode(iTooltipStringCode);
			pUIMan->SetTooltipDialogID(GetDialogKey());
		}

	}
}

#pragma warning ( push )
#pragma warning(disable : 4800)
//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::ExitZoneCreate()
{
    ShowInterface(FALSE);

    GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST )->ShowInterface( TRUE );
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::Clear()
{
    if(!m_bInitControls) return;

    m_cfgs.clear();
#ifdef _CN_EXCECPT_MISSION	
    m_cfgs.roomType = 0;
#else
    m_cfgs.roomType = eCREATE_BATTLE_MISSION_TYPE;
#endif//_CN_EXCECPT_MISSION
    bool canChoice = true;
    if (!setMapType(m_cfgs.mapType))
    {
        canChoice = false;

        for (int i=0; i<eCHKMAP_EP2_MAX; ++i)
        {
            if (setMapType(i))
            {
                canChoice = true;								 
                break;
            }
        }
    }

    if (!canChoice)
    {
        m_cfgs.mapType = eCHKMAP_EP2_MAX;
    }

    clearModeList();
    clearPeopleList();

    m_pEdtControls[EDT_MIN_LEVEL]->SetFontID(StrToWzID("mn12"));
    m_pEdtControls[EDT_MAX_LEVEL]->SetFontID(StrToWzID("mn12"));
    m_pEdtControls[EDT_MIN_LEVEL]->SetLimitText(3);
    m_pEdtControls[EDT_MAX_LEVEL]->SetLimitText(3);
    m_pEdtControls[EDT_TITLE]->SetFontID(StrToWzID("mn12"));
    m_pEdtControls[EDT_TITLE]->SetLimitText(MAX_ROOMTITLE_LENGTH-1);
    m_pEdtControls[EDT_PASSWORD]->SetFontID(StrToWzID("mn12"));
    m_pEdtControls[EDT_PASSWORD]->SetLimitText(MAX_ROOMPASSWORD_LENGTH-1);
    m_pEdtControls[EDT_PASSWORD]->SetColorCaret(c_InterfaceCaretColor);
    GlobalFunc::SetText(m_pEdtControls[EDT_MIN_LEVEL], _T(""));
    GlobalFunc::SetText(m_pEdtControls[EDT_MAX_LEVEL], _T(""));
    GlobalFunc::SetText(m_pEdtControls[EDT_TITLE], _T(""));
    GlobalFunc::SetText(m_pEdtControls[EDT_PASSWORD], _T(""));

    m_pEdtControls[EDT_PASSWORD]->ShowWindowWZ(WZ_SHOW);

    //gs_bug_fix:화면이 열린상태에서만 타이틀창에 포커스를 맞춘다
    if(this->IsVisibleWindow())
    {
        GlobalFunc::SetFocus((SolarDialog *)this, m_pEdtControls[EDT_TITLE]);
    }

#ifdef _GS_GSP_HIDE_BATTLEZONE_LIST_BTN
    m_pBtnControls[4]->ShowWindowWZ(WZ_HIDE);
#else
    m_pBtnControls[4]->ShowWindowWZ(WZ_SHOW);	
#endif//
    setDefaultRoomName();

}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::clearModeList()
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH];

    m_pLstControls[0]->SetColorSelBar(RGBA(0,0,0,0));
    m_pLstControls[0]->SetSellHeight(16);
    m_pLstControls[0]->SetUseReport(false);
    m_pLstControls[0]->SetReportUseCnt(0);
    m_pLstControls[0]->SetHideReportRender(true);

    m_pLstControls[0]->DeleteItemAll();
    m_pLstControls[0]->SetBeginDrawIndex(0);

    for (int i=0; i<eCREATE_BATTLE_MAX; ++i)
    {
        int idx = m_pLstControls[0]->InsertItemBack();

        this->GetUIMan()->GetInterfaceString(m_cfgs.vRoomCode[i], szMessage, INTERFACE_STRING_LENGTH);
        GlobalFunc::SetItem(m_pLstControls[0],
            idx,
            szMessage,
            StrToWzID("mn12"),
            DT_CENTER,
            RGBA(0,0,0,0),              // BKColor
            RGBA(255, 255, 255, 255),    // TextColor
            RGBA(255, 255, 255, 255)    // SelTextColor
            );
    }

    m_pLstControls[0]->ShowWindowWZ(WZ_HIDE);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::clearPeopleList()
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

    m_pLstControls[1]->SetColorSelBar(RGBA(0,0,0,0));
    m_pLstControls[1]->SetSellHeight(16);
    m_pLstControls[1]->SetUseReport(false);
    m_pLstControls[1]->SetReportUseCnt(0);
    m_pLstControls[1]->SetHideReportRender(true);

    m_pLstControls[1]->DeleteItemAll();
    m_pLstControls[1]->SetBeginDrawIndex(0);

    for (int i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        int idx = m_pLstControls[1]->InsertItemBack();

        Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), (i + 1));
        GlobalFunc::SetItem(m_pLstControls[1],
            idx,
            szMessage,
            StrToWzID("mn12"),
            DT_CENTER,
            RGBA (0,0,0,0),              // BKColor
            RGBA(255, 255, 255, 255),    // TextColor
            RGBA(255, 255, 255, 255)    // SelTextColor
            );
    }

    m_pLstControls[1]->ShowWindowWZ(WZ_HIDE);
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneMan*
uiBattleZoneCreateEp2::GetManager()
{
    uiBattleZoneMan* battleZoneMan =
        static_cast<uiBattleZoneMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLEZONE));
    assert (battleZoneMan);
    return battleZoneMan;
}

//------------------------------------------------------------------------------
/**
*/
bool
uiBattleZoneCreateEp2::checkValidMap(MAPCODE fromCode, MAPCODE toCode)
{
    const sMAPINFO* fromMapInfo = MapInfoParser::Instance()->FindMapInfo(fromCode);
    if (!fromMapInfo)
        return false;

    const sMAPINFO* toMapInfo = MapInfoParser::Instance()->FindMapInfo(toCode);
    if (!toMapInfo)
        return false;

    LEVELTYPE	lv;

    if (g_pHero)
    {
        lv = g_pHero->GetLevel();
    }
    else
    {
        lv = 1;
    }

    if (toMapInfo->minLV > (lv+1) || toMapInfo->maxLV < lv)
        return false;

    switch (m_cfgs.roomType)
    {
#ifdef _CN_EXCECPT_MISSION
        //no operation
#else
    case eCREATE_BATTLE_MISSION_TYPE:
        {
            //! 2011.5.6 / i4u4me / 스크립트에서 FileVersion 필드 제거
            //! fromMapInfo->FileVersion >= 1.3 임
            //if(1.3 <= fromMapInfo->FileVersion)
            {
                for(int i = 0; i < sMAPINFO::MAX_MISSION_MAP_NUM_V1_3; ++i)
                {
                    if(fromMapInfo->byMissionMapVec[i] == toMapInfo->MapKind)
                        return true;
                }
            }
            //else
            //{
            //    for( int i=0; i < sMAPINFO::MAX_MISION_MAP_NUM; i++ )
            //    {
            //        if (fromMapInfo->byMissionMap[i] == toMapInfo->MapCode)
            //        {
            //            return true;
            //        }
            //    }
            //}
        }
        break;
#endif//_CN_EXCECPT_MISSION(미션선택제거)    
    case eCREATE_BATTLE_PVP_TYPE:
        {
            //! 2011.5.6 / i4u4me / 스크립트에서 FileVersion 필드 제거
            //! fromMapInfo->FileVersion >= 1.3 임
            //if(1.3 <= fromMapInfo->FileVersion)
            {
                for(int i = 0; i < sMAPINFO::MAX_PVP_MAP_NUM_V1_3; ++i)
                {
                    if(fromMapInfo->wPVPMapVec[i] == toMapInfo->MapKind)
                        return true;
                }

            }
            //else
            //{
            //    for( int i=0; i < sMAPINFO::MAX_PVP_MAP_NUM; i++ )
            //    {
            //        if (fromMapInfo->wPVPMap[i] == toMapInfo->MapCode)
            //        {
            //            return true;
            //        }
            //    }
            //}
        }
        break;

#ifndef _EXCECPT_HUNTING_001011
    case eCREATE_BATTLE_CHUNTING:
        {
            //! 2011.5.6 / i4u4me / 스크립트에서 FileVersion 필드 제거
            //! fromMapInfo->FileVersion >= 1.3 임
            //if(1.3 <= fromMapInfo->FileVersion)
            {
                for(int i = 0; i < sMAPINFO::MAX_CHUNTING_MAP_NUM_V1_3; ++i)
                {
                    if(fromMapInfo->wCHuntingMapVec[i] == toMapInfo->MapKind)
                        return true;
                }

            }
            //else
            //{
            //    for( int i=0; i < sMAPINFO::MAX_CHUNTING_MAP_NUM; i++ )
            //    {
            //        if (fromMapInfo->wCHuntingMap[i] == toMapInfo->MapCode)
            //        {
            //            return true;
            //        }
            //    }
            //}
        }
        break;
#endif
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::checkEditMinMaxLevel()
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

    if (!m_pEdtControls[EDT_MIN_LEVEL]->HaveFocus())
    {
        GlobalFunc::GetText(m_pEdtControls[EDT_MIN_LEVEL], szMessage, 4);
        szMessage[4] = 0;

        bool allAlpha = false;
        int i;

        for (i=0; i<lstrlen(szMessage); ++i)
        {
            if (szMessage[i]>=_T('0') && szMessage[i]<=_T('9'))
            {
                allAlpha = true;
            }
        }

        int value = _ttoi(szMessage);

        if (allAlpha)
        {
            if (m_cfgs.maxLevel < value)
            {
                m_cfgs.minLevel = m_cfgs.maxLevel-1;
            }
            else if (m_cfgs.fix_minLevel > value)
            {
                m_cfgs.minLevel = m_cfgs.fix_minLevel;
            }
            else
            {
                m_cfgs.minLevel = value;
            }
        }

        Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), m_cfgs.minLevel);
        GlobalFunc::SetText(m_pEdtControls[EDT_MIN_LEVEL], szMessage);
    }

    if (!m_pEdtControls[EDT_MAX_LEVEL]->HaveFocus())
    {
        GlobalFunc::GetText(m_pEdtControls[EDT_MAX_LEVEL], szMessage, 4);
        szMessage[4] = 0;

        bool allAlpha = false;
        int i;

        for (i=0; i<lstrlen(szMessage); ++i)
        {
            if (szMessage[i]>=_T('0') && szMessage[i]<=_T('9'))
            {
                allAlpha = true;
            }
        }

        int value = _ttoi(szMessage);

        if (allAlpha)
        {
            if (m_cfgs.minLevel > value)
            {
                m_cfgs.maxLevel = m_cfgs.minLevel+1;
            }
            else if (m_cfgs.fix_maxLevel < value)
            {
                m_cfgs.maxLevel = m_cfgs.fix_maxLevel;
            }
            else
            {
                m_cfgs.maxLevel = value;
            }
        }

        Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), m_cfgs.maxLevel);
        GlobalFunc::SetText(m_pEdtControls[EDT_MAX_LEVEL], szMessage);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
uiBattleZoneCreateEp2::setMapType(WORD wType)
{
    bool setIsFine = false;

    switch (m_cfgs.roomType)
    {
#ifdef _CN_EXCECPT_MISSION	
        //no operation
#else
    case eCREATE_BATTLE_MISSION_TYPE: { setIsFine = setMapTypeMission(wType); } break;
#endif//_CN_EXCECPT_MISSION	
    case eCREATE_BATTLE_PVP_TYPE: { setIsFine = setMapTypePVP(wType); } break;
#ifndef _EXCECPT_HUNTING_001011
    case eCREATE_BATTLE_CHUNTING: { setIsFine = setMapTypeCHunting(wType); } break;
#endif
    }

    if (!setIsFine)
    {
        // couldn't select it.
    }
    else
        m_cfgs.mapType = wType;

    return setIsFine;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::setMapTypeInfo(const sMAPINFO* mapInfo)
{
    assert (mapInfo);
    if (!mapInfo)
        return;

    m_cfgs.mapCode = mapInfo->MapCode;
    m_cfgs.peopleType = mapInfo->byMaxUserNum-1;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    if( GetManager()->IsSpecialType(eROOM_SPECIAL_GUILD_PVP) )
    {
        m_cfgs.peopleType = MAX_GUILD_PVP_JOIN_MEMBER -1;
    }
#endif
    m_cfgs.fix_peopleType = m_cfgs.peopleType;

#ifdef _CN_EXCECPT_MISSION
#ifndef _EXCECPT_HUNTING_001011
    if (g_pHero && m_cfgs.roomType == eCREATE_BATTLE_CHUNTING)
    {
        if (mapInfo->minLV < g_pHero->GetLevel() - HUNTING_JOIN_LEVEL_OFFSET)
            m_cfgs.minLevel = g_pHero->GetLevel() - HUNTING_JOIN_LEVEL_OFFSET;
        else
            m_cfgs.minLevel = mapInfo->minLV;
        m_cfgs.fix_minLevel = m_cfgs.minLevel;

        if (mapInfo->maxLV > g_pHero->GetLevel() + HUNTING_JOIN_LEVEL_OFFSET)
            m_cfgs.maxLevel = g_pHero->GetLevel() + HUNTING_JOIN_LEVEL_OFFSET;
        else
            m_cfgs.maxLevel = mapInfo->maxLV;

        m_cfgs.fix_maxLevel = m_cfgs.maxLevel;
    }
    else
    {
        m_cfgs.minLevel = mapInfo->minLV;
        m_cfgs.fix_minLevel = m_cfgs.minLevel;
        m_cfgs.maxLevel = mapInfo->maxLV;
        m_cfgs.fix_maxLevel = m_cfgs.maxLevel;
    }
#else//일본 헌팅목록제거
    m_cfgs.minLevel = mapInfo->minLV;
    m_cfgs.fix_minLevel = m_cfgs.minLevel;
    m_cfgs.maxLevel = mapInfo->maxLV;
    m_cfgs.fix_maxLevel = m_cfgs.maxLevel;
#endif
#else//else _CN_EXCECPT_MISSION
    if( g_pHero && m_cfgs.roomType == eCREATE_BATTLE_MISSION_TYPE)
    {
        m_cfgs.minLevel = mapInfo->minLV;
        m_cfgs.fix_minLevel = mapInfo->minLV;

        if(TUTO_MISSION == mapInfo->MapCode)//튜토리얼 맵이 맞다면.
            m_cfgs.maxLevel = mapInfo->maxLV;//튜토리얼 제한 레벨.
        else
            m_cfgs.maxLevel = min( g_pHero->GetLevel() + MISSION_LEVEL_JOIN_OFFSET , mapInfo->maxLV);

        m_cfgs.fix_maxLevel = m_cfgs.maxLevel;
    }
#ifndef _EXCECPT_HUNTING_001011
    else if (g_pHero && m_cfgs.roomType == eCREATE_BATTLE_CHUNTING)
    {
        if (mapInfo->minLV < g_pHero->GetLevel() - HUNTING_JOIN_LEVEL_OFFSET)
            m_cfgs.minLevel = g_pHero->GetLevel() - HUNTING_JOIN_LEVEL_OFFSET;
        else
            m_cfgs.minLevel = mapInfo->minLV;
        m_cfgs.fix_minLevel = m_cfgs.minLevel;

        if (mapInfo->maxLV > g_pHero->GetLevel() + HUNTING_JOIN_LEVEL_OFFSET)
            m_cfgs.maxLevel = g_pHero->GetLevel() + HUNTING_JOIN_LEVEL_OFFSET;
        else
            m_cfgs.maxLevel = mapInfo->maxLV;

        m_cfgs.fix_maxLevel = m_cfgs.maxLevel;
    }
#endif
    else
    {
        m_cfgs.minLevel = mapInfo->minLV;
        m_cfgs.fix_minLevel = m_cfgs.minLevel;
        m_cfgs.maxLevel = mapInfo->maxLV;
        m_cfgs.fix_maxLevel = m_cfgs.maxLevel;
    }
#endif//_CN_EXCECPT_MISSION



    TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), m_cfgs.minLevel);
    GlobalFunc::SetText(m_pEdtControls[EDT_MIN_LEVEL], szMessage);
    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), m_cfgs.maxLevel);
    GlobalFunc::SetText(m_pEdtControls[EDT_MAX_LEVEL], szMessage);
}

//------------------------------------------------------------------------------
/**
*/
bool
uiBattleZoneCreateEp2::setMapTypeMission(WORD wType)
{
    const sMAPINFO* villMapInfo;
    if (GENERALPARAM->IsTestUIScene())
        villMapInfo = MapInfoParser::Instance()->FindMapInfo(10001);
    else
        villMapInfo = MapInfoParser::Instance()->FindMapInfo(g_pMap->GetCurrentFieldID());

    if (!villMapInfo)
        return false;

    int groupCode = 2 + (wType + eCHKMAP_CODE_MISSION);// EP2는 kind가 9부터 시작 7+2

    MapGroup* pMapGroup = MapInfoParser::Instance()->GetMapGroup(groupCode);
    if (!pMapGroup)
        return false;

    // 해당 멥을 생성할때 해당 그룹에 존재하는 멥을 골라서 생성한다.
    // 미션담당자(김미숙) 요청으로 디폴트 멥을 서브 1, 2, 3, 4... 마지막에 메인 미션(0)을 찾는식으로 함. 
    size_t	MapGroupCnt = pMapGroup->GetMapInfoHash().size();
    if ( 0 == MapGroupCnt )
    {
        return false;
    }

    sMAPINFO* goMapInfo = NULL;

    //서브미션을 먼저찾는다.
    for ( size_t i = 0; i < MapGroupCnt; ++i )
    {
        goMapInfo = pMapGroup->GetMapInfoIndex(i+1);
        if ( goMapInfo )
            break;
    }

    //서브미션이 없다는 메인으로 설정
    if (!goMapInfo)
    {
        // 메인미션 확인.		
        goMapInfo = pMapGroup->GetMapInfoIndex(0);
    }


    if ( !goMapInfo )
        return false;

    if (checkValidMap(villMapInfo->MapCode, goMapInfo->MapCode))
    {
        setMapTypeInfo(goMapInfo);
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
uiBattleZoneCreateEp2::setMapTypeHunting(WORD wType)
{
    const sMAPINFO* villMapInfo;
    if (GENERALPARAM->IsTestUIScene())
        villMapInfo = MapInfoParser::Instance()->FindMapInfo(10001);
    else
        villMapInfo = MapInfoParser::Instance()->FindMapInfo(g_pMap->GetCurrentFieldID());

    if (!villMapInfo)
        return false;

    int groupCode = 2 + (wType + eCHKMAP_CODE_HUNTING);

    MapGroup* pMapGroup = MapInfoParser::Instance()->GetMapGroup(groupCode);
    if (!pMapGroup)
        return false;

    sMAPINFO* goMapInfo = pMapGroup->GetMapInfoIndex(0);
    if (!goMapInfo)
        return false;

    if (checkValidMap(villMapInfo->MapCode, goMapInfo->MapCode))
    {
        setMapTypeInfo(goMapInfo);
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
uiBattleZoneCreateEp2::setMapTypePVP(WORD wType)
{
    const sMAPINFO* villMapInfo;
    if (GENERALPARAM->IsTestUIScene())
        villMapInfo = MapInfoParser::Instance()->FindMapInfo(10001);
    else
        villMapInfo = MapInfoParser::Instance()->FindMapInfo(g_pMap->GetCurrentFieldID());

    if (!villMapInfo)
        return false;

    int groupCode = 2 + (wType + eCHKMAP_CODE_PVP);

    MapGroup* pMapGroup = MapInfoParser::Instance()->GetMapGroup(groupCode);
    if (!pMapGroup)
        return false;

    sMAPINFO* goMapInfo = pMapGroup->GetMapInfoIndex(0);
    if (!goMapInfo)
        return false;

    if (checkValidMap(villMapInfo->MapCode, goMapInfo->MapCode))
    {
        setMapTypeInfo(goMapInfo);
        return true;
    }

    return false;
}

bool _sort_chunting_index_low_to_lv(stCHuntingMap a, stCHuntingMap b)
{
    return (a.iRequireMinLV  < b.iRequireMinLV);
}

//------------------------------------------------------------------------------
/**
*/
bool
uiBattleZoneCreateEp2::setMapTypeCHunting(WORD wType)
{
    const sMAPINFO* villMapInfo;
    if (GENERALPARAM->IsTestUIScene())
        villMapInfo = MapInfoParser::Instance()->FindMapInfo(10001);
    else
        villMapInfo = MapInfoParser::Instance()->FindMapInfo(g_pMap->GetCurrentFieldID());

    if (!villMapInfo)
        return false;

    int groupCode = 2 + (wType + eCHKMAP_CODE_CHUNTING);

    MapGroup* pMapGroup = MapInfoParser::Instance()->GetMapGroup(groupCode);
    if (!pMapGroup)
        return false;

    std::vector<stCHuntingMap>	vecCHuntingMapIndex;
    vecCHuntingMapIndex.clear();

    sMAPINFO_HASH & mapHash = pMapGroup->GetMapInfoHash();

    sMAPINFO_HASH::const_iterator itr = mapHash.begin();
    sMAPINFO * pMapInfo = NULL;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    BOOL FoundGuildPVP = FALSE;
    if (GetManager()->IsSpecialType(eROOM_SPECIAL_GUILD_PVP) )
    {
        FoundGuildPVP = TRUE;
    }
#endif
    while ( itr != mapHash.end())
    {
        pMapInfo =  itr->second;
        itr++;
        BASE_RequireFieldLevelInfo * pInfo = NULL;
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        pInfo = RequireFieldLevelInfoParser::Instance()->GetRequireFieldLevelInfo(
            (WORD)pMapInfo->MapCode,
            (BYTE)0, FoundGuildPVP );
#else
        pInfo = RequireFieldLevelInfoParser::Instance()->GetRequireFieldLevelInfo(
            (WORD)pMapInfo->MapCode,
            (BYTE)0 );
#endif

        int iRequireMinLV = 0;
        if (pInfo)
        {
            iRequireMinLV = pInfo->m_wMinlv;
        }
        else
        {
            iRequireMinLV = pMapInfo->minLV;
        }

        stCHuntingMap sCh;
        sCh.MapCode = pMapInfo->MapCode;
        sCh.iRequireMinLV = iRequireMinLV;
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        if(FoundGuildPVP && pMapInfo->EntranceItemCode == 0)
            continue;
#endif
        vecCHuntingMapIndex.push_back(sCh);

    }

    std::sort( 
        vecCHuntingMapIndex.begin(), 
        vecCHuntingMapIndex.end(), 
        _sort_chunting_index_low_to_lv
        );

    MAPCODE mapcode = vecCHuntingMapIndex[0].MapCode;
    const sMAPINFO* goMapInfo = MapInfoParser::Instance()->FindMapInfo(mapcode);
    if (!goMapInfo)
        return false;

    if (checkValidMap(villMapInfo->MapCode, goMapInfo->MapCode))
    {
        this->GetManager()->ClearPendings();
        setMapTypeInfo(goMapInfo);
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::setClassType(POSTYPE pos)
{
    if (!g_pHero)
        return;

    eCLASS_TYPE classLimit = m_cfgs.classLimit;

    eCLASS_TYPE comp_classLimit;
    switch (pos)
    {
    case DLGPOS_CHKBTN_CLASS_B: { comp_classLimit = eCLASS_BERSERKER; } break;
    case DLGPOS_CHKBTN_CLASS_D: { comp_classLimit = eCLASS_DRAGON; } break;
    case DLGPOS_CHKBTN_CLASS_V: { comp_classLimit = eCLASS_VALKYRIE; } break;
    case DLGPOS_CHKBTN_CLASS_E: { comp_classLimit = eCLASS_ELEMENTALIST; } break;
    case DLGPOS_CHKBTN_CLASS_S: { comp_classLimit = eCLASS_SHADOW; } break;
    default:
        assert (!"unknown class type");
        return;
    }

    eCLASS_TYPE hero_classLimit;
    switch (g_pHero->GetClass())
    {
    case eCHAR_BERSERKER:       { hero_classLimit = eCLASS_BERSERKER; } break;
    case eCHAR_DRAGON:          { hero_classLimit = eCLASS_DRAGON; } break;
    case eCHAR_VALKYRIE:        { hero_classLimit = eCLASS_VALKYRIE; } break;
    case eCHAR_ELEMENTALIST:    { hero_classLimit = eCLASS_ELEMENTALIST; } break;
    case eCHAR_SHADOW:			{ hero_classLimit = eCLASS_SHADOW; } break;
    default:
        assert (!"unknown class type");
        return;
    }

    // compare to hero classlimit
    if (hero_classLimit == comp_classLimit)
    {
        TCHAR	szMessage[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString(eST_CAN_NOT_LIMIT_OWN_CLASS,szMessage,INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Guide(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        return;
    }

    // modify classlimit
    bool toggleClassLimit = !(classLimit & comp_classLimit);
    if (toggleClassLimit)
    {
        classLimit = 
            static_cast<eCLASS_TYPE>(classLimit | comp_classLimit);
    }
    else
    {
        classLimit = 
            static_cast<eCLASS_TYPE>(classLimit & ~comp_classLimit);
    }

    m_cfgs.classLimit = classLimit;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::updateCheckBtns()
{
    for (int i=POS_CHKBTN_CLASS_BEGIN; i<POS_CHKBTN_CLASS_END; ++i)
    {
        CCtrlButtonCheckWZ* pClassChkBtn =
            m_pChkBtnClassControls[i - POS_CHKBTN_CLASS_BEGIN];

        switch (i)
        {
        case DLGPOS_CHKBTN_CLASS_B:
            pClassChkBtn->SetCheckState((bool)(m_cfgs.classLimit & eCLASS_BERSERKER));
            break;
        case DLGPOS_CHKBTN_CLASS_D:
            pClassChkBtn->SetCheckState((bool)(m_cfgs.classLimit & eCLASS_DRAGON));
            break;
        case DLGPOS_CHKBTN_CLASS_V:
            pClassChkBtn->SetCheckState((bool)(m_cfgs.classLimit & eCLASS_VALKYRIE));
            break;
        case DLGPOS_CHKBTN_CLASS_E:
            pClassChkBtn->SetCheckState((bool)(m_cfgs.classLimit & eCLASS_ELEMENTALIST));
            break;
        case DLGPOS_CHKBTN_CLASS_S:
            {
                pClassChkBtn->SetCheckState((bool)(m_cfgs.classLimit & eCLASS_SHADOW));
#ifdef __CN_CREATE_BLOCK_SHADOW
                pClassChkBtn->EnableWindowWZ(FALSE);
#endif//__CN_CREATE_BLOCK_SHADOW
            }
            break;	

        default:
            assert (!"unknown class type");
            break;
        }
    }

    for (int i=eCHKMAP_THE_EARTH_OF_TREES_SHADOW; i<eCHKMAP_EP2_MAX; ++i)
    {
        m_pChkBtnMapControls[i - eCHKMAP_THE_EARTH_OF_TREES_SHADOW]->SetCheckState(false);
    }

    if (m_cfgs.mapType != eCHKMAP_EP2_MAX)
        m_pChkBtnMapControls[m_cfgs.mapType - eCHKMAP_THE_EARTH_OF_TREES_SHADOW]->SetCheckState(true);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::updateRoomType()
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH];
    g_InterfaceManager.GetInterfaceString( m_cfgs.vRoomCode[m_cfgs.roomType], szMessage, INTERFACE_STRING_LENGTH );
    GlobalFunc::SetCaption(m_pTxtControls[1], szMessage);

    if (this->m_pLstControls[0]->GetState() != WZ_SHOW)
        return;

    m_pLstControls[0]->SetColorSelBar(RGBA(0,0,0,0));
    m_pLstControls[0]->SetSellHeight(24);
    m_pLstControls[0]->SetUseReport(false);
    m_pLstControls[0]->SetReportUseCnt(0);
    m_pLstControls[0]->SetHideReportRender(true);

    m_pLstControls[0]->DeleteItemAll();
    m_pLstControls[0]->SetBeginDrawIndex(0);

    for (int i=0; i<eCREATE_BATTLE_MAX; ++i)
    {
        int idx = m_pLstControls[0]->InsertItemBack();

        this->GetUIMan()->GetInterfaceString(m_cfgs.vRoomCode[i], szMessage, INTERFACE_STRING_LENGTH);
        GlobalFunc::SetItem(m_pLstControls[0],
            idx,
            szMessage,
            StrToWzID("mn12"),
            DT_CENTER | DT_BOTTOM,
            RGBA(0,0,0,0),              // BKColor
            RGBA(255, 255, 255, 255),    // TextColor
            RGBA(255, 255, 255, 255)    // SelTextColor
            );
    }

    int curSel = this->m_pLstControls[0]->GetCurSel();
    if (curSel != -1)
    {
        m_cfgs.roomType = curSel;
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        SetDefaultSuperMasterList();
#endif
        bool canChoice = true;
        if (!setMapType(m_cfgs.mapType))
        {
            canChoice = false;

            for (int i=0; i<eCHKMAP_EP2_MAX; ++i)
            {
                if (setMapType(i))
                {
                    canChoice = true;
                    break;
                }
            }
        }

        if (!canChoice)
        {
            m_cfgs.mapType = eCHKMAP_EP2_MAX;
        }

    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::updatePeopleLimitType()
{
    m_pBtnControls[3]->ShowWindowWZ(WZ_SHOW);

    TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

    m_pLstControls[1]->SetColorSelBar(RGBA(0,0,0,0));
    m_pLstControls[1]->SetSellHeight(16);
    m_pLstControls[1]->SetUseReport(false);
    m_pLstControls[1]->SetReportUseCnt(0);
    m_pLstControls[1]->SetHideReportRender(true);

    m_pLstControls[1]->DeleteItemAll();
    m_pLstControls[1]->SetBeginDrawIndex(0);

    for (int i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        int idx = m_pLstControls[1]->InsertItemBack();

        DWORD color;

        if (m_cfgs.fix_peopleType>=i)
        {
            color = RGBA(255, 255, 255, 255);
        }
        else
        {
            color = RGBA(255, 0, 0, 255);
        }

        Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), (i + 1));
        GlobalFunc::SetItem(m_pLstControls[1],
            idx,
            szMessage,
            StrToWzID("mn12"),
            DT_CENTER | DT_BOTTOM,
            RGBA(0,0,0,0),              // BKColor
            color,    // TextColor
            RGBA(255, 255, 255, 255)    // SelTextColor
            );
    }

    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), m_cfgs.peopleType+1);
    GlobalFunc::SetCaption(m_pTxtControls[0], szMessage);

    if (this->m_pLstControls[1]->GetState() != WZ_SHOW)
        return;

    int curSel = this->m_pLstControls[1]->GetCurSel();
    if (curSel != -1)
    {
        if (m_cfgs.fix_peopleType >= curSel)
            m_cfgs.peopleType = curSel;
        else
            m_cfgs.peopleType = m_cfgs.fix_peopleType;

        m_pLstControls[1]->ShowWindowWZ(WZ_HIDE);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::updatePeopleLimitTypeTemp()
{
    m_cfgs.peopleType = 0;
    m_pBtnControls[3]->ShowWindowWZ(WZ_HIDE);

    TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), m_cfgs.peopleType+1);
    GlobalFunc::SetCaption(m_pTxtControls[0], szMessage);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::updateLevelLimit()
{
    const sMAPINFO* mapInfo = MapInfoParser::Instance()->FindMapInfo(m_cfgs.mapCode);
    if (!mapInfo)
    {
        GlobalFunc::SetCaption (m_pTxtControls[3], _T("1"));
        GlobalFunc::SetCaption (m_pTxtControls[4], _T("100"));
        return;
    }

    TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), mapInfo->minLV);
    GlobalFunc::SetCaption (m_pTxtControls[3], szMessage);
    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), mapInfo->maxLV);
    GlobalFunc::SetCaption (m_pTxtControls[4], szMessage);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::renderRoomTypeDescription()
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH];

    g_InterfaceManager.GetInterfaceString( m_cfgs.vRoomDescCode[m_cfgs.roomType], szMessage, INTERFACE_STRING_LENGTH );

    std::vector<util::_tstring> vstr;


    if( !m_pTxtControls[2])
        return;

    RECT rtList = m_pTxtControls[2]->GetSize();

    GlobalFunc::ProcessWordWrapByFontSize(&vstr , szMessage , StrToWzID("m216") , _tcslen(szMessage) ,rtList.right -5 );


    RECT drc;
    RECT rc;
    RECT rc2;

    drc = m_pTxtControls[2]->GetSizeRect();

    SetRect(&rc,
        drc.left - 20,
        drc.top,
        drc.right,
        drc.bottom);



    this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(StrToWzID("m216"));

    for (size_t i=0; i<vstr.size(); ++i)
    {
        SetRect(&rc2,
            rc.left     + 25,
            rc.top      + 25 + (18*i),
            rc.left     + 25 + 400,
            rc.top      + 25 + (18*i) + 18);

        TCHAR *pStr =
            const_cast<TCHAR *>(vstr[i].c_str());

        if (pStr)
        {
            this->GetDrawBase()->x_pManagerTextOut->DrawText(
                pStr,
                &rc2,
                WzColor_RGBA(180, 180, 180, 255),
                WzColor_RGBA(0,0,0,0),
                TP_HLEFT,
                ETS_OUTLINE);
        }
    }

    if (m_pLstControls[0]->GetState() == WZ_SHOW)
    {
        m_pLstControls[0]->OnDraw(true);

    }

    if( m_pListSuperMaster->GetState() == WZ_SHOW )
        m_pListSuperMaster->OnDraw( true );

    this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(StrToWzID("m216"));

    vstr = m_vMapDesc;

    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

    int sx, sy, ex, ey;

    sx = iMouseX + 25;
    sy = iMouseY;

    for (size_t i=0; i<vstr.size(); ++i)
    {
        SetRect(&rc2,
            iMouseX + 25,
            iMouseY + (18*i),
            iMouseX + 25 + 400,
            iMouseY + (18*i) + 18);

        ex = rc2.right;
        ey = rc2.bottom;

        TCHAR *pStr =
            const_cast<TCHAR *>(vstr[i].c_str());

        if (pStr)
        {
            this->GetDrawBase()->x_pManagerTextOut->DrawText(
                pStr,
                &rc2,
                WzColor_RGBA(255, 255, 255, 255),
                WzColor_RGBA(0,0,0,0),
                TP_HLEFT,


                ETS_OUTLINE);
        }
    }

    if (vstr.size())
    {
        SetRect(&rc2, sx, sy, ex, ey);
        this->GetUIMan()->RenderRect(rc2, WzColor_RGBA(15,18,14,80));
    }
}
//=====================================================================================
//	DESC: 해당지역 툴팁정보를 m_vMapDesc에 삽입한다.
//=====================================================================================
void
uiBattleZoneCreateEp2::refreshToolTip(POSTYPE pos)
{
    m_vMapDesc.clear();

    if(pos==eCHKMAP_EP2_MAX) return; 

    if(m_cfgs.roomType == eCREATE_BATTLE_PVP_TYPE)
        return;

    TCHAR   szText[INTERFACE_STRING_LENGTH] = {0,};

    const sMAPINFO* villMapInfo;
    if (GENERALPARAM->IsTestUIScene())
        villMapInfo = MapInfoParser::Instance()->FindMapInfo(10001);
    else
        villMapInfo = MapInfoParser::Instance()->FindMapInfo(g_pMap->GetCurrentFieldID());

    if (!villMapInfo)
        return;

    DWORD	from;
    TCHAR	szModeName[INTERFACE_STRING_LENGTH] = {0,};

    switch (m_cfgs.roomType)
    {
#ifdef _CN_EXCECPT_MISSION	
        //no operation
#else
    case eCREATE_BATTLE_MISSION_TYPE: 
        { 
            // 1651	미션 레벨 제한 
            g_InterfaceManager.GetInterfaceString(1651, szModeName, INTERFACE_STRING_LENGTH);
            from = eCHKMAP_CODE_MISSION; 
        } 
        break;
#endif//_CN_EXCECPT_MISSION
    case eCREATE_BATTLE_PVP_TYPE: 
        { 
            // 1652	PVP 레벨 제한
            g_InterfaceManager.GetInterfaceString(1652, szModeName, INTERFACE_STRING_LENGTH);
            from = eCHKMAP_CODE_PVP; 
        } 
        break;

#ifndef _EXCECPT_HUNTING_001011
    case eCREATE_BATTLE_CHUNTING: 
        { 
            // 1653	헌팅 레벨 제한
            g_InterfaceManager.GetInterfaceString(1653, szModeName, INTERFACE_STRING_LENGTH);
            from = eCHKMAP_CODE_CHUNTING; 
        } 
        break;
#endif
    }

    int groupCode = 2 + (pos + from);   // EP2는 그룹이 9부터 시작 +2 (7+2)

    MapGroup* pMapGroup = MapInfoParser::Instance()->GetMapGroup(groupCode);
    if (!pMapGroup)
        return;

    CODETYPE mapcode = -1;

    for(int nCnt=0;nCnt<4;++nCnt)	//메인미션이 아닐때 서브정보를 참조한다.
    {
        if(pMapGroup->GetMapInfoIndex(nCnt))
        {
            mapcode = pMapGroup->GetMapInfoIndex(nCnt)->MapCode;
            break;
        }
    }

    if(mapcode==-1) return;

    const sMAPINFO* goMapInfo = MapInfoParser::Instance()->FindMapInfo( static_cast<MAPCODE>(mapcode));
    if (!goMapInfo)
        return;

    if (!checkValidMap(villMapInfo->MapCode, goMapInfo->MapCode))
    {
        return;
    }

    this->GetUIMan()->GetInterfaceString(goMapInfo->dwANCode, szText, INTERFACE_STRING_LENGTH ); 
    this->m_vMapDesc.push_back(szText);
    this->m_vMapDesc.push_back(_T("")); // 공백 추가

#ifdef _JAPAN
    Snprintf (szText, INTERFACE_STRING_LENGTH-1, _T("%s: %d - %d"), szModeName, goMapInfo->minLV, goMapInfo->maxLV);
#else
    Snprintf (szText, INTERFACE_STRING_LENGTH-1, _T("%s: %d ~ %d"), szModeName, goMapInfo->minLV, goMapInfo->maxLV);
#endif
    this->m_vMapDesc.push_back(szText);

#ifndef _EXCECPT_HUNTING_001011
    if (g_pHero && m_cfgs.roomType == eCREATE_BATTLE_CHUNTING)
    {
        LEVELTYPE	baseLevel = g_pHero->GetLevel();

        short destLevel;
        short destMinLV;
        short destMaxLV;

        destLevel = baseLevel - HUNTING_JOIN_LEVEL_OFFSET;
        if (goMapInfo->minLV < destLevel)
            destMinLV = destLevel;
        else
            destMinLV  = goMapInfo->minLV;

        destLevel = baseLevel + HUNTING_JOIN_LEVEL_OFFSET;
        if (goMapInfo->maxLV > destLevel)
            destMaxLV = destLevel;
        else
            destMaxLV = goMapInfo->maxLV;

        // 1654	현재 자신의 레벨에 따른 선택가능 레벨범위: %d ~ %d
        TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
        g_InterfaceManager.GetInterfaceString(1654, szTemp, INTERFACE_STRING_LENGTH);
        Snprintf (szText, INTERFACE_STRING_LENGTH-1, szTemp, destMinLV, destMaxLV);
        this->m_vMapDesc.push_back(szText);
        this->m_vMapDesc.push_back(_T("")); // 공백 추가
    }
    else
#endif
    {
        this->m_vMapDesc.push_back(_T("")); // 공백 추가
    }

    this->GetUIMan()->GetInterfaceString(goMapInfo->dwDCode, szText, INTERFACE_STRING_LENGTH );
    util::_tstring strMapDesc = szText;

    const TCHAR* cstrMapDesc;
    util::_tstring tempstrMapDescSpatial;
    std::vector<util::_tstring> strMapDescSpatial;
    cstrMapDesc = strMapDesc.c_str();
    tempstrMapDescSpatial.clear();
    for (size_t strIter = 0; strIter<strMapDesc.size(); ++strIter)
    {
        if (cstrMapDesc[strIter] == _T('#'))
        {
            strMapDescSpatial.push_back(tempstrMapDescSpatial);
            tempstrMapDescSpatial.clear();
        }
        else
        {
            tempstrMapDescSpatial += strMapDesc[strIter];
        }
    }

    if (tempstrMapDescSpatial.size())
    {
        strMapDescSpatial.push_back(tempstrMapDescSpatial);
        tempstrMapDescSpatial.clear();
    }

    if (strMapDescSpatial.size())
    {
        for (size_t i=0; i<strMapDescSpatial.size(); ++i)
            this->m_vMapDesc.push_back(strMapDescSpatial[i]);
    }
    else
    {
        this->m_vMapDesc.push_back(strMapDesc);
    }

}
//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateEp2::setDefaultRoomName(int iSelect/* = -1*/)
{
    int iStartString = 0;
    int iRoomType = 0;

    if(iSelect < 0)
    {
        iRoomType = m_cfgs.roomType;
    }
    else
    {
        iRoomType = iSelect;
    }

    switch (iRoomType)
    {
#ifdef _CN_EXCECPT_MISSION	
        //no operation
#else		
    case eCREATE_BATTLE_MISSION_TYPE: 
        {
            iStartString = c_iDefaultRoomNameStartIndex_Mission;
        }
        break;
#endif//_CN_EXCECPT_MISSION

#ifndef _EXCECPT_HUNTING_001011
    case eCREATE_BATTLE_CHUNTING: 
        { 
            iStartString = c_iDefaultRoomNameStartIndex_CHunting;
        } 
        break;
#endif

    case eCREATE_BATTLE_PVP_TYPE: 
        { 
            iStartString = c_iDefaultRoomNameStartIndex_PVP;
        } 
        break;

    default:
        return;	// 그 외에는 그냥 리턴
    }

    TCHAR szRoomName[INTERFACE_STRING_LENGTH] = {0,};
    int iCnt = 0;
    for(int i=iStartString; i<(iStartString+c_iDefaultRoomNameNumMax); ++i)
    {
        if(g_InterfaceManager.GetInterfaceStringWithCheck(i,szRoomName,INTERFACE_STRING_LENGTH))
        {
            ++iCnt;
        }
    }

    if(iCnt)
    {
        int iIndex = clock_function::GetTickCount() % iCnt;

        g_InterfaceManager.GetInterfaceString(iStartString+iIndex, szRoomName, INTERFACE_STRING_LENGTH);
        GlobalFunc::SetText(m_pEdtControls[EDT_TITLE], szRoomName);
    }
}

void uiBattleZoneCreateEp2::SetDefaultSuperMasterList()
{
    m_pListSuperMaster->DeleteItemAll();
    m_pListSuperMaster->SetSellHeight(24);

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    static int iItemData[6] = { eROOM_SPECIAL_PCBANG_SUPER_ROOM,eROOM_SPECIAL_ITEM1_SUPER_ROOM ,eROOM_SPECIAL_NORMAL ,
        eROOM_SPECIAL_PCBANG_SUPER_ROOM_GUILD_PVP ,eROOM_SPECIAL_ITEM1_SUPER_ROOM_GUILD_PVP ,eROOM_SPECIAL_GUILD_PVP };
#else
    static int iItemData[3] = { eROOM_SPECIAL_PCBANG_SUPER_ROOM,eROOM_SPECIAL_ITEM1_SUPER_ROOM ,eROOM_SPECIAL_NORMAL};
#endif

    TCHAR  szMessage[INTERFACE_STRING_LENGTH] = { 0,};
    int index = 0;
#ifndef __CHINA_PCBANG_SUPER_ROOM_BLOCK
    if( g_pHero->GetPCBangMode() )
    {
        index = m_pListSuperMaster->InsertItemBack();
        m_pListSuperMaster->SetItemData(index, &iItemData[0] );

        //155303	가맹 PC방
        g_InterfaceManager.GetInterfaceString(155303 , szMessage ,INTERFACE_STRING_LENGTH );
        GlobalFunc::SetItem( m_pListSuperMaster , index  , szMessage , StrToWzID("mn12") ,DT_CENTER | DT_BOTTOM ,
            c_InterfaceBKColor , c_InterfaceTextColor , c_InterfaceSelColor);
    }
#endif

    POSTYPE OutPos;

    if ( ItemManager::Instance()->FindActiveChargeItem( eITEMTYPE_SUPERMASTER, OutPos ) )
    {
        index = m_pListSuperMaster->InsertItemBack();
        m_pListSuperMaster->SetItemData(index, &iItemData[1] );

        // 155302	명지휘관의 지휘봉
        g_InterfaceManager.GetInterfaceString(155302 , szMessage ,INTERFACE_STRING_LENGTH);
        GlobalFunc::SetItem( m_pListSuperMaster , index , szMessage , StrToWzID("mn12") ,DT_CENTER | DT_BOTTOM ,
            c_InterfaceBKColor , c_InterfaceTextColor , c_InterfaceSelColor);
    }

    index = m_pListSuperMaster->InsertItemBack();
    m_pListSuperMaster->SetItemData(index, &iItemData[2] );

    //155301	기본 방
    g_InterfaceManager.GetInterfaceString(155301 , szMessage , INTERFACE_STRING_LENGTH );
    GlobalFunc::SetItem( m_pListSuperMaster , index , szMessage , StrToWzID("mn12") ,DT_CENTER | DT_BOTTOM ,
        c_InterfaceBKColor , c_InterfaceTextColor , c_InterfaceSelColor);

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
#ifndef _EXCECPT_HUNTING_001011
    if( m_cfgs.roomType == eCREATE_BATTLE_CHUNTING )
    {
        BOOL foundGuildPVPItem = ItemManager::Instance()->FindFirstItem(eITEMTYPE_GUILD_PVP_ENTRANCE, OutPos );
        if( foundGuildPVPItem )
        {
            if( g_pHero->GetPCBangMode() )
            {
                index = m_pListSuperMaster->InsertItemBack();
                m_pListSuperMaster->SetItemData(index, &iItemData[3] );
                // 717 PC방 길드 PVP
                g_InterfaceManager.GetInterfaceString(eST_PCBANG_GUILD_PVP , szMessage ,INTERFACE_STRING_LENGTH);
                GlobalFunc::SetItem( m_pListSuperMaster, index, szMessage, StrToWzID("mn12") ,DT_CENTER | DT_BOTTOM ,
                    c_InterfaceBKColor , c_InterfaceTextColor , c_InterfaceSelColor);
            }

            if ( ItemManager::Instance()->FindActiveChargeItem( eITEMTYPE_SUPERMASTER, OutPos ) )
            {
                index = m_pListSuperMaster->InsertItemBack();
                m_pListSuperMaster->SetItemData(index, &iItemData[4] );
                // 718 지휘봉 길드PVP
                g_InterfaceManager.GetInterfaceString(eST_ITEM_GUILD_PVP , szMessage , INTERFACE_STRING_LENGTH);
                GlobalFunc::SetItem( m_pListSuperMaster, index, szMessage , StrToWzID("mn12") ,DT_CENTER | DT_BOTTOM ,
                    c_InterfaceBKColor , c_InterfaceTextColor , c_InterfaceSelColor);
            }

            index = m_pListSuperMaster->InsertItemBack();
            m_pListSuperMaster->SetItemData(index, &iItemData[5] );
            // 716 길드 PVP
            g_InterfaceManager.GetInterfaceString(eST_GUILD_PVP, szMessage ,INTERFACE_STRING_LENGTH);
            GlobalFunc::SetItem( m_pListSuperMaster, index, szMessage, StrToWzID("mn12") ,  DT_CENTER | DT_BOTTOM ,
                c_InterfaceBKColor , c_InterfaceTextColor , c_InterfaceSelColor);
        }
    }
#endif
#endif 

    SetTextCurrentSuperMasterType(0);
}

void uiBattleZoneCreateEp2::SetTextCurrentSuperMasterType(int curSel)
{
    int * pcurType = (int *) m_pListSuperMaster->GetItemData(curSel);

    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
    switch (*pcurType )
    {
#ifndef __CHINA_PCBANG_SUPER_ROOM_BLOCK
    case eROOM_SPECIAL_PCBANG_SUPER_ROOM:
        {
            //155303	가맹 PC방
            g_InterfaceManager.GetInterfaceString(155303 , szMessage ,INTERFACE_STRING_LENGTH );
        }
        break;
#endif

    case eROOM_SPECIAL_ITEM1_SUPER_ROOM:
        {
            // 155302	명지휘관의 지휘봉
            g_InterfaceManager.GetInterfaceString(155302 , szMessage ,INTERFACE_STRING_LENGTH);
        }
        break;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    case eROOM_SPECIAL_GUILD_PVP:
        {
            // 716 길드 PVP
            g_InterfaceManager.GetInterfaceString(eST_GUILD_PVP, szMessage ,INTERFACE_STRING_LENGTH);
        }
        break;

    case eROOM_SPECIAL_PCBANG_SUPER_ROOM_GUILD_PVP:
        {
            // 717 PC방 길드 PVP
            g_InterfaceManager.GetInterfaceString(eST_PCBANG_GUILD_PVP , szMessage ,INTERFACE_STRING_LENGTH);
        }
        break;

    case eROOM_SPECIAL_ITEM1_SUPER_ROOM_GUILD_PVP:
        {
            // 718 지휘봉 길드PVP
            g_InterfaceManager.GetInterfaceString(eST_ITEM_GUILD_PVP , szMessage , INTERFACE_STRING_LENGTH);
        }
        break;
#endif

    default:
        {
            //155301	기본 방
            g_InterfaceManager.GetInterfaceString(155301 , szMessage , INTERFACE_STRING_LENGTH );
        }
        break;
    }
    GetManager()->SetRoomSpecialType(*pcurType );
    GlobalFunc::SetCaption( m_pTextSuperMaster , szMessage);
}

#pragma warning ( pop )

