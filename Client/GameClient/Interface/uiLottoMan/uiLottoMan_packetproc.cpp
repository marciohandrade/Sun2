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
	//로또패킷만 처리한다
	if(pMsg->m_byCategory!=CW_LOTTO) return;

	TCHAR	szTemp[INTERFACE_STRING_LENGTH];
    TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	switch (pMsg->m_byProtocol)
	{
	case CW_LOTTO_INFO_NAK:      
		{
			// (내) 하임 로또 정보 응답
			MSG_CW_LOTTO_INFO_NAK *pRecvPacket = (MSG_CW_LOTTO_INFO_NAK *)pMsg;				

#ifdef _DEV_VER
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CW_LOTTO_INFO_NAK:%d"),pRecvPacket->bReason);
#endif 

			switch(pRecvPacket->bReason)
			{
			case MSG_CW_LOTTO_INFO_NAK::NOT_READY:
				{
					//5288: 당첨자 추첨중에는 응모할수없습니다.
					g_InterfaceManager.GetInterfaceString(5288,szMessage);
					LottoMessage(szMessage,FALSE);
				}
				break;
			default:
				{
					//알수없는 에러코드
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
			// (내) 하임 로또 정보 응답
			MSG_CW_LOTTO_INFO_ACK *pRecvPacket = (MSG_CW_LOTTO_INFO_ACK *)pMsg;
			uiLottoWindow* pLottoWindow= GET_CAST_DIALOG( uiLottoWindow, IM_LOTTO_MANAGER::LOTTO_WINDOW_DLG );

			SetWaitPacket(FALSE);

			if(!pLottoWindow) break;

			//인터페이스 열어준다.(단순숫자정보요청일때는 열지말자)
			if(pRecvPacket->bFlag)
			{
				pLottoWindow->ShowInterface(TRUE);
				//추첨중일때는 버튼비활성
				pLottoWindow->SetEnableWindow(pRecvPacket->bStatus ? (true):(false));
			}

			SetLottoActive(pRecvPacket->bStatus);						// 로또활성비활성(추첨중에는비활성)

			SetLottoNumber(pRecvPacket->i64TotalAccum);					// 동기화를위해 로또NPC도 갱신	
			
			//bFlag==false일경우 나머지 내용채울필요없다(창안열기때문)
			if(!pRecvPacket->bFlag) break;

			m_LottoInfo.m_bNumTickets	= pRecvPacket->bNumTickets;		// 현재 소지하고 있는 복권 용지 수
			pLottoWindow->SetItemNum(m_LottoInfo.m_bNumTickets);		
			m_LottoInfo.m_i64TotalAccum = pRecvPacket->i64TotalAccum;	// 현재 총 적립액
			m_LottoInfo.m_wLottoDrawNum = pRecvPacket->wCurrentDrawNum;	// 현재 회차
			//--------------------------------
			//	지난회차정보가있을때(기본정보)
			//--------------------------------
			// 1회차 응모때는 지난회자 정보가 없기때문에 적립금처리부분이 처리안되고 SetLottoBaseInfo()도 호출안됨, 현재의 적립금액이 UI창에 표시 되지 못함
			// 해당 부분을 처리하기 위해 적립금처리부분과 SetLottoBaseInfo()호출 위치를 변경함. 그리고 if(pRecvPacket->bNumResultRecords>0) 주석처리함
			LOTTO_BASE_INFO baseinfo;
			memset(&baseinfo,0,sizeof(baseinfo));
			
			//당첨번호(UI는 하나밖에 없음)
			if(pRecvPacket->bNumResultRecords>0)
			{
				memcpy(baseinfo.arLuckyNum,&pRecvPacket->results[0].bWinningNum,sizeof(baseinfo.arLuckyNum));
				baseinfo.wDrawNum	 = pRecvPacket->results[0].wDrawNum;		// 몇회차					
				baseinfo.WinnerHeim  = pRecvPacket->results[0].i64Jackpot;		// 1등 당첨금
				baseinfo.dwWinnerNum = pRecvPacket->results[0].bNumWinners;		// 1등 당첨자수				
			}
			baseinfo.LottoHeim   = m_LottoInfo.m_i64TotalAccum;					// 적립금액
			pLottoWindow->SetLottoBaseInfo(baseinfo);

			//--------------------------------
			//	나의 응모정보가 있을때
			//  서버에서는 내림차순의로 정의되
			//	어져서 옴 2회차-1회차 이런식
			//--------------------------------
			if(pRecvPacket->bNumEntryRecords>0)
			{
				//LOTTO_APPLY_DATA 하나에 같은회차(wDrawNum)정보  최대5개씩들어갈수있다
				//아래구문은 서버에서 bNumEntryRecords갯수만큼 데이터가 날라오는데 이것들을
				//회차별로 구분삽입하기위한 구문이다 
				LOTTO_APPLY_DATA ApplyData;
				memset(&ApplyData,0,sizeof(ApplyData));
				int nIndexCnt=0;
				int nDrawNum = pRecvPacket->entries[0].wDrawNum;
				for(int i=0;i<pRecvPacket->bNumEntryRecords;++i)
				{
					//회차가 같을때는 같은곳에 집어넣는다
					if(nDrawNum==pRecvPacket->entries[i].wDrawNum)
					{
						//nIndexCnt의 최대값은 5개이다
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
						//회차가 바뀌었다면  기존기입된거삽입해주고(InsertApplyInfo)
						//지금 이데이터는 앞부분에 삽입한다
						pLottoWindow->InsertApplyInfo(ApplyData,TRUE);
						memset(&ApplyData,0,sizeof(ApplyData));
						
						//번호를 갱신 + 이건삽입한다
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
			//5291 복권용지를 획득 하였습니다.
			MSG_CW_LOTTO_TICKET_NTF *pRecvPacket = (MSG_CW_LOTTO_TICKET_NTF *)pMsg;
			m_LottoInfo.m_bNumTickets = pRecvPacket->bNumTickets;
			g_InterfaceManager.GetInterfaceString(5291,szMessage);
			LottoMessage(szMessage,FALSE);
		}
		break;
	case CW_LOTTO_ENTRY_ACK:      
		{	
			MSG_CW_LOTTO_ENTRY_ACK *pRecvPacket = (MSG_CW_LOTTO_ENTRY_ACK *)pMsg;
			//5295 응모에 성공하였습니다.
			g_InterfaceManager.GetInterfaceString(5295,szMessage);
			LottoMessage(szMessage);	

			uiLottoWindow* pLottoWindow= GET_CAST_DIALOG( uiLottoWindow, IM_LOTTO_MANAGER::LOTTO_WINDOW_DLG );
			if(pLottoWindow)
			{
				//로또창에 정보를 갱신해준다.
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
			//5296	응모가 실패 하였습니다.
			g_InterfaceManager.GetInterfaceString(5296,szMessage);
			LottoMessage(szMessage);

			switch(pRecvPacket->bReason)
			{
			case MSG_CW_LOTTO_ENTRY_NAK::NOT_READY:// 하임 로또가 진행 중이 아님
				{
					
					//5288: 당첨자 추첨중에는 응모할수없습니다.
					g_InterfaceManager.GetInterfaceString(5288,szMessage);
					LottoMessage(szMessage,FALSE);
					_asm nop;
				}
				break;
			case MSG_CW_LOTTO_ENTRY_NAK::OUT_OF_TICKET: // 하임 로또 용지 없음
				{
					//5289: 복권용지가 없습니다.
					g_InterfaceManager.GetInterfaceString(5289,szMessage);
					LottoMessage(szMessage,FALSE);
					_asm nop;
				}
				break;
            case MSG_CW_LOTTO_ENTRY_NAK::NOT_ENOUGH_MONEY:// 구매 금액 부족합니다.
                {
                    //551: 하임이 부족합니다.
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
			// 하임 로또 적립액 갱신 공지(로또 NPC기입)
			MSG_CW_LOTTO_ACCUM_BRD *pRecvPacket = (MSG_CW_LOTTO_ACCUM_BRD *)pMsg;
			SetLottoNumber(pRecvPacket->i64TotalAccum);
			
		}
		break;
	case CW_LOTTO_COUNTDOWN_BRD:  // 하임 로또 추첨 카운트다운 공지
		{
			MSG_CW_LOTTO_COUNTDOWN_BRD *pRecvPacket = (MSG_CW_LOTTO_COUNTDOWN_BRD *)pMsg;
			//5292	%d분 후 %d회 하임 로또 당첨번호를 추첨합니다.
			g_InterfaceManager.GetInterfaceString(5292,szTemp);
			int nMin = pRecvPacket->wSeconds/60;
            Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, nMin, pRecvPacket->wDrawNum);
			LottoMessage(szMessage);
		}
		break;

	case CW_LOTTO_START_BRD:
		{
			// 하임 로또 추첨/시작 공지
			MSG_CW_LOTTO_START_BRD*pRecvPacket = (MSG_CW_LOTTO_START_BRD *)pMsg;
			
			//로또창모두닫기
			SetLottoActive(TRUE);

			//로또창모두닫기
			closeAllLottoDlg();

			//5287 %d회 로또 추첨이 시작되었습니다.
			g_InterfaceManager.GetInterfaceString(5287,szTemp);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp,pRecvPacket->wDrawNum);
			LottoMessage(szMessage);
		}
		break;

	case CW_LOTTO_FINISH_BRD:     
		{
			// 하임 로또 추첨/종료 공지
			MSG_CW_LOTTO_FINISH_BRD *pRecvPacket = (MSG_CW_LOTTO_FINISH_BRD *)pMsg;

			SetLottoActive(FALSE);

			//로또창모두닫기
			closeAllLottoDlg();

			//5293	%d회 하임 로또 당첨번호는 %02d-%02d-%02d-%02d번입니다.
			g_InterfaceManager.GetInterfaceString(5293,szTemp);

			Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp,
									pRecvPacket->wDrawNum,		  //몇회	
									pRecvPacket->bWinningNum[0],  //첫번째
									pRecvPacket->bWinningNum[1],  //두번째
									pRecvPacket->bWinningNum[2],  //세번째
									pRecvPacket->bWinningNum[3]); //네번째
			
			LottoMessage(szMessage);
		}
		break;
	}
}


