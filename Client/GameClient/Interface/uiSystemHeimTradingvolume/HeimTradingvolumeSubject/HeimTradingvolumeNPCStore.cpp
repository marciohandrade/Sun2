#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "HeimTradingvolumeNPCStore.h"
#include "ShopDialog.h"
#include <SCSlotContainer.h>
#include "ItemManager.h"

HeimTradingvolumeNPCStore::HeimTradingvolumeNPCStore( const NUMTYPE& divide_num, 
													  const SLOTIDX& from_slot_idx, 
													  const POSTYPE& from_postype, 
													  const eSlotType& slot_type, 
													  const SLOTCODE& slot_code, 
													  const MONEY& price ) : 
divide_num_(divide_num), 
from_slot_idx_(from_slot_idx), 
from_postype_(from_postype), 
slot_type_(slot_type), 
slot_code_(slot_code), 
price_(price) 
{

}

HeimTradingvolumeNPCStore::~HeimTradingvolumeNPCStore(void)
{
}

void HeimTradingvolumeNPCStore::Execute()
{
	SCSlotContainer* at_container = ItemManager::Instance()->GetContainer(from_slot_idx_);
	assert(at_container);
	SolarDialog* container_dialog = ItemManager::Instance()->GetItemContainerDialog(from_slot_idx_);
	assert(container_dialog);
	SCItemSlot& ref_slot1 = (SCItemSlot &)at_container->GetSlot(from_postype_);
	ShopDialog* shop_dialog = static_cast<ShopDialog*>(container_dialog);
	DWORD dwShopID = (DWORD)shop_dialog->GetShopID();
	SHOPINFO* shop_info = (SHOPINFO *)ShopInfoParser::Instance()->GetShopList(dwShopID);
	SHOPINFO::SHOP_ITEM* shop_item = shop_info->GetItem(ref_slot1.GetCode());
	if (!shop_item)
	{
		assert(!"ShopInfo.txt È®ÀÎ!!");
		return;
	}

	ItemManager::Instance()->SendBuyMsg(
		shop_dialog ->GetShopID(), 
		ref_slot1.GetCode(), shop_dialog->GetCurrentTab(), shop_dialog->GetRelativeSlotPos(from_postype_),
		divide_num_);
}

MONEY HeimTradingvolumeNPCStore::GetTradingvolume()
{
	return price_;
}

#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
