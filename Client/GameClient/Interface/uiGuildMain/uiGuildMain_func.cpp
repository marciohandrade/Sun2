//------------------------------------------------------------------------------
//  uiGuildCreate_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMain.h"
#include "battleroomentry.h"
#include "MapInfoParser.h"
#include "BattleScene.h"
#include "uiGuildMan/uiGuildMan.h"
#include "interfacemanager.h"
#include "GlobalFunc.h"
#include "uiGuildDutyVest/uiGuildDutyVest.h"
#include "uiGuildRightDelegation/uiGuildRightDelegation.h"
#include "uisystemman/uiSystemMan.h"
#include "GuildRankInfoParser.h"
#include "uiGuildMemberRightView/uiGuildMemberRightView.h"
#include "GuildRightInfoParser.h"
#include "uiGuildNotice/uiGuildNotice.h"
#include "uiGuildCorpsChange/uiGuildCorpsChange.h"
#include "GameConst_Color.h"
//------------------------------------------------------------------------------
/**
*/
void fnGuildMasterDelegation(bool bYes)
{
	if( bYes )
	{
		uiGuildMain * pGuildMain = GET_CAST_DIALOG( uiGuildMain, IM_GUILD_MANAGER::GUILD_MAIN );

		if( pGuildMain )
		{
			MSG_CG_GUILD_CHANGE_MASTER_SYN SendMsg;
			StrnCopy(SendMsg.m_szTargetCharName, pGuildMain->GetAttemptChageMasterName(),MAX_CHARNAME_LENGTH);
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg) );
		}
	}
}

void uiGuildMain::updateMemberInfos()
{
	assert (this->GetManager());
	if (!this->GetManager())
		return;

	if (!this->GetManager()->IsGuildInfo())
		return;

	const VEC_GUILD_MEMBER & rGuildMembers = GetManager()->GetGuildMembers();
	if( rGuildMembers.empty() )
		return;

    int current_select = m_pLstControl->GetCurSel();
    
	this->m_pLstControl->DeleteItemAll();
	this->m_pLstControl->SetBeginDrawIndex(0);
	this->m_pLstControl->SetColorSelBar(RGBA (150, 10, 10, 180));

	int start, iCorpMemberNum =0;

	start = this->m_pScrollControl->GetScrollPosWZ();

	for (int i=0; i<(int)rGuildMembers.size(); ++i)
	{
		if( rGuildMembers[i].m_byCorpsNum != m_iCurSelCorps)
			continue;

		iCorpMemberNum++;
		if( iCorpMemberNum <= m_MaxPage )
		{
			int idx = this->m_pLstControl->InsertItemBack();
			this->m_pLstControl->SetItemBKColor(idx, 0, RGBA (150, 10, 10, 180));
		}
	}

	int iPage = iCorpMemberNum- m_MaxPage;

	if(iPage<=0)iPage=0;

	this->m_pScrollControl->SetScrollRangeWZ(0, iPage);

    m_pLstControl->SetCurSel(current_select);
}


//------------------------------------------------------------------------------
/**
*/
void
uiGuildMain::renderMemberInfos()
{
	assert (this->GetManager());
	if (!this->GetManager())
		return;

	if (!this->GetManager()->IsGuildInfo())
		return;

	DWORD dwNameColor = WzColor_RGBA (255, 255, 255, 255);

	const int c_iTOP_Margin = 3;
	const int c_iLEFT_Margin = 2;
	const int c_iHEIGHT_Gap = 20;

	RECT rc_listctrl;
	rc_listctrl = this->m_pLstControl->GetSizeRect();
	rc_listctrl.top += c_iTOP_Margin;
	rc_listctrl.left += c_iLEFT_Margin;

#ifdef _INTERNATIONAL_UI
	this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(StrToWzID("st10"));
#else
	this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(StrToWzID("mn12"));
#endif//_INTERNATIONAL_UI

	RECT rc;
	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

	int i,insertCount, Ypos, Xpos1, Xpos2;

	int iAllCorpMemberIndex =0;
	int iStartIndex = 0;

	iStartIndex = this->m_pScrollControl->GetScrollPosWZ();

	const VEC_GUILD_MEMBER & rGuildMembers = GetManager()->GetGuildMembers();
	for (i=0, insertCount=0; i<(int)rGuildMembers.size(); ++i)
	{
		stGUILD_MEMBER_INFO* pMember =
			const_cast<stGUILD_MEMBER_INFO *>(&rGuildMembers[i]);
		assert (pMember);

		//���ܿ� ���� ���͸�
		if( pMember->m_byCorpsNum != m_iCurSelCorps)
			continue;

		// ��ũ�� ������ġ�� ���� ��ŵó��
		iAllCorpMemberIndex++;
		if( iAllCorpMemberIndex <= iStartIndex)
			continue;

		Ypos = insertCount;
		insertCount++;
		if (insertCount > m_MaxPage)
			break;
		Xpos1 = rc_listctrl.left;

		if(pMember->m_byChaoState==eCHAO_STATE_PREV_CHAO)
			dwNameColor = WzColor_RGBA (255, 128, 0, 255);
		else if(pMember->m_byChaoState==eCHAO_STATE_CHAO)
			dwNameColor = WzColor_RGBA (255, 0, 0, 255);
		else
			dwNameColor = WzColor_RGBA (255, 255, 255, 255);

		if(!pMember->m_bLogin)	// ���� ���ο� ���� ���� ����
		{
			dwNameColor = WzColor_RGBA (128, 128, 128, 255);	// ���Ӿ��� : ȸ��
		}

		//  ���� �̸�, ����, ����, ���� ����

		//  ���� �̸�
		Xpos2 = Xpos1 + m_pSortName->GetSize().right;

#ifdef _INTERNATIONAL_UI
		Xpos2-=7;
#endif//_INTERNATIONAL_UI
		

		SetRect(&rc, Xpos1, rc_listctrl.top + (Ypos*c_iHEIGHT_Gap),
			Xpos2, rc_listctrl.top + (Ypos*c_iHEIGHT_Gap) + c_iHEIGHT_Gap);


		//Todo:ī�� �Ӽ� �޾Ƽ� ���� �ٸ��� ��� -���� 
		Sprintf (szMessage, _T("%s"),pMember->m_tszCharName);
		
		this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage,&rc,
			dwNameColor,WzColor_RGBA(  0,  0,  0,   0), TP_HCENTER );//TP_HLEFT);
		
		//  ����
		Xpos1 = Xpos2;
		Xpos2 = Xpos1 + m_pSortLevel->GetSize().right;

		SetRect(&rc, Xpos1, rc_listctrl.top + (Ypos*c_iHEIGHT_Gap),
				Xpos2, rc_listctrl.top + (Ypos*c_iHEIGHT_Gap) + c_iHEIGHT_Gap);

		Sprintf (szMessage, _T("%d"), pMember->m_Level);
		this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage,
			&rc,
			dwNameColor,
			WzColor_RGBA(  0,  0,  0,   0),
			TP_HCENTER);


		//  ����
		Xpos1 = Xpos2;
		Xpos2 = Xpos1 + m_pSortClass->GetSize().right;

		SetRect( &rc, Xpos1, rc_listctrl.top + (Ypos*c_iHEIGHT_Gap),
			    Xpos2, rc_listctrl.top + (Ypos*c_iHEIGHT_Gap) + c_iHEIGHT_Gap );

		if (pMember->m_byClass > 5 || (pMember->m_byClass < 0) )
		{
			Sprintf (szMessage, _T("??"));
		}
		else
		{
            Snprintf(szMessage, INTERFACE_STRING_LENGTH, _T("%s"),
                GlobalFunc::GetClassString(static_cast<eCHAR_TYPE>(pMember->m_byClass)));
		}
		this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage,
			&rc, dwNameColor, WzColor_RGBA(  0,  0,  0,   0), TP_HCENTER);

		//��å
		Xpos1 = Xpos2;
		Xpos2 = Xpos1 + m_pSortDuty->GetSize().right;

		SetRect(&rc, Xpos1, rc_listctrl.top + (Ypos * c_iHEIGHT_Gap),
			Xpos2, rc_listctrl.top + (Ypos*c_iHEIGHT_Gap) + c_iHEIGHT_Gap);

		GUILD_DUTY_USE_INFO * pGuildUseInfo = GuildRankInfoParser::Instance()->GetGuildDutyUseInfo(
			GetManager()->GetGuildInfo().m_GuildGrade , pMember->m_eGuildDuty);
		if( pGuildUseInfo )
		{
			g_InterfaceManager.GetInterfaceString(pGuildUseInfo->m_dwStringCode , szMessage ,INTERFACE_STRING_LENGTH);
			this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage,
				&rc, dwNameColor, WzColor_RGBA(  0,  0,  0,   0),TP_HCENTER);
		}

		//	������ / ���Ӿ���
		Xpos1 = Xpos2;
		Xpos2 = Xpos1 + m_pSortConnect->GetSize().right;

		SetRect(&rc, Xpos1, rc_listctrl.top + (Ypos*c_iHEIGHT_Gap),
			Xpos2, rc_listctrl.top + (Ypos*c_iHEIGHT_Gap) + c_iHEIGHT_Gap);

		if( pMember->m_bLogin )
		{
			g_InterfaceManager.GetInterfaceString( eST_CONNECTING, szMessage, INTERFACE_STRING_LENGTH );
		}
		else
		{
			g_InterfaceManager.GetInterfaceString( eST_NOT_CONNECTING, szMessage, INTERFACE_STRING_LENGTH );
		}
		this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage,
			&rc, dwNameColor, WzColor_RGBA(  0,  0,  0,   0), TP_HCENTER);
	}

	if( m_pListCorps->GetState() == WZ_SHOW)
	{
		m_pListCorps->OnDraw(TRUE); // <- ����Ʈ â �Ʒ��� �۾��� ���̹Ƿ� ����Ʈ�� �ѹ��� �׷�����!
	}

	int iTotalCorpsMemberNum = 0;
	int iCorpsOnlineMemberNum = 0;
	for( size_t i =0; i < rGuildMembers.size(); ++i)
	{
		if( rGuildMembers[i].m_byCorpsNum == m_iCurSelCorps )
		{
			iTotalCorpsMemberNum++;
			if( rGuildMembers[i].m_bLogin)
				iCorpsOnlineMemberNum++;
		}
	}
	Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, _T("(%d /%d)"), iCorpsOnlineMemberNum , iTotalCorpsMemberNum );
	GlobalFunc::SetCaption(m_pStaticNumGuildMember, szMessage);

}

void uiGuildMain::OnClickDutyVest()
{
	if( !GetManager() || !g_pHero)
		return;

	stGUILD_MEMBER_INFO * member = GetSelectedPacketForGuildMember();
	const stGUILD_MEMBER_INFO * pHeroMemberInfo = GetManager()->GetGuildMemberInfo(g_pHero->GetName());
	if( member && pHeroMemberInfo )
	{
		if( StrnCmp(member->m_tszCharName , g_pHero->GetName(), MAX_CHARNAME_LENGTH) == 0 )
		{
			//5197	�ڽſ��Դ� �̿��Ҽ� ���� ����Դϴ�.
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5197));
			return;
		}

		bool bHeroHighDuty = (member->m_eGuildDuty - pHeroMemberInfo->m_eGuildDuty > 0) && pHeroMemberInfo->m_eGuildDuty != 0;
		if( bHeroHighDuty ) //Hero�� ��å�� �� ����
		{
			uiGuildDutyVest * pDutyVest = GET_CAST_DIALOG( uiGuildDutyVest, IM_GUILD_MANAGER::GUILD_DUTY_VEST );

			if( pDutyVest )
			{
				pDutyVest->SetTargetName(member->m_tszCharName);
				pDutyVest->ShowInterface(TRUE);
			}
		}
		else
		{
			//6195 ���� ��å�� �ִ� �������Դ� ��å�� �ο��� �� �����ϴ�.
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6195) );
			return;
		}
	}
}
void uiGuildMain::OnClickRightDelegation()
{
	if( !GetManager() || !g_pHero)
		return;
	
	const stGUILD_MEMBER_INFO * pHeroMemberInfo = GetManager()->GetGuildMemberInfo(g_pHero->GetName());
	stGUILD_MEMBER_INFO * member = GetSelectedPacketForGuildMember();
	if( member && pHeroMemberInfo)
	{
		if( StrnCmp(member->m_tszCharName , g_pHero->GetName(), MAX_CHARNAME_LENGTH) == 0 )
		{
			//5197	�ڽſ��Դ� �̿��Ҽ� ���� ����Դϴ�.
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5197));
			return;
		}

		bool bGuildMaster = pHeroMemberInfo->m_eGuildDuty == eGUILD_DUTY_MASTER;
		if( ! bGuildMaster)
		{
			//6221	����常 ��� ������ ����Դϴ�.
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6221));
			return;
		}

		bool bTargetAvalible = (member->m_eGuildDuty == eGUILD_DUTY_SUBMASTER || member->m_eGuildDuty == eGUILD_DUTY_STRATEGIST);
		if( !bTargetAvalible)
		{
			//6197 ���� ������,���翡�Ը� ������ �����Ҽ� �ֽ��ϴ�.
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6197));
			return;
		}

		MSG_CG_GUILD_RIGHT_DELEGATION_LIST_SYN SendPacket;
		Snprintf(SendPacket.m_szTargetCharName , MAX_CHARNAME_LENGTH , member->m_tszCharName);
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
	}
}
void uiGuildMain::OnClickGuildMasterDelegation()
{
	if( !GetManager() || !g_pHero)
		return;

	const stGUILD_MEMBER_INFO * pHeroMemberInfo = GetManager()->GetGuildMemberInfo(g_pHero->GetName());
	stGUILD_MEMBER_INFO * member = GetSelectedPacketForGuildMember();
	if( member && pHeroMemberInfo)
	{
		if( StrnCmp(member->m_tszCharName , g_pHero->GetName(), MAX_CHARNAME_LENGTH) == 0 )
		{
			//5197	�ڽſ��Դ� �̿��Ҽ� ���� ����Դϴ�.
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(5197));
			return;
		}

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
		{
			m_strAttemptChageMasterName = member->m_tszCharName;
			TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
			Snprintf(szMessage, INTERFACE_STRING_LENGTH -1,g_InterfaceManager.GetInterfaceString(6214), member->m_tszCharName);
			//6214	������� 1�� �����ϸ� 14�ϰ� ����� ������ �������� �ʽ��ϴ�. \n%s������ ����� ������ �Ͻðڽ��ϱ�?
			pSystemMan->Sys_ConfirmLock(szMessage, fnGuildMasterDelegation);
		}
	}
}

void uiGuildMain::OpenGuildNotice()
{
	uiGuildMan* pGuildMan  = GET_DIALOG_MANAGER( uiGuildMan, UIMAN_GUILD );

	if(!g_pHero || !pGuildMan) return; 

	//1.�ڽ��� ����������
	stGUILD_MEMBER_INFO * pHeroInfo = pGuildMan->GetGuildMemberInfo(g_pHero->GetName());

	if(!pHeroInfo)
		return;

	//2.���������� �ִ��� üũ
    BOOL bRightNotice = FALSE;
    if ((GuildRightInfoParser::Instance()->CheckDutyForRight(eGUILD_RIGHTS_NOTICE, pHeroInfo->m_eGuildDuty) == TRUE) && 
        (GuildRightInfoParser::Instance()->CheckGradeForRight(eGUILD_RIGHTS_NOTICE, GetManager()->GetGuildInfo().m_GuildGrade) == TRUE))
    {
        bRightNotice = TRUE;
    }

	if(bRightNotice)
	{

		uiBase * pGuildNotice = GET_DIALOG( IM_GUILD_MANAGER::GUILD_NOTICE );
				
		if(pGuildNotice)
		{
			//2_1:�������ִٸ� ����â�����ֱ�
			pGuildNotice->ShowInterface(TRUE);
		}
	}
	else
	{
		//2_2:�����̾��ٸ� �޽������
		//7206 ����� ������ ���ų�,���� ���� ������ �����ϴ�
		TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] ={0,};
		g_InterfaceManager.GetInterfaceString(7206, szMessage);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	}
}

void uiGuildMain::OnClickGuildMemberRightInfo()
{
	stGUILD_MEMBER_INFO * pMember = GetSelectedPacketForGuildMember();
	if( pMember )
	{
		if( pMember->m_eGuildDuty == eGUILD_DUTY_SUBMASTER ||
			pMember->m_eGuildDuty == eGUILD_DUTY_STRATEGIST)
		{
			// 
			m_bClickMemberRightInfoButton = true;
			MSG_CG_GUILD_RIGHT_DELEGATION_LIST_SYN SendPacket;
			Snprintf(SendPacket.m_szTargetCharName , MAX_CHARNAME_LENGTH , pMember->m_tszCharName);
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
		}
		else
		{
			//TO Do: uiGuildMeberRightView ���̾�α� ����
			uiGuildMemberRightView * pGuildMemberRightVew = GET_CAST_DIALOG( uiGuildMemberRightView, IM_GUILD_MANAGER::GUILD_MEMBER_RIGHT_VIEW );

			if( pGuildMemberRightVew )
			{
				pGuildMemberRightVew->ShowInterface(TRUE);
				pGuildMemberRightVew->RegisteAllItem(pMember, NULL);
			}
		}
	}
}
//���� ���õ� ��� ����� ��ȯ �Ѵ�.
stGUILD_MEMBER_INFO * uiGuildMain::GetSelectedPacketForGuildMember()
{
	if( !GetManager())
		return NULL;

	int cursel = this->m_pLstControl->GetCurSel();
	if( cursel >= 0)
	{

		int start = this->m_pScrollControl->GetScrollPosWZ();

		int iIndex = start + cursel;

		VEC_GUILD_MEMBER & rGuildMembers =  const_cast<VEC_GUILD_MEMBER &>(GetManager()->GetGuildMembers());

		int RealCount = 0;
		int FilterCount = 0;
		for( size_t i = 0; i < rGuildMembers.size();i ++)
		{
			// ���� ���õ� ���ܰ� ��ġ�ϴ� �ɹ��� �˻��Ѵ�.
			if( rGuildMembers[i].m_byCorpsNum == m_iCurSelCorps)
			{
				if( FilterCount == iIndex)
				{
					return &rGuildMembers[RealCount];
				}
				FilterCount++;
			}
			RealCount++;
		}
	}

	return NULL;
}


void uiGuildMain::OnClickGuildCropChange()
{
	if( !GetManager())
		return;

	stGUILD_MEMBER_INFO * pMember = GetSelectedPacketForGuildMember();
	if( !pMember)
		return;

	VEC_GUILD_MEMBER & rGuildMembers = const_cast<VEC_GUILD_MEMBER &>(GetManager()->GetGuildMembers());
	//TODO: pMember Name �� bStateCorps �� UI�� ����.
	uiGuildCorpsChange * pGuildCorpsChange = GET_CAST_DIALOG( uiGuildCorpsChange, IM_GUILD_MANAGER::GUILD_CORPS_CHANGE );
	
	if( !pGuildCorpsChange )
		return;

	pGuildCorpsChange->InitAllCheckButton();
	int index = 0;
	for( int i =0; i< MAX_GUILD_CORPS_NUM; ++i)
	{
		if( GetManager()->GetCorpsTotalInfo().m_CorpsInfo[i].m_bCreate)
		{
			if( pMember->m_byCorpsNum != i)
			{
				pGuildCorpsChange->ShowAndInsertCheckCorps(index, i);
				index++;
			}
		}
	}
	if( index == 0 )
	{
		//6259	�̵��� ������ �������� �ʽ��ϴ�.
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6259));
		return;
	}
	pGuildCorpsChange->SetTargetName(pMember->m_tszCharName);
	pGuildCorpsChange->ShowInterface(TRUE);
}

void uiGuildMain::InsertAllCorps()
{
	if( !GetManager() )
		return;
	const VEC_GUILD_MEMBER &  rGuildMembers =  GetManager()->GetGuildMembers();

	// ȿ���� ���� ������ �ε����� �迭�� ������ �ִ´�.
	int	 arrIndexCorpMaster[MAX_GUILD_CORPS_NUM];
	memset(arrIndexCorpMaster,-1,sizeof(arrIndexCorpMaster));
	for( size_t i = 0; i < rGuildMembers.size(); ++i )
	{	
		const stGUILD_MEMBER_INFO * pMember =&rGuildMembers[i];

		if( pMember )
		{
			if( pMember->m_byCorpsNum >= 0 && pMember->m_byCorpsNum < MAX_GUILD_CORPS_NUM)
			{
				//������ �ΰ�?
				if( pMember->m_eGuildDuty == eGUILD_DUTY_COMMANDER || pMember->m_eGuildDuty == eGUILD_DUTY_MASTER)
				{
					arrIndexCorpMaster[pMember->m_byCorpsNum] = i;
				}
			}
		}
	}	

	GUILD_CORPS_TOTAL_INFO & rCorpsTotalInfo = (GUILD_CORPS_TOTAL_INFO &)GetManager()->GetCorpsTotalInfo();
	m_pListCorps->DeleteItemAll();
	for( int i =0; i < MAX_GUILD_CORPS_NUM; ++i)
	{
		//������ â���Ǿ��ִ�
		if( rCorpsTotalInfo.m_CorpsInfo[i].m_bCreate)
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

			//������ ������ �ε��� �迭�� ����� �ٷ� ������ ����.
			int iCorpsMasterIndex = arrIndexCorpMaster[i];

			if( iCorpsMasterIndex >= 0) // �������� �ִ�
			{
				const stGUILD_MEMBER_INFO * pMember = &rGuildMembers[iCorpsMasterIndex];
				if( pMember )
				{
					//1269	%s ���� : %s
					TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
					g_InterfaceManager.GetInterfaceString(1269, szTemp);
					Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, szTemp, GlobalFunc::GetCorpsName(i), pMember->m_tszCharName);
				}
			}
			else	// �������� �����̴�.
			{
				//6285	%s ����
				Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(6285) , GlobalFunc::GetCorpsName(i) );
			}

			if( rCorpsTotalInfo.m_CorpsInfo[i].m_bRearCamp)
			{
				//1093	(����ķ������)
				_tcscat(szMessage, g_InterfaceManager.GetInterfaceString(1093));
			}

			int ipushIndex = m_pListCorps->InsertItemBack();
#ifdef _INTERNATIONAL_UI
			GlobalFunc::SetItem(m_pListCorps, ipushIndex, 0, szMessage,StrToWzID("st10"), DT_LEFT,
				c_InterfaceBKColor,c_InterfaceTextColor,c_InterfaceSelColor);
#else
			GlobalFunc::SetItem(m_pListCorps, ipushIndex, 0, szMessage,StrToWzID("mn12"), DT_LEFT,
				c_InterfaceBKColor,c_InterfaceTextColor,c_InterfaceSelColor);
#endif//_INTERNATIONAL_UI
				

			m_pListCorps->SetItemData(ipushIndex, &rCorpsTotalInfo.m_CorpsInfo[i].m_byCorpsNum);
		}
	}
}

void uiGuildMain::SetListCaption()
{
    if (IsVisible() == FALSE)
    {
        return;
    }

	if( !GetManager())
		return;

	if( m_iCurSelCorps >= MAX_GUILD_CORPS_NUM)
		return;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	VEC_GUILD_MEMBER & rGuildMember = const_cast< VEC_GUILD_MEMBER &> (GetManager()->GetGuildMembers());

	GUILD_CORPS_TOTAL_INFO & rCorpsTotalInfo = (GUILD_CORPS_TOTAL_INFO &)GetManager()->GetCorpsTotalInfo();
	
	if( rCorpsTotalInfo.m_CorpsInfo[m_iCurSelCorps].m_bCreate )
	{
		//6285	%s ����
		Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, g_InterfaceManager.GetInterfaceString(6285), GlobalFunc::GetCorpsName(m_iCurSelCorps));

		for( size_t i = 0; i < rGuildMember.size(); ++i )
		{	
			stGUILD_MEMBER_INFO* pMember =
				static_cast<stGUILD_MEMBER_INFO *>(&rGuildMember[i]);

			if( pMember )
			{
				if( pMember->m_byCorpsNum == m_iCurSelCorps)
				{
					//������ �ΰ�?
					if( pMember->m_eGuildDuty == eGUILD_DUTY_COMMANDER || pMember->m_eGuildDuty == eGUILD_DUTY_MASTER)
					{
						//1269	%s ���� : %s
						TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
						g_InterfaceManager.GetInterfaceString(1269, szTemp);
						Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, szTemp, GlobalFunc::GetCorpsName(m_iCurSelCorps),
							pMember->m_tszCharName);	

						if( GetManager()->GetCorpsTotalInfo().m_CorpsInfo[m_iCurSelCorps].m_bRearCamp )
						{
							////1093	(����ķ������)
							_tcscat(szMessage, g_InterfaceManager.GetInterfaceString(1093));
						}

						GlobalFunc::SetCaption(m_pCaptinCurSel,szMessage);
						return;
					}
				}
			}
		}
		if( GetManager()->GetCorpsTotalInfo().m_CorpsInfo[m_iCurSelCorps].m_bRearCamp )
		{
			////1093	(����ķ������)
			_tcscat(szMessage, g_InterfaceManager.GetInterfaceString(1093));
		}
	}	

	GlobalFunc::SetCaption(m_pCaptinCurSel,szMessage);
}
void uiGuildMain::updateMouseWheel()
{
	
	if( !m_pLstControl || !m_pScrollControl)
		return;

	RECT rc = m_pLstControl->GetSizeRect();
	switch (GetMouseWheelStatus(&rc))
	{
	default:
		break;
	case MOUSE_WHEEL_UP:
		{
			int iMin = 0 , iMax = 0;
			m_pScrollControl->GetScrollRangeWZ( &iMin, &iMax);
			int iCurPos = m_pScrollControl->GetScrollPosWZ();
			if( iMin < iCurPos )
			{
				iCurPos--;
				m_pScrollControl->SetScrollPosWZ(iCurPos);
			}

		}
		break;

	case MOUSE_WHEEL_DOWN:
		{
			int iMin = 0 , iMax = 0;
			m_pScrollControl->GetScrollRangeWZ( &iMin, &iMax);
			int iCurPos = m_pScrollControl->GetScrollPosWZ();

			if( iCurPos < iMax )
			{
				iCurPos++;
				m_pScrollControl->SetScrollPosWZ(iCurPos);
			}
		}
		break;
	}
}

//------------------------------------------------------------------------------
stGUILD_MEMBER_INFO* uiGuildMain::GetMemberInfo_byListboxCurSel()
{
    if ((m_pLstControl == NULL) ||
	   (m_pScrollControl == NULL))
    {
        return NULL;
    }

    int select_index = m_pLstControl->GetCurSel();
    int scroll_start_index = m_pScrollControl->GetScrollPosWZ();

    int display_member_count = 0;
    int corp_member_count = 0;

    const VEC_GUILD_MEMBER& rGuildMembers = GetManager()->GetGuildMembers();
    for (int i = 0; i < (int)rGuildMembers.size(); ++i)
    {
        stGUILD_MEMBER_INFO* member_info = const_cast<stGUILD_MEMBER_INFO *>(&rGuildMembers[i]);

        //���ܿ� ���� ���͸�
        if (member_info->m_byCorpsNum != m_iCurSelCorps)
        {
            continue;
        }

       ++corp_member_count;

        //! ��ũ�� ������ġ�� ���� ��ŵó��
        if (corp_member_count <= scroll_start_index)
        {
            continue;   
        }

        if (select_index == display_member_count)
        {
            return member_info;
        }

        ++display_member_count;
    }

    return NULL;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
