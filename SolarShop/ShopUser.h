
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



//=============================================================================================================================
/// ShopUser class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 12. 3(日)
	@remark
		- 유저에 대한 정보
	@note
		- 
	@history 
		- 
*/
//=============================================================================================================================
#pragma once

#include <list>
#include "Define.ShopServer.h"
#include "ShopDefine.h"

class History;
class PresentRecvBox;


class ShopUser
{
public:
	typedef std::list<History*> SHOPHISTORY_LIST;

public:
	ShopUser();
	virtual ~ShopUser();

	class TransactionInfo
	{
	public:
		TransactionInfo();
		~TransactionInfo(){}

		void Releas();

		const char* GetToGiftMessage() { return m_pszToGiftMessage; }
		void SetToGiftMessage(const char *p) { strncpy(m_pszToGiftMessage,p,MAX_GIFT_MESSAGE_LENGTH); m_pszToGiftMessage[MAX_GIFT_MESSAGE_LENGTH]='\0';}
	private:
		__PROPERTY(PRICEGUID, PriceGuid);
		char m_pszToGiftMessage[MAX_GIFT_MESSAGE_LENGTH+1];
		__PROPERTY(INT, ToUserGuid);
		__PROPERTY(INT, ToCharGuid);
	};

	virtual void Release();

    // (CHANGES) (100111) (WAVERIX) change data io interface, _NA00XXXX_090111_CASH_TYPE_SHARED_
    void SetUserCash(CASH cash);
    CASH GetUserCash() const;
    void SetUserMileage(DWORD mileage);
    DWORD GetUserMileage() const;
    void SetUserPPCash(CASH pp_cash);
    CASH GetUserPPCash() const;

    void SetCashData(const CashData& cash_data);
    const CashData& GetCashData() const;

private:
	__PTR_PROPERTY(TransactionInfo, TransactionInfo);
	__PROPERTY(USERGUID, UserGuid)
private:
    CashData cash_data_;
//	__PROPERTY(CASH, UserCash)
//#ifdef __CN_0_20070507_SHOP_MILEAGE
//	__PROPERTY(DWORD, UserMileage)
//#endif
	__BOOL_PROPERTY(Transaction)
	__PTR_PROPERTY(PresentRecvBox, PresentRecvBox);
};

//==================================================================================================

inline void
ShopUser::SetUserCash(CASH cash) {
    cash_data_.cash_ = cash;
}

inline CASH
ShopUser::GetUserCash() const {
    return cash_data_.cash_;
}

inline void
ShopUser::SetUserMileage(DWORD mileage) {
    cash_data_.mileage_ = mileage;
}

inline DWORD
ShopUser::GetUserMileage() const {
    return cash_data_.mileage_;
}

inline void
ShopUser::SetUserPPCash(CASH pp_cash) {
    cash_data_.pp_cash_ = pp_cash;
}

inline CASH
ShopUser::GetUserPPCash() const {
    return cash_data_.pp_cash_;
}

inline void
ShopUser::SetCashData(const CashData& cash_data) {
    cash_data_ = cash_data;
}

inline const CashData&
ShopUser::GetCashData() const {
    return cash_data_;
}

#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
