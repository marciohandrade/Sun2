#include "SunClientPrecompiledHeader.h"


#include "PcbangPlayTimeDialog.h"
#include "Hero.h"


WzID PcbangPlayTimeDialog::m_wzId[PCBANGPLAYTIME_MAX] =
{
	StrToWzID("P001"),
	StrToWzID("T003"),
};

PcbangPlayTimeDialog::PcbangPlayTimeDialog(void)
{
	
}


PcbangPlayTimeDialog::~PcbangPlayTimeDialog(void)
{

}

VOID PcbangPlayTimeDialog::Init( CDrawBase * pDrawBase )
{

	m_pPicture = static_cast<CCtrlPictureWZ *> (getControl(PCBANGPLAYTIME_PCITURE) );
	RECT rt = m_pPicture->GetSizeRect();
	m_iMaxWidth = rt.right - rt.left;

	m_iCurTime = 0;
	UpdateGauge();
	ZeroMemory(&m_sysLocalTime , sizeof(m_sysLocalTime) );
	util::TimeSync::GetLocalTime(&m_sysLocalTime);
	m_iPendingTime = m_sysLocalTime.wMinute;

	m_pCaption = static_cast<CCtrlStaticWZ *> (getControl(PCBANGPLAYTIME_CAPTION) );

	TCHAR szString[INTERFACE_STRING_LENGTH] = {0, };
	int nSCode = 237;
    int EventMode = TIMERPARAM->GetShowEventTimer();
	
	
	if ( EventMode == 2 )
		nSCode = 1844;	// 일반
	else if ( EventMode == 3 )
		nSCode = 1843;	// PC방 + 일반
	
	g_InterfaceManager.GetInterfaceString( nSCode, szString, INTERFACE_STRING_LENGTH);
	m_pCaption->SetTextWZ(szString);
}

VOID PcbangPlayTimeDialog::Release()
{

}

CControlWZ * PcbangPlayTimeDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < PCBANGPLAYTIME_MAX );
	return GetControlWZ( m_wzId[AtPos] );
}

VOID PcbangPlayTimeDialog::MessageProc( SI_MESSAGE * pMessage )
{

}

VOID PcbangPlayTimeDialog::NetworkProc( MSG_BASE * pMsg )
{
	
}

VOID PcbangPlayTimeDialog::Process(DWORD dwTick)
{
    if ( m_iCurTime >= TIMERPARAM->GetMaxEventTime())
    {
        return;
    }

    int EventMode = TIMERPARAM->GetShowEventTimer();
	BOOL PCBangMode = (g_pHero && g_pHero->GetPCBangMode());
	if ((EventMode == 1 && PCBangMode)
		|| (EventMode == 2 && !PCBangMode)
		|| EventMode == 3)
	{
		util::TimeSync::GetLocalTime(&m_sysLocalTime);
		if(m_iPendingTime != m_sysLocalTime.wMinute)
		{
			m_iPendingTime = m_sysLocalTime.wMinute;
			m_iCurTime += 1;
			UpdateGauge();
		}
	}
	
}


VOID PcbangPlayTimeDialog::OnShowWindow(BOOL val)
{
}

void PcbangPlayTimeDialog::SetTime(int iTime)
{
	m_iCurTime = iTime;
    int max_event_time = TIMERPARAM->GetMaxEventTime();
	if( m_iCurTime > max_event_time)
    {
		m_iCurTime = max_event_time;
    }
	if( m_iCurTime < 0)
    {
		m_iCurTime = 0;
    }
	UpdateGauge();
}

void PcbangPlayTimeDialog::UpdateGauge()
{
	int iMinute = m_iCurTime % 60;
	int iHour	= m_iCurTime / 60;

	TCHAR szMessage[INTERFACE_STRING_LENGTH];

	TCHAR szHour[100] ,szMinute[100];

	if( iHour < 10 )
	{
		Sprintf(szHour , _T("0%d") , iHour);
	}
	else
	{
		Sprintf(szHour , _T("%d") , iHour);
	}

	if( iMinute < 10)
	{
		Sprintf(szMinute , _T("0%d") , iMinute);
	}
	else
	{
		Sprintf(szMinute , _T("%d") , iMinute);
	}

	Sprintf(szMessage , _T("%s : %s") , szHour , szMinute );
	SetCaptionPcbangPlayTime(szMessage);

    float fPercentage = (float) m_iCurTime / (float) TIMERPARAM->GetMaxEventTime();
	
	m_fPictureWidth = (float) m_iMaxWidth * fPercentage ;
	
	
	Sprintf(szMessage , _T("%3.2f%%") , fPercentage * 100.0f );
	SetCaptionPcbangPlayPercentage(szMessage);

	RECT rt = m_pPicture->GetSizeRect();

	m_pPicture->MoveWindowWZ((float) rt.left , (float) rt.top , m_fPictureWidth , (float) (rt.bottom - rt.top) );

}
