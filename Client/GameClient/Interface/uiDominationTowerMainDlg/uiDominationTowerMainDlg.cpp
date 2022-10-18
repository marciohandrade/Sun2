#include "SunClientPrecompiledHeader.h"

#include "uiDominationTowerMainDlg.h"
#include "uiDominationTowerBettingListDlg/uiDominationTowerBettingListDlg.h"
#include "uiDominationTowerManagerDlg/uiDominationTowerManagerDlg.h"
#include "uiSystemMan/uiSystemMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "Hero.h"
#include "Map.h"
#include "MapInfoParser.h"
#include "DominationInfoParser.h"

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
#include "uiDominationJoinList1/uiDominationJoinList1.h"
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

//------------------------------------------------------------------------------
 WzID uiDominationTowerMainDlg::m_wzId[DIALOG_MAX] = 
 {
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
         StrToWzID("PI00"),
         StrToWzID("PI02"),

         StrToWzID("BT00"),

         StrToWzID("S000"),
         StrToWzID("S001"),

         StrToWzID("B999"),
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
         StrToWzID("T999"),
         StrToWzID("ST01"),
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
#else
         StrToWzID("PI01"),
         StrToWzID("PI00"),
         StrToWzID("PI02"),

         StrToWzID("BT00"),
         StrToWzID("BT01"),
         StrToWzID("BT02"),	
         StrToWzID("B000"),	
         StrToWzID("B001"),	

         StrToWzID("B999"),
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
 };

//------------------------------------------------------------------------------
/**
*/
uiDominationTowerMainDlg::uiDominationTowerMainDlg(InterfaceManager *pUIMan)
:uiBase(pUIMan)
{
	m_pGuildMarkPic1 = NULL;
    m_pRelationBackPic = NULL;
    m_pApplyButton = NULL;
    m_pCloseButton = NULL;

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
   guild_name_ = NULL;
   guild_master_name_ = NULL;
#else
    m_pGuildMarkPic2 = NULL;
    m_pBettingButton = NULL;
    m_pTowerManageButton = NULL;
    m_pJoinDominationButton = NULL;
    m_pObserverDominationButton = NULL;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    domination_explanation_string_ = NULL;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
}

//------------------------------------------------------------------------------
/**
*/
uiDominationTowerMainDlg::~uiDominationTowerMainDlg()
{

}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMainDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	m_pGuildMarkPic1 = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_GUILDMARK_PIC1]));
	assert(m_pGuildMarkPic1);
    m_pRelationBackPic = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_GUILDRELATION_BACKPANEL_PIC]));
    assert(m_pRelationBackPic);
    m_pApplyButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_ENTER_BUTTON]));
    assert(m_pApplyButton);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    guild_name_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_STATIC_GUILD_NAME]));
    guild_master_name_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_STATIC_GUILD_MASTER_NAME]));
#else
	m_pGuildMarkPic2 = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_GUILDMARK_PIC2]));
    assert(m_pGuildMarkPic2);
    m_pBettingButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_BETTING_BUTTON]));
    assert(m_pBettingButton);
    m_pTowerManageButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_TOWER_MANAGE_BUTTON]));
    assert(m_pTowerManageButton);
    m_pJoinDominationButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_JOIN_DOMINATION_BUTTON]));
    assert(m_pJoinDominationButton);
    m_pObserverDominationButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_OBSERVER_DOMINATION_BUTTON]));
    assert(m_pObserverDominationButton);
    if (m_pObserverDominationButton != NULL)
    {
        m_pObserverDominationButton->EnableWindowWZ(false);
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    domination_explanation_string_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_EXPLANATION_TEXT]));
    dialog_title_string_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_TITLE_TEXT]));
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMainDlg::OnShowWindow(BOOL val)
{
	if (val)
	{		
		MoveDefaultWindowPos();

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        if (m_pRelationBackPic)
        {
            m_pRelationBackPic->ShowWindowWZ(WZ_HIDE);
        }

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        set_domination_explanation_text();
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

        uiDominationTowerMan* domination_tower_manager = (uiDominationTowerMan*)GetMan();
        if (domination_tower_manager)
        {
            if (guild_name_)
            {
                guild_name_->SetTextWZ(domination_tower_manager->GetDominationGuildInfo()->m_tszGuildName);
            }
            if (guild_master_name_)
            {
                guild_master_name_->SetTextWZ(domination_tower_manager->GetDominationGuildInfo()->m_tszGuildMaster);
            }

            // �������� ������ ��û�Ѵ�. (������ ������ �Ǿƽĺ��� ���Ǳ� ����)
            domination_tower_manager->SEND_DOMINATION_MINIMAP_LIST_SYN();
        }
#else
		uiDominationTowerMan* pUIMan = (uiDominationTowerMan*)GetMan();
		if ( g_pHero && pUIMan )
		{
			if (pUIMan->GetDominationGuildInfo()->m_GuildGuid == g_pHero->GetPlayerGuildID())
			{
				m_pRelationBackPic->ShowWindowWZ(WZ_HIDE);
			}
			else
			{
				m_pRelationBackPic->ShowWindowWZ(WZ_SHOW);
			}
		}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

		// Űť�޽�������
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Domination_Main;
		msg.wParam=IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TOWER_MAIN_DIALOG;
		msg.DoSomething=GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		// Űť�޽�������
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Domination_Main);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
        uiBase* domination_member_list = GET_DIALOG(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_JOIN_LIST1);
        if ((domination_member_list != NULL) && 
            (domination_member_list->IsVisible() == TRUE))
        {
            domination_member_list->ShowInterface(FALSE);
        }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMainDlg::OnRenderSolarDialog()
{
	DOMINATION_GUILD_INFO* pGuildInfo = ((uiDominationTowerMan*)GetMan())->GetDominationGuildInfo();
	if (pGuildInfo->m_GuildGuid)
	{
		uiDominationTowerMan::RenderGuildMark(m_pGuildMarkPic1, pGuildInfo, 255, (m_dwTooltipControl == DLGPOS_GUILDMARK_PIC1), GetDialogKey());
	}

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
	pGuildInfo = ((uiDominationTowerMan*)GetMan())->GetChallengerGuildInfo();
	if (pGuildInfo->m_GuildGuid)
	{
		uiDominationTowerMan::RenderGuildMark(m_pGuildMarkPic2, pGuildInfo, 255, (m_dwTooltipControl == DLGPOS_GUILDMARK_PIC2), GetDialogKey());
	}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMainDlg::MessageProc( SI_MESSAGE * pMessage )
{
    POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
    CControlWZ * pControl = GetControlWZ(m_wzId[pos]);
    if (NULL == pControl)
    {
        m_dwTooltipControl = DIALOG_MAX;
        return;
    }

	switch( pMessage->eResultMsg ) 
	{
    case RT_MSG_MOUSEOVER:
        {
            m_dwTooltipControl = pos;
        }
        break;
	case RT_MSG_LBUTTONCLICK:
		{
			// ��Ŷ ����� ���¸� ����
			uiDominationTowerMan* pUIMan = (uiDominationTowerMan*)GetMan();
			if (NULL == pUIMan || pUIMan->IsPKPending())
			{
				return;
			}			

			switch(pos)
			{
			case DLGPOS_ENTER_BUTTON:
				{
                    // �����ϱ�
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
                    OnClickDominationEnterButton();
#else
					MAPCODE FieldMapCode = g_pMap->GetCurrentFieldID();
					MAPCODE DominationMapCode = DominationInfoParser::Instance()->GetDominationFieldCode(FieldMapCode);
					WORD wIndex = MapInfoParser::Instance()->GetCurrentMapPortalIndexVer2(FieldMapCode, DominationMapCode);
					
					MSG_CG_ZONE_PORTAL_FIELD_MOVE_SYN sync;
					sync.m_wIndex = wIndex;
					GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sync, sizeof(sync));
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
				}
				break;
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
			case DLGPOS_BETTING_BUTTON: 
				{
					// ���� ����
					pUIMan->SEND_DOMINATION_APPLY_SYN();
				}
				break;
            case DLGPOS_TOWER_MANAGE_BUTTON: 
				{
					// �������¼���					
					pUIMan->SEND_DOMINATION_MANAGE_SYN();
				}
				break;
            case DLGPOS_JOIN_DOMINATION_BUTTON: 
                {
                    // ������ ���� ��ư
                    OnClickMemberJoinButton();
                }
                break;
            case DLGPOS_OBSERVER_DOMINATION_BUTTON: 
                {
                    // ������ ���� ��ư
                    // ���� ���� ����(���� ���� ����) [4/11/2011 iforall]
                }
                break;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
			case DLGPOS_CLOSE_BUTTON:
				{
					ShowInterface(FALSE);
				}				
				break;
			default:
				{

				}
			}
		}	
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerMainDlg::SetShowButtonFlag(MSG_CG_DOMINATION_NOTICE_ACK* recv_packet)
{
	// �����ư
	if (recv_packet->m_EnterFlag)
	{
		m_pApplyButton->EnableWindowWZ(TRUE);
	}
	else
	{
		m_pApplyButton->EnableWindowWZ(FALSE);
	}

	// ��������
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
	if (recv_packet->m_AuctionFlag)
	{
		m_pBettingButton->EnableWindowWZ(TRUE);
	}
	else
	{
		m_pBettingButton->EnableWindowWZ(FALSE);
	}
	// ������
	if (recv_packet->m_ManageFlag)
	{
		m_pTowerManageButton->EnableWindowWZ(TRUE);
	}
	else
	{
		m_pTowerManageButton->EnableWindowWZ(FALSE);
	}
    // ������ ���� ��ư
    if (recv_packet->m_JoinFlag)
    {
        m_pJoinDominationButton->EnableWindowWZ(TRUE);
    }
    else
    {
        m_pJoinDominationButton->EnableWindowWZ(FALSE);
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

}

//------------------------------------------------------------------------------
/**
*/
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
void uiDominationTowerMainDlg::OnClickDominationEnterButton()
{
    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager != NULL)
    {
        TCHAR comfirm_message[INTERFACE_STRING_LENGTH] = {'\0',};
        // 3470 ���Ǽ��� ���� �� ������ǥ 1���� �Ҹ�˴ϴ�.\n���Ǽ����� ���� �Ͻðڽ��ϱ�?
        g_InterfaceManager.GetInterfaceString(3470, comfirm_message);
        ui_system_manager->Sys_ConfirmLock(comfirm_message, uiDominationTowerMan::ComfirmDominationEnter);
    }
}
#else
void uiDominationTowerMainDlg::RECV_DOMINATION_MEMBER_JOIN_ACK(MSG_CG_DOMINATION_MEMBER_JOIN_ACK* pMsg)
{
    m_pJoinDominationButton->EnableWindowWZ(FALSE);
}

void uiDominationTowerMainDlg::RECV_DOMINATION_MEMBER_LEAVE_ACK(MSG_CG_DOMINATION_MEMBER_LEAVE_ACK* pMsg)
{
    if (g_pHero == NULL)
    {
        return;
    }

    uiDominationJoinList1* join_list = GET_CAST_DIALOG(uiDominationJoinList1, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_JOIN_LIST1);
    if (join_list == NULL)
    {
        return;
    }

    // �ڽ��̸� ������ư Ȱ��ȭ
    DominationMemberInfo* member_info = join_list->GetDominationMemberInfo(pMsg->memeber_guid_);
    if (member_info != NULL)
    {
        if (Strcmp(member_info->member_name_, g_pHero->GetCharInfo()->m_tszCharName) == 0)
        {
            m_pJoinDominationButton->EnableWindowWZ(TRUE);
        }
    }
}

void uiDominationTowerMainDlg::OnClickMemberJoinButton()
{
    uiDominationTowerMan* ui_domination_manager = static_cast<uiDominationTowerMan*>(GetMan());
    if (ui_domination_manager == NULL)
    {
        return;
    }

    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    MAPCODE map_code = ui_domination_manager->GetDominationManagedMapCode();
    const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
    if ((ui_system_manager != NULL) && 
        (map_info != NULL))
    {
        TCHAR comfirm_message[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR temp_message[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR map_name[INTERFACE_STRING_LENGTH] = {'\0', };
        g_InterfaceManager.GetInterfaceString(map_info->dwNCode, map_name);
        // 3456 %s ������ ���Ǽ��� ���� ��û�� �Ͻðڽ��ϱ�?
        g_InterfaceManager.GetInterfaceString(3456, temp_message);
        Snprintf(comfirm_message, INTERFACE_STRING_LENGTH - 1, temp_message, map_name);

        ui_system_manager->Sys_ConfirmLock(comfirm_message, uiDominationTowerMan::ComfirmDominationJoin);
    }
}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
void uiDominationTowerMainDlg::set_domination_explanation_text()
{    
    if (g_pMap != NULL)
    {
        MAPCODE current_mapcode = g_pMap->GetCurrentFieldID();
        const sDOMINATION_INFO* domination_info = 
            DominationInfoParser::Instance()->GetDominationInfo(current_mapcode);

        if (NULL == domination_info)
            return;

        TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0, };

        switch (domination_info->m_DominationType)
        {
        case DOMINATION_TYPE_NORMAL:
            {
                // Title�� �ٲٱ�
                // 3279 ���Ǽ��� ������
                ZeroMemory(temp_message, sizeof(temp_message));
                g_InterfaceManager.GetInterfaceString(3279, temp_message, INTERFACE_STRING_LENGTH);
                dialog_title_string_->SetTextWZ(temp_message);

                ZeroMemory(temp_message, sizeof(temp_message));
                // 3277 ���� �Ͽ��� ���� 7�ú��� ���Ǽ��� ��������\n���ֵ˴ϴ�. ��� ��尡 �����Ӱ� ������ �� ������,\n
                // ���������� ���ο� ������ ��尡 �� ������\n���ο� ������ �˴ϴ�.\n\n
                // ������ ���ð� : �Ͽ��� 19:00 ~ 20:00\n������ ���۽ð� : �Ͽ��� 20:00 ~ 21:00\n\n
                // (������ �ð� ������ ���Ǽ��� ���� �� ����\n����, ���� ���� ��� 1���� ������ǥ�� �Ҹ�˴ϴ�.)
                g_InterfaceManager.GetInterfaceString(3277, temp_message, INTERFACE_STRING_LENGTH);
                domination_explanation_string_->SetTextWZ(temp_message);                    
            }
            break;
        case DOMINATION_TYPE_MONDSCHIEN:
            {
                // Title�� �ٲٱ� 400349 ��Ʈ���� ������ ����
                ZeroMemory(temp_message, sizeof(temp_message));
                g_InterfaceManager.GetInterfaceString(400349, temp_message);
                dialog_title_string_->SetTextWZ(temp_message);

                ZeroMemory(temp_message, INTERFACE_STRING_LENGTH);
                // 3478	���� �Ͽ��� ���� 9�ú��� 10�ñ��� '��Ʈ���� ��� ������'��\n���ֵ˴ϴ�. ���� ���� ���ο� ������ ��常�� ���� �����ϸ�,\n
                // ���ı��� ����� ������ ���ѳ� ��尡 �������� �����ڰ� �˴ϴ�.\n\n
                // ��� ������ �ð� �� ����� ��Ʈ���� ��� ������ �����Ӱ� ���� �� ��\n�ֽ��ϴ�.
                g_InterfaceManager.GetInterfaceString(3478, temp_message);
                domination_explanation_string_->SetTextWZ(temp_message);
            }
            break;
        default:
            {

            }
            break;
        }
    }
}
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
