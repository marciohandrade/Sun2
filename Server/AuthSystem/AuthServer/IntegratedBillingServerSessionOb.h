#ifndef AUTHSERVER_INTEGRATEDBILLINGSERVEROB_H
#define AUTHSERVER_INTEGRATEDBILLINGSERVEROB_H

#pragma once

#ifdef __NA_20100201_INTERGRATEDBILLING_

class IntegratedBillingServerSessionInterface
{
public:
    virtual void				OnDisconnectLib()   = 0;
    virtual void				OnConnectLib(bool success)      = 0;    
    virtual void                OnUserStatus(DWORD dwAccountGUID,
                                             long dwBillingGUID,
                                             DWORD RealEndDate,
                                             DWORD EndDate,
                                             double dRestPoint,
                                             double dRestTime,
                                             short nDeductType,
                                             short nAccessCheck,
                                             short nResultCode) = 0;
    virtual void                OnInquirePCRoomPoint(long AccountID, 
                                            long CompAccountSeq, 
                                            long ResultCode) = 0;
	virtual void				OnInquirePersonDeduct(long AccountGUID, 
					    							  long ResultCode) = 0;
	virtual void				OnCheckLoginUser(long AccountID, 
												 long BillingGUID) = 0;
    virtual void                OnPCRoomIPUpdate(long RoomGUID, long GameCode) = 0;
    virtual void                OnInquireMultiUser(long AccountGUID, 
                                                   long RoomGUID, 
                                                   long Result) = 0;
};

#endif //__NA_20100201_INTERGRATEDBILLING_

#endif //AUTHSERVER_INTEGRATEDBILLINGSERVEROB_H







