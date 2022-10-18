#include "SunClientPrecompiledHeader.h"

#include "ItemShareSystem.h"
#include "GameConst_Color.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiPartyMember/uiPartyMember.h"
#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2Lobby/uiBattlezone2Lobby.h"
#endif//#ifdef _DH_BATTLEZONE2_
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
  #include "map.h"
  #include "BattlezoneMissionTreeParser.h"
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
#else
  #define LIST_START_STRING_ID 286
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

WzID ItemShareSystem::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("LS00"),	//eDIALOG_LIST
	StrToWzID("BT00"),	//eDIALOG_BTN,
    StrToWzID("PI00"),  //eDIVIDE_ITEM_BACK_IMAGE
    StrToWzID("ST00"),  //eDIVIDE_ITEM_TEXT,
	StrToWzID("PI01"),	//ePTBACK_IMG,
	StrToWzID("BT01"),	//ePTWIN_VIEW_OPT_BTN,
	StrToWzID("CT00"),	//eCHK_ALL,
	StrToWzID("CT01"),	//eCHK_FACE,
	StrToWzID("CT02"),	//eCHK_HP_MP,
	StrToWzID("CT03"),	//eCHK_BUF,
	StrToWzID("ST01"),	//eTXT_ALL,
	StrToWzID("ST02"),	//eTXT_FACE,
	StrToWzID("ST03"),	//eTXT_HP_MP,
	StrToWzID("ST04"),	//eTXT_BUF,
#ifdef _NA_000000_20130114_RENEWER_UI
	StrToWzID("P002"), //ePICTURE_P002
#endif
};

DWORD ItemShareSystem::distribution_type_string_code[eITEM_DISTRIBUTION_MAX] = 
{
    286,    // ���� ȹ��            DISTRIBUTION_TYPE_DEFAULT
    287,    // ���ʴ�� ȹ��        DISTRIBUTION_TYPE_FIFO_PRIORITY
    288,    // ��Ƽ�常 ȹ��        DISTRIBUTION_TYPE_MASTER_PRIORITY
    289,    // ���� ȹ��            DISTRIBUTION_TYPE_RANDOM
    290,    // ������ �켱 ���� ȹ�� DISTRIBUTION_TYPE_CLASS_PRIORITY
    2912,   // �ֻ��� ���� ȹ��     DISTRIBUTION_TYPE_DICE_AND_DEFAULT
    2913    // �ֻ��� ���ʴ�� ȹ�� DISTRIBUTION_TYPE_DICE_AND_FIFO
};

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
UINT ItemShareSystem::distribution_type_list[eITEM_DISTRIBUTION_MAX] = 
{
    //����Ʈ ��Ʈ�ѿ� �� ����
    eITEM_DISTRIBUTION_DICE_AND_DEFAULT,    // �ֻ��� ���� ȹ��
    eITEM_DISTRIBUTION_DICE_AND_FIFO,       // �ֻ��� ���ʴ�� ȹ��
    eITEM_DISTRIBUTION_DEFAULT,             // ���� ȹ��
    eITEM_DISTRIBUTION_FIFO_PRIORITY,       // ���ʴ�� ȹ��
    eITEM_DISTRIBUTION_MASTER_PRIORITY,     // ��Ƽ�常 ȹ��
    eITEM_DISTRIBUTION_RANDOM,              // ���� ȹ��
    eITEM_DISTRIBUTION_CLASS_PRIORITY       // ������ �켱 ���� ȹ��
};
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
//------------------------------------------------------------------------------
/**
*/
ItemShareSystem::ItemShareSystem(InterfaceManager * pUIMan) : uiBase(pUIMan) , m_bPartyLeader(FALSE)
{

}
//------------------------------------------------------------------------------
/**
*/
ItemShareSystem::~ItemShareSystem()
{

}
//------------------------------------------------------------------------------
/**
*/
void ItemShareSystem::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	m_pList = (CCtrlListWZ *) getControl(eDIALOG_LIST);
	assert(m_pList);

	m_pList->ShowWindowWZ(WZ_HIDE);
	m_iShareType = 0;
	InitList();

	int i = 0;
	m_bIsShowPtViewOption = false;
	m_pPicture = (CCtrlPictureWZ*) getControl(ePTBACK_IMG);
	assert(m_pPicture && "Can not find background image for 'PARTY_VIEWOPT'");	// ��Ƽ ���� �ɼ� ��� �׸� ��Ʈ���� �����ϴ�.

	m_pBtnOptView = (CCtrlButtonWZ*) getControl(ePTWIN_VIEW_OPT_BTN);
	assert(m_pBtnOptView && "Can not find button for 'PARTY_VIEWOPT'");	// ��Ƽ ���� �ɼ� ��ư ��Ʈ���� �����ϴ�.

	for(i = OPTCHK_ALL; i < PTVIEWOPT_CHECK_MAX; ++i)
	{
		m_pBtnChecks[i] = (CCtrlButtonCheckWZ*) getControl(eCHK_ALL + i);
		m_pTxtCtrls[i] = (CCtrlStaticWZ*) getControl(eTXT_ALL + i);

		assert(m_pBtnChecks && "Can not find check button control for 'PARTY_VIEWOPT'");// ��Ƽ ���� �ɼ� üũ ��ư ��Ʈ���� �����ϴ�.
		assert(m_pTxtCtrls && "Can not find static text control for 'PARTY_VIEWOPT'");	// ��Ƽ ���� �ɼ� �ؽ�Ʈ ��Ʈ���� �����ϴ�.
	}

    m_pItemDivideBack = (CCtrlPictureWZ*) getControl(eDIVIDE_ITEM_BACK_IMAGE);
    assert(m_pItemDivideBack);
    m_pItemDivideText = (CCtrlStaticWZ*) getControl(eDIVIDE_ITEM_TEXT);
    assert(m_pItemDivideText);
    m_pItemDivideButton = (CCtrlButtonWZ*) getControl(eDIALOG_BTN);
    assert(m_pItemDivideButton);
    m_pItemDivideList = (CCtrlListWZ*) getControl(eDIALOG_LIST);
    assert(m_pItemDivideList);

	m_bIsDisplayDivOpt = true;
    m_bIsVisibilityDivOpt = false;

#ifdef _NA_000000_20130114_RENEWER_UI
	CCtrlPictureWZ* showPickUp = (CCtrlPictureWZ*) getControl(ePICTURE_P002);
	if(showPickUp)
	{
		showPickUp->ShowWindowWZ(WZ_HIDE);
	}
	
#endif
    GetCtrlSelectName()->SetTextKind(ETK_SHADOW);
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    party_obtain_room_ = false;
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
}
//------------------------------------------------------------------------------
/**
*/
void ItemShareSystem::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void ItemShareSystem::Process(DWORD dwTick)
{

}

//------------------------------------------------------------------------------
/**
*/
void ItemShareSystem::ShowViewOptControls(bool bSetVal)
{
	int i = 0;

	if(bSetVal == true)
	{
		if(m_pPicture)
		{
			m_pPicture->ShowWindowWZ(WZ_SHOW);
		}

		for(i = OPTCHK_ALL; i < PTVIEWOPT_CHECK_MAX; ++i)
		{
			if(m_pBtnChecks[i])
			{
				m_pBtnChecks[i]->ShowWindowWZ(WZ_SHOW);	
			}
			if(m_pTxtCtrls[i])
			{
				m_pTxtCtrls[i]->ShowWindowWZ(WZ_SHOW);	
			}
		}
	}
	else
	{
		if(m_pPicture)
		{
			m_pPicture->ShowWindowWZ(WZ_HIDE);
		}

		for(i = OPTCHK_ALL; i < PTVIEWOPT_CHECK_MAX; ++i)
		{
			if(m_pBtnChecks[i])
			{
				m_pBtnChecks[i]->ShowWindowWZ(WZ_HIDE);	
			}
			if(m_pTxtCtrls[i])
			{
				m_pTxtCtrls[i]->ShowWindowWZ(WZ_HIDE);	
			}
		}
	}
	UpdateDialogSize();
}

//------------------------------------------------------------------------------
/**
*/
void ItemShareSystem::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_pList->ShowWindowWZ(WZ_HIDE);
		UpdateDialogSize();
#ifdef _DH_BATTLEZONE2_
        uiBattlezone2Lobby* battlezone2_lobby = GET_CAST_DIALOG(uiBattlezone2Lobby, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);
        if (battlezone2_lobby && battlezone2_lobby->IsVisible())
        {
            g_InterfaceManager.AttachFirstDialog(IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);
            g_InterfaceManager.DeAttachFirstDialog(IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);
        }
#endif//_DH_BATTLEZONE2_
	}

	if( val )
	{
		ShowViewOptControls(false);	// �����
	}
}

void ItemShareSystem::CloseItemShareSystemWindow()
{
	if(m_pList)
	{
		m_pList->ShowWindowWZ(WZ_HIDE);
	}
	UpdateDialogSize();
}
//------------------------------------------------------------------------------
/**
*/
void ItemShareSystem::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getControlIDToPos(pMessage->dwCtrlID) )
			{
			case eDIALOG_LIST:
				{
					if( m_pList->GetCurSel() != -1 )
					{
						m_pList->ShowWindowWZ(WZ_HIDE);
						UpdateDialogSize();
						NET_SEND_CG_PARTY_ITEM_DISTIRIBUTE_CHANGE_SYN();
					}
				}
				break;

			case eDIALOG_BTN:
				{
					// ��Ƽ�常�� ���� ��ų�� �ִ�.
					if(m_bPartyLeader)
					{
						if( m_pList->GetState() == WZ_SHOW )
							m_pList->ShowWindowWZ(WZ_HIDE);
						else
							m_pList->ShowWindowWZ(WZ_SHOW);

						UpdateDialogSize();
					}
					else
					{
						TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
						//5334	��Ƽ���� �ƴմϴ�.
						g_InterfaceManager.GetInterfaceString(5334, szMessage ,INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
				}
				break;
			case ePTWIN_VIEW_OPT_BTN:
				{
#ifdef _NA_000000_20130114_RENEWER_UI
					CCtrlPictureWZ* showPickUp = (CCtrlPictureWZ*) getControl(ePICTURE_P002);
					if(showPickUp)
					{
						ENUM_STATEWZ state = showPickUp->GetState();
						showPickUp->ShowWindowWZ(state == WZ_HIDE ? WZ_SHOW : WZ_HIDE);
					}
#endif
					ShowViewOptControls(ToggleShowViewOpt());
				}
				break;
			case eCHK_ALL:
				{
					if(m_pBtnChecks[OPTCHK_ALL])
					{
						DoAllCheckBtn(m_pBtnChecks[OPTCHK_ALL]->GetCheckState());
					}
				}
				break;
			case eCHK_FACE:
				{
					uiPartyMan* partyMan = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY);				
					if(partyMan && partyMan->IsParty())
					{
						if(partyMan->GetShowFace())
						{
							partyMan->SetShowFace(false);
						}
						else
						{
							partyMan->SetShowFace(true);
						}
					}
					SetAllCheckBtn(GetAllCheckBtn());
				}
				break;
			case eCHK_HP_MP:
				{
					uiPartyMan* partyMan = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY);				
					if(partyMan && partyMan->IsParty())
					{
						if(partyMan->GetShowGague())
						{
							partyMan->SetShowGague(false);
						}
						else
						{
							partyMan->SetShowGague(true);
						}
					}
					SetAllCheckBtn(GetAllCheckBtn());
				}
				break;
			case eCHK_BUF:
				{
					uiPartyMan* partyMan = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY);				
					if(partyMan && partyMan->IsParty())
					{
						if(partyMan->GetShowBuf())
						{
							partyMan->SetShowBuf(false);
						}
						else
						{
							partyMan->SetShowBuf(true);
						}
					}
					SetAllCheckBtn(GetAllCheckBtn());
				}
				break;
			}
		}
		break;		
	}
}

//------------------------------------------------------------------------------
/**
*/
bool ItemShareSystem::GetAllCheckBtn()
{
	uiPartyMan* partyMan = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY);				
	if(partyMan && partyMan->IsParty())
	{
		if(!partyMan->GetShowBuf() && !partyMan->GetShowFace() && !partyMan->GetShowGague())
		{
			return true;
		}
	}
	return false;
}
void ItemShareSystem::SetAllCheckBtn(bool val)
{
	if(m_pBtnChecks[OPTCHK_ALL])
	{
		m_pBtnChecks[OPTCHK_ALL]->SetCheckState(val);
	}

	uiPartyMan* partyMan = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY);
	{
		partyMan->uiSetPartView();
	}
}

void ItemShareSystem::DoAllCheckBtn(bool val)
{
	uiPartyMan* partyMan = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY);				
	if(partyMan && partyMan->IsParty())
	{
		partyMan->SetShowBuf(!val);
		partyMan->SetShowFace(!val);
		partyMan->SetShowGague(!val);
		partyMan->uiSetPartView();
	}

	if (m_pBtnChecks[OPTCHK_FACE])
	{
		m_pBtnChecks[OPTCHK_FACE]->SetCheckState(val);
	}
	if (m_pBtnChecks[OPTCHK_HP_MP])
	{
		m_pBtnChecks[OPTCHK_HP_MP]->SetCheckState(val);
	}
	if (m_pBtnChecks[OPTCHK_BUF])
	{
		m_pBtnChecks[OPTCHK_BUF]->SetCheckState(val);
	}
}
//------------------------------------------------------------------------------
/**
*/
void ItemShareSystem::NetworkProc(MSG_BASE * pMsg)
{

}

void ItemShareSystem::InitList()
{
	m_pList->DeleteItemAll();
	m_pList->SetColorSelBar(RGBA (0,0,0,0));

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    m_pList->SetSellHeight(18);
    DWORD distribution_string;
    TCHAR message[INTERFACE_STRING_LENGTH] ={0,};
    for (int i = 0; i < eITEM_DISTRIBUTION_MAX; ++i)
    {
        distribution_string = distribution_type_list[i];
        g_InterfaceManager.GetInterfaceString(
            ItemShareSystem::distribution_type_string_code[distribution_string],
            message,
            INTERFACE_STRING_LENGTH);
        int index = m_pList->InsertItemBack();
#ifdef _INTERNATIONAL_UI
        GlobalFunc::SetItem(m_pList, index, message, StrToWzID("st10"), DT_CENTER | DT_BOTTOM,
            c_InterfaceBKColor, c_InterfaceTextColor, c_InterfaceSelColor);
#else	
        GlobalFunc::SetItem(m_pList, index, message, StrToWzID("mn12"), DT_CENTER | DT_BOTTOM,
            c_InterfaceBKColor, c_InterfaceTextColor, c_InterfaceSelColor);
#endif//_INTERNATIONAL_UI
    }

    distribution_string = distribution_type_list[0];
    g_InterfaceManager.GetInterfaceString(
        ItemShareSystem::distribution_type_string_code[distribution_string],
        message,
        INTERFACE_STRING_LENGTH);
    SetCaptionSelectName(message); // Macro�Դϴ�.
#else
    for ( int i = 0; i < 5; i++ )
    {
        TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
        g_InterfaceManager.GetInterfaceString(LIST_START_STRING_ID +i , szMessage , INTERFACE_STRING_LENGTH);
        int index = m_pList->InsertItemBack();
#ifdef _INTERNATIONAL_UI
        GlobalFunc::SetItem(m_pList, index , szMessage , StrToWzID("st10") ,    DT_CENTER | DT_BOTTOM  ,
            c_InterfaceBKColor ,c_InterfaceTextColor , c_InterfaceSelColor);
#else	
        GlobalFunc::SetItem(m_pList, index , szMessage , StrToWzID("mn12") ,    DT_CENTER | DT_BOTTOM  ,
            c_InterfaceBKColor ,c_InterfaceTextColor , c_InterfaceSelColor);
#endif//_INTERNATIONAL_UI
    }

    TCHAR szShareType[INTERFACE_STRING_LENGTH] = {0,};
    g_InterfaceManager.GetInterfaceString(LIST_START_STRING_ID, szShareType , INTERFACE_STRING_LENGTH);
    SetCaptionSelectName(szShareType); // Macro�Դϴ�.
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
}


void ItemShareSystem::SetToggleName(int SelType)
{
	TCHAR szShareType[INTERFACE_STRING_LENGTH] = {0,};
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    if (SelType < eITEM_DISTRIBUTION_MAX)
    {
        g_InterfaceManager.GetInterfaceString(distribution_type_string_code[SelType],
        szShareType,
        INTERFACE_STRING_LENGTH);
    }

    for (int i = 0; i<eITEM_DISTRIBUTION_MAX; ++i)
    {
        if (SelType == distribution_type_list[i])
        {
            m_pList->SetCurSel(i);
        }
    }
#else
	g_InterfaceManager.GetInterfaceString(LIST_START_STRING_ID + SelType ,
        szShareType ,
        INTERFACE_STRING_LENGTH);
    m_pList->SetCurSel(SelType);
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
	SetCaptionSelectName(szShareType); // Macro�Դϴ�.
	m_iShareType = SelType;
	
	if(GetDisplayDivOpt() == true)		// �̼��� �ƴ� ��츸 ���
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,} , szResult[INTERFACE_STRING_LENGTH] = {0,};
		//5219	������ �й� ����� %s�� ����Ǿ����ϴ�.
		g_InterfaceManager.GetInterfaceString(5219, szMessage, INTERFACE_STRING_LENGTH);
		Snprintf(szResult, INTERFACE_STRING_LENGTH -1 , szMessage , szShareType );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szResult);

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        if (party_obtain_room_ == true)
        {
            // 5248 (���� ��Ʋ���� ��Ƽ�� ��ü ȹ���̹Ƿ�, �ʵ忡 �ִ� ��Ƽ�����Ը� ���� �˴ϴ�.)
            g_InterfaceManager.GetInterfaceString(5248, szResult, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szResult);
        }
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
	}

#ifdef _DH_BATTLEZONE2_
    //��Ʋ�� �κ��� �˸���.
    uiBattlezone2Man* ui_battlezone2_manager_ =
        GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
    if (ui_battlezone2_manager_)
    {
        ui_battlezone2_manager_->SetShareType(SelType);
    }
#endif //_DH_BATTLEZONE2_
}

void ItemShareSystem::NET_SEND_CG_PARTY_ITEM_DISTIRIBUTE_CHANGE_SYN()
{
	MSG_CG_PARTY_ITEM_DISTRIBUTION_CHANGE_SYN SendMsg;
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    int index = m_pList->GetCurSel();
    SendMsg.m_byItemDistributionType = distribution_type_list[index];
#else
	SendMsg.m_byItemDistributionType = m_pList->GetCurSel();
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg , sizeof(SendMsg) );
}


void ItemShareSystem::UpdateDialogSize()
{
	RECT DialogRect;
	GetDialogWZ()->GetClientRect(&DialogRect);

	LONG maxHeight = DialogRect.top + 20;
	LONG maxWidth  = DialogRect.right;
	RECT rcDisableRect = {DialogRect.left, DialogRect.top + 20, DialogRect.left, DialogRect.top + 20};	// ��ȿȭ ����
	
	if(m_pBtnOptView)	// ��Ƽ���� �ɼ� ��ư�� �ִٸ� �ɼ� ��ư�� �������� maxWidth��
	{
		maxWidth = m_pBtnOptView->GetSizeRect().right;
		rcDisableRect.right = m_pBtnOptView->GetSizeRect().left;
	}

	if( m_pList->GetState() == WZ_SHOW )
	{
		maxHeight = m_pList->GetSizeRect().bottom;
		rcDisableRect.left = m_pList->GetSizeRect().right;
		rcDisableRect.bottom  = maxHeight;
	}
	if(GetShowViewOpt() == true)
	{
		if(maxHeight < m_pPicture->GetSizeRect().bottom)	// ���� ū bottom���� ����
		{
			maxHeight = m_pPicture->GetSizeRect().bottom;
			rcDisableRect.bottom  = maxHeight;
		}
		maxWidth = m_pPicture->GetSizeRect().right;
	}

	DialogRect.right = maxWidth;
	DialogRect.bottom = maxHeight;

	DisableInterSectRect(rcDisableRect);

	this->GetDialogWZ()->SetDlgPartSizeRect(DialogRect);
}

void ItemShareSystem::ShowDivideItemControls(ENUM_STATEWZ visibility)
{
    if (m_bIsVisibilityDivOpt == visibility)// ����� ��츸 �����Ѵ�.
    {
        return;
    }
    m_bIsVisibilityDivOpt = visibility;
    // visibility (WZ_SHOW, WZ_HIDE)
    if (m_pItemDivideButton != NULL)
    {
        m_pItemDivideButton->ShowWindowWZ(visibility);
    }
    if (m_pItemDivideList != NULL)
    {
        m_pItemDivideList->ShowWindowWZ(WZ_HIDE);//����Ʈ�� ������ default�� �����.
    }
    if (m_pItemDivideBack != NULL)
    {
        m_pItemDivideBack->ShowWindowWZ(visibility);
    }
    if (m_pItemDivideText != NULL)
    {
        m_pItemDivideText->ShowWindowWZ(visibility);
    }
}

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
void ItemShareSystem::SetPartyObtainRoom(bool val)
{
    party_obtain_room_ = val;

    // ���� ��ġ�� ��Ƽ�����ȹ�� ���̸� �ؽ�Ʈ ȸ��ó��
    if (val == true)
    {
        m_pItemDivideText->SetTextColor(c_InterfaceTextColor);
    }
    else
    {
        m_pItemDivideText->SetTextColor(c_InterfaceSelColor);
    }
}
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN