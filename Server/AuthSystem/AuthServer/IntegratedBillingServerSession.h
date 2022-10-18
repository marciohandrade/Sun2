#ifndef AUTHSERVER_INTEGRATIONBILLINGSERVERADAPTOR_H
#define AUTHSERVER_INTEGRATIONBILLINGSERVERADAPTOR_H

#pragma once

#ifdef __NA_20100201_INTERGRATEDBILLING_

//libcmtd.lib; 
//#pragma comment(linker, "/nodefaultlib:libcmtd.lib")
//#pragma comment(linker, "/nodefaultlib:libcmt.lib")

#include <WxMessageIpc.h>
#include <ServerSession.h>
#include "IntegratedBillingServerSessionLib.h"
#include "IntegratedBillingServerSessionOb.h"
#include "BillingServerCommonImpl.h"
#include "PCRoomUserManager.h"

//==============================================================================

struct BillingMsg : public util::WX_INTER_MSG_HEADER
{
    enum eBillingMsgProtocol
    {
        eBillingMsg_None = 0,
        eBillingMsg_OnDisconnectLib,
        eBillingMsg_OnConnectLib,
        eBillingMsg_OnUserStatus,
        eBillingMsg_OnInquirePCRoomPoint,
        eBillingMsg_OnInquirePersonDeduct,
        eBillingMsg_OnCheckLoginUser,
        eBillingMsg_OnPCRoomIPUpdate,
        //
        eBillingMsg_Counts
    };

    union
    {
        struct {
        } OnDisconnectLib;
        struct {
            bool success;
        } OnConnectLib;
        struct {
            DWORD dwAccountGUID;
            long dwBillingGUID;
            DWORD RealEndDate;
            DWORD EndDate;
            double dRestPoint;
            double dRestTime;
            short nDeductType;
            short nAccessCheck;
            short nResultCode;
        } OnUserStatus;
        struct {
            long AccountID;
            long CompAccountSeq;
            long ResultCode;
        } OnInquirePCRoomPoint;
        struct {
            long AccountGUID;
            long ResultCode;
        } OnInquirePersonDeduct;
        struct {
            long AccountID;
            long BillingGUID;
        } OnCheckLoginUser;
        struct {
            long RoomGUID;
            long GameCode;
        } OnPCRoomIPUpdate;
    };
};

//==============================================================================

class IntegratedBillingServerSession
    : public ServerSession
    , public IntegratedBillingServerSessionInterface
    // CHANGES: f110830.4L, detach the main thread context
    // from CBillEventHandler worker thread contexts
    , public util::IWxMainMessageHandler
{
public:

    IntegratedBillingServerSession(void);
    ~IntegratedBillingServerSession(void);

    inline virtual eSERVER_TYPE GetServerType() { return BILLING_SERVER; }
    //������ �����Ѵ�.
    void                Connect();
    //���̺귯���� �̿��ϱ� ���� �ʱ�ȭ�Ѵ�.
    void DoInit();
    //���̺귯���� ��Ʈ�� ������ �ޱ� ���� �ݹ� �޼ҵ�
    static void ErrorHandler( DWORD lastError, char* desc );
    
    //����ڰ� �α��� �ϸ� ���������� �˸���.
    void SendLogin(AccountInfo& info);
    //����ڰ� �α׾ƿ� �ϸ� ���������� �˸���.
    void SendLogout(AccountInfo& info);
	void SendLogout(long intergratebilling_guid);
    //������� �ܿ�����Ʈ�� ��ȸ �Ѵ�.
    void QueryRestPoint(AccountInfo* account);
	//���� ���� ������ Ȯ���Ѵ�.
	void InquirePersonDeduct(AccountInfo& account);
    void InquireMultiUser(AccountInfo& account);

#ifdef _NA_005011_20120621_MULTICLIENTS
    bool CheckMultiUserTypeAndLogin(AccountInfo& info);
    void CheckIn(AccountInfo& info);
    void CheckOut(AccountInfo& info);

    PCRoomUserManager pcroom_user_manager_;
#endif //_NA_005011_20120621_MULTICLIENTS

public:
    // util::IWxMainMessageHandler interface
    // CHANGES: f110830.4L, detach the main thread context
    // from CBillEventHandler worker thread contexts
    // NOTE: this handler is worked in the main thread context
    virtual bool OnReceive(
        util::WxMessageIpc::MainLogicHandler* const handler,
        const util::WX_INTER_MSG_HEADER* msg_base, size_t msg_size);

    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    //���������� ������ ��������
    virtual void OnDisconnectLib();
    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    //���������� ������ ����������
    virtual void OnConnectLib(bool success);
    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    //PC�� �ܾ� ��ȸ ���
    virtual void OnInquirePCRoomPoint(long AccountID,
        long CompAccountSeq, long ResultCode);

    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    // ��� ä�ο��� ���� ���� ���� Ȯ�� ���.
    virtual void OnInquirePersonDeduct(long AccountGUID, long ResultCode);
    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    //������� ���°� ����Ǿ�����
    virtual void OnUserStatus(
        DWORD dwAccountGUID,
        long dwBillingGUID,
        DWORD RealEndDate, DWORD EndDate,
        double dRestPoint, double dRestTime,
        short nDeductType, short nAccessCheck, short nResultCode);

    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    // 2010.09.07 ������ �α����� ���� ������ ���������� ����
    // ������ ���������� �α��� �ߴ��� üũ�ϵ��� �˷��ִ� �������̽��̴�.
    // �α��� �� 3�п� �ѹ��� ���� �ȴ�.
    // ��ǰ�� ����Ǵ� ������, �α׾ƿ� �Ǹ� ���۵��� �ʴ´�.
    virtual void OnCheckLoginUser(long AccountID, long BillingGUID);

    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    //PC�� ������ ������ ����Ǹ� �뺸�ȴ�.
    virtual void OnPCRoomIPUpdate(long RoomGUID, long GameCode);

    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    //PC�� ������ ������ ����Ǹ� �뺸�ȴ�.
    virtual void OnInquireMultiUser(long AccountGUID, long RoomGUID, long Result);

private:
    enum {DEDUCT_TYPE_GOLDCHANNEL = 1, DEDUCT_TYPE_COLOR = 3, DEDUCT_TYPE_NORMAL = 6};
    // CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
    virtual void OnRedirect(DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);

    //�㿡�� ���Ǵ� ������� �÷�������� �Ǿ��� �Ϲ� �������̴�.
    //�÷�������� �Ϲ� �������� ���������� �Ⱓ������ �ִ�.
    BOOL                IsLibInit;
    IntegratedBillingServerSessionLib BillingLib;
    BillingServerCommonImpl BillingCommonImpl;

    enum eBILLING_SERVER_TYPE
    {
        eBILLING_SERVER_TYPE_TEST = 697,
        eBILLING_SERVER_TYPE_LIVE = 696,
    };
    eBILLING_SERVER_TYPE ServerType;

#ifdef _AUTH_KOREA
    static const int INTERGRATEDBILLING_GAMECODE    = 113;    
#elif defined(_AUTH_GLOBAL) || defined(_AUTH_RUSSIA)
	static const int INTERGRATEDBILLING_GAMECODE    = 503;
#else 
	#error "Impossible Build!! - Reason: Not Define Intergrated Billing GameCode"
#endif
	
};

// CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
inline void IntegratedBillingServerSession::OnRedirect(DWORD, MSG_SERVER_TYPE* const)
{
    assert(!"this session can't use this method");
}

#endif //__NA_20100201_INTERGRATEDBILLING_

#endif //AUTHSERVER_INTEGRATIONBILLINGSERVERADAPTOR_H







