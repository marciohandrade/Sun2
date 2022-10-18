#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildCorpsChange.h"
#include "GlobalFunc.h"

WzID uiGuildCorpsChange::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("C100"),	//DLGPOS_BTN1,
	StrToWzID("CT00"),	//DLGPOS_BTN2,
	StrToWzID("CT01"),	//DLGPOS_BTN3,
	StrToWzID("CT02"),	//DLGPOS_BTN4,
	StrToWzID("CT03"),	//DLGPOS_BTN5,
	StrToWzID("CT04"),	//DLGPOS_BTN6,
	StrToWzID("CT05"),	//DLGPOS_BTN7,
	StrToWzID("CT06"),	//DLGPOS_BTN8,
						
	StrToWzID("T103"),	//DLGPOS_TXT1,
	StrToWzID("T104"),	//DLGPOS_TXT2,
	StrToWzID("T105"),	//DLGPOS_TXT3,
	StrToWzID("T106"),	//DLGPOS_TXT4,
	StrToWzID("ST00"),	//DLGPOS_TXT5,
	StrToWzID("ST01"),	//DLGPOS_TXT6,
	StrToWzID("ST02"),	//DLGPOS_TXT7,
	StrToWzID("ST03"),	//DLGPOS_TXT8,

	StrToWzID("B100"),	//DLGPOS_BTN_EXIT,
	StrToWzID("B101"),	//DLGPOS_BTN_OK,
	StrToWzID("B102"),	//DLGPOS_BTN_CANCLE,
};
//------------------------------------------------------------------------------
/**
*/
uiGuildCorpsChange::uiGuildCorpsChange(InterfaceManager * pUIMan) : uiBase(pUIMan)
{

}
//------------------------------------------------------------------------------
/**
*/
uiGuildCorpsChange::~uiGuildCorpsChange()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildCorpsChange::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	for( int i = 0; i < BUTTON_MAX; ++i)
	{
		m_CheckState[i].pCheck = (CCtrlButtonCheckWZ *) getControl(DLGPOS_BTN1 +i);
		assert(m_CheckState[i].pCheck);
		m_CheckState[i].pStatic = (CCtrlStaticWZ *) getControl(DLGPOS_TXT1 +i);
		assert(m_CheckState[i].pStatic);
	}

	m_bShow = false;
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildCorpsChange::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildCorpsChange::OnUpdateSolarDialog()
{
	if( !m_bShow)
	{
		ShowInterface(false);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildCorpsChange::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildCorpsChange::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_bShow = true;

		MoveCenter();
	}
	else
	{
		m_bShow = false;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildCorpsChange::MessageProc(SI_MESSAGE * pMessage)
{
	switch( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch(  getControlIDToPos( pMessage->dwCtrlID) )
			{
			case DLGPOS_BTN_EXIT:
			case DLGPOS_BTN_CANCLE:
				{
					m_bShow = false;
				}
				break;
			case DLGPOS_BTN1:
			case DLGPOS_BTN2:
			case DLGPOS_BTN3:
			case DLGPOS_BTN4:
			case DLGPOS_BTN5:
			case DLGPOS_BTN6:
			case DLGPOS_BTN7:
			case DLGPOS_BTN8:
				{
					int checkID = getControlIDToPos( pMessage->dwCtrlID);
					bool oldValue = m_CheckState[checkID].pCheck->GetCheckState();
					if( checkID < BUTTON_MAX)
					{
						for( int i = 0; i < BUTTON_MAX; ++i)
						{
							m_CheckState[i].pCheck->SetCheckState(false);
						}
					}
					m_CheckState[checkID].pCheck->SetCheckState(oldValue);
				}
				break;


			case DLGPOS_BTN_OK:
				{
					if( m_FnOkBtnProcess() )
						m_bShow = false;
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
void uiGuildCorpsChange::NetworkProc(MSG_BASE * pMsg)
{

}

bool uiGuildCorpsChange::NET_SEND_CG_GUILD_CHANGE_CORPS_SYN()
{
	MSG_CG_GUILD_CHANGE_CORPS_SYN SendPacket;

	StrnCopy(SendPacket.m_szTargetCharName , m_strTargetName.c_str(), MAX_CHARNAME_LENGTH);
	
	for( int i = 0; i< BUTTON_MAX; ++i)
	{
		if( m_CheckState[i].pCheck->GetCheckState() )
		{
			SendPacket.m_byCorpsNum = m_CheckState[i].Corps;
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
			return true;
		}
	}

	//6256	이동하실 군단을 선택하셔야 합니다
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6256));
	return false;
}

bool uiGuildCorpsChange::NET_SEND_CG_GUILD_DESTROY_CORPS_SYN()
{
	MSG_CG_GUILD_DESTROY_CORPS_SYN SendPacket;
	for( int i = 0; i< BUTTON_MAX; ++i)
	{
		if( m_CheckState[i].pCheck->GetCheckState() )
		{
			SendPacket.m_byCorpsNum = m_CheckState[i].Corps;
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
			return true;
		}
	}
	//6275	해산 하실 군단을 선택하셔야 합니다
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6275));
	return false;
}

void uiGuildCorpsChange::ShowAndInsertCheckCorps(int index, int iCorps)
{
	TCHAR szMessage[MAX_PATH + 1] ={0,};

	if( index  < BUTTON_MAX)
	{
		m_CheckState[index].Corps = iCorps;
		m_CheckState[index].pCheck->ShowWindowWZ(WZ_SHOW);
		m_CheckState[index].pStatic->ShowWindowWZ(WZ_SHOW);
		//6285	%s 군단
		Snprintf(szMessage, MAX_PATH, g_InterfaceManager.GetInterfaceString(6285) , GlobalFunc::GetCorpsName(iCorps));
		GlobalFunc::SetCaption(m_CheckState[index].pStatic, szMessage);
	}
}

void uiGuildCorpsChange::InitAllCheckButton()
{
	for( int i = 0; i < BUTTON_MAX; ++i)
	{
		m_CheckState[i].pCheck->ShowWindowWZ(WZ_HIDE);
		m_CheckState[i].pStatic->ShowWindowWZ(WZ_HIDE);
		m_CheckState[i].pCheck->SetCheckState(FALSE);
		m_CheckState[i].Corps = 0;
	}
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
