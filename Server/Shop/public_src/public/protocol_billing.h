#pragma once
#pragma pack(1)

/**************************************************************************************************

작성일: 2008-07-18
작성자: 문상현 (youngmoon@webzen.co.kr)

설명: 빌링서버 프로토콜 정의

**************************************************************************************************/

#define MAX_ACCOUNTID							51
#define MAX_QUREY_LENGTH						1024
#define MAX_PCROOM_GAMECODE_LENGTH				3

#define PROTOCOL_CLIENT_LOGIN					0x01
#define PROTOCOL_SERVER_USERSTATUS				0x02
#define PROTOCOL_CLIENT_LOGOUT					0x03
#define PROTOCOL_CLIENT_INQUIRE					0x04
#define PROTOCOL_CLIENT_INQUIRE_MULTIUSER		0x05
#define PROTOCOL_SERVER_INQUIRE_MULTIUSER		0x06
#define PROTOCOL_CLIENT_INQUIRE_PERSON_DEDUCT	0x07
#define PROTOCOL_SERVER_INQUIRE_PERSON_DEDUCT	0x08
#define PROTOCOL_CLIENT_INQUIRE_PCROOM_DEDUCT	0x09
#define PROTOCOL_SERVER_INQUIRE_PCROOM_DEDUCT	0x0A
#define PROTOCOL_SERVER_CHECK_LOGIN_USER		0x0B
#define PROTOCOL_CLIENT_UPDATE_PCBANGIPADDR		0x0C
#define PROTOCOL_SERVER_UPDATE_PCBANGIPADDR		0x0D


namespace BillingProtocol
{

template <class _subcls>
class MSG_BASE
{
public:
	MSG_BASE() {}
	MSG_BASE(DWORD dwID) : dwProtocolID(dwID), dwPacketSize(sizeof( _subcls )) {}
	
public:
	DWORD 	dwPacketSize; 					//패킷의 전체 사이즈
	DWORD	dwProtocolID;					//패킷 아이디
};

/// 2.3.1.	빌링 로그인 요청
/*
	(Client->Server) 로그인 할때 보내는 패킷
*/
class MSG_CLIENT_LOGIN : public MSG_BASE<MSG_CLIENT_LOGIN>	
{
public:
	MSG_CLIENT_LOGIN() : MSG_BASE<MSG_CLIENT_LOGIN>(PROTOCOL_CLIENT_LOGIN), 
		dwAccountGUID(0), 
		dwIPAddress(0), 
		dwRoomGUID(0), 
		dwGameCode(0), 
		dwServerType(0)
	{}

public:
	DWORD 	dwAccountGUID;				 	//계정 GUID	
	DWORD	dwIPAddress;					//클라이언트 아이피 주소
	DWORD 	dwRoomGUID;						//PC 방 GUID(없을경우 0)
											//PC 방 GUID는 게임 DB에서 조회 하시면 됩니다.
											//(PC방 정보는 게임 DB와 빌링 DB 동기화 됨)
	DWORD	dwGameCode;						//게임 코드
	DWORD	dwServerType;					//서버 유형 코드 : 테섭인지 라이브인지 구분 코드	
};


/// 2.3.2.	빌링 로그아웃 요청
/*
	(Client->Server) 로그아웃 할때 보내는 패킷
*/
class MSG_CLIENT_LOGOUT : public MSG_BASE<MSG_CLIENT_LOGOUT>
{
public:
	MSG_CLIENT_LOGOUT() : MSG_BASE<MSG_CLIENT_LOGOUT>(PROTOCOL_CLIENT_LOGOUT), 
		dwBillingGUID(0)
	{}

public:
	long	dwBillingGUID;					//로그인 결과로 받은 GUID
};

/// 2.3.3.	빌링 로그인 정보 조회 요청
/*
	(Client->Server) 사용자 정보 조회
*/
class MSG_CLIENT_INQUIRE : public MSG_BASE<MSG_CLIENT_INQUIRE>
{
public:
	MSG_CLIENT_INQUIRE() : MSG_BASE<MSG_CLIENT_INQUIRE>(PROTOCOL_CLIENT_INQUIRE), 	
		dwBillingGUID(0)
		{}

public:
	long	dwBillingGUID;					//로그인 결과로 받은 GUID
};

/// 2.3.4.	로그인 처리 결과 / 빌링 정보 변경 통보 / 과금 만료로 인한 로그 아웃 통보 / 오류로 인한 강제 로그 아웃 통보 (2.3.1 ~ 2.3.3의 처리 결과)
/*
	(Server->Client) 유저 상태 정보 변경
*/
class MSG_SERVER_USERSTATUS : public MSG_BASE<MSG_SERVER_USERSTATUS>	
{
public:
	MSG_SERVER_USERSTATUS() : MSG_BASE<MSG_SERVER_USERSTATUS>(PROTOCOL_SERVER_USERSTATUS), 	
		dwAccountGUID(0), 
		dwBillingGUID(0), 
		RealEndDate(0), 
		EndDate(0), 
		dRestPoint(0), 
		dRestTime(0), 
		sDeductType(0), 
		sAccessCheck(0), 
		sResultCode(0)
	{}

public:
	DWORD 	dwAccountGUID;				 	//아이디 GUID	
	long	dwBillingGUID;					//빌링에서 사용하는 GUID (로그아웃시 보내줘야 한다.)
	DWORD	RealEndDate;					//정액일경우 실제 종료일자 (특정 시간 정액일 경우 적용)
	DWORD	EndDate;						//정액일경우 결제 종료일자
	double	dRestPoint;						//정량일 경우 잔여포인트
	double	dRestTime;						//정량일 경우 시간 (Sec)
	short	sDeductType;					//사용자 차감 유형
	short	sAccessCheck;					//접근 체크 결과
	short	sResultCode;					//처리 결과 코드
};


/// 2.4.1.	PC방 멀티 접속 가능 여부 조회
/*
	(Client->Server) 멀티유저 가능한지 조회
*/
class MSG_CLIENT_INQUIRE_MULTIUSER : public MSG_BASE<MSG_CLIENT_INQUIRE_MULTIUSER>	
{
public:
	MSG_CLIENT_INQUIRE_MULTIUSER() : MSG_BASE<MSG_CLIENT_INQUIRE_MULTIUSER>(PROTOCOL_CLIENT_INQUIRE_MULTIUSER), 	
		dwRoomGUID(0)
		{}

public:
	DWORD   dwAccountID;
	DWORD	dwRoomGUID;						//게임방 고유 번호
};

//2.4.1.	PC방 멀티 접속 가능 여부 조회
/*
	(Server->Client) 멀티유저 가능한지 조회
*/
class MSG_SERVER_INQUIRE_MULTIUSER : public MSG_BASE<MSG_SERVER_INQUIRE_MULTIUSER>	
{
public:
	MSG_SERVER_INQUIRE_MULTIUSER() : MSG_BASE<MSG_SERVER_INQUIRE_MULTIUSER>(PROTOCOL_SERVER_INQUIRE_MULTIUSER), 	
		dwRoomGUID(0)
		{}

public:
	DWORD   dwAccountID;
	DWORD	dwRoomGUID;						//게임방 고유 번호
	long	result;							//조회 결과
};

/// 2.4.3.	개인 정액 정량 보유 여부 조회
/*
	(Client->Server) 개인의 게임 별 사용 가능한 정액,정량 보유 여부를 조회한다.
*/
class MSG_CLIENT_INQUIRE_PERSON_DEDUCT : public MSG_BASE<MSG_CLIENT_INQUIRE_PERSON_DEDUCT>	
{
public:
	MSG_CLIENT_INQUIRE_PERSON_DEDUCT() 
		: MSG_BASE<MSG_CLIENT_INQUIRE_PERSON_DEDUCT>(PROTOCOL_CLIENT_INQUIRE_PERSON_DEDUCT), 
		dwAccountGUID(0), dwGameCode(0)
		{
		}

public:
	DWORD	dwAccountGUID;
	DWORD	dwGameCode;
};


/// 2.4.3.	개인 정액 정량 보유 여부 조회
/*
	(Server->Client) 개인의 게임 별 사용 가능한 정액,정량 보유 여부를 조회한다.
*/
class MSG_SERVER_INQUIRE_PERSON_DEDUCT : public MSG_BASE<MSG_SERVER_INQUIRE_PERSON_DEDUCT>	
{
public:
	MSG_SERVER_INQUIRE_PERSON_DEDUCT() 
		: MSG_BASE<MSG_SERVER_INQUIRE_PERSON_DEDUCT>(PROTOCOL_SERVER_INQUIRE_PERSON_DEDUCT), 	
		dwAccountGUID(0), dwGameCode(0)
		{
		}

public:
	DWORD	dwAccountGUID;
	DWORD	dwGameCode;
	long	ResultCode;
};


/// 2.4.2.	 PC방 정액 정량 보유 여부 조회
/*
	(Client->Server) 특정 PC방의 게임 별 정량 보유 여부를 조회한다.
*/
class MSG_CLIENT_INQUIRE_PCROOM_DEDUCT : public MSG_BASE<MSG_CLIENT_INQUIRE_PCROOM_DEDUCT>
{
public:
	MSG_CLIENT_INQUIRE_PCROOM_DEDUCT() 
		: MSG_BASE<MSG_CLIENT_INQUIRE_PCROOM_DEDUCT>(PROTOCOL_CLIENT_INQUIRE_PCROOM_DEDUCT), 
		dwAccountGUID(0), dwRoomGUID(0), dwGameCode(0)
		{
		}

public:
	DWORD	dwAccountGUID;
	DWORD   dwRoomGUID;
	DWORD	dwGameCode;
};

/// 2.4.2.	 PC방 정액 정량 보유 여부 조회
/*
	(Server->Client) 특정 PC방의 게임 별 정량 보유 여부를 조회한다.
*/
class MSG_SERVER_INQUIRE_PCROOM_DEDUCT: public MSG_BASE<MSG_SERVER_INQUIRE_PCROOM_DEDUCT>
{
public:
	MSG_SERVER_INQUIRE_PCROOM_DEDUCT() 
		: MSG_BASE<MSG_SERVER_INQUIRE_PCROOM_DEDUCT>(PROTOCOL_SERVER_INQUIRE_PCROOM_DEDUCT),
		dwAccountGUID(0), dwRoomGUID(0), dwGameCode(0)
		{
		}

public:
	DWORD	dwAccountGUID;
	DWORD   dwRoomGUID;
	DWORD	dwGameCode;
	long	ResultCode;
};


/// 2.3.5.	로그인 유저 체크
/*
	(Server->Client) 체크 로그인 유저 
	빌링에 로그인한 유저 정보를 인증서버에 전송
	실제로 인증서버에 로그인 했는지 체크하도록 알려주는 인터페이스이다.
*/
class MSG_SERVER_CHECK_LOGIN_USER: public MSG_BASE<MSG_SERVER_CHECK_LOGIN_USER>
{
public:
	MSG_SERVER_CHECK_LOGIN_USER() 
		: MSG_BASE<MSG_SERVER_CHECK_LOGIN_USER>(PROTOCOL_SERVER_CHECK_LOGIN_USER),
		dwAccountGUID(0), dwBillingGUID(0)
		{
		}

public:
	DWORD	dwAccountGUID;
	long	dwBillingGUID;
};



/// 2.5.1.	PC방 IP변경 알림
/*
	Client->Server : PC방 IP 변경
	IBS Manager에서만 사용하고 게임 스튜디오는 MSG_SERVER_UPDATE_PCBANGIPADDR 를 받는다.
*/
class MSG_CLIENT_UPDATE_PCBANGIPADDR : public MSG_BASE<MSG_CLIENT_UPDATE_PCBANGIPADDR>	
{
public:
	MSG_CLIENT_UPDATE_PCBANGIPADDR() : MSG_BASE<MSG_CLIENT_UPDATE_PCBANGIPADDR>(PROTOCOL_CLIENT_UPDATE_PCBANGIPADDR), 	
		dwRoomGUID(0), dwGameCode(0)
		{}
public:
	DWORD	dwRoomGUID;						//게임방 고유 번호
	DWORD	dwGameCode;
};

/// 2.5.1.	PC방 IP변경 알림
/*
	Server->Client : PC방 IP 변경
*/
class MSG_SERVER_UPDATE_PCBANGIPADDR : public MSG_BASE<MSG_SERVER_UPDATE_PCBANGIPADDR>	
{
public:
	MSG_SERVER_UPDATE_PCBANGIPADDR() : MSG_BASE<MSG_SERVER_UPDATE_PCBANGIPADDR>(PROTOCOL_SERVER_UPDATE_PCBANGIPADDR), 	
		dwRoomGUID(0), dwGameCode(0)
		{}

public:
	DWORD	dwRoomGUID;						//게임방 고유 번호
	DWORD	dwGameCode;						//게임 코드
};

}

#pragma pack()