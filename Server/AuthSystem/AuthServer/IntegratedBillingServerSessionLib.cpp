#include "StdAfx.h"

#ifdef __NA_20100201_INTERGRATEDBILLING_

#include "IntegratedBillingServerSessionLib.h"

IntegratedBillingServerSessionLib::IntegratedBillingServerSessionLib(void)
{

}

IntegratedBillingServerSessionLib::~IntegratedBillingServerSessionLib(void)
{

}

void IntegratedBillingServerSessionLib::WriteLog(char* szMsg)
{
    //로그가 너무 많이 남는다.
    //SUNLOG(eCRITICAL_LOG, "[Billing_LIB_Msg] %s", szMsg);
}

void IntegratedBillingServerSessionLib::OnNetConnect(bool success, DWORD error)
{
    m_BillingInterface->OnConnectLib(success);
}

void IntegratedBillingServerSessionLib::OnNetSend( int size )
{

}

void IntegratedBillingServerSessionLib::OnNetClose( DWORD error )
{
    m_BillingInterface->OnDisconnectLib();
}

void IntegratedBillingServerSessionLib::OnUserStatus(DWORD dwAccountGUID, 
                  long dwBillingGUID, 
                  DWORD RealEndDate, 
                  DWORD EndDate, 
                  double dRestPoint, 
                  double dRestTime, 
                  short nDeductType, 
                  short nAccessCheck, 
                  short nResultCode)
{
    m_BillingInterface->OnUserStatus(dwAccountGUID, 
        dwBillingGUID, 
        RealEndDate, 
        EndDate, 
        dRestPoint, 
        dRestTime, 
        nDeductType, 
        nAccessCheck, 
        nResultCode);
}

void IntegratedBillingServerSessionLib::OnInquireDeductType(long AccountID, long Result)
{
    SUNLOG(eCRITICAL_LOG, "Not Implement Event: OnInquireDeductType");
}

void IntegratedBillingServerSessionLib::OnInquireMultiUser(long AccountID, long RoomGUID, long Result)
{
    m_BillingInterface->OnInquireMultiUser(AccountID, RoomGUID, Result);
}

void IntegratedBillingServerSessionLib::OnInquirePCRoomPoint(long AccountGUID, 
                                                             long RoomGUID, 
                                                             long GameCode, 
                                                             long ResultCode)
{
    m_BillingInterface->OnInquirePCRoomPoint(AccountGUID, RoomGUID, ResultCode);
}

void IntegratedBillingServerSessionLib::OnInquirePersonDeduct(long AccountGUID, 
															  long GameCode, 
															  long ResultCode)
{
	m_BillingInterface->OnInquirePersonDeduct(AccountGUID, ResultCode);
}

void IntegratedBillingServerSessionLib::OnCheckLoginUser(long AccountID, 
														 long BillingGUID)
{
	m_BillingInterface->OnCheckLoginUser(AccountID, BillingGUID);
}

void IntegratedBillingServerSessionLib::OnPCRoomIPUpdate(long RoomGUID, long GameCode)
{
    m_BillingInterface->OnPCRoomIPUpdate(RoomGUID, GameCode);
}

#endif //__NA_20100201_INTERGRATEDBILLING_