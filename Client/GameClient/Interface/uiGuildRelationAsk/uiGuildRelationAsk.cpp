#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "InterfaceManager.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiGuildRelationAsk/uiGuildRelationAsk.h"
#include "uiGuildHostility/uiGuildHostility.h"
#include "uiGuildAlliance/uiGuildAlliance.h"
#include "uiGuildOneSideHostility/uiGuildOneSideHostility.h"
#include "GlobalFunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Mouse.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiGuildRelationAsk::m_wzID[eDIALOG_GUILD_RELATION_ASK_MAX] = 
{
		StrToWzID("T100"),//eDIALOG_GUILD_RELATION_ASK_TITLE,
		StrToWzID("B100"),//eDIALOG_GUILD_RELATION_ASK_EXIT_BTN,
		StrToWzID("B101"),//eDIALOG_GUILD_RELATION_ASK_OK_BTN,
		StrToWzID("B102"),//eDIALOG_GUILD_RELATION_ASK_CANCEL_BTN,

		StrToWzID("T101"),//eDIALOG_GUILD_RELATION_ASK_TEXT,
		StrToWzID("E100"),//eDIALOG_GUILD_RELATION_ASK_EDIT,
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiGuildRelationAsk::uiGuildRelationAsk(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiGuildRelationAsk::~uiGuildRelationAsk()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildRelationAsk::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildRelationAsk::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildRelationAsk::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildRelationAsk::OnRenderSolarDialog()
{
#ifdef _INTERNATIONAL_UI
	if(m_pMainText)
		AddMainTextInfo();
#endif//_INTERNATIONAL_UI
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildRelationAsk::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildRelationAsk::OnShowWindow(BOOL val)
{
	if (val)
	{
		TCHAR szTitle[INTERFACE_STRING_LENGTH + 1] = {0,};
		TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
		TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
		TCHAR szRelationState1[INTERFACE_STRING_LENGTH + 1] = {0,};
		TCHAR szRelationState2[INTERFACE_STRING_LENGTH + 1] = {0,};
		TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
		g_InterfaceManager.GetInterfaceString(1166, szMessage);//%s %s 길드명을 정확히 입력하세요
		g_InterfaceManager.GetInterfaceString(1216, szTemp);   //%s

		if(m_bRelationState)
		{
			g_InterfaceManager.GetInterfaceString(6216, szRelationState2);//권유할
		}
		else
		{
			g_InterfaceManager.GetInterfaceString(6217, szRelationState2);//해제할
		}

		switch(m_btRelation)
		{
		case eGUILD_RELATION_NONE:
			{
				assert(!"uiGuildRelationAsk::OnShowWindow - case eGUILD_RELATION_NONE");
			}
			break;

		case eGUILD_RELATION_ALLIENCE:
			{
				if(m_bRelationState)
					g_InterfaceManager.GetInterfaceString(1048, szTitle, INTERFACE_STRING_LENGTH);//동맹신청
				else
					g_InterfaceManager.GetInterfaceString(1172, szTitle, INTERFACE_STRING_LENGTH);//동맹해제

				g_InterfaceManager.GetInterfaceString(6215, szRelationState1);//동맹을
				Snprintf(szFinal, INTERFACE_STRING_LENGTH, szMessage, szRelationState1, szRelationState2);
			}
			break;

		case eGUILD_RELATION_PROCLAIM_HOSTILITY:
			{
				if(m_bRelationState)
					g_InterfaceManager.GetInterfaceString(1049, szTitle, INTERFACE_STRING_LENGTH);//적대신청
				else
					g_InterfaceManager.GetInterfaceString(1173, szTitle, INTERFACE_STRING_LENGTH);//적대해제

				g_InterfaceManager.GetInterfaceString(6219, szRelationState1);//적대를
				Snprintf(szFinal, INTERFACE_STRING_LENGTH, szMessage, szRelationState1, szRelationState2);
			}
			break;

		case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
			{
				if(m_bRelationState)
					g_InterfaceManager.GetInterfaceString(1198, szTitle, INTERFACE_STRING_LENGTH);//쌍방적대신청
				else
				{
					assert(!"uiGuildRelationAsk::OnShowWindow - case eGUILD_RELATION_ONE_SIDE_HOSTILITY");
				}

				g_InterfaceManager.GetInterfaceString(6220, szRelationState1);//쌍방적대를
				Snprintf(szFinal, INTERFACE_STRING_LENGTH, szMessage, szRelationState1, szRelationState2);
			}
			break;

		default:
			{
				assert(!"uiGuildRelationAsk::OnShowWindow - default");
			}
		}

		m_pTitle->SetTextWZ(szTitle);
		m_pOKBtn->SetTextWZ(szTitle);

		m_pMainText->SetTextWZ(szFinal);

#ifdef _INTERNATIONAL_UI
		m_pMainText->ShowWindowWZ(WZ_HIDE);
#endif//_INTERNATIONAL_UI

		//uiBase::MoveCenter();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Guild_Relation_ask;
		msg.wParam = InterfaceManager::DIALOG_GUILD_RELATION_ASK;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
		m_bShow = TRUE;
	}
	else
	{
		if(m_pMainEdit)
			m_pMainEdit->SetTextWZ("");

		g_KeyQueueManager.DeleteMsg(eDoType_Guild_Relation_ask);
		m_bShow = FALSE;
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void uiGuildRelationAsk::AddMainTextInfo()
{
	TCHAR szFinal[INTERFACE_STRING_LENGTH+1] = {0,};
	
	//1.Static에 담겨진 내용을 얻어온다.
	m_pMainText->GetTextWZA( szFinal, INTERFACE_STRING_LENGTH );
	int nLen = _tcslen(szFinal);

	const int nOffset = 3;
	DWORD dwFont = m_pMainText->GetFontID();
	int nHeight = GlobalFunc::GetTextHeight(_T("A"),dwFont);
	
	//2.이 스트링을 줄넘어가는지 체크후 랜더한다.(개행처리를위해)
	std::vector<util::_tstring> vstr;
	GlobalFunc::ProcessWordWrapByFontSize(&vstr, szFinal, dwFont, nLen, m_pMainText->GetSize().right-nOffset);
	
	RECT rt = m_pMainText->GetSizeRect();
	
	this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(dwFont);
	for(size_t i=0; i<vstr.size();++i)
	{
		RECT rcResult;
		SetRect(&rcResult,rt.left + 2,nOffset + rt.top + (nHeight*i),rt.right,nOffset + rt.top + (nHeight*i) + nHeight + 4);
		TCHAR *pStr = const_cast<TCHAR *>(vstr[i].c_str());
		if (pStr) 
		{
			this->GetDrawBase()->x_pManagerTextOut->DrawText(pStr,&rcResult,WzColor_RGBA(255,255,255,255),WzColor_RGBA(0,0,0,0),TP_HLEFT);
		}
	}
	this->GetDrawBase()->x_pManagerTextOut->EndUseFont();

}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildRelationAsk::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_GUILD_RELATION_ASK_EXIT_BTN:
				{
					m_pMainEdit->ClearText();
					OnShowWindow(FALSE);
				}
				break;

			case eDIALOG_GUILD_RELATION_ASK_OK_BTN:
				{
					TCHAR szGuildName[MAX_GUILDNAME_LENGTH+1] = {0,};
					m_pMainEdit->GetTextWZ(szGuildName, MAX_GUILDNAME_LENGTH);
					if(0 == strlen(szGuildName))
						break;

					if(g_pHero)
					{
						if(NULL == Strcmp(g_pHero->GetGuildName(), szGuildName))
						{
							TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
							//7217	자신의 길드와는 어떤 관계도 맺을 수 없습니다.
							g_InterfaceManager.GetInterfaceString(7217, szMessage);
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
							return;
						}
					}

					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
					//6176 %s 길드 마스터에게 %s관계 신청 메시지를 정상적으로 발송하였습니다.
					g_InterfaceManager.GetInterfaceString(6176, szMessage);

					TCHAR szRelation[INTERFACE_STRING_LENGTH + 1] = {0,};
					TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};

					if(m_bRelationState)//관계 신청
					{
						if((eGUILD_RELATION_NONE == m_btRelation) || (eGUILD_RELATION_ONE_SIDE_HOSTILITY < m_btRelation))
							break;

						//////////////////////////////////////////////////////////////////////////
						BOOL bAlready = FALSE;
						std::list<GUILD_RELATION*>& rAllianceCon		 = this->GetManager()->GetAllianceCon();
						std::list<GUILD_RELATION*>& rHostilityCon		 = this->GetManager()->GetHostilityCon();
						std::list<GUILD_RELATION*>& rOneSideHostilityCon = GetManager()->GetOneSideHostilityCon();
						std::list<GUILD_RELATION*>::iterator iter;
						for(iter = rAllianceCon.begin(); iter != rAllianceCon.end(); ++iter)
						{
							GUILD_RELATION* pGuildRelation = (*iter);
							if(NULL == Strcmp(szGuildName, pGuildRelation->m_GuildName))
							{
								bAlready = TRUE;
								break;
							}
						}

						for(iter = rHostilityCon.begin(); iter != rHostilityCon.end(); ++iter)
						{
							GUILD_RELATION* pGuildRelation = (*iter);
							if(NULL == Strcmp(szGuildName, pGuildRelation->m_GuildName))
							{
								bAlready = TRUE;
								break;
							}
						}

						for(iter = rOneSideHostilityCon.begin(); iter != rOneSideHostilityCon.end(); ++iter)
						{
							GUILD_RELATION* pGuildRelation = (*iter);
							if(NULL == Strcmp(szGuildName, pGuildRelation->m_GuildName))
							{
								if(eGUILD_RELATION_ONE_SIDE_HOSTILITY != m_btRelation)
								{
									bAlready = TRUE;
									break;
								}
							}
						}

						if(TRUE == bAlready) //이미 관계가 맺어진 길드다
						{
							//7180	길드 관계를 맺으려는 길드와 이미 다른 관계가 성립되어 있습니다.
							TCHAR szAlready[INTERFACE_STRING_LENGTH + 1] = {0,};
							g_InterfaceManager.GetInterfaceString(7180, szAlready);
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szAlready);

							return;
						}
						//////////////////////////////////////////////////////////////////////////

						//동맹/적대 신청.
						if(eGUILD_RELATION_ALLIENCE == m_btRelation || eGUILD_RELATION_PROCLAIM_HOSTILITY == m_btRelation)
						{
							MSG_CG_GUILD_RELATION_SYN SendPacket;
							SendPacket.m_eType = (eGUILD_RELATION)m_btRelation;
							StrnCopy(SendPacket.m_szRelationGuildName, szGuildName, MAX_GUILDNAME_LENGTH);

							BOOL bVal = GlobalFunc::SendPacket(CI_GAMESERVERIDX , &SendPacket , sizeof(SendPacket) );
							if(bVal)
							{
								if(eGUILD_RELATION_ALLIENCE == m_btRelation)
								{
									//동맹
									g_InterfaceManager.GetInterfaceString(1170, szRelation);

								}
								else
								{
									//적대
									g_InterfaceManager.GetInterfaceString(1171, szRelation);
								}
							}
						}
						else if(eGUILD_RELATION_ONE_SIDE_HOSTILITY == m_btRelation) //쌍방적대 신청
						{
							GUILDGUID GuildID = -1;
							if(this->GetManager())
							{
								std::list<GUILD_RELATION*>& rOneSideHostilityCon = this->GetManager()->GetOneSideHostilityCon();
								if(rOneSideHostilityCon.empty())
									return;

								std::list<GUILD_RELATION*>::iterator iter;
								for(iter = rOneSideHostilityCon.begin(); iter != rOneSideHostilityCon.end(); iter++)
								{
									GUILD_RELATION* pTemp = (*iter);

									if(0 == strcmp(pTemp->m_GuildName, szGuildName))
									{
										GuildID = pTemp->m_GuildGuid;
									}
								}
							}

							if(-1 == GuildID)
							{
								TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
								//6233	일방적 적대관계가 아닙니다.
								g_InterfaceManager.GetInterfaceString(6233, szTemp);
								GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szTemp);
							}
							else
							{
								MSG_CG_GUILD_RELATION_BOTH_AGAINST_SYN SendPacket;
								SendPacket.m_BothAgainstGuildGuid = GuildID;

								BOOL bVal = GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
								if(bVal)
								{
									//1208	일방적 적대관계
									g_InterfaceManager.GetInterfaceString(1208, szRelation);
								}
							}
						}

						if(eGUILD_RELATION_ALLIENCE == m_btRelation)//동맹관계일때만 뿌려주면 되겠다.
						{
							Snprintf(szFinal, INTERFACE_STRING_LENGTH, szMessage, szGuildName, szRelation);
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szFinal);
						}

						m_pMainEdit->ClearText();
						OnShowWindow(FALSE);
					}
					else //관계 해제
					{
						//적대관계일 경우 쌍방적대와 적대선포 상황을 구분
						GUILDGUID GuildID = -1;
						if(eGUILD_RELATION_PROCLAIM_HOSTILITY == (eGUILD_RELATION)m_btRelation)
						{
							GuildID = -1;

							if(this->GetManager())
							{
								std::list<GUILD_RELATION*>& rHostilityCon = this->GetManager()->GetHostilityCon();

								std::list<GUILD_RELATION*>::iterator iter;
								for(iter = rHostilityCon.begin(); iter != rHostilityCon.end(); iter++)
								{
									GUILD_RELATION* pTemp = (*iter);

									if(0 == strcmp(pTemp->m_GuildName, szGuildName))
									{
										 SetbtRelation(pTemp->m_eRelationType);
										 GuildID = pTemp->m_GuildGuid;
									}
								}
							}
						}
						else if(eGUILD_RELATION_ALLIENCE == (eGUILD_RELATION)m_btRelation)
						{
							GuildID = -1;

							if(this->GetManager())
							{
								std::list<GUILD_RELATION*>& rAllianceCon = this->GetManager()->GetAllianceCon();

								std::list<GUILD_RELATION*>::iterator iter;
								for(iter = rAllianceCon.begin(); iter != rAllianceCon.end(); iter++)
								{
									GUILD_RELATION* pTemp = (*iter);

									if(0 == strcmp(pTemp->m_GuildName, szGuildName))
									{
										GuildID = pTemp->m_GuildGuid;
									}
								}
							}
						}

						if(-1 == GuildID)
						{
							TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
							
							if(eGUILD_RELATION_ALLIENCE == (eGUILD_RELATION)m_btRelation)
							{
								//6173	동맹관계가 아닙니다.
								g_InterfaceManager.GetInterfaceString(6173, szTemp);
							}
							else
							{
								//6232	적대관계가 아닙니다.
								g_InterfaceManager.GetInterfaceString(6232, szTemp);
							}
							
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szTemp);
						}
						else
						{
							MSG_CG_GUILD_RELATION_DELETE_SYN SendPacket;
							SendPacket.m_eType = (eGUILD_RELATION)m_btRelation;
							SendPacket.m_eDeleteGuildGuid = GuildID;

							GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
						}

						m_pMainEdit->ClearText();
						OnShowWindow(FALSE);
					}
				}
				break;

			case eDIALOG_GUILD_RELATION_ASK_CANCEL_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;

			case eDIALOG_GUILD_RELATION_ASK_TEXT:
				{
				}
				break;

			case eDIALOG_GUILD_RELATION_ASK_EDIT:
				{
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

void	uiGuildRelationAsk::InitControl()
{
	m_pTitle			   = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_GUILD_RELATION_ASK_TITLE));
	assert(m_pTitle);
	m_pExitBtn			   = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_RELATION_ASK_EXIT_BTN));
	assert(m_pExitBtn);
	m_pOKBtn			   = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_RELATION_ASK_OK_BTN));
	assert(m_pOKBtn);
	m_pCancelBtn		   = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_RELATION_ASK_CANCEL_BTN));
	assert(m_pCancelBtn);

	m_pMainEdit			   = static_cast<CCtrlDxIMEEditBox*>(getControl(eDIALOG_GUILD_RELATION_ASK_EDIT));
	assert(m_pMainEdit);
	if(m_pMainEdit)
	{
		m_pMainEdit->SetFontID(StrToWzID("st10"));
		m_pMainEdit->SetColorText(RGBA(255,255,255,255));
	}

	m_pMainText			   = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_GUILD_RELATION_ASK_TEXT));
	assert(m_pMainText);


}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
