#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL

#pragma once

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_GS.h>
#include <Define.ShopServer.h>
#include <Protocol.ShopServer.h>


#pragma pack(push,1)




#define PACKET_GS_OPEN( c, p )	struct MSG_##p : public MSG_BASE_INTERNAL { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_GS_CLOSE		};


// (NOTE) �ܿ� ĳ�� ��û
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_USER_CASH_REQ_SYN )
	DWORD	dwUserGuid;	//< ����� Guid
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    char user_id[ID_MAX_LEN_FOR_SHOP_USER_ID + 1];
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN  gamechu_usn_;
#endif
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_USER_CASH_REQ_ACK )
	DWORD	dwUserGuid;	//< ����� Guid
	DWORD	dwUserCash;	//< �ܿ� ĳ��
#ifdef __CN_0_20070507_SHOP_MILEAGE
	DWORD	dwHonors;	//< �ܿ� ���ϸ��� ����(�ع決�� ����)
#endif
#if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    DWORD   pp_cash;  //< ���� ī��� ������ ĳ��
#endif
PACKET_GS_CLOSE


PACKET_GS_OPEN( GS_SHOP, GS_SHOP_ITEM_LIST_REQ_SYN )
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_ITEM_LIST_REQ_ACK )
	DWORD 			dwItemCount;				//< ������ ��� ����
	tblSellItem 	sellItems[MAX_ITEM_SIZE];	//< ������ ���	
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_PACK_LIST_REQ_SYN )
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_PACK_LIST_REQ_ACK )
	DWORD 			dwPackageCount;					//< ��Ű�� ��� ����
	tblSellPackage 	sellPackages[MAX_PACKAGE_SIZE];	//< ��Ű�� ���
PACKET_GS_CLOSE

// ����, �� ������ Ȯ��ɼ� ��û
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_EXTEND_OPTION_LIST_REQ_SYN )
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK )
	DWORD 					dwItemOptionCount;								//< �����ۿɼ� ��� ����
	tblItemExtendOption 	itemExtendOptions[MAX_ITEM_EXTEND_OPTION_SIZE];	//< �����ۿɼ� ���	
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_BRANCH_LIST_REQ_SYN )
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_BRANCH_LIST_REQ_ACK )
	DWORD 			dwItemCount;						//< ��� ����
	tblItemBranch 	branchItems[MAX_BRANCH_ITEM_SIZE];	//< ���
PACKET_GS_CLOSE


// (NOTE) ������ ���� ��û
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_BUY_ITEM_SYN )
	DWORD	dwServerGuid;							//< ���� ��û ���� Guid
	DWORD	dwUserGuid;								//< ����� Guid
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    char user_id[ID_MAX_LEN_FOR_SHOP_USER_ID + 1];
    DWORD   char_class;
    DWORD   char_level;
#endif
	DWORD	dwCharGuid;								//< ĳ���� Guid
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];	//< ĳ���� �̸�
	DWORD	dwCount;								//< ���� ������/��Ű�� ����
	DWORD	dwPriceGuids[MAX_BUY_REQ_SIZE];	
#if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    enum {
        PURCHASE_DEFAULT = buyItemTransaction::PURCHASE_DEFAULT,
        PURCHASE_PP_CASH = buyItemTransaction::PURCHASE_PP_CASH
    };
    BYTE    buy_cash_types[MAX_BUY_REQ_SIZE];      //< ���� ĳ��Ÿ�� (1:ĳ��, 2:����ī��_PP_)
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN  gamechu_usn_;
    char        ip_address_[MAX_IP_LENGTH];
#endif
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_BUY_ITEM_ACK )
	DWORD					dwServerGuid;					///> ���� ��û ���� Guid
	DWORD					dwUserGuid;						///> ����� Guid
	DWORD					dwCharGuid;						///> ĳ���� Guid
	DWORD					dwUserCash;						///> ����� �ܿ� ĳ��
#ifdef __CN_0_20070507_SHOP_MILEAGE
	DWORD					dwHonors;						///> �ܿ� ���ϸ��� ����(�ع決�� ����)
#endif
#if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    DWORD                   pp_cash;                        // ����ī�� ĳ�� �ܾ�
#endif
	DWORD					dwResult;						///> ���� ���
	DWORD					dwCount;						///> ���� transaction ����
	tblBuyItemTransaction	transactions[MAX_BUY_REQ_SIZE];	///> ���� transaction
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_BUY_ITEM_NTF )
	DWORD					dwServerGuid;					//< ���� ��û ���� Guid
	DWORD					dwUserGuid;						//< ����� Guid
	DWORD					dwCharGuid;						//< ĳ���� Guid
	DWORD					dwCount;						//< ���� transaction ����
	tblBuyItemTransaction	transactions[MAX_BUY_REQ_SIZE];	//< ���� transaction			
PACKET_GS_CLOSE



PACKET_GS_OPEN( GS_SHOP, GS_SHOP_USER_BUY_HISTORY_SYN )
	DWORD	dwServerGuid;	//< ��û ���� Guid
	DWORD	dwUserGuid;		//< ����� Guid
	DWORD	dwCharGuid;		//< ĳ���� Guid
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_USER_BUY_HISTORY_ACK )
	DWORD		dwServerGuid;					//< ��û ���� Guid
	DWORD 		dwUserGuid;						//< ����� Guid
	DWORD		dwCharGuid;						//< ĳ���� Guid
	DWORD 		dwCount;						//< ���� �̷� ��� ����
	tblBuyItem 	buyItems[MAX_BUY_ITEM_SIZE]; 	//< ���� �̷� ���
PACKET_GS_CLOSE


PACKET_GS_OPEN( GS_SHOP, GS_SHOP_RELOAD_DATA_NTF )
PACKET_GS_CLOSE


// 2007/1/17 ���� 4:57:28
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_SEND_GIFT_SYN )
	DWORD	dwServerGuid;								//< ���� ��û ���� Guid
	DWORD 	dwUserGuid;									//< ����� Guid
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    char    user_id[ID_MAX_LEN_FOR_SHOP_USER_ID + 1];
    DWORD   char_class;
    DWORD   char_level;
#endif
	DWORD	dwCharGuid;									//< ĳ���� Guid
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];		//< ĳ���� �̸�
	DWORD	dwToServerGuid;								//< ������ ���� ���� Guid
	DWORD 	dwToUserGuid;								//< ������ ���� ����� Guid
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    char    to_user_id[ID_MAX_LEN_FOR_SHOP_USER_ID + 1];
    DWORD   to_char_class;
    DWORD   to_char_level;
#endif
	DWORD	dwToCharGuid;								//< ������ ���� ĳ���� Guid	
	char	szToCharName[MAX_CHARACTER_NAME_LENGTH];  	//< ������ ���� ĳ���� �̸�
	DWORD 	dwPriceGuid;								//< �Ǹ� ���� ���� ��ȣ
#if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    enum {
        PURCHASE_DEFAULT = buyItemTransaction::PURCHASE_DEFAULT,
        PURCHASE_PP_CASH = buyItemTransaction::PURCHASE_PP_CASH
    };
    BYTE   buy_cash_type;                              //< ���� ĳ�� Ÿ��(1:etc, 2:pp_cash)
#endif
	char 	szGiftMessage[MAX_GIFT_MESSAGE_LENGTH];		//< ���� �޼���
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN  gamechu_usn_;
    char        ip_address_[MAX_IP_LENGTH];
#endif
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_SEND_GIFT_ACK )
	DWORD	dwServerGuid;		//< ���� ��û ���� Guid
	DWORD 	dwUserGuid;			//< ����� Guid
	DWORD	dwCharGuid;			//< ĳ���� Guid
	DWORD	dwToServerGuid;		//< ������ ���� ����� Guid
	DWORD	dwToUserGuid;		//< ������ ���� ����� Guid
	DWORD	dwToCharGuid;		//< ������ ���� ĳ���� Guid
	DWORD	dwUserCash;			//< ����� �ܿ� ĳ��
#ifdef __CN_0_20070507_SHOP_MILEAGE
	DWORD	dwHonors;			///> �ܿ� ���ϸ��� ����(�ع決�� ����)
#endif
#if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    DWORD   pp_cash;            //< �ܿ� ���� ĳ��
#endif
	DWORD	dwTransactionGuid;	//< ���� ��û Transaction Guid
	DWORD 	dwPriceGuid;		//< �Ǹ� ���� ���� ��ȣ
	DWORD	dwResult;			//< ���� ���
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_ACTION_RECV_GIFT_SYN )
	DWORD	dwServerGuid;	//< ���� GUID
	DWORD 	dwUserGuid;		//< ����� GUID
	DWORD	dwCharGuid;		//< ĳ���� GUID
	DWORD	dwGiftItemID;	//< ���� ID
	DWORD 	dwAction;		//< ���� ó�� ��� (1:���� �ޱ�, 2:���� ������, 3:��Ͽ��� ���� ����) ���� : eGIFT_ACTION
	char 	szMessage[MAX_GIFT_MESSAGE_LENGTH];		//< ���� �޼���
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_ACTION_RECV_GIFT_ACK )
	DWORD	dwServerGuid;	///> ���� GUID
	DWORD 	dwUserGuid;		///> ����� GUID
	DWORD	dwCharGuid;		///> ĳ���� GUID
	DWORD	dwGiftItemID;	///> ���� ID
	DWORD 	dwAction;		///> ���� ó�� ��� (1:���� �ޱ�, 2:���� ������, 3:��Ͽ��� ���� ����) ���� : eGIFT_ACTION
	DWORD	dwResult;		///> ó�� ���
PACKET_GS_CLOSE
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_ACTION_RECV_GIFT_NTF )
	DWORD	dwServerGuid;	//< ���� GUID
	DWORD 	dwUserGuid;		//< ����� GUID
	DWORD	dwCharGuid;		//< ĳ���� GUID
	DWORD	dwGiftItemID;	//< ���� GUID
	DWORD	dwResult;		//< ó�� ���(0 : ����, 1 :  ����)
PACKET_GS_CLOSE


PACKET_GS_OPEN( GS_SHOP, GS_SHOP_IS_RECV_GIFT_SYN )
	DWORD		dwServerGuid;	//< ���� GUID
	DWORD 		dwUserGuid;		//< ����� GUID
	DWORD		dwCharGuid;		//< ĳ���� GUID
PACKET_GS_CLOSE
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_IS_RECV_GIFT_ACK )
	DWORD	dwServerGuid;	//< ���� GUID
	DWORD 	dwUserGuid;		//< ����� GUID
	DWORD	dwCharGuid;		//< ĳ���� GUID
	DWORD 	dwGiftCount;	//< ���� ���� ���� (0 : ���� ���� ���� N : N���� ���� ������ ����)
PACKET_GS_CLOSE



PACKET_GS_OPEN( GS_SHOP, GS_SHOP_RECV_GIFT_LIST_SYN )
	DWORD	dwServerGuid;	//< ���� GUID
	DWORD	dwUserGuid;		//< ����� GUID
	DWORD	dwCharGuid;		//< ĳ���� GUID
	DWORD	dwMaxCount;		//< �ִ� ��� ����(0 : �⺻, N : N���� �ִ� ���)
PACKET_GS_CLOSE
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_RECV_GIFT_LIST_ACK )
	DWORD			dwServerGuid;						//< ���� GUID
	DWORD 			dwUserGuid;							//< ����� GUID
	DWORD			dwCharGuid;							//< ĳ���� GUID
	DWORD			dwCount;							//< ���� ��� ����
	tblRecvGiftItem recvGiftItems[MAX_GIFT_ITEM_SIZE];	//< ���� ���� ���
PACKET_GS_CLOSE


PACKET_GS_OPEN( GS_SHOP, GS_SHOP_SEND_GIFT_LIST_SYN )
	DWORD	dwServerGuid;	//< ���� GUID
	DWORD	dwUserGuid;		//< ����� GUID
	DWORD	dwCharGuid;		//< ĳ���� GUID
	DWORD	dwMaxCount;		//< �ִ� ��� ����(0 : �⺻, N : N���� �ִ� ���)
PACKET_GS_CLOSE
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_SEND_GIFT_LIST_ACK )
	DWORD			dwServerGuid;	//< ���� GUID
	DWORD			dwUserGuid;		//< ����� GUID
	DWORD			dwCharGuid;		//< ĳ���� GUID
	DWORD			dwCount;							//< ���� ��� ����
	tblSendGiftItem sendGiftItems[MAX_GIFT_ITEM_SIZE];	//< ���� ���� ���
PACKET_GS_CLOSE

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
// ���ϸ��� ���� ��Ŷ
PACKET_GS_OPEN( GS_SHOP, GS_SHOP_MILEAGE_LIST_REQ_SYN )
PACKET_GS_CLOSE

PACKET_GS_OPEN( GS_SHOP, GS_SHOP_MILEAGE_LIST_REQ_ACK )
	DWORD 				dwItemCount;							//< �Ǹ� ���ϸ��� ������ ��� ����
	tblSellItem 	sellItems[MAX_ITEM_SIZE];					//< ������ ���	
	// ������ �� struct �� tblSellItem == tblSellMileageItem
	// tblSellMileageItem 	sellMileageItems[MAX_ITEM_SIZE];	//< �Ǹ� ���ϸ��� ������ ���	
PACKET_GS_CLOSE
#endif

#pragma pack(pop)

#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
