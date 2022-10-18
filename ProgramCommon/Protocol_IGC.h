#ifndef PROTOCOL_IGC_H
#define PROTOCOL_IGC_H

//////////
//
//  Define
//
#ifdef _YJ_IGC
#pragma pack(1)
#define HEARTBEAT_INTERVAL		1000 * 60					// HEARTBEAT 전송 간격, 1분에 한번씩 보낸다

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//
//  IGC Server <-> Game Client Packet Define
//
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

#define MAX_FAQ_COUNT				5		// 카테고리당 최대 FAQ개수
#define MAX_IP						18		// 사용자 IP
#define MAX_USER_ID					41		// 사용자 ID
#define MAX_USER_CHAR_NAME			41		// 캐릭터 이름
#define MAX_CATEGORY_NAME			101		// 카테고리명
#define MAX_SUBCATEGORY_NAME		101		// 서브 카테고리명
#define MAX_FAQ_NAME				201		// FAQ명
#define MAX_CLIENT_CHAT_STRING		1025	// 클라이언트 채팅 메시지
#define MAX_CLIENT_MESSAGE_STRING	1025	// 메시지 답변
#define MAX_USER_OPINION			201		// 고객만족도 설문시 고객의견
#define MAX_LOCATION				201		// 위치정보
#define MAX_INQUIRY_TITLE			201		// 문의 제목
#define MAX_INQUIRY_CONTENT			2001	// 문의 내용
#define MAX_FAQ_CONTENT				2001	// FAQ 내용

#define IGC_PACKET_HEADER_SIZE					4		// 헤더 사이즈

#define DEFOULT_INQUIRY_TYPE		5		// 고정값.


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//
//  IGC Server -> GameClient Error Code
//
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

// CATEGORY ERROR CODE
#define CATEGORY_SUCCESS_TRANSFER		0		// 전송 성공
#define CATEGORY_EMPTY					1		// 카테고리 정보 없음

// FAQ ERROR CODE
#define FAQ_SUCCESS_TRANSFER			0		// 전송 성공
#define FAQ_EMPTY						1		// FAQ 정보 없음

// REGISTER INQUIRY CODE
#define ALREADY_REGISTERED				0		// 이미 등록되어 있음
#define USING_REGISTERED_NUMBER			1		// 이미 사용중인 등록번호
#define CANNOT_REGISTERED				2		// 서버 장애로 문의등록 실패
#define INQUIRY_OVERTIME				3		// 1일 문의 제한수 초과
#define INVLAID_PARAMETER				4		// 등록 정보가 올바르지 않음.

// CANCEL INQUIRY CODE
#define CANCEL_SUCCESS					0		// 문의 취소 성공
#define CANCEL_FAILED					1		// 문의 취소 실패

// 1:1 CHAT ERROR CODE
#define INVALID_GM_CONNECTION			0		// 유효하지 않은 GM 세션
#define GM_DISCONNECTED					1		// GM 세션이 끊김
#define CANNOT_DB_CHANGE_STATE			2		// GM 세션이 끊긴다음 해당 문의 상태변경 실패

// SATISFACTION ERROR CODE
#define SATISFACTION_SUCCESS_REGISTER	0		// 만족도 결과 등록 성공
#define SATISFACTION_FAILED_REGISTER	1		// 만족도 결과 등록 실패

// RECEIVE USER INFO ERROR CODE
#define RECEIVE_SUCCESS_USER_INFO		0		// 유저의 정보 수신 성공
#define RECEIVE_FAILED_USER_INFO		1		// 유저의 정보가 올바르지 않음(재전송 요청)
#define ALREADY_CLIENT_CONNECTED		2		// 이미 서버에 접속된 클라이언트임

#define FINISHED_CHAT					0		//유저와 채팅 상담 종료.
#define FINISHED_MESSAGE_NFT			1		//메세지 상담 종료.

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//
//  Game Client <-> IGC Server Protocol, 0x01XX
//
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

#define C_TO_S_CATEGORY_REQ					0x0101		// CATEGORY 목록 요청
#define C_TO_S_SUBCATEGORY_REQ				0x0102		// SUB CATEGORY 목록 요청
#define S_TO_C_CATEGORY_ANS					0x0103		// CATEGORY 목록 전송
#define S_TO_C_SUBCATEGORY_ANS				0x0104		// SUBCATEGORY 목록 전송
#define S_TO_C_CATEGORY_ANS_FINISH			0x0105		// CATEGORY 목록 전송완료
#define S_TO_C_SUBCATEGORY_ANS_FINISH		0x0106		// SUBCATEGORY 목록 전송완료

#define C_TO_S_FAQ_REQ						0x0107		// FAQ 정보 요청
#define S_TO_C_FAQ_ANS						0x0108		// FAQ 정보 전송
#define S_TO_C_FAQ_ANS_FINISH				0x0109		// FAQ 정보 전송완료

#define C_TO_S_REGISTER_INQUIRY_REQ			0x0110		// 문의 등록 요청
#define S_TO_C_REGISTER_INQUIRY_FAILED		0x0111		// 문의 등록 실패(실패시만 전송한다)

#define C_TO_S_CANCEL_INQUIRY_REQ			0x0112		// 문의 취소 요청
#define S_TO_C_CANCEL_INQUIRY_ANS			0x0113		// 문의 취소 결과(0:성공, 1:실패)

#define S_TO_C_WAITING_NUMBER_NTF			0x0114		// 문의등록시 생성된 대기표
#define C_TO_S_UPDATE_WAITING_NUMBER_REQ	0x0115		// 유저의 대기표 요청
#define S_TO_C_UPDATE_WAITING_NUMBER_ANS	0x0116		// 유저의 요청시 보내는 대기표

#define C_TO_S_CHAT_MESSAGE					0x0117		// 유저 채팅메시지
#define S_TO_C_CHAT_MESSAGE					0x0118		// GM 채팅메시지
#define S_TO_C_INVALID_GM_SESSION			0x0119		// 유효하지 않은 GM 세션과 통신한 경우

#define S_TO_C_MESSAGE_NTF					0x0120		// 메시지 답변

#define S_TO_C_SATISFACTION_REQ				0x0121		// 만족도 조사
#define C_TO_S_SATISFACTION_ANS				0x0122		// 만족도 결과
#define S_TO_C_SATISFACTION_REGISTER_RESULT	0x0123		// 만족도 저장 결과(0:성공, 1:실패)

//// 게임 클라이언트와 처음 접속을 맺을때 클라이언트의 정보를 요청
#define S_TO_C_USER_INFO_REQ				0x0124		// 유저의 기본정보 요청
#define C_TO_S_USER_INFO_ANS				0x0125		// 유저의 기본정보
#define S_TO_C_RECEIVE_USER_INFO_RESULT		0x0126		// 유저의 정보 수신 상태

#define C_TO_S_HEARTBEAT					0x0127		// 대기중에 주기적으로 커넥션 이상유무를 확인
#define S_TO_C_CONGESTION_NTF				0x0128		// 서버 폭주로 연결할 수 없음을 알림
#define C_TO_S_CONGESTION_ANS				0x0129		// 위 패킷에 대한 답신 패킷

#define S_TO_C_DISCONNECT_NTF				0X0130		//서버에서 클라이언트 세션 종료를 통보.

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//
//  테이블 구조체 정의
//
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

// 문의 요청시 작성될 내용( C -> S 메시지 )  <== 수정된 DB로 업데이트 해야함
typedef struct sInquiry
{
	BYTE	btWorldID;									// 월드 ID
	BYTE	btChannel;									// 채널
	BYTE	btInquiryType;								// 문의 타입
	char	szUserID[MAX_USER_ID];						// 유저 ID
	char	szCharacterName[MAX_USER_CHAR_NAME];		// 캐릭터명
	BYTE	btCategoryID;								// 문의 카테고리 ID
	BYTE	btSubCategoryID;							// 문의 서브 카테고리 ID
	WORD	wInquiryTitleLen;							// 문의 제목 길이
	WORD	wInquiryContentLen;							// 문의 내용 길이
	char	szInquiryTitle[MAX_INQUIRY_TITLE];			// 문의 제목
	char	szInquiryContent[MAX_INQUIRY_CONTENT]	;	// 문의 내용	
	char	szIP[MAX_IP];								// 클라이언트 IP
	char	szLocation[MAX_LOCATION];					// 위치 정보

	sInquiry()
	{
		memset(this, 0, sizeof(*this));
	}

} INQUIRY, *LPINQUIRY;

// FAQ패킷 수신시 각각의 FAQ에 대한 정보를 저장한다
typedef struct sFAQInfo
{
    char szFaqName[MAX_FAQ_NAME];			// FAQ 명
	char szFaqContent[MAX_FAQ_CONTENT];		// FAQ 내용

    sFAQInfo()
	{
		Reset();
	}

    void Reset()
	{
        memset(this, 0, sizeof(*this));
	}

} FAQINFO, *LPFAQINFO;



/////////////////////////////////
//							   //
//  메시지별 패킷 클래스 정의  //
//							   //
/////////////////////////////////

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//
//  Game Cilent <-> IGC Server 패킷 클래스
//
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

// 문의 카테고리 정보 요청
class MSG_C_TO_S_CATEGORY_REQ : public IGC_MSG_HEADER8
{
public:

	MSG_C_TO_S_CATEGORY_REQ()
	{
//		dwPacketSize = sizeof(*this);
		dwID = C_TO_S_CATEGORY_REQ;
	}
	~MSG_C_TO_S_CATEGORY_REQ()
	{
	}
};

// 서브 문의 카테고리 정보 요청
class MSG_C_TO_S_SUBCATEGORY_REQ : public IGC_MSG_HEADER8		// 2 depth일 경우
{
public:

	MSG_C_TO_S_SUBCATEGORY_REQ()
	{
//		dwPacketSize = sizeof(*this);
		dwID = C_TO_S_SUBCATEGORY_REQ;
	}
	~MSG_C_TO_S_SUBCATEGORY_REQ()
	{
	}

	BYTE btCategoryID;		// 상위 카테고리 ID
};

// 문의 카테고리 정보 전송
class MSG_S_TO_C_CATEGORY_ANS : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_CATEGORY_ANS()
	{
		Initialize();
	}
	~MSG_S_TO_C_CATEGORY_ANS()
	{
	}
	void Initialize()
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_CATEGORY_ANS;
		btCategoryID = 0;
		memset(&szCategoryName[0], 0, MAX_CATEGORY_NAME);
	}

	BYTE btCategoryID;							// 문의 카테고리 ID
	char szCategoryName[MAX_CATEGORY_NAME];		// 문의 카테고리 제목
};

// 서브 문의 카테고리 정보 전송
class MSG_S_TO_C_SUBCATEGORY_ANS : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_SUBCATEGORY_ANS()
	{
        Initialize();		
	}
	~MSG_S_TO_C_SUBCATEGORY_ANS()
	{
	}
	void Initialize()
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_SUBCATEGORY_ANS;
		btCategoryID = 0;
		btSubCategoryID = 0;
		memset(&szSubCategoryName[0], 0, MAX_SUBCATEGORY_NAME);
	}

	BYTE btCategoryID;									// 문의 카테고리 ID
	BYTE btSubCategoryID;								// 서브 문의 카테고리 id
	char szSubCategoryName[MAX_SUBCATEGORY_NAME];		// 서브 문의 카테고리 제목
};

// 문의 카테고리 정보 전송완료
class MSG_S_TO_C_CATEGORY_ANS_FINISH : public IGC_MSG_HEADER8
{
public:
	
	MSG_S_TO_C_CATEGORY_ANS_FINISH() : dwErrorCode(CATEGORY_SUCCESS_TRANSFER)
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_CATEGORY_ANS_FINISH;
	}
	~MSG_S_TO_C_CATEGORY_ANS_FINISH()
	{
	}

	DWORD dwErrorCode;			// 에러코드
};

// 서브 문의 카테고리 정보 전송완료
class MSG_S_TO_C_SUBCATEGORY_ANS_FINISH : public IGC_MSG_HEADER8
{
public:
	
	MSG_S_TO_C_SUBCATEGORY_ANS_FINISH() : dwErrorCode(CATEGORY_SUCCESS_TRANSFER)
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_SUBCATEGORY_ANS_FINISH;
	}
	~MSG_S_TO_C_SUBCATEGORY_ANS_FINISH()
	{
	}

	DWORD dwErrorCode;			// 에러코드
};

// FAQ 정보 요청
class MSG_C_TO_S_FAQ_REQ : public IGC_MSG_HEADER8
{
public:

	MSG_C_TO_S_FAQ_REQ()
	{
//		dwPacketSize = sizeof(*this);
		dwID = C_TO_S_FAQ_REQ;
	}
	~MSG_C_TO_S_FAQ_REQ()
	{
	}

	BYTE btFAQID;		// FAQ ID
	BYTE btSubFAQID;	// 하위 FAQ ID
};

// FAQ 정보 전송
class MSG_S_TO_C_FAQ_ANS : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_FAQ_ANS() : btFAQID(0), btSubFAQID(0), dwFAQCount(0), dwUseBufferSize(0)
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_FAQ_ANS;
		memset(&szFAQStream[0], 0, (MAX_FAQ_NAME + MAX_FAQ_CONTENT) * MAX_FAQ_COUNT);
	}
    ~MSG_S_TO_C_FAQ_ANS()
	{
	}
	void SetPacket(MSG_S_TO_C_FAQ_ANS* SendPacket)
	{
        SendPacket->btFAQID			= btFAQID;
		SendPacket->btSubFAQID		= btSubFAQID;
		SendPacket->dwFAQCount		= dwFAQCount;
		SendPacket->dwUseBufferSize	= dwUseBufferSize;
		memcpy(&SendPacket->szFAQStream[0], &szFAQStream[0], dwUseBufferSize);
	}
	bool SetFAQName(char* pFAQName, int nFAQNameLen)
	{
		if( dwFAQCount && dwUseBufferSize )
		{
			if( szFAQStream[dwUseBufferSize - 1] != '/' )
			{
                szFAQStream[dwUseBufferSize] = '/';
				++dwUseBufferSize;
			}
		}
		memcpy(&szFAQStream[dwUseBufferSize], pFAQName, nFAQNameLen);
		dwUseBufferSize += nFAQNameLen;
		szFAQStream[dwUseBufferSize] = ':';		// 구분자 삽입
        ++dwUseBufferSize;
		return true;
	}
	bool SetFAQContent(char* pFAQContent, int nFAQContentLen)
	{
		if( !dwUseBufferSize )
		{
			return false;
		}

		if( dwFAQCount && dwFAQCount )
		{
			if( szFAQStream[dwUseBufferSize - 1] != ':' )
			{
				szFAQStream[dwUseBufferSize] = ':';
				++dwUseBufferSize;
			}
		}
		memcpy(&szFAQStream[dwUseBufferSize], pFAQContent, nFAQContentLen);
        dwUseBufferSize += nFAQContentLen;
		++dwFAQCount;
		return true;
	}
	
	BYTE btFAQID;					// FAQ ID
	BYTE btSubFAQID;				// 서브 FAQ ID
	DWORD dwFAQCount;				// FAQ 개수
	DWORD dwUseBufferSize;			// 사용한 버퍼 사이즈

	// 하나의 카테고리에 2개이상의 FAQ가 존재할수 있고 다수의 FAQ 존재시에는
	// 각각을 구별할수 있는 구분자를 두어 '하나의 패킷'에 복사하여 전송한다
	// 구분자는 ':', '/' 가지를 사용하며 ':'는 FAQ제목과 내용을 구분하고 '/'는 FAQ정보(제목+내용)를 구분한다
	// 패킷 Format ex) 'FAQ제목:FAQ내용/FAQ제목:FAQ내용...'
	char szFAQStream[ (MAX_FAQ_NAME + MAX_FAQ_CONTENT) * MAX_FAQ_COUNT ];	// FAQ정보를 저장할 배열 Stream
};

// FAQ 정보 전송완료
class MSG_S_TO_C_FAQ_ANS_FINISH : public IGC_MSG_HEADER8
{
public:
	
	MSG_S_TO_C_FAQ_ANS_FINISH() : dwErrorCode(FAQ_SUCCESS_TRANSFER)
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_FAQ_ANS_FINISH;
	}
	~MSG_S_TO_C_FAQ_ANS_FINISH()
	{
	}

	DWORD dwErrorCode;			// 에러코드
};

// 문의 등록 요청
class MSG_C_TO_S_REGISTER_INQUIRY_REQ : public IGC_MSG_HEADER8
{
public:

    MSG_C_TO_S_REGISTER_INQUIRY_REQ()
	{
//		dwPacketSize = sizeof(*this);
		dwID = C_TO_S_REGISTER_INQUIRY_REQ;
		memset(&User_Inquiry, 0, sizeof(INQUIRY));	// 구조체 초기화
	}
	~MSG_C_TO_S_REGISTER_INQUIRY_REQ()
	{
	}

	INQUIRY  User_Inquiry;			// 유저 문의 구조체
};

// 문의 등록 실패(등록 실패시 보낸다. 성공하면 대기표를 보낸다)
class MSG_S_TO_C_REGISTER_INQUIRY_FAILED : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_REGISTER_INQUIRY_FAILED() : dwErrorCode(0)
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_REGISTER_INQUIRY_FAILED;
	}
	~MSG_S_TO_C_REGISTER_INQUIRY_FAILED()
	{
	}

	DWORD dwErrorCode;				// 에러코드
};

// 문의 취소 요청
class MSG_C_TO_S_CANCEL_INQUIRY_REQ : public IGC_MSG_HEADER8
{
public:
    
    MSG_C_TO_S_CANCEL_INQUIRY_REQ()
	{
//		dwPacketSize = sizeof(*this);
		dwID = C_TO_S_CANCEL_INQUIRY_REQ;
	}
	~MSG_C_TO_S_CANCEL_INQUIRY_REQ()
	{
	}
};

// 문의 취소 결과(0:성공, 1:실패)
class MSG_S_TO_C_CANCEL_INQUIRY_ANS : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_CANCEL_INQUIRY_ANS() : dwResult(CANCEL_SUCCESS)
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_CANCEL_INQUIRY_ANS;
	}
	~MSG_S_TO_C_CANCEL_INQUIRY_ANS()
	{
	}

	DWORD dwResult;		// 문의 취소 결과
};

// 문의등록시 생성한 대기표 전송
class MSG_S_TO_C_WAITING_NUMBER_NTF : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_WAITING_NUMBER_NTF() : dwWaitNum(0)
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_WAITING_NUMBER_NTF;
	}
	~MSG_S_TO_C_WAITING_NUMBER_NTF()
	{
	}

	DWORD dwWaitNum;		// 대기표
};

// 유저의 대기표 요청
class MSG_C_TO_S_UPDATE_WAITING_NUMBER_REQ : public IGC_MSG_HEADER8
{
public:

	MSG_C_TO_S_UPDATE_WAITING_NUMBER_REQ()
	{
//		dwPacketSize = sizeof(*this);
		dwID = C_TO_S_UPDATE_WAITING_NUMBER_REQ;
	}
	~MSG_C_TO_S_UPDATE_WAITING_NUMBER_REQ()
	{
	}
};

// 유저의 요청시 보내는 대기표
class MSG_S_TO_C_UPDATE_WAITING_NUMBER_ANS : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_UPDATE_WAITING_NUMBER_ANS() : dwWaitNum(0)
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_UPDATE_WAITING_NUMBER_ANS;
	}
	~MSG_S_TO_C_UPDATE_WAITING_NUMBER_ANS()
	{
	}

	DWORD dwWaitNum;		// 대기표
};

// 유저 채팅메시지
class MSG_C_TO_S_CHAT_MESSAGE : public IGC_MSG_HEADER8
{
public:

	MSG_C_TO_S_CHAT_MESSAGE() : dwStrLen(0)
	{
//		dwPacketSize = sizeof(*this);
		dwID = C_TO_S_CHAT_MESSAGE;
		memset(&szStr[0], 0, MAX_CLIENT_CHAT_STRING);
	}
	~MSG_C_TO_S_CHAT_MESSAGE()
	{
	}

	DWORD dwStrLen;									// 채팅 문자열 길이
	char szStr[MAX_CLIENT_CHAT_STRING];				// 채팅 메시지 버퍼
};

// GM 채팅 메시지
class MSG_S_TO_C_CHAT_MESSAGE : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_CHAT_MESSAGE() : dwStrLen(0)
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_CHAT_MESSAGE;
		memset(&szStr[0], 0, MAX_CLIENT_CHAT_STRING);
	}
	~MSG_S_TO_C_CHAT_MESSAGE()
	{
	}

	DWORD dwStrLen;									// 채팅 문자열 길이
	char szStr[MAX_CLIENT_CHAT_STRING];				// 채팅 메시지 버퍼
};

// 유효하지 않은 GM세션
class MSG_S_TO_C_INVALID_GM_SESSION : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_INVALID_GM_SESSION() : dwErrorCode(INVALID_GM_CONNECTION)
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_INVALID_GM_SESSION;
	}
	~MSG_S_TO_C_INVALID_GM_SESSION()
	{
	}

	DWORD dwErrorCode;								// 에러코드
};

// 메시지 답변
class MSG_S_TO_C_MESSAGE_NTF : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_MESSAGE_NTF() : dwStrLen(0)
	{
//		dwPacketSize = sizeof(*this);				// 메시지는 고정길이로 보냄
		dwID = S_TO_C_MESSAGE_NTF;
		memset(&szStr[0], 0, MAX_CLIENT_MESSAGE_STRING);
	}
	~MSG_S_TO_C_MESSAGE_NTF()
	{
	}

	DWORD dwStrLen;									// 메시지 길이
	char szStr[MAX_CLIENT_MESSAGE_STRING];			// 메시지 버퍼
};

// 만족도 조사
class MSG_S_TO_C_SATISFACTION_REQ : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_SATISFACTION_REQ() : btGrade(0)
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_SATISFACTION_REQ;
	}
	~MSG_S_TO_C_SATISFACTION_REQ()
	{
	}

	BYTE btGrade;			// 만족도 점수
};

// 만족도 결과
class MSG_C_TO_S_SATISFACTION_ANS : public IGC_MSG_HEADER8
{
public:

	MSG_C_TO_S_SATISFACTION_ANS() : btGrade(0)
	{
//		dwPacketSize = sizeof(*this);
		dwID = C_TO_S_SATISFACTION_ANS;
		memset(&szUser_Opinion[0], 0, MAX_USER_OPINION);
	}
	~MSG_C_TO_S_SATISFACTION_ANS()
	{
	}

	BYTE btGrade;							// 만족도 점수
	char szUser_Opinion[MAX_USER_OPINION];	// 고객 의견
};

// 만족도 등록 결과(0:성공, 1:실패)
class MSG_S_TO_C_SATISFACTION_REGISTER_RESULT : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_SATISFACTION_REGISTER_RESULT() : dwResult(SATISFACTION_SUCCESS_REGISTER)		// Default로 성공
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_SATISFACTION_REGISTER_RESULT;
	}
	~MSG_S_TO_C_SATISFACTION_REGISTER_RESULT()
	{
	}

	DWORD dwResult;			// 만족도 저장 결과
};

// 게임 클라이언트 접속시 게임 클라이언트의 정보 요청
class MSG_S_TO_C_USER_INFO_REQ : public IGC_MSG_HEADER8
{
public:
	MSG_S_TO_C_USER_INFO_REQ()
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_USER_INFO_REQ;
	}
	~MSG_S_TO_C_USER_INFO_REQ()
	{
	}
};

// 게임 클라이언트의 정보를 전송
class MSG_C_TO_S_USER_INFO_ANS : public IGC_MSG_HEADER8
{
public:
	MSG_C_TO_S_USER_INFO_ANS()
	{
//		dwPacketSize = sizeof(*this);
		dwID = C_TO_S_USER_INFO_ANS;
		memset(&szUserID[0], 0, MAX_USER_ID);
        memset(&szCharacterName[0], 0, MAX_USER_CHAR_NAME);
	}
	~MSG_C_TO_S_USER_INFO_ANS()
	{
	}

	char szUserID[MAX_USER_ID];						// 유저 ID
	char szCharacterName[MAX_USER_CHAR_NAME];		// 캐릭터 이름
};

// 게임 클라이언트 정보 수신 결과
class MSG_S_TO_C_RECEIVE_USER_INFO_RESULT : public IGC_MSG_HEADER8
{
public:
	MSG_S_TO_C_RECEIVE_USER_INFO_RESULT() : dwErrorCode(RECEIVE_SUCCESS_USER_INFO)
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_RECEIVE_USER_INFO_RESULT;
	}
	~MSG_S_TO_C_RECEIVE_USER_INFO_RESULT()
	{
	}

	DWORD dwErrorCode;				// 에러코드
};

class MSG_C_TO_S_HEARTBEAT : public IGC_MSG_HEADER8
{
	public:
	MSG_C_TO_S_HEARTBEAT()
	{
//		dwPacketSize = sizeof(*this);
		dwID = C_TO_S_HEARTBEAT;
	}
	~MSG_C_TO_S_HEARTBEAT()
	{
	}
};

class MSG_S_TO_C_CONGESTION_NTF : public IGC_MSG_HEADER8
{
	public:
	MSG_S_TO_C_CONGESTION_NTF()
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_CONGESTION_NTF;
	}
	~MSG_S_TO_C_CONGESTION_NTF()
	{
	}
};

class MSG_C_TO_S_CONGESTION_ANS : public IGC_MSG_HEADER8
{
public:
	MSG_C_TO_S_CONGESTION_ANS()
	{
//		dwPacketSize = sizeof(*this);
		dwID = C_TO_S_CONGESTION_ANS;
	}
	~MSG_C_TO_S_CONGESTION_ANS()
	{
	}
};

class MSG_S_TO_C_DISCONNECT_NTF : public IGC_MSG_HEADER8
{
public:
	MSG_S_TO_C_DISCONNECT_NTF()
	{
		dwID = S_TO_C_DISCONNECT_NTF;
	}
	~MSG_S_TO_C_DISCONNECT_NTF()
	{
	}

	DWORD dwErrorCode;			//종료코드.
};

#pragma pack()

#endif //_YJ_IGC

#endif