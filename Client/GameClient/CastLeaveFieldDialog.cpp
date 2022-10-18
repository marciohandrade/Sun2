#include "SunClientPrecompiledHeader.h"
#include "castleavefielddialog.h"
#include "HeroActionInput.h"
#include "BattleScene.h"
#include "SystemDialog.h"
#include "ItemManager.h"
#include "Application.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "LotteryInventoryDialog.h"
#include <ssq/SSQInfoStruct.h>
#include "Hero.h"
#include "uiSystemMan/uiSystemMan.h"

#include "uiDominationTowerMan/uiDominationTowerMan.h"
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "ChaosSystem/ChaosSystemPacketFunction.h"
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP

#include "uiPersonalWayPointWindow/uiPersonalWayPointWindow.h"

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
  #include "SCItemSlot.h"
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

WzID CastLeaveFieldDialog::m_wzId[] = 
{
	StrToWzID("P000"),	
	StrToWzID("T002"), // CAST_TXT_REMAIN_SEC
	StrToWzID("T001"), //eCAST_LEAVE_TXT_DESC
};


void StartLeaveFieldCastTimer( eDoType eType, DWORD dwTime )
{
	CastLeaveFieldDialog * pDlg = 
		(CastLeaveFieldDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_DELAY_CASTING);
	if ( pDlg )
	{
        if (pDlg->GetDoType() != eDoType_MAX)
        {
            pDlg->CancleCast();
        }

		pDlg->SetDoType( eType );
		pDlg->SetDelayTimer(dwTime);	
		pDlg->ShowWindow(TRUE);

        if (pDlg->CheckToLockBydoType())
        {
            g_InterfaceManager.LockDlgs(pDlg->GetDialogWZ()->GetDlgID());
        }
	}
}


bool CastLeaveFieldDialog::CheckToLockBydoType()
{
    if ((m_eDoType == eDoType_ServerSelectStandingBy) || 
        (m_eDoType == eDoType_CharacterSelectStandingBy) || 
        (m_eDoType == eDoType_Delay_Use_Return_Stone) || 
        (m_eDoType == eDoType_Delay_Leave_Field) || 
        (m_eDoType == eDoType_Delay_Leave_Domination)
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
         || (m_eDoType == eDoType_Delay_SpaWarp_Dialog)
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
        )
    {
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------------------------
/**
*/
CastLeaveFieldDialog::CastLeaveFieldDialog(void) : m_pRemainTimer(NULL), m_eDoType(eDoType_MAX)
{
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	m_pServerSelectTimer = NULL;
#endif
}

//-------------------------------------------------------------------------------------------
/**
*/
CastLeaveFieldDialog::~CastLeaveFieldDialog(void)
{
}


//-------------------------------------------------------------------------------------------
/**
*/
VOID 
CastLeaveFieldDialog::Init( CDrawBase * pDrawBase )
{
    m_fGaugeSize		 = (float)getControl(eCAST_LEAVE_PIC_GAUGE)->GetSize().right;
	m_pRemainTextControl = (CCtrlStaticWZ *)getControl(eCAST_LEAVE_TXT_REMAIN_SEC);
	m_pDescText = (CCtrlStaticWZ *)getControl(eCAST_LEAVE_TXT_DESC);
	assert(m_pDescText);

	if(NULL == m_pRemainTextControl || NULL == m_pDescText)
	{
		return;
	}

	m_pRemainTextControl->SetTextWZ(" ");
	m_pDescText->SetTextWZ(" ");

    SetGage(0);

	m_pRemainTimer		= new util::Timer;
	assert(m_pRemainTimer);
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	m_pServerSelectTimer = new util::Timer;
	assert(m_pServerSelectTimer);
#endif
}


//-------------------------------------------------------------------------------------------
/**
*/
VOID 
CastLeaveFieldDialog::Release()
{
	SAFE_DELETE( m_pRemainTimer );
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	SAFE_DELETE( m_pServerSelectTimer );
#endif
}


//-------------------------------------------------------------------------------------------
/**
*/
VOID 
CastLeaveFieldDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch ( pMessage->eResultMsg )
	{
	case 0:
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}


//-------------------------------------------------------------------------------------------
/**
*/
VOID 
CastLeaveFieldDialog::NetworkProc( MSG_BASE * pMsg )
{

}


//-------------------------------------------------------------------------------------------
/**
*/
CControlWZ * 
CastLeaveFieldDialog::getControl( POSTYPE AtPos )
{
	if( AtPos < Dialog_Max )
	{
		return GetControlWZ( m_wzId[AtPos] );
	}
	else
	{
		return NULL;
	}
}


//-------------------------------------------------------------------------------------------
/**
*/
POSTYPE 
CastLeaveFieldDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Dialog_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Dialog_Max;
}

//-------------------------------------------------------------------------------------------
/**
*/
VOID 
CastLeaveFieldDialog::SetGage( float fHPPercent )
{
	if(100.f < fHPPercent)
	{
#ifdef _DEV_VER
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "[Warning]CastLeaveFieldDialog::HPPercent = %f", fHPPercent);
#endif 
		fHPPercent = 100.f;
	}

    if (fHPPercent >= 0.0f )
    {
        float width = (m_fGaugeSize/100.f) * fHPPercent;
        getControl(eCAST_LEAVE_PIC_GAUGE)->SetSizeWidth((long)width);
    }

}

//-------------------------------------------------------------------------------------------
/**
*/
void			
CastLeaveFieldDialog::SetDelayTimer( DWORD dwTimetick )
{
	if ( this->m_pRemainTimer )
	{
		this->m_pRemainTimer->SetTimer( dwTimetick );
		
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
		// 서버선택 최대 대기시간 15초
		if (m_pServerSelectTimer && eDoType_ServerSelectStandingBy == m_eDoType)
		{
			dwTimetick += 5000;
			m_pServerSelectTimer->SetTimer(dwTimetick);
		}
#endif
		g_HeroInput.SetMoveDelay( dwTimetick );
	}
}

//-------------------------------------------------------------------------------------------
/** 캐스팅 취소 
*/
void			
CastLeaveFieldDialog::CancleCast()
{
	if ( this->m_pRemainTimer )
	{
		this->m_pRemainTimer->DisableCheckTime();
		g_HeroInput.SetMoveDelay( 0 );
	}
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	if (m_pServerSelectTimer)
	{
		m_pServerSelectTimer->DisableCheckTime();
	}	
#endif

   if (CheckToLockBydoType())
    {
        g_InterfaceManager.UnLockDlgs();
    }

	g_KeyQueueManager.DeleteMsg( m_eDoType );

    m_eDoType = eDoType_MAX;
}


//-------------------------------------------------------------------------------------------
/**
*/
VOID 
CastLeaveFieldDialog::OnShowWindow( BOOL val )
{
	if (val)
	{
		UpdatePosition();
	}
	else
	{
		if ((m_eDoType == eDoType_Delay_Use_Return_Stone)
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
            || (m_eDoType == eDoType_Delay_SpaWarp_Dialog)
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
            )
		{
			ItemManager::Instance()->SetItemLock(ItemManager::Instance()->GetPendingUseItemSlotIdx(), 
				ItemManager::Instance()->GetPendingUseItemPos(), false); 
		}

        // 창이 닫히면 자동적으로 캐스팅을 취소한다.
        CancleCast();
	}
}

bool CastLeaveFieldDialog::IsCancleDealyCast()
{
    bool is_cancle_cast = false;
    if ((g_pHero != NULL) && 
        (g_pHero->IsDead() == TRUE))
    {
        switch (m_eDoType)
        {
        case eDoType_Delay_Leave_Domination:
        case eDoType_Delay_Spawn_Village:
        case eDoType_Delay_Spawn_DominationWar:
        case eDoType_ServerSelectStandingBy:
        case eDoType_CharacterSelectStandingBy:
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        case eDoType_Delay_Spawn_BattleGround:
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
        case eDoType_Delay_Spawn_FreePVP:
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_000000_20120527_CHAOS_ZONE
        case eDoType_Delay_Leave_ChaosZone:
#endif //_NA_000000_20120527_CHAOS_ZONE
		case eDoType_PersonalWarp:  
		case eDoType_GeneralWarp:
#ifdef _NA_008334_20150608_SONNENSCHEIN
        case eDoType_Delay_Spawn_SonnenSchein:
#endif //_NA_008334_20150608_SONNENSCHEIN
            {
                is_cancle_cast = false; 
            }
            break;
        default:
            {
                is_cancle_cast = true;
            }
        }   
    }
    return is_cancle_cast;
}

//-------------------------------------------------------------------------------------------
/**
*/
VOID			
CastLeaveFieldDialog::updateCheckExpireCast()
{
	if (!m_pRemainTimer)
	{
		return;
	}

	if (!g_bObserverMode)
	{
        if (IsCancleDealyCast() == true)
        {
            this->CancleCast();
            this->ShowWindow(FALSE);
            return;
        }
	}

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	if ( this->m_pRemainTimer->IsExpiredManual(FALSE) ) 
	{
#else
	if ( this->m_pRemainTimer->IsExpired() ) 
	{
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__		

		switch( m_eDoType )
		{
			case eDoType_Delay_Leave_Domination:
				{
#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
                    g_KeyQueueManager.SetDefaultFun(GlobalFunc::DoSystemDialog);
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM

					uiDominationTowerMan::SEND_LEAVE_DOMINATION();
				}
				break;
			case eDoType_Delay_Leave_Field			:
#ifdef _NA_000000_20120527_CHAOS_ZONE
            case eDoType_Delay_Leave_ChaosZone:
#endif //_NA_000000_20120527_CHAOS_ZONE
				{
					BattleScene::BackToTheVillageSyn();
				}
				break;

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
				//no operation
#else
			case eDoType_ServerSelectStandingBy		:
				{
					SystemDialog * pDlg = ( SystemDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_SYSTEM );
					if( pDlg ) pDlg->ProcessServerSelect();
				}
				break;
#endif					
			case eDoType_CharacterSelectStandingBy	:
				{
					SystemDialog * pDlg = ( SystemDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_SYSTEM );
					if( pDlg ) pDlg->ProcessCharacterSelect();
				}
				break;			

			case eDoType_Delay_Use_Return_Stone:
				{
					ItemManager::Instance()->SendItemUseMsg(
						ItemManager::Instance()->GetPendingUseItemSlotIdx(), 
						ItemManager::Instance()->GetPendingUseItemPos() );
				}
				break;

			case eDoType_Delay_Spawn_Village:
				{
					MSG_CG_STATUS_REBIRTH_ON_VILLAGE_SYN		SendPacket;
					SendPacket.m_byCategory		= CG_STATUS;
					SendPacket.m_byProtocol		= CG_STATUS_REBIRTH_ON_VILLAGE_SYN;

					if( FALSE == GlobalFunc::SendPacket( CI_GAMESERVERIDX, 
						&SendPacket, 
						sizeof(SendPacket) ) )
					{
						assert( !"eDoType_Delay_Spawn_Village" );
					}
				}
				break;

			case eDoType_Lottery_OpenDlg:
				{
					LotteryInventoryDialog* pPandoraDlg =  GET_CAST_DIALOG(LotteryInventoryDialog, IM_INVENTORY_MANAGER::LOTTERYINVEN_DLG );
					if (pPandoraDlg) 
                    {
                        pPandoraDlg->SendItemUseSync(TRUE);	//판도라 아이템 열기패킷보내기
                    }
				}
				break;

			case eDoType_ObserverMode_Leave:
				{
					if (GENERALPARAM->IsNet())
                    {
						GlobalFunc::SendObserverPacket(nsSSQ::Observer::Info::eRequest_Leave);
                    }
				}
				break;

			case eDoType_ObserverMode_ChangePos:
				{
					if(GENERALPARAM->IsNet())
                    {
						GlobalFunc::SendObserverCtrl(g_ObserverKey);
                    }
				}
				break;
			case eDoType_Delay_Spawn_DominationWar:
#ifdef _NA_008334_20150608_SONNENSCHEIN
            case eDoType_Delay_Spawn_SonnenSchein:
#endif //_NA_008334_20150608_SONNENSCHEIN
				{
#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM
                    g_KeyQueueManager.SetDefaultFun(GlobalFunc::DoSystemDialog);
#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM

					MSG_CG_STATUS_RESURRECTION_TO_SAFEZONE_SYN SendSafeZonePacket;
					GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendSafeZonePacket, sizeof(SendSafeZonePacket));
				}
				break;
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
            case eDoType_Delay_Spawn_FreePVP:
                {
                    ChaosSystem::PacketFunction::SEND_CG_FREE_PVP_RESURRECTION_SYN();
                }
                break;
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
			case eDoType_PersonalWarp:  
				{  
					PERSIONERWARP::SEND_CG_FREE_CHAR_WAYPOINT();  
				}  
				break;  
			case eDoType_GeneralWarp:
				{
					GlobalFunc::isRunUsePoralCoordItem();
				}
				break;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
            case eDoType_Delay_SpaWarp_Dialog:
                {
                    //타락한 사원
                    if (g_pHero)
                    {
                        g_pHero->SendWingsControlByRider(false, false);
                        g_pHero->SendWingsControlByAccessory(false, false);
                    }

                    // 아이템 소모
                    ItemManager::Instance()->SendItemUseMsg(
                        ItemManager::Instance()->GetPendingUseItemSlotIdx(), 
                        ItemManager::Instance()->GetPendingUseItemPos());

                    ItemManager::Instance()->SetItemLock(ItemManager::Instance()->GetPendingUseItemSlotIdx(), 
                        ItemManager::Instance()->GetPendingUseItemPos(), false); 
                }
                break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
		}
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
        if ( eDoType_ServerSelectStandingBy != m_eDoType )
        {
            this->CancleCast();
            ShowWindow(FALSE);
        }
#else
        this->CancleCast();
        ShowWindow(FALSE);
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__
	}

	if (m_pRemainTextControl && m_pDescText)
	{
		const int c_RemainTimeID = 899;
		DWORD dwRemainTime = 
			DWORD( m_pRemainTimer->GetIntervalTime() * (1.0f - m_pRemainTimer->GetProgressRatio()) / 1000 );

		
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

		if ((eDoType_Delay_Spawn_DominationWar == m_eDoType)
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            || (eDoType_Delay_Spawn_BattleGround == m_eDoType)
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008334_20150608_SONNENSCHEIN
            || (eDoType_Delay_Spawn_SonnenSchein== m_eDoType)
#endif //_NA_008334_20150608_SONNENSCHEIN
            )
		{
			m_pRemainTextControl->SetTextWZ(" ");

			//3326	%d 초 후 시작지점으로 리스폰 됩니다.
			g_InterfaceManager.GetInterfaceString(3326, szMessage, INTERFACE_STRING_LENGTH);
			Sprintf(m_szResultInfo, szMessage, dwRemainTime);
			m_pDescText->SetTextWZ(m_szResultInfo);
		}
		else
		{
			//5116	잠시만 기다려 주십시오.
			g_InterfaceManager.GetInterfaceString(5116, m_szResultInfo);
			m_pDescText->SetTextWZ(m_szResultInfo);

			// %d 초 남았습니다.
			g_InterfaceManager.GetInterfaceString(eST_REMAIN_SECOND,szMessage,INTERFACE_STRING_LENGTH);
			Sprintf( m_szResultInfo, szMessage, dwRemainTime);
			m_pRemainTextControl->SetTextWZ(m_szResultInfo);
		}

		SetGage(m_pRemainTimer->GetProgressRatio()*100.f);
	}
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	if ( m_pServerSelectTimer && m_pServerSelectTimer->IsExpiredManual(FALSE) 
			&& eDoType_ServerSelectStandingBy == m_eDoType )
	{
		// 서버선택 최대 대기시간 초과
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
		// 85122	서버 상태가 원활하지 않습니다. 잠시 후 다시 시도해 주십시오.
		g_InterfaceManager.GetInterfaceString(eST_SERVER_STATUS_NOT_SMOOTH, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(szMessage);

		this->CancleCast();
		ShowWindow(FALSE);
	}	
#endif

#ifdef _NA_008334_20150608_SONNENSCHEIN
    if (eDoType_Delay_Spawn_SonnenSchein == m_eDoType &&
        g_pHero &&
        g_pHero->IsDead() == FALSE)
    {
        g_KeyQueueManager.SetDefaultFun(GlobalFunc::DoSystemDialog);
        this->CancleCast();
        ShowWindow(FALSE);
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
}

//-------------------------------------------------------------------------------------------
/**
*/
VOID 
CastLeaveFieldDialog::OnUpdateSolarDialog()
{
	updateCheckExpireCast();
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
CastLeaveFieldDialog::UpdatePosition()
{
	RECT rcDlg;
	GetDialogWZ()->GetClientRect(&rcDlg);

	int iDelta = ( rcDlg.right - rcDlg.left ) / 2;

	float x, y;

	POINT pt = g_pApplication->Get2DModeMapping();
	
	//m_eDoType타입에따라 캐스팅위치가 다르다.
	switch(m_eDoType)
	{
	case eDoType_Lottery_OpenDlg:
		{
			x = (float)pt.x / 2 - iDelta;
			y = (float)pt.y * 0.5f;				// 대략 2/3 위치

		}
		break;
	default:
		{
			x = (float)pt.x / 2 - iDelta;
			y = (float)pt.y * 0.3f;				// 대략 2/3 위치
		}
		break;
	}
	
	

	MoveWindowWZ( (float)x, (float)(y) );
}
