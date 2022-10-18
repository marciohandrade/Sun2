#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM

#else

#include "inputnumbankdialog.h"
#include "BankDialog.h"
#include "ChatDialog.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "Hero.h"
#include "uiGuildWareHouse/uiGuildWareHouse.h"

//----------------------------------------------------------------------------
/**
*/
InputNumBankDialog::InputNumBankDialog(void)
{
}


//----------------------------------------------------------------------------
/**
*/
InputNumBankDialog::~InputNumBankDialog(void)
{
}


//------------------------------------------------------------------------------
/**
*/
void
InputNumBankDialog::Init( CDrawBase * pDrawBase )
{
    InputNumDialog::Init(pDrawBase);
	m_CurMoney = 0;

	m_eWareHouseType = eWAREHOUSE_TYPE_MAX;
	m_GuildWareHouseMoneySyn.m_byType = MSG_CG_GUILD_WAREHOUSE_MONEY_SYN::PUTMONEY;

	
	m_WarehouseMoneySyn.m_byType = MSG_CG_WAREHOUSE_MONEY_SYN::PUTMONEY;

	m_dwPacketStatus = NONE;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	const int	c_iInputMoney = 566;	//	입금
	g_InterfaceManager.GetInterfaceString( c_iInputMoney, szMessage, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption( (CCtrlStaticWZ *)getControl(INPUTNUM_BTN_OK), szMessage );

	const int	c_iOutputMoney = 567;	//	출금
	g_InterfaceManager.GetInterfaceString( c_iOutputMoney, szMessage, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption( (CCtrlStaticWZ *)getControl(INPUTNUM_BTN_INIT), szMessage );

	const int	c_iCancel = 101;	//	취소
	g_InterfaceManager.GetInterfaceString( c_iCancel, szMessage, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption( (CCtrlStaticWZ *)getControl(INPUTNUM_BTN_CANCEL), szMessage );
}


//------------------------------------------------------------------------------
/**
*/
void
InputNumBankDialog::Release()
{
    InputNumDialog::Release();
}


//------------------------------------------------------------------------------
/**
*/
void
InputNumBankDialog::OnShowWindow( BOOL val )
{
    InputNumDialog::OnShowWindow(val);

    if (val)
    {
		RECT rcDlgSize;
		this->GetDialogWZ()->GetDlgSize(&rcDlgSize);
		int x = 512 - (rcDlgSize.right / 2);
		MoveWindowWZ( (float)x, 15.0f );

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_num_bank;
		msg.wParam=InterfaceManager::DIALOG_INPUT_NUM;
		msg.DoSomething=GlobalFunc::DoInputNumBank;
		g_KeyQueueManager.PushBack(msg);

    }
    else
    {
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_num_bank);

		m_dwPacketStatus = 0;
    }
}


//------------------------------------------------------------------------------
/**
*/
void
InputNumBankDialog::MoneyProcess()
{
	TCHAR	szMessage[MAX_MESSAGE_LENGTH] = {0,};
	TCHAR	pszInputMoney[128] = {0,};

	// 금액의 최대 사이즈(128 byte)를 넘었습니다.
	if(!GlobalFunc::CompareLimitText(m_pCtrlEditInput, pszInputMoney, 128, 5794))
		return;

	if (lstrlen(pszInputMoney)==0)
	{
		//	금액을 입력해주세요!!
		g_InterfaceManager.GetInterfaceString( eST_INPUT_MONEY, szMessage, MAX_MESSAGE_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
	}
	else
	{
		TCHAR szTemp[128] = {0,};
		int iLen = strlen(pszInputMoney);
		int j = 0;
		for(int i = 0; i < iLen; ++i, ++j)
		{
			if(',' != pszInputMoney[j])
			{
				szTemp[i] = pszInputMoney[j];
			}
			else 
			{
				++j;
				szTemp[i] = pszInputMoney[j];
			}
		}
		if(this->checkDigit(szTemp))
		{
			m_CurMoney = (MONEY)_ttoi64(szTemp);

			SolarDialog::ShowWindow(FALSE);
		}
		else
		{
			//	금액은 숫자로만 입력하십시오
			g_InterfaceManager.GetInterfaceString( eST_INPUT_MONEY_IN_FIGURES, szMessage, MAX_MESSAGE_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
bool
InputNumBankDialog::checkDigit(const TCHAR *str)
{
    int len = lstrlen(str);
    int i;

    for (i=0; i<len; ++i)
    {
        if (str[i]<_T('0') || str[i]>_T('9'))
        {
            return false;
        }
    }

    return true;
}


//------------------------------------------------------------------------------
/**
*/
void
InputNumBankDialog::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            this->m_status = INPUTNUM_STATUS_OK;

            this->MoneyProcess();
        }
        break;

    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case INPUTNUM_BTN_OK:
                {
					// 입금
					this->m_status = INPUTNUM_STATUS_OK;

                    this->MoneyProcess();

					if (m_CurMoney <= 0)
					{
						TCHAR szMessage[MAX_MESSAGE_LENGTH + 1] = {0,};
						//	금액을 입력해주세요!!
						g_InterfaceManager.GetInterfaceString( eST_INPUT_MONEY, szMessage, MAX_MESSAGE_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
						return;
					}

					if(m_CurMoney > g_pHero->GetMoney())
					{
						TCHAR szMessage[MAX_MESSAGE_LENGTH + 1] = {0,};
						//	하임이 부족합니다.
						g_InterfaceManager.GetInterfaceString( eST_NOT_ENOUGH_MONEY, szMessage, MAX_MESSAGE_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
						return;
					}

					if(eWAREHOUSE_TYPE_BANK == m_eWareHouseType)
					{
						if (BIT_CHECK(m_dwPacketStatus, WAREHOUSEMONEY_SEND))
						{
							return;
						}

						BIT_ADD( m_dwPacketStatus, WAREHOUSEMONEY_SEND );

						this->m_WarehouseMoneySyn.m_byType	= MSG_CG_WAREHOUSE_MONEY_SYN::PUTMONEY;
						this->m_WarehouseMoneySyn.m_Money	= m_CurMoney;

						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &m_WarehouseMoneySyn, sizeof(m_WarehouseMoneySyn));
					}
					else if(eWAREHOUSE_TYPE_GUILD == m_eWareHouseType)
					{
						if (BIT_CHECK(m_dwPacketStatus, GUILDWAREHOUSEMONEY_SEND))
						{
							return;
						}

						BIT_ADD( m_dwPacketStatus, GUILDWAREHOUSEMONEY_SEND );

						this->m_GuildWareHouseMoneySyn.m_byType	= MSG_CG_GUILD_WAREHOUSE_MONEY_SYN::PUTMONEY;
						this->m_GuildWareHouseMoneySyn.m_Money	= m_CurMoney;

						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &m_GuildWareHouseMoneySyn, sizeof(m_GuildWareHouseMoneySyn));
					}
					else
					{
						assert(!"InputNumBankDialog::MessageProc");
					}

                }
                break;

            case INPUTNUM_BTN_INIT:
                {
					this->MoneyProcess();

					if (m_CurMoney <= 0)
					{
						TCHAR szMessage[MAX_MESSAGE_LENGTH];
						//	금액을 입력해주세요!!
						g_InterfaceManager.GetInterfaceString( eST_INPUT_MONEY, szMessage, MAX_MESSAGE_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

						return;
					}

					if(eWAREHOUSE_TYPE_BANK == m_eWareHouseType)
					{
						if (BIT_CHECK(m_dwPacketStatus, WAREHOUSEMONEY_SEND))
						{
							return;
						}

						BIT_ADD( m_dwPacketStatus, WAREHOUSEMONEY_SEND );

						// 출금
						this->m_status = INPUTNUM_STATUS_INIT;

						this->m_WarehouseMoneySyn.m_byType	= MSG_CG_WAREHOUSE_MONEY_SYN::GETMONEY;
						this->m_WarehouseMoneySyn.m_Money	= m_CurMoney;

						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &m_WarehouseMoneySyn, sizeof(m_WarehouseMoneySyn));
					}
					else if(eWAREHOUSE_TYPE_GUILD == m_eWareHouseType)
					{
						uiGuildWareHouse * pDlg = GET_CAST_DIALOG( uiGuildWareHouse, IM_GUILD_MANAGER::GUILD_WAREHOUSE );

						if(pDlg)
						{
							if(m_CurMoney > pDlg->GetMoney())
							{
								TCHAR szMessage[MAX_MESSAGE_LENGTH + 1] = {0,};
								//	하임이 부족합니다.
								g_InterfaceManager.GetInterfaceString( eST_NOT_ENOUGH_MONEY, szMessage, MAX_MESSAGE_LENGTH );
								GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
								return;
							}
						}

						if (BIT_CHECK(m_dwPacketStatus, GUILDWAREHOUSEMONEY_SEND))
						{
							return;
						}

						BIT_ADD( m_dwPacketStatus, GUILDWAREHOUSEMONEY_SEND );

						// 출금
						this->m_status = INPUTNUM_STATUS_INIT;

						this->m_GuildWareHouseMoneySyn.m_byType	= MSG_CG_GUILD_WAREHOUSE_MONEY_SYN::GETMONEY;
						this->m_GuildWareHouseMoneySyn.m_Money	= m_CurMoney;

						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &m_GuildWareHouseMoneySyn, sizeof(m_GuildWareHouseMoneySyn));
					}
					else
					{
						assert(!"InputNumBankDialog::MessageProc");
					}

				}
                break;

            case INPUTNUM_BTN_CANCEL:
                {
					
					this->m_status = INPUTNUM_STATUS_CANCEL;
					ShowWindow( FALSE );

                }

            case INPUTNUM_BTN_EXIT:
                {			
					Exit();
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


//----------------------------------------------------------------------------
/**
*/
void InputNumBankDialog::Exit()
{
	this->m_status = INPUTNUM_STATUS_NORMAL;
	ClearPacketFlag();
	ShowWindow( FALSE );

}


//----------------------------------------------------------------------------
/**
*/
VOID 
InputNumBankDialog::ClearPacketFlag()
{
	this->m_packetStatus = NONE;
}

//----------------------------------------------------------------------------
/**
*/
VOID			
InputNumBankDialog::SendWareHouseMoney(MONEY money)
{
}

#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
