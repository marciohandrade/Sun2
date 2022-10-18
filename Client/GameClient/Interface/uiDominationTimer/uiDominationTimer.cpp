#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiDominationTimer/uiDominationTimer.h"

#include "GlobalFunc.h"
#include "Hero.h"
#include "ItemManager.h"
#include "DominationInfoParser.h"
#include "Map.h"

#ifdef _NA_008334_20150608_SONNENSCHEIN
  #include "uiDominationTowerMan/uiDominationTowerMan.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiDominationTimer::m_wzID[eDIALOG_DOMINATION_TIMER_MAX] = 
{
	StrToWzID("ST00"),//eDIALOG_DOMINATION_TIMER_TIME,
	StrToWzID("ST01"),//eDIALOG_DOMINATION_TIMER_PLAYER_CNT,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiDominationTimer::uiDominationTimer(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiDominationTimer::~uiDominationTimer()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationTimer::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;
	m_bStarted		  = false;
	m_dwCurrentTime   = 0;

	if(g_pMap)
	{
		const sDOMINATION_INFO* pDI = DominationInfoParser::Instance()->GetDominationInfo(g_pMap->GetCurrentFieldID());
		if(pDI)
		{
			m_iMaxPlayerCnt = pDI->m_MaxUser;
		}
	}

	InitControl();

#ifdef _NA_008334_20150608_SONNENSCHEIN
    event_state_ = 0;
#endif //_NA_008334_20150608_SONNENSCHEIN
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationTimer::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationTimer::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationTimer::Process(DWORD dwTick)
{
	if(true == m_bStarted)
	{
		if(m_dwCurrentTime > dwTick)
			m_dwCurrentTime -= dwTick;
		else
			m_dwCurrentTime = 0;
		
		SetTimer(m_dwCurrentTime);

		if(0 == m_dwCurrentTime)
			SetStartTimer(false);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationTimer::OnShowWindow(BOOL val)
{
	if( val )
	{
		if(g_pMap)
		{
			const sDOMINATION_INFO* pDI = DominationInfoParser::Instance()->GetDominationInfo(g_pMap->GetCurrentFieldID());
			if(pDI)
			{
				m_iMaxPlayerCnt = pDI->m_MaxUser;
			}
		}

		this->MoveDefaultWindowPos();

		m_bShow = TRUE;
	}
	else
	{
		m_bShow   = FALSE;

#ifdef _NA_008334_20150608_SONNENSCHEIN
        event_state_ = 0;
#endif //_NA_008334_20150608_SONNENSCHEIN
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationTimer::SetTimer(DWORD dwTime, bool bFirst /*= false*/ )
{
	if(bFirst)
		m_dwCurrentTime = dwTime;

	DWORD minTick   = 0;
	DWORD secTick   = 0;
	DWORD msecTick  = 0;

	if(dwTime)
	{
		DWORD dwTimeTemp1 = dwTime / SEC_PER_TICK;
		DWORD dwTimeTemp2 = dwTime % SEC_PER_TICK;

		if(dwTimeTemp1)
		{
			minTick   = dwTimeTemp1 / 60;
			secTick   = dwTimeTemp1 % 60;
		}

		if(dwTimeTemp2)
			msecTick  = dwTimeTemp2 / 10;
	}

	TCHAR szTime[16] = {0,};
	sprintf(szTime, "%02d\'%02d\"%02d", minTick, secTick, msecTick);

	if(10 <= minTick)
		m_pctrlTime->SetTextColor(RGBA(255, 255, 0, 255));
	else
		m_pctrlTime->SetTextColor(RGBA(255, 0, 0, 255));

	m_pctrlTime->SetTextWZ(szTime);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void uiDominationTimer::SetStartTimer(bool bStart)
{
	m_bStarted = bStart;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationTimer::SetPlayerCnt(int iPlayerCnt /*= 20*/)
{
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
	m_iPlayerCnt = iPlayerCnt;

	TCHAR szPlayerCnt[16] = {0,};
	sprintf(szPlayerCnt, "%d / %d", m_iPlayerCnt, m_iMaxPlayerCnt);
	m_pctrlPlayerCnt->SetTextWZ(szPlayerCnt);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiDominationTimer::InitControl()
{
	m_pctrlTime = (CCtrlStaticWZ*)getControl(eDIALOG_DOMINATION_TIMER_TIME);
	assert(m_pctrlTime);

	m_pctrlPlayerCnt = (CCtrlStaticWZ*)getControl(eDIALOG_DOMINATION_TIMER_PLAYER_CNT);
	assert(m_pctrlPlayerCnt);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    if (m_pctrlPlayerCnt)
    {
        m_pctrlPlayerCnt->ShowWindowWZ(WZ_HIDE);
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
}

#ifdef _NA_008334_20150608_SONNENSCHEIN
void uiDominationTimer::set_event_state(BYTE state)
{
    // 점령전과 소넨샤인에서 진행상태가 바뀌었을때 버프 띄울지 안띄울지를 갱신해주려고 사용
    event_state_ = state;
    uiDominationTowerMan::RefreshZoneMarkInfo();
}
#endif //_NA_008334_20150608_SONNENSCHEIN
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

