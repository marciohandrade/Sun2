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
    //서버에 연결한다.
    void                Connect();
    //라이브러리를 이용하기 위해 초기화한다.
    void DoInit();
    //라이브러리의 네트웍 오류를 받기 위한 콜백 메소드
    static void ErrorHandler( DWORD lastError, char* desc );
    
    //사용자가 로그인 하면 빌링서버에 알린다.
    void SendLogin(AccountInfo& info);
    //사용자가 로그아웃 하면 빌링서버에 알린다.
    void SendLogout(AccountInfo& info);
	void SendLogout(long intergratebilling_guid);
    //사용자의 잔여포인트를 조회 한다.
    void QueryRestPoint(AccountInfo* account);
	//개인 정량 정액을 확인한다.
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
    //빌링서버와 연결이 끊겼을때
    virtual void OnDisconnectLib();
    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    //빌링서버와 연결이 성공했을때
    virtual void OnConnectLib(bool success);
    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    //PC방 잔액 조회 결과
    virtual void OnInquirePCRoomPoint(long AccountID,
        long CompAccountSeq, long ResultCode);

    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    // 골드 채널에서 개인 정량 정액 확인 결과.
    virtual void OnInquirePersonDeduct(long AccountGUID, long ResultCode);
    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    //사용자의 상태가 변경되었을때
    virtual void OnUserStatus(
        DWORD dwAccountGUID,
        long dwBillingGUID,
        DWORD RealEndDate, DWORD EndDate,
        double dRestPoint, double dRestTime,
        short nDeductType, short nAccessCheck, short nResultCode);

    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    // 2010.09.07 빌링에 로그인한 유저 정보를 인증서버에 전송
    // 실제로 인증서버에 로그인 했는지 체크하도록 알려주는 인터페이스이다.
    // 로그인 후 3분에 한번씩 전송 된다.
    // 상품이 변경되는 시점과, 로그아웃 되면 전송되지 않는다.
    virtual void OnCheckLoginUser(long AccountID, long BillingGUID);

    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    //PC방 아이피 정보가 변경되면 통보된다.
    virtual void OnPCRoomIPUpdate(long RoomGUID, long GameCode);

    // WARNING: IntegratedBillingServerSessionInterface interface, thread#n
    //PC방 아이피 정보가 변경되면 통보된다.
    virtual void OnInquireMultiUser(long AccountGUID, long RoomGUID, long Result);

private:
    enum {DEDUCT_TYPE_GOLDCHANNEL = 1, DEDUCT_TYPE_COLOR = 3, DEDUCT_TYPE_NORMAL = 6};
    // CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
    virtual void OnRedirect(DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);

    //썬에서 사용되는 요금제는 컬러요금제와 피씨방 일반 정량제이다.
    //컬러요금제는 일방 정량제와 동일하지만 기간제한이 있다.
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







