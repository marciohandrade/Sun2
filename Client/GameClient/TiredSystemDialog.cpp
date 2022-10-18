#include "SunClientPrecompiledHeader.h"

#include "tiredsystemdialog.h"
#include "SceneBase.h"
#include "Application.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "Hero.h"
#include "ItemManager.h"
#include "SunEventInfoParser.h"

#define	MIN_OFFLINE_TIME	10	// 10분 미만 유저는 표시하지 않는다.

WzID TiredSystemDialog::m_wzId[TIREDSYSTEM_DIALOG_MAX] =
{
	StrToWzID("T001"),
	StrToWzID("PI00"),
#ifdef _SCJ_EXP_EVENT_ENABLE_EFFECT
    StrToWzID("P000"),
#endif //_SCJ_EXP_EVENT_ENABLE_EFFECT
};

BOOL	TiredSystemDialog::m_bOnceMessage = FALSE;

//------------------------------------------------------------------------------
/**
*/
TiredSystemDialog::TiredSystemDialog(InterfaceManager * pUIman):
	uiBase(pUIman),m_bUseFatigue( FALSE )
{
	m_pStatic  = NULL;
	m_pPicture = NULL;
	m_iAllAreaTime = 0;
	m_fPictureHeight = 0.0f;
	m_iPendingAllAreaHour =0;
	m_iTiredLevel = eTIRED_NO;
	ZeroMemory(&m_sysLocalTime,sizeof(m_sysLocalTime));
	m_bOncePrint[0] = FALSE;
	m_bOncePrint[1] = FALSE;
}

//------------------------------------------------------------------------------
/**
*/
TiredSystemDialog::~TiredSystemDialog(void)
{

}

//------------------------------------------------------------------------------
/**
*/
void TiredSystemDialog::Release()
{
}


//------------------------------------------------------------------------------
/**
*/
void TiredSystemDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	m_pStatic = static_cast< CCtrlStaticWZ*>(getControl(TIREDSYSTEM_TIME));
    if (m_pStatic != NULL)
    {
        m_pStatic->SetTextKind(ETK_SHADOW);
    }    
	m_pPicture = static_cast<CCtrlPictureWZ*>(getControl(TIREDSYSTEM_PICTURE));
	m_pPicture->SetOutlineColor(WzColor_RGBA(255,0,0,255));
	m_rtPicture = m_pPicture->GetSizeRect();
	m_rtSizePicture = m_pPicture->GetSize();
	this->m_fPictureWidth = (float)m_rtPicture.right -m_rtPicture.left;
	this->m_fPictureHeight = (float)m_rtSizePicture.bottom - m_rtSizePicture.top ;
	m_bMouseOver = FALSE;
	util::TimeSync::GetLocalTime(&m_sysLocalTime);
	m_iPendingTime = m_sysLocalTime.wMinute;

	m_pBattlezoneMan = (uiBattleZoneMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE);
	assert(m_pBattlezoneMan);

	m_pPicture->MoveWindowWZ((float)m_rtPicture.left, (float)m_rtPicture.top, 0.0f, (float)m_fPictureHeight + 2);

#ifdef _SCJ_EXP_EVENT_ENABLE_EFFECT
    peak_time_start_tick_ = 0;
    peak_start_effect_ = false;
    CCtrlPictureWZ* picture_control = static_cast<CCtrlPictureWZ*>(getControl(TIREDSYSTEM_EXP_EVENT));
    if (picture_control)
    {
        picture_control->ShowWindowWZ(WZ_HIDE);
        picture_control->SetDiscardMessage(true);
    }
    base_text_color_ = m_pStatic->GetTextColor();
    peak_time_text_color_ = RGBA(255,255,119,255);
#endif //_SCJ_EXP_EVENT_ENABLE_EFFECT
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	gm_event_exp_ratio_ = 0.0f;
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
}

//------------------------------------------------------------------------------
/**
*/
void TiredSystemDialog::MessageProc( SI_MESSAGE * pMessage )
{
	SolarDialog::MessageProc( pMessage );
}

//------------------------------------------------------------------------------
/**
*/
void TiredSystemDialog::NetworkProc( MSG_BASE * pMsg )
{
}

//------------------------------------------------------------------------------
/**
*/
void TiredSystemDialog::Process(DWORD dwTick)
{
	
}

//------------------------------------------------------------------------------
/**
*/
void TiredSystemDialog::OnShowWindow(BOOL val)
{
}

//------------------------------------------------------------------------------
/**
*/
void TiredSystemDialog::OnUpdateSolarDialog()
{
	util::TimeSync::GetLocalTime(&m_sysLocalTime);

	if (GlobalFunc::IsSSQScene())
    {
		return;
    }

#ifdef _SCJ_EXP_EVENT_ENABLE_EFFECT
    // 경험치이벤트 체크
    UpdatePeakTime();
#endif //_SCJ_EXP_EVENT_ENABLE_EFFECT

    if (TIMERPARAM->GetShowTiredSystem() == 2)
    {
        return;
    }

	UpdateEachCaseTime();
	UpdatePictureGaugeBar();
	CalculateTiredLevel();
}

//------------------------------------------------------------------------------
/**
*/
void TiredSystemDialog::OnRenderSolarDialog()
{
	RenderSystemTime();

#ifdef _SCJ_EXP_EVENT_ENABLE_EFFECT
    RenderPeakTime(); //경험치 이벤트
#endif //_SCJ_EXP_EVENT_ENABLE_EFFECT

    if (TIMERPARAM->GetShowTiredSystem() != 2)
    {
#if defined(__KR) || defined(_KOREA)
        RenderTiredsystemMessage();
#endif // 
    }




	UpdateToolTip();
}

//------------------------------------------------------------------------------
/**
*/
CControlWZ * TiredSystemDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < TIREDSYSTEM_DIALOG_MAX );
	return GetControlWZ( m_wzId[AtPos] );
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE TiredSystemDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < TIREDSYSTEM_DIALOG_MAX; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return TIREDSYSTEM_DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
void TiredSystemDialog::RenderTiredsystemMessage()
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	if( m_iTiredLevel == eTIRED_100PERSENT && !m_bOncePrint[0])
	{
		m_bOncePrint[0] = TRUE;
		g_InterfaceManager.GetInterfaceString(eST_TIRED_SYSTEM_ITEM_NOT_GET,szMessage,INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
		m_iTiredLevel = eTIRED_NO;
	}
	else if( m_iTiredLevel == eTIRED_75PERSENT && !m_bOncePrint[1])
	{
		m_bOncePrint[1] = TRUE;
		g_InterfaceManager.GetInterfaceString(eST_TIRED_SYSTEM_75PERSENT_OVER,szMessage,INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);	
		m_iTiredLevel = eTIRED_NO;
	}
}

//------------------------------------------------------------------------------
/**
*/
void TiredSystemDialog::CalculateTiredLevel()
{
    int max_tired_time = TIMERPARAM->GetMaxTiredTime();
    if(m_iTime >= max_tired_time)
        m_iTiredLevel = eTIRED_100PERSENT;	
    else if(m_iTime >= (max_tired_time * 0.75f))
        m_iTiredLevel = eTIRED_75PERSENT;
    else 
        m_iTiredLevel = eTIRED_NO;
}

//------------------------------------------------------------------------------
/**
*/
void TiredSystemDialog::UpdatePictureGaugeBar()
{
    m_fGaugeWidth =  m_fPictureWidth * ((float)m_iTime / (float)TIMERPARAM->GetMaxTiredTime());

	if(m_fGaugeWidth < 0.0f) m_fGaugeWidth = 0.0f;
	if(m_fGaugeWidth > m_fPictureWidth) m_fGaugeWidth = m_fPictureWidth;
	RECT rtTiredDialog;
	CControlWZ* pControl = 	this->getControl(TIREDSYSTEM_TIME);
	rtTiredDialog = pControl->GetSizeRect(); 

	// 피로도에 따른 게이지바 출력
	m_pPicture->MoveWindowWZ(
		(float)rtTiredDialog.left,
		(float)rtTiredDialog.top,
		(float)m_fGaugeWidth,
		(float)m_fPictureHeight + 2);
}


//------------------------------------------------------------------------------
/**
*/
void TiredSystemDialog::RenderSystemTime()
{
	TCHAR	time_string[128];

    // 현재시간 출력
    Sprintf(time_string, 
            _T("%02d:%02d:%02d"), 
            m_sysLocalTime.wHour, 
            m_sysLocalTime.wMinute, 
            m_sysLocalTime.wSecond);

    // 현재날짜 출력
    TCHAR	date_string[128] = {0, };
    
#ifdef _INTERNATIONAL_DATE_TOOLTIP
    uiToolTipMan* tooltipman_ptr = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if(tooltipman_ptr)
    {
        TCHAR month_string[32] = {0, };
        int month_stringcode = tooltipman_ptr->GetStringMonth(m_sysLocalTime.wMonth);
        g_InterfaceManager.GetInterfaceString(month_stringcode, month_string, 32 );
        Snprintf(date_string, 
                 128, 
                 _T("%s %02d, %4d "), 
                 month_string,			//달
                 m_sysLocalTime.wDay,   //일 
                 m_sysLocalTime.wYear); //년도
    }	
#else
    Snprintf(date_string, 
             128, 
             _T("%4d/%02d/%02d "), 
             m_sysLocalTime.wYear,      //년도
             m_sysLocalTime.wMonth,     //달
             m_sysLocalTime.wDay);      //일
#endif //_INTERNATIONAL_DATE_TOOLTIP

    strcat(date_string, time_string);
    m_pStatic->SetTextWZ(date_string);
}

void TiredSystemDialog::UpdateEachCaseTime()
{
	if( !g_pHero)
		return;

	// 죽은 상태라면 피로도 계산을 하지 않는다.
	if( g_pHero->IsDead() )
		return;

	// 1분이 지나면, 누적접속시간을 1분 증가시켜준다.
	if(m_iPendingAllAreaTime != m_sysLocalTime.wMinute)
	{
		if(  m_sysLocalTime.wSecond == 0)
		{
			this->m_iPendingAllAreaTime = m_sysLocalTime.wMinute;
			this->m_iAllAreaTime += 1;
		}
	}

	if( GetUseFatigue() == FALSE )			// 피로도 계산을 안하는 넘이라면 리턴
		return;

	// 1분이 지나면, 피로도시간을 1분 증가시켜준다.
#ifndef _CHINA
	if(GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_VILLAGE)
#endif
	{
		if(m_iPendingTime != m_sysLocalTime.wMinute)
		{
			m_iPendingTime = m_sysLocalTime.wMinute;

			POSTYPE Outpos;
			if( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_MISSION ||
				GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHUNTING 
				|| GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_INSTANCE_DUNGEON
			  )
			{
				uiBattleZoneMan * pBattleZoneMan = ( uiBattleZoneMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE);
				if( !pBattleZoneMan)
					return;

				BOOL bHaveSuperMasterItem = ItemManager::Instance()->FindFirstItem(eITEMTYPE_SUPERMASTER , Outpos);
#ifdef	_CHINA
				BOOL bIsPcBangRoomMaster  = g_pHero->GetPCBangMode();
#else
				BOOL bIsPcBangRoomMaster  = g_pHero->GetPCBangMode() && pBattleZoneMan->IsMaster(); 
#endif
				// 아이템도 가지고 있지않고 PC방 마스터도 아니면 피로도 시간 증가

				if( bHaveSuperMasterItem || bIsPcBangRoomMaster )
					return;
			}

			// 필드에서는 무조건 피로도가 증가
			m_iTime += 1;
		}
		
	}
}

BOOL TiredSystemDialog::GetUseFatigue() const
{
#ifdef	__CN_OO0510__FATIGUE_MATCHING_UPDATE
	return m_bUseFatigue; 
#else
	return TRUE;
#endif
}

void TiredSystemDialog::UpdateToolTip()
{

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	m_bMouseOver = FALSE;
	CControlWZ * pControl = getControl(TIREDSYSTEM_TIME);

	if(pControl)
	{
		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				m_bMouseOver = TRUE;
				uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
				if(pUIMan)
				{
#ifdef _SCJ_EXP_EVENT_ENABLE_EFFECT
                    if (TIMERPARAM->GetShowTiredSystem() != 2)
                    {
                        if( GetUseFatigue() == FALSE )
                            return;

	    				pUIMan->RegisterTiredSystemToolTip( m_iTime );
						pUIMan->SetTooltipDialogID(GetDialogKey());
                    }
                    else
                    {
                        // 경험치이벤트 체크
                        if (peak_reward_exp_rate > 0.0001f)
                        {
                            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
                            TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0,};

                            //5671 지금은 경험치 이벤트 적용 시간입니다. %d%의 경험치를 추가로 얻습니다.
                            g_InterfaceManager.GetInterfaceString(5671, temp_message, INTERFACE_STRING_LENGTH-1);
                            Sprintf(message, temp_message, static_cast<int>(peak_reward_exp_rate*100));

                            pUIMan->RegisterTooltipSimpleText(message);
							pUIMan->SetTooltipDialogID(GetDialogKey());

                        }
                    }
#endif //_SCJ_EXP_EVENT_ENABLE_EFFECT
				}
			}
		}
	}

}

void TiredSystemDialog::RenderTiredsystemMessage(int iTime)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,} , szResult[INTERFACE_STRING_LENGTH] ={0,};
	const int iExtraTime = 3;

	if( iTime >= 300 )
	{
		// 5042 이미 피로시간이므로 경험치와 젠을 획득할수 없으면, 게임을 종료하고 휴식하기를 바랍니다.
		g_InterfaceManager.GetInterfaceString(5042 , szResult ,INTERFACE_STRING_LENGTH);
	}
	else if ( iTime >= 210 && iTime < 300  )
	{
		// 5043 누적 게임시간이 3 시간을 초과했기때문에 경험치와 젠 획득량이 반으로 줄어듭니다.
		g_InterfaceManager.GetInterfaceString(5043, szResult ,INTERFACE_STRING_LENGTH);
	}
	else if( iTime >= 180 && iTime < 210 )
	{
		//5022 게임 누적시간이 %d 시간에 도달했습니다. 적당한 휴식이 필요합니다.
		g_InterfaceManager.GetInterfaceString(5022 , szMessage ,INTERFACE_STRING_LENGTH );
		Snprintf(szResult , INTERFACE_STRING_LENGTH -1 , szMessage , 3);
	}
	else if ( iTime >= 120 && iTime < 180)
	{
		//5021 게임 누적시간이 %d 시간에 도달했습니다.
		g_InterfaceManager.GetInterfaceString(5021 , szMessage ,INTERFACE_STRING_LENGTH);
		Snprintf( szResult  ,INTERFACE_STRING_LENGTH -1 ,szMessage , 2 );
	}
	else if (  iTime >= 60  && iTime < 120)
	{
		////5021 게임 누적시간이 %d 시간에 도달했습니다.
		g_InterfaceManager.GetInterfaceString(5021 , szMessage ,INTERFACE_STRING_LENGTH);
		Snprintf( szResult  ,INTERFACE_STRING_LENGTH -1 ,szMessage , 1 );
	}
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szResult);
}

void	TiredSystemDialog::OffLineTimeOfFatigueMessage( void )
{
	if( GetUseFatigue() == FALSE )			// 피로도 계산을 안하는 넘이라면 리턴
		return;

#ifdef	_K2P_PRINT_ONCE_MESSAGE
	if( m_bOnceMessage == TRUE )
		return;

	m_bOnceMessage = TRUE;

	if( m_dwOffLineTime < MIN_OFFLINE_TIME )
		return;
#endif


	int day = 0, hour = 0, min = 0;
	TCHAR szRemainTime[INTERFACE_STRING_LENGTH] = { '\0', };
	GlobalFunc::CalcTime( m_dwOffLineTime, day, hour, min, szRemainTime);

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,} , szResult[INTERFACE_STRING_LENGTH] = {0,};
	g_InterfaceManager.GetInterfaceString( 6044, szMessage , INTERFACE_STRING_LENGTH);
	Snprintf( szResult , INTERFACE_STRING_LENGTH -1, szMessage, szRemainTime );
	GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szResult );
}

#ifdef _SCJ_EXP_EVENT_ENABLE_EFFECT
void TiredSystemDialog::RenderPeakTime()
{

    EVENT_SUB_INFO_ITER it;
    EVENTINFO_MAP_ITER	eventinfo_itr;

    std::pair<EVENT_SUB_INFO_ITER, EVENT_SUB_INFO_ITER> range;
    range = SunEventInfoParser::Instance()->GetEventSubInfoMap()->equal_range(eEVENT_PEAKTIME);
    peak_reward_exp_rate = 0.0f;
    bool is_peak_time = false;
    for (it=range.first; it != range.second; ++it)
    {
        EVENT_SUB_INFO info = (*it).second;
        eventinfo_itr = SunEventInfoParser::Instance()->GetEventInfoMap()->find(info.m_dwEventID);
        EVENT_INFO event_info = (*eventinfo_itr).second;

        if (event_info.CheckEvent())
        {
            SYSTEMTIME system_time;
            util::TimeSync::GetLocalTime(&system_time);
            WORD time = (system_time.wHour*100) + (system_time.wMinute);

            if ((time >= event_info.m_wPeakTimeStart) &&
                (time <= event_info.m_wPeakTimeEnd))
            {
                //피크 타임일 때
                if (peak_start_effect_ == false)
                {
                    //그리기가 시작 안되었을 때
                    peak_start_effect_ = true;
                    peak_time_start_tick_ = GetTickCount();
                }
                peak_reward_exp_rate += event_info.m_fRewardExpRate;
                is_peak_time = true;
            }
        }//if (event_info.CheckEvent())
    }//for (it=range.first; it != range.second; ++it)

#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	if ( gm_event_exp_ratio_ > 0.0001f )
	{
		peak_reward_exp_rate += gm_event_exp_ratio_;

		// 피크 타임일 때
		if ( peak_start_effect_ == false )
		{
			// 그리기가 시작 안되었을 때
			peak_start_effect_ = true;
			peak_time_start_tick_ = GetTickCount();
		}
		is_peak_time = true;
	}
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT

    //eEVENT_PCBANG_PEAKTIME
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    BOOL is_pc_room = g_pHero->GetPCRoomStatus()==CHANGE_PCROOM_STATUS::eStatus_PcRoom;
#else
    BOOL is_pc_room = g_pHero->GetPCBangMode();
#endif//_PCBANG_POLICY_CHANGE_POST_EXPIRED
    if (is_pc_room == TRUE)
    {
        range = SunEventInfoParser::Instance()->GetEventSubInfoMap()->equal_range(eEVENT_PCBANG_PEAKTIME);
        for (it=range.first; it != range.second; ++it)
        {
            EVENT_SUB_INFO info = (*it).second;
            eventinfo_itr = SunEventInfoParser::Instance()->GetEventInfoMap()->find(info.m_dwEventID);
            EVENT_INFO event_info = (*eventinfo_itr).second;

            if (event_info.CheckEvent())
            {
                SYSTEMTIME system_time;
                util::TimeSync::GetLocalTime(&system_time);
                WORD time = (system_time.wHour*100) + (system_time.wMinute);

                if ((time >= event_info.m_wPeakTimeStart) &&
                    (time <= event_info.m_wPeakTimeEnd))
                {
                    //피크 타임일 때
                    if (peak_start_effect_ == false)
                    {
                        //그리기가 시작 안되었을 때
                        peak_start_effect_ = true;
                        peak_time_start_tick_ = GetTickCount();
                    }
                    peak_reward_exp_rate += event_info.m_fRewardExpRate;
                    is_peak_time = true;
                }
            }
        }
    }

    if ((is_peak_time == false) &&
        (peak_start_effect_ == true))
    {
        //피크 타임이 아닐 때 + peak_effect_start_ 가 true이면 (피크 종료됬을 때)
        peak_start_effect_ = false;
        peak_time_start_tick_ = GetTickCount();
    }
    

    const DWORD PEAK_TIME_ANIMATION_TICK = 1000;
    const DWORD HALF_PEAK_TIME_ANIMATION_TICK = 500;

    const float active_effect_increase_alpha = 0.3f;
    const float active_effect_decrease_alpha = 0.3f;

    DWORD progress_tick = GetTickCount() - peak_time_start_tick_;
    int color_fill_alpha = 0;

    if (progress_tick < HALF_PEAK_TIME_ANIMATION_TICK)
    {
        //반바퀴 동안 페이드인
        color_fill_alpha = static_cast<int>(active_effect_increase_alpha * progress_tick);
        if (color_fill_alpha > 255)
        {
            color_fill_alpha = min(color_fill_alpha,255);
        }
    }
    else if (progress_tick < PEAK_TIME_ANIMATION_TICK)
    {
        //반바퀴 후 페이드아웃
        color_fill_alpha = static_cast<int>((active_effect_increase_alpha * HALF_PEAK_TIME_ANIMATION_TICK) -
            active_effect_decrease_alpha * (progress_tick - HALF_PEAK_TIME_ANIMATION_TICK));
        if (color_fill_alpha < 0)
        {
            color_fill_alpha = max(color_fill_alpha,0);
        }
    }

    if (m_pStatic != NULL)
    {
        const float offset = 2.0f;
        g_pSunRenderer->ColorFill(WzColor_RGBA(255,255,255,color_fill_alpha), 
            static_cast<float>(m_pStatic->GetSize().left-offset), 
            static_cast<float>(m_pStatic->GetSize().top-offset),
            m_pStatic->GetSizeWidth()+(offset*2),
            m_pStatic->GetSizeHeight()+(offset*2));
    }
}

void TiredSystemDialog::UpdatePeakTime()
{
    CCtrlPictureWZ* picture_control = static_cast<CCtrlPictureWZ*>(getControl(TIREDSYSTEM_EXP_EVENT));
    if (picture_control && m_pStatic)
    {
        if (peak_reward_exp_rate > 0.0001f)
        {
            picture_control->ShowWindowWZ(WZ_SHOW);
            m_pStatic->SetTextColor(peak_time_text_color_);
        }
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
		else if (gm_event_exp_ratio_ > 0.0001f)
		{
			picture_control->ShowWindowWZ(WZ_SHOW);
			m_pStatic->SetTextColor(peak_time_text_color_);
		}
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
        else
        {
            picture_control->ShowWindowWZ(WZ_HIDE);
            m_pStatic->SetTextColor(base_text_color_);
        }
    }
}

void TiredSystemDialog::MoveuiForce( int _x,int _y )
{
	float x = this->GetDialogWZ()->GetDlgX();

	this->GetDialogWZ()->MoveWindowWZ(x,(float)_y);
}
#endif //_SCJ_EXP_EVENT_ENABLE_EFFECT
