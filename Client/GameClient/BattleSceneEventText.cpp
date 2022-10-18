#include "SunClientPrecompiledHeader.h"
#include "BattleSceneEventText.h"

#include "Application.h"
#include "InterfaceManager.h"
#include "SoundEffect.h"
#include "EventInfoParser.h"
#include "GlobalFunc.h"
#include "Message.h"

cSceneEventText::cSceneEventText()
{
}

void cSceneEventText::Init()
{
    m_iEventTick = 0;
    m_bEventState = FALSE;
    m_iEventCurScript = 0;
    m_iEventCurPage = 0;
    m_iEventCurLine = 0;
    m_iEventVoiceHandle = -1;

    for (int a = 0; a < MAX_EVENT_LINE; a++)
    {
        ZeroMemory(m_szEventLine[a],sizeof(TCHAR) * MAX_MESSAGE_LENGTH);
    }
}

void cSceneEventText::SkipEventScript()
{
    if( m_iEventCurScript )
    {
        SetEventScript(0);
    }
}



const int EVENT_LINE_SHOW_INTERVAL = 1500;
const int EVENT_PAGE_SHOW_INTERVAL = 1500;
const int EVENT_PANE_APPEAR_INTERVAL = 500;
const int EVENT_PANE_DISAPPEAR_INTERVAL = 500;
const int EVENT_PANE_HEIGHT = 200;
const int EVENT_LINE_HEIGHT = 30;
const int EVENT_LINE_START_X = 40;
const int EVENT_LINE_START_Y = 50;


void cSceneEventText::ProcessEventScript(DWORD dwTick)
{	
    if (m_bEventState)
    {
        // 처음 스르륵
        if (m_iEventCurPage == 0) 
        {
            if (m_iEventAppearPane < EVENT_PANE_APPEAR_INTERVAL)
            {
                m_iEventAppearPane += dwTick;
                if (m_iEventAppearPane > EVENT_PANE_APPEAR_INTERVAL)
                {
                    m_iEventAppearPane = EVENT_PANE_APPEAR_INTERVAL;

                }
                return;
            }
        }

        if (m_iEventCurLine < m_iEventMaxLine)
        {
            m_iEventLineTick += dwTick;
            if (m_iEventLineTick > EVENT_LINE_SHOW_INTERVAL)
            {
                m_iEventLineTick = 0;
                m_iEventCurLine++;
            }
        }
        else
        {
            m_iEventPageTick += dwTick;

            if (m_iEventCurPage < m_iEventMaxPage) 
            {
                if ( m_iEventPageTick > EVENT_PAGE_SHOW_INTERVAL )
                {					
                    // 보이스가 없거나 보이스 플레이가 끝났으면 다음 페이지로 고고
                    if (m_iEventVoiceHandle == -1 || ( (m_iEventVoiceHandle != -1) && wzsnd_getState_voice(m_iEventVoiceHandle) != 1) )
                    {
                        m_iEventCurPage++;
                        m_iEventPageTick = 0;
                        m_iEventLineTick = 0;
                        SetEventPage(m_iEventCurPage);
                    }
                }
            }
            else
            {
                if (m_iEventAppearPane > 0)
                {
                    m_iEventAppearPane -= dwTick;	
                    if (m_iEventAppearPane < 0)
                    {
                        m_iEventAppearPane = 0;
                    }
                }
                else
                {
                    SetEventScript(0);
                }
            }
        }
    }
}


//-------------------------------------------------------------------------------------------
/**
*/
void cSceneEventText::RenderEventScript()
{
	if (m_bEventState)
	{
		POINT pt = g_pApplication->Get2DModeMapping();

		if (m_iEventAppearPane)
		{
			float fPaneRate = (float)m_iEventAppearPane  / (float)EVENT_PANE_APPEAR_INTERVAL;

			BOOL bOld2dMode,bOldRealSize;
			g_pSunRenderer->Check2DMode(&bOld2dMode,&bOldRealSize);
			g_pSunRenderer->Set2DMode( TRUE );

			RECT rc;

			rc.left = 0;
			rc.right = pt.x;
			rc.top = 0;
			rc.bottom = (LONG)((float)EVENT_PANE_HEIGHT * fPaneRate);

			g_InterfaceManager.RenderRect(rc,WzColor_RGBA(0,0,0,255));

			rc.top = pt.y - (LONG)((float)EVENT_PANE_HEIGHT * fPaneRate);
			rc.bottom = pt.y;

			g_InterfaceManager.RenderRect(rc,WzColor_RGBA(0,0,0,255));

			if (fPaneRate >= 1.0f)
			{
#ifdef _JAPAN
				g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m212") );
#else
				g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m216") );
#endif//_JAPAN 일본나레이션에서는 폰트를 약간크게해다라고 요청

				RECT textrc;

				textrc.left = EVENT_LINE_START_X;
				textrc.right = pt.x;
				textrc.top = pt.y - EVENT_PANE_HEIGHT + EVENT_LINE_START_Y;
				textrc.bottom = textrc.top + EVENT_LINE_HEIGHT;

				for (int a = 0; a < m_iEventCurLine; a++)
				{					
					g_pSunRenderer->x_pManagerTextOut->DrawText(m_szEventLine[a],&textrc,WzColor_RGBA(255,255,255,255));

					textrc.top += EVENT_LINE_HEIGHT;
					textrc.bottom += EVENT_LINE_HEIGHT;
				}

				// 마지막라인은 알파로~
				if (m_iEventCurLine < m_iEventMaxLine) 
                {
					float fAlpha = (float)m_iEventLineTick / (float)EVENT_LINE_SHOW_INTERVAL;
					const int fontcolor = (int)(255.0f * fAlpha);
					g_pSunRenderer->x_pManagerTextOut->DrawText(m_szEventLine[m_iEventCurLine],&textrc,WzColor_RGBA(fontcolor,fontcolor,fontcolor,255));
				}

				g_pSunRenderer->x_pManagerTextOut->EndUseFont();
			}

			g_pSunRenderer->Set2DMode(bOld2dMode,bOldRealSize);
		}		
	}
}

void cSceneEventText::SetEventScript(int iScript)
{
	if (m_iEventVoiceHandle != -1)
	{
		g_SoundEffect.StopVoice(m_iEventVoiceHandle);
		m_iEventVoiceHandle = -1;
	}

	if (iScript > 0)
	{
		// 한번 플레이한놈은 다시 안한다.
		if (EventInfoParser::Instance()->IsPlayed(iScript))
		{
			return;
		}

		m_bEventState = TRUE;
		m_iEventCurScript = iScript;
		m_iEventCurPage = 0;
		m_iEventCurLine = 0;
		m_iEventTick = 0;
		m_iEventVoiceHandle = -1;
		m_iEventMaxPage = 0;

		BASE_EventInfo *pInfo = EventInfoParser::Instance()->GetEventInfo(m_iEventCurScript);
		if (pInfo)
		{
			for (int a = 0; a < MAX_EVENT_INFO; a++)
			{
				if (pInfo->m_EventInfo[a].wText != 0)
				{
					m_iEventMaxPage++;					
				}
			}
		}
	
		m_iEventAppearPane = 0;
		m_iEventDisappearPane = 0;
			
		SetEventPage(0);
		g_InterfaceManager.SetRenderState(FALSE);

		EventInfoParser::Instance()->AddPlayedList(iScript);

		keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_invoke_scene_event;
        msg.wParam = eSceneEventSkip_EventScript;
		msg.DoSomething = GlobalFunc::DoInvokeSceneEvent;
        g_KeyQueueManager.PushBack(msg);		
	}
	else
	{
		m_bEventState = FALSE;
		m_iEventCurScript = 0;
		m_iEventCurPage = 0;
		m_iEventCurLine = 0;
		m_iEventTick = 0;
		m_iEventVoiceHandle = -1;
		m_iEventAppearPane = 0;
		m_iEventDisappearPane = 0;

		g_InterfaceManager.SetRenderState(TRUE);	

		g_KeyQueueManager.DeleteMsg(eDoType_invoke_scene_event);
    }
}

void cSceneEventText::SetEventPage(int iPage)
{
	// 페이지가 더이상없으면 끝
	if (iPage < 0 || iPage >= MAX_EVENT_INFO)
	{
		return;
	}
	BASE_EventInfo *pInfo = EventInfoParser::Instance()->GetEventInfo(m_iEventCurScript);

	if (pInfo)
	{		
		if (pInfo->m_EventInfo[iPage].wSound != 0)
		{		
			m_iEventVoiceHandle = g_SoundEffect.PlayVoice(pInfo->m_EventInfo[iPage].wSound,NULL);
		}
		else
		{
			m_iEventVoiceHandle = -1;
		}

		// 마지막 페이지면 끝~
		if (pInfo->m_EventInfo[iPage].wText == 0)
		{		
			return;
		}

		if( MESSAGE_INFO* pMessageInfo = MessageInfoParser::Instance()->GetMessage(pInfo->m_EventInfo[iPage].wText) )
        {
		    const TCHAR seps[] = _T("#");
		    assert(_tcslen(pMessageInfo->pszMessage) < MAX_MESSAGE_LENGTH);
    		
		    TCHAR szBuffer[MAX_MESSAGE_LENGTH + 1] = {0,};
		    StrnCopy(szBuffer,pMessageInfo->pszMessage,MAX_MESSAGE_LENGTH);

		    TCHAR* token = _tcstok(szBuffer, seps);
		    int linecount = 0;
		    while( token != NULL )
		    {      
			    if (linecount >= MAX_EVENT_LINE)
			    {
				    break;
			    }

			    StrnCopy(m_szEventLine[linecount],token,MAX_MESSAGE_LENGTH);
			    token = _tcstok( NULL, seps );
			    linecount++;
    		
		    }

		    m_iEventCurLine = 0;
		    m_iEventMaxLine = linecount;
		    m_iEventPageTick = 0;
		    m_iEventLineTick = 0;	
        }
	}
}