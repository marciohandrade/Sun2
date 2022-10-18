#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "InterfaceManager.h"
#include "uiChangeGuildName/uiChangeGuildName.h"

#include <SCSlot.h>
#include <SCSlotContainer.h>
#include <SCItemSlot.h>

#include "GlobalFunc.h"
#include "Hero.h"
#include "ItemManager.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiChangeGuildName::m_wzID[eDIALOG_CHANGE_GUILD_NAME_MAX] = 
{
		StrToWzID("TI00"),//eDIALOG_CHANGE_GUILD_NAME_OK_BTN,
		StrToWzID("TI01"),//eDIALOG_CHANGE_GUILD_NAME_CANCEL_BTN,
		StrToWzID("BT00"),//eDIALOG_CHANGE_GUILD_NAME_EXIT_BTN,
		StrToWzID("ST02"),//eDIALOG_CHANGE_GUILD_NAME_CUR_GUILD_NAME,
		StrToWzID("ED00"),//eDIALOG_CHANGE_GUILD_NAME_EDIT,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiChangeGuildName::uiChangeGuildName(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
	m_pctrlEdit		  = NULL;
	m_pctrlStatic	  = NULL;
	SetSlotInfo(0, INVALID_POSTYPE_VALUE);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiChangeGuildName::~uiChangeGuildName()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeGuildName::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeGuildName::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeGuildName::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeGuildName::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeGuildName::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_bShow = TRUE;
		if(g_pHero && m_pctrlStatic)
			m_pctrlStatic->SetTextWZ(g_pHero->GetGuildName());
	}
	else
	{
		m_bShow   = FALSE;
		if(m_pctrlEdit)
		{
			m_pctrlEdit->ClearText();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeGuildName::SetSlotInfo(SLOTIDX SlotIdx, POSTYPE PosType)
{
	m_SlotIdx = SlotIdx;
	m_PosType = PosType;
	SlotLock(true);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeGuildName::SlotLock(bool bLock)
{
	SCSlotContainer* pInvenSlotCon = ItemManager::Instance()->GetContainer(m_SlotIdx);
	if(pInvenSlotCon)
	{
		SCItemSlot& rItemSlot = (SCItemSlot&)pInvenSlotCon->GetSlot(m_PosType);
		rItemSlot.SetLock(bLock);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeGuildName::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_CHANGE_GUILD_NAME_OK_BTN:
				{
					TCHAR szGuildName[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
					m_pctrlEdit->GetTextWZ(szGuildName, INTERFACE_STRING_LENGTH);

					int iStrLen = _tcslen(szGuildName);
					if(0 == iStrLen)
					{
						//	길드명을 입력하세요.
						g_InterfaceManager.GetInterfaceString( eST_INPUT_GUILD_NAME, szMessage);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
						return;
					}

					if(MAX_GUILDNAME_LENGTH < iStrLen)
					{
						//	16자 까지만 적용됩니다.
						g_InterfaceManager.GetInterfaceString( eST_CHECK_GUILD_NAME_LENGTH_16, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
						return;
					}
					
#if defined(__KR) || defined(_KOREA)
					if(g_slangNameFilter.IsHaveReplace(szGuildName)||g_slangNameFilter.CheckIncludeString(szGuildName))
					{
						//	금지 단어 사용
						g_InterfaceManager.GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, MAX_MESSAGE_LENGTH );
						g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
						return;
					}
#else	
					if(g_slangNameFilter.IsHaveReplace(szGuildName)||GlobalFunc::CheckIncludeStringOnAbroad(szGuildName) )
					{
						//	금지 단어 사용
						g_InterfaceManager.GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, MAX_MESSAGE_LENGTH );
						g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
						return;
					}
#endif	//#if defined(__KR) || defined(_KOREA)
					else
					{
						MSG_CG_GUILD_NAME_CHANGE_SYN SendMsg;
						StrnCopy(SendMsg.m_tszGuildName, szGuildName, MAX_GUILDNAME_LENGTH + 1);
						SendMsg.m_atIndex = m_SlotIdx;
						SendMsg.m_atPos	  = m_PosType;
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));

						SlotLock(false);
						OnShowWindow(FALSE);
					}
				}
				break;

			case eDIALOG_CHANGE_GUILD_NAME_CANCEL_BTN:
			case eDIALOG_CHANGE_GUILD_NAME_EXIT_BTN:
				{
					SlotLock(false);
					OnShowWindow(FALSE);
				}
				break;
			}
		}
		break;

	default:
		{

		}
		break;

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeGuildName::InitControl()
{
	m_pctrlEdit = (CCtrlDxIMEEditBox*)getControl(eDIALOG_CHANGE_GUILD_NAME_EDIT);
	assert(m_pctrlEdit);

	m_pctrlStatic = (CCtrlStaticWZ*)getControl(eDIALOG_CHANGE_GUILD_NAME_CUR_GUILD_NAME);
	assert(m_pctrlStatic);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
