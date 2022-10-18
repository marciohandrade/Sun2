#pragma once

/**************************************************************************************************

작성일: 2008-07-08
작성자: 문상현 (youngmoon@webzen.co.kr)

설명: 빌링서버를 사용하는 클라이언트에게 수신된 패킷을 전달해 주기 위한 이벤트 객체 
      이벤트를 받는쪽(빌링클라이언트)에서 상속받아 사용해야 한다.

**************************************************************************************************/

#include "ClientSession.h"

class CBillEventHandler : public CClientSession
{
public:
	CBillEventHandler(void);
	virtual ~CBillEventHandler(void);

	// 초기화 함수 (프로그램 실행하고 한번만 사용해야 한다.)
	bool CreateSession(WBANetwork::LPFN_ErrorHandler errhandler = NULL);

	// 빌링 로그인 요청
	bool UserLogin(
		DWORD	dwAccountGUID,		// 계정 GUID
		DWORD	dwIPAddress,		// 클라이언트 아이피 주소
		DWORD	dwRoomGUID,			// PC 방 GUID (없을경우 0), PC 방 GUID는 게임 DB에서 조회 하시면 됩니다. (PC방 정보는 게임 DB와 빌링 DB 동기화 됨)
		DWORD	dwGameCode,			// 게임 코드
		DWORD	dwServerType);		// 서버 유형 코드

	// 빌링 로그아웃 요청
	bool UserLogout(
		long	dwBillingGUID);		// 빌링 유니크 아이디(로그인 성공 시 OnUserStatus 함수를 통해 전달된 값)

	// 빌링 로그인 정보 조회 요청
	bool InquireUser(
		long	dwBillingGUID);		// 빌링 유니크 아이디(로그인 성공 시 OnUserStatus 함수를 통해 전달된 값)

	// PC 방이 멀티 유저 지원 여부 조회
	bool InquireMultiUser(
		long	AccountGUID,		// 계정 GUID
		long	RoomGUID);			// PC 방 GUID

	// 개인 정액/정량 보유 여부 조회
	bool InquirePersonDeduct(
		long	AccountGUID,		// 계정 GUID
		long	GameCode);			// 게임 코드

	// 특정 PC방의 게임 별 정량 보유 여부 조회
	bool InquirePCRoomPoint(
		long	AccountGUID,		// 계정 GUID
		long	RoomGUID,			// PC 방 GUID
		long	GameCode);			// 게임 코드

protected:
	// [do not override] - 로깅후 적절한 응답처리 함수(On...()) 호출
	void OnReceive(PBYTE buffer, int size);

	// [do not override] - 로깅만 하고 OnNetConnect() 호출
	void OnConnect(bool success, DWORD error);

	// [do not override] - 로깅만 하고 OnNetSend() 호출
	void OnSend(int size);

	// [do not override] - 로깅만 하고 OnNetClose() 호출
	void OnClose(DWORD error);	


protected:
	
	// 유저의 결재 정보가 변경되었을 때 전달받는 콜백
	// 정상 로그인의 경우 : (0 == nResultCode) && (0 == nAccessCheck) && (0 != nDeductType)
	virtual void OnUserStatus(
		DWORD	dwAccountGUID,		// 계정 GUID
		long	dwBillingGUID,		// 빌링 유니크 아이디
		DWORD	RealEndDate,		// 정액 사용 종료 일시 (__time32_t) 
		DWORD	EndDate,			// 정액 실제 종료 일시
		double	dRestPoint,			// 정량 잔여 포인트
		double	dRestTime,			// 정량 잔연 시간(초)
		short	nDeductType,		// 사용자 차감 유형
		short	nAccessCheck,		// 접근 체크 결과
		short	nResultCode)		// 처리 결과
		= 0;

	// 개인의 게임 별 정액, 정량 보유 여부 조회 결과
	virtual void OnInquirePersonDeduct(
		long	AccountGUID, 		// 계정 GUID
		long	GameCode, 			// 게임 코드
		long	ResultCode)			// 처리 결과
		= 0;

	// 특정 PC방의 게임 별 정량 보유 여부 조회 결과
	virtual void OnInquirePCRoomPoint(
		long	AccountGUID,  		// 계정 GUID
		long	RoomGUID,			// PC 방 GUID
		long	GameCode,			// 게임 코드
		long	ResultCode)			// 처리 결과
		= 0;

	// PC 방 멀티 유저 조회 결과 
	//		Result : 1 (가능), 2 (불가), -1 (PC방 정보 없음)
	virtual void OnInquireMultiUser(
		long	AccountID,			// 계정 GUID
		long	RoomGUID,			// PC 방 GUID
		long	Result)				// 처리 결과
		= 0;	

	// 빌링에 로그인한 유저 정보를 인증서버에 전송
	// 실제로 인증서버에 로그인 했는지 체크하도록 알려주는 인터페이스
	// 정량에 대해서만 로그인 후 약 3분에 한번씩 전송.
	// 상품이 변경되는 시점과, 로그아웃 되면 전송되지 않는다.
	virtual void OnCheckLoginUser(
		long	AccountID,			// 계정 GUID
		long	BillingGUID)		// 빌링 유니크 아이디(로그인 성공 시 OnUserStatus 함수를 통해 전달된 값)
		= 0;

	// 빌링서버에 접속한 이후 PC방 업주가 PC방 IP를 변경했을 때,
	// 빌링서버에 접속되어져 있는 게임서버로 IP변경한 PC방 GUID를 알려줌.
	virtual void OnPCRoomIPUpdate(
		long	RoomGUID,			// PC 방 GUID
		long	GameCode)			// 처리 결과
		= 0;

	// 빌링서버의 연결 성공 여부
	// (새로 연결되면 현재 차감이 필요한 모든 유저의 정보를 다시 로그인해야한다.)
	//		success	: 연결 성공 여부
	//		error	: 에러코드
	virtual void OnNetConnect(bool success, DWORD error) = 0;
		
	// 빌링서버로 보낸 데이타 전송 여부
	//		size	: 전송된 패킷 사이즈
	virtual void OnNetSend(int size) = 0;
	
	// 빌링서버와 세션이 끊겼을때 (빌링서버가 죽은것으로 처리 해야한다.)	
	//		error	: 에러코드
	virtual void OnNetClose( DWORD error ) = 0;	
	
	//로그를 써야할때 보내는 이벤트 메소드
	virtual void WriteLog(char* szMsg) = 0;

private:
	// 빌링서버의 로그를 작성 이벤트 : 함수내부에서 로그를 남길수 있도록 해야 함.
	void WriteLog(const char* szFormat, ...);
};
