#pragma once

// ���� ���� ���� ����

#include <ImportLibrary.h>		// �ʿ��� ClientStub ���̺귯�� import
#include <Winsock2.h>
#include <WBANetwork/WBANetwork.h>
#include "BillEventHandler.h"

class CLibClientSession : public CBillEventHandler
{
public:
	CLibClientSession(void);
	virtual  ~CLibClientSession(void);

protected:		
	// ���������� �α׸� �ۼ� �̺�Ʈ : �Լ����ο��� �α׸� ����� �ֵ��� �ؾ� ��.
	void WriteLog(char* szMsg);

	// ���������� ���� ���� ����
	// (���� ����Ǹ� ���� ������ �ʿ��� ��� ������ ������ �ٽ� �α����ؾ��Ѵ�.)
	//		success	: ���� ���� ����
	//		error	: �����ڵ�
	void OnNetConnect(bool success, DWORD error);
	
	// ���������� ���� ����Ÿ ���� ����
	//		size	: ���۵� ��Ŷ ������
	void OnNetSend(int size);
	
	// ���������� ������ �������� (���������� ���������� ó�� �ؾ��Ѵ�.)	
	//		error	: �����ڵ�
	void OnNetClose(DWORD error);	

	// ������ ���� ������ ����Ǿ��� �� ���޹޴� �ݹ�
	// ���� �α����� ��� : (0 == nResultCode) && (0 == nAccessCheck) && (0 != nDeductType)
	void OnUserStatus(
		DWORD	dwAccountGUID,		// ���� GUID
		long	dwBillingGUID,		// ���� ����ũ ���̵�
		DWORD	RealEndDate,		// ���� ��� ���� �Ͻ� (__time32_t) 
		DWORD	EndDate,			// ���� ���� ���� �Ͻ�
		double	dRestPoint,			// ���� �ܿ� ����Ʈ
		double	dRestTime,			// ���� �ܿ� �ð�(��)
		short	nDeductType,		// ����� ���� ����
		short	nAccessCheck,		// ���� üũ ���
		short	nResultCode);		// ó�� ���

	// PC �� ��Ƽ ���� ��ȸ ��� 
	//		Result : 1 (����), 2 (�Ұ�), -1 (PC�� ���� ����)
	void OnInquireMultiUser(
		long	AccountID,			// ���� GUID
		long	RoomGUID,			// PC �� GUID
		long	Result);			// ó�� ���

	// ������ ���� �� ����, ���� ���� ���� ��ȸ ���
	void OnInquirePersonDeduct(
		long	AccountGUID, 		// ���� GUID
		long	GameCode, 			// ���� �ڵ�
		long	ResultCode);		// ó�� ���

	// Ư�� PC���� ���� �� ���� ���� ���� ��ȸ ���
	void OnInquirePCRoomPoint(
		long	AccountGUID,  		// ���� GUID
		long	RoomGUID,			// PC �� GUID
		long	GameCode,			// ���� �ڵ�
		long	ResultCode);		// ó�� ���

	// ������ �α����� ���� ������ ���������� ����
	// ������ ���������� �α��� �ߴ��� üũ�ϵ��� �˷��ִ� �������̽�
	// ������ ���ؼ��� �α��� �� �� 3�п� �ѹ��� ����.
	// ��ǰ�� ����Ǵ� ������, �α׾ƿ� �Ǹ� ���۵��� �ʴ´�.
	void OnCheckLoginUser(
		long	AccountID,			// ���� GUID
		long	BillingGUID);		// ���� ����ũ ���̵�(�α��� ���� �� OnUserStatus �Լ��� ���� ���޵� ��)


	// ���������� ������ ���� PC�� ���ְ� PC�� IP�� �������� ��,
	// ���������� ���ӵǾ��� �ִ� ���Ӽ����� IP������ PC�� GUID�� �˷���.
	void OnPCRoomIPUpdate(
		long	RoomGUID,			// PC �� GUID
		long	GameCode);			// ó�� ���

private:
	CRITICAL_SECTION m_cs;
};
