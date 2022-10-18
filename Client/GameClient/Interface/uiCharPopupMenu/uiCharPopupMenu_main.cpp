#include "SunClientPrecompiledHeader.h"

#include "uiCharPopupMenu.h"
#include "uiCharPopupMan/uiCharPopupMan.h"

#include "Mouse.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#ifdef _NA_002935_20110704_ITEM_SCORE
#include "..\ObjectManager.h"
#endif //_NA_002935_20110704_ITEM_SCORE
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildRightInfoParser.h"
#include "uiTargetMan/uiTargetMan.h"
#include "TargetDialog.h"
#include "Hero.h"
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
  #include "uiPartyMan/uiPartyMan.h"
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
int uiCharPopupMenu::m_PopupMenuArray[ePMENU_MAX] =
{
    eST_NORMAL_WHISPER_STRING_INDEX,     // �ӼӸ�
#ifdef _NA_002935_20110704_ITEM_SCORE
    eST_NORMAL_EQUIP_SHOW_STRING_INDEX, // ��񺸱�
#endif //_NA_002935_20110704_ITEM_SCORE
    eST_NORMAL_FRIEND_STRING_INDEX,     // ģ���߰�
    eST_NORMAL_TRADE_STRING_INDEX,      // �ŷ�
    eST_NORMAL_FOLLOW_STRING_INDEX,     // ���󰡱�
#ifdef _KIKI_ADD_G_LOG_MENU
	eST_NORMAL_G_LOG_STRING_INDEX,		// G-�α�
#endif

	eST_GROUP_ADD,				// ������
	eST_GROUP_DEL,				// �������

    eST_PARTY_MASTER_INVITE_STRING_INDEX,       // ��Ƽ�ʴ�
    eST_PARTY_MASTER_MIGRATION_STRING_INDEX,    // ��������
	eST_PARTY_MASTER_KICK,						// ��Ƽ�߹�

#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
    eST_GUILD_INVITE,            // 21007 ����ʴ�
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL

    eST_BLOCK_ADD,  // ����
    eST_BLOCK_DELETE,//���� ����

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    eST_BATTLE_GROUND_VOTE, // ���� �߹���ǥ
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

	eST_CANCEL,
};

//------------------------------------------------------------------------------
WzID uiCharPopupMenu::m_wzId[DIALOG_MAX] = 
{
	StrToWzID("L001"), 
};

//------------------------------------------------------------------------------
/**
*/
uiCharPopupMenu::uiCharPopupMenu(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    m_pCharPopupMan = GET_DIALOG_MANAGER(uiCharPopupMan, UIMAN_CHAR_POPUP);
    assert (m_pCharPopupMan);

    if (!m_pCharPopupMan)
    {
#ifdef _DEBUG
		uiSystemMan * pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pCharPopupMan�� �����ϴ�."));

#endif
        ShowInterface(FALSE);
        return;
    }

    m_pListControl = NULL;
	m_vMenu.clear();
	m_iSaveType = -1;
	m_bSaveGroupType = false;
    block_member_type_ = false;
#ifdef _NA_002935_20110704_ITEM_SCORE
    show_equip_menu_ = false;
#endif //_NA_002935_20110704_ITEM_SCORE
	m_iListWidth = c_iListWIDTH;
	
}

//------------------------------------------------------------------------------
/**
*/
uiCharPopupMenu::~uiCharPopupMenu()
{
    Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCharPopupMenu::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCharPopupMenu::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiCharPopupMenu::InitControls()
{
    m_pListControl =
        static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_LST_MENU));
    assert (m_pListControl);

    m_bControlsInitialized = true;
    m_pListControl->SetSellHeight(c_iHEIGHT_Gap);
}


//------------------------------------------------------------------------------
void uiCharPopupMenu::OnShowWindow( BOOL val )
{
    if (val)
    {
		if (!m_bControlsInitialized)
        {
            ShowInterface(FALSE);
            return;
        }

		assert (GetManager());
		if (!GetManager())
			return;

#ifdef _DH_SHOW_VENDOR_LIST_POPUP
        //nop ������ ȣ��
#else
		GetManager()->CheckPopupType();
#endif//_DH_SHOW_VENDOR_LIST_POPUP

		_setListWidth();

		keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_char_popup_menu;
        msg.wParam = InterfaceManager::DIALOG_CHAR_POPUP_MENU;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);

		ClearListSelect();

        m_timerShowoffLater.Off();
        this->OffLock();
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_char_popup_menu);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiCharPopupMenu::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

	if (!GetManager())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_LST_MENU:
                {
					if(!m_vMenu.size()) break;

                    int cursel = m_pListControl->GetCurSel();
					
					if (cursel >= 0)
					{
						if (GetManager()->ProcessMenuFunc(m_vMenu[cursel]))
                        {
                            this->OnLock();
                            m_timerShowoffLater.OnFloat(0.1f);
                        }
					}
					break;
				}
            }
        }
        break;

	case RT_MSG_MOUSEOVER:
		{
			switch(getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_LST_MENU:
                {
					if(!m_vMenu.size()) break;

					int iMouseY = Mouse::Instance()->GetMouseY();
					RECT rc_listctrl = m_pListControl->GetSizeRect();
					int iPos = iMouseY - rc_listctrl.top;

					if(iPos > 0)
					{
						int cursel = iPos / c_iHEIGHT_Gap;
						m_pListControl->SetCurSel(cursel);
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
void uiCharPopupMenu::OnUpdateSolarDialog()
{
    if (IsLock())
        return;

	updateMenu();
}

//------------------------------------------------------------------------------
/**
*/
void uiCharPopupMenu::Process(DWORD dwTick)
{
    int timerStat = m_timerShowoffLater.Check();
    if (timerStat != TIMER_STAT_NONE)
    {
        if (timerStat == TIMER_STAT_FINISH)
        {
			if(this->IsVisible())
				ShowInterface(FALSE);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiCharPopupMenu::OnRenderSolarDialog()
{
    renderMenu();

	// ��ŵó�� 
    // �������̽����� Dialog ���������� Mouse Over�� ���� �ʴ´�.
    BOOL bMouseOver = FALSE;
    
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	RECT rc;
	GetDialogWZ()->GetClientRect(&rc);
	if (rc.left <= iMouseX && rc.right >= iMouseX)
    {
        if (rc.top <= iMouseY && rc.bottom >= iMouseY)
        {
            bMouseOver = TRUE;
        }
    }

    if ( !bMouseOver )
    {
		ClearListSelect();
    }
}


//------------------------------------------------------------------------------
/**
*/
void uiCharPopupMenu::NetworkProc( MSG_BASE * pMsg )
{

}

//------------------------------------------------------------------------------
void uiCharPopupMenu::updateMenu()
{
    bool do_update = false;
    assert (GetManager());
    if (!GetManager())
        return;

	if(GetManager()->GetPopupType() != m_iSaveType)
    {
        do_update = true;
    }
	else if(m_bSaveGroupType != GetManager()->GetGroupType())
    {
        do_update = true;
    }
    else if(block_member_type_ != GetManager()->block_type())
    {
        do_update = true;
    }
#ifdef _NA_002935_20110704_ITEM_SCORE
    else if(show_equip_menu_ != GetManager()->show_equip())
    {
        do_update = true;
    }
#endif //_NA_002935_20110704_ITEM_SCORE

    if (do_update == true)
	{
		m_iSaveType = GetManager()->GetPopupType();
		m_bSaveGroupType = GetManager()->GetGroupType();
        block_member_type_ = GetManager()->block_type();
#ifdef _NA_002935_20110704_ITEM_SCORE
        show_equip_menu_ = GetManager()->show_equip();
#endif //_NA_002935_20110704_ITEM_SCORE

		m_pListControl->DeleteItemAll();
		m_pListControl->SetBeginDrawIndex(0);
		m_pListControl->SetColorSelBar(c_dwBackColor);

		m_vMenu.clear();

		for (int i=0; i<uiCharPopupMenu::ePMENU_MAX; ++i)
		{
			if(_checkSkipMenu(i))
				continue;

			TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
			bool IsInterfaceString = g_InterfaceManager.GetInterfaceStringWithCheck( m_PopupMenuArray[i], szMessage, INTERFACE_STRING_LENGTH );
            if (!IsInterfaceString)
                continue;

			int idx = m_pListControl->InsertItemBack();
			
			GlobalFunc::SetItemText( m_pListControl, idx ,szMessage);

			m_pListControl->SetItemFontID( idx,  0, c_Font_popupMenu);

			m_pListControl->SetItemTextColor(
					idx, 
					0, 
					c_dwTextColor
					);

			m_pListControl->SetItemBKColor(
					idx, 
					0, 
					c_dwBackColor
					);

			m_pListControl->SetItemSelTextColor(
					idx, 
					0, 
					c_dwSelTextColor
					);

			m_vMenu.push_back(m_PopupMenuArray[i]);
		}

		RECT rc_listctrl = m_pListControl->GetSizeRect();
		int iListHeight = (m_vMenu.size()*c_iHEIGHT_Gap);
		RECT rc;

		GetDialogWZ()->GetClientRect(&rc);

		GetDialogWZ()->MoveWindowWZ((float)rc.left, (float)rc.top, (float)m_iListWidth+c_iDlgWIDTH, (float)(iListHeight + c_iHEIGHT_Gap));
		m_pListControl->MoveWindowWZ((float)rc.left + (float)c_iListLeftMARGIN, (float)(rc.top + c_iListTopMARGIN), (float)m_iListWidth+c_iListRightMARGIN, (float)iListHeight);
	}
}
//------------------------------------------------------------------------------
void uiCharPopupMenu::renderMenu()
{
    assert (GetManager());
    if (!GetManager())
        return;
}

//------------------------------------------------------------------------------
BOOL uiCharPopupMenu::_checkSkipMenu(int iMenuIndex)
{
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
    if (iMenuIndex == uiCharPopupMenu::ePMENU_GUILD_INVITE)
    {
        if (GuildSystem::GetGuildSystemData().is_recv_guild_info() == true)
        {
            bool is_guild = true;
            TargetDialog* target_dialog = 
                GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
            if (target_dialog != NULL)
            {
                Player* player_ptr = 
                    static_cast<Player*>(g_ObjectManager.GetObject(target_dialog->GetTargetKey()));
                if (player_ptr != NULL)
                {
                    is_guild = (strcmp(player_ptr->GetGuildName(), "") == 0) ? false:true;//��� �̸��� ""��� ���� �����, �ƴϸ� ���
                }                
            }
            
            bool have_right = false;
            if (g_pHero != NULL)
            {
                eGUILD_DUTY guild_duty = 
                    static_cast<eGUILD_DUTY>(g_pHero->GetGuildRenderInfo().m_GuildDuty);
                have_right = GuildRightInfoParser::Instance()->CheckDutyForRight(
                    eGUILD_RIGHTS_JOIN, guild_duty);
            }

            if ((is_guild == false) && //�ش� �÷��̾ ��尡 �ƴѰ��
                (have_right == true))
            {
                return FALSE;
            }
        }

        return TRUE;
    }
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
    if (iMenuIndex == uiCharPopupMenu::ePMENU_BLOCK_ADD)
    {
        if(GetManager()->IsTargetBlockMember())//���ܵ� ĳ���Ͷ��
        {
            return TRUE;//���� ���ϵ���
        }
        else
        {
            return FALSE;
        }

    }
    if (iMenuIndex == uiCharPopupMenu::ePMENU_BLOCK_DELETE)
    {
        if(GetManager()->IsTargetBlockMember())
		{
			return FALSE;
		}
		else
		{
			return TRUE;// ���ܵ� ĳ���Ͱ� �ƴ϶�� ���� ���ϵ��� �޴��� Skip
		}
    }
	// �׷� �߰� �Ұ���
	if(iMenuIndex == uiCharPopupMenu::ePMENU_GROUP_ADD)
	{	// �׷����� �߰��Ұ��� / �׷����ƴϸ� �߰�����
		if(GetManager()->IsTargetGroupMember())
		{
			return TRUE;// ����� �߰� ���ϵ��� �޴��� Skip
		}
		else
		{
			return FALSE;
		}
	}
	// �׷� ���� ����
	if(iMenuIndex == uiCharPopupMenu::ePMENU_GROUP_DEL)
	{	// �׷����� �������� / �׷����ƴϸ� �����Ұ���
		if(GetManager()->IsTargetGroupMember())
		{
			return FALSE;
		}
		else
		{
			return TRUE;// ����� �ƴϸ� ���� ���ϵ��� �޴��� Skip
		}
	}
#ifdef _NA_002935_20110704_ITEM_SCORE
    if (iMenuIndex == uiCharPopupMenu::ePMENU_SHOW_EQUIP)
    {
#ifdef _RU_0000_20120227_RUSSIA_BLOCK_ITEM_SCORE
        return TRUE;
#endif //_RU_0000_20120227_RUSSIA_BLOCK_ITEM_SCORE

        if(GetManager()->show_equip() == false)
        {
            return TRUE;
        }
    }
#endif //_NA_002935_20110704_ITEM_SCORE

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    if (iMenuIndex == uiCharPopupMenu::ePMENU_BATTLE_GROUND_VOTE)
    {
        TargetDialog* target_dialog = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
        if ((target_dialog == NULL) || (g_pHero == NULL))
        {
            return TRUE;
        }

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        if ((GlobalFunc::IsBattleGroundScene() == FALSE && GlobalFunc::IsGoldRushScene() == FALSE) ||
            (g_pHero->GetObjectKey() == target_dialog->GetTargetKey()))
#else
        if ((GlobalFunc::IsBattleGroundScene() == FALSE) ||
            (g_pHero->GetObjectKey() == target_dialog->GetTargetKey()))
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        {
            return TRUE;
        }

        uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
        if (party_manager && party_manager->IsPartyMember(target_dialog->GetTargetKey()))
        {
            return FALSE;
        }

        return TRUE;
    }
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

	if(GetManager()->GetPopupType() == eCPK_PARTY_MASTER)
	{
		//gs_ps:������ ��Ƽ��Ŭ���ÿ��� �ʴ�޴��¾�����Ѵ�.
		if(iMenuIndex==(uiCharPopupMenu::ePMENU_PARTY_MASTER_INVITE))
		{
			return TRUE;
		}

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
        if (iMenuIndex == uiCharPopupMenu::ePMENU_PARTY_MASTER_KICK)
        {
            if (GlobalFunc::IsBattleGroundScene() == TRUE
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                || GlobalFunc::IsGoldRushScene() == TRUE
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                )
            {
                // ���忡�� ��Ƽ�߹� ����
                return TRUE;
            }
            return FALSE;
        }
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
		
		return FALSE;
	}

    if(GetManager()->GetPopupType() == eCPK_PARTY)
    {
		//gs_ps:��Ƽ���ϰ�� ��Ƽ�ʴ��Ҽ��¾���
        if(iMenuIndex==(uiCharPopupMenu::ePMENU_PARTY_MASTER_INVITE))
		{
			return TRUE;
		}

		if (iMenuIndex > uiCharPopupMenu::ePMENU_PARTY_MASTER_INVITE && iMenuIndex < uiCharPopupMenu::ePMENU_PARTY_MASTER_KICK + 1)
            return TRUE;

        return FALSE;
    }

    // ��Ƽ�� �Ҽӵ��� �ʾ��� ���, �ʴ� �ɼ��׸��� Ư���� �޴��� �߰��ϵ��� �Ѵ�.
    if (iMenuIndex == uiCharPopupMenu::ePMENU_PARTY_MASTER_INVITE)
        return FALSE;

    if (iMenuIndex > uiCharPopupMenu::ePMENU_PARTY_MASTER_INVITE
        && iMenuIndex < uiCharPopupMenu::ePMENU_PARTY_MASTER_KICK + 1)
        return TRUE;

	return FALSE;
}

//------------------------------------------------------------------------------
void uiCharPopupMenu::_setListWidth()
{	
	int	iListWidth = c_iListWIDTH;

	for (int i=0; i<uiCharPopupMenu::ePMENU_MAX; ++i)
	{
		if(_checkSkipMenu(i))
			continue;

		TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
		bool IsInterfaceString = g_InterfaceManager.GetInterfaceStringWithCheck( m_PopupMenuArray[i], szMessage, INTERFACE_STRING_LENGTH );
        if (!IsInterfaceString)
            continue;

		SIZE	TextSize;
		int iTextLength = (int)_tcslen(szMessage);
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont(c_Font_popupMenu);
		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szMessage, iTextLength, &TextSize );
		g_pSunRenderer->x_pManagerTextOut->EndUseFont();

		if( iListWidth < TextSize.cx )
		{
			iListWidth = TextSize.cx;
		}
	}
	
	m_iListWidth = iListWidth;
}
