
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL




#pragma once
#ifndef SOLAR_SHOP_SHOP_SERVER_PROTOCOL_DEFINITION_H
#define SOLAR_SHOP_SHOP_SERVER_PROTOCOL_DEFINITION_H

#if SUN_CODE_BACKUP

// NOTE: this file declares protocols related to a ShopServer communication.
// the SUN server redefine a below protocol information to fit the SUN protocol format.

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////////////////////////
/// protocol define
/// GAME	: Game Server
///	SHOP	: ItemShop Server
/// Manager : ItemShop Manager

//////////////////////////////////////////////////////////////////////////
///  protocol id (GAME <-> SHOP)
#define GTOS_USER_CASH_REQ				0x0100		///> �ܿ�ĳ�� ��ȸ ��û    
#define STOG_USER_CASH_ANS				0x0200		///> �ܿ�ĳ�� ��ȸ ����    
#define GTOS_ITEM_LIST_REQ				0x0300		///> ���� ������ ��� ��û
#define STOG_ITEM_LIST_ANS				0x0400		///> ���� ������ ��� ���� 
#define GTOS_PACKAGE_LIST_REQ			0x0500		///> ��Ű�� ��� ��û	   
#define STOG_PACKAGE_LIST_ANS			0x0600		///> ��Ű�� ��� ����      
#define GTOS_BRANCH_ITEM_LIST_REQ		0x0700		///> Ưȭ �з� ������ ��� ��û	
#define STOG_BRANCH_ITEM_LIST_ANS		0x0800		///> Ưȭ �з� ������ ��� ����
#define GTOS_BUY_ITEM_REQ				0x0900		///> ������ ���� ��û      
#define STOG_BUY_ITEM_ANS				0x0A00		///> ������ ���� ����      
#define GTOS_BUY_ITEM_NTF				0x0B00		///> ������ ���� ���ӳ� ó�� ���
#define GTOS_USER_BUY_HISTORY_REQ		0x0C00		///> ����� ���� �̷� ��û 
#define STOG_USER_BUY_HISTORY_ANS		0x0D00		///> ����� ���� �̷� ���� 
#define STOG_RELOAD_SHOP_DATA_NTF		0x0E00		///> SHOP DATA ���� ��û
#define GTOS_SEND_GIFT_ITEM_REQ			0x0F00		///> ������ ���� �ϱ� ��û
#define STOG_SEND_GIFT_ITEM_ANS			0x1000		///> ������ ���� �ϱ� ��� ����
#define GTOS_ACTION_RECV_GIFT_REQ		0x1100		///> ���� ���� ó�� ��û
#define STOG_ACTION_RECV_GIFT_ANS		0x1200		///> ���� ���� ó�� ��û ��� ����
#define GTOS_ACTION_RECV_GIFT_NTF		0x1300		///> ���ӳ� ó�� ��� ����
#define GTOS_IS_RECV_GIFT_REQ			0x1400		///> ���� ���� ���� ����
#define STOG_IS_RECV_GIFT_ANS			0x1500		///> ���� ���� ���� ���� Ȯ�� ��� ����
#define GTOS_RECV_GIFT_LIST_REQ			0x1600		///> ���� ���� ��� ��û
#define STOG_RECV_GIFT_LIST_ANS			0x1700		///> ���� ���� ��� ����
#define GTOS_SEND_GIFT_LIST_REQ			0x1800		///> ���� ���� ��� ��û
#define STOG_SEND_GIFT_LIST_ANS			0x1900		///> ���� ���� ��� ����

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
//2007/6/5 �߰�
#define GTOS_MILEAGE_ITEM_LIST_REQ		0x1A00		///> ���ϸ��� ������ ��� ��û
#define STOG_MILEAGE_ITEM_LIST_ANS		0x1B00		///> ���ϸ��� ������ ��� ����
#endif

//////////////////////////////////////////////////////////////////////////
///  protocol id (Manager <-> SHOP)
#define MTOS_RELOAD_SHOP_DATA_NTF		0x0101		///> SHOP DATA ���� ��û

namespace protocol {

class HDRMessageID4
{
public:
	BYTE category;
	BYTE cessageID;
};

class HDRMessageID8
{
public:
	WORD category;
	WORD messageID;
};

class MSG_HEADER4
{
public:
	WORD		size;				
	union
	{
		WORD			id;			//  Message ID
		HDRMessageID4	hdrid;
	};
};

class MSG_HEADER8
{
public:
	DWORD		size;				
	union
	{
		DWORD			id;			//  Message ID
		HDRMessageID8	hdrid;
	};	
};

/*
	SUN�� 4����Ʈ ����� ������ 2byte�� ���Ե��� ���� ����� ����Ѵ�.
	#define __HEADER4_SIZE_2__ 
	MU�� 8����Ʈ �ش��� ��ü ��� ����� ���Ե� ����� ����Ѵ�.
*/

#define __HEADER4_SIZE_2__ 

#ifdef	__HEADER4_SIZE_2__
	///////////////////////////////////////////////////////////////////////////////
	/// 4Byte Header	size�� header size 2byte�� ����
	//#define  MSG_HEADER		MSG_HEADER4
    //#define  SIZEOFHDR		2
    typedef MSG_HEADER4 MSG_HEADER;
    const WORD SIZEOFHDR = 2;

#elif	__HEADER8_SIZE_8__
	///////////////////////////////////////////////////////////////////////////////
	/// 8Byte Header	size�� header size ������
	//#define  MSG_HEADER		MSG_HEADER8
	//#define  SIZEOFHDR		8
    typedef MSG_HEADER8 MSG_HEADER;
    const WORD SIZEOFHDR = 8;

#else
	///////////////////////////////////////////////////////////////////////////////
	/// 8Byte Header	size�� header size ����
	//#define  MSG_HEADER		MSG_HEADER8
	//#define  SIZEOFHDR		0
    typedef MSG_HEADER8 MSG_HEADER;
    const WORD SIZEOFHDR = 8;
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : �ܿ�ĳ�� ��ȸ ��û
class MSG_GTOS_USER_CASH_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_USER_CASH_REQ()
	{
		size	= sizeof(MSG_GTOS_USER_CASH_REQ) - SIZEOFHDR;
		id		= GTOS_USER_CASH_REQ;
	}
	~MSG_GTOS_USER_CASH_REQ()
	{
	}

	DWORD	dwUserGuid;	///> ����� Guid
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : �ܿ�ĳ�� ��ȸ ����
class MSG_STOG_USER_CASH_ANS : public MSG_HEADER
{
public:
	MSG_STOG_USER_CASH_ANS()
	{
		size	= sizeof(MSG_STOG_USER_CASH_ANS) - SIZEOFHDR;
		id		= STOG_USER_CASH_ANS;
	}
	~MSG_STOG_USER_CASH_ANS()
	{
	}

	DWORD	dwUserGuid;	///> ����� Guid
	DWORD	dwUserCash;	///> �ܿ� ĳ��
#ifdef SHOPSERVER_FOR_CHINA
	DWORD	dwHonors;	///> �ܿ� ���ϸ��� ����(�ع決�� ����)
#endif
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : ���� ������ ��� ��û
class MSG_GTOS_ITEM_LIST_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_ITEM_LIST_REQ()
	{
		size	= sizeof(MSG_GTOS_ITEM_LIST_REQ) - SIZEOFHDR;
		id		= GTOS_ITEM_LIST_REQ;
	}
	~MSG_GTOS_ITEM_LIST_REQ()
	{
	}	
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : ���� ������ ��� ����
class MSG_STOG_ITEM_LIST_ANS : public MSG_HEADER
{
public:
	MSG_STOG_ITEM_LIST_ANS()
	{
		size	= sizeof(MSG_STOG_ITEM_LIST_ANS) - SIZEOFHDR;
		id		= STOG_ITEM_LIST_ANS;
	}
	~MSG_STOG_ITEM_LIST_ANS()
	{
	}
	
	void resize()
	{
		size = size - (MAX_ITEM_SIZE - (WORD)dwItemCount)* sizeof(tblSellItem);
	}
	
	DWORD 			dwItemCount;				///> ������ ��� ����
	tblSellItem 	sellItems[MAX_ITEM_SIZE];	///> ������ ���	
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : ��Ű�� ��� ��û
class MSG_GTOS_PACKAGE_LIST_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_PACKAGE_LIST_REQ()
	{
		size	= sizeof(MSG_GTOS_PACKAGE_LIST_REQ) - SIZEOFHDR;
		id		= GTOS_PACKAGE_LIST_REQ;
	}
	~MSG_GTOS_PACKAGE_LIST_REQ()
	{
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : ��Ű�� ��� ����
class MSG_STOG_PACKAGE_LIST_ANS : public MSG_HEADER
{
public:
	MSG_STOG_PACKAGE_LIST_ANS()
	{
		size	= sizeof(MSG_STOG_PACKAGE_LIST_ANS) - SIZEOFHDR;
		id		= STOG_PACKAGE_LIST_ANS;
	}
	~MSG_STOG_PACKAGE_LIST_ANS()
	{
	}
	
	void resize()
	{
		size = size - (MAX_PACKAGE_SIZE - (WORD)dwPackageCount)* sizeof(tblSellPackage);
	}
	
	DWORD 			dwPackageCount;					///> ��Ű�� ��� ����
	tblSellPackage 	sellPackages[MAX_PACKAGE_SIZE];	///> ��Ű�� ���
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : �з� ������ ��� ��û
class MSG_GTOS_BRANCH_ITEM_LIST_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_BRANCH_ITEM_LIST_REQ()
	{
		size	= sizeof(MSG_GTOS_BRANCH_ITEM_LIST_REQ) - SIZEOFHDR;
		id		= GTOS_BRANCH_ITEM_LIST_REQ;
	}
	~MSG_GTOS_BRANCH_ITEM_LIST_REQ()
	{
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : Ưȭ �з� ������ ��� ����
class MSG_STOG_BRANCH_ITEM_LIST_ANS : public MSG_HEADER
{
public:
	MSG_STOG_BRANCH_ITEM_LIST_ANS()
	{
		size	= sizeof(MSG_STOG_BRANCH_ITEM_LIST_ANS) - SIZEOFHDR;
		id		= STOG_BRANCH_ITEM_LIST_ANS;
	}
	~MSG_STOG_BRANCH_ITEM_LIST_ANS()
	{
	}
	
	void resize()
	{
		size = size - (MAX_BRANCH_ITEM_SIZE - (WORD)dwItemCount)* sizeof(tblItemBranch);
	}
	
	DWORD 			dwItemCount;						///> ��� ����
	tblItemBranch 	branchItems[MAX_BRANCH_ITEM_SIZE];	///> ���
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : ������ ���� ��û
class MSG_GTOS_BUY_ITEM_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_BUY_ITEM_REQ()
	{
		size	= sizeof(MSG_GTOS_BUY_ITEM_REQ) - SIZEOFHDR;
		id		= GTOS_BUY_ITEM_REQ;
	}
	~MSG_GTOS_BUY_ITEM_REQ()
	{
	}

	void resize()
	{
		size = size - (MAX_BUY_REQ_SIZE - (WORD)dwCount)* sizeof(DWORD);
	}

	DWORD	dwServerGuid;							///> ���� ��û ���� Guid
	DWORD	dwUserGuid;								///> ����� Guid
	DWORD	dwCharGuid;								///> ĳ���� Guid
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];	///> ĳ���� �̸�
	DWORD	dwCount;								///> ���� ������/��Ű�� ����
	DWORD	dwPriceGuids[MAX_BUY_REQ_SIZE];			///> �Ǹ� ���� ���� ��ȣ ���	
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : ������ ���� ����
class MSG_STOG_BUY_ITEM_ANS : public MSG_HEADER
{
public:
	MSG_STOG_BUY_ITEM_ANS()
	{
		size	= sizeof(MSG_STOG_BUY_ITEM_ANS) - SIZEOFHDR;
		id		= STOG_BUY_ITEM_ANS;
	}
	~MSG_STOG_BUY_ITEM_ANS()
	{
	}

	void resize()
	{
		size = size - (MAX_BUY_REQ_SIZE - (WORD)dwCount)* sizeof(tblBuyItemTransaction);
	}
	
	DWORD					dwServerGuid;					///> ���� ��û ���� Guid
	DWORD					dwUserGuid;						///> ����� Guid
	DWORD					dwCharGuid;						///> ĳ���� Guid
	DWORD					dwUserCash;						///> ����� �ܿ� ĳ��
#ifdef SHOPSERVER_FOR_CHINA
	DWORD					dwHonors;						///> �ܿ� ���ϸ��� ����(�ع決�� ����)
#endif
	DWORD					dwResult;						///> ���� ���
	DWORD					dwCount;						///> ���� transaction ����
	tblBuyItemTransaction	transactions[MAX_BUY_REQ_SIZE];	///> ���� transaction
	
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : ������ ���� ���ӳ� ó�� ���
class MSG_GTOS_BUY_ITEM_NTF : public MSG_HEADER
{
public:
	MSG_GTOS_BUY_ITEM_NTF()
	{
		size	= sizeof(MSG_GTOS_BUY_ITEM_NTF) - SIZEOFHDR;
		id		= GTOS_BUY_ITEM_NTF;
	}
	~MSG_GTOS_BUY_ITEM_NTF()
	{
	}

	void resize()
	{
		size = size - (MAX_BUY_REQ_SIZE - (WORD)dwCount)* sizeof(tblBuyItemTransaction);
	}

	DWORD					dwServerGuid;					///> ���� ��û ���� Guid
	DWORD					dwUserGuid;						///> ����� Guid
	DWORD					dwCharGuid;						///> ĳ���� Guid
	DWORD					dwCount;						///> ���� transaction ����
	tblBuyItemTransaction	transactions[MAX_BUY_REQ_SIZE];	///> ���� transaction		
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : ����� ���� �̷� ��û
class MSG_GTOS_USER_BUY_HISTORY_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_USER_BUY_HISTORY_REQ()
	{
		size	= sizeof(MSG_GTOS_USER_BUY_HISTORY_REQ) - SIZEOFHDR;
		id		= GTOS_USER_BUY_HISTORY_REQ;
	}
	~MSG_GTOS_USER_BUY_HISTORY_REQ()
	{
	}

	DWORD	dwServerGuid;	///> ��û ���� Guid
	DWORD	dwUserGuid;		///> ����� Guid
	DWORD	dwCharGuid;		///> ĳ���� Guid
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : ����� ���� �̷� ����
class MSG_STOG_USER_BUY_HISTORY_ANS : public MSG_HEADER
{
public:
	MSG_STOG_USER_BUY_HISTORY_ANS()
	{
		size	= sizeof(MSG_STOG_USER_BUY_HISTORY_ANS) - SIZEOFHDR;
		id		= STOG_USER_BUY_HISTORY_ANS;
	}
	~MSG_STOG_USER_BUY_HISTORY_ANS()
	{
	}
	void resize()
	{
		size = size - (MAX_BUY_ITEM_SIZE - (WORD)dwCount)* sizeof(tblBuyItem);
	}

	DWORD		dwServerGuid;					///> ��û ���� Guid
	DWORD 		dwUserGuid;						///> ����� Guid
	DWORD		dwCharGuid;						///> ĳ���� Guid
	DWORD 		dwCount;						///> ���� �̷� ��� ����
	tblBuyItem 	buyItems[MAX_BUY_ITEM_SIZE]; 	///> ���� �̷� ���
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : �� ����Ÿ ���濡 ���� ���� ��û�� �Ѵ�.
class MSG_STOG_RELOAD_SHOP_DATA_NTF : public MSG_HEADER
{
public:
	MSG_STOG_RELOAD_SHOP_DATA_NTF()
	{
		size	= sizeof(MSG_STOG_RELOAD_SHOP_DATA_NTF) - SIZEOFHDR;
		id		= STOG_RELOAD_SHOP_DATA_NTF;
	}
	~MSG_STOG_RELOAD_SHOP_DATA_NTF()
	{
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : Manager -> GAME
//  Description : �� ����Ÿ ���濡 ���� ���� ��û�� �Ѵ�.
class MSG_MTOS_RELOAD_SHOP_DATA_NTF : public MSG_HEADER8
{
public:
	MSG_MTOS_RELOAD_SHOP_DATA_NTF()
	{
		size	= sizeof(MSG_MTOS_RELOAD_SHOP_DATA_NTF);
		id		= MTOS_RELOAD_SHOP_DATA_NTF;
	}
	~MSG_MTOS_RELOAD_SHOP_DATA_NTF()
	{
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : ������ �����ϱ�
class MSG_GTOS_SEND_GIFT_ITEM_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_SEND_GIFT_ITEM_REQ()
	{
		size	= sizeof(MSG_GTOS_SEND_GIFT_ITEM_REQ) - SIZEOFHDR;
		id		= GTOS_SEND_GIFT_ITEM_REQ;
	}
	~MSG_GTOS_SEND_GIFT_ITEM_REQ()
	{
	}
	
	DWORD	dwServerGuid;								///> ���� ��û ���� Guid
	DWORD 	dwUserGuid;									///> ����� Guid
	DWORD	dwCharGuid;									///> ĳ���� Guid
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];		///> ĳ���� �̸�
	DWORD	dwToServerGuid;								///> ������ ���� ���� Guid
	DWORD 	dwToUserGuid;								///> ������ ���� ����� Guid
	DWORD	dwToCharGuid;								///> ������ ���� ĳ���� Guid	
	char	szToCharName[MAX_CHARACTER_NAME_LENGTH];  	///> ������ ���� ĳ���� �̸�
	DWORD 	dwPriceGuid;								///> �Ǹ� ���� ���� ��ȣ
	char 	szGiftMessage[MAX_GIFT_MESSAGE_LENGTH];		///> ���� �޼���
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : ������ ���� �ϱ� ��� ����
class MSG_STOG_SEND_GIFT_ITEM_ANS : public MSG_HEADER
{
public:
	MSG_STOG_SEND_GIFT_ITEM_ANS()
	{
		size	= sizeof(MSG_STOG_SEND_GIFT_ITEM_ANS) - SIZEOFHDR;
		id		= STOG_SEND_GIFT_ITEM_ANS;
	}
	~MSG_STOG_SEND_GIFT_ITEM_ANS()
	{
	}
	
	DWORD	dwServerGuid;		///> ���� ��û ���� Guid
	DWORD 	dwUserGuid;			///> ����� Guid
	DWORD	dwCharGuid;			///> ĳ���� Guid
	DWORD	dwToServerGuid;		///> ������ ���� ���� Guid
	DWORD	dwToUserGuid;		///> ������ ���� ����� Guid
	DWORD	dwToCharGuid;		///> ������ ���� ĳ���� Guid
	DWORD	dwUserCash;			///> ����� �ܿ� ĳ��
#ifdef SHOPSERVER_FOR_CHINA
	DWORD	dwHonors;			///> �ܿ� ���ϸ��� ����(�ع決�� ����)
#endif
	DWORD	dwTransactionGuid;	///> ���� ��û Transaction Guid
	DWORD 	dwPriceGuid;		///> �Ǹ� ���� ���� ��ȣ
	DWORD	dwResult;			///> ���� ���
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : ������ ���� �ޱ� ��û
class MSG_GTOS_ACTION_RECV_GIFT_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_ACTION_RECV_GIFT_REQ()
	{
		size	= sizeof(MSG_GTOS_ACTION_RECV_GIFT_REQ) - SIZEOFHDR;
		id		= GTOS_ACTION_RECV_GIFT_REQ;
	}
	~MSG_GTOS_ACTION_RECV_GIFT_REQ()
	{
	}
	DWORD	dwServerGuid;	///> ���� GUID
	DWORD 	dwUserGuid;		///> ����� GUID
	DWORD	dwCharGuid;		///> ĳ���� GUID
	DWORD	dwGiftItemID;	///> ���� ID
	DWORD 	dwAction;		///> ���� ó�� ��� (1:���� �ޱ�, 2:���� ������, 3:��Ͽ��� ���� ����) ���� : eGIFT_STATE
	char 	szMessage[MAX_GIFT_MESSAGE_LENGTH];		///> ���� �޼���
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : ������ ���� �ޱ� ��� ����
class MSG_STOG_ACTION_RECV_GIFT_ANS : public MSG_HEADER
{
public:
	MSG_STOG_ACTION_RECV_GIFT_ANS()
	{
		size	= sizeof(MSG_STOG_ACTION_RECV_GIFT_ANS) - SIZEOFHDR;
		id		= STOG_ACTION_RECV_GIFT_ANS;
	}
	~MSG_STOG_ACTION_RECV_GIFT_ANS()
	{
	}
	DWORD	dwServerGuid;	///> ���� GUID
	DWORD 	dwUserGuid;		///> ����� GUID
	DWORD	dwCharGuid;		///> ĳ���� GUID
	DWORD	dwGiftItemID;	///> ���� ID
	DWORD 	dwAction;		///> ���� ó�� ��� (1:���� �ޱ�, 2:���� ������, 3:��Ͽ��� ���� ����) ���� : eGIFT_STATE
	DWORD	dwResult;		///> ó�� ���
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : ������ ���� �ޱ� ��� ����
class MSG_GTOS_ACTION_RECV_GIFT_NTF : public MSG_HEADER
{
public:
	MSG_GTOS_ACTION_RECV_GIFT_NTF()
	{
		size	= sizeof(MSG_GTOS_ACTION_RECV_GIFT_NTF) - SIZEOFHDR;
		id		= GTOS_ACTION_RECV_GIFT_NTF;
	}
	~MSG_GTOS_ACTION_RECV_GIFT_NTF()
	{
	}
	DWORD	dwServerGuid;	///> ���� GUID
	DWORD 	dwUserGuid;		///> ����� GUID
	DWORD	dwCharGuid;		///> ĳ���� GUID
	DWORD	dwGiftItemID;	///> ���� GUID
	DWORD	dwResult;		///> ó�� ���(0 : ����, 1 :  ����)
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : ���� ���� ���� ����
class MSG_GTOS_IS_RECV_GIFT_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_IS_RECV_GIFT_REQ()
	{
		size	= sizeof(MSG_GTOS_IS_RECV_GIFT_REQ) - SIZEOFHDR;
		id		= GTOS_IS_RECV_GIFT_REQ;
	}
	~MSG_GTOS_IS_RECV_GIFT_REQ()
	{
	}
	
	DWORD		dwServerGuid;	///> ���� GUID
	DWORD 		dwUserGuid;		///> ����� GUID
	DWORD		dwCharGuid;		///> ĳ���� GUID
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : ���� ���� ���� ���� Ȯ�� ��� ����
class MSG_STOG_IS_RECV_GIFT_ANS : public MSG_HEADER
{
public:
	MSG_STOG_IS_RECV_GIFT_ANS()
	{
		size	= sizeof(MSG_STOG_IS_RECV_GIFT_ANS) - SIZEOFHDR;
		id		= STOG_IS_RECV_GIFT_ANS;
	}
	~MSG_STOG_IS_RECV_GIFT_ANS()
	{
	}
	DWORD	dwServerGuid;	///> ���� GUID
	DWORD 	dwUserGuid;		///> ����� GUID
	DWORD	dwCharGuid;		///> ĳ���� GUID
	DWORD 	dwGiftCount;	///> ���� ���� ���� (0 : ���� ���� ���� N : N���� ���� ������ ����)
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : ���� ���� ��� ��û
class MSG_GTOS_RECV_GIFT_LIST_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_RECV_GIFT_LIST_REQ()
	{
		size	= sizeof(MSG_GTOS_RECV_GIFT_LIST_REQ) - SIZEOFHDR;
		id		= GTOS_RECV_GIFT_LIST_REQ;
	}
	~MSG_GTOS_RECV_GIFT_LIST_REQ()
	{
	}
	DWORD	dwServerGuid;	///> ���� GUID
	DWORD	dwUserGuid;		///> ����� GUID
	DWORD	dwCharGuid;		///> ĳ���� GUID
	DWORD	dwMaxCount;		///> �ִ� ��� ����(0 : �⺻, N : N���� �ִ� ���)
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : ���� ���� ��� ����
class MSG_STOG_RECV_GIFT_LIST_ANS : public MSG_HEADER
{
public:
	MSG_STOG_RECV_GIFT_LIST_ANS()
	{
		size	= sizeof(MSG_STOG_RECV_GIFT_LIST_ANS) - SIZEOFHDR;
		id		= STOG_RECV_GIFT_LIST_ANS;
	}
	~MSG_STOG_RECV_GIFT_LIST_ANS()
	{
	}
	
	void resize()
	{
		size = size - (MAX_GIFT_ITEM_SIZE - (WORD)dwCount)* sizeof(tblRecvGiftItem);
	}
	
	DWORD			dwServerGuid;						///> ���� GUID
	DWORD 			dwUserGuid;							///> ����� GUID
	DWORD			dwCharGuid;							///> ĳ���� GUID
	DWORD			dwCount;							///> ���� ��� ����
	tblRecvGiftItem recvGiftItems[MAX_GIFT_ITEM_SIZE];	///> ���� ���� ���
};


//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : ���� ���� ��� ��û
class MSG_GTOS_SEND_GIFT_LIST_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_SEND_GIFT_LIST_REQ()
	{
		size	= sizeof(MSG_GTOS_SEND_GIFT_LIST_REQ) - SIZEOFHDR;
		id		= GTOS_SEND_GIFT_LIST_REQ;
	}
	~MSG_GTOS_SEND_GIFT_LIST_REQ()
	{
	}
	DWORD	dwServerGuid;	///> ���� GUID
	DWORD	dwUserGuid;		///> ����� GUID
	DWORD	dwCharGuid;		///> ĳ���� GUID
	DWORD	dwMaxCount;		///> �ִ� ��� ����(0 : �⺻, N : N���� �ִ� ���)
};

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : ���� ���� ��� ����
class MSG_STOG_SEND_GIFT_LIST_ANS : public MSG_HEADER
{
public:
	MSG_STOG_SEND_GIFT_LIST_ANS()
	{
		size	= sizeof(MSG_STOG_SEND_GIFT_LIST_ANS) - SIZEOFHDR;
		id		= STOG_SEND_GIFT_LIST_ANS;
	}
	~MSG_STOG_SEND_GIFT_LIST_ANS()
	{
	}

	void resize()
	{
		size = size - (MAX_GIFT_ITEM_SIZE - (WORD)dwCount)* sizeof(tblSendGiftItem);
	}

	DWORD			dwServerGuid;	///> ���� GUID
	DWORD			dwUserGuid;		///> ����� GUID
	DWORD			dwCharGuid;		///> ĳ���� GUID
	DWORD			dwCount;							///> ���� ��� ����
	tblSendGiftItem sendGiftItems[MAX_GIFT_ITEM_SIZE];	///> ���� ���� ���
};

#ifdef __NA_00387_20070605_SHOP_MILEAGE2

//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : GAME -> SHOP
//  Description : �Ǹ� ���ϸ��� ������ ��� ��û
class MSG_GTOS_MILEAGE_ITEM_LIST_REQ : public MSG_HEADER
{
public:
	MSG_GTOS_MILEAGE_ITEM_LIST_REQ()
	{
		size	= sizeof(MSG_GTOS_MILEAGE_ITEM_LIST_REQ) - SIZEOFHDR;
		id		= GTOS_MILEAGE_ITEM_LIST_REQ;
	}
	~MSG_GTOS_MILEAGE_ITEM_LIST_REQ()
	{
	}	
};
//////////////////////////////////////////////////////////////////////////////////////////////
//  Redirection : SHOP -> GAME
//  Description : �Ǹ� ���ϸ��� ������ ��� ����
class MSG_STOG_MILEAGE_ITEM_LIST_ANS : public MSG_HEADER
{
public:
	MSG_STOG_MILEAGE_ITEM_LIST_ANS()
	{
		size	= sizeof(MSG_STOG_MILEAGE_ITEM_LIST_ANS) - SIZEOFHDR;
		id		= STOG_MILEAGE_ITEM_LIST_ANS;
	}
	~MSG_STOG_MILEAGE_ITEM_LIST_ANS()
	{
	}

	void resize()
	{
		size = size - (MAX_ITEM_SIZE - (WORD)dwItemCount)* sizeof(tblSellMileageItem);
	}

	DWORD 				dwItemCount;														///> �Ǹ� ���ϸ��� ������ ��� ����
	tblSellMileageItem 	sellMileageItems[MAX_ITEM_SIZE];	///> �Ǹ� ���ϸ��� ������ ���	
};

#endif

};// namespace protocol

using namespace protocol;
#pragma pack()

#endif //SUN_CODE_BACKUP

#endif //SOLAR_SHOP_SHOP_SERVER_PROTOCOL_DEFINITION_H



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
