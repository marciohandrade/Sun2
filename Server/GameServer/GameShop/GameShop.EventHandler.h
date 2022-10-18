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



    // 상품 샵 스크립트 최신 버전 정보 조회
    virtual void OnInquireSalesZoneScriptVersion(unsigned short SalesZone, 
        unsigned short Year, 
        unsigned short YearIdentity, 
        long ResultCode);

    // 배너 스크립트 최신 버전 정보 조회
    virtual void OnInquireBannerZoneScriptVersion(unsigned short BannerZone, 
        unsigned short Year, 
        unsigned short YearIdentity, 
        long ResultCode);

    // 캐시 조회의 결과
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

    // 상품 구매의 결과
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

    // 상품 선물의 결과
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

    // 캐시 선물 결과
    virtual void OnGiftCash(DWORD SenderSeq, 
        DWORD ReceiverSeq, 
        long ResultCode, 
        double GiftSendLimit);

    // 상품 구매/선물 가능 조회 결과
    virtual void OnInquireBuyGiftPossibility(DWORD AccountID, 
        long ResultCode, 
        long ItemBuy, 
        long Present, 
        double MyPresentCash, 
        double MyPresentSendLimit);

    // 이벤트 상품 리스트 조회 결과
    virtual void OnInquireEventProductList(DWORD AccountSeq, 
        long ProductDisplaySeq, 
        int PackagesCount, 
        long Packages[]);

    // 전시 상품 잔여 개수 조회 결과
    virtual void OnInquireProductLeftCount(long PackageSeq, long LeftCount);

    // 보관함 전체 조회 결과
    virtual void OnInquireStorageList(DWORD AccountID, 
        long ListCount, 
        long ResultCode, 
        ShopProtocol::STStorage StorageList[]);

    // 보관함 페이지 조회 결과
    virtual void OnInquireStorageListPage(DWORD AccountID, 
        int	ResultCode, 
        char  StorageType,
        int   NowPage,
        int   TotalPage,
        int   TotalCount,
        int   ListCount, 
        ShopProtocol::STStorage StorageList[]);

    // 보관함 페이지 조회 결과 - 선물 메시지 제외
    virtual void OnInquireStorageListPageNoGiftMessage(DWORD AccountID, 
        int   ResultCode, 
        char  StorageType,
        int   NowPage,
        int   TotalPage,
        int   TotalCount,
        int   ListCount, 
        ShopProtocol::STStorageNoGiftMessage StorageList[]);

    // 보관함 조회 결과 + 만료시점 추가
    virtual void OnInquireStorageListEx(DWORD AccountID, 
        long ListCount, 
        long ResultCode, 
        ShopProtocol::STStorageEx StorageListEx[]);

    // 보관함 페이지 조회 결과 + 만료시점 추가
    virtual void OnInquireStorageListPageEx(DWORD AccountID, 
        int	ResultCode, 
        char  StorageType,
        int	NowPage,
        int   TotalPage,
        int   TotalCount,
        int   ListCount, 
        ShopProtocol::STStorageEx StorageListEx[]);

    // 보관함 페이지 조회 결과 - 선물 메시지 제외 + 만료시점 추가
    virtual void OnInquireStorageListPageNoGiftMessageEx(DWORD AccountID, 
        int   ResultCode, 
        char  StorageType,
        int   NowPage,
        int   TotalPage,
        int   TotalCount,
        int   ListCount, 
        ShopProtocol::STStorageNoGiftMessageEx StorageListEx[]);

    // 보관함 상품 사용하기 결과
    virtual void OnUseStorage(DWORD AccountSeq, 
        long ResultCode, 
        char InGameProductID[MAX_TYPENAME_LENGTH], 
        BYTE PropertyCount, 
        long ProductSeq, 
        long StorageSeq, 
        long StorageItemSeq, 
        ShopProtocol::STItemProperty PropertyList[]);

    // 보관함 상품 사용하기 결과 Ex : 모든 게임
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

    // 보관함 상품 사용하기 결과 Ex : SUN_CN
    //	DeductMileageFlag 추가 (false : 캐시상품(0), true : 마일리지 상품(1))
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

    // 보관함 상품 사용하기 롤백
    virtual void OnRollbackUseStorage(DWORD AccountSeq, long ResultCode);

    // 보관함 상품 버리기 결과
    virtual void OnThrowStorage(DWORD AccountSeq, long ResultCode);

    // 마일리지 소진
    virtual void OnMileageDeduct(DWORD AccountSeq, long ResultCode);

    // 마일리지 적립
    virtual void OnMileageSave(DWORD AccountSeq, long ResultCode);

    // 실시간 마일리지 적립
    virtual void OnMileageLiveSaveUp(DWORD AccountSeq, long ResultCode);

    // 아이템 시리얼 코드 업데이트
    virtual void OnItemSerialUpdate(DWORD AccountSeq, long ResultCode);

    // 상품 샵 스크립트 버전 정보 업데이트 알림
    virtual void OnUpdateVersion(long GameCode, 
        unsigned short SalesZone, 
        unsigned short year, 
        unsigned short yearIdentity);

    // 배너 스크립트 버전 정보 업데이트 알림
    virtual void OnUpdateBannerVersion(long GameCode, 
        unsigned short BannerZone, 
        unsigned short year, 
        unsigned short yearIdentity);

    // 인게임 포인트 유형별 지급 퍼센트 조회
    virtual void OnInquireInGamePointValue(long ResultCode,
        long PointCount,
        ShopProtocol::STPointDetail PointList[]);

    // 로그를 써야할때 보내는 이벤트 메소드
    virtual void WriteLog(char* szMsg);


private:
    bool connect_;
    GameShop::System* shop_system_;
};

}; //namespace GameShop

#endif // GAMESHOP_EVENTHANDLER_H_

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
