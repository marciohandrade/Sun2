//------------------------------------------------------------------------------
//  uiBattleZoneReadyMissionConfig_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
#include "uiBattleZoneReadyMissionConfig.h"
#include "BattleLobby.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "MapInfoParser.h"
#include "globalfunc.h"
#include "interfacemanager.h"
#include "uiChatMan/uiChatMan.h"
#include "ChatDialog.h"

//------------------------------------------------------------------------------
WzID uiBattleZoneReadyMissionConfig::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("C001"),	//DLGPOS_CHK_MAIN_MISSION
    StrToWzID("C002"),	//DLGPOS_CHK_SUB_MISSION,
    StrToWzID("B001"),	//DLGPOS_BTN_SUB_MISSION_UP,
    StrToWzID("B002"),	//DLGPOS_BTN_SUB_MISSION_DOWN,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiBattleZoneReadyMissionConfig::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->CheckControls())
            return;

		//706	서브미션 선택
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		g_InterfaceManager.GetInterfaceString(706, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::SetCaption (m_pChkControls[1], szMessage);

        //UpdatePosition();
    }
    else
    {
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyMissionConfig::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    if (!this->GetManager())
        return;

	if(!this->GetManager()->GetBattleLobby())
		return;

	if (!this->GetManager()->IsMaster())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_CHK_MAIN_MISSION:
                {
                    if (this->GetManager()->IsReadyToModifyMissionCfg())
                    {
                        this->SelectMainMission();

                        if (!GENERALPARAM->IsTestUIScene())
                            this->GetManager()->ModifyMap(0);
                    }
                }
                break;

            case DLGPOS_CHK_SUB_MISSION:
                {
                    if (this->GetManager()->IsReadyToModifyMissionCfg())
                    {
						m_byCurSubMission = GetManager()->GetSubMissionUp(1);
						if(m_byCurSubMission == 0)
							return;

                        if (!GENERALPARAM->IsTestUIScene())
                            this->GetManager()->ModifyMap(m_byCurSubMission);

                        this->SelectSubMission();
                    }
                }
                break;

            case DLGPOS_BTN_SUB_MISSION_UP:
                {
                    this->DoSubMissionUP();
                }
                break;

            case DLGPOS_BTN_SUB_MISSION_DOWN:
                {
                    this->DoSubMissionDOWN();
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

////------------------------------------------------------------------------------
///**
//*/
//void     
//uiBattleZoneReadyMissionConfig::UpdatePosition()
//{
//    //MoveWindowWZ(748, 298);
//}
//
//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyMissionConfig::OnUpdateSolarDialog()
{
    if (!this->CheckControls())
        return;

    if (!this->GetManager())
        return;

	if(!this->GetManager()->GetBattleLobby())
		return;

	if (!this->GetManager()->IsMaster())
    {
        m_pBtnControls[0]->ShowWindowWZ(WZ_HIDE);
        m_pBtnControls[1]->ShowWindowWZ(WZ_HIDE);
    }
	else
	{
		m_pBtnControls[0]->ShowWindowWZ(WZ_SHOW);
        m_pBtnControls[1]->ShowWindowWZ(WZ_SHOW);
	}

    if (this->GetManager()->IsReadyToModifyMissionCfg())
    {
        // 서브미션이 선택되었을 때만 휠 적용토록
        if (m_pChkControls[1]->GetCheckState())
        {
			RECT rc = {0,0,0,0};
			ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
			if (pChatDlg)
			{
				rc = pChatDlg->GetListRect();
				rc.right += rc.left;
				rc.bottom += rc.top;
			}
			if ( this->GetMouseWheelStatus(&rc) == 0 )
			{
				switch (g_Input.GetMouseWheel())
				{
				default:
					break;
				case MOUSE_WHEEL_UP:
					{
						this->DoSubMissionUP();
					}
					break;

				case MOUSE_WHEEL_DOWN:
					{
						this->DoSubMissionDOWN();
					}
					break;
				}
			}            
        }
    }

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szFinal[INTERFACE_STRING_LENGTH] = {0,};

    int i = 0;

	const MapGroup * pGroupInfo	=  this->GetManager()->GetBattleRoomEntry().GetCurGroup();

	assert (pGroupInfo);
    if ( !pGroupInfo)
        return;

    //
    //  Mtype
    //  0:  메인 미션
    //  1;  서브 미션 1
    //  |
    //  |
    //  n:  서브 미션 n
    //

    sMAPINFO_HASH & rMapInfoHash = const_cast<MapGroup *>(pGroupInfo)->GetMapInfoHash();

	sMAPINFO_HASH::const_iterator itr = rMapInfoHash.begin();
	const sMAPINFO * pMapInfo = itr->second;

    assert (pMapInfo);
    if (!pMapInfo)
        return;

	// 메인 미션 이름 얻어오기
	const sMAPINFO * pMainMission = ((MapGroup *)pGroupInfo)->GetMapInfoIndex(0);
	if ( !pMainMission )
	{
		// 708	비공개 메인미션
		g_InterfaceManager.GetInterfaceString(708, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::SetCaption( m_pChkControls[0] , szMessage);
		m_pChkControls[0]->SetTextColor(m_dwTextColorMainMission);
	}	
	else
	{
		//영주 수정
		g_InterfaceManager.GetInterfaceString(pMainMission->dwNCode, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::SetCaption(m_pChkControls[0] , szMessage );
		m_pChkControls[0]->SetTextColor(m_dwTextColorMainMission);
	}

    int iFieldIndex = 0;

    if (GENERALPARAM->IsTestUIScene())
	{
        iFieldIndex = m_byCurSubMission;
	}
	else
	{
		iFieldIndex = this->GetManager()->GetBattleRoomEntry().GetCurMapInfo()->byMType;
	}

    if (iFieldIndex)
	{
		// 서브 미션
		SelectSubMission();
        m_byCurSubMission = iFieldIndex;
	}
    else
	{
		g_InterfaceManager.GetInterfaceString(pMapInfo->dwNCode, szMessage, INTERFACE_STRING_LENGTH);
		g_InterfaceManager.GetInterfaceString(971, szTemp, INTERFACE_STRING_LENGTH);
		
		Snprintf (szFinal, INTERFACE_STRING_LENGTH-1, _T("%s %s"), szMessage, szTemp);
		GlobalFunc::SetCaption(m_pChkControls[1] , szFinal );

		// 메인 미션
		SelectMainMission();
	}


	m_pChkControls[0]->SetTextColor(m_dwTextColorMainMission);
	m_pChkControls[1]->SetTextColor(m_dwTextColorSubMission);

	const sMAPINFO * _pMapInfo = MapInfoParser::Instance()->FindMapInfo( static_cast<MAPCODE>( this->GetManager()->GetBattleRoomEntry().GetMapCode() ) );

    if ( _pMapInfo )
    {
        MapGroup * _pMapGroup = MapInfoParser::Instance()->GetMapGroup( _pMapInfo->MapKind );
        if ( _pMapGroup ) 
        {
			sMAPINFO * _pMapInfo2 = _pMapGroup->GetMapInfoIndex(m_byCurSubMission);
            if (_pMapInfo2) 
            {
                // 미션 이름 얻어오기
                g_InterfaceManager.GetInterfaceString(_pMapInfo2->dwNCode, szMessage, INTERFACE_STRING_LENGTH);
                // 서브 미션이므로 미션이름뒤에 숫자 추가
				if (iFieldIndex == 0)
				{
					// 1655	%s 서브미션
					g_InterfaceManager.GetInterfaceString(1655, szTemp, INTERFACE_STRING_LENGTH);
					Snprintf (szFinal, INTERFACE_STRING_LENGTH-1, szTemp, szMessage);
				}
				else
				{
					Snprintf (szFinal, INTERFACE_STRING_LENGTH-1, _T("%s"), szMessage);
				}
				GlobalFunc::SetCaption(m_pChkControls[1] , szFinal );
                m_pChkControls[1]->SetTextColor(m_dwTextColorSubMission);
            }
        }

    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
#endif // !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
