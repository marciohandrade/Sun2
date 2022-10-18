#pragma once

// ������ ���� ���� : ��ũ�ε�(AR) ����

#include <ImportLibrary.h>		// �ʿ��� ClientStub ���̺귯�� import
#include <Winsock2.h>
#include <WBANetwork/WBANetwork.h>
#include "ShopEventHandler_AR.h"

class CShopEH_AR : public CShopEventHandler_AR
{
public:
	CShopEH_AR(void);
	virtual  ~CShopEH_AR(void);

protected:
	// ������ �α� �ۼ� �̺�Ʈ : �Լ����ο��� �α׸� ����� �ֵ��� �ؾ� ��.
	void WriteLog(char* szMsg);

	// �������� ���� ���� ����
	//		success	: ���� ���� ����
	//		error	: �����ڵ�
	void OnNetConnect(bool success, DWORD error);

	// �������� ���� ����Ÿ ���� ���� 
	//		size : ���۵� ��Ŷ ������
	void OnNetSend(int size);
	
	// �������� ������ �������� (�������� ���������� ó�� �ؾ��Ѵ�.)	
	//		error: �����ڵ�
	void OnNetClose(DWORD error);

	// ĳ����ȸ ��� : AR_KR
	void OnInquireCash(
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNT_LENGTH], 
		double	CashSum, 
		double	PointSum, 
		ShopProtocol::STCashDetail_GB	Detail[], 
		long nCashInfoCount, 
		long ResultCode);

	// ��ǰ���� ��� : AR_KR
	void OnBuyProduct(
		DWORD	AccountSeq, 
		char	AccountID[MAX_ACCOUNT_LENGTH], 
		DWORD	DeductCashSeq, 
		DWORD	InGamePurchaseSeq, 
		long	ResultCode);

	// ��ǰ���� ��� : AR_KR
	void OnGiftProduct(
		DWORD	AccountSeq, 
		DWORD	ReceiverSeq, 
		DWORD	DeductCashSeq, 
		long	ResultCode);

	// ĳ����ȸ ��� : AR_TW
	void OnInquireCash(
		char	AccountID[MAX_ACCOUNT_LENGTH], 
		double	Cash,
		long	ResultCode);


	// ��ǰ���� ��� : AR_TW
	void OnBuyProduct(
		char	AccountID[MAX_ACCOUNT_LENGTH], 
		INT64	ItemNo,
		INT64	OrderNo,
		int		OrderNumber,					
		WCHAR	RetMessage[MAX_RETMESSAGE_LENGTH],
		long	ResultCode);

private:
	CRITICAL_SECTION m_cs;		// �α��� ���� lock
};
