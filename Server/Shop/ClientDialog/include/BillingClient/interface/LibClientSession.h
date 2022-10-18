#pragma once

// 빌링 서버 연동 샘플

#include <ImportLibrary.h>		// 필요한 ClientStub 라이브러리 import
#include <Winsock2.h>
#include <WBANetwork/WBANetwork.h>
#include "BillEventHandler.h"

class CLibClientSession : public CBillEventHandler
{
public:
	CLibClientSession(void);
	virtual  ~CLibClientSession(void);

protected:		
	// 빌링서버의 로그를 작성 이벤트 : 함수내부에서 로그를 남길수 있도록 해야 함.
	void WriteLog(char* szMsg);

	// 빌링서버의 연결 성공 여부
	// (새로 연결되면 현재 차감이 필요한 모든 유저의 정보를 다시 로그인해야한다.)
	//		success	: 연결 성공 여부
	//		error	: 에러코드
	void OnNetConnect(bool success, DWORD error);
	
	// 빌링서버로 보낸 데이타 전송 여부
	//		size	: 전송된 패킷 사이즈
	void OnNetSend(int size);
	
	// 빌링서버와 세션이 끊겼을때 (빌링서버가 죽은것으로 처리 해야한다.)	
	//		error	: 에러코드
	void OnNetClose(DWORD error);	

	// 유저의 결재 정보가 변경되었을 때 전달받는 콜백
	// 정상 로그인의 경우 : (0 == nResultCode) && (0 == nAccessCheck) && (0 != nDeductType)
	void OnUserStatus(
		DWORD	dwAccountGUID,		// 계정 GUID
		long	dwBillingGUID,		// 빌링 유니크 아이디
		DWORD	RealEndDate,		// 정액 사용 종료 일시 (__time32_t) 
		DWORD	EndDate,			// 정액 실제 종료 일시
		double	dRestPoint,			// 정량 잔여 포인트
		double	dRestTime,			// 정량 잔연 시간(초)
		short	nDeductType,		// 사용자 차감 유형
		short	nAccessCheck,		// 접근 체크 결과
		short	nResultCode);		// 처리 결과

	// PC 방 멀티 유저 조회 결과 
	//		Result : 1 (가능), 2 (불가), -1 (PC방 정보 없음)
	void OnInquireMultiUser(
		long	AccountID,			// 계정 GUID
		long	RoomGUID,			// PC 방 GUID
		long	Result);			// 처리 결과

	// 개인의 게임 별 정액, 정량 보유 여부 조회 결과
	void OnInquirePersonDeduct(
		long	AccountGUID, 		// 계정 GUID
		long	GameCode, 			// 게임 코드
		long	ResultCode);		// 처리 결과

	// 특정 PC방의 게임 별 정량 보유 여부 조회 결과
	void OnInquirePCRoomPoint(
		long	AccountGUID,  		// 계정 GUID
		long	RoomGUID,			// PC 방 GUID
		long	GameCode,			// 게임 코드
		long	ResultCode);		// 처리 결과

	// 빌링에 로그인한 유저 정보를 인증서버에 전송
	// 실제로 인증서버에 로그인 했는지 체크하도록 알려주는 인터페이스
	// 정량에 대해서만 로그인 후 약 3분에 한번씩 전송.
	// 상품이 변경되는 시점과, 로그아웃 되면 전송되지 않는다.
	void OnCheckLoginUser(
		long	AccountID,			// 계정 GUID
		long	BillingGUID);		// 빌링 유니크 아이디(로그인 성공 시 OnUserStatus 함수를 통해 전달된 값)


	// 빌링서버에 접속한 이후 PC방 업주가 PC방 IP를 변경했을 때,
	// 빌링서버에 접속되어져 있는 게임서버로 IP변경한 PC방 GUID를 알려줌.
	void OnPCRoomIPUpdate(
		long	RoomGUID,			// PC 방 GUID
		long	GameCode);			// 처리 결과

private:
	CRITICAL_SECTION m_cs;
};
