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
				//7228	�ش� ������ ���� ���� �ʽ��ϴ�.
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
				////7228	�ش� ������ ���� ���� �ʽ��ϴ�.
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
		//1486	����ķ���� �ػ��Ͻðڽ��ϱ�?
		pSystemMan->Sys_ConfirmLock(g_InterfaceManager.GetInterfaceString(1486), _CallBack_DestoryCamp);
	}
	else
	{
		////1390	������ �ػ��Ͻðڽ��ϱ�?
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
		//1485	�Ʒô������� �Ӹ��� ���̵� ��Ȯ�ϰ� �Է��ϼ���.
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
			//1389 ���������� �Ӹ��� ���̵� ��Ȯ�ϰ� �Է��ϼ���
			pSystemMan->Sys_ConfirmMessageUnLock (g_InterfaceManager.GetInterfaceString(1389), _CallBack_CreateCorp);
			pSystemMan->Sys_ConfirmMessageSetFocus();
		}
		else
		{
			//6271	�ش� ����� �̿��� ������ �����ϴ�.
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
			eState = WZ_HIDE; //��� �ػ� ��� ���¸� "�ػ� ���" ��ư �Ѱ��� Show

			//6230 ��� �ػ� ����
			g_InterfaceManager.GetInterfaceString(6230, szBtn);
		}
		else if(eGUILD_DELCHK_NORMAL == GetManager()->GetGuildDelChk())
		{
			//1502 ��� �ػ�
			g_InterfaceManager.GetInterfaceString(1052, szBtn); 
		}
	}
	else
	{
		//4530 ��� â��
		g_InterfaceManager.GetInterfaceString(4530, szBtn);
	}

	m_pBtnControls[DLGPOS_BTN_GUILD_CREATE_AND_DESTORY]->SetTextWZ(szBtn);


	this->m_pBtnControls[DLGPOS_BTN_GUILD_RANK_UP]->ShowWindowWZ(eState);

	m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_CREATE]->ShowWindowWZ(eState);
	m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_DESTORY]->ShowWindowWZ(eState);

	stGUILD_MEMBER_INFO * pMyGuildMemberInfo = GetManager()->GetGuildMemberInfo(g_pHero->GetName());
	if( pMyGuildMemberInfo )
	{
		// ���� ������ �Ʒ� ����̸� �Ʒ� ��ư�� Hide ��Ŵ
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
	//�ϴ��� �̷��� �־� ����

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
		//1488	����ķ��â��
		g_InterfaceManager.GetInterfaceString(1488 ,szMessage);
		GlobalFunc::SetCaption(m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_CREATE] , szMessage);

		//1490	����ķ�� �ػ�
		g_InterfaceManager.GetInterfaceString(1490,szMessage);
		GlobalFunc::SetCaption(m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_DESTORY] , szMessage);

	}
	else
	{
		//1487	����â��
		g_InterfaceManager.GetInterfaceString(1487, szMessage );
		GlobalFunc::SetCaption(m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_CREATE] , szMessage);

		//1489	���� �ػ�
		g_InterfaceManager.GetInterfaceString(1489, szMessage );
		GlobalFunc::SetCaption(m_pBtnControls[DLGPOS_BTN_CORP_AND_CAMP_DESTORY] , szMessage);
	}
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
