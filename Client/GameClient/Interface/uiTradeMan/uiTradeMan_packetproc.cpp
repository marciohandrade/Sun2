//------------------------------------------------------------------------------
//  uiTradeMan_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiTradeMan.h"
#include "hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiSystemConfirm/uiSystemConfirm.h"

//------------------------------------------------------------------------------
/**
*/
void _callback_Response_Modify(bool bYes)
{
	if(bYes)
	{	
		MSG_CG_TRADE_MODIFY_RESPONSE_SYN SendMsg;
		SendMsg.m_bAccept = TRUE;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
	}
	else
	{
		MSG_CG_TRADE_MODIFY_RESPONSE_SYN SendMsg;
		SendMsg.m_bAccept = FALSE;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
	}
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::NetworkProc( MSG_BASE * pMsg )
{
	TCHAR   szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};

	if(!g_pHero)
		return;

    switch( pMsg->m_byCategory )
    {
    case CG_TRADE:
        {
            switch (pMsg->m_byProtocol)
            {

			case CG_SECURE_POLICY_PATTERN04_SIMPLEX_SNK_CMD:
				{
					ISecureWorkNode::OnMsgRecvSignal::SimplexSNK_T4( pMsg, 0 );
				}
				break;


			case CG_TRADE_OPEN_ACK:
				{
					MSG_CG_TRADE_OPEN_ACK * pRecvMsg = ( MSG_CG_TRADE_OPEN_ACK * )pMsg;

					g_pHero->SetBeHaveState( PLAYER_BEHAVE_TRADE_STATE );
					m_dwTradeModifyDlgKey = 0;
					TradeStart();
				}
				break;

			case CG_TRADE_OPEN_NAK:
				{
					MSG_CG_TRADE_OPEN_NAK * pRecvMsg = ( MSG_CG_TRADE_OPEN_NAK * )pMsg;
					this->ErrorCode(pRecvMsg->m_byErrorCode);
					this->SendTradeCancel();
					this->Clear();

					g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
				}
				break;

			case CG_TRADE_OPEN_CMD:
				{
					MSG_CG_TRADE_OPEN_CMD * pRecvMsg = ( MSG_CG_TRADE_OPEN_CMD * )pMsg;

					g_pHero->SetBeHaveState(PLAYER_BEHAVE_TRADE_STATE);
					SetName( g_pHero->GetObjectKey() );
					SetNameTo( pRecvMsg->m_dwPlayerKey );
					TradeStart();
				}
				break;



            //------------------------------------------------------------------------------
            // 거래 취소
            //
            case CG_TRADE_CANCEL_ACK:
                {
                    //	거래를 취소했습니다.
                    this->TradeClose();
                    this->Clear();
					uiSystemMan* pSysMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSysMan)
					{
						pSysMan->Destroy_Sys_Confirm(GetTradeModifyDlgKey());
					}
                    g_InterfaceManager.GetInterfaceString( eST_TRADE_CANCEL_ACK, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

                    g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
                }
                break;

            case CG_TRADE_CANCEL_NAK:
                {
					MSG_CG_TRADE_CANCEL_NAK * pRecvMsg  = (MSG_CG_TRADE_CANCEL_NAK *) pMsg;

					this->ErrorCode(pRecvMsg->m_byErrorCode);
                    this->TradeClose();
                    this->Clear();

                    g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
                }
                break;

            case CG_TRADE_CANCEL_CMD:
                {
                    //	거래가 취소 되었습니다.
                    this->TradeClose();
                    this->Clear();
					uiSystemMan* pSysMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSysMan)
					{
						pSysMan->Destroy_Sys_Confirm(GetTradeModifyDlgKey());
					}
                    g_InterfaceManager.GetInterfaceString( eST_TRADE_CANCEL_CMD, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

                    g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
                }
                break;
        
                //------------------------------------------------------------------------------
                // CG_TRADE_PUT_ACK / CG_TRADE_PUT_NAK
                //
                // 아이템을 거래창에 올린다.
                // 
            case CG_TRADE_PUT_ACK:
                {
                    MSG_CG_TRADE_PUT_ACK *msg = (MSG_CG_TRADE_PUT_ACK *)pMsg;
                    
                    this->RECV_Put1(msg->m_TradePos, msg->m_OrgPos);
                }
                break;

            case CG_TRADE_PUT_NAK:
                {
                    MSG_CG_TRADE_PUT_NAK *msg = (MSG_CG_TRADE_PUT_NAK *)pMsg;
					this->ErrorCode(msg->m_byErrorCode);
					this->RECV_PutNak(msg->m_byErrorCode);
                }
                break;

                //------------------------------------------------------------------------------
                // 상대방이 아이템을 올림
                //
            case CG_TRADE_PUT_CMD:
                {
                    MSG_CG_TRADE_PUT_CMD *msg = (MSG_CG_TRADE_PUT_CMD *)pMsg;

					this->RECV_Put2(msg->m_TradePos, msg->m_ItemStream);
                }
                break;


                //------------------------------------------------------------------------------
                // CG_TRADE_GET_ACK / CG_TRADE_GET_NAK
                //
                // 아이템을 거래창에서 내린다.
                // 
            case CG_TRADE_GET_ACK:
                {
                    MSG_CG_TRADE_GET_ACK *msg = (MSG_CG_TRADE_GET_ACK *)pMsg;

                    this->RECV_Get1(msg->m_TradePos);
                }
                break;

            case CG_TRADE_GET_NAK:
                {
                    MSG_CG_TRADE_GET_NAK *msg = (MSG_CG_TRADE_GET_NAK *)pMsg;
					this->ErrorCode(msg->m_byErrorCode);
					this->RECV_GetNak(msg->m_byErrorCode);
                }
                break;

                //------------------------------------------------------------------------------
                // 상대방이 아이템을 내림
                //
            case CG_TRADE_GET_CMD:
                {
                    MSG_CG_TRADE_GET_CMD *msg = (MSG_CG_TRADE_GET_CMD *)pMsg;

                    this->RECV_Get2(msg->m_TradePos);
                }
                break;

                //------------------------------------------------------------------------------
                // CG_TRADE_PUT_MONEY_ACK / CG_TRADE_PUT_MONEY_NAK
                //
                // 돈을 거래창에 올린다.
                // 
            case CG_TRADE_PUT_MONEY_ACK:
                {
                    MSG_CG_TRADE_PUT_MONEY_ACK *msg = (MSG_CG_TRADE_PUT_MONEY_ACK *)pMsg;

                    this->RECV_PutMoney1(msg->m_money);
                }
                break;

            case CG_TRADE_PUT_MONEY_NAK:
                {
                    MSG_CG_TRADE_PUT_MONEY_NAK *msg = (MSG_CG_TRADE_PUT_MONEY_NAK *)pMsg;
					this->ErrorCode(msg->m_byErrorCode);
					this->RECV_PutMoneyNak(msg->m_byErrorCode);
                }
                break;

                //------------------------------------------------------------------------------
                // 상대방이 돈을 올림
                //
            case CG_TRADE_PUT_MONEY_CMD:
                {
                    MSG_CG_TRADE_PUT_MONEY_CMD *msg = (MSG_CG_TRADE_PUT_MONEY_CMD *)pMsg;

                    this->RECV_PutMoney2(msg->m_money);
                }
                break;


                //------------------------------------------------------------------------------
                // CG_TRADE_GET_MONEY_ACK / CG_TRADE_GET_MONEY_NAK
                //
                // 돈을 거래창에서 뺀다
                // 
            case CG_TRADE_GET_MONEY_ACK:
                {
                    MSG_CG_TRADE_GET_MONEY_ACK *msg = (MSG_CG_TRADE_GET_MONEY_ACK *)pMsg;

                    this->RECV_GetMoney1(msg->m_money);
                }
                break;

            case CG_TRADE_GET_MONEY_NAK:
                {
                    MSG_CG_TRADE_GET_MONEY_NAK *msg = (MSG_CG_TRADE_GET_MONEY_NAK *)pMsg;
					this->ErrorCode(msg->m_byErrorCode);
					this->RECV_GetMoneyNak(msg->m_byErrorCode);
                }
                break;

                //------------------------------------------------------------------------------
                // 상대방이 돈을 거래창에서 뺐다
                //
            case CG_TRADE_GET_MONEY_CMD:
                {
                    MSG_CG_TRADE_GET_MONEY_CMD *msg = (MSG_CG_TRADE_GET_MONEY_CMD *)pMsg;

                    this->RECV_GetMoney2(msg->m_money);
                }
                break;

                //------------------------------------------------------------------------------
                // 거래 제시
                //
            case CG_TRADE_PROPOSAL_ACK:
                {
                    //	거래를 제시했습니다.
                    this->RECV_Proposal1();
                    g_InterfaceManager.GetInterfaceString( eST_TRADE_PROPOSAL_ACK, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                }
                break;

            case CG_TRADE_PROPOSAL_NAK:
                {
                    MSG_CG_TRADE_PROPOSAL_NAK *msg = (MSG_CG_TRADE_PROPOSAL_NAK *)pMsg;
					this->ErrorCode(msg->m_byErrorCode);
					this->RECV_ProposalNak(msg->m_byErrorCode);
                }
                break;

            case CG_TRADE_PROPOSAL_CMD:
                {
                    //70175	상대방이 거래를 제시했습니다.
                    this->RECV_Proposal2();
                    g_InterfaceManager.GetInterfaceString( eST_TRADE_PROPOSAL_CMD, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                }
                break;


                //------------------------------------------------------------------------------
                // 거래 수정
                //
            case CG_TRADE_MODIFY_ACK:
                {
                    
                    this->RECV_Modify1();
					//6344	상대방이 물품변경에 동의했습니다.
					g_InterfaceManager.GetInterfaceString(6344, szMessage);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                }
                break;

            case CG_TRADE_MODIFY_NAK:
                {
                    MSG_CG_TRADE_MODIFY_NAK *msg = (MSG_CG_TRADE_MODIFY_NAK *)pMsg;
					this->ErrorCode(msg->m_byErrorCode);
					this->RECV_ModifyNak(msg->m_byErrorCode);
                }
                break;

            case CG_TRADE_MODIFY_CMD:
                {
					uiSystemMan* pSysMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSysMan)
					{
						TCHAR szSysMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
						//6346	상대방이 물품변경을 요청했습니다. 물품 변경에 동의하시겠습니까?
						g_InterfaceManager.GetInterfaceString(6346, szSysMessage);

						m_dwTradeModifyDlgKey = 0;
						m_dwTradeModifyDlgKey = pSysMan->Sys_Confirm(szSysMessage, _callback_Response_Modify);
					}
                }
                break;

			case CG_TRADE_MODIFY_RESPONSE_ACK:
				{
					this->RECV_Modify2();

					//6345	상대방의 물품변경을 수락합니다.	
					g_InterfaceManager.GetInterfaceString(6345, szMessage);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);			
				}
				break;

			case CG_TRADE_MODIFY_RESPONSE_NAK:
				{
					MSG_CG_TRADE_MODIFY_RESPONSE_NAK* msg = (MSG_CG_TRADE_MODIFY_RESPONSE_NAK*)pMsg;
					this->ErrorCode(msg->m_byErrorCode);
				}
				break;

                //------------------------------------------------------------------------------
                // 거래 수락
                //
            case CG_TRADE_ACCEPT_ACK:
                {
                    //	거래를 수락했습니다.
                    this->RECV_Accept1();
                    g_InterfaceManager.GetInterfaceString( eST_TRADE_ACCEPT_ACK, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                }
                break;

            case CG_TRADE_ACCEPT_NAK:
                {
                    MSG_CG_TRADE_ACCEPT_NAK *msg = (MSG_CG_TRADE_ACCEPT_NAK *)pMsg;
					this->ErrorCode(msg->m_byErrorCode);
					this->RECV_AcceptNak(msg->m_byErrorCode);
                }
                break;

            case CG_TRADE_ACCEPT_CMD:
                {
                    //	상대방이 거래를 수락했습니다.
                    this->RECV_Accept2();
                    g_InterfaceManager.GetInterfaceString( eST_TRADE_ACCEPT_CMD, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                }
                break;

				//------------------------------------------------------------------------------
                // 거래 수락 후 아이템 정보
                //
			case CG_TRADE_ACCEPT_BRD:
				{
                    MSG_CG_TRADE_ACCEPT_BRD *msg = ( MSG_CG_TRADE_ACCEPT_BRD * )pMsg;

                    //	거래가 완료되었습니다.
                    this->TradeEnd(msg->m_Money, msg->m_TradeInfo);
                    g_InterfaceManager.GetInterfaceString( eST_TRADE_TRADE_BRD, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice((eOUTPUT_MSG_TYPE)(eOUTPUT_MSG_CHAT | eOUTPUT_MSG_TOP_SHOW), szMessage);
                }
			break;
			}
        }
        break;
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------