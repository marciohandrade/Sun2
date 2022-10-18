//------------------------------------------------------------------------------
//  uiBattleZoneCHuntingScoreBoard.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "ObjectManager.h"
#include "uiBattleZoneCHuntingScoreBoard.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "uiPartyMan/uiPartyMan.h"
#include "Hero.h"
#include "Character.h"
#include "interfacemanager.h"
#include "globalfunc.h"
//------------------------------------------------------------------------------
WzID uiBattleZoneCHuntingScoreBoard::m_wzId[DIALOG_MAX] = 
{ 
        StrToWzID("L001"),	//DLGPOS_LST_HOMETEAM_SCORE,
        StrToWzID("L002"),	//DLGPOS_LST_AWAYTEAM_SCORE,
        StrToWzID("T101"),	//DLGPOS_TXT_WIN_SCORE,
		StrToWzID("B067"),	//DLGPOS_BTN_EXIT,
};

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneMan*
uiBattleZoneCHuntingScoreBoard::GetManager()
{
    uiBattleZoneMan* battleZoneMan =
        static_cast<uiBattleZoneMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLEZONE));
    assert (battleZoneMan);
    return battleZoneMan;
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneCHuntingScoreBoard::uiBattleZoneCHuntingScoreBoard(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneCHuntingScoreBoard::~uiBattleZoneCHuntingScoreBoard()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCHuntingScoreBoard::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	m_iMemberCnt = 0;

	ZeroMemory(&m_cp, sizeof(m_cp));
    InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCHuntingScoreBoard::InitControls()
{
    m_pLstHomeTeamScoreBoard = (CCtrlListWZ *)getControl(DLGPOS_LST_HOMETEAM_SCORE);
    assert (m_pLstHomeTeamScoreBoard);
    m_pLstAwayTeamScoreBoard = (CCtrlListWZ *)getControl(DLGPOS_LST_AWAYTEAM_SCORE);
    assert (m_pLstAwayTeamScoreBoard);
    m_pTxtWinScore = (CCtrlStaticWZ *)getControl(DLGPOS_TXT_WIN_SCORE);
    assert (m_pTxtWinScore);

    this->m_bInitControls = true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCHuntingScoreBoard::Release()
{
}


//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiBattleZoneCHuntingScoreBoard::OnShowWindow( BOOL val )
{
    if (val)
    {
		if (!this->CheckControls())
            return;

        //UpdatePosition();

        SolarDialog::MoveCenter();
    }
    
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCHuntingScoreBoard::ShowCompetitionPoint(int iCount, CompetitionPoint* pCp, int iWin)
{
	m_iMemberCnt = iCount;

	// 우리팀 전적 리스트
	m_pLstHomeTeamScoreBoard->SetColorSelBar(RGBA(0,0,0,125));
    m_pLstHomeTeamScoreBoard->SetSellHeight(20);
    m_pLstHomeTeamScoreBoard->SetUseReport(true);
    m_pLstHomeTeamScoreBoard->SetReportUseCnt(4);
    m_pLstHomeTeamScoreBoard->SetHideReportRender(true);

    int iCnt = 0;
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};

	//	이름
    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("0"));
    GlobalFunc::SetReportButtonInfo( m_pLstHomeTeamScoreBoard, iCnt, 130, szMessage );

    //	Kill
    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("1"));
    GlobalFunc::SetReportButtonInfo( m_pLstHomeTeamScoreBoard, ++iCnt, 100, szMessage );

    //  Death
    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("2"));
    GlobalFunc::SetReportButtonInfo( m_pLstHomeTeamScoreBoard, ++iCnt,	100, szMessage );

    //	Combo
    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("3"));
    GlobalFunc::SetReportButtonInfo( m_pLstHomeTeamScoreBoard, ++iCnt,	160, szMessage );

    m_pLstHomeTeamScoreBoard->ShowWindowWZ(WZ_SHOW);

    m_pLstHomeTeamScoreBoard->DeleteItemAll();
    m_pLstHomeTeamScoreBoard->SetBeginDrawIndex(0);


	// 상대팀 전적 리스트
	m_pLstAwayTeamScoreBoard->SetColorSelBar(RGBA(0,0,0,125));
    m_pLstAwayTeamScoreBoard->SetSellHeight(20);
    m_pLstAwayTeamScoreBoard->SetUseReport(true);
    m_pLstAwayTeamScoreBoard->SetReportUseCnt(4);
    m_pLstAwayTeamScoreBoard->SetHideReportRender(true);

	iCnt = 0;

    //	이름
    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("0"));
    GlobalFunc::SetReportButtonInfo( m_pLstAwayTeamScoreBoard, iCnt, 130, szMessage );

    //	Kill
    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("1"));
    GlobalFunc::SetReportButtonInfo( m_pLstAwayTeamScoreBoard, ++iCnt, 100, szMessage );

    //  Death
    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("2"));
    GlobalFunc::SetReportButtonInfo( m_pLstAwayTeamScoreBoard, ++iCnt,	100, szMessage );

    //	Combo
    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("3"));
    GlobalFunc::SetReportButtonInfo( m_pLstAwayTeamScoreBoard, ++iCnt,	160, szMessage );

    m_pLstAwayTeamScoreBoard->ShowWindowWZ(WZ_SHOW);


    m_pLstAwayTeamScoreBoard->DeleteItemAll();
    m_pLstAwayTeamScoreBoard->SetBeginDrawIndex(0);

	TCHAR szWin[INTERFACE_STRING_LENGTH] ={0,};
	Snprintf(szWin, INTERFACE_STRING_LENGTH-1,  _T("%d Win"), iWin);
	GlobalFunc::SetCaption(m_pTxtWinScore, szWin);

	ZeroMemory(&m_cp, sizeof(m_cp));
	CopyMemory(&m_cp, pCp, sizeof(CompetitionPoint)*iCount);

	uiPartyMan* partyMan =
				static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

	int idxHome = 0;
	int idxAway = 0;

	if(partyMan && iCount && g_pHero)
	{
		for(int i=0; i<iCount; ++i)
		{
			bool bMyParty = false;
			
			if(g_pHero->GetObjectKey() == m_cp[i].m_dwObjKey)
				bMyParty = true;

			bMyParty = partyMan->IsPartyMember(m_cp[i].m_dwObjKey);

			if(bMyParty)
			{
				idxHome = m_pLstHomeTeamScoreBoard->InsertItemBack();

				Character *pChar = (Character *)g_ObjectManager.GetObject(m_cp[i].m_dwObjKey);
				if(pChar)
				{
					//	이름
					iCnt = 0;
					Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s"), pChar->GetName());
					GlobalFunc::SetItem(m_pLstHomeTeamScoreBoard, idxHome, iCnt,
										szMessage,
										StrToWzID("mn12"),
										DT_VCENTER | DT_CENTER,
										RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA(100, 100, 180, 255));

					//	Kill
					iCnt = 1;
					Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), m_cp[i].m_wKillPoint);
					GlobalFunc::SetItem(m_pLstHomeTeamScoreBoard, idxHome, iCnt,
										szMessage,
										StrToWzID("mn12"),
										DT_VCENTER | DT_CENTER,
										RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA(100, 100, 180, 255));

					//  Death
					iCnt = 2;
					Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), m_cp[i].m_byDieNum);
					GlobalFunc::SetItem(m_pLstHomeTeamScoreBoard, idxHome, iCnt,
										szMessage,
										StrToWzID("mn12"),
										DT_VCENTER | DT_CENTER,
										RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA(100, 100, 180, 255));

					//	Combo
					iCnt = 3;
					Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), m_cp[i].m_byCombo);
					GlobalFunc::SetItem(m_pLstHomeTeamScoreBoard, idxHome, iCnt,
										szMessage,
										StrToWzID("mn12"),
										DT_VCENTER | DT_CENTER,
										RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA(100, 100, 180, 255));
				}
			
			}
			else
			{
				idxAway = m_pLstAwayTeamScoreBoard->InsertItemBack();

				Character *pChar = (Character *)g_ObjectManager.GetObject(m_cp[i].m_dwObjKey);
				if(pChar)
				{
					//	이름
					iCnt = 0;
					Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s"), pChar->GetName());
					GlobalFunc::SetItem(m_pLstAwayTeamScoreBoard, idxAway, iCnt,
										szMessage,
										StrToWzID("mn12"),
										DT_VCENTER | DT_CENTER,
										RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA(100, 100, 180, 255));

					//	Kill
					iCnt = 1;
					Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), m_cp[i].m_wKillPoint);
					GlobalFunc::SetItem(m_pLstAwayTeamScoreBoard, idxAway, iCnt,
										szMessage,
										StrToWzID("mn12"),
										DT_VCENTER | DT_CENTER,
										RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA(100, 100, 180, 255));

					//  Death
					iCnt = 2;
					Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), m_cp[i].m_byDieNum);
					GlobalFunc::SetItem(m_pLstAwayTeamScoreBoard, idxAway, iCnt,
										szMessage,
										StrToWzID("mn12"),
										DT_VCENTER | DT_CENTER,
										RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA (100, 100, 180, 255));

					//	Combo
					iCnt = 3;
					Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%d"), m_cp[i].m_byCombo);
					GlobalFunc::SetItem(m_pLstAwayTeamScoreBoard, idxAway, iCnt,
										szMessage,
										StrToWzID("mn12"),
										DT_VCENTER | DT_CENTER,
										RGBA(0,0,0,0), RGBA(180, 180, 180, 255), RGBA(100, 100, 180, 255));
				}

			}
		}
	}
	

	ShowInterface(TRUE);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCHuntingScoreBoard::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
			case DLGPOS_BTN_EXIT:
				{
					this->ShowInterface(FALSE);
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


//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
