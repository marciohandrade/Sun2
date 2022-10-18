#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM

#include "uiMissionRankTimer.h"
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
WzID uiMissionRankTimer::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("T001"),		// 최고기록		//DLGPOS_MRT_TXT_BEST,
		StrToWzID("T002"),		// 진행기록		//DLGPOS_MRT_TXT_PROGRESS,
};

//------------------------------------------------------------------------------
/**
*/
uiMissionRankTimer::uiMissionRankTimer(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    m_pMissionRankMan = GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );
#else
    m_pMissionRankMan = GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );
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
}

//------------------------------------------------------------------------------
/**
*/
uiMissionRankTimer::~uiMissionRankTimer()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankTimer::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankTimer::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankTimer::InitControls()
{
    m_pTxtControl[DLGPOS_MRT_TXT_BEST]	 = static_cast<CCtrlStaticWZ*>(this->getControl(DLGPOS_MRT_TXT_BEST));
    m_pTxtControl[DLGPOS_MRT_TXT_PROGRESS]= static_cast<CCtrlStaticWZ*>(this->getControl(DLGPOS_MRT_TXT_PROGRESS));

    if (m_pTxtControl[DLGPOS_MRT_TXT_BEST])
    {
        m_pTxtControl[DLGPOS_MRT_TXT_BEST]->SetDiscardMessage(true);
    }
    CControlWZ* control_pointer = static_cast<CControlWZ*>(this->getControl(DLGPOS_MRT_TXT_BEST_PICTURE));
    if (control_pointer)
    {
        control_pointer->SetDiscardMessage(true);
    }

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiMissionRankTimer::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }

		this->MoveDefaultWindowPos();

		UpdateTime();
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiMissionRankTimer::_isEnable()
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
void uiMissionRankTimer::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankTimer::OnUpdateSolarDialog()
{
    if (!_isEnable())
        return;

	DWORD dwCurTime = clock_function::GetTickCount();
	if(GetManager()->m_dwUpdateMissionTime)
	{
		GetManager()->m_dwElapsedTime = GetManager()->m_dwMissionTime + (dwCurTime - GetManager()->m_dwUpdateMissionTime);
	}
	else if(GetManager()->m_dwStartMissionTime)
	{
		GetManager()->m_dwElapsedTime = GetManager()->m_dwMissionTime + (dwCurTime - GetManager()->m_dwStartMissionTime);
	}
	else
	{
		GetManager()->m_dwElapsedTime = 0;
		return;

	}

	UpdateBestTime();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankTimer::OnRenderSolarDialog()
{
	if (!_isEnable())
        return;

	if(GetManager()->IsEnableMissionRanking())	// 5인이상의 파티만 미션랭킹 대상이 된다.
	{
		BOOL bTest = g_pSunRenderer->IsEnableZTest();
		g_pSunRenderer->SetZBufferTest(FALSE);
		WzColor wzColor = WzColor_RGBA(0,0,0,255);
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont(StrToWzID("mn12"));
	
		RECT rcDlg, rc;
		this->GetDialogWZ()->GetClientRect(&rcDlg);

		TCHAR szMessage[TEMP_STRING_LENGTH] = {0,};
		
		if(GetManager()->m_dwTotalMonsterKillNum < GetManager()->m_dwMaxMonsterKillNum)
		{
			// 잡은 몬스터의 마리수 표시
			Snprintf(szMessage, TEMP_STRING_LENGTH-1, _T("%d / %d"), GetManager()->m_dwTotalMonsterKillNum, GetManager()->m_dwMaxMonsterKillNum);
		}
		else
		{
			Snprintf(szMessage, TEMP_STRING_LENGTH-1, _T("MISSION RANKING"));
		}

		LONG textWidth = GlobalFunc::GetTextWidth(szMessage, StrToWzID("mn12"));
#ifdef _INTERNATIONAL_UI
		//영문은 아래다 찍는다.
		SetRect(&rc,
			rcDlg.right - textWidth - 10,
			rcDlg.bottom+2,
			rcDlg.right,
			rcDlg.bottom+15);
		
#else
		SetRect(&rc,
			rcDlg.right - textWidth - 10,
			rcDlg.top - 15,
			rcDlg.right,
			rcDlg.top - 2);
#endif//_INTERNATIONAL_UI
		g_pSunRenderer->x_pManagerTextOut->DrawText(szMessage,
											&rc,
											WzColor_RGBA(255, 255, 255, 255),
											WzColor_RGBA(0,0,0,0),
											TP_HLEFT,
											ETS_OUTLINE,
											wzColor);
	

		
		g_pSunRenderer->x_pManagerTextOut->EndUseFont();
		g_pSunRenderer->SetZBufferTest(bTest);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankTimer::UpdateBestTime()
{
	if(GetManager())
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

		util::_tstring strTime;

		// 최고기록 표시
		if(GetManager()->m_dwMissionBestTime)
		{
			strTime.clear();
			GetManager()->GetTimeString(GetManager()->m_dwMissionBestTime, strTime);
			g_InterfaceManager.GetInterfaceString( 9903, szTemp, INTERFACE_STRING_LENGTH );
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, strTime.c_str());
		}
		else
		{
			g_InterfaceManager.GetInterfaceString( 9902, szMessage, INTERFACE_STRING_LENGTH );
		}

		if(m_pTxtControl[DLGPOS_MRT_TXT_BEST])
		{
			GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRT_TXT_BEST], szMessage );
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankTimer::UpdateTime()
{
	if(GetManager())
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szTime[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

		DWORD minTick   = 0;
		DWORD secTick   = 0;
		DWORD msecTick  = 0;

		// 미션 종료
		if(GetManager()->m_bMissionCleared)
		{
			if(GetManager()->m_dwMissionClearTime) //최종 결과 시간
			{
				DWORD dwTime1 = GetManager()->m_dwMissionClearTime / SEC_PER_TICK;
				DWORD dwTime2 = GetManager()->m_dwMissionClearTime % SEC_PER_TICK;

				if(dwTime1)
				{
					minTick   = dwTime1 / 60;
					secTick   = dwTime1 % 60;
				}

				if(dwTime2)
					msecTick  = dwTime2 / 10;
			}

			Snprintf (szTime, INTERFACE_STRING_LENGTH-1, _T("%03d'%02d''%02d"), minTick, secTick, msecTick);
			g_InterfaceManager.GetInterfaceString( 9906, szTemp, INTERFACE_STRING_LENGTH );
			
		}
		else
		{
			// 미션 진행중
			if(GetManager()->m_dwElapsedTime)
			{
				DWORD dwTime1 = GetManager()->m_dwElapsedTime / SEC_PER_TICK;
				DWORD dwTime2 = GetManager()->m_dwElapsedTime % SEC_PER_TICK;

			
				if(dwTime1)
				{
					minTick   = dwTime1 / 60;
					secTick   = dwTime1 % 60;
				}

				if(dwTime2)
					msecTick  = dwTime2 / 10;
			}
			
			Snprintf (szTime, INTERFACE_STRING_LENGTH-1, _T("%03d'%02d''%02d"), minTick, secTick, msecTick);
			g_InterfaceManager.GetInterfaceString( 9905, szTemp, INTERFACE_STRING_LENGTH );
		}

		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szTime);

		if(m_pTxtControl[DLGPOS_MRT_TXT_PROGRESS])
		{
			GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRT_TXT_PROGRESS], szMessage );
		}
	}
}
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM
