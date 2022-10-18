
#include "stdafx.h"

#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL




#include ".\shopuser.h"
#include ".\PresentRecvBox.h"

ShopUser::ShopUser()
:	m_UserGuid(0)
//,	m_UserCash(0)
//#ifdef __CN_0_20070507_SHOP_MILEAGE
//,	m_UserMileage(0)
//#endif
,	m_Transaction(false)
,	m_pPresentRecvBox(NULL)
{
    ZeroMemory(&cash_data_, sizeof(cash_data_));

	m_pPresentRecvBox = new PresentRecvBox;
	m_pTransactionInfo = new TransactionInfo;
}

ShopUser::~ShopUser()
{
	SAFE_DELETE(m_pTransactionInfo);
	SAFE_DELETE(m_pPresentRecvBox);
}

void ShopUser::Release()
{
	m_UserGuid=(0);
	//m_UserCash=(0);
    ZeroMemory(&cash_data_, sizeof(cash_data_));

	m_Transaction=false;

	m_pPresentRecvBox->Release();
}


ShopUser::TransactionInfo::TransactionInfo()
:	m_PriceGuid(0)
,	m_ToUserGuid(0)
,	m_ToCharGuid(0)
{
	m_pszToGiftMessage[0] = '\0';
}
void ShopUser::TransactionInfo::Releas()
{
	m_PriceGuid=(0);
	m_ToUserGuid=(0);
	m_ToCharGuid=(0);
	m_pszToGiftMessage[0] = '\0';
}


#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
