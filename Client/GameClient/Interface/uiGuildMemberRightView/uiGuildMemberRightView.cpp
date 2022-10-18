#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMemberRightView.h"
#include "GuildRightInfoParser.h"
#include "uiGuildMan/uiGuildMan.h"

WzID uiGuildMemberRightView::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("CT00"),		//DLGPOS_CHECK_1,
	StrToWzID("CT12"),		//DLGPOS_CHECK_2,
	StrToWzID("CT01"),		//DLGPOS_CHECK_3,
	StrToWzID("CT13"),		//DLGPOS_CHECK_4,
	StrToWzID("CT02"),		//DLGPOS_CHECK_5,
	StrToWzID("CT14"),		//DLGPOS_CHECK_6,
	StrToWzID("CT03"),		//DLGPOS_CHECK_7,
	StrToWzID("CT15"),		//DLGPOS_CHECK_8,
	StrToWzID("CT04"),		//DLGPOS_CHECK_9,
	StrToWzID("CT16"),		//DLGPOS_CHECK_10,
	StrToWzID("CT05"),		//DLGPOS_CHECK_11,
	StrToWzID("CT17"),		//DLGPOS_CHECK_12,
	StrToWzID("CT06"),		//DLGPOS_CHECK_13,
	StrToWzID("CT18"),		//DLGPOS_CHECK_14,
	StrToWzID("CT07"),		//DLGPOS_CHECK_15,
	StrToWzID("CT19"),		//DLGPOS_CHECK_16,
	StrToWzID("CT08"),		//DLGPOS_CHECK_17,
	StrToWzID("CT20"),		//DLGPOS_CHECK_18,
	StrToWzID("CT09"),		//DLGPOS_CHECK_19,
	StrToWzID("CT21"),		//DLGPOS_CHECK_20,
	StrToWzID("CT10"),		//DLGPOS_CHECK_21,
	StrToWzID("CT22"),		//DLGPOS_CHECK_22,

	StrToWzID("ST14"),		//DLGPOS_TXT_1,
	StrToWzID("ST26"),		//DLGPOS_TXT_2,
	StrToWzID("ST15"),		//DLGPOS_TXT_3,
	StrToWzID("ST27"),		//DLGPOS_TXT_4,
	StrToWzID("ST16"),		//DLGPOS_TXT_5,
	StrToWzID("ST28"),		//DLGPOS_TXT_6,
	StrToWzID("ST17"),		//DLGPOS_TXT_7,
	StrToWzID("ST29"),		//DLGPOS_TXT_8,
	StrToWzID("ST18"),		//DLGPOS_TXT_9,
	StrToWzID("ST30"),		//DLGPOS_TXT_10,
	StrToWzID("ST19"),		//DLGPOS_TXT_11,
	StrToWzID("ST31"),		//DLGPOS_TXT_12,
	StrToWzID("ST20"),		//DLGPOS_TXT_13,
	StrToWzID("ST32"),		//DLGPOS_TXT_14,
	StrToWzID("ST21"),		//DLGPOS_TXT_15,
	StrToWzID("ST33"),		//DLGPOS_TXT_16,
	StrToWzID("ST22"),		//DLGPOS_TXT_17,
	StrToWzID("ST34"),		//DLGPOS_TXT_18,
	StrToWzID("ST23"),		//DLGPOS_TXT_19,
	StrToWzID("ST35"),		//DLGPOS_TXT_20,
	StrToWzID("ST24"),		//DLGPOS_TXT_21,
	StrToWzID("ST36"),		//DLGPOS_TXT_22,
	StrToWzID("B101"),	//DLGPOS_BTN_OK,
	StrToWzID("BT00"),	//DLGPOS_BTN_EXIT,
	StrToWzID("BT02"),		//DLGPOS_BTN_LEFT,
	StrToWzID("BT01"),		//DLGPOS_BTN_RIGHT,
	StrToWzID("ST02"),		//DLGPOS_TXT_PAGE,
};
//------------------------------------------------------------------------------
/**
*/
uiGuildMemberRightView::uiGuildMemberRightView(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_pGuildMan = (uiGuildMan *)GetUIMan()->GetUserInterfaceManager(UIMAN_GUILD);
	assert(m_pGuildMan);

}
//------------------------------------------------------------------------------
/**
*/
uiGuildMemberRightView::~uiGuildMemberRightView()
{
	m_vecTotalRight.clear();
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMemberRightView::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	m_pTextPage = (CCtrlStaticWZ *) getControl(DLGPOS_TXT_PAGE);
	assert(m_pTextPage);

	for(int i =0; i < MAX_GUILD_RIGHT_VIEW_ITEM; ++i)
	{
		m_CheckState[i].pCheck = (CCtrlButtonCheckWZ *) getControl(DLGPOS_CHECK_1 +i);
		assert(m_CheckState[i].pCheck);
		m_CheckState[i].pStatic = (CCtrlStaticWZ *) getControl(DLGPOS_TXT_1 +i);
		assert(m_CheckState[i].pStatic);
	}
	m_iCurPage = 0;
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMemberRightView::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMemberRightView::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMemberRightView::OnShowWindow(BOOL val)
{
	if( val )
	{

	}
	else
	{
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMemberRightView::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg)
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ))
			{
			case DLGPOS_BTN_EXIT:
			case DLGPOS_BTN_OK:
				{
					ShowInterface(FALSE);
				}
				break;

			case DLGPOS_BTN_LEFT:
				{
					if(m_iCurPage > 0)
					{
						m_iCurPage--;
						RefreshItem();
					}
				}
				break;

			case DLGPOS_BTN_RIGHT:
				{
					if( static_cast<size_t>((m_iCurPage +1) * MAX_GUILD_RIGHT_VIEW_ITEM) < m_vecTotalRight.size() )
					{
						m_iCurPage++;
						RefreshItem();
					}
				}
				break;
			}

		}
		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildMemberRightView::NetworkProc(MSG_BASE * pMsg)
{

}



void uiGuildMemberRightView::RegisteAllItem(stGUILD_MEMBER_INFO * pMember, GUILD_RIGHT_USE_INFO * pRightUseInfo)
{
    // 권한에 대한 모든 데이타를 수집한다.
    if( !pMember)
        return;

    m_vecTotalRight.clear();
    std::map<eGUILD_RIGHTS, bool> right_state_map;

    if (pRightUseInfo != NULL)
    {
        for (int index = 0; index < MAX_GUILD_DELEGATE_RIGHT; ++index)
        {
            if (pRightUseInfo[index].m_Right != 0)
            {
                right_state_map.insert(std::make_pair<eGUILD_RIGHTS, bool>(pRightUseInfo->m_Right, 
                    pRightUseInfo->m_bRightUse));
            }
        }
    }    

    SetCaptionTargetName(pMember->m_tszCharName);

    GUILD_RIGHT_INFO_MAP * pRightInfoMap = GuildRightInfoParser::Instance()->GetGuildRightInfoMap();
    GUILD_RIGHT_INFO_MAP_ITER itr;
    std::map<eGUILD_RIGHTS, bool>::iterator found_itr;
    std::map<eGUILD_RIGHTS, bool>::iterator end_itr = right_state_map.end();

    for(itr = pRightInfoMap->begin(); itr != pRightInfoMap->end(); itr++)
    {
        GuildRightInfo * pInfo = itr->second;
        if( pInfo )
        {
            sITEM_DATA sData;

            found_itr = right_state_map.find(pInfo->m_GuildRight);
            if (found_itr != end_itr)
            {
                // 서버로부터 받은 권한정보를 CheckState 세팅
                sData.bCheck = found_itr->second;
            }
            else
            {
                // 서버로 부터 받지 않은 정보는 스크립트 설정대로
                BOOL right_by_grade = 
                    GuildRightInfoParser::Instance()->CheckGradeForRight(
                        pInfo->m_GuildRight, GetManager()->GetGuildInfo().m_GuildGrade);
                BOOL right_by_duty = 
                    GuildRightInfoParser::Instance()->CheckDutyForRight(
                    pInfo->m_GuildRight, pMember->m_eGuildDuty);

                sData.bCheck = (FALSE != (right_by_grade && right_by_duty));
            }
            sData.dwStringCode = pInfo->m_dwStringCode;
            m_vecTotalRight.push_back(sData);
        }
    }

    m_iCurPage = 0;
    RefreshItem();
}

void uiGuildMemberRightView::RefreshItem()
{

	for( int i =0; i < MAX_GUILD_RIGHT_VIEW_ITEM; ++i)
	{
		m_CheckState[i].pCheck->ShowWindowWZ(WZ_HIDE);
		m_CheckState[i].pStatic->ShowWindowWZ(WZ_HIDE);

		m_CheckState[i].pCheck->EnableWindowWZ(false);
	}

	int startLoopCount = m_iCurPage * MAX_GUILD_RIGHT_VIEW_ITEM;
	int endLoopCount =  static_cast<int>(min((int)m_vecTotalRight.size() , ((m_iCurPage + 1) * MAX_GUILD_RIGHT_VIEW_ITEM)));

	int count =0;
	for( int i = startLoopCount; i < endLoopCount; ++i)
	{
		if( count >= MAX_GUILD_RIGHT_VIEW_ITEM)
			return;

		m_CheckState[count].pCheck->ShowWindowWZ(WZ_SHOW);
		m_CheckState[count].pStatic->ShowWindowWZ(WZ_SHOW);
		m_CheckState[count].pCheck->SetCheckState(m_vecTotalRight[i].bCheck);
		GlobalFunc::SetCaption(m_CheckState[count].pStatic , (LPTSTR)g_InterfaceManager.GetInterfaceString(m_vecTotalRight[i].dwStringCode));
		count++;
	}

    TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
    TCHAR szPage[INTERFACE_STRING_LENGTH] ={0, };
    //5796 %d페이지
    g_InterfaceManager.GetInterfaceString(5796, szPage);
    Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, szPage, m_iCurPage +1);
	GlobalFunc::SetCaption(m_pTextPage, szMessage);
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
