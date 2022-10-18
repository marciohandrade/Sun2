#include "SunClientPrecompiledHeader.h"
#include "uiDominationTowerApplyBettingDlg2.h"

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else

#include "uiDominationTowerBettingListDlg/uiDominationTowerBettingListDlg.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "Hero.h"
#include "Map.h"
#include "DominationInfoParser.h"
#include "SCItemSlotContainer.h"
#include "ItemManager.h"

const int kEdit_Max = 6;

//------------------------------------------------------------------------------
 WzID uiDominationTowerApplyBettingDlg2::controls_id_[DIALOG_MAX] = 
 {
	StrToWzID("ST04"),// DLGPOS_BETTING_NOTICE_TEXT,
	StrToWzID("B003"),// DLGPOS_TOKEN_SLOT_BUTTON,
    StrToWzID("ED00"),// DLGPOS_BETTING_COST_EDIT,
	StrToWzID("BT00"),// DLGPOS_BETTING_APPLY_BUTTON,
	StrToWzID("BT04"),// DLGPOS_CANCEL_BUTTON,
	StrToWzID("B999"),// DLGPOS_CLOSE_BUTTON,
 };

//------------------------------------------------------------------------------
/**
*/
uiDominationTowerApplyBettingDlg2::uiDominationTowerApplyBettingDlg2(InterfaceManager *pUIMan)
:uiBase(pUIMan)
{
    notice_text_ = NULL;
    token_slot_button_ = NULL;
    betting_button_ = NULL;
    cancel_button_ = NULL;
    close_button_ = NULL;
    betting_edit_ = NULL;
}

//------------------------------------------------------------------------------
/**
*/
uiDominationTowerApplyBettingDlg2::~uiDominationTowerApplyBettingDlg2()
{
	Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerApplyBettingDlg2::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	notice_text_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(controls_id_[DLGPOS_BETTING_NOTICE_TEXT]));
	assert(notice_text_);
    token_slot_button_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(controls_id_[DLGPOS_TOKEN_SLOT_BUTTON]));
    assert(token_slot_button_);
    betting_edit_ = static_cast<CCtrlDxIMEEditBox*>(GetControlWZ(controls_id_[DLGPOS_BETTING_COST_EDIT]));
    assert(betting_edit_);
    if (betting_edit_ != NULL)
    {
        betting_edit_->SetLimitText(kEdit_Max);
    }
	betting_button_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(controls_id_[DLGPOS_BETTING_APPLY_BUTTON]));
	assert(betting_button_);
	cancel_button_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(controls_id_[DLGPOS_CANCEL_BUTTON]));
	assert(cancel_button_);
	close_button_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(controls_id_[DLGPOS_CLOSE_BUTTON]));
	assert(close_button_);
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerApplyBettingDlg2::OnShowWindow(BOOL val)
{
	if (val)
	{
		MoveDefaultWindowPos();

        if (betting_edit_ != NULL)
        {
            betting_edit_->ClearText();
            GlobalFunc::SetFocus((SolarDialog*)this, betting_edit_);
        }

        g_InterfaceManager.AttachFirstDialog(GetDialogWZ()->GetDlgID());

		//키큐메시지삽입
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Domination_ApplyBetting;
		msg.wParam=IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_APPLY_BETTING_DIALOG2;
		msg.DoSomething=GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		//키큐메시지삭제
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Domination_ApplyBetting);

        g_InterfaceManager.DeAttachFirstDialog(GetDialogWZ()->GetDlgID());

		uiDominationTowerMan* pUIMan = (uiDominationTowerMan*)GetMan();
		uiDominationTowerBettingListDlg* pDlg = GET_CAST_DIALOG(uiDominationTowerBettingListDlg, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_BETTING_LIST_DIALOG);
		if (pUIMan && pDlg && pDlg->IsVisible())
		{
			pUIMan->SEND_DOMINATION_APPLY_SYN();
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerApplyBettingDlg2::OnUpdateSolarDialog()
{
    if (betting_edit_ != NULL)
    {
        memset(current_money, 0, INTERFACE_STRING_LENGTH);
        const BYTE c_btMaxEditNum = 19;

        betting_edit_->GetTextWZ(money_old, INTERFACE_STRING_LENGTH);
        int iLen = strlen(money_old);
        int j = 0;
        for(int i = 0; i < iLen; ++i, ++j)
        {
            if(',' != money_old[j])
            {
                current_money[i] = money_old[j];
            }
            else 
            {
                ++j;
                current_money[i] = money_old[j];
            }
        }
        if(isdigit(current_money[0]))
        {
            if(c_btMaxEditNum > strlen(current_money)) //에디트 컨트롤 범위 넘지 않게
            {
                NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
                ::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, current_money, &nFmt, money_new, sizeof(money_new));
                betting_edit_->SetTextWZ(money_new);
            }
            else
            {
                betting_edit_->SetTextWZ("");
            }
        }
        else
        {
            betting_edit_->SetTextWZ("");
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerApplyBettingDlg2::OnRenderSolarDialog()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationTowerApplyBettingDlg2::MessageProc(SI_MESSAGE * pMessage)
{
	switch (pMessage->eResultMsg) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			CControlWZ * pControl = GetControlWZ(controls_id_[pos]);
			if (NULL == pControl)
			{
				return;
			}

			// 패킷 펜딩셋 상태면 리턴
			uiDominationTowerMan* pUIMan = (uiDominationTowerMan*)GetMan();
			if (NULL == pUIMan || pUIMan->IsPKPending())
			{
				return;
			}			

			switch(pos)
			{
            case DLGPOS_BETTING_APPLY_BUTTON:
                {
                    // 베팅하기					
                    OnApplyButton();
                }
                break;
			case DLGPOS_CANCEL_BUTTON: 
			case DLGPOS_CLOSE_BUTTON: 
				{
					// 베팅 취소 창닫기 
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

void uiDominationTowerApplyBettingDlg2::OnApplyButton()
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
            // 3268 해당 지역은 %s레벨 이상의 길드만 입찰 가능합니다.
            g_InterfaceManager.GetInterfaceStringFormat(message, INTERFACE_STRING_LENGTH, 3268, limit_level);
            GlobalFunc::VerifyMessage(message, NULL, NULL, true);

            return;
        }
#endif //_NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST

        bool is_valid_token = false;

        // 인벤에 입력갯수 만큼 있는지 체크
        MONEY betting_cost = _ttoi64(current_money);
        WORD token_num = 
            ItemManager::Instance()->GetItemCount(SI_INVENTORY, domination_info->m_BattingItemCode);

        if ((betting_cost > 0) && 
            (token_num >= betting_cost))
        {
            is_valid_token = true;
        }

        if (is_valid_token == true)
        {
            ui_domination_manager->SEND_DOMINATION_ACCEP_HEIM_SYN(betting_cost);
            ShowInterface(FALSE);
        }
        else
        {
            TCHAR message_string[INTERFACE_STRING_LENGTH] = {0, };
            // 3384 왕의 증표 아이템을 최소 1개 이상 등록하셔야 신청하실 수 있습니다.
            g_InterfaceManager.GetInterfaceString(3384, message_string);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_string);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
#endif //_NA_006826_20130722_DOMINATION_RENEWAL