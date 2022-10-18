#include "SunClientPrecompiledHeader.h"
#include "uiDominationTowerApplyBettingDlg.h"

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else

#include "uiDominationTowerBettingListDlg/uiDominationTowerBettingListDlg.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "ItemManager.h"

#include "MouseHandler.h"
#include "DominationInfoParser.h"
#include "Map.h"
#include "Hero.h"
#include "SCItemSlotContainer.h"

    const int kEdit_Max = 6;

//------------------------------------------------------------------------------
 WzID uiDominationTowerApplyBettingDlg::m_wzId[DIALOG_MAX] = 
 {
	StrToWzID("ST04"),// DLGPOS_BETTING_NOTICE_TEXT,
    StrToWzID("B003"),// DLGPOS_TOKEN_SLOT_BUTTON,
    StrToWzID("S000"),// DLGPOS_APPLY_HEIM_TEXT,
    StrToWzID("ED00"),// DLGPOS_BETTING_COST_EDIT,
	StrToWzID("ST00"),// DLGPOS_GUILD_BALANCE_TEXT,
	StrToWzID("BT00"),// DLGPOS_BETTING_APPLY_BUTTON,
	StrToWzID("BT04"),// DLGPOS_CANCEL_BUTTON,

	StrToWzID("B999"),// DLGPOS_CLOSE_BUTTON,
 };

//------------------------------------------------------------------------------
/**
*/
uiDominationTowerApplyBettingDlg::uiDominationTowerApplyBettingDlg(InterfaceManager *pUIMan)
:uiBase(pUIMan)
{
	m_pNoticeText = NULL;
    m_pTokenSlotButton = NULL;
    m_pApplayHeim = NULL;
    m_pBettingEdit = NULL;
	m_pHiemBalance = NULL;
	m_pBettingButton = NULL;
	m_pCancelButton = NULL;
	m_pCloseButton = NULL;
}

//------------------------------------------------------------------------------
/**
*/
uiDominationTowerApplyBettingDlg::~uiDominationTowerApplyBettingDlg()
{
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerApplyBettingDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	m_pNoticeText = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_BETTING_NOTICE_TEXT]));
	assert(m_pNoticeText);
    m_pTokenSlotButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_TOKEN_SLOT_BUTTON]));
    assert(m_pTokenSlotButton);
    m_pApplayHeim = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_APPLY_HEIM_TEXT]));
    assert(m_pApplayHeim);
    m_pBettingEdit = static_cast<CCtrlDxIMEEditBox*>(GetControlWZ(m_wzId[DLGPOS_BETTING_COST_EDIT]));
    assert(m_pBettingEdit);
    if (m_pBettingEdit != NULL)
    {
        m_pBettingEdit->SetLimitText(kEdit_Max);
    }
	m_pHiemBalance = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_GUILD_BALANCE_TEXT]));
	assert(m_pHiemBalance);
	m_pBettingButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_BETTING_APPLY_BUTTON]));
	assert(m_pBettingButton);
	m_pCancelButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_CANCEL_BUTTON]));
	assert(m_pCancelButton);
	m_pCloseButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_CLOSE_BUTTON]));
	assert(m_pCloseButton);
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerApplyBettingDlg::OnShowWindow(BOOL val)
{
    uiDominationTowerMan* ui_domination_manager = static_cast<uiDominationTowerMan*>(GetMan());
	if (val)
	{
        if (ui_domination_manager != NULL)
        {
            // �⺻�����ݾ�
            MAPCODE current_mapcode = ui_domination_manager->GetDominationManagedMapCode();
            const sDOMINATION_INFO* domination_info = DominationInfoParser::Instance()->GetDominationInfo(current_mapcode);
            if (g_pHero != NULL && domination_info != NULL)
            {
                GlobalFunc::SetMoneyText(m_pApplayHeim, domination_info->m_BaseBattingMoney);

                GlobalFunc::SetMoneyText(m_pHiemBalance, g_pHero->GetMoney());
            }
        }

        if (m_pBettingEdit != NULL)
        {
            m_pBettingEdit->ClearText();
            GlobalFunc::SetFocus((SolarDialog*)this, m_pBettingEdit);
        }

        g_InterfaceManager.AttachFirstDialog(GetDialogWZ()->GetDlgID());
		
		//Űť�޽�������
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Domination_ApplyBetting;
		msg.wParam=IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_APPLY_BETTING_DIALOG;
		msg.DoSomething=GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		//Űť�޽�������
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Domination_ApplyBetting);

        g_InterfaceManager.DeAttachFirstDialog(GetDialogWZ()->GetDlgID());

		uiDominationTowerBettingListDlg* domination_list_dialog = 
            GET_CAST_DIALOG(uiDominationTowerBettingListDlg, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST_DIALOG);
		if (ui_domination_manager != NULL)
		{
            if ((domination_list_dialog != NULL) && 
                (domination_list_dialog->IsVisible() == TRUE))
            {
                ui_domination_manager->SEND_DOMINATION_APPLY_SYN();
            }
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerApplyBettingDlg::OnUpdateSolarDialog()
{
    // ���ΰ��� �ؽ�Ʈ����
    if (m_pNoticeText)
    {
        FTextParser text_parser;
        FTextSentence text_sentence;
        TCHAR notice_string[INTERFACE_STRING_LENGTH] = {0, };
        RECT notice_rect = m_pNoticeText->GetSizeRect();

        m_pNoticeText->SetTextWZ(_T(" "));
        notice_text_render_.Init(GetDrawBase(), notice_rect);
        notice_text_render_.SetVCenterAlign(true);
        notice_text_render_.ClearSentence();
        text_parser.SetDefaultFont(m_pNoticeText->GetFontID(), m_pNoticeText->GetTextColor());
        g_InterfaceManager.GetInterfaceString(3391, notice_string);
        BOOL result = text_parser.Parser(notice_string, text_sentence);
        if (result == TRUE)
        {
            notice_text_render_.Append(text_sentence);
            notice_text_render_.convertLineInfo();
        }
        notice_text_render_.SetBeginLineIndex(0);
    }

    if (m_pBettingEdit != NULL)
    {
        memset(m_szCurrentMoney, 0, INTERFACE_STRING_LENGTH);
        const BYTE c_btMaxEditNum = 19;

        m_pBettingEdit->GetTextWZ(m_szMoneyOld, INTERFACE_STRING_LENGTH);
        int iLen = strlen(m_szMoneyOld);
        int j = 0;
        for(int i = 0; i < iLen; ++i, ++j)
        {
            if(',' != m_szMoneyOld[j])
            {
                m_szCurrentMoney[i] = m_szMoneyOld[j];
            }
            else 
            {
                ++j;
                m_szCurrentMoney[i] = m_szMoneyOld[j];
            }
        }
        if(isdigit(m_szCurrentMoney[0]))
        {
            if(c_btMaxEditNum > strlen(m_szCurrentMoney)) //����Ʈ ��Ʈ�� ���� ���� �ʰ�
            {
                NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
                ::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, m_szCurrentMoney, &nFmt, m_szMoneyNew, sizeof(m_szMoneyNew));
                m_pBettingEdit->SetTextWZ(m_szMoneyNew);
            }
            else
            {
                m_pBettingEdit->SetTextWZ("");
            }
        }
        else
        {
            m_pBettingEdit->SetTextWZ("");
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerApplyBettingDlg::OnRenderSolarDialog()
{
    // �ؽ�Ʈ ����
    if (IsVisibleWindow())
    {
        notice_text_render_.Render();
    }	
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerApplyBettingDlg::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_EDIT_ENTERKEYDOWN:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_GUILD_BALANCE_TEXT:
				{
					// ����Ű �Է� ó��
				}
				break;
			}
		}
		break;
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			CControlWZ * pControl = GetControlWZ(m_wzId[pos]);
			if (NULL == pControl)
			{
				return;
			}

			// ��Ŷ ����� ���¸� ����
			uiDominationTowerMan* pUIMan = (uiDominationTowerMan*)GetMan();
			if (NULL == pUIMan || pUIMan->IsPKPending())
			{
				return;
			}			

			switch(pos)
			{
            case DLGPOS_BETTING_APPLY_BUTTON:
                {
                    // �����ϱ�
                    OnApplyButton();
                }
                break;
			case DLGPOS_CANCEL_BUTTON: 
			case DLGPOS_CLOSE_BUTTON: 
				{
					// ���� ��� â�ݱ� 
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

void uiDominationTowerApplyBettingDlg::OnApplyButton()
{
    uiDominationTowerMan* ui_domination_manager = static_cast<uiDominationTowerMan*>(GetMan());
    if (ui_domination_manager == NULL)
    {
        return;
    }

    MAPCODE current_mapcode = ui_domination_manager->GetDominationManagedMapCode();
    const sDOMINATION_INFO* domination_info = DominationInfoParser::Instance()->GetDominationInfo(current_mapcode);
    if ((domination_info != NULL) && 
        (g_pHero != NULL))
    {
#ifdef _NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST
        if (domination_info->m_BattingGuildLevelLimit > g_pHero->GetGuildRenderInfo().m_GuildGrade)
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
            TCHAR limit_level[INTERFACE_STRING_LENGTH] = {'\0', };
            Snprintf(limit_level, INTERFACE_STRING_LENGTH, "%d", domination_info->m_BattingGuildLevelLimit);
            // 3268 �ش� ������ %s���� �̻��� ��常 ���� �����մϴ�.
            g_InterfaceManager.GetInterfaceStringFormat(message, INTERFACE_STRING_LENGTH, 3268, limit_level);
            GlobalFunc::VerifyMessage(message, NULL, NULL, true);
            
            return;
        }
#endif //_NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST

        bool is_valid_token = false;
        bool have_heim = false;

        // �κ��� �Է°��� ��ŭ �ִ��� üũ
        MONEY betting_cost = _ttoi64(m_szCurrentMoney);
        WORD token_num = 
            ItemManager::Instance()->GetItemCount(SI_INVENTORY, domination_info->m_BattingItemCode);

        if ((betting_cost > 0) && 
            (token_num >= betting_cost))
        {
            is_valid_token = true;
        }

        // �������� üũ
        if (g_pHero->GetMoney() >= domination_info->m_BaseBattingMoney)
        {
            have_heim = true;
        }

        if ((is_valid_token == true) &&
            (have_heim == true))
        {
            ui_domination_manager->SEND_DOMINATION_ACCEP_HEIM_SYN(betting_cost);
            ShowInterface(FALSE);
        }
        else
        {
            DWORD string_code = 0;

            if (have_heim == false)
            {
                // 3383 �������� �����Ͽ� ���Ǽ��� �������� ���� ��û�� �� �� �����ϴ�.
                string_code = 3383;
            }
            else
            {
                //3385 ���Ǽ��� �������� ��û�Ͻ÷��� �ּ� ��û �ݾװ� ���� ��ǥ �������� �ʿ��մϴ�.
                string_code = 3385;
            }

            if (string_code != 0)
            {
                TCHAR message_string[INTERFACE_STRING_LENGTH] = {0, };
                g_InterfaceManager.GetInterfaceString(string_code, message_string);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_string);
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
#endif //_NA_006826_20130722_DOMINATION_RENEWAL