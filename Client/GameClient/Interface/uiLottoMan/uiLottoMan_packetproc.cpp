#include "SunClientPrecompiledHeader.h"
#include "uiLottoMan.h"
#include "InterfaceManager.h"
#include "uiLottoWindow/uiLottoWindow.h"
#include "uiLottoNumInput/uiLottoNumInput.h"
#ifdef _DEV_VER
#include "GlobalFunc.h"
#endif//_DEV_VER
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
#include "Hero.h"
#endif//_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

//------------------------------------------------------------------------------
/**
*/
void uiLottoMan::NetworkProc( MSG_BASE * pMsg )
{

}
//------------------------------------------------------------------------------
/**
*/
void uiLottoMan::NetworkProcWorld(MSG_BASE * pMsg )
{
	//�ζ���Ŷ�� ó���Ѵ�
	if(pMsg->m_byCategory!=CW_LOTTO) return;

	TCHAR	szTemp[INTERFACE_STRING_LENGTH];
    TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	switch (pMsg->m_byProtocol)
	{
	case CW_LOTTO_INFO_NAK:      
		{
			// (��) ���� �ζ� ���� ����
			MSG_CW_LOTTO_INFO_NAK *pRecvPacket = (MSG_CW_LOTTO_INFO_NAK *)pMsg;				

#ifdef _DEV_VER
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CW_LOTTO_INFO_NAK:%d"),pRecvPacket->bReason);
#endif 

			switch(pRecvPacket->bReason)
			{
			case MSG_CW_LOTTO_INFO_NAK::NOT_READY:
				{
					//5288: ��÷�� ��÷�߿��� �����Ҽ������ϴ�.
					g_InterfaceManager.GetInterfaceString(5288,szMessage);
					LottoMessage(szMessage,FALSE);
				}
				break;
			default:
				{
					//�˼����� �����ڵ�
					_asm nop;
				}
				break;
			}

		}
		break;

	case CW_LOTTO_INFO_ACK:      
		{

#ifdef _DEV_VER
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CW_LOTTO_INFO_ACK"));
#endif 
			// (��) ���� �ζ� ���� ����
			MSG_CW_LOTTO_INFO_ACK *pRecvPacket = (MSG_CW_LOTTO_INFO_ACK *)pMsg;
			uiLottoWindow* pLottoWindow= GET_CAST_DIALOG( uiLottoWindow, IM_LOTTO_MANAGER::LOTTO_WINDOW_DLG );

			SetWaitPacket(FALSE);

			if(!pLottoWindow) break;

			//�������̽� �����ش�.(�ܼ�����������û�϶��� ��������)
			if(pRecvPacket->bFlag)
			{
				pLottoWindow->ShowInterface(TRUE);
				//��÷���϶��� ��ư��Ȱ��
				pLottoWindow->SetEnableWindow(pRecvPacket->bStatus ? (true):(false));
			}

			SetLottoActive(pRecvPacket->bStatus);						// �ζ�Ȱ����Ȱ��(��÷�߿��º�Ȱ��)

			SetLottoNumber(pRecvPacket->i64TotalAccum);					// ����ȭ������ �ζ�NPC�� ����	
			
			//bFlag==false�ϰ�� ������ ����ä���ʿ����(â�ȿ��⶧��)
			if(!pRecvPacket->bFlag) break;

			m_LottoInfo.m_bNumTickets	= pRecvPacket->bNumTickets;		// ���� �����ϰ� �ִ� ���� ���� ��
			pLottoWindow->SetItemNum(m_LottoInfo.m_bNumTickets);		
			m_LottoInfo.m_i64TotalAccum = pRecvPacket->i64TotalAccum;	// ���� �� ������
			m_LottoInfo.m_wLottoDrawNum = pRecvPacket->wCurrentDrawNum;	// ���� ȸ��
			//--------------------------------
			//	����ȸ��������������(�⺻����)
			//--------------------------------
			// 1ȸ�� ���𶧴� ����ȸ�� ������ ���⶧���� ������ó���κ��� ó���ȵǰ� SetLottoBaseInfo()�� ȣ��ȵ�, ������ �����ݾ��� UIâ�� ǥ�� ���� ����
			// �ش� �κ��� ó���ϱ� ���� ������ó���κа� SetLottoBaseInfo()ȣ�� ��ġ�� ������. �׸��� if(pRecvPacket->bNumResultRecords>0) �ּ�ó����
			LOTTO_BASE_INFO baseinfo;
			memset(&baseinfo,0,sizeof(baseinfo));
			
			//��÷��ȣ(UI�� �ϳ��ۿ� ����)
			if(pRecvPacket->bNumResultRecords>0)
			{
				memcpy(baseinfo.arLuckyNum,&pRecvPacket->results[0].bWinningNum,sizeof(baseinfo.arLuckyNum));
				baseinfo.wDrawNum	 = pRecvPacket->results[0].wDrawNum;		// ��ȸ��					
				baseinfo.WinnerHeim  = pRecvPacket->results[0].i64Jackpot;		// 1�� ��÷��
				baseinfo.dwWinnerNum = pRecvPacket->results[0].bNumWinners;		// 1�� ��÷�ڼ�				
			}
			baseinfo.LottoHeim   = m_LottoInfo.m_i64TotalAccum;					// �����ݾ�
			pLottoWindow->SetLottoBaseInfo(baseinfo);

			//--------------------------------
			//	���� ���������� ������
			//  ���������� ���������Ƿ� ���ǵ�
			//	������ �� 2ȸ��-1ȸ�� �̷���
			//--------------------------------
			if(pRecvPacket->bNumEntryRecords>0)
			{
				//LOTTO_APPLY_DATA �ϳ��� ����ȸ��(wDrawNum)����  �ִ�5���������ִ�
				//�Ʒ������� �������� bNumEntryRecords������ŭ �����Ͱ� ������µ� �̰͵���
				//ȸ������ ���л����ϱ����� �����̴� 
				LOTTO_APPLY_DATA ApplyData;
				memset(&ApplyData,0,sizeof(ApplyData));
				int nIndexCnt=0;
				int nDrawNum = pRecvPacket->entries[0].wDrawNum;
				for(int i=0;i<pRecvPacket->bNumEntryRecords;++i)
				{
					//ȸ���� �������� �������� ����ִ´�
					if(nDrawNum==pRecvPacket->entries[i].wDrawNum)
					{
						//nIndexCnt�� �ִ밪�� 5���̴�
						ApplyData.wDrawNum = nDrawNum;
						if(nIndexCnt<LOTTO_MAX_TICKETS)
						{
							memcpy(&ApplyData.arNumInfo[nIndexCnt][0],&pRecvPacket->entries[i].bPickedNum[0],sizeof(pRecvPacket->entries[i].bPickedNum));
							++nIndexCnt;
							++ApplyData.byEntryCnt;
						}
					}
					else
					{	
						//ȸ���� �ٲ���ٸ�  �������ԵȰŻ������ְ�(InsertApplyInfo)
						//���� �̵����ʹ� �պκп� �����Ѵ�
						pLottoWindow->InsertApplyInfo(ApplyData,TRUE);
						memset(&ApplyData,0,sizeof(ApplyData));
						
						//��ȣ�� ���� + �̰ǻ����Ѵ�
						nDrawNum = pRecvPacket->entries[i].wDrawNum;
						ApplyData.wDrawNum = nDrawNum;
						memcpy(&ApplyData.arNumInfo[0][0],&pRecvPacket->entries[i].bPickedNum[0],sizeof(pRecvPacket->entries[i].bPickedNum));
						nIndexCnt=ApplyData.byEntryCnt =1;
					}	
				}
				
				pLottoWindow->InsertApplyInfo(ApplyData,TRUE);
			}

			pLottoWindow->SetApplyInfo();
		}
		break;
	case CW_LOTTO_TICKET_NTF:     
		{
			//5291 ���ǿ����� ȹ�� �Ͽ����ϴ�.
			MSG_CW_LOTTO_TICKET_NTF *pRecvPacket = (MSG_CW_LOTTO_TICKET_NTF *)pMsg;
			m_LottoInfo.m_bNumTickets = pRecvPacket->bNumTickets;
			g_InterfaceManager.GetInterfaceString(5291,szMessage);
			LottoMessage(szMessage,FALSE);
		}
		break;
	case CW_LOTTO_ENTRY_ACK:      
		{	
			MSG_CW_LOTTO_ENTRY_ACK *pRecvPacket = (MSG_CW_LOTTO_ENTRY_ACK *)pMsg;
			//5295 ���� �����Ͽ����ϴ�.
			g_InterfaceManager.GetInterfaceString(5295,szMessage);
			LottoMessage(szMessage);	

			uiLottoWindow* pLottoWindow= GET_CAST_DIALOG( uiLottoWindow, IM_LOTTO_MANAGER::LOTTO_WINDOW_DLG );
			if(pLottoWindow)
			{
				//�ζ�â�� ������ �������ش�.
				pLottoWindow->InsertLastApply(pRecvPacket->wDrawNum,pRecvPacket->bPickedNum);
				pLottoWindow->SetItemNum(pRecvPacket->bNumTickets);
				m_LottoInfo.m_bNumTickets =pRecvPacket->bNumTickets;
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
                g_pHero->SetMoney( g_pHero->GetMoney() - pRecvPacket->i64TicketPrice );
#endif//_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
			}
		}
		break;
	case CW_LOTTO_ENTRY_NAK:      
		{
			MSG_CW_LOTTO_ENTRY_NAK *pRecvPacket = (MSG_CW_LOTTO_ENTRY_NAK *)pMsg;

#ifdef _DEV_VER
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("CW_LOTTO_ENTRY_NAK:%d"),pRecvPacket->bReason);
#endif 
			//5296	���� ���� �Ͽ����ϴ�.
			g_InterfaceManager.GetInterfaceString(5296,szMessage);
			LottoMessage(szMessage);

			switch(pRecvPacket->bReason)
			{
			case MSG_CW_LOTTO_ENTRY_NAK::NOT_READY:// ���� �ζǰ� ���� ���� �ƴ�
				{
					
					//5288: ��÷�� ��÷�߿��� �����Ҽ������ϴ�.
					g_InterfaceManager.GetInterfaceString(5288,szMessage);
					LottoMessage(szMessage,FALSE);
					_asm nop;
				}
				break;
			case MSG_CW_LOTTO_ENTRY_NAK::OUT_OF_TICKET: // ���� �ζ� ���� ����
				{
					//5289: ���ǿ����� �����ϴ�.
					g_InterfaceManager.GetInterfaceString(5289,szMessage);
					LottoMessage(szMessage,FALSE);
					_asm nop;
				}
				break;
            case MSG_CW_LOTTO_ENTRY_NAK::NOT_ENOUGH_MONEY:// ���� �ݾ� �����մϴ�.
                {
                    //551: ������ �����մϴ�.
                    g_InterfaceManager.GetInterfaceString(eST_NOT_ENOUGH_MONEY,szMessage);
                    LottoMessage(szMessage,FALSE);
                }
                break;
			default:
				{
					_asm nop;	
				}
				break;
			}

				
		}
		break;
	case CW_LOTTO_ACCUM_BRD:      
		{
			// ���� �ζ� ������ ���� ����(�ζ� NPC����)
			MSG_CW_LOTTO_ACCUM_BRD *pRecvPacket = (MSG_CW_LOTTO_ACCUM_BRD *)pMsg;
			SetLottoNumber(pRecvPacket->i64TotalAccum);
			
		}
		break;
	case CW_LOTTO_COUNTDOWN_BRD:  // ���� �ζ� ��÷ ī��Ʈ�ٿ� ����
		{
			MSG_CW_LOTTO_COUNTDOWN_BRD *pRecvPacket = (MSG_CW_LOTTO_COUNTDOWN_BRD *)pMsg;
			//5292	%d�� �� %dȸ ���� �ζ� ��÷��ȣ�� ��÷�մϴ�.
			g_InterfaceManager.GetInterfaceString(5292,szTemp);
			int nMin = pRecvPacket->wSeconds/60;
            Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, nMin, pRecvPacket->wDrawNum);
			LottoMessage(szMessage);
		}
		break;

	case CW_LOTTO_START_BRD:
		{
			// ���� �ζ� ��÷/���� ����
			MSG_CW_LOTTO_START_BRD*pRecvPacket = (MSG_CW_LOTTO_START_BRD *)pMsg;
			
			//�ζ�â��δݱ�
			SetLottoActive(TRUE);

			//�ζ�â��δݱ�
			closeAllLottoDlg();

			//5287 %dȸ �ζ� ��÷�� ���۵Ǿ����ϴ�.
			g_InterfaceManager.GetInterfaceString(5287,szTemp);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp,pRecvPacket->wDrawNum);
			LottoMessage(szMessage);
		}
		break;

	case CW_LOTTO_FINISH_BRD:     
		{
			// ���� �ζ� ��÷/���� ����
			MSG_CW_LOTTO_FINISH_BRD *pRecvPacket = (MSG_CW_LOTTO_FINISH_BRD *)pMsg;

			SetLottoActive(FALSE);

			//�ζ�â��δݱ�
			closeAllLottoDlg();

			//5293	%dȸ ���� �ζ� ��÷��ȣ�� %02d-%02d-%02d-%02d���Դϴ�.
			g_InterfaceManager.GetInterfaceString(5293,szTemp);

			Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp,
									pRecvPacket->wDrawNum,		  //��ȸ	
									pRecvPacket->bWinningNum[0],  //ù��°
									pRecvPacket->bWinningNum[1],  //�ι�°
									pRecvPacket->bWinningNum[2],  //����°
									pRecvPacket->bWinningNum[3]); //�׹�°
			
			LottoMessage(szMessage);
		}
		break;
	}
}


