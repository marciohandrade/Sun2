#pragma once

/**************************************************************************************************

�ۼ���: 2008-07-08
�ۼ���: ������ (youngmoon@webzen.co.kr)

����: ���������� ����ϴ� Ŭ���̾�Ʈ���� ���ŵ� ��Ŷ�� ������ �ֱ� ���� �̺�Ʈ ��ü 
      �̺�Ʈ�� �޴���(����Ŭ���̾�Ʈ)���� ��ӹ޾� ����ؾ� �Ѵ�.

**************************************************************************************************/

#include "ClientSession.h"

class CBillEventHandler : public CClientSession
{
public:
	CBillEventHandler(void);
	virtual ~CBillEventHandler(void);

	// �ʱ�ȭ �Լ� (���α׷� �����ϰ� �ѹ��� ����ؾ� �Ѵ�.)
	bool CreateSession(WBANetwork::LPFN_ErrorHandler errhandler = NULL);

	// ���� �α��� ��û
	bool UserLogin(
		DWORD	dwAccountGUID,		// ���� GUID
		DWORD	dwIPAddress,		// Ŭ���̾�Ʈ ������ �ּ�
		DWORD	dwRoomGUID,			// PC �� GUID (������� 0), PC �� GUID�� ���� DB���� ��ȸ �Ͻø� �˴ϴ�. (PC�� ������ ���� DB�� ���� DB ����ȭ ��)
		DWORD	dwGameCode,			// ���� �ڵ�
		DWORD	dwServerType);		// ���� ���� �ڵ�

	// ���� �α׾ƿ� ��û
	bool UserLogout(
		long	dwBillingGUID);		// ���� ����ũ ���̵�(�α��� ���� �� OnUserStatus �Լ��� ���� ���޵� ��)

	// ���� �α��� ���� ��ȸ ��û
	bool InquireUser(
		long	dwBillingGUID);		// ���� ����ũ ���̵�(�α��� ���� �� OnUserStatus �Լ��� ���� ���޵� ��)

	// PC ���� ��Ƽ ���� ���� ���� ��ȸ
	bool InquireMultiUser(
		long	AccountGUID,		// ���� GUID
		long	RoomGUID);			// PC �� GUID

	// ���� ����/���� ���� ���� ��ȸ
	bool InquirePersonDeduct(
		long	AccountGUID,		// ���� GUID
		long	GameCode);			// ���� �ڵ�

	// Ư�� PC���� ���� �� ���� ���� ���� ��ȸ
	bool InquirePCRoomPoint(
		long	AccountGUID,		// ���� GUID
		long	RoomGUID,			// PC �� GUID
		long	GameCode);			// ���� �ڵ�

protected:
	// [do not override] - �α��� ������ ����ó�� �Լ�(On...()) ȣ��
	void OnReceive(PBYTE buffer, int size);

	// [do not override] - �α븸 �ϰ� OnNetConnect() ȣ��
	void OnConnect(bool success, DWORD error);

	// [do not override] - �α븸 �ϰ� OnNetSend() ȣ��
	void OnSend(int size);

	// [do not override] - �α븸 �ϰ� OnNetClose() ȣ��
	void OnClose(DWORD error);	


protected:
	
	// ������ ���� ������ ����Ǿ��� �� ���޹޴� �ݹ�
	// ���� �α����� ��� : (0 == nResultCode) && (0 == nAccessCheck) && (0 != nDeductType)
	virtual void OnUserStatus(
		DWORD	dwAccountGUID,		// ���� GUID
		long	dwBillingGUID,		// ���� ����ũ ���̵�
		DWORD	RealEndDate,		// ���� ��� ���� �Ͻ� (__time32_t) 
		DWORD	EndDate,			// ���� ���� ���� �Ͻ�
		double	dRestPoint,			// ���� �ܿ� ����Ʈ
		double	dRestTime,			// ���� �ܿ� �ð�(��)
		short	nDeductType,		// ����� ���� ����
		short	nAccessCheck,		// ���� üũ ���
		short	nResultCode)		// ó�� ���
		= 0;

	// ������ ���� �� ����, ���� ���� ���� ��ȸ ���
	virtual void OnInquirePersonDeduct(
		long	AccountGUID, 		// ���� GUID
		long	GameCode, 			// ���� �ڵ�
		long	ResultCode)			// ó�� ���
		= 0;

	// Ư�� PC���� ���� �� ���� ���� ���� ��ȸ ���
	virtual void OnInquirePCRoomPoint(
		long	AccountGUID,  		// ���� GUID
		long	RoomGUID,			// PC �� GUID
		long	GameCode,			// ���� �ڵ�
		long	ResultCode)			// ó�� ���
		= 0;

	// PC �� ��Ƽ ���� ��ȸ ��� 
	//		Result : 1 (����), 2 (�Ұ�), -1 (PC�� ���� ����)
	virtual void OnInquireMultiUser(
		long	AccountID,			// ���� GUID
		long	RoomGUID,			// PC �� GUID
		long	Result)				// ó�� ���
		= 0;	

	// ������ �α����� ���� ������ ���������� ����
	// ������ ���������� �α��� �ߴ��� üũ�ϵ��� �˷��ִ� �������̽�
	// ������ ���ؼ��� �α��� �� �� 3�п� �ѹ��� ����.
	// ��ǰ�� ����Ǵ� ������, �α׾ƿ� �Ǹ� ���۵��� �ʴ´�.
	virtual void OnCheckLoginUser(
		long	AccountID,			// ���� GUID
		long	BillingGUID)		// ���� ����ũ ���̵�(�α��� ���� �� OnUserStatus �Լ��� ���� ���޵� ��)
		= 0;

	// ���������� ������ ���� PC�� ���ְ� PC�� IP�� �������� ��,
	// ���������� ���ӵǾ��� �ִ� ���Ӽ����� IP������ PC�� GUID�� �˷���.
	virtual void OnPCRoomIPUpdate(
		long	RoomGUID,			// PC �� GUID
		long	GameCode)			// ó�� ���
		= 0;

	// ���������� ���� ���� ����
	// (���� ����Ǹ� ���� ������ �ʿ��� ��� ������ ������ �ٽ� �α����ؾ��Ѵ�.)
	//		success	: ���� ���� ����
	//		error	: �����ڵ�
	virtual void OnNetConnect(bool success, DWORD error) = 0;
		
	// ���������� ���� ����Ÿ ���� ����
	//		size	: ���۵� ��Ŷ ������
	virtual void OnNetSend(int size) = 0;
	
	// ���������� ������ �������� (���������� ���������� ó�� �ؾ��Ѵ�.)	
	//		error	: �����ڵ�
	virtual void OnNetClose( DWORD error ) = 0;	
	
	//�α׸� ����Ҷ� ������ �̺�Ʈ �޼ҵ�
	virtual void WriteLog(char* szMsg) = 0;

private:
	// ���������� �α׸� �ۼ� �̺�Ʈ : �Լ����ο��� �α׸� ����� �ֵ��� �ؾ� ��.
	void WriteLog(const char* szFormat, ...);
};
