#ifndef AUTHSERVER_BILLINGSERVERCOMMONIMPL_H
#define AUTHSERVER_BILLINGSERVERCOMMONIMPL_H

#pragma once

class BillingServerCommonImpl
{
public:

    BillingServerCommonImpl(void);
    ~BillingServerCommonImpl(void);

public:
    //PC�� ���� ����
    void OnBS_BILLING_EXPIRED_NTF(DWORD dwUserGUID, DWORD dwPCRoomGUID, DWORD dwResult, short nAccessCheck = 0, short nDeductType = 0);
    //PC�� ��ȸ ���
    void OnBS_BILLING_RESTPOINT_ACK(DWORD user_guid, DWORD pc_room_id, BOOLEAN is_charged_billing);
    void SendPCRoomStatus(AccountInfo* pAccount);
private:
    
};

#endif //AUTHSERVER_BILLINGSERVERCOMMONIMPL_H







