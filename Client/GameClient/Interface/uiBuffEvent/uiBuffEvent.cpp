#include "SunClientPrecompiledHeader.h"

#include "InterfaceManager.h"
#include "uiBuffEvent/uiBuffEvent.h"
#include "uiEventMan/uiEventMan_def.h"
#include "GlobalFunc.h"
#ifdef _NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL
  #include "SunEventInfoParser.h"
#endif //_NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiBuffEvent::m_wzID[eDIALOG_BUFF_EVENT_MAX] = 
{
	StrToWzID("B002"),//eDIALOG_BUFF_EVENT_OK_BTN
	StrToWzID("B001"),//eDIALOG_BUFF_EVENT_EXIT_BTN
#ifdef _NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL
    StrToWzID("ST02"),//eDIALOG_BUFF_EVENT_STATIC_LEVELRANGE
    StrToWzID("ST08"),//eDIALOG_BUFF_EVENT_STATIC_LEVELRANGE_PCBANG
#endif //_NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiBuffEvent::uiBuffEvent(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiBuffEvent::~uiBuffEvent()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiBuffEvent::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiBuffEvent::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiBuffEvent::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiBuffEvent::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiBuffEvent::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_bShow = TRUE;

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_Dialog_Buffer_Event_Dlg;
        msg.wParam = IM_EVENT_MANAGER::EVENT_BUFF;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);

#ifdef _NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL
        SetTextControl();
#endif //_NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL
	}
	else
	{
		m_bShow   = FALSE;

        g_KeyQueueManager.DeleteMsg(eDoType_Dialog_Buffer_Event_Dlg);

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiBuffEvent::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_BUFF_EVENT_OK_BTN:
				{
					MSG_CG_EVENT_BUFF_EVENT_SYN SendMsg;
					SendMsg.m_NpcCode = eNPC_TYPE_EVENT_BUFF;
					GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));

					OnShowWindow(FALSE);
				}
				break;

			case eDIALOG_BUFF_EVENT_EXIT_BTN:
				{
					OnShowWindow(FALSE);
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

void	uiBuffEvent::InitControl()
{
	m_pBtnOk = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_BUFF_EVENT_OK_BTN));
	assert(m_pBtnOk);
}


#ifdef _NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL
void uiBuffEvent::SetTextControl()
{
    const int buff_ctystal_type = 22;
    EVENT_INFO_VEC* event_info_vector = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(buff_ctystal_type);
    if (event_info_vector == NULL)
    {
        return;
    }

    EVENT_INFO_VEC::iterator begin_iter = event_info_vector->begin();
    EVENT_INFO_VEC::iterator end_iter = event_info_vector->end();
    for (; begin_iter < end_iter; ++begin_iter)
    {
        EVENT_INFO* event_info = (*begin_iter);

        if (event_info == NULL)
        {
            continue;
        }

        if (event_info->CheckEvent())
        {
            SYSTEMTIME system_time;
            util::TimeSync::GetLocalTime(&system_time);

            WORD time = (system_time.wHour*100) + (system_time.wMinute);

            if (time >= event_info->m_wPeakTimeStart &&
                time <= event_info->m_wPeakTimeEnd)
            {
                //이벤트 타임이라면

                TCHAR min_level_string[5] = {0,};
                TCHAR max_level_string[5] = {0,};
                _itot(event_info->m_byApplyToLvRng[EVENT_INFO::LEVEL_MIN], min_level_string, 10);
                _itot(event_info->m_byApplyToLvRng[EVENT_INFO::LEVEL_MAX], max_level_string, 10);
                
                // 1689
                TCHAR temp_string[INTERFACE_STRING_LENGTH];
                g_InterfaceManager.GetInterfaceStringFormat(temp_string, INTERFACE_STRING_LENGTH,
                    1689, min_level_string, max_level_string);

                CCtrlStaticWZ* static_control = NULL;
                static_control = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_BUFF_EVENT_STATIC_LEVELRANGE));
                if (static_control)
                {
                    static_control->SetTextWZ(temp_string);
                }
                static_control = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_BUFF_EVENT_STATIC_LEVELRANGE_PCBANG));
                if (static_control)
                {
                    static_control->SetTextWZ(temp_string);
                }
                return;
            }
        }
    }
}
#endif //_NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
