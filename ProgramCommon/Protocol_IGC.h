#ifndef PROTOCOL_IGC_H
#define PROTOCOL_IGC_H

//////////
//
//  Define
//
#ifdef _YJ_IGC
#pragma pack(1)
#define HEARTBEAT_INTERVAL		1000 * 60					// HEARTBEAT ���� ����, 1�п� �ѹ��� ������

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//
//  IGC Server <-> Game Client Packet Define
//
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

#define MAX_FAQ_COUNT				5		// ī�װ��� �ִ� FAQ����
#define MAX_IP						18		// ����� IP
#define MAX_USER_ID					41		// ����� ID
#define MAX_USER_CHAR_NAME			41		// ĳ���� �̸�
#define MAX_CATEGORY_NAME			101		// ī�װ���
#define MAX_SUBCATEGORY_NAME		101		// ���� ī�װ���
#define MAX_FAQ_NAME				201		// FAQ��
#define MAX_CLIENT_CHAT_STRING		1025	// Ŭ���̾�Ʈ ä�� �޽���
#define MAX_CLIENT_MESSAGE_STRING	1025	// �޽��� �亯
#define MAX_USER_OPINION			201		// �������� ������ ���ǰ�
#define MAX_LOCATION				201		// ��ġ����
#define MAX_INQUIRY_TITLE			201		// ���� ����
#define MAX_INQUIRY_CONTENT			2001	// ���� ����
#define MAX_FAQ_CONTENT				2001	// FAQ ����

#define IGC_PACKET_HEADER_SIZE					4		// ��� ������

#define DEFOULT_INQUIRY_TYPE		5		// ������.


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//
//  IGC Server -> GameClient Error Code
//
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

// CATEGORY ERROR CODE
#define CATEGORY_SUCCESS_TRANSFER		0		// ���� ����
#define CATEGORY_EMPTY					1		// ī�װ� ���� ����

// FAQ ERROR CODE
#define FAQ_SUCCESS_TRANSFER			0		// ���� ����
#define FAQ_EMPTY						1		// FAQ ���� ����

// REGISTER INQUIRY CODE
#define ALREADY_REGISTERED				0		// �̹� ��ϵǾ� ����
#define USING_REGISTERED_NUMBER			1		// �̹� ������� ��Ϲ�ȣ
#define CANNOT_REGISTERED				2		// ���� ��ַ� ���ǵ�� ����
#define INQUIRY_OVERTIME				3		// 1�� ���� ���Ѽ� �ʰ�
#define INVLAID_PARAMETER				4		// ��� ������ �ùٸ��� ����.

// CANCEL INQUIRY CODE
#define CANCEL_SUCCESS					0		// ���� ��� ����
#define CANCEL_FAILED					1		// ���� ��� ����

// 1:1 CHAT ERROR CODE
#define INVALID_GM_CONNECTION			0		// ��ȿ���� ���� GM ����
#define GM_DISCONNECTED					1		// GM ������ ����
#define CANNOT_DB_CHANGE_STATE			2		// GM ������ ������� �ش� ���� ���º��� ����

// SATISFACTION ERROR CODE
#define SATISFACTION_SUCCESS_REGISTER	0		// ������ ��� ��� ����
#define SATISFACTION_FAILED_REGISTER	1		// ������ ��� ��� ����

// RECEIVE USER INFO ERROR CODE
#define RECEIVE_SUCCESS_USER_INFO		0		// ������ ���� ���� ����
#define RECEIVE_FAILED_USER_INFO		1		// ������ ������ �ùٸ��� ����(������ ��û)
#define ALREADY_CLIENT_CONNECTED		2		// �̹� ������ ���ӵ� Ŭ���̾�Ʈ��

#define FINISHED_CHAT					0		//������ ä�� ��� ����.
#define FINISHED_MESSAGE_NFT			1		//�޼��� ��� ����.

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//
//  Game Client <-> IGC Server Protocol, 0x01XX
//
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

#define C_TO_S_CATEGORY_REQ					0x0101		// CATEGORY ��� ��û
#define C_TO_S_SUBCATEGORY_REQ				0x0102		// SUB CATEGORY ��� ��û
#define S_TO_C_CATEGORY_ANS					0x0103		// CATEGORY ��� ����
#define S_TO_C_SUBCATEGORY_ANS				0x0104		// SUBCATEGORY ��� ����
#define S_TO_C_CATEGORY_ANS_FINISH			0x0105		// CATEGORY ��� ���ۿϷ�
#define S_TO_C_SUBCATEGORY_ANS_FINISH		0x0106		// SUBCATEGORY ��� ���ۿϷ�

#define C_TO_S_FAQ_REQ						0x0107		// FAQ ���� ��û
#define S_TO_C_FAQ_ANS						0x0108		// FAQ ���� ����
#define S_TO_C_FAQ_ANS_FINISH				0x0109		// FAQ ���� ���ۿϷ�

#define C_TO_S_REGISTER_INQUIRY_REQ			0x0110		// ���� ��� ��û
#define S_TO_C_REGISTER_INQUIRY_FAILED		0x0111		// ���� ��� ����(���нø� �����Ѵ�)

#define C_TO_S_CANCEL_INQUIRY_REQ			0x0112		// ���� ��� ��û
#define S_TO_C_CANCEL_INQUIRY_ANS			0x0113		// ���� ��� ���(0:����, 1:����)

#define S_TO_C_WAITING_NUMBER_NTF			0x0114		// ���ǵ�Ͻ� ������ ���ǥ
#define C_TO_S_UPDATE_WAITING_NUMBER_REQ	0x0115		// ������ ���ǥ ��û
#define S_TO_C_UPDATE_WAITING_NUMBER_ANS	0x0116		// ������ ��û�� ������ ���ǥ

#define C_TO_S_CHAT_MESSAGE					0x0117		// ���� ä�ø޽���
#define S_TO_C_CHAT_MESSAGE					0x0118		// GM ä�ø޽���
#define S_TO_C_INVALID_GM_SESSION			0x0119		// ��ȿ���� ���� GM ���ǰ� ����� ���

#define S_TO_C_MESSAGE_NTF					0x0120		// �޽��� �亯

#define S_TO_C_SATISFACTION_REQ				0x0121		// ������ ����
#define C_TO_S_SATISFACTION_ANS				0x0122		// ������ ���
#define S_TO_C_SATISFACTION_REGISTER_RESULT	0x0123		// ������ ���� ���(0:����, 1:����)

//// ���� Ŭ���̾�Ʈ�� ó�� ������ ������ Ŭ���̾�Ʈ�� ������ ��û
#define S_TO_C_USER_INFO_REQ				0x0124		// ������ �⺻���� ��û
#define C_TO_S_USER_INFO_ANS				0x0125		// ������ �⺻����
#define S_TO_C_RECEIVE_USER_INFO_RESULT		0x0126		// ������ ���� ���� ����

#define C_TO_S_HEARTBEAT					0x0127		// ����߿� �ֱ������� Ŀ�ؼ� �̻������� Ȯ��
#define S_TO_C_CONGESTION_NTF				0x0128		// ���� ���ַ� ������ �� ������ �˸�
#define C_TO_S_CONGESTION_ANS				0x0129		// �� ��Ŷ�� ���� ��� ��Ŷ

#define S_TO_C_DISCONNECT_NTF				0X0130		//�������� Ŭ���̾�Ʈ ���� ���Ḧ �뺸.

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//
//  ���̺� ����ü ����
//
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

// ���� ��û�� �ۼ��� ����( C -> S �޽��� )  <== ������ DB�� ������Ʈ �ؾ���
typedef struct sInquiry
{
	BYTE	btWorldID;									// ���� ID
	BYTE	btChannel;									// ä��
	BYTE	btInquiryType;								// ���� Ÿ��
	char	szUserID[MAX_USER_ID];						// ���� ID
	char	szCharacterName[MAX_USER_CHAR_NAME];		// ĳ���͸�
	BYTE	btCategoryID;								// ���� ī�װ� ID
	BYTE	btSubCategoryID;							// ���� ���� ī�װ� ID
	WORD	wInquiryTitleLen;							// ���� ���� ����
	WORD	wInquiryContentLen;							// ���� ���� ����
	char	szInquiryTitle[MAX_INQUIRY_TITLE];			// ���� ����
	char	szInquiryContent[MAX_INQUIRY_CONTENT]	;	// ���� ����	
	char	szIP[MAX_IP];								// Ŭ���̾�Ʈ IP
	char	szLocation[MAX_LOCATION];					// ��ġ ����

	sInquiry()
	{
		memset(this, 0, sizeof(*this));
	}

} INQUIRY, *LPINQUIRY;

// FAQ��Ŷ ���Ž� ������ FAQ�� ���� ������ �����Ѵ�
typedef struct sFAQInfo
{
    char szFaqName[MAX_FAQ_NAME];			// FAQ ��
	char szFaqContent[MAX_FAQ_CONTENT];		// FAQ ����

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
//  �޽����� ��Ŷ Ŭ���� ����  //
//							   //
/////////////////////////////////

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//
//  Game Cilent <-> IGC Server ��Ŷ Ŭ����
//
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

// ���� ī�װ� ���� ��û
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

// ���� ���� ī�װ� ���� ��û
class MSG_C_TO_S_SUBCATEGORY_REQ : public IGC_MSG_HEADER8		// 2 depth�� ���
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

	BYTE btCategoryID;		// ���� ī�װ� ID
};

// ���� ī�װ� ���� ����
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

	BYTE btCategoryID;							// ���� ī�װ� ID
	char szCategoryName[MAX_CATEGORY_NAME];		// ���� ī�װ� ����
};

// ���� ���� ī�װ� ���� ����
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

	BYTE btCategoryID;									// ���� ī�װ� ID
	BYTE btSubCategoryID;								// ���� ���� ī�װ� id
	char szSubCategoryName[MAX_SUBCATEGORY_NAME];		// ���� ���� ī�װ� ����
};

// ���� ī�װ� ���� ���ۿϷ�
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

	DWORD dwErrorCode;			// �����ڵ�
};

// ���� ���� ī�װ� ���� ���ۿϷ�
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

	DWORD dwErrorCode;			// �����ڵ�
};

// FAQ ���� ��û
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
	BYTE btSubFAQID;	// ���� FAQ ID
};

// FAQ ���� ����
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
		szFAQStream[dwUseBufferSize] = ':';		// ������ ����
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
	BYTE btSubFAQID;				// ���� FAQ ID
	DWORD dwFAQCount;				// FAQ ����
	DWORD dwUseBufferSize;			// ����� ���� ������

	// �ϳ��� ī�װ��� 2���̻��� FAQ�� �����Ҽ� �ְ� �ټ��� FAQ ����ÿ���
	// ������ �����Ҽ� �ִ� �����ڸ� �ξ� '�ϳ��� ��Ŷ'�� �����Ͽ� �����Ѵ�
	// �����ڴ� ':', '/' ������ ����ϸ� ':'�� FAQ����� ������ �����ϰ� '/'�� FAQ����(����+����)�� �����Ѵ�
	// ��Ŷ Format ex) 'FAQ����:FAQ����/FAQ����:FAQ����...'
	char szFAQStream[ (MAX_FAQ_NAME + MAX_FAQ_CONTENT) * MAX_FAQ_COUNT ];	// FAQ������ ������ �迭 Stream
};

// FAQ ���� ���ۿϷ�
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

	DWORD dwErrorCode;			// �����ڵ�
};

// ���� ��� ��û
class MSG_C_TO_S_REGISTER_INQUIRY_REQ : public IGC_MSG_HEADER8
{
public:

    MSG_C_TO_S_REGISTER_INQUIRY_REQ()
	{
//		dwPacketSize = sizeof(*this);
		dwID = C_TO_S_REGISTER_INQUIRY_REQ;
		memset(&User_Inquiry, 0, sizeof(INQUIRY));	// ����ü �ʱ�ȭ
	}
	~MSG_C_TO_S_REGISTER_INQUIRY_REQ()
	{
	}

	INQUIRY  User_Inquiry;			// ���� ���� ����ü
};

// ���� ��� ����(��� ���н� ������. �����ϸ� ���ǥ�� ������)
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

	DWORD dwErrorCode;				// �����ڵ�
};

// ���� ��� ��û
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

// ���� ��� ���(0:����, 1:����)
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

	DWORD dwResult;		// ���� ��� ���
};

// ���ǵ�Ͻ� ������ ���ǥ ����
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

	DWORD dwWaitNum;		// ���ǥ
};

// ������ ���ǥ ��û
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

// ������ ��û�� ������ ���ǥ
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

	DWORD dwWaitNum;		// ���ǥ
};

// ���� ä�ø޽���
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

	DWORD dwStrLen;									// ä�� ���ڿ� ����
	char szStr[MAX_CLIENT_CHAT_STRING];				// ä�� �޽��� ����
};

// GM ä�� �޽���
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

	DWORD dwStrLen;									// ä�� ���ڿ� ����
	char szStr[MAX_CLIENT_CHAT_STRING];				// ä�� �޽��� ����
};

// ��ȿ���� ���� GM����
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

	DWORD dwErrorCode;								// �����ڵ�
};

// �޽��� �亯
class MSG_S_TO_C_MESSAGE_NTF : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_MESSAGE_NTF() : dwStrLen(0)
	{
//		dwPacketSize = sizeof(*this);				// �޽����� �������̷� ����
		dwID = S_TO_C_MESSAGE_NTF;
		memset(&szStr[0], 0, MAX_CLIENT_MESSAGE_STRING);
	}
	~MSG_S_TO_C_MESSAGE_NTF()
	{
	}

	DWORD dwStrLen;									// �޽��� ����
	char szStr[MAX_CLIENT_MESSAGE_STRING];			// �޽��� ����
};

// ������ ����
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

	BYTE btGrade;			// ������ ����
};

// ������ ���
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

	BYTE btGrade;							// ������ ����
	char szUser_Opinion[MAX_USER_OPINION];	// �� �ǰ�
};

// ������ ��� ���(0:����, 1:����)
class MSG_S_TO_C_SATISFACTION_REGISTER_RESULT : public IGC_MSG_HEADER8
{
public:

	MSG_S_TO_C_SATISFACTION_REGISTER_RESULT() : dwResult(SATISFACTION_SUCCESS_REGISTER)		// Default�� ����
	{
//		dwPacketSize = sizeof(*this);
		dwID = S_TO_C_SATISFACTION_REGISTER_RESULT;
	}
	~MSG_S_TO_C_SATISFACTION_REGISTER_RESULT()
	{
	}

	DWORD dwResult;			// ������ ���� ���
};

// ���� Ŭ���̾�Ʈ ���ӽ� ���� Ŭ���̾�Ʈ�� ���� ��û
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

// ���� Ŭ���̾�Ʈ�� ������ ����
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

	char szUserID[MAX_USER_ID];						// ���� ID
	char szCharacterName[MAX_USER_CHAR_NAME];		// ĳ���� �̸�
};

// ���� Ŭ���̾�Ʈ ���� ���� ���
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

	DWORD dwErrorCode;				// �����ڵ�
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

	DWORD dwErrorCode;			//�����ڵ�.
};

#pragma pack()

#endif //_YJ_IGC

#endif