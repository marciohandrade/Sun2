#ifndef AUTHSERVER_INTEGRATIONBILLINGSERVERSESSION_H
#define AUTHSERVER_INTEGRATIONBILLINGSERVERSESSION_H

#pragma once

#ifdef __NA_20100201_INTERGRATEDBILLING_

#include <ClientStub/WBANetwork/WBANetwork.h>
#include <ClientStub/BillEventHandler.h>
#include "IntegratedBillingServerSessionOb.h"

class IntegratedBillingServerSessionLib : public CBillEventHandler
{
public:

    IntegratedBillingServerSessionLib(void);
    ~IntegratedBillingServerSessionLib(void);

public:
    void SetBillingInterface(IntegratedBillingServerSessionInterface* BillingInterface) {m_BillingInterface = BillingInterface;};

protected:		
    //���������� �α׸� �ۼ� �̺�Ʈ �̴�.
    //�Լ����ο��� �α׸� ����� �ֵ��� �ؾ��Ѵ�.
    void WriteLog(char* szMsg);

    //���������� ���� ���� ���� (���� ����Ǹ� ���� ������ �ʿ��� ��� ������ ������ �ٽ� �α����ؾ��Ѵ�.)
    //		success: ���� ���� ����
    //		error: �����ڵ�
    void OnNetConnect(bool success, DWORD error);

    //���������� ���� ����Ÿ ���� ����
    //		size: ���۵� ��Ŷ ������
    void OnNetSend( int size );

    //���������� ������ �������� (���������� ���������� ó�� �ؾ��Ѵ�.)	
    //		error: �����ڵ�
    void OnNetClose( DWORD error );	

    //������ ���� ������ ����Ǿ��� ��
    //		dwBillingGUID: ���� ����ũ ���̵�
    //		RealEndDate: ���� ��� ���� �Ͻ� (__time32_t) 
    //		EndDate: ���� ���� ���� �Ͻ�
    //		dwRestPoint: ���� �ܿ� ����Ʈ 
    //		nDeductType: ����� ���� ����
    //		nAccessCheck: ���� üũ ���
    //		nResultCode: ó�����
    void OnUserStatus(DWORD dwAccountGUID, 
        long dwBillingGUID, 
        DWORD RealEndDate, 
        DWORD EndDate, 
        double dRestPoint, 
        double dRestTime, 
        short nDeductType, 
        short nAccessCheck, 
        short nResultCode);

    // ���� Ÿ�� ��ȸ
    void OnInquireDeductType(long AccountID, long Result);

    //PC �� ��Ƽ ���� ��ȸ ��� 
    //		��� �ڵ�
    //		1 : ����
    //		2 : �Ұ�
    //		-1 : PC�� ���� ���� 
    void OnInquireMultiUser(long AccountID, long RoomGUID, long Result);

    // Ư�� PC���� ���� �� ���� ���� ���θ� ��ȸ�Ѵ�.    
    void OnInquirePCRoomPoint(long AccountGUID, 
        long RoomGUID, 
        long GameCode, 
        long ResultCode);

	// ������ ���� �� ����, ���� ���� ���θ� ��ȸ�Ѵ�.
	void OnInquirePersonDeduct(long AccountGUID, 
		long GameCode, 
		long ResultCode);

	// 2010.09.07 ������ �α����� ���� ������ ���������� ����
	// ������ ���������� �α��� �ߴ��� üũ�ϵ��� �˷��ִ� �������̽��̴�.
	// �α��� �� 3�п� �ѹ��� ���� �ȴ�.
	// ��ǰ�� ����Ǵ� ������, �α׾ƿ� �Ǹ� ���۵��� �ʴ´�.
	void OnCheckLoginUser(long AccountID, long BillingGUID);

    //PC �� ������ ������ ����Ǿ����� ������Ʈ �Ѵ�.
    void OnPCRoomIPUpdate(long RoomGUID, long GameCode);

private:
    IntegratedBillingServerSessionInterface* m_BillingInterface;
};

#endif //__NA_20100201_INTERGRATEDBILLING_

#endif //AUTHSERVER_INTEGRATIONBILLINGSERVERSESSION_H







