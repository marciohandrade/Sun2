#include "StdAfx.h"

#ifdef __NA_20100201_INTERGRATEDBILLING_

#include "AuthFrame.h"

#include "IntegratedBillingServerSession.h"
#include "AccountManager.h"
#include "QueryManager.h"


IntegratedBillingServerSession::IntegratedBillingServerSession(void) : IsLibInit(FALSE)
{    
    BillingLib.SetBillingInterface(this);        
}

IntegratedBillingServerSession::~IntegratedBillingServerSession(void)
{
    
}

void IntegratedBillingServerSession::Connect()
{
    if(!IsLibInit)
    {
        DoInit();
        if(!IsLibInit) return;
    }

    SetConnecting();
    BillingLib.Connect(const_cast<char*>(GetConnectIP().c_str()), GetConnectPort());    
}

void IntegratedBillingServerSession::DoInit()
{
    if(BillingLib.CreateSession(IntegratedBillingServerSession::ErrorHandler))
    {
        ServerType = GetFrame()->GetMode() == eSERVER_LIVE ? eBILLING_SERVER_TYPE_LIVE : eBILLING_SERVER_TYPE_TEST;
        IsLibInit = TRUE;
    }
}

void IntegratedBillingServerSession::ErrorHandler( DWORD lastError, char* desc )
{
    SUNLOG(eCRITICAL_LOG, "[Billing_NetWork_Msg] %s (%d)", desc, lastError);
}

void IntegratedBillingServerSession::SendLogin(AccountInfo& info)
{
    DWORD ip_value = ::inet_addr(info.m_clientIp);    
    if(!BillingLib.UserLogin(info.m_userGuid, ip_value,info.m_pcBangId, INTERGRATEDBILLING_GAMECODE, ServerType))
    {
        SUNLOG(eCRITICAL_LOG, "Send Login Fial (%u)", info.m_userGuid);
    }
}

void IntegratedBillingServerSession::SendLogout(AccountInfo& info)
{
	SendLogout(info.IntergratedBillingGUID);
}

void IntegratedBillingServerSession::SendLogout(long intergratebilling_guid)
{
	if(!BillingLib.UserLogout(intergratebilling_guid))
	{
		SUNLOG(eCRITICAL_LOG, "Send LogOut Fial (%u)", intergratebilling_guid);
	} 
}

void IntegratedBillingServerSession::QueryRestPoint(AccountInfo* account)
{   
    if(!BillingLib.InquirePCRoomPoint(account->m_userGuid, account->m_pcBangId, INTERGRATEDBILLING_GAMECODE))
    {
        SUNLOG(eCRITICAL_LOG, "Send QueryRestPoint Fial (%u,%u)", account->m_userGuid, account->m_pcBangId);
    }     
}

void IntegratedBillingServerSession::InquirePersonDeduct(AccountInfo& account)
{   
	if (account.m_pcBangStatus.status_ == CHANGE_PCROOM_STATUS::eStatus_None)
	{
		account.m_pcBangStatus.status_ = CHANGE_PCROOM_STATUS::eStatus_PcRoomExpired;
	}

	if(!BillingLib.InquirePersonDeduct(account.m_userGuid, INTERGRATEDBILLING_GAMECODE))
	{
		SUNLOG(eCRITICAL_LOG, "Send InquirePersonDeduct Fial (%u)", account.m_userGuid);
	}     
}

void IntegratedBillingServerSession::InquireMultiUser(AccountInfo& account)
{
    if(!BillingLib.InquireMultiUser(account.m_userGuid, account.m_pcBangId))
    {
        SUNLOG(eCRITICAL_LOG, "Send InquireMultiUser Fial (%u)", account.m_userGuid);
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
// util::IWxMainMessageHandler interface
// CHANGES: f110830.4L, detach the main thread context
// from CBillEventHandler worker thread contexts
// NOTE: this handler is worked in the main thread context
bool IntegratedBillingServerSession::OnReceive(
    util::WxMessageIpc::MainLogicHandler* const handler,
    const util::WX_INTER_MSG_HEADER* msg_base, size_t msg_size)
{
    const BillingMsg* recv_msg = static_cast<const BillingMsg*>(msg_base);
    ASSERT(sizeof(*recv_msg) == msg_size);
    switch (recv_msg->protocol)
    {
    //case eBillingMsg_None:
    case recv_msg->eBillingMsg_OnDisconnectLib:
        ;{  //���������� ������ ��������
            ServerSession::OnDisconnect();
            return true;
        };
        break;
    case recv_msg->eBillingMsg_OnConnectLib:
        ;{  //���������� ������ ����������
            ServerSession::OnConnect(\
                recv_msg->OnConnectLib.success, GetSessionIndex());
            return true;
        };
        break;
    case recv_msg->eBillingMsg_OnUserStatus:
        ;{  //������� ���°� ����Ǿ�����
            AccountInfo* const account_info = \
                AccountManager::Instance()->FindWithUserGuid(\
                    recv_msg->OnUserStatus.dwAccountGUID);
            if (account_info == NULL)
            {
                SUNLOG(eCRITICAL_LOG,
                    _T("[eBillingMsg_OnUserStatus] account_info[%u] is NULL!"),
                    recv_msg->OnUserStatus.dwAccountGUID);
                return false;
            }

            if (0 == recv_msg->OnUserStatus.nResultCode)//��� ����
            {
                if (0 == recv_msg->OnUserStatus.nAccessCheck)//PC�� ����,���� ����
                {
                    account_info->IntergratedBillingGUID = \
                        recv_msg->OnUserStatus.dwBillingGUID;

                    short deduct_type = recv_msg->OnUserStatus.nDeductType;
                    if (deduct_type == DEDUCT_TYPE_COLOR ||
                        deduct_type == DEDUCT_TYPE_NORMAL ||
                        deduct_type == DEDUCT_TYPE_GOLDCHANNEL) //PC�� �����
                    {
                        account_info->m_pcBangStatus.status_ = \
                            account_info->m_pcBangStatus.eStatus_PcRoom;
#ifdef _NA_005011_20120621_MULTICLIENTS
                        BillingCommonImpl.SendPCRoomStatus(account_info);
#endif //_NA_005011_20120621_MULTICLIENTS
                        return true;
                    }
                    else
                    {
                        SUNLOG(eCRITICAL_LOG, "unexpected PCRoom type : %d", deduct_type);
                    }
                };
            }

            account_info->m_pcBangStatus.status_ = \
                account_info->m_pcBangStatus.eStatus_PcRoomExpired;
            BillingCommonImpl.OnBS_BILLING_EXPIRED_NTF(\
                recv_msg->OnUserStatus.dwAccountGUID,
        #ifdef __20100916_BILLING_GODLCHANNEL__
                INTERGRATEDBILLING_GAMECODE,
        #else
                account_info->m_pcBangId,
        #endif
                recv_msg->OnUserStatus.nResultCode,
                recv_msg->OnUserStatus.nAccessCheck,
                recv_msg->OnUserStatus.nDeductType);
            return true;
        };
        break;
    case recv_msg->eBillingMsg_OnInquirePCRoomPoint:
        ;{  //PC�� �ܾ� ��ȸ ���
            BillingCommonImpl.OnBS_BILLING_RESTPOINT_ACK(\
                recv_msg->OnInquirePCRoomPoint.AccountID,
                recv_msg->OnInquirePCRoomPoint.CompAccountSeq,
                (recv_msg->OnInquirePCRoomPoint.ResultCode == 1));
            return true;
        };
        break;
    case recv_msg->eBillingMsg_OnInquirePersonDeduct:
        ;{  // ��� ä�ο��� ���� ���� ���� Ȯ�� ���.
            // �Ǿ��� GUID �� INTERGRATEDBILLING_GAMECODE ���� ������ ������
            // ��� ä�� ���񽺰� ���������� �Ǿ��� ó�� �����ؾ� �ϱ� �����̴�.
            // �Ǿ��� GUID �� 0�� �ָ� �Ǿ����� �ƴѰ����� �ν��ϹǷ�
            // 0�� �ƴ� �ƹ� ���̳� �ָ� �ȴ�.
            BillingCommonImpl.OnBS_BILLING_RESTPOINT_ACK(\
                recv_msg->OnInquirePersonDeduct.AccountGUID,
                INTERGRATEDBILLING_GAMECODE,
                (recv_msg->OnInquirePersonDeduct.ResultCode == 1));
            return true;
        };
        break;
    case recv_msg->eBillingMsg_OnCheckLoginUser:
        ;{
            // 2010.09.07 ������ �α����� ���� ������ ���������� ����
            // ������ ���������� �α��� �ߴ��� üũ�ϵ��� �˷��ִ� �������̽��̴�.
            // �α��� �� 3�п� �ѹ��� ���� �ȴ�.
            // ��ǰ�� ����Ǵ� ������, �α׾ƿ� �Ǹ� ���۵��� �ʴ´�.
            AccountInfo* const account_info = \
                AccountManager::Instance()->FindWithUserGuid(\
                    recv_msg->OnCheckLoginUser.AccountID);
            if (account_info == NULL) {
                SendLogout(recv_msg->OnCheckLoginUser.BillingGUID);
                return true;
            };
            return false;
        };
        break;
    case recv_msg->eBillingMsg_OnPCRoomIPUpdate:
        ;{  //PC�� ������ ������ ����Ǹ� �뺸�ȴ�.
            if (recv_msg->OnPCRoomIPUpdate.GameCode ==
                    INTERGRATEDBILLING_GAMECODE)
            {
                QueryManager* query_manager = QueryManager::Instance();
                query_manager->QueryPCRoomIPList(\
                    recv_msg->OnPCRoomIPUpdate.RoomGUID);
                return true;
            }
            else
            {
                SUNLOG(eCRITICAL_LOG,
                    _T("OnPCRoomIPUpdate::Not Matched GameCode (%d)"),
                    recv_msg->OnPCRoomIPUpdate.GameCode);
                return false;
            }
        };
        break;
    default:
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=")
               _T("unexpected message, protocol=%d, size=%d|"),
               recv_msg->protocol, msg_size);
        return false;
    };
    return false;
};

//==============================================================================
//==============================================================================
//==============================================================================

// WARNING: IntegratedBillingServerSessionInterface interface, thread#n
//���������� ������ ��������
void IntegratedBillingServerSession::OnDisconnectLib()
{
    BillingMsg billing_msg;
    billing_msg.protocol = billing_msg.eBillingMsg_OnDisconnectLib;
    //
    util::WxMessageIpc::WorkerHandler handler = \
        AuthFrame::Instance()->GetBillingIpc()->GetWorkerHandler();
    handler.SendMessageToMain(&billing_msg, sizeof(billing_msg));

#if SUN_CODE_BACKUP
    ServerSession::OnDisconnect();
#endif
}

// WARNING: IntegratedBillingServerSessionInterface interface, thread#n
//���������� ������ ����������
void IntegratedBillingServerSession::OnConnectLib(bool success)
{
    BillingMsg billing_msg;
    billing_msg.protocol = billing_msg.eBillingMsg_OnConnectLib;
    billing_msg.OnConnectLib.success = success;
    //
    util::WxMessageIpc::WorkerHandler handler = \
        AuthFrame::Instance()->GetBillingIpc()->GetWorkerHandler();
    handler.SendMessageToMain(&billing_msg, sizeof(billing_msg));

#if SUN_CODE_BACKUP
    ServerSession::OnConnect(success, GetSessionIndex());
#endif
}

// WARNING: IntegratedBillingServerSessionInterface interface, thread#n
//������� ���°� ����Ǿ�����
void IntegratedBillingServerSession::OnUserStatus(
    DWORD dwAccountGUID, long dwBillingGUID,
    DWORD RealEndDate, DWORD EndDate,
    double dRestPoint, double dRestTime,
    short nDeductType, short nAccessCheck, short nResultCode)
{
    BillingMsg billing_msg;
    billing_msg.protocol = billing_msg.eBillingMsg_OnUserStatus;
    billing_msg.OnUserStatus.dwAccountGUID = dwAccountGUID;
    billing_msg.OnUserStatus.dwBillingGUID = dwBillingGUID;
    billing_msg.OnUserStatus.RealEndDate = RealEndDate;
    billing_msg.OnUserStatus.EndDate = EndDate;
    billing_msg.OnUserStatus.dRestPoint = dRestPoint;
    billing_msg.OnUserStatus.dRestTime = dRestTime;
    billing_msg.OnUserStatus.nDeductType = nDeductType;
    billing_msg.OnUserStatus.nAccessCheck = nAccessCheck;
    billing_msg.OnUserStatus.nResultCode = nResultCode;
    //
    util::WxMessageIpc::WorkerHandler handler = \
        AuthFrame::Instance()->GetBillingIpc()->GetWorkerHandler();
    handler.SendMessageToMain(&billing_msg, sizeof(billing_msg));

#if SUN_CODE_BACKUP
    AccountInfo* pAccount = AccountManager::Instance()->FindWithUserGuid(dwAccountGUID);
    if (pAccount == 0)
    {
        SUNLOG(eCRITICAL_LOG, "[OnUserStatus] pAccount[%u] is NULL!", dwAccountGUID);
        return;
    }

    if (0 == nResultCode)//��� ����
    {
        if (0 == nAccessCheck)//PC�� ����,���� ����
        {
            pAccount->IntergratedBillingGUID = dwBillingGUID;

            if (DEDUCT_TYPE_COLOR == nDeductType || 
                DEDUCT_TYPE_NORMAL == nDeductType || 
                DEDUCT_TYPE_GOLDCHANNEL == nDeductType) //PC�� �����
            {
                pAccount->m_pcBangStatus.status_ = CHANGE_PCROOM_STATUS::eStatus_PcRoom;
                return;
            }
        };
    }

    pAccount->m_pcBangStatus.status_ = CHANGE_PCROOM_STATUS::eStatus_PcRoomExpired;
    BillingCommonImpl.OnBS_BILLING_EXPIRED_NTF(dwAccountGUID, 
#ifdef __20100916_BILLING_GODLCHANNEL__
        INTERGRATEDBILLING_GAMECODE,
#else
        pAccount->m_pcBangId, 
#endif
        nResultCode, nAccessCheck, nDeductType);
#endif //SUN_CODE_BACKUP
}

// WARNING: IntegratedBillingServerSessionInterface interface, thread#n
//PC�� �ܾ� ��ȸ ���
void IntegratedBillingServerSession::OnInquirePCRoomPoint(
    long AccountID, long CompAccountSeq,  long ResultCode)
{
    BillingMsg billing_msg;
    billing_msg.protocol = billing_msg.eBillingMsg_OnInquirePCRoomPoint;
    billing_msg.OnInquirePCRoomPoint.AccountID = AccountID;
    billing_msg.OnInquirePCRoomPoint.CompAccountSeq = CompAccountSeq;
    billing_msg.OnInquirePCRoomPoint.ResultCode = ResultCode;
    //
    util::WxMessageIpc::WorkerHandler handler = \
        AuthFrame::Instance()->GetBillingIpc()->GetWorkerHandler();
    handler.SendMessageToMain(&billing_msg, sizeof(billing_msg));

#if SUN_CODE_BACKUP
    BillingCommonImpl.OnBS_BILLING_RESTPOINT_ACK(\
        AccountID, CompAccountSeq, ResultCode == 1 ? TRUE : FALSE);
#endif
}

// WARNING: IntegratedBillingServerSessionInterface interface, thread#n
// ��� ä�ο��� ���� ���� ���� Ȯ�� ���.
void IntegratedBillingServerSession::OnInquirePersonDeduct(
    long AccountGUID, long ResultCode)
{
    BillingMsg billing_msg;
    billing_msg.protocol = billing_msg.eBillingMsg_OnInquirePersonDeduct;
    billing_msg.OnInquirePersonDeduct.AccountGUID = AccountGUID;
    billing_msg.OnInquirePersonDeduct.ResultCode = ResultCode;
    //
    util::WxMessageIpc::WorkerHandler handler = \
        AuthFrame::Instance()->GetBillingIpc()->GetWorkerHandler();
    handler.SendMessageToMain(&billing_msg, sizeof(billing_msg));

#if SUN_CODE_BACKUP
    // �Ǿ��� GUID �� INTERGRATEDBILLING_GAMECODE ���� ������ ������
    // ��� ä�� ���񽺰� ���������� �Ǿ��� ó�� �����ؾ� �ϱ� �����̴�.
    // �Ǿ��� GUID �� 0�� �ָ� �Ǿ����� �ƴѰ����� �ν��ϹǷ� 0�� �ƴ� �ƹ� ���̳� �ָ� �ȴ�.
    BillingCommonImpl.OnBS_BILLING_RESTPOINT_ACK(\
        AccountGUID, INTERGRATEDBILLING_GAMECODE, ResultCode == 1 ? TRUE : FALSE);
#endif
}

// WARNING: IntegratedBillingServerSessionInterface interface, thread#n
// 2010.09.07 ������ �α����� ���� ������ ���������� ����
// ������ ���������� �α��� �ߴ��� üũ�ϵ��� �˷��ִ� �������̽��̴�.
// �α��� �� 3�п� �ѹ��� ���� �ȴ�.
// ��ǰ�� ����Ǵ� ������, �α׾ƿ� �Ǹ� ���۵��� �ʴ´�.
void IntegratedBillingServerSession::OnCheckLoginUser(
    long AccountID, long BillingGUID)
{
    BillingMsg billing_msg;
    billing_msg.protocol = billing_msg.eBillingMsg_OnCheckLoginUser;
    billing_msg.OnCheckLoginUser.AccountID = AccountID;
    billing_msg.OnCheckLoginUser.BillingGUID = BillingGUID;
    //
    util::WxMessageIpc::WorkerHandler handler = \
        AuthFrame::Instance()->GetBillingIpc()->GetWorkerHandler();
    handler.SendMessageToMain(&billing_msg, sizeof(billing_msg));

#if SUN_CODE_BACKUP
    AccountInfo* pAccount = AccountManager::Instance()->FindWithUserGuid(AccountID);
    if (pAccount == 0)
    {       
        SendLogout(BillingGUID);
    }
#endif
}

// WARNING: IntegratedBillingServerSessionInterface interface, thread#n
//PC�� ������ ������ ����Ǹ� �뺸�ȴ�.
void IntegratedBillingServerSession::OnPCRoomIPUpdate(
    long RoomGUID, long GameCode)
{
    BillingMsg billing_msg;
    billing_msg.protocol = billing_msg.eBillingMsg_OnPCRoomIPUpdate;
    billing_msg.OnPCRoomIPUpdate.RoomGUID = RoomGUID;
    billing_msg.OnPCRoomIPUpdate.GameCode = GameCode;
    //
    util::WxMessageIpc::WorkerHandler handler = \
        AuthFrame::Instance()->GetBillingIpc()->GetWorkerHandler();
    handler.SendMessageToMain(&billing_msg, sizeof(billing_msg));

#if SUN_CODE_BACKUP
    if (INTERGRATEDBILLING_GAMECODE == GameCode)
    {
        QueryManager* query_manager = QueryManager::Instance();
        query_manager->QueryPCRoomIPList(RoomGUID);
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "OnPCRoomIPUpdate::Not Matched GameCode (%d)", GameCode);
    }
#endif
}

void IntegratedBillingServerSession::OnInquireMultiUser(
    long AccountGUID, long RoomGUID, long Result)
{
#ifdef _NA_005011_20120621_MULTICLIENTS
    if (Result < 0)
        return;

    pcroom_user_manager_.SetPCRoomMultiUserType(RoomGUID, Result);

    AccountInfo* account_info = AccountManager::Instance()->FindWithUserGuid(AccountGUID);
    if (account_info == NULL)
        return;

    int multi_user_enable_type = \
        pcroom_user_manager_.GetPCRoomMultiUserType(account_info->m_pcBangId);
    if (multi_user_enable_type == 1) {
        // ��Ƽ���� �α��� ����
        SendLogin(*account_info);
    }
    else if (multi_user_enable_type == 2) {
        // ��Ƽ���� �α��� �Ұ���
        USERGUID guid = pcroom_user_manager_.GetFirstUser(account_info->m_clientIp);
        if (guid == account_info->m_userGuid) {
            // ���� ó�� �α����� �����̸�
            SendLogin(*account_info);
        }
        //else
        //{
        //    // �� ���� �ٸ� ������ ó���̴�
        //    AccountInfo* account_info2 = AccountManager::Instance()->FindWithUserGuid(guid);
        //    if (account_info2 == NULL)
        //        return;

        //    SendLogin(*account_info2);
        //}
    }
#endif //_NA_005011_20120621_MULTICLIENTS
}

#ifdef _NA_005011_20120621_MULTICLIENTS
bool IntegratedBillingServerSession::CheckMultiUserTypeAndLogin(AccountInfo& info)
{
    int multi_user_enable_type = pcroom_user_manager_.GetPCRoomMultiUserType(info.m_pcBangId);
    if (multi_user_enable_type == 0) {
        InquireMultiUser(info);
        return false;
    }
    else if (multi_user_enable_type == 1) {
        // ��Ƽ���� �α��� ����
        return true;
    }
    else if (multi_user_enable_type == 2) {
        // ��Ƽ���� �α��� �Ұ���
        USERGUID guid = pcroom_user_manager_.GetFirstUser(info.m_clientIp);
        if (guid == info.m_userGuid) {
            // ���� ó�� �α����� �����̸�
            return true;
        }
    }

    return false;
}

void IntegratedBillingServerSession::CheckIn(AccountInfo& info)
{
    if (info.m_pcBangId == 0)
        return;

    pcroom_user_manager_.AddAccount(info);
}

void IntegratedBillingServerSession::CheckOut(AccountInfo& info)
{
    if (info.m_pcBangId == 0)
        return;

    //
    USERGUID first_login_user_guid = pcroom_user_manager_.GetFirstUser(info.m_clientIp);
    pcroom_user_manager_.RemoveAccount(info);

    //
    int multi_user_enable_type = pcroom_user_manager_.GetPCRoomMultiUserType(info.m_pcBangId);
    if (multi_user_enable_type == 1) {
        // ��Ƽ���� �α��� ����
        SendLogout(info);
    }
    else if (multi_user_enable_type == 2) {
        // ��Ƽ���� �α��� �Ұ���
        if (first_login_user_guid == info.m_userGuid) {
            // ���� �α��� �Ǿ� ������
            SendLogout(info);

            ;{
                // ���� ����� ã�Ƽ� �α��� ������
                USERGUID guid = pcroom_user_manager_.GetFirstUser(info.m_clientIp);
                AccountInfo* account_info2 = AccountManager::Instance()->FindWithUserGuid(guid);
                if (account_info2 == NULL)
                    return;

                SendLogin(*account_info2);
            };
        }
    }
}
#endif //_NA_005011_20120621_MULTICLIENTS
#endif //__NA_20100201_INTERGRATEDBILLING_