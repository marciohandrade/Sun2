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
    //빌링서버의 로그를 작성 이벤트 이다.
    //함수내부에서 로그를 남길수 있도록 해야한다.
    void WriteLog(char* szMsg);

    //빌링서버의 연결 성공 여부 (새로 연결되면 현재 차감이 필요한 모든 유저의 정보를 다시 로그인해야한다.)
    //		success: 연결 성공 여부
    //		error: 에러코드
    void OnNetConnect(bool success, DWORD error);

    //빌링서버로 보낸 데이타 전송 여부
    //		size: 전송된 패킷 사이즈
    void OnNetSend( int size );

    //빌링서버와 세션이 끊겼을때 (빌링서버가 죽은것으로 처리 해야한다.)	
    //		error: 에러코드
    void OnNetClose( DWORD error );	

    //유저의 결재 정보가 변경되었을 때
    //		dwBillingGUID: 빌링 유니크 아이디
    //		RealEndDate: 정액 사용 종료 일시 (__time32_t) 
    //		EndDate: 정액 실제 종료 일시
    //		dwRestPoint: 정량 잔여 포인트 
    //		nDeductType: 사용자 차감 유형
    //		nAccessCheck: 접근 체크 결과
    //		nResultCode: 처리결과
    void OnUserStatus(DWORD dwAccountGUID, 
        long dwBillingGUID, 
        DWORD RealEndDate, 
        DWORD EndDate, 
        double dRestPoint, 
        double dRestTime, 
        short nDeductType, 
        short nAccessCheck, 
        short nResultCode);

    // 차감 타입 조회
    void OnInquireDeductType(long AccountID, long Result);

    //PC 방 멀티 유저 조회 결과 
    //		결과 코드
    //		1 : 가능
    //		2 : 불가
    //		-1 : PC방 정보 없음 
    void OnInquireMultiUser(long AccountID, long RoomGUID, long Result);

    // 특정 PC방의 게임 별 정량 보유 여부를 조회한다.    
    void OnInquirePCRoomPoint(long AccountGUID, 
        long RoomGUID, 
        long GameCode, 
        long ResultCode);

	// 개인의 게임 별 정액, 정량 보유 여부를 조회한다.
	void OnInquirePersonDeduct(long AccountGUID, 
		long GameCode, 
		long ResultCode);

	// 2010.09.07 빌링에 로그인한 유저 정보를 인증서버에 전송
	// 실제로 인증서버에 로그인 했는지 체크하도록 알려주는 인터페이스이다.
	// 로그인 후 3분에 한번씩 전송 된다.
	// 상품이 변경되는 시점과, 로그아웃 되면 전송되지 않는다.
	void OnCheckLoginUser(long AccountID, long BillingGUID);

    //PC 방 아이피 정보가 변경되었을때 업데이트 한다.
    void OnPCRoomIPUpdate(long RoomGUID, long GameCode);

private:
    IntegratedBillingServerSessionInterface* m_BillingInterface;
};

#endif //__NA_20100201_INTERGRATEDBILLING_

#endif //AUTHSERVER_INTEGRATIONBILLINGSERVERSESSION_H







