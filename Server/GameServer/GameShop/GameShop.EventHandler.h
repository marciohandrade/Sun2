#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


#ifndef GAMESHOP_EVENTHANDLER_H_
#define GAMESHOP_EVENTHANDLER_H_


#if _MSC_VER > 1000
#pragma once
#endif

//#include "GameShopHeader.h"
//#include <Winsock2.h>
//#include <WBANetwork/WBANetwork.h>

#include <ShopEventHandler.h>

#pragma warning(disable:4995)
#pragma warning(disable:4996)

namespace GameShop
{

class System;

class EventHandler : public CShopEventHandler
{
public:
    EventHandler();
    virtual ~EventHandler(void);

    bool IsConnect()
    {
        return connect_;
    }

    void Init(GameShop::System* shop_system)
    {
        shop_system_ = shop_system;
    }

protected:
    //���������� ���� ���� ���� (���� ����Ǹ� ���� ������ �ʿ��� ��� ������ ������ �ٽ� �α����ؾ��Ѵ�.)
    //		success: ���� ���� ����
    //		error: �����ڵ�
    void OnNetConnect(bool success, DWORD error);

    //���������� ���� ����Ÿ ���� ����
    //		size: ���۵� ��Ŷ ������
    void OnNetSend( int size );

    //���������� ������ �������� (���������� ���������� ó�� �ؾ��Ѵ�.)	
    //		error: �����ڵ�
    void OnNetClose( DWORD error );	



    // ��ǰ �� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ
    virtual void OnInquireSalesZoneScriptVersion(unsigned short SalesZone, 
        unsigned short Year, 
        unsigned short YearIdentity, 
        long ResultCode);

    // ��� ��ũ��Ʈ �ֽ� ���� ���� ��ȸ
    virtual void OnInquireBannerZoneScriptVersion(unsigned short BannerZone, 
        unsigned short Year, 
        unsigned short YearIdentity, 
        long ResultCode);

    // ĳ�� ��ȸ�� ���
    // MU KR, Battery, R2
    virtual void OnInquireCash(DWORD AccountSeq, 
        double CashSum, 
        double PointSum, 
        double MileageSum, 
        int DetailCount, 
        ShopProtocol::STCashDetail Detail[], 
        long ResultCode);
    // MU GB
    virtual void OnInquireCash(DWORD AccountSeq, 
        double CashSum, 
        double PointSum, 
        double MileageSum, 
        int ListCount, 
        ShopProtocol::STCashDetail_GB Detail[], 
        long ResultCode);
    // MU JP - GameChu, GameOn 
    virtual void OnInquireCash(DWORD AccountSeq, 
        long ResultCode,
        long OutBoundResultCode,
        double CashSum, 
        double MileageSum);
    // Hx
    virtual void OnInquireCash(DWORD AccountSeq, 
        char AccountID[MAX_ACCOUNTID_LENGTH], 
        double CashSum, 
        double PointSum, 
        double MileageSum, 
        int DetailCount, 
        ShopProtocol::STCashDetail Detail[], 
        long ResultCode,
        long OutBoundResultCode);

    // ��ǰ ������ ���
    // MU KR, GB, Battery, R2
    virtual void OnBuyProduct(DWORD AccountSeq, 
        long ResultCode, 
        long LeftProductCount);
    // MU JP - GameChu, GameOn 
    virtual void OnBuyProduct(DWORD AccountSeq, 
        long ResultCode,
        long OutBoundResultCode,
        long LeftProductCount);
    // Hx
    virtual void OnBuyProduct(DWORD AccountSeq, 
        char AccountID[MAX_ACCOUNTID_LENGTH], 
        long LeftProductCount, 
        long ResultCode,
        long OutBoundResultCode);

    // ��ǰ ������ ���
    // MU KR, GB, Battery, R2
    virtual void OnGiftProduct(DWORD SenderSeq, 
        DWORD ReceiverSeq, 
        double LimitedCash, 
        long LeftProductCount, 
        long ResultCode);
    // MU JP - GameChu, GameOn 
    virtual void OnGiftProduct(DWORD  SenderAccountSeq, 
        long   ResultCode,
        long   OutBoundResultCode,
        DWORD  ReceiverAccountSeq, 
        long   LeftProductCount);
    // Hx
    virtual void OnGiftProduct(DWORD  SenderAccountSeq, 
        char   SenderAccountID[MAX_ACCOUNTID_LENGTH], 
        WCHAR  SenderCharName[MAX_CHARACTERID_LENGTH], 
        DWORD  ReceiverAccountSeq, 
        char   ReceiverAccountID[MAX_ACCOUNTID_LENGTH], 
        WCHAR  ReceiverCharName[MAX_CHARACTERID_LENGTH], 
        WCHAR  Message[MAX_MESSAGE_LENGTH], 
        double LimitedCash, 
        long   LeftProductCount, 
        long   ResultCode,
        long   OutBoundResultCode);

    // ĳ�� ���� ���
    virtual void OnGiftCash(DWORD SenderSeq, 
        DWORD ReceiverSeq, 
        long ResultCode, 
        double GiftSendLimit);

    // ��ǰ ����/���� ���� ��ȸ ���
    virtual void OnInquireBuyGiftPossibility(DWORD AccountID, 
        long ResultCode, 
        long ItemBuy, 
        long Present, 
        double MyPresentCash, 
        double MyPresentSendLimit);

    // �̺�Ʈ ��ǰ ����Ʈ ��ȸ ���
    virtual void OnInquireEventProductList(DWORD AccountSeq, 
        long ProductDisplaySeq, 
        int PackagesCount, 
        long Packages[]);

    // ���� ��ǰ �ܿ� ���� ��ȸ ���
    virtual void OnInquireProductLeftCount(long PackageSeq, long LeftCount);

    // ������ ��ü ��ȸ ���
    virtual void OnInquireStorageList(DWORD AccountID, 
        long ListCount, 
        long ResultCode, 
        ShopProtocol::STStorage StorageList[]);

    // ������ ������ ��ȸ ���
    virtual void OnInquireStorageListPage(DWORD AccountID, 
        int	ResultCode, 
        char  StorageType,
        int   NowPage,
        int   TotalPage,
        int   TotalCount,
        int   ListCount, 
        ShopProtocol::STStorage StorageList[]);

    // ������ ������ ��ȸ ��� - ���� �޽��� ����
    virtual void OnInquireStorageListPageNoGiftMessage(DWORD AccountID, 
        int   ResultCode, 
        char  StorageType,
        int   NowPage,
        int   TotalPage,
        int   TotalCount,
        int   ListCount, 
        ShopProtocol::STStorageNoGiftMessage StorageList[]);

    // ������ ��ȸ ��� + ������� �߰�
    virtual void OnInquireStorageListEx(DWORD AccountID, 
        long ListCount, 
        long ResultCode, 
        ShopProtocol::STStorageEx StorageListEx[]);

    // ������ ������ ��ȸ ��� + ������� �߰�
    virtual void OnInquireStorageListPageEx(DWORD AccountID, 
        int	ResultCode, 
        char  StorageType,
        int	NowPage,
        int   TotalPage,
        int   TotalCount,
        int   ListCount, 
        ShopProtocol::STStorageEx StorageListEx[]);

    // ������ ������ ��ȸ ��� - ���� �޽��� ���� + ������� �߰�
    virtual void OnInquireStorageListPageNoGiftMessageEx(DWORD AccountID, 
        int   ResultCode, 
        char  StorageType,
        int   NowPage,
        int   TotalPage,
        int   TotalCount,
        int   ListCount, 
        ShopProtocol::STStorageNoGiftMessageEx StorageListEx[]);

    // ������ ��ǰ ����ϱ� ���
    virtual void OnUseStorage(DWORD AccountSeq, 
        long ResultCode, 
        char InGameProductID[MAX_TYPENAME_LENGTH], 
        BYTE PropertyCount, 
        long ProductSeq, 
        long StorageSeq, 
        long StorageItemSeq, 
        ShopProtocol::STItemProperty PropertyList[]);

    // ������ ��ǰ ����ϱ� ��� Ex : ��� ����
    virtual void OnUseStorageEx(
        DWORD	AccountSeq,
        long	ResultCode,
        char	InGameProductID[MAX_TYPENAME_LENGTH],
        BYTE	PropertyCount,
        long	ProductSeq,
        long	StorageSeq,
        long	StorageItemSeq,
        ShopProtocol::STItemProperty PropertyList[],
        int		CashType);

    // ������ ��ǰ ����ϱ� ��� Ex : SUN_CN
    //	DeductMileageFlag �߰� (false : ĳ�û�ǰ(0), true : ���ϸ��� ��ǰ(1))
    virtual void OnUseStorageExDeductMileageFlag(
        DWORD	AccountSeq,
        long	ResultCode,
        char	InGameProductID[MAX_TYPENAME_LENGTH],
        BYTE	PropertyCount,
        long	ProductSeq,
        long	StorageSeq,
        long	StorageItemSeq,
        ShopProtocol::STItemProperty PropertyList[],
        bool	DeductMileageFlag);

    // ������ ��ǰ ����ϱ� �ѹ�
    virtual void OnRollbackUseStorage(DWORD AccountSeq, long ResultCode);

    // ������ ��ǰ ������ ���
    virtual void OnThrowStorage(DWORD AccountSeq, long ResultCode);

    // ���ϸ��� ����
    virtual void OnMileageDeduct(DWORD AccountSeq, long ResultCode);

    // ���ϸ��� ����
    virtual void OnMileageSave(DWORD AccountSeq, long ResultCode);

    // �ǽð� ���ϸ��� ����
    virtual void OnMileageLiveSaveUp(DWORD AccountSeq, long ResultCode);

    // ������ �ø��� �ڵ� ������Ʈ
    virtual void OnItemSerialUpdate(DWORD AccountSeq, long ResultCode);

    // ��ǰ �� ��ũ��Ʈ ���� ���� ������Ʈ �˸�
    virtual void OnUpdateVersion(long GameCode, 
        unsigned short SalesZone, 
        unsigned short year, 
        unsigned short yearIdentity);

    // ��� ��ũ��Ʈ ���� ���� ������Ʈ �˸�
    virtual void OnUpdateBannerVersion(long GameCode, 
        unsigned short BannerZone, 
        unsigned short year, 
        unsigned short yearIdentity);

    // �ΰ��� ����Ʈ ������ ���� �ۼ�Ʈ ��ȸ
    virtual void OnInquireInGamePointValue(long ResultCode,
        long PointCount,
        ShopProtocol::STPointDetail PointList[]);

    // �α׸� ����Ҷ� ������ �̺�Ʈ �޼ҵ�
    virtual void WriteLog(char* szMsg);


private:
    bool connect_;
    GameShop::System* shop_system_;
};

}; //namespace GameShop

#endif // GAMESHOP_EVENTHANDLER_H_

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
