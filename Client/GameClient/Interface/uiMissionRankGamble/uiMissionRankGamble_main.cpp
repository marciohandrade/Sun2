#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM

#include "uiMissionRankGamble.h"

#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uiMissionRankMan2/uiMissionRankMan2.h"
#else
#include "uiMissionRankMan/uiMissionRankMan.h"
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "interfacemanager.h"

//------------------------------------------------------------------------------
WzID uiMissionRankGamble::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("B999"),	// 종료		// DLGPOS_MRG_BTN_EXIT
		StrToWzID("B001"),	// 확인		// DLGPOS_MRG_BTN_OK
		StrToWzID("B002"),	// 취소		// DLGPOS_MRG_BTN_CANCEL

		// 카드: 1등 앞면
		StrToWzID("B012"),		// DLGPOS_MRG_BTN_FRONT11
		StrToWzID("B013"),		// DLGPOS_MRG_BTN_FRONT12
		StrToWzID("B014"),		// DLGPOS_MRG_BTN_FRONT13

		// 카드: 1등 뒷면
		StrToWzID("B003"),		// DLGPOS_MRG_BTN_BACK11
		StrToWzID("B004"),		// DLGPOS_MRG_BTN_BACK12
		StrToWzID("B005"),		// DLGPOS_MRG_BTN_BACK13

		// 카드: 2등 앞면
		StrToWzID("B015"),		// DLGPOS_MRG_BTN_FRONT21
		StrToWzID("B016"),		// DLGPOS_MRG_BTN_FRONT22
		StrToWzID("B017"),		// DLGPOS_MRG_BTN_FRONT23

		// 카드: 2등 뒷면
		StrToWzID("B006"),		// DLGPOS_MRG_BTN_BACK21
		StrToWzID("B007"),		// DLGPOS_MRG_BTN_BACK22
		StrToWzID("B008"),		// DLGPOS_MRG_BTN_BACK23

		// 카드: 3등 앞면
		StrToWzID("B018"),		// DLGPOS_MRG_BTN_FRONT31
		StrToWzID("B019"),		// DLGPOS_MRG_BTN_FRONT32
		StrToWzID("B020"),		// DLGPOS_MRG_BTN_FRONT33

		// 카드: 3등 뒷면
		StrToWzID("B009"),		// DLGPOS_MRG_BTN_BACK31
		StrToWzID("B010"),		// DLGPOS_MRG_BTN_BACK32
		StrToWzID("B011"),		// DLGPOS_MRG_BTN_BACK33

};

//------------------------------------------------------------------------------
/**
*/
uiMissionRankGamble::uiMissionRankGamble(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    this->m_pMissionRankMan = GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );
#else
    this->m_pMissionRankMan = GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

    assert (m_pMissionRankMan);

    if (!this->m_pMissionRankMan)
    {
#ifdef _DEBUG
		 uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(_T("m_pMissionRankMan이 없습니다."));
#endif
        this->ShowInterface(FALSE);
        return;
    }

	m_bShow = false;
}

//------------------------------------------------------------------------------
/**
*/
uiMissionRankGamble::~uiMissionRankGamble()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankGamble::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankGamble::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankGamble::InitControls()
{
    this->m_bControlsInitialized = true;

	for(int i=0; i<POS_BTN_SIZE; ++i)
	{
		m_pBtnControls[i] = NULL;
		m_pBtnControls[i] = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_MRG_BTN_FRONT11 + i));
	}
}


//------------------------------------------------------------------------------
void uiMissionRankGamble::OnShowWindow( BOOL val )
{
    if (val)
    {
        //uiBase::MoveCenter();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
		m_bShow = true;
		
		_setGambleCard();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_mission_rank_gamble;
		msg.wParam = InterfaceManager::DIALOG_MISSION_RANK_GAMBLE;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_mission_rank_gamble);
    }
}

//------------------------------------------------------------------------------
void uiMissionRankGamble::_hideAllCard()
{
	for(int i=0; i<POS_BTN_SIZE; ++i)
	{
		m_pBtnControls[i]->ShowWindowWZ(WZ_HIDE);
	}
}

//------------------------------------------------------------------------------
bool uiMissionRankGamble::_isValidCardControl()
{
	for(int i=0; i<POS_BTN_SIZE; ++i)
	{
		if(m_pBtnControls[i] == NULL)
			return false;
	}

	return true;
}

//------------------------------------------------------------------------------
void uiMissionRankGamble::_showCardBack(DWORD dwRank)
{
	_hideAllCard();
	if(dwRank == 1)
	{
		for(int i=MRG_BTN_BACK11; i<=MRG_BTN_BACK13; ++i)
		{
			m_pBtnControls[i]->ShowWindowWZ(WZ_SHOW);
		}
	}
	else if(dwRank == 2)
	{
		for(int i=MRG_BTN_BACK21; i<=MRG_BTN_BACK23; ++i)
		{
			m_pBtnControls[i]->ShowWindowWZ(WZ_SHOW);
		}
	}
	else if(dwRank == 3)
	{
		for(int i=MRG_BTN_BACK31; i<=MRG_BTN_BACK33; ++i)
		{
			m_pBtnControls[i]->ShowWindowWZ(WZ_SHOW);
		}
	}
}

//------------------------------------------------------------------------------
void uiMissionRankGamble::_setGambleCard()
{
	if (!_isEnable())
        return;

	_hideAllCard();

	_showCardBack(GetManager()->m_dwMyRankForReward);
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiMissionRankGamble::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if( g_pHero && g_pHero->IsShowFullMap() )
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
void uiMissionRankGamble::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	if(!_isValidCardControl())
		return;

	switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
            switch( pos )
            {
			case DLGPOS_MRG_BTN_EXIT:
			case DLGPOS_MRG_BTN_CANCEL:
                {
                    m_bShow = false;
                }
                break;

			case DLGPOS_MRG_BTN_OK:
				{
					GetManager()->Send_CG_MISSION_RANKING_REWARD_SYN();
					
					m_bShow = false;
				}
				break;

			// 카드: 1등 앞면
			case DLGPOS_MRG_BTN_FRONT11:
			case DLGPOS_MRG_BTN_BACK11:
				{
					_showCardBack(1);
					m_pBtnControls[MRG_BTN_FRONT11]->ShowWindowWZ(WZ_SHOW);
					
				}
				break;

			case DLGPOS_MRG_BTN_FRONT12:
			case DLGPOS_MRG_BTN_BACK12:
				{
					_showCardBack(1);
					m_pBtnControls[MRG_BTN_FRONT12]->ShowWindowWZ(WZ_SHOW);
				}
				break;

			case DLGPOS_MRG_BTN_FRONT13:
			case DLGPOS_MRG_BTN_BACK13:
				{
					_showCardBack(1);
					m_pBtnControls[MRG_BTN_FRONT13]->ShowWindowWZ(WZ_SHOW);
				}
				break;

			// 카드: 2등 앞면
			case DLGPOS_MRG_BTN_FRONT21:
			case DLGPOS_MRG_BTN_BACK21:
				{
					_showCardBack(2);
					m_pBtnControls[MRG_BTN_FRONT21]->ShowWindowWZ(WZ_SHOW);

				}
				break;

			case DLGPOS_MRG_BTN_FRONT22:
			case DLGPOS_MRG_BTN_BACK22:
				{	
					_showCardBack(2);
					m_pBtnControls[MRG_BTN_FRONT22]->ShowWindowWZ(WZ_SHOW);
				}
				break;

			case DLGPOS_MRG_BTN_FRONT23:
			case DLGPOS_MRG_BTN_BACK23:
				{
					_showCardBack(2);
					m_pBtnControls[MRG_BTN_FRONT23]->ShowWindowWZ(WZ_SHOW);
				}
				break;

			// 카드: 3등 앞면
			case DLGPOS_MRG_BTN_FRONT31:
			case DLGPOS_MRG_BTN_BACK31:
				{
					_showCardBack(3);
					m_pBtnControls[MRG_BTN_FRONT31]->ShowWindowWZ(WZ_SHOW);
				}
				break;

			case DLGPOS_MRG_BTN_FRONT32:
			case DLGPOS_MRG_BTN_BACK32:
				{
					_showCardBack(3);
					m_pBtnControls[MRG_BTN_FRONT32]->ShowWindowWZ(WZ_SHOW);
				}
				break;

			case DLGPOS_MRG_BTN_FRONT33:
			case DLGPOS_MRG_BTN_BACK33:
				{
					_showCardBack(3);
					m_pBtnControls[MRG_BTN_FRONT33]->ShowWindowWZ(WZ_SHOW);
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
/**
*/
void uiMissionRankGamble::OnUpdateSolarDialog()
{
	if(!m_bShow)
	{
		ShowInterface(FALSE);
		return;
	}

    if (!_isEnable())
        return;
}
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM


