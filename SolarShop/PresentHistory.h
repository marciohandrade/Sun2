
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



//=============================================================================================================================
/// PresentHistory class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2007. 1. 8(��)
	@remark
		- ���� ������ ���� Ŭ����
	@note
		- priceguid�� ������������ ã�´�.
	@history 
		- 
*/
//=============================================================================================================================
#pragma once

#include "History.h"

class PresentHistory :
	public History
{
public:
	PresentHistory();
	virtual ~PresentHistory();

	char			m_Message[512+1];
	__REF_PROPERTY(ITEMGUID, PriceGuid);
};



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
