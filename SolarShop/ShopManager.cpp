
#include "stdafx.h"

#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



#include ".\shopmanager.h"
#include <algorithm>
#include <ArchiveOfStream.h>
#include "../SolarSlot/SCSlotHeader.h"

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
#include "./ShopPartition.h"
#include "./ShopCategory.h"
#endif
//---------------------------------------------------------------------------------------
// functor
#include <functional>

struct wholeAdder : public std::unary_function<ShopItemPrice*,void>
{
	ShopManager& m_mgr;
	wholeAdder(ShopManager& mgr) :m_mgr(mgr) {}
	void operator()(ShopItemPrice* price)
	{
		m_mgr.AddShopItemPrice(price);
	}
};


ShopManager::ShopManager()
:	m_Transaction(false)
,	m_pDefaultShopUserPool(NULL)
{
	const DWORD size = 10;
	
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    shop_partition_[SHOP_PARTITION_SINGLEITEM] = new SingleItemPartition();
    shop_partition_[SHOP_PARTITION_PACKAGEITEM] = new PackageItemPartition();
    shop_partition_[SHOP_PARTITION_MILEAGEITEM] = new MileageItemPartition();
#else
    m_ItemCategoryPool.Initialize(size);
    m_PackageCategoryPool.Initialize(size);
    ItemCategory::SetPool(&m_ItemCategoryPool);
    PackageCategory::SetPool(&m_PackageCategoryPool);
#endif
	m_SingleItemPool.Initialize(50, 50);
	m_PackageItemPool.Initialize(50, 50);
	m_ShopItemPricePool.Initialize(50, 50);
	m_PresentInfoPool.Initialize(50, 50);

	SingleItem::SetPool(&m_SingleItemPool);
	PackageItem::SetPool(&m_PackageItemPool);
	ShopItemPrice::SetPool(&m_ShopItemPricePool);
	PresentRecvBox::PresentInfo::SetPool(&m_PresentInfoPool);
}

ShopManager::~ShopManager()
{
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    for (uint idx = 0 ; idx < SHOP_PARTITION_MAXCOUNT ; ++idx)
        delete shop_partition_[idx];
#endif

	SAFE_RELEASENDELETE(m_pDefaultShopUserPool);
}

void ShopManager::Initialize_DefaultUserPool(DWORD size)
{
	ASSERT(!m_pDefaultShopUserPool);
	SAFE_RELEASENDELETE(m_pDefaultShopUserPool);
	m_pDefaultShopUserPool = new util::CMemoryPoolFactory<ShopUser>;
	m_pDefaultShopUserPool->Initialize(size);
}

void ShopManager::Init()
{
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    for (uint idx = 0 ; idx < SHOP_PARTITION_MAXCOUNT ; ++idx)
    {
        shop_partition_[idx]->Initialize();
        shop_partition_[idx]->BuildCategory();
    }
#else // _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
	
    //카테고리 생성
	for (int sep  = 0 ; sep < SHOP_SEPERATE_MAX ; ++sep)
	{
		ASSERT(m_CategoryHash[sep].size() == 0);
		for (int i = 0 ; i < CT_MAX ; ++i)
		{
			switch(sep)
			{
			case SHOP_ITEM: 
				{
					if (i==CT_PACKAGE) continue; 
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
					if (i==CT_MILEAGE) continue; 
#endif
				}
				break;
			case SHOP_PACKAGE:
				{
					if (i==CT_WASTEITEM||i==CT_FUNCTIONAL||i==CT_ETC) continue; 
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
					if (i==CT_MILEAGE) continue; 
#endif
				}
				break;
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
			case SHOP_MILEAGE: 
				{
					if (i==CT_WASTEITEM||i==CT_FUNCTIONAL||i==CT_ETC) continue; 
					if (i==CT_PACKAGE) continue; 
				}
				break;
#endif
			}
			ShopCategory* pCategory = CreateShopCategory((eSHOP_CATEGORY_SEPERATE)sep);
			pCategory->SetCategory((eSHOP_CATEGORY)i);
			AddShopCategory((const eSHOP_CATEGORY_SEPERATE)sep, pCategory);
		}
	}
#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
}

void ShopManager::Release()
{
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    for (uint idx = 0 ; idx < SHOP_PARTITION_MAXCOUNT ; ++idx)
        shop_partition_[idx]->Release();
#else
	for (int sep  = 0 ; sep < SHOP_SEPERATE_MAX ; ++sep)
	{
		std::for_each(m_CategoryHash[sep].begin(), m_CategoryHash[sep].end(), 
			compose1(std::bind1st(std::mem_fun(&ShopManager::DestroyShopCategory), this), select2nd<CT_HASH::value_type>()));
		m_CategoryHash[sep].clear();
	}
#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
	Clear();
}

void ShopManager::Clear()
{
#ifndef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
	for (int sep  = 0 ; sep < SHOP_SEPERATE_MAX ; ++sep)
	{
		std::for_each(m_CategoryHash[sep].begin(), m_CategoryHash[sep].end(), 
			compose1(std::mem_fun(&ShopCategory::Release), select2nd<CT_HASH::value_type>()));
	}
#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

	std::for_each(m_ShopUserHash.begin(), m_ShopUserHash.end(), 
		compose1(std::bind1st(std::mem_fun(&ShopManager::DestroyShopUser), this), select2nd<SHOPUSER_HASH::value_type>()));
	m_ShopUserHash.clear();

	std::for_each(m_ShopItemHash.begin(), m_ShopItemHash.end(), 
		compose1(std::bind1st(std::mem_fun(&ShopManager::DestroyShopItem), this), select2nd<SHOPITEM_HASH::value_type>()));
	m_ShopItemHash.clear();

	std::for_each(m_ShopItemPriceHash.begin(), m_ShopItemPriceHash.end(), 
		compose1(std::bind1st(std::mem_fun(&ShopManager::DestroyShopItemPrice), this), select2nd<SHOPITEMPRICE_HASH::value_type>()));
	m_ShopItemPriceHash.clear();

	SetTransaction(false);
}
ShopUser* ShopManager::CreateShopUser()
{
	if (!m_pDefaultShopUserPool) return NULL;
	return (ShopUser *)m_pDefaultShopUserPool->Alloc();
}
void ShopManager::DestroyShopUser(ShopUser* pUser)
{
	ASSERT(m_pDefaultShopUserPool);
	if (m_pDefaultShopUserPool)
	{
		pUser->Release();
		m_pDefaultShopUserPool->Free(pUser);
	}
}


#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
ShopItem* ShopManager::CreateShopItem(eSHOP_PARTITION partition_index)
{
    ShopItem* pShopItem = NULL;
    switch(partition_index)
    {
    case SHOP_PARTITION_PACKAGEITEM:
        pShopItem = PackageItem::ALLOC();
        break;
    default:
        pShopItem = SingleItem::ALLOC();
        break;
    }
    return pShopItem;
}
#else
ShopItem* ShopManager::CreateShopItem(eSHOP_CATEGORY_SEPERATE sep)
{
	ShopItem* pShopItem = NULL;
	switch(sep)
	{
	case SHOP_PACKAGE:
		pShopItem = PackageItem::ALLOC();
		break;
	default:
		pShopItem = SingleItem::ALLOC();
		break;
	}
	return pShopItem;
}
#endif

void ShopManager::DestroyShopItem(ShopItem* p)
{
    p->Release();
	switch(p->GetType())
	{
	case ShopItem::PACKAGE:
		PackageItem::FREE((PackageItem*)p);	break;
	default:
		SingleItem::FREE((SingleItem*)p);		break;
	}
}

#ifndef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
ShopCategory* ShopManager::CreateShopCategory(eSHOP_CATEGORY_SEPERATE sep)
{
	switch(sep)
	{
	case SHOP_PACKAGE:
		return PackageCategory::ALLOC();
	default:
		return ItemCategory::ALLOC();
	}
	//return NULL;
    assert(!"impossible reached point");
}

void ShopManager::DestroyShopCategory(ShopCategory* p)
{
	eSHOP_CATEGORY_SEPERATE sep = p->GetSeperateType();
	p->Release();
	switch(sep)
	{
	case SHOP_PACKAGE:
		PackageCategory::FREE((PackageCategory*)p); break;
	default:
		ItemCategory::FREE((ItemCategory*)p);	break;
	}
}
#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

ShopItemPrice* ShopManager::CreateShopItemPrice()
{
	return ShopItemPrice::ALLOC();
}
void ShopManager::DestroyShopItemPrice(ShopItemPrice* p)
{
	p->Clear();
	ShopItemPrice::FREE(p);
}

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
RC::eSHOP_RESULT ShopManager::Serialize(eSHOP_PARTITION partition_index, eSHOP_CATEGORY category_index, BYTE* stream, uint size)
{
    ArchiveOfStream archive(stream, size);

    ShopCategory* category = FindCategory(partition_index, category_index);
    ASSERT(category);
    if (category == NULL)
        return RC::RC_SHOP_FAILED;

    BYTE count = 0;
    ITEMGUID guid = 0;
    archive >> count;
    for (BYTE idx = 0 ; idx < count ; ++idx)
    {
        archive >> guid;
        ShopItem* pShopItem = FindShopItem(guid);
        if (!pShopItem)
        {
            pShopItem = CreateShopItem(partition_index);
            pShopItem->Release();
            ShopItem::ShopItemData* shop_item_data = pShopItem->GetEditableShopItemData();
            shop_item_data->item_guid_ = guid;
            shop_item_data->category_  = static_cast<uint8_t>(category_index);
            shop_item_data->partition_index_ = static_cast<uint8_t>(partition_index);
            AddShopItem(pShopItem);
        }

        category->InsertShopItem(pShopItem);

        pShopItem->Serialize(*this, archive, eSHOP_SERIALIZE_ITEM_LIST_SAVE);
    }

    return RC::RC_SHOP_SUCCESS;
}

#else

RC::eSHOP_RESULT ShopManager::Serialize(BYTE Sep/*eSHOP_CATEGORY_SEPERATE*/, BYTE Category, //< eSHOP_CATEGORY
	BYTE Page, BYTE* pStream INOUT, WORD& Size INOUT, eSHOP_SERIALIZE se)
{
	if (Size == 0) return RC::RC_SHOP_SUCCESS;

	switch(se)
	{
	case eSHOP_SERIALIZE_ITEM_LIST_SAVE:
		{
			ArchiveOfStream lArchive(pStream, Size);
			// 카테고리 반드시 존재
			ShopCategory* pCategory = FindShopCategory((const eSHOP_CATEGORY_SEPERATE)Sep, 
														(const eSHOP_CATEGORY)Category);
			ASSERT(pCategory); if (!pCategory) return RC::RC_SHOP_FAILED;
			pCategory->Serialize(*this, Page, lArchive, se);

		}break;
	case eSHOP_SERIALIZE_ITEM_LIST_LOAD:
		{
			ArchiveOfStream lArchive(pStream, Size);
			ShopCategory* pCategory = FindShopCategory((const eSHOP_CATEGORY_SEPERATE)Sep, (const eSHOP_CATEGORY)Category);
			if (pCategory)
			{
				pCategory->Serialize(*this, Page, lArchive, se);
			}
            int archive_size = lArchive.GetSize();
            ASSERT(archive_size <= USHRT_MAX);
			Size = static_cast<WORD>(archive_size);
		}break;
	}

	return RC::RC_SHOP_SUCCESS;
}

#endif

RC::eSHOP_RESULT ShopManager::SerializeItemDetail(ITEMGUID Guid, BYTE* pStream INOUT, WORD& Size INOUT, eSHOP_SERIALIZE se)
{
	ArchiveOfStream lArchive(pStream, Size);

	switch(se)
	{
	case eSHOP_SERIALIZE_ITEM_SAVE:
		{
			ShopItem* pShopItem = FindShopItem((const ITEMGUID)Guid);
			if (pShopItem)
			{
				pShopItem->Serialize(*this, lArchive, se);
			}
		}break;
	case eSHOP_SERIALIZE_ITEM_LOAD:
		{
			ShopItem* pShopItem = FindShopItem((const ITEMGUID)Guid);
			if (pShopItem)
			{
				pShopItem->Serialize(*this, lArchive, se);
			}
            int archive_size = lArchive.GetSize();
            ASSERT(archive_size <= USHRT_MAX);
            Size = static_cast<WORD>(archive_size);
		}break;
	}
	return RC::RC_SHOP_SUCCESS;
}

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
void ShopManager::BuildShopItem(DWORD Count, tblSellItem* pItem IN)
{
    __UNUSED((Count, pItem));
}

void ShopManager::BuildPackageItem(DWORD Count, tblSellPackage* pPackage IN)
{
    __UNUSED((Count, pPackage));
}

void ShopManager::BuildTree(DWORD Count, tblItemBranch* pBranch IN)
{
    __UNUSED((Count, pBranch));
}

void ShopManager::BuildShopItem(const tblSellItemContainer& container)
{
    ShopCategory* category = FindCategory((eSHOP_PARTITION)(container.partition_index)
                                          ,(eSHOP_CATEGORY)(container.category_index));
    ASSERT(category);
    if (category == NULL)
        return;

    ShopItem* pShopItem = CreateShopItem((eSHOP_PARTITION)(container.partition_index));
    pShopItem->Release();
    pShopItem->Set(container, *this);

    if (pShopItem->GetState() == 1)
    {
        category->InsertShopItem(pShopItem);
    }

    // 전체 리스트에서 추가
    wholeAdder op(*this);
    pShopItem->for_each(op);
    bool b = AddShopItem(pShopItem);
    ASSERT(b && "ShopItem Add Failed");
}

#else

//---------------------------------------------------------------------------------------------------------------------------------
//Protocol.ShopServer.h의 패킷 구조는 엉망이다.
//정말 이렇게 작업하기가 싫고 힘들기도 하다..ㅠ_ㅠ(제발이런식으로설계하지말자.엉?엉?.오xx한테말할수도없고.-_-)
void ShopManager::BuildShopItem(DWORD Count, tblSellItem* pItem IN)
{
	for (DWORD i = 0 ; i < Count ; ++i)
	{
		eSHOP_CATEGORY_SEPERATE sep = SHOP_ITEM;
		// 가정 : 패키지 이면서 아이템용은 없다.
		switch(pItem[i].dwCategoryID)
		{
		case CT_PACKAGE: //< 패키지용
			sep = SHOP_PACKAGE;
			ASSERT("Invalid Item Category");
			continue;
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
		case CT_MILEAGE:
			{
				sep = SHOP_MILEAGE;
			}break;
#endif

		case CT_TEMP:
			ASSERT("[ShopManager::BuildShopItem] Invalid Item Category");
			continue;
		}

		ShopCategory* pAllCategory = FindShopCategory(sep, CT_ALLITEM);
		ASSERT(pAllCategory);
        SingleItem* pShopItem = static_cast<SingleItem*>(CreateShopItem(sep));
        pShopItem->Release();
        pShopItem->Set(pItem[i], *this);

		ShopCategory* pCategory = FindShopCategory(sep, (const eSHOP_CATEGORY)pShopItem->GetCategory());
		if (!pCategory) 
		{
			DestroyShopItem(pShopItem);
			continue;
		}

		if (pShopItem->GetState() == 1)
		{
            eSHOP_CATEGORY shop_category = static_cast<eSHOP_CATEGORY>(pShopItem->GetCategory());
            if (shop_category == CT_WEAPON) {
                pShopItem->SetRank(KindOfItemType::Shell);
            }
#ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
            // (CHANGES) (WAVERIX) |KEY=#100224.1|Global version has requested below changes \
            // so that changes to support pp cash item filtered in 'show all items'
            if (shop_category != CT_PERIOD_EXTEND &&
                shop_category != CT_PCROOM &&
                shop_category != CT_PPCASH)
            {
                pAllCategory->AddShopItem(pShopItem);
            }
#else //else if _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
			if (shop_category != CT_PERIOD_EXTEND && shop_category != CT_PCROOM)
			{
				pAllCategory->AddShopItem(pShopItem);
			}
#endif

			pCategory->AddShopItem(pShopItem);
		}

		// 전체 리스트에서 추가
		wholeAdder op(*this);
		pShopItem->for_each(op);
		bool b = AddShopItem(pShopItem);
		ASSERT(b && "ShopItem Add Failed");

#ifdef _DEBUG
//		pShopItem->Display();
#endif
	}
}

// 패키지 껍데기 정보
void ShopManager::BuildPackageItem(DWORD Count, tblSellPackage* pPackage IN)
{
	eSHOP_CATEGORY_SEPERATE sep = SHOP_PACKAGE;
	ShopCategory* pAllCategory = FindShopCategory(sep, CT_ALLITEM);
	for (DWORD i = 0 ; i < Count ; ++i)
	{
		ASSERT(pAllCategory);
        PackageItem* pShopItem = static_cast<PackageItem*>(CreateShopItem(sep));
        pShopItem->Release();
        pShopItem->Set(pPackage[i], *this);

		if (pShopItem->GetCategory() != CT_PACKAGE)
		{
			printf("PackageItem category(%d) info Error\n", pShopItem->GetCategory());
		}

		ShopCategory* pCategory = FindShopCategory(sep, (const eSHOP_CATEGORY)pShopItem->GetCategory());
		if (!pCategory)//|| pShopItem->GetCategory() == CT_ALLITEM) 
		{
			//SUNLOG(eCRITICAL_LOG, "PackageItem category info registration failed");
			printf("PackageItem seperate(%d), category (%d) info Registration Error\n", sep, pShopItem->GetCategory());
			DestroyShopItem(pShopItem);
			continue;
		}

		if (pShopItem->GetState() == 1)
		{
			pAllCategory->AddShopItem(pShopItem);
			pCategory->AddShopItem(pShopItem);
		}
		bool b = AddShopItem(pShopItem);
		ASSERT(b && "ShopPackageItem Add Failed");
#ifdef _DEBUG
//		pShopItem->Display();
#endif

	}
}
void ShopManager::BuildTree(DWORD Count, tblItemBranch* pBranch IN)
{
	for (DWORD i = 0 ; i < Count ; ++i)
	{
		if (pBranch[i].dwBranchID == CT_TEMP)
		{
			ASSERT("[ShopManager::BuildTree] Invalid Item Category");
			continue;
		}

		eSHOP_CATEGORY_SEPERATE sep = SHOP_PACKAGE;
		if (1 == pBranch[i].dwItemType) //< 아이템
		{
			sep = SHOP_ITEM;
		}
		else if (2 == pBranch[i].dwItemType)
		{
			sep = SHOP_PACKAGE;
		}

		ShopItem* pShopItem = FindShopItem(pBranch[i].dwGuid);
		if (!pShopItem) continue;
		ShopCategory* pCategory = FindShopCategory(sep, (const eSHOP_CATEGORY)pBranch[i].dwBranchID);
		if (!pCategory)
		{
			pCategory = CreateShopCategory(sep);
			pCategory->SetCategory((eSHOP_CATEGORY)pBranch[i].dwBranchID);
			AddShopCategory(sep, pCategory);
		}
		if (pShopItem->GetState() == 1)
		{
			pCategory->AddShopItem(pShopItem);
		}
	}
}

#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

//---------------------------------------------------------------------------------------------------------------------------------
void ShopManager::Display()
{
	std::cout << "--------------------------------Shop Info--------------------------------" << std::endl;
	for (int sep  = 0 ; sep < SHOP_SEPERATE_MAX ; ++sep)
	{
		switch(sep)
		{
		case SHOP_PACKAGE:
			{
				std::cout << "Package List" << std::endl;
			}break;
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
		case SHOP_MILEAGE:
			{
				std::cout << "Mileage List" << std::endl;
			}break;
#endif
		}

#ifndef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
		std::for_each(m_CategoryHash[sep].begin(), m_CategoryHash[sep].end(), 
			compose1(std::mem_fun(&ShopCategory::Display), select2nd<CT_HASH::value_type>()));
#endif
	}
	std::cout << "--------------------------------Shop Info--------------------------------" << std::endl;
}

RC::eSHOP_RESULT ShopManager::CanRequestBuy(USERGUID UserGuid)
{
	ShopUser* pUser = FindShopUser(UserGuid);
	if (!pUser)
	{
		return RC::RC_SHOP_USERISNOTFOUND;
	}

	if (pUser->IsTransaction())
	{
		return RC::RC_SHOP_ALREADY_BUYING_TRANSACTION;
	}

	return RC::RC_SHOP_SUCCESS;
}

RC::eSHOP_RESULT ShopManager::DoRequestBuy(USERGUID UserGuid)
{
	ShopUser* pUser = FindShopUser(UserGuid);
	if (!pUser)
	{
		return RC::RC_SHOP_USERISNOTFOUND;
	}

	pUser->SetTransaction(true);

	return RC::RC_SHOP_SUCCESS;
}

RC::eSHOP_RESULT
ShopManager::OnResultBuy(USERGUID user_guid, const CashData& cash_data)
{
	ShopUser* pUser = FindShopUser(user_guid);
	if (!pUser)
	{
		ASSERT(pUser);
		pUser = CreateShopUser();
		pUser->Release();
		pUser->SetUserGuid(user_guid);

		AddShopUser(pUser);
	}

	pUser->SetTransaction(false);
    pUser->SetCashData(cash_data);

	return RC::RC_SHOP_SUCCESS;
}


RC::eSHOP_RESULT ShopManager::DoUserCash(USERGUID UserGuid)
{
    __UNUSED(UserGuid);
	// Shop서버로 보냄
	return RC::RC_SHOP_PENDING;
}

//_NA00XXXX_090111_CASH_TYPE_SHARED_
RC::eSHOP_RESULT
ShopManager::OnResultUserCash(USERGUID user_guid, const CashData& cash_data)
{
	// Shop서버로부터 받음
	ShopUser* pUser = FindShopUser(user_guid);
	if (!pUser)
	{
		pUser = CreateShopUser();
		pUser->Release();
		pUser->SetUserGuid(user_guid);
		
		AddShopUser(pUser);
	}

    pUser->SetCashData(cash_data);
    return RC::RC_SHOP_SUCCESS;
}

BOOLEAN
ShopManager::HaveCash(USERGUID user_guid, CASH cash)
{
    ShopUser* shop_user = FindShopUser(user_guid);
    if (shop_user == NULL) {
        return false;
    }

    const CashData& cash_data = shop_user->GetCashData();
    return cash_data.cash_ >= cash;
}

BOOLEAN
ShopManager::HaveMileage(USERGUID user_guid, DWORD mileage)
{
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
    ShopUser* shop_user = FindShopUser(user_guid);
    if (shop_user == NULL) {
        return false;
    }

    const CashData& cash_data = shop_user->GetCashData();
    return cash_data.mileage_ >= mileage;
#else
    __UNUSED((user_guid, mileage));
    return false;
#endif
}

//_NA00XXXX_090111_CASH_TYPE_SHARED_
BOOLEAN
ShopManager::HavePPCash(USERGUID user_guid, CASH pp_cash)
{
    ShopUser* shop_user = FindShopUser(user_guid);
    if (shop_user == NULL) {
        return false;
    }

    const CashData& cash_data = shop_user->GetCashData();
    return cash_data.pp_cash_ >= pp_cash;
}

void ShopManager::OnRemoveShopUser(USERGUID UserGuid)
{
	SHOPUSER_HASH::iterator it = m_ShopUserHash.find(UserGuid);
	if (it != m_ShopUserHash.end())
	{
		ShopUser* pUser = it->second;
		m_ShopUserHash.erase(it);
		DestroyShopUser(pUser);
	}
}


#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

ShopCategory* ShopManager::FindCategory(eSHOP_PARTITION partition_index, eSHOP_CATEGORY category_index)
{
    ASSERT(partition_index < SHOP_PARTITION_MAXCOUNT);
    if (partition_index < SHOP_PARTITION_MAXCOUNT)
        return shop_partition_[partition_index]->FindCategory(category_index);
    return NULL;
}

void ShopManager::Update()
{
    static DWORD LastTick = GetTickCount();
    if (LastTick + 1000 > GetTickCount()) return;
    LastTick = GetTickCount();

    if (IsTransaction()) return;

    SHOPITEM_HASH::iterator it = m_ShopItemHash.begin(), end = m_ShopItemHash.end();
    for (; it != end ;)
    {
        ShopItem* pShopItem = (it->second);
        if ((pShopItem->GetState() == 1) && pShopItem->IsSellEndDate())	//< 판매중이고
        {
            ShopCategory* category = FindCategory((eSHOP_PARTITION)pShopItem->GetPartition_Index(), (eSHOP_CATEGORY)pShopItem->GetCategory());
            if (category == NULL)
                continue;

            // 카테고리에서도 뺌
            std::cout << "==================Item Sell End==================" << std::endl;
            pShopItem->Display();
            std::cout << "==================Item Sell End==================" << std::endl;

            category->RemoveShopItem(pShopItem);
            m_ShopItemHash.erase(it++);
            DestroyShopItem(pShopItem);
        }
        else
            ++it;
    }
}




#else //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
void ShopManager::Update()
{
	static DWORD LastTick = GetTickCount();
	if (LastTick + 1000 > GetTickCount()) return;
	LastTick = GetTickCount();

	if (IsTransaction()) return;

	SHOPITEM_HASH::iterator it = m_ShopItemHash.begin(), end = m_ShopItemHash.end();
	for (; it != end ;)
	{
		ShopItem* pShopItem = (it->second);
		if ((pShopItem->GetState() == 1)		//< 판매중이고
			&& pShopItem->IsSellEndDate())
		{
			eSHOP_CATEGORY_SEPERATE sep = SHOP_ITEM;
			switch(pShopItem->GetCategory())
			{
			case CT_PACKAGE: sep = SHOP_PACKAGE; break;
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
			case CT_MILEAGE: sep = SHOP_MILEAGE; break;
#endif
			}
			// 카테고리에서도 뺌
			ShopCategory* pAllCategory = FindShopCategory(sep, CT_ALLITEM);
			ShopCategory* pRealCategory = FindShopCategory(sep, (eSHOP_CATEGORY)(pShopItem->GetCategory()));
			if (pAllCategory && pRealCategory)
			{
				std::cout << "==================Item Sell End==================" << std::endl;
				pShopItem->Display();
				std::cout << "==================Item Sell End==================" << std::endl;

				pAllCategory->RemoveShopItem(pShopItem);
				pRealCategory->RemoveShopItem(pShopItem);

				m_ShopItemHash.erase(it++);
				DestroyShopItem(pShopItem);
			}
		}
		else
			++it;
	}
}

#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
