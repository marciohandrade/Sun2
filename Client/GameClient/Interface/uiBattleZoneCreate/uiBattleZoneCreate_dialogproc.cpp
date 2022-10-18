//------------------------------------------------------------------------------
//  uiBattleZoneCreate_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZoneCreate.h"
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
#include "Hero.h"

//------------------------------------------------------------------------------
WzID uiBattleZoneCreate::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("B999"),  // DLGPOS_BTN_EXIT=0,
    StrToWzID("TI02"),  // DLGPOS_BTN_CREATE,
    StrToWzID("TI03"),  // DLGPOS_BTN_CANCEL,
    StrToWzID("B000"),  // DLGPOS_BTN_PEOPLE_LIMIT,
    StrToWzID("B001"),  // DLGPOS_BTN_ROOM_TYPE,

    StrToWzID("C001"),  // DLGPOS_CHKBTN_CLASS_B,  // ����Ŀ
    StrToWzID("C002"),  // DLGPOS_CHKBTN_CLASS_D,  // �巡�� ����Ʈ
    StrToWzID("C003"),  // DLGPOS_CHKBTN_CLASS_V,  // ��Ű��
    StrToWzID("C004"),  // DLGPOS_CHKBTN_CLASS_E,  // ������Ż ����Ʈ
	StrToWzID("C005"),	// DLGPOS_CHKBTN_CLASS_S,  // ������

    StrToWzID("M101"),  // DLGPOS_CHKBTN_MAP_FOREST,			// �߼����� ��
    StrToWzID("M102"),  // DLGPOS_CHKBTN_MAP_UNDERWAY,			// ���ڷ��� ���ϼ���
    StrToWzID("M201"),  // DLGPOS_CHKBTN_MAP_FROZENPALACE,		// ������ ����	
    StrToWzID("M202"),  // DLGPOS_CHKBTN_MAP_VALLYOFDRAGON,		// ������ ���
	StrToWzID("M203"),	//	DLGPOS_CHKBTN_MAP_PUBLIC_CEMETERY,	// ��������
	StrToWzID("M301"),	//	DLGPOS_CHKBTN_MAP_HELLON_TOWER,		// ����� ��
	StrToWzID("M001"),	// DLGPOS_CHKBTN_MAP_TUTORIAL,			// Ʃ�丮��

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

	StrToWzID("CT00"),//DLGPOS_CHKBTN_MAP_0,
	StrToWzID("CT01"),//DLGPOS_CHKBTN_MAP_1,
	StrToWzID("CT02"),//DLGPOS_CHKBTN_MAP_2,
	StrToWzID("CT03"),//DLGPOS_CHKBTN_MAP_3,
	StrToWzID("CT04"),//DLGPOS_CHKBTN_MAP_4,
	StrToWzID("CT05"),//DLGPOS_CHKBTN_MAP_5,
	StrToWzID("CT07"),//DLGPOS_CHKBTN_MAP_6,
	StrToWzID("CT08"),//DLGPOS_CHKBTN_MAP_7,
	StrToWzID("CT09"),//DLGPOS_CHKBTN_MAP_8,
	StrToWzID("CT10"),//DLGPOS_CHKBTN_MAP_9,
	StrToWzID("CT11"),//DLGPOS_CHKBTN_MAP_10,
	StrToWzID("CT12"),//DLGPOS_CHKBTN_MAP_11,
	StrToWzID("CT13"),//DLGPOS_CHKBTN_MAP_12,
	StrToWzID("CT14"),//DLGPOS_CHKBTN_MAP_13,
	StrToWzID("CT15"),//DLGPOS_CHKBTN_MAP_14,
	StrToWzID("CT16"),//DLGPOS_CHKBTN_MAP_15,
	StrToWzID("CT17"),//DLGPOS_CHKBTN_MAP_16,
	StrToWzID("CT18"),//DLGPOS_CHKBTN_MAP_17,
	StrToWzID("CT19"),//DLGPOS_CHKBTN_MAP_18,
	StrToWzID("CT20"),//DLGPOS_CHKBTN_MAP_19,

	StrToWzID("CT21"),//DLGPOS_CHKBTN_MAP_20,
	StrToWzID("CT22"),//DLGPOS_CHKBTN_MAP_21,
	StrToWzID("CT23"),//DLGPOS_CHKBTN_MAP_22,
	StrToWzID("CT24"),//DLGPOS_CHKBTN_MAP_23,
	StrToWzID("CT25"),//DLGPOS_CHKBTN_MAP_24,
	StrToWzID("CT26"),//DLGPOS_CHKBTN_MAP_25,

	StrToWzID("CT06"),//DLGPOS_CHKBTN_MAP_26,

	StrToWzID("CT27"),//DLGPOS_CHKBTN_MAP_27,
	StrToWzID("CT28"),//DLGPOS_CHKBTN_MAP_28,
	StrToWzID("CT29"),//DLGPOS_CHKBTN_MAP_29,
	StrToWzID("CT30"),//DLGPOS_CHKBTN_MAP_30,
};

//------------------------------------------------------------------------------
/**
called to show request
*/
void
uiBattleZoneCreate::OnShowWindow( BOOL val )
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

        // TODO: �̰� ����� �����ϴ°�?
        g_HeroInput.ClearPickingTarget();

        // ��Ʋ �� ��� UI Ȱ��ȭ �� ��� �� �������� OFF
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

        // �� ������ On
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
//	DESC: ��Ʈ�ѷ� �ε������� �ش�������� �����Ѵ�.
//=====================================================================================
POSTYPE
uiBattleZoneCreate::getMapAreaInfo(int nPos)
{
	POSTYPE MapArea = eCHKMAP_MAX;
	switch( nPos )
	{
	case DLGPOS_CHKBTN_MAP_0: //Ʃ�丮��
	case DLGPOS_CHKBTN_MAP_1:
	case DLGPOS_CHKBTN_MAP_2:
	case DLGPOS_CHKBTN_MAP_3: MapArea = eCHKMAP_TUTORIAL;break;

	case DLGPOS_CHKBTN_MAP_4: //�߽�
	case DLGPOS_CHKBTN_MAP_5:
	case DLGPOS_CHKBTN_MAP_6:
	case DLGPOS_CHKBTN_MAP_7: MapArea = eCHKMAP_FOREST;break;

	case DLGPOS_CHKBTN_MAP_8: //���ϼ���
	case DLGPOS_CHKBTN_MAP_9:
	case DLGPOS_CHKBTN_MAP_10:
	case DLGPOS_CHKBTN_MAP_11:
	case DLGPOS_CHKBTN_MAP_26:
		MapArea = eCHKMAP_UNDERWAY;break;

	case DLGPOS_CHKBTN_MAP_12: //��������
	case DLGPOS_CHKBTN_MAP_13:
	case DLGPOS_CHKBTN_MAP_14:
	case DLGPOS_CHKBTN_MAP_15: MapArea = eCHKMAP_FROZENPALACE;break;

	case DLGPOS_CHKBTN_MAP_16: //ȭ��
	case DLGPOS_CHKBTN_MAP_17:
	case DLGPOS_CHKBTN_MAP_18:
	case DLGPOS_CHKBTN_MAP_19: MapArea = eCHKMAP_VALLYOFDRAGON;break;

	case DLGPOS_CHKBTN_MAP_20://���̿�Ʈ
	case DLGPOS_CHKBTN_MAP_21:
	case DLGPOS_CHKBTN_MAP_22:
	case DLGPOS_CHKBTN_MAP_23:
	case DLGPOS_CHKBTN_MAP_24:
	case DLGPOS_CHKBTN_MAP_25:	MapArea = eCHKMAP_PUBLIC_CEMETERY;break;

	case DLGPOS_CHKBTN_MAP_27://����� ��
	case DLGPOS_CHKBTN_MAP_28:
	case DLGPOS_CHKBTN_MAP_29:
	case DLGPOS_CHKBTN_MAP_30:	MapArea = eCHKMAP_HELLON_TOWER;break;
	}

	return MapArea; 
}
//=====================================================================================
//	DESC: ��Ʋ�� ����â���� �ش����� Ŭ���� �������� Ȱ��ȭ��Ų��.
//=====================================================================================
void
uiBattleZoneCreate::clickMapArea(POSTYPE pos)
{
	POSTYPE MapArea = getMapAreaInfo(pos);

	if(MapArea==eCHKMAP_MAX)//��üũ��ư�� �ƴϴ�
		return;

	bool canChoice = true;
	
	if (!setMapType(MapArea))
	{
		canChoice = false;

		for (int i=0; i<eCHKMAP_MAX; ++i)
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
		m_cfgs.mapType = eCHKMAP_MAX;
	}
}

//=====================================================================================
//	DESC: ��Ʋ�� �ʿ���â�� �������� ������Ʈ���� �������ش�.
//=====================================================================================
void
uiBattleZoneCreate::checkMapMouseOver()
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
uiBattleZoneCreate::MessageProc( SI_MESSAGE * pMessage )
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
			
			clickMapArea(pos);//�ش�����Ȱ��ȭ

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
						// �������� �ִ� ������(20 byte)�� �Ѿ����ϴ�.
						if(!GlobalFunc::CompareLimitText(m_pEdtControls[EDT_TITLE], m_cfgs.strTitle, MAX_ROOMTITLE_LENGTH-1, 5791))
							break;
					}

					// ��й�ȣ�� �ִ� ������(7 byte)�� �Ѿ����ϴ�.
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
						// �����ܾ� check
						if(GlobalFunc::IsSlang(m_cfgs.strTitle, false) == TRUE)
						{
							this->GetUIMan()->GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, MAX_MESSAGE_LENGTH );

							uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
							if(pSystemMan)
								pSystemMan->Sys_VerifyLock(szMessage);

							break;
						}
						//! ��Ʋ�� ������ ���� ����
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
uiBattleZoneCreate::OnUpdateSolarDialog()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	if (m_cfgs.mapType == eCHKMAP_MAX)
	{
		// �������ѿ� ���� ������ �� �ִ� ���� �������� �ʽ��ϴ�.\n�ٸ� ��带 �����Ͻðų� ���� �̵��� ���� �ڽ��� ������ �´� ���� �������ּ��� : 5623
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
uiBattleZoneCreate::OnRenderSolarDialog()
{
    this->renderRoomTypeDescription();

	renderTooltip();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreate::renderTooltip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

    // ��ŵó�� 
    // �������̽����� Dialog ���������� Mouse Over�� ���� �ʴ´�.
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

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
