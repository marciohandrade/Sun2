//------------------------------------------------------------------------------
//  uiGuildWindow_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildWindow.h"
#include "uiGuildMan/uiGuildMan.h"
#include "battleroomentry.h"
#include "MapInfoParser.h"
#include "BattleScene.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "GuildRankInfoParser.h"
//------------------------------------------------------------------------------
/**
*/
void
uiGuildWindow::updateGuildInfo()
{
    if (!this->GetManager()->IsGuildInfo())
        return;

    const stGUILD_INFO& guildInfo = this->GetManager()->GetGuildInfo();

	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR   szText[INTERFACE_STRING_LENGTH] = {0,};
	
	int nFocus = POS_TXT_BEGIN;
	
    // ��� �̸�: %s
	g_InterfaceManager.GetInterfaceString( eST_GUILD_NAME, szText, INTERFACE_STRING_LENGTH );
    Sprintf (szMessage, szText, GetManager()->GetGuildName());
    GlobalFunc::SetCaption (this->m_pTxtControls[DLGPOS_TXT_GUILD_NAME-nFocus], szMessage);

    // �����: %s
	g_InterfaceManager.GetInterfaceString( eST_GUILD_MASTER_NAME, szText, INTERFACE_STRING_LENGTH );
    Sprintf (szMessage, szText, GetManager()->GetGuildMasterName());
    GlobalFunc::SetCaption (this->m_pTxtControls[DLGPOS_TXT_MASTER_NAME-nFocus], szMessage);

    // ��� ����: %d
	g_InterfaceManager.GetInterfaceString( eST_GUILD_LEVEL, szText, INTERFACE_STRING_LENGTH );
    Sprintf (szMessage, szText, guildInfo.m_GuildGrade);
    GlobalFunc::SetCaption (this->m_pTxtControls[DLGPOS_TXT_GUILD_LEVEL-nFocus], szMessage);

    // ���� ��: %d
	g_InterfaceManager.GetInterfaceString( eST_GUILD_MEMBER_COUNT, szText, INTERFACE_STRING_LENGTH );

	int iGuildMemberNum = GetManager()->GetGuildMembers().size();
	Sprintf (szMessage, szText, iGuildMemberNum);
    GlobalFunc::SetCaption (this->m_pTxtControls[DLGPOS_TXT_NUM_MEMBERS-nFocus], szMessage);

	TCHAR szPoint[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szPointFinal[INTERFACE_STRING_LENGTH] = {0,};
	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };

	// ��� UP: 0
	//////////////////////////////////////////////////////////////////////////
	Sprintf(szPoint, _T("%d"), guildInfo.m_UserPoint);
	::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szPoint, &nFmt, szPointFinal, sizeof(szPointFinal));
	g_InterfaceManager.GetInterfaceString( eST_GUILD_UP, szText, INTERFACE_STRING_LENGTH );
	Sprintf (szMessage, _T("%s: %s"), szText, szPointFinal);
	GlobalFunc::SetCaption(this->m_pTxtControls[DLGPOS_TXT_GUILD_UP-nFocus], szMessage );
	//////////////////////////////////////////////////////////////////////////

	// ���� UP: 0
	//////////////////////////////////////////////////////////////////////////
	g_InterfaceManager.GetInterfaceString( eST_PERSONAL_UP, szText, INTERFACE_STRING_LENGTH );
	if(g_pHero)
	{
		BASE_PLAYERINFO *	pPlayInfo = g_pHero->GetCharInfo();
		if(pPlayInfo)
		{
			Sprintf(szPoint, _T("%d"), pPlayInfo->m_UserPoint);
			::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szPoint, &nFmt, szPointFinal, sizeof(szPointFinal));
			Sprintf (szMessage, _T("%s: %s"), szText, szPointFinal );
		}
	}
	else
		Sprintf (szMessage, _T("%s: %d"), szText, 0 );

	GlobalFunc::SetCaption (this->m_pTxtControls[DLGPOS_TXT_PERSON_UP-nFocus], szMessage);
	//////////////////////////////////////////////////////////////////////////
	
	// ��� ���� GP: 0
	//////////////////////////////////////////////////////////////////////////
	Sprintf(szPoint, _T("%d"), guildInfo.m_GuildPoint);
	::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szPoint, &nFmt, szPointFinal, sizeof(szPointFinal));
	g_InterfaceManager.GetInterfaceString( eST_GUILD_GP, szText, INTERFACE_STRING_LENGTH );
	Sprintf (szMessage, _T("%s: %s"), szText, szPointFinal );
	GlobalFunc::SetCaption (this->m_pTxtControls[DLGPOS_TXT_GUILD_OWN_GP-nFocus], szMessage);
	//////////////////////////////////////////////////////////////////////////

    // �������� ��
    g_InterfaceManager.GetInterfaceString(3394, szText, INTERFACE_STRING_LENGTH);
    Sprintf(szMessage, szText, guildInfo.m_DominationMapCount);
    GlobalFunc::SetCaption(m_pTxtControls[DLGPOS_TXT_DOMINATION_MAP_COUNT-nFocus], szMessage);
    // ������ ���� Ƚ��
    g_InterfaceManager.GetInterfaceString(3395, szText, INTERFACE_STRING_LENGTH);
    Sprintf(szMessage, szText, guildInfo.m_DominationJoinCount);
    GlobalFunc::SetCaption(m_pTxtControls[DLGPOS_TXT_DOMINATION_JOIN_COUNT-nFocus], szMessage);
    // ������ ��� ���� Ƚ��
    g_InterfaceManager.GetInterfaceString(3396, szText, INTERFACE_STRING_LENGTH);
    Sprintf(szMessage, szText, guildInfo.m_DominationDefenseCount);
    GlobalFunc::SetCaption(m_pTxtControls[DLGPOS_TXT_DOMINATION_DEFENSE_COUNT-nFocus], szMessage);
    // ������ ���� ��� Ƚ��
    g_InterfaceManager.GetInterfaceString(3397, szText, INTERFACE_STRING_LENGTH);
    Sprintf(szMessage, szText, guildInfo.m_DominationHuntingCount);
    GlobalFunc::SetCaption(m_pTxtControls[DLGPOS_TXT_DOMINATION_BOSS_HUNTTING_COUNT-nFocus], szMessage);

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	// ����� ����: %d �� %d �� %d �� %d �� 1393
	g_InterfaceManager.GetInterfaceString( 1393, szText );
	Snprintf(szMessage , INTERFACE_STRING_LENGTH -1 , szText,
			guildInfo.m_ChuntingPVPInfo.m_CHuntingTotCount , guildInfo.m_ChuntingPVPInfo.m_CHuntingVCount,
			guildInfo.m_ChuntingPVPInfo.m_CHuntingLCount, guildInfo.m_ChuntingPVPInfo.m_CHuntingDisCount);
	GlobalFunc::SetCaption(m_pTxtControls[DLGPOS_TXT_GUILD_PVP_RESULT-nFocus] , szMessage);

	// ����� ����Ʈ: %d ����Ʈ 1394
	g_InterfaceManager.GetInterfaceString( 1394, szText );
	Snprintf( szMessage, INTERFACE_STRING_LENGTH -1, szText,
		guildInfo.m_ChuntingPVPInfo.m_CHuntingPVPPoint );

	GlobalFunc::SetCaption(m_pTxtControls[DLGPOS_TXT_GUILD_PVP_POINT-nFocus] , szMessage);
#endif

	GuildRankInfo * pGuildRankInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo(guildInfo.m_GuildGrade);
	if( pGuildRankInfo )
	{
		 //1225 �����: %s
		g_InterfaceManager.GetInterfaceString(1225 , szText, INTERFACE_STRING_LENGTH);
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szText, g_InterfaceManager.GetInterfaceString(pGuildRankInfo->m_dwLevelStringCode));
		SetCaptionGuildGradeName(szMessage);
	}
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildWindow::setGuildNotice()
{
	if(!m_pLstControlNotice)
		return;
	
	if(m_pScroll)
		m_pScroll->SetScrollPosWZ(0);

	this->m_pLstControlNotice->DeleteItemAll();
	this->m_vstrNotice.clear();

	if (!this->GetManager())
		return;

	if (!this->GetManager()->IsGuildInfo())
		return;

	TCHAR *ptszMsg = GetManager()->GetGuildNotice();

	if(ptszMsg == NULL)
		return;

	RECT rc = m_pLstControlNotice->GetSizeRect();
	int iListWidth = rc.right - rc.left - 10;
	GlobalFunc::ProcessWordWrapByFontSize( &m_vstrNotice, ptszMsg, StrToWzID("m212"), _tcslen(ptszMsg), iListWidth );
	this->refreshGuildNotice();
}

//=====================================================================================
//	1.������ ����
//=====================================================================================
void 
uiGuildWindow::refreshGuildNotice()
{	
	//0.��Ʈ�ѷ� �⺻����
	m_pLstControlNotice->DeleteItemAll();
	m_pLstControlNotice->SetSellHeight(20);
	m_pLstControlNotice->SetUseReport(false);	
	m_pLstControlNotice->SetReportUseCnt(0);
	m_pLstControlNotice->SetHideReportRender(true);
	m_pLstControlNotice->SetFontID(StrToWzID("m212"));
	
	//1.��ũ�� ���������
	int iScrollMaxLine = this->m_vstrNotice.size();	
	int iPage = (iScrollMaxLine - SHOW_DEFAULT_NOTICE_LINE);
	if( iPage<=0) iPage =0;
	m_pScroll->SetScrollRangeWZ(0,iPage);
	int iCurScrollPos = m_pScroll->GetScrollPosWZ();

	int iAdd=0;
	//2.����Ʈ��Ʈ�ѿ� ������ ����
	for( int iLine=0; static_cast<size_t>(iLine) <this->m_vstrNotice.size() ; ++iLine)
	{
		//2_1.�⺻��¶��μ��� �����Ѵ�
		if(iAdd>=SHOW_DEFAULT_NOTICE_LINE)
			break;	

		//2_2.��ũ�ѿ���� ����Ʈ�� ��������Ʈ��� 
		if(iLine < iCurScrollPos)
			continue;

		int index = m_pLstControlNotice->InsertItemBack();
				
		TCHAR *pStr = const_cast<TCHAR *>(m_vstrNotice[iLine].c_str());
		GlobalFunc::SetItem(
			m_pLstControlNotice, 
			index, 0, pStr, 
			StrToWzID("m212"),
			DT_VCENTER | DT_LEFT, 
			RGBA(255,255,255,0), 
			RGBA(255,255,255,255), 
			RGBA(255,255,255,255));
		
		++iAdd;
	}

}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
