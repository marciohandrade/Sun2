#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "HeimTradingvolumeVendorBuy.h"
#include "GlobalFunc.h"

HeimTradingvolumeVendorBuy::HeimTradingvolumeVendorBuy(const POSTYPE& vendor_pos, const MONEY& money) : 
vendor_pos_(vendor_pos), 
money_(money) 
{
}

HeimTradingvolumeVendorBuy::~HeimTradingvolumeVendorBuy(void)
{
}

void HeimTradingvolumeVendorBuy::Execute()
{
	MSG_CG_VENDOR_BUY_SYN sendMsg;
	sendMsg.m_byCategory    = CG_VENDOR;
	sendMsg.m_byProtocol    = CG_VENDOR_BUY_SYN;
	sendMsg.m_VendorPos     = vendor_pos_;

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(sendMsg) );
}

MONEY HeimTradingvolumeVendorBuy::GetTradingvolume()
{
	return money_;
}

#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
