#include "SunClientPrecompiledHeader.h"

#include "uiCommunityReadMail.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "DominationInfoParser.h"
#include "MapInfoParser.h"
#include "interfacemanager.h"
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
#include "Application.h"
#include "RankingDefine.h"
#include "RankingManager.h"
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

void _CallBack_Mail_Delete_in_ReadDlg(bool bYes)
{
	if( uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY)) )
    {
        pCommMan->OnCallBackMailDeleteInReadDialog( bYes );
    }
}

//------------------------------------------------------------------------------
WzID uiCommunityReadMail::m_wzId[COMMUNITY_READ_MAIL_DIALOG__MAX] = 
{
        StrToWzID("BT00"),	//COMMUNITY_READ_MAIL_BTN_EXIT,	

        StrToWzID("BT01"),	//COMMUNITY_READ_MAIL_BTN_REPLY,
		StrToWzID("BT02"),	//COMMUNITY_READ_MAIL_BTN_DEL,	
		StrToWzID("BT03"),	//COMMUNITY_READ_MAIL_BTN_CLOSE,

		StrToWzID("ST01"),	//COMMUNITY_READ_MAIL_TXT_FROM,	
		StrToWzID("ST02"),	//COMMUNITY_READ_MAIL_TXT_DATE,	

		StrToWzID("LS00"),	//COMMUNITY_READ_MAIL_LST,
		StrToWzID("VS00"),	//COMMUNITY_READ_MAIL_VSCROLL,
};

//------------------------------------------------------------------------------
/**
*/
uiCommunityReadMail::uiCommunityReadMail(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pCommunityMan =
        static_cast<uiCommunityMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_COMMUNITY));
    assert (m_pCommunityMan);

    if (!this->m_pCommunityMan)
    {
#ifdef _DEBUG
		 uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(_T("m_pCommunityMan이 없습니다."));
#endif
        this->ShowInterface(FALSE);
        return;
    }

	m_bShow = false;
	
	ZeroMemory(&m_subject, sizeof(m_subject));
	ZeroMemory(m_ptszMemo, sizeof(m_ptszMemo));
}

//------------------------------------------------------------------------------
/**
*/
uiCommunityReadMail::~uiCommunityReadMail()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityReadMail::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityReadMail::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityReadMail::InitControls()
{
	m_pListCtrlRead = static_cast<CCtrlListWZ *>(this->getControl(COMMUNITY_READ_MAIL_LST));
    assert (m_pListCtrlRead);
	m_pListCtrlRead->SetUnuseSelBar(false);

    for (int i=0; i<READ_MAIL_TXT_MAX; ++i)
    {
        this->m_pTxtCtrls[i] =
            static_cast<CCtrlStaticWZ *>(this->getControl(COMMUNITY_READ_MAIL_TXT_FROM + i));
        assert (this->m_pTxtCtrls[i]);
    }

	m_pScroll = static_cast<CCtrlVScrollWZ *>(getControl(COMMUNITY_READ_MAIL_VSCROLL));
	assert(m_pScroll);

	m_pListCtrlRead->SetUseReport(false);
	m_pListCtrlRead->SetSellHeight(22);
	m_pListCtrlRead->SetFontID(c_wiCommunityFont);
	m_pListCtrlRead->SetColorSelBar(RGBA(255,255,255,0));

	RECT rc = m_pListCtrlRead->GetSizeRect();
    m_LinePerPage = (int)((rc.bottom - rc.top) / 22 + 1);

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiCommunityReadMail::OnShowWindow( BOOL val )
{
    if (val)
    {
        this->MoveDefaultWindowPos();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
		if(m_pScroll)
			m_pScroll->SetScrollPosWZ(0);

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_community_read_mail;
        msg.wParam = InterfaceManager::DIALOG_COMMUNITY_READ_MAIL;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);

		m_bShow = true;

		_displaySetting();

	}
    else
    {
		m_bShow = false;
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_community_read_mail);
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityReadMail::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if(g_pHero && g_pHero->IsShowFullMap())
			return FALSE;

		if(!GetManager())
			return FALSE;

		return TRUE;
	}
	else
	{
		return TRUE;
	}

}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityReadMail::CloseAnswerBtn(BOOL bClose)
{
	CCtrlButtonWZ *pBtn = static_cast<CCtrlButtonWZ *>(this->getControl(COMMUNITY_READ_MAIL_BTN_REPLY));
	
	if(pBtn)
	{
		pBtn->ShowWindowWZ( bClose ? WZ_HIDE:WZ_SHOW);
	}
}
//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityReadMail::SetMailData(MAIL_DATA* pData)
{
	BOOL bSystemMail = FALSE;

	ZeroMemory(&m_subject, sizeof(m_subject));
	ZeroMemory(m_ptszMemo, sizeof(m_ptszMemo));

	CopyMemory(&m_subject, &(pData->m_subject), sizeof(STRUCT_SUBJECT_PACKETINFO));

	if(pData->m_subject.m_MemoType==STRUCT_SUBJECT_PACKETINFO::SYSTEM_MEMO)
	{
		char *pStr= pData->m_subject.m_ptszSubject;

		int nSystemType =(int)GetManager()->ParseMemoInfo(pStr,0,MAX_SUBJECT_STRLENGTH);
		
		pStr = pData->m_ptszMemo;

		bSystemMail = TRUE;
		
		TCHAR szName[INTERFACE_STRING_LENGTH]={0,};
		TCHAR szTemp[INTERFACE_STRING_LENGTH]={0,};
		TCHAR szMessage[INTERFACE_STRING_LENGTH]={0,};

		switch(nSystemType)
		{
		case STRUCT_SUBJECT_PACKETINFO::LOTTO_TYPE://로또타입
        case STRUCT_SUBJECT_PACKETINFO::LOTTO_TYPE_NEW:
			{
				int		nDrawNum =(int)GetManager()->ParseMemoInfo(pStr,2,MAX_MEMO_STRLENGTH-1);
				int		nRank	 =(int)GetManager()->ParseMemoInfo(pStr,3,MAX_MEMO_STRLENGTH-1);
				INT64	nHeim	 =	   GetManager()->ParseMemoInfo(pStr,4,MAX_MEMO_STRLENGTH-1);
				//5294	축하드립니다! %s 님께서 %d회 하임 로또 %d등에 당첨되셨습니다.\n당첨금으로 %d 하임이 지급 되었습니다.
				TCHAR szTemp[INTERFACE_STRING_LENGTH]={0,};
				TCHAR szMessage[INTERFACE_STRING_LENGTH]={0,};
				g_InterfaceManager.GetInterfaceString( 5294,szTemp);
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, g_pHero->GetName(),nDrawNum,nRank,nHeim);
				StrnCopy(m_ptszMemo, szMessage, MAX_MEMO_STRLENGTH-1);
			}
			break;

		case STRUCT_SUBJECT_PACKETINFO::GROUP_TYPE://그룹멤버타입
			{
				INT64 nExp = GetManager()->ParseMemoInfo(pStr,1,MAX_MEMO_STRLENGTH-1);
				TCHAR szTemp[INTERFACE_STRING_LENGTH]={0,};
				TCHAR szMessage[INTERFACE_STRING_LENGTH]={0,};

				if(nExp >= MAX_GROUP_EXP)
				{
					//  그룹 멤버로부터 최대 보너스 경험치 5,000,000을 보상 받았습니다.
					g_InterfaceManager.GetInterfaceString(7042,szTemp);	
				}
				else
				{
					//7036:그룹 멤버로부터 %d 경험치를 보상 받았습니다.	
					g_InterfaceManager.GetInterfaceString(7036,szTemp);
				}
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, nExp);
				StrnCopy(m_ptszMemo, szMessage, MAX_MEMO_STRLENGTH-1);
			}
			break;

		case STRUCT_SUBJECT_PACKETINFO::FRINVATE_START_TYPE://친구초대한 플레이어가 캐릭터를 만들어서 접속
		case STRUCT_SUBJECT_PACKETINFO::FRBEINVATED_START_TYPE://초대된 플레이어가 캐릭터를 만들어서 접속
			{
				GetManager()->ParseMemoInfo(pStr,1,MAX_MEMO_STRLENGTH-1, INTERFACE_STRING_LENGTH, szName);
				//7828 %s님이 친구로 등록되었습니다. 아인으로부터 자세한 설명을 들으시기 바랍니다.
				g_InterfaceManager.GetInterfaceString(6076,szTemp);
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szName);
				StrnCopy(m_ptszMemo, szMessage, MAX_MEMO_STRLENGTH-1);
			}
			break;
		case STRUCT_SUBJECT_PACKETINFO::FRINVATE_END_TYPE://친구초대한 캐릭터가 80레벨이 되어 만료
		case STRUCT_SUBJECT_PACKETINFO::FRBEINVATED_END_TYPE://초대된 플레이어가 80레벨이 되어 만료
			{
				GetManager()->ParseMemoInfo(pStr,1,MAX_MEMO_STRLENGTH-1, INTERFACE_STRING_LENGTH, szName);
				//7830 %s님의 레벨이 80이 되었습니다. 이벤트가 종료됩니다. 아인으로부터 마지막 선물상자를 받아가시기 바랍니다.
				g_InterfaceManager.GetInterfaceString(6078,szTemp);
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szName);
				StrnCopy(m_ptszMemo, szMessage, MAX_MEMO_STRLENGTH-1);
			}
			break;
		case STRUCT_SUBJECT_PACKETINFO::FRINVATE_PRESENT_TYPE://친구초대한 캐릭터가 20/40/60레벨이 되었을 때
		case STRUCT_SUBJECT_PACKETINFO::FRBEINVATED_PRESENT_TYPE://초대된 캐릭터가 20/40/60레벨이 되었을 때
			{
				GetManager()->ParseMemoInfo(pStr,1,MAX_MEMO_STRLENGTH-1, INTERFACE_STRING_LENGTH, szName);
				int iLv = (int)GetManager()->ParseMemoInfo(pStr,2,MAX_MEMO_STRLENGTH-1, INTERFACE_STRING_LENGTH);
				//7829 %s님의 레벨이 %d이 되었습니다. 아인으로부터 선물상자를 받아가시기 바랍니다.
				g_InterfaceManager.GetInterfaceString(6077,szTemp);
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szName, iLv);
				StrnCopy(m_ptszMemo, szMessage, MAX_MEMO_STRLENGTH-1);
			}
			break;

		case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_RETURN:
        case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_RETURN_NEW:
			{
				TCHAR szMessage[INTERFACE_STRING_LENGTH]={0,};
				TCHAR szTemp[INTERFACE_STRING_LENGTH]={0,};
				TCHAR szGuildName[INTERFACE_STRING_LENGTH]={0,};
				TCHAR szMapName[INTERFACE_STRING_LENGTH]={0,};
				TCHAR szHeim[INTERFACE_STRING_LENGTH]={0,};

				MONEY Heim = GetManager()->ParseMemoInfo(pStr,2,MAX_MEMO_STRLENGTH-1);  //- 1.번 하임 -> 2
				GetManager()->ParseMemoInfo(pStr,3,MAX_MEMO_STRLENGTH-1, INTERFACE_STRING_LENGTH, szGuildName);
				MAPCODE MapCode = (MAPCODE)GetManager()->ParseMemoInfo(pStr,1,MAX_MEMO_STRLENGTH-1); // 2번. 맵코드 -> 1
				
				const sMAPINFO* pFieldInfo = MapInfoParser::Instance()->FindMapInfo(MapCode);
				if (pFieldInfo)
				{
					g_InterfaceManager.GetInterfaceString(pFieldInfo->dwNCode, szMapName, INTERFACE_STRING_LENGTH);
				}

                // 왕의증표 및 하임 돌려받는 메시지
                g_InterfaceManager.GetInterfaceString(3393,szTemp);
                Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szMapName);
                StrnCopy(m_ptszMemo, szMessage, MAX_MEMO_STRLENGTH-1);
			}
			break;
		case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_REWARD:
        case STRUCT_SUBJECT_PACKETINFO::DOMINATION_TYPE_REWARD_NEW:
			{
				TCHAR szMessage[INTERFACE_STRING_LENGTH]={0,};
				TCHAR szTemp[INTERFACE_STRING_LENGTH]={0,};
				TCHAR szGuildName[INTERFACE_STRING_LENGTH]={0,};
				TCHAR szMapName[INTERFACE_STRING_LENGTH]={0,};
                TCHAR szHeim[INTERFACE_STRING_LENGTH]={0,};
				int nHeimRate = 50;

				MONEY Heim = GetManager()->ParseMemoInfo(pStr,2,MAX_MEMO_STRLENGTH-1);
				GetManager()->ParseMemoInfo(pStr,3,MAX_MEMO_STRLENGTH-1, INTERFACE_STRING_LENGTH, szGuildName);
				MAPCODE MapCode = (MAPCODE)GetManager()->ParseMemoInfo(pStr,1,MAX_MEMO_STRLENGTH-1);

				const sMAPINFO* pFieldInfo = MapInfoParser::Instance()->FindMapInfo(MapCode);
				if (pFieldInfo)
				{
					g_InterfaceManager.GetInterfaceString(pFieldInfo->dwNCode, szMapName, INTERFACE_STRING_LENGTH);
				}
				
				const sDOMINATION_INFO* pDI = DominationInfoParser::Instance()->GetDominationInfo(MapCode);
				if (pDI)
				{
					nHeimRate = pDI->m_BattingCharge;
				}

                NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
                Sprintf(szTemp, _T("%I64u"), Heim);
                ::GetNumberFormat(LOCALE_SYSTEM_DEFAULT, NULL, szTemp, &nFmt, szHeim, sizeof(szHeim));

				// 50% 하임 보상받는 메시지
				g_InterfaceManager.GetInterfaceString(3375,szTemp);
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szMapName, szGuildName, szHeim);
				StrnCopy(m_ptszMemo, szMessage, MAX_MEMO_STRLENGTH-1);
			}
			break;
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
            case STRUCT_SUBJECT_PACKETINFO::WORLDRANKING_TYPE_REWARD_ALARAM: // 월드 랭킹 보상 알림
                {
                    int		ranking_id       = static_cast<int>(GetManager()->ParseMemoInfo(pStr,1,MAX_MEMO_STRLENGTH-1));
                    int     reward_ranking	 = static_cast<int>(GetManager()->ParseMemoInfo(pStr,2,MAX_MEMO_STRLENGTH-1));
                    TCHAR   mission_name[INTERFACE_STRING_LENGTH] = {0, };

                    const RankingManager::MISSION_LEVEL_AREA_HASH& area_hash = GET_MISSION_RANK_MANAGER()->GetMissionLevelAreaTable();
                    RankingManager::MISSION_LEVEL_AREA_HASH::const_iterator iter = area_hash.begin();

                    for ( ; iter != area_hash.end(); ++iter )
                    {
                        RankingManager::LEVEL_AREA_BASE* area_ptr = 
                            const_cast<RankingManager::LEVEL_AREA_BASE*>(iter->second);

                        if ( area_ptr )
                        {
                            if (area_ptr->m_Id == ranking_id)
                            {
                                g_InterfaceManager.GetInterfaceString( area_ptr->m_MissionString,
                                    mission_name,
                                    INTERFACE_STRING_LENGTH );
                                break;
                            }
                        }
                    }                    
                    
                    // %s 월드 랭킹 %s위를 달성 했습니다. 월드 랭킹 화면에서 보상을 수령하세요.
                    TCHAR temp_string[INTERFACE_STRING_LENGTH]={0,};
                    TCHAR message_string[INTERFACE_STRING_LENGTH]={0,};
                    TCHAR reward_ranking_string[INTERFACE_STRING_LENGTH] = {0, };

                    g_InterfaceManager.GetInterfaceString( 2788,temp_string );
                    _itoa(reward_ranking, reward_ranking_string, 10);
                    Snprintf(message_string, INTERFACE_STRING_LENGTH-1, 
                        temp_string, mission_name, reward_ranking_string);
                    StrnCopy(m_ptszMemo, message_string, MAX_MEMO_STRLENGTH-1);
                }
                break;
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            case STRUCT_SUBJECT_PACKETINFO::GUILD_JOIN_REQUEST:
                {
                    GetManager()->ParseMemoInfo(pStr,1,MAX_MEMO_STRLENGTH-1, INTERFACE_STRING_LENGTH, szName);

                    TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
                    // 7299 : %s님이 길드가입을 희망합니다.\n%s님을 길드원으로 맞이하려면 지금 바로 길드에 초대하세요.
                    TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
                    g_InterfaceManager.GetInterfaceString(7299, format_string);
                    Snprintf(result_message, INTERFACE_STRING_LENGTH-1, format_string, szName, szName);
                    StrnCopy(m_ptszMemo, result_message, MAX_MEMO_STRLENGTH-1);
                }
                break;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
			case STRUCT_SUBJECT_PACKETINFO::SUN_RANKING_SETTLE_RESULT_TYPE:
				{
					eSUN_RANKING_TYPE ranking_type = static_cast<eSUN_RANKING_TYPE>(GetManager()->ParseMemoInfo(pStr,1,MAX_MEMO_STRLENGTH-1, INTERFACE_STRING_LENGTH));
					int my_ranking = static_cast<int>(GetManager()->ParseMemoInfo(pStr,2,MAX_MEMO_STRLENGTH-1, INTERFACE_STRING_LENGTH));

					if ( g_pHero )
					{
						_tcsncpy(szName, g_pHero->GetName(), MAX_CHARNAME_LENGTH-1);
					}

					TCHAR my_ranking_string[INTERFACE_STRING_LENGTH] = {0, };
					_itoa(my_ranking, my_ranking_string, 10);

					TCHAR ranking_type_string[INTERFACE_STRING_LENGTH] = {0, };

					int ranking_type_string_index = 0;
					
					switch ( ranking_type )
					{
					case eSUN_RANKING_TYPE_BEST_OF_BEST:
						{
							// 3602 : Best of Best
							ranking_type_string_index = 3602;
						}
						break;
					case eSUN_RANKING_TYPE_ELETE_WARRIOR: // 엘리트 전사
						{
							// 3603 : 엘리트 전사
							ranking_type_string_index = 3603;
						}
						break;
					case eSUN_RANKING_TYPE_LEVEL_KING:      // 레벨왕
						{
							// 3604 : 레벨왕
							ranking_type_string_index = 3604;
						}
						break;
					case eSUN_RANKING_TYPE_LEGEND_EQUIP:    // 전설적인 장비
						{
							// 3605 : 전설적인 장비
							ranking_type_string_index = 3605;
						}
						break;
					case eSUN_RANKING_TYPE_STRONG_ONE_SHOT: // 강력한 한방
						{
							// 3606 : 강력한 한방
							ranking_type_string_index = 3606;
						}
						break;
					case eSUN_RANKING_TYPE_BEST_ATTACKER:   // 최고의 공격수
						{
							// 3607 : 최고의 공격수
							ranking_type_string_index = 3607;
						}
						break;
					case eSUN_RANKING_TYPE_MASTER_OF_HEAL:  // 치유의 달인
						{
							// 3608 : 치유의 달인
							ranking_type_string_index = 3608;
						}
						break;
					}

					if ( ranking_type_string_index != 0 )
					{
						g_InterfaceManager.GetInterfaceString(ranking_type_string_index, ranking_type_string);
					}

					TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
					// 3624 : %s님은 지난 시즌 SUN랭킹 %s에서 %s위로 선정되셨습니다. 해당하는 보상마크가 주어집니다.
					TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
					g_InterfaceManager.GetInterfaceString(3624, format_string);
					Snprintf(result_message, INTERFACE_STRING_LENGTH-1, format_string, szName, ranking_type_string, my_ranking_string);
					StrnCopy(m_ptszMemo, result_message, MAX_MEMO_STRLENGTH-1);
				}
				break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
		default:
			break;
		}
	}
	else
	{
		StrnCopy(m_ptszMemo, pData->m_ptszMemo, MAX_MEMO_STRLENGTH-1);
	}

	return bSystemMail;
}


//------------------------------------------------------------------------------
/**
*/
void uiCommunityReadMail::_displaySetting()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

	TCHAR szSender[INTERFACE_STRING_LENGTH] ={0,};

	if(m_pTxtCtrls[READ_MAIL_TXT_FROM])
	{
		ZeroMemory(szMessage, sizeof(szMessage));
		StrnCopy(szMessage, m_subject.m_SendName, MAX_CHARNAME_LENGTH);
		//5786	보낸사람: %s
		g_InterfaceManager.GetInterfaceString( 5786 , szSender ,INTERFACE_STRING_LENGTH);
		Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, szSender, szMessage);
		
		m_pTxtCtrls[READ_MAIL_TXT_FROM]->SetTextWZ(szTemp);
		
	}

	if(m_pTxtCtrls[READ_MAIL_TXT_DATE])
	{
		ZeroMemory(szMessage, sizeof(szMessage));
		StrnCopy(szMessage, m_subject.m_tszCreateDate, MAX_DATE_SIZE);
		m_pTxtCtrls[READ_MAIL_TXT_DATE]->SetTextWZ(szMessage);
	}

	if(m_pListCtrlRead)
	{
		m_pListCtrlRead->DeleteItemAll();
		RECT rc = m_pListCtrlRead->GetSizeRect();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
		int iListWidth = rc.right - rc.left - 20;
#else
		int iListWidth = rc.right - rc.left - 10;
#endif	

		m_vstr.clear();
		GlobalFunc::ProcessWordWrapByFontSize( &m_vstr, const_cast<TCHAR *>(m_ptszMemo), c_wiCommunityFont, _tcslen(m_ptszMemo), iListWidth );
		
		int index = 0;

		for( unsigned int i =0; i < m_vstr.size(); ++i)
		{
			if ( 0 != m_vstr[i].length() )			
			{
				index = m_pListCtrlRead->InsertItemBack();

				GlobalFunc::SetItem(m_pListCtrlRead,index,0,const_cast<TCHAR *>(m_vstr[i].c_str()),c_wiCommunityFont, 
					DT_LEFT | DT_VCENTER,
					RGBA(255,255,255,0), 
					RGBA(255,255,255,255), 
					RGBA(255,255,255,255));
			}
		}

		m_pScroll->SetScrollPosWZ(0);
		int iPage = (index/m_LinePerPage);
		if(m_pScroll)
		{
			if (iPage < 1)
			{
				m_pScroll->ShowWindowWZ(WZ_HIDE);
			}
			else
			{
				m_pScroll->ShowWindowWZ(WZ_SHOW);
				m_pScroll->SetScrollRangeWZ(0,iPage);	//스크롤 범위
			}						
		}
		
		this->_refreshList();

	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityReadMail::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch( pMessage->eResultMsg ) 
    {
	case RT_MSG_SB_PAGETOP:			//. 윗쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINETOP:			//.윗쪽 화살표를 눌렀다.
		{
			if(m_pScroll)
			{
				UpScroll(1);
			}
		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:				//. 아래쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINEBOTTOM:				//.아랫쪽 화살표를 눌렀다
		{
			if(m_pScroll)
			{
				DownScroll(1);
			}
		}
		break;

	case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
	case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
		{
			if(m_pScroll)
			{
				int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				m_pScroll->SetScrollPosWZ(pos);

				_refreshList();
			}
		}
		break;


    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case COMMUNITY_READ_MAIL_BTN_EXIT:
			case COMMUNITY_READ_MAIL_BTN_CLOSE:
                {
                    CloseDlg();
                }
                break;

			case COMMUNITY_READ_MAIL_BTN_REPLY:
				{
					GetManager()->ShowWriteMailDlg(m_subject.m_SendName);

					CloseDlg();
				}
				break;

			case COMMUNITY_READ_MAIL_BTN_DEL:
				{
					if(GetManager()->IsShowSystemDialog())
						break;

					if(m_subject.m_MemoGuid)
					{
						GetManager()->SetPendingDelMemoGuid(m_subject.m_MemoGuid);

						// 5784	쪽지를 삭제하시겠습니까?
						g_InterfaceManager.GetInterfaceString( 5784 , szMessage ,INTERFACE_STRING_LENGTH);


						uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
						if(pSystemMan)
							pSystemMan->Sys_Confirm(szMessage,_CallBack_Mail_Delete_in_ReadDlg);						

						GetManager()->SetSystemDialog(true);
					}

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
/**	스크롤 다운
*/
void uiCommunityReadMail::DownScroll(int nRange)
{

	int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
	int mmin = 0 , mmax = 0;
	m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

	if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
	{

		( (pos+nRange)>mmax ? (pos=mmax):(pos+=nRange));
		m_pScroll->SetScrollPosWZ(pos);
		_refreshList();
	}
}
//----------------------------------------------------------------------------
/** 스크롤 업
*/
void uiCommunityReadMail::UpScroll(int nRange)
{
	int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
	int mmin = 0 , mmax = 0;
	m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

	if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
	{
		( (pos-nRange)<mmin ? (pos=mmin):(pos-=nRange));

		m_pScroll->SetScrollPosWZ(pos);
		_refreshList();
	}
}
//----------------------------------------------------------------------------
/** 업데이트 스크롤
*/
void uiCommunityReadMail::UpdateMouseWheel()
{
	//1.영역체크는 다이얼로그 전체사이즈로 하겠씀
	if(!m_pListCtrlRead) return;

	RECT rc = m_pListCtrlRead->GetSizeRect();

	switch(GetMouseWheelStatus(&rc))
	{
	case MOUSE_WHEEL_UP: UpScroll(1);break;

	case MOUSE_WHEEL_DOWN:DownScroll(1);break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCommunityReadMail::CloseDlg()
{
	m_bShow = false;

	ZeroMemory(&m_subject, sizeof(m_subject));
	ZeroMemory(m_ptszMemo, sizeof(m_ptszMemo));

}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityReadMail::_refreshList()
{
	if(m_pListCtrlRead && m_pScroll)
	{
		int iCurScrollPos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다
		int iStartDrawIndex = (m_LinePerPage * iCurScrollPos);

		m_pListCtrlRead->SetBeginDrawIndex( iStartDrawIndex );
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityReadMail::OnUpdateSolarDialog()
{
	if(!m_bShow)
	{
		this->ShowInterface(FALSE);
		return;
	}
	
    if (!_isEnable())
        return;

	UpdateMouseWheel();
}
