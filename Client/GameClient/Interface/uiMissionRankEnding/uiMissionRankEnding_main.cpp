#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM

#include "uiMissionRankEnding.h"
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
WzID uiMissionRankEnding::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("ST00"),	//진행시간	//DLGPOS_MRE_TXT_TIME
		StrToWzID("ST01"),	//미션랭킹	//DLGPOS_MRE_TXT_RANK	
		StrToWzID("ST03"),	//파티장	//DLGPOS_MRE_TXT_MASTER	
		StrToWzID("ST02"),	//파티번호	//DLGPOS_MRE_TXT_NUMBER


		StrToWzID("ST05"),	//미션종료시간		//DLGPOS_MRE_TXT_CLEAR_TIME,
		StrToWzID("ST06"),	//몬스터단축시간	//DLGPOS_MRE_TXT_MON_DEC_TIME,
		StrToWzID("ST07"),	//일반몬스터킬수	//DLGPOS_MRE_TXT_GEN_MON_KILL,
		StrToWzID("ST08"),	//일반몬스터단축초	//DLGPOS_MRE_TXT_GEN_MON_TIME,
		StrToWzID("ST09"),	//리더몬스터킬수	//DLGPOS_MRE_TXT_LEADER_MON_KILL,
		StrToWzID("ST10"),	//리더몬스터단축초	//DLGPOS_MRE_TXT_LEADER_MON_TIME,
		StrToWzID("ST11"),	//최종클리어시간	//DLGPOS_MRE_TXT_TOTAL_CLEAR_TIME,

		StrToWzID("BT00"), //닫기 버튼

};

//------------------------------------------------------------------------------
/**
*/
uiMissionRankEnding::uiMissionRankEnding(InterfaceManager *pUIMan) :
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
}

//------------------------------------------------------------------------------
/**
*/
uiMissionRankEnding::~uiMissionRankEnding()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankEnding::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankEnding::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankEnding::InitControls()
{
    for(int i=0; i<POS_TXT_SIZE; ++i)
	{
		this->m_pTxtControl[i] = NULL;
		m_pTxtControl[i] = static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MRE_TXT_TIME+i));
	}

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiMissionRankEnding::OnShowWindow( BOOL val )
{
    if (val)
    {
        //uiBase::MoveCenter();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
		_setData();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_mission_rank_ending;
		msg.wParam = InterfaceManager::DIALOG_MISSION_RANK_ENDING;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_mission_rank_ending);
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiMissionRankEnding::_isEnable()
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
void uiMissionRankEnding::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	switch( pMessage->eResultMsg ) 
    {
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
				case DLGPOS_MRE_BTN_CLOSE:	// 닫기버튼
					{
						GetManager()->CloseEndingDlg();
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
void uiMissionRankEnding::_setData()
{
	if(GetManager())
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
		
		util::_tstring strTime;

		if(GetManager()->m_bMissionCleared)
		{
			if(m_pTxtControl[DLGPOS_MRE_TXT_TIME])
			{
				strTime.clear();
				GetManager()->GetTimeString(GetManager()->m_dwMissionClearTime, strTime);	//최종결과시간!

				g_InterfaceManager.GetInterfaceString( 9915, szTemp, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szTemp, strTime.c_str());
				GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_TIME], szMessage );
			}

			if(m_pTxtControl[DLGPOS_MRE_TXT_RANK])
			{
				g_InterfaceManager.GetInterfaceString( 9908, szTemp, INTERFACE_STRING_LENGTH );	// 미션랭킹 %d위
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_iMyRank);
				GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_RANK], szMessage );
			}

			if(m_pTxtControl[DLGPOS_MRE_TXT_MASTER])
			{
				g_InterfaceManager.GetInterfaceString( 9914, szTemp, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_strMyPartyMasterName.c_str());
				GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_MASTER], szMessage );
			}

			if(m_pTxtControl[DLGPOS_MRE_TXT_NUMBER])
			{
				g_InterfaceManager.GetInterfaceString( 9913, szTemp, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_iMyPartyNum);
				GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_NUMBER], szMessage );
			}


			///////////////새로 추가된 내용
			if(m_pTxtControl[DLGPOS_MRE_TXT_CLEAR_TIME])	//5494	미션 종료 시간 : %s (몬스터 kill 수 빼기 전 시간)
			{
				strTime.clear();
				
				GetManager()->GetTimeString(GetManager()->m_dwResultMissionClear, strTime);

				g_InterfaceManager.GetInterfaceString( 5494, szTemp, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szTemp, strTime.c_str());
				GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_CLEAR_TIME], szMessage );
			}

#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
            if(m_pTxtControl[DLGPOS_MRE_TXT_MON_DEC_TIME])	//5495	몬스터 단축 시간 : %s
			{                
                strTime.clear();
                GetManager()->GetTimeString(GetManager()->m_dwTotalMonsterDecTimeSecond, strTime);

				g_InterfaceManager.GetInterfaceString( 5495, szTemp, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szTemp, strTime.c_str());
				GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_MON_DEC_TIME], szMessage );
			}
#else
			if(m_pTxtControl[DLGPOS_MRE_TXT_MON_DEC_TIME])	//5495	몬스터 단축 시간 : %s
			{                
				strTime.clear();
				GetManager()->GetTimeString(GetManager()->m_dwTotalMonsterDecTimeSecond * 1000, strTime);
                                

				g_InterfaceManager.GetInterfaceString( 5495, szTemp, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szTemp, strTime.c_str());
				GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_MON_DEC_TIME], szMessage );
			}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

			if(m_pTxtControl[DLGPOS_MRE_TXT_GEN_MON_KILL])	//5496	일반 몬스터 킬수 : %d 마리
			{
				g_InterfaceManager.GetInterfaceString( 5496, szTemp, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_dwMonsterKillNum_General);
				GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_GEN_MON_KILL], szMessage );
			}

#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)            
            if (m_pTxtControl[DLGPOS_MRE_TXT_GEN_MON_TIME])     //5516  일반 몬스터 단축초 : %s
            {
                strTime.clear();
                GetManager()->GetTimeString(GetManager()->m_dwGeneralMonsterDecSecond, strTime);

                g_InterfaceManager.GetInterfaceString( 5516, szTemp, INTERFACE_STRING_LENGTH );                
                Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, strTime.c_str());
                GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_GEN_MON_TIME], szMessage );
            }
#else
			if(m_pTxtControl[DLGPOS_MRE_TXT_GEN_MON_TIME])	//5497	일반 몬스터 단축초 : -%d 초            
			{
				g_InterfaceManager.GetInterfaceString( 5497, szTemp, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_dwGeneralMonsterDecSecond);                
				GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_GEN_MON_TIME], szMessage );
			}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

			if(m_pTxtControl[DLGPOS_MRE_TXT_LEADER_MON_KILL])	//5498	리더 몬스터 킬수 : %d 마리
			{
				g_InterfaceManager.GetInterfaceString( 5498, szTemp, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_dwMonsterKillNum_Leader);
				GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_LEADER_MON_KILL], szMessage );
			}

#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)            
            if (m_pTxtControl[DLGPOS_MRE_TXT_LEADER_MON_TIME]) // 5517  리더 몬스터 단축초 : -%s
            {
                strTime.clear();
                GetManager()->GetTimeString(GetManager()->m_dwLeaderMonsterDecSecond, strTime);

                g_InterfaceManager.GetInterfaceString( 5517, szTemp, INTERFACE_STRING_LENGTH );
                Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, strTime.c_str());
                GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_LEADER_MON_TIME], szMessage );
            }
#else
			if(m_pTxtControl[DLGPOS_MRE_TXT_LEADER_MON_TIME])	//5499	리더 몬스터 단축초 : -%d 초            
			{
                strTime.clear();
                GetManager()->GetTimeString(GetManager()->m_dwLeaderMonsterDecSecond, strTime);

				g_InterfaceManager.GetInterfaceString( 5499, szTemp, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_dwLeaderMonsterDecSecond);                
				GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_LEADER_MON_TIME], szMessage );
			}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

			if(m_pTxtControl[DLGPOS_MRE_TXT_TOTAL_CLEAR_TIME])	//5500	최종 클리어 시간 : %s
			{
				strTime.clear();
				GetManager()->GetTimeString(GetManager()->m_dwMissionClearTime, strTime);
				
				g_InterfaceManager.GetInterfaceString( 5500, szTemp, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szTemp, strTime.c_str());
				GlobalFunc::SetCaption( m_pTxtControl[DLGPOS_MRE_TXT_TOTAL_CLEAR_TIME], szMessage );
			}
		}
	}
}
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

