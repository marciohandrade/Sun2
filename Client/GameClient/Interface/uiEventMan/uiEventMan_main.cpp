#include "SunClientPrecompiledHeader.h"

#include "uiEventMan.h"
#include "uiEventWindow/uiEventWindow.h"
#include "uiEventItemSelect/uiEventItemSelect.h"
#include "ItemManager.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "SunEventInfoParser.h"
#include "GameFunc.h"

#include "ObjectManager.h"
#include "Hero.h"

#include <SCItemSlot.h>

#include "ui_XMas_SantaReward/ui_XMas_SantaReward.h"
#include "ui_XMas_DonationReward/ui_XMas_DonationReward.h"
#include "ui_XMas_SoloCoupleReward/ui_XMas_SoloCoupleReward.h"

#include "Player.h"
#include "NPC.h"
#include "ProjectileManagerEx.h"

#include "uiEventFriendWindow.h"

//------------------------------------------------------------------------------
/**
*/
uiEventMan::uiEventMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
    m_pUIEventWindow = NULL;
	m_pUIEventItemSelect = NULL;

	m_iEventMode = -1;

	m_iEvente_Regitem_GetItem		= 0;
	m_iEvente_Regitem_RandReward	= 0;

	SetWait(FALSE);
	
}

void uiEventMan::OnInitialize()
{
    _LoadUI();

    _setEnableEvent();
}

//------------------------------------------------------------------------------
/**
*/
void uiEventMan::ShowEventWindow(BOOL bShow)
{
	if(this->m_pUIEventWindow)
	{
		this->m_pUIEventWindow->ShowInterface(bShow);
	}
}

//------------------------------------------------------------------------------
void uiEventMan::ShowEventItemSelectWindow(BOOL bShow, int iEventMode)
{
	if(bShow)
	{
		SetWait(FALSE);
		m_iEventMode = iEventMode;
	}

	if(this->m_pUIEventItemSelect)
	{
		this->m_pUIEventItemSelect->ShowInterface(bShow);
	}
}

//------------------------------------------------------------------------------
/**
*/	
BOOL uiEventMan::RequestEventStatus()
{
	// 이벤트 아이템 등록 상태 조회 요청

	BOOL bRet = FALSE;

	MSG_CG_EVENT_ITEM_STATUS_SYN	sync;

	bRet = GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));

	if(!bRet)
	{
#ifdef _DEBUG
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, _T("ERR SendPacket: MSG_CG_EVENT_ITEM_STATUS_SYN"));
#endif
		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiEventMan::RequestEventItemReg(CODETYPE dwEventID)
{
	if(IsWait())
	{
		//5122	아이템 등록 요청중입니다. 잠시만 기다려 주세요	
		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
		GetUIMan()->GetInterfaceString( 5122, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		return FALSE;
	}
	
	BOOL bRet = FALSE;

	MSG_CG_EVENT_ITEM_REG_SYN	sync;
	sync.m_dwEventID = dwEventID;
	
	bRet = GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));

	if(!bRet)
	{
#ifdef _DEBUG
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, _T("ERR SendPacket: MSG_CG_EVENT_ITEM_REG_SYN"));
#endif
		return FALSE;
	}

	SetWait(TRUE);	// 이벤트 등록 요청하고 기다린다.

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void uiEventMan::NetworkProc( MSG_BASE * pMsg )
{
	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR   szTemp[INTERFACE_STRING_LENGTH] = {0,};

	TCHAR szDebug[256] = {0,};

    switch (pMsg->m_byCategory)
    {
    case CG_EVENT:
        {
            switch (pMsg->m_byProtocol)
            {		
			case CG_EVENT_ITEM_STATUS_ACK:	// // 이벤트 아이템 등록 상태 받기
				{
					SetWait(FALSE);

					m_pUIEventItemSelect->SetEventItemStatusList(pMsg);
				}
				break;

			case CG_EVENT_ITEM_STATUS_NAK:	// // 등록 상태 조회 실패
				{
					SetWait(FALSE);

					MSG_CG_EVENT_ITEM_STATUS_NAK *pRecvPacket = (MSG_CG_EVENT_ITEM_STATUS_NAK *)pMsg; 

				}
				break;

			case CG_EVENT_ITEM_REG_ACK:		//// 이벤트 아이템 등록 성공(경품 응모)
				{
					SetWait(FALSE);

					MSG_CG_EVENT_ITEM_REG_ACK *pRecvPacket = (MSG_CG_EVENT_ITEM_REG_ACK *)pMsg; 

					DWORD dwEventNpcType[c_dw_Special_Event_Num] = {c_dw_Donation_NPC_Type, c_dw_SoloCouple_NPC_Type, c_dw_Santa_NPC_Type, c_dw_Buff_Event_NPC_Type };
					
					BOOL bVal = FALSE;
					for(int i = 0; i < c_dw_Special_Event_Num; ++i)
					{
						EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(dwEventNpcType[i]);
						if(NULL != pEventInfoVec)
						{
							int iCnt = pEventInfoVec->size();
							for(int j = 0; j < iCnt; ++j)
							{
								EVENT_INFO* pEventInfo = (*pEventInfoVec)[j];
								if((pEventInfo) && (pRecvPacket->m_dwEventID == pEventInfo->m_dwEventID))
								{
									bVal = TRUE;
									break;
								}
							}
						}

					}
					
					if(FALSE == bVal)
					{
						m_pUIEventItemSelect->SuccessItemReg(pRecvPacket->m_dwEventID);					
						// TODO: pRecvPacket->m_dwEventID 경품 응모 성공
						m_pUIEventItemSelect->UpdateList();
					}
				}
				break;

			case CG_EVENT_ITEM_REG_NAK:		// 이벤트 아이템 등록 실패
				{
					SetWait(FALSE);

					MSG_CG_EVENT_ITEM_REG_NAK *pRecvPacket = (MSG_CG_EVENT_ITEM_REG_NAK *)pMsg;

					ShowErrorMessage(pRecvPacket->m_dwErrorCode);

				}
				break;

			case CG_EVENT_ITEM_REWARD_CMD:	//이벤트 아이템 보상 받음(W->C) - 아이템 보상
				{
					SetWait(FALSE);

					MSG_CG_EVENT_ITEM_REWARD_CMD * pRecvPacket = (MSG_CG_EVENT_ITEM_REWARD_CMD *)pMsg;
					if(g_pHero)
						g_pHero->SetMoney(pRecvPacket->m_Money);

					DWORD dwEvnetNpcType[2] = { c_dw_Santa_NPC_Type, c_dw_Donation_NPC_Type };
					DWORD dwEventID[2] = {0,0};
					for(int i =0; i < 2; ++i)
					{
						EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(dwEvnetNpcType[i]);
						if(NULL != pEventInfoVec)
						{
							EVENT_INFO* pEventInfo = pEventInfoVec->back();
							if(pEventInfo)
							{
								dwEventID[i] = pEventInfo->m_dwEventID;
							}
						}
					}	

					if(dwEventID[0] == pRecvPacket->m_dwEventID)  //산타 관련 Event
					{
						ui_XMas_SantaReward * pSantaReward = GET_CAST_DIALOG( ui_XMas_SantaReward, IM_EVENT_MANAGER::EVENT_X_MAS_SANTA_REWARD );
						
						if(pSantaReward)
						{
							pSantaReward->SetRewardInfo(pRecvPacket->m_TotalInfo);
							pSantaReward->SetSlotState(ui_XMas_SantaReward::eSLOT_STATE_DEST);
						}
					}
					else if(dwEventID[1] == pRecvPacket->m_dwEventID) //기부 관련 Event
					{
						SCItemSlot ItemBuf;
						ITEMSLOTEX * pSlot = pRecvPacket->m_TotalInfo.m_Slot;
						POSTYPE start = 0;
						POSTYPE total = pRecvPacket->m_TotalInfo.m_InvenCount;

						const BASE_ITEMINFO * pBaseItemInfo = NULL;

						for(POSTYPE i=start;i<total;++i)
						{
							ItemBuf.Clear();
							ItemBuf.Copy( pSlot[i].m_Stream );
							pBaseItemInfo = ItemBuf.GetItemInfo();
						}

						if(NULL == pBaseItemInfo)
						{
							assert(!"CG_EVENT_ITEM_REWARD_CMD");
						}

						ui_XMas_DonationReward * pDonationReward = GET_CAST_DIALOG( ui_XMas_DonationReward, IM_EVENT_MANAGER::EVENT_X_MAS_DONATION_REWARD );

						if(pDonationReward && pBaseItemInfo)
						{
							pDonationReward->SetItemCode(pBaseItemInfo->m_Code);
							pDonationReward->ShowInterface(TRUE);
						}
					}

					ItemManager::Instance()->InsertInventoryTotalInfo( REWARD, pRecvPacket->m_TotalInfo);

					DWORD dwEventNpcType[c_dw_Special_Event_Num] = {c_dw_Donation_NPC_Type, c_dw_SoloCouple_NPC_Type, c_dw_Santa_NPC_Type, c_dw_Buff_Event_NPC_Type };

					BOOL bVal = FALSE;
					for(int i = 0; i < c_dw_Special_Event_Num; ++i)
					{
						EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(dwEventNpcType[i]);
						if(NULL != pEventInfoVec)
						{
							int iCnt = pEventInfoVec->size();
							for(int j = 0; j < iCnt; ++j)
							{
								EVENT_INFO* pEventInfo = (*pEventInfoVec)[j];
								if((pEventInfo) && (pRecvPacket->m_dwEventID == pEventInfo->m_dwEventID))
								{
									bVal = TRUE;
									break;
								}
							}
						}
					}
					
					if(FALSE == bVal)
					{
						m_pUIEventItemSelect->UpdateList();
					}

				}
				break;
			case CG_EVENT_TIME_PERIOD_ACK:
				{
					MSG_CG_EVENT_TIME_PERIOD_ACK * pRecvPacket = (MSG_CG_EVENT_TIME_PERIOD_ACK *)pMsg;
					m_CurTime = pRecvPacket->time;
					_setEnableEvent();
					m_pUIEventWindow->ShowInterface( TRUE );

				}
				break;
			case CG_EVENT_TIME_PERIOD_NAK:
				{
					MSG_CG_EVENT_TIME_PERIOD_NAK * pRecvPacket = (MSG_CG_EVENT_TIME_PERIOD_NAK *)pMsg;
				}
				break;

			case CG_EVENT_REQUEST_ITEM_NAK:
				{
					MSG_CG_EVENT_REQUEST_ITEM_NAK* pRecvPacket = (MSG_CG_EVENT_REQUEST_ITEM_NAK*)pMsg;
					ShowErrorMessage(pRecvPacket->m_ErrorCode);
				}
				break;

			case CG_EVENT_DONATION_HEIM_ACK:
				{
					MSG_CG_EVENT_DONATION_HEIM_ACK* pRecvPacket = (MSG_CG_EVENT_DONATION_HEIM_ACK*)pMsg;

					if(RC_EVENT_SUCCESS_HEIM_DONATION == pRecvPacket->m_ErrorCode)
					{
						if(g_pHero)
							g_pHero->SetMoney(pRecvPacket->m_TotalHeims);
					}

					ShowErrorMessage(pRecvPacket->m_ErrorCode);
				}
				break;

			case CG_EVENT_BUFF_EVENT_ACK:
				{
					MSG_CG_EVENT_BUFF_EVENT_ACK* pRecvPacket = (MSG_CG_EVENT_BUFF_EVENT_ACK*)pMsg;

					if(RC_EVENT_SUCCESS == pRecvPacket->m_ResultCode)
					{
						//6353 번 신비로운 빛과 함께 마법의 효과가 발동합니다 
						TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
						g_InterfaceManager.GetInterfaceString(6353, szMessage);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

						cTargetPosition Position;
						Position.SetTargetVisiblePosition( g_pHero->GetObjectKey(), TRUE );
						g_ProjectileManagerEx.CreateEffect( g_pHero->GetObjectKey(), StrToWzID("A590"), 3000, Position );
					}
					else
					{
						ShowErrorMessage(pRecvPacket->m_ResultCode);
					}
				}
				break;
            }
        }
        break;
    }
}


BOOL uiEventMan::SendEventTimePeriod( void )
{
	MSG_CG_EVENT_TIME_PERIOD_SYN sync;
	
	return GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
}

//////////////////////////////////////////////////////////////////////////
//#ifdef _YJ_X_MAX_EVENT_000752
void uiEventMan::ShowErrorMessage(DWORD dwErrorCode)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};

	switch(dwErrorCode)
	{
	case RC_EVENT_SUCCESS:
		{
		}
		break;

	case RC_EVENT_UNABLE_FUNCTION_FOR_CHAOSTATE:
		{
			//// 카오 상태에서는 이용이 가능하지 않습니다.
				GetUIMan()->GetInterfaceString(eST_CHAO_IS_INAVALIBLE_STATUS, szMessage);

		}
		break;

	case RC_EVENT_SUCCESS_HEIM_DONATION:				// 기부는 성공했지만 보상은 없다!
		{
			//5325	정상적으로 기부되었습니다.
			GetUIMan()->GetInterfaceString(5325, szMessage);
		}
		break;

	case RC_EVENT_INVALID_EVENT:						// 이벤트 중이 아니다
		{
			GetUIMan()->GetInterfaceString( eST_INVALID_EVENT_STRING, szMessage);
		}
		break;

	case RC_EVENT_ITEM_NUM_OVERFLOW:					// 이벤트 아이템 등록 제한 개수를 넘었다
		{
			GetUIMan()->GetInterfaceString( eST_EVENT_ITEM_NUM_OVERFLOW_STRING, szMessage);
		}
		break;

	case RC_EVENT_NOTEXISTITEM:						// 이벤트에 응모할 아이템이 없다
		{
			GetUIMan()->GetInterfaceString( eST_NOT_ENOUGH_ITEM_STRING, szMessage);
		}
		break;

	case RC_EVENT_CANT_OBTAIN:						// 보상 아이템을 넣을 수가 없다 (지급 실패)
		{
			GetUIMan()->GetInterfaceString( eST_CANT_OBTAIN_STRING, szMessage);
		}
		break;

	case RC_EVENT_INSUFFICIENT_HEIM:				// [하임보상] 기부하려는 하임 > 보유 하임
		{
			//5323	기부하려는 하임이 현재 캐릭터가 보유하고 있는 하임보다 많습니다.
			GetUIMan()->GetInterfaceString( 5323, szMessage);
		}
		break;

	case RC_EVENT_DONATION_HEIM_LIMIT:				// [하임보상] 최소 기부 하임 제한 만족 못함
		{
			//5324	등록하신 금액이 최소 기부 하임보다 적습니다.
			GetUIMan()->GetInterfaceString( 5324, szMessage);
		}
		break;

	case RC_EVENT_REWARDED_ITEM_NUM_OVERFLOW:		// [아이템보상] 더이상 아이템을 받을 수 없다.
		{
			//5072	더이상 보상을 받을수 없습니다.
			GetUIMan()->GetInterfaceString(5072, szMessage);
		}
		break;

	case RC_EVENT_ITEM_INVENTORY_LOCKED:			// [아이템보상] 인벤이 잠겨있어서 보상받지 못한다.
		{
			// 5587	인벤토리가 잠겨있어 보상을 받으실 수 없습니다.
			GetUIMan()->GetInterfaceString(5587, szMessage);
		}
		break;

	case RC_EVENT_CANT_BUFF_GRANTED:
		{
			// 6357 이미 성스러운 돌의 축복을 받았습니다.
			GetUIMan()->GetInterfaceString(6357, szMessage);
		}
		break;

	case RC_EVENT_UNQUALIFIED:
		{
			//6358 적용 가능한 레벨이 아닙니다.
			GetUIMan()->GetInterfaceString(6358, szMessage);
		}
		break;

	default:
	case RC_EVENT_FAILED:			
		{
#ifdef _DEBUG
			GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, _T("EVENT_ERR_DEFAULT"));
#endif
		}
		break;
	}

	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
}
//#endif _YJ_X_MAX_EVENT_000752
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/**
*/
bool uiEventMan::IsEnableEvent(int iEventMode)
{
	if(eEVENT_REGITEM_GETITEM == iEventMode)
	{
		if(m_iEvente_Regitem_GetItem)
			return true;
	}
	else if(eEVENT_REGITEM_RANDREWARD == iEventMode)
	{
		if(m_iEvente_Regitem_RandReward)
			return true;
	}

	return false;
}

//------------------------------------------------------------------------------
/**
*/
void uiEventMan::_setEnableEvent()
{
	m_iEvente_Regitem_GetItem = 0;
	m_iEvente_Regitem_RandReward = 0;

	EVENT_SUB_INFO_ITER it;

	EVENTINFO_MAP_ITER	eventinfo_itr;
	SYSTEMTIME Current_systemTime;
	util::TimeSync::GetLocalTime(&Current_systemTime);

	std::pair<EVENT_SUB_INFO_ITER, EVENT_SUB_INFO_ITER> range;

	range = SunEventInfoParser::Instance()->GetEventSubInfoMap()->equal_range(eEVENT_REGITEM_GETITEM);

	for(it=range.first; it!=range.second; ++it)
	{
		EVENT_SUB_INFO info = (*it).second;

		eventinfo_itr =   SunEventInfoParser::Instance()->GetEventInfoMap()->find(info.m_dwEventID);
		EVENT_INFO Eventinfo = (*eventinfo_itr).second;


		if(Eventinfo.CheckEvent( m_CurTime ))
		{
			++m_iEvente_Regitem_GetItem;
		}
	}

	range = SunEventInfoParser::Instance()->GetEventSubInfoMap()->equal_range(eEVENT_REGITEM_RANDREWARD);
	for(it=range.first; it!=range.second; ++it)
	{
		EVENT_SUB_INFO info = (*it).second;

		eventinfo_itr =   SunEventInfoParser::Instance()->GetEventInfoMap()->find(info.m_dwEventID);
		EVENT_INFO Eventinfo = (*eventinfo_itr).second;


		if(Eventinfo.CheckEvent( m_CurTime ) )
		{
			++m_iEvente_Regitem_RandReward;
		}
	}


}



BOOL uiEventMan::IsValidEventTime(SYSTEMTIME * pstCurrent , SYSTEMTIME * pstEventStart , SYSTEMTIME * pstEventEnd)
{
	FILETIME ftCurret , ftEventStart , ftEventEnd;
	
	pstEventStart->wDayOfWeek = 0;
	pstEventStart->wMilliseconds = 0;
	pstEventStart->wSecond = 0;

	pstEventEnd->wDayOfWeek = 0;
	pstEventEnd->wMilliseconds = 0;
	pstEventEnd->wSecond = 0;

	SystemTimeToFileTime( pstCurrent , &ftCurret );
	SystemTimeToFileTime( pstEventStart , &ftEventStart );
	SystemTimeToFileTime( pstEventEnd , &ftEventEnd );

	if( CompareFileTime( &ftEventStart , &ftCurret ) == 1)
		return FALSE;
	if( CompareFileTime( &ftCurret , &ftEventEnd ) == 1)
		return FALSE;

	return TRUE;
}

BOOL uiEventMan::SendEventFriend( void )
{
	MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_SYN sync;
	if(g_pHero)
	{
		return GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
	}
	return false;
}

void uiEventMan::ShowEventFriendWin(BOOL bShow)
{
	if(this->m_pUIFriendEvent)
	{
		this->m_pUIFriendEvent->ShowInterface(bShow);
	}
}

BOOL uiEventMan::IsShowEventFriendWin()
{ 
	return (m_pUIFriendEvent && m_pUIFriendEvent->IsActivated()); 
}

void uiEventMan::InsertList(InvitateFriendRewardInfo *pRewardInfo)
{ 
	if(m_pUIFriendEvent)
	{
		m_pUIFriendEvent->InsertList(pRewardInfo); 
	}
}
void uiEventMan::InitInvitateFriendList()
{ 
    if(m_pUIFriendEvent)
    {
        m_pUIFriendEvent->_initList();
    }
}
