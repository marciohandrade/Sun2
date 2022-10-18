#pragma once

// 샵서버 연동 샘플 : 아크로드(AR) 전용

#include <ImportLibrary.h>		// 필요한 ClientStub 라이브러리 import
#include <Winsock2.h>
#include <WBANetwork/WBANetwork.h>
#include "ShopEventHandler_AR.h"

class CShopEH_AR : public CShopEventHandler_AR
{
public:
	CShopEH_AR(void);
	virtual  ~CShopEH_AR(void);

protected:
	// 샵서버 로그 작성 이벤트 : 함수내부에서 로그를 남길수 있도록 해야 함.
	void WriteLog(char* szMsg);

	// 샵서버의 연결 성공 여부
	//		success	: 연결 성공 여부
	//		error	: 에러코드
	void OnNetConnect(bool success, DWORD error);

	// 샵서버로 보낸 데이타 전송 여부 
	//		size : 전송된 패킷 사이즈
	void OnNetSend(int size);
	
	// 샵서버와 세션이 끊겼을때 (샵서버가 죽은것으로 처리 해야한다.)	
	//		error: 에러코드
	void OnNetClose(DWORD error);

	// 캐시조회 결과 : AR_KR
	void OnInquireCash(
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNT_LENGTH], 
		double	CashSum, 
		double	PointSum, 
		ShopProtocol::STCashDetail_GB	Detail[], 
		long nCashInfoCount, 
		long ResultCode);

	// 상품구매 결과 : AR_KR
	void OnBuyProduct(
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNT_LENGTH], 
		DWORD	DeductCashSeq, 
		DWORD	InGamePurchaseSeq, 
		long	ResultCode);

	// 상품선물 결과 : AR_KR
	void OnGiftProduct(
		DWORD	AccountSeq, 
		DWORD	ReceiverSeq, 
		DWORD	DeductCashSeq, 
		long	ResultCode);

	// 캐시조회 결과 : AR_TW
	void OnInquireCash(
		char	AccountID[MAX_ACCOUNT_LENGTH], 
		double	Cash,
		long	ResultCode);


	// 상품구매 결과 : AR_TW
	void OnBuyProduct(
		char	AccountID[MAX_ACCOUNT_LENGTH], 
		INT64	ItemNo,
		INT64	OrderNo,
		int		OrderNumber,					
		WCHAR	RetMessage[MAX_RETMESSAGE_LENGTH],
		long	ResultCode);

private:
	CRITICAL_SECTION m_cs;		// 로깅을 위한 lock
};
