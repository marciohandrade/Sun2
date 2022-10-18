//------------------------------------------------------------------------------
//  uiGuildCreate_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMenu.h"
#include "battleroomentry.h"
#include "MapInfoParser.h"
#include "BattleScene.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uisystemman/uiSystemMan.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"

void _CallBack_DestoryCamp(bool bYes)
{
	if( bYes )
	{
		uiGuildMan * pGuildMan = (uiGuildMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
		if( pGuildMan )
		{
			pGuildMan->SEND_CG_GUILD_DESTROY_REAR_CAMP_SYN();
		}
	}
}

void _CallBack_DestoryCorp(bool bYes)
{
	if( bYes )
	{
		MSG_CG_GUILD_DESTROY_CORPS_SYN SendMsg;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
	}
}

void _CallBack_CreateCorp(bool bYes, const char * pString)
{
	if( bYes )
	{
		uiGuildMan * pGuildMan = (uiGuildMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
		if( pGuildMan )
		{
			stGUILD_MEMBER_INFO * pGuildMemberInfo = pGuildMan->GetGuildMemberInfo(pString);
			if( pGuildMemberInfo )
			{
				MSG_CG_GUILD_CREATE_CORPS_SYN  SendMsg;
				StrnCopy(SendMsg.m_szTargetCharName , pString,MAX_CHARNAME_LENGTH);
				GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
			}
			else
			{
				//7228	해당 길드원이 존재 하지 않습니다.
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(7228));
			}
		}
	}
}

void _CallBack_CreateCamp(bool bYes, const char * pString)
{
	if( bYes )
	{
		uiGuildMan * pGuildMan = (uiGuildMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
		if( pGuildMan )
		{
			stGUILD_MEMBER_INFO * pGuildMemberInfo = pGuildMan->GetGuildMemberInfo(pString);
			if( pGuildMemberInfo )
			{
				MSG_CG_GUILD_CREATE_REAR_CAMP_SYN SendMsg;
				StrnCopy(SendMsg.m_szTargetCharName, pString, MAX_GUILDNAME_LENGTH);
				GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
			}
			else
			{
				////7228	해당 길드원이 존재 하지 않습니다.
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(7228));
			}
		}
	}
}


void uiGuildMenu::OnClickCorpNCampDestory()
{
	uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if( !pSystemMan)
		return;

	if( !GetManager())
		return;

	if(!g_pHero)
		return;


	stGUILD_MEMBER_INFO * pMember = GetManager()->GetGuildMemberInfo(g_pHero->GetName());
	if( !pMember )
		return;

	if( pMember->m_eGuildDuty == eGUILD_DUTY_COMMANDER)
	{
		//1486	육성캠프를 해산하시겠습니까?
		pSystemMan->Sys_ConfirmLock(g_InterfaceManager.GetInterfaceString(1486), _CallBack_DestoryCamp);
	}
	else
	{
		////1390	군단을 해산하시겠습니까?
		GetManager()->ShowGuildDestoryWindow();
	}
}

void uiGuildMenu::OnClickCorpNCampCreate()
{
	uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if( !pSystemMan )
		return;
	
	if( !GetManager())
		return;

	if(!g_pHero)
		return;


	stGUILD_MEMBER_INFO * pMember = GetManager()->GetGuildMemberInfo(g_pHero->GetName());
	if( !pMember )
		return;
	
	if(pMember->m_eGuildDuty == eGUILD_DUTY_COMMANDER )
	{
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if( !pSystemMan)
			return;
		//1485	훈련대장으로 임명할 아이디를 정확하게 입력하세요.
		pSystemMan->Sys_ConfirmMessageUnLock (g_InterfaceManager.GetInterfaceString(1485), _CallBack_CreateCamp);
		pSystemMan->Sys_ConfirmMessageSetFocus();
	}
	else
	{
		if(pMember->m_eGuildDuty > eGUILD_DUTY_NONE && pMember->m_eGuildDuty <= eGUILD_DUTY_STRATEGIST)
		{
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if( !pSystemMan)
				return;
			//1389 군단장으로 임명할 아이디를 정확하게 입력하세요
			pSystemMan->Sys_ConfirmMessageUnLock (g_InterfaceManager.GetInterfaceString(1389), _CallBack_CreateCorp);
			pSystemMan->Sys_ConfirmMessageSetFocus();
		}
		else
		{
			//6271	해당 기능을 이용할 권한이 없습니다.
			pSystemMan->Sys_Verify(g_InterfaceManager.GetInterfaceString(6271) , FALSE);
		}
	}
}

void uiGuildMenu::UpdateButtonShowNHide()
{
    if (GetManager() == NULL)
        return;

	ENUM_STATEWZ eState = (GetManager()->IsGuildInfo()  == TRUE ? WZ_SHOW : WZ_HIDE);

	TCHAR szBtn[INTERFACE_STRING_LENGTH + 1] = {0,};

	if(GetManager()->IsGuildInfo())
	{
		if(eGUILD_DELCHK_DESTROY_WAIT == GetManager()->GetGuildDelChk())
		{
			eState = WZ_HIDE; //길드 해산 대기 상태면 "해산 대기" 버튼 한개만 Show

			//6230 길드 해산 해제
			g_InterfaceManager.GetInterfaceString(6230, szBtn);
		}
		else if(eGUILD_DELCHK_NORMAL == GetManager()->GetGuildDelChk())
		{
			//1502 길드 해산
			g_InterfaceManager.GetInterfaceString(1052, szBtn); 
		}
	}
	else
	{
		//4530 길드 창설
		g_InterfaceManager.GetInterfaceString(4530, szBtn);
	}

	m_pBtnControls[DLGPOS_BTN_GUILD_CREATE_AND_DESTORY]->SetTextWZ(szBtn);


	this->m_pBtnControls[DLGPOS_BTN_GUILD_RANK_UP]->ShowWindowWZ(eState);

	m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_CREATE]->ShowWindowWZ(eState);
	m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_DESTORY]->ShowWindowWZ(eState);

	stGUILD_MEMBER_INFO * pMyGuildMemberInfo = GetManager()->GetGuildMemberInfo(g_pHero->GetName());
	if( pMyGuildMemberInfo )
	{
		// 만약 군단장 아래 등급이면 아래 버튼은 Hide 시킴
		if( pMyGuildMemberInfo->m_eGuildDuty > eGUILD_DUTY_COMMANDER)
		{
			m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_CREATE]->ShowWindowWZ(WZ_HIDE);
			m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_DESTORY]->ShowWindowWZ(WZ_HIDE);
		}
	}

	m_pBtnControls[DLGPOS_BTN_GUILD_MARK]->ShowWindowWZ(eState);
	m_pBtnControls[DLGPOS_BTN_GUILD_WAREHOUSE]->ShowWindowWZ(eState);
}

void uiGuildMenu::UpdateButtonCaption()
{
	//일단은 이렇게 넣어 놓자

	if( !GetManager())
		return;

	if( !g_pHero)
		return;

	stGUILD_MEMBER_INFO * pMember = GetManager()->GetGuildMemberInfo(g_pHero->GetName());

	if( !pMember)
		return;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

	if( pMember->m_eGuildDuty == eGUILD_DUTY_COMMANDER )
	{
		//1488	육성캠프창설
		g_InterfaceManager.GetInterfaceString(1488 ,szMessage);
		GlobalFunc::SetCaption(m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_CREATE] , szMessage);

		//1490	육성캠프 해산
		g_InterfaceManager.GetInterfaceString(1490,szMessage);
		GlobalFunc::SetCaption(m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_DESTORY] , szMessage);

	}
	else
	{
		//1487	군단창설
		g_InterfaceManager.GetInterfaceString(1487, szMessage );
		GlobalFunc::SetCaption(m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_CREATE] , szMessage);

		//1489	군단 해산
		g_InterfaceManager.GetInterfaceString(1489, szMessage );
		GlobalFunc::SetCaption(m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_DESTORY] , szMessage);
	}
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
