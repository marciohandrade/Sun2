#include "stdafx.h"

#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



#include ".\presentrecvbox.h"
#include <oleauto.h>
#include <SolarDateTime.h>
#include <ArchiveOfStream.h>
#include <functional_custom.h>
#include <algorithm>
#include "ShopManager.h"
#include "ShopManager.h"

__IMPL_CUSTOMPOOL_PTR(PresentRecvBox::PresentInfo);

PresentRecvBox::PresentInfo::PresentInfo()
{
    ZeroMemory(&present_info_data_, sizeof(present_info_data_));

    //STATIC_ASSERT(eGS_ACCEPT == 4);
    BOOST_STATIC_ASSERT(eGS_ACCEPT == ePS_ACCEPT &&
                        eGS_REJECT == ePS_REJECT &&
                        eGS_DELETE == ePS_DELETE);
}

void PresentRecvBox::PresentInfo::SetPriceGuid(ShopManager& mgr IN, PRICEGUID guid)
{
    present_info_data_.price_guid_ = guid;
    present_info_data_.number_of_pieces_ = 0;
    ShopItemPrice* const shop_item_price = mgr.FindShopItemPrice(present_info_data_.price_guid_);
    if (shop_item_price == NULL) {
        return;
    }

    ShopItem* shop_item = shop_item_price->GetShopItem();
    if (shop_item->GetType() == ShopItem::SINGLE)
    {
        // 패키지냐 아이템이냐 개수가 달라짐
        SingleItem* single_item = static_cast<SingleItem*>(shop_item);
        uint8_t amount = shop_item_price->GetAmount();
        ItemPiece* const target_piece = &m_ItemPieces[present_info_data_.number_of_pieces_];
        target_piece->ItemCode = single_item->GetItemCode();
        target_piece->Amount   = amount == 0 ? 1 : amount;
        target_piece->UseTime  = shop_item_price->GetUseTime();
        target_piece->Enchant  = single_item->GetEnchant();
        target_piece->Rank     = single_item->GetRank();
        ++present_info_data_.number_of_pieces_;
    }
    else
    {
        PackageItem* package_item = static_cast<PackageItem*>(shop_item);
        const uint8_t number_of_pieces = package_item->GetPieceCount();
        for (BYTE piece_idx = 0; piece_idx < number_of_pieces; ++piece_idx)
        {
            const PackageItem::ItemPiece* piece = package_item->GetItemPiece(piece_idx);
            ItemPiece* const target_piece = &m_ItemPieces[piece_idx];
            target_piece->ItemCode = piece->ItemCode;
            target_piece->Amount   = piece->Amount == 0 ? 1 : piece->Amount;
            target_piece->UseTime  = piece->UseTime;
            target_piece->Enchant  = piece->Enchant;
            target_piece->Rank     = piece->Rank;
        }
        present_info_data_.number_of_pieces_ = number_of_pieces;
    }
}

void PresentRecvBox::PresentInfo::Serialize(ArchiveOfStream& ar, eSHOP_SERIALIZE se)
{
    switch(se)
    {
    case eSHOP_SERIALIZE_SAVE:
        {
            Release();
            //ar >> present_info_data_.server_key_;
            ar >> present_info_data_.gift_guid_;
            //ar >> m_ItemID;
            ar >> present_info_data_.price_guid_;
            ar >> present_info_data_.state_;
            ar >> present_info_data_.send_date_time_;

            ar >> present_info_data_.number_of_pieces_;
            for (BYTE i = 0 ; i < present_info_data_.number_of_pieces_ ; ++i)
            {
                ItemPiece* target_piece = &m_ItemPieces[i];
                ar >> target_piece->ItemCode;
                ar >> target_piece->Amount;
                ar >> target_piece->UseTime;
                ar >> target_piece->Enchant;
                ar >> target_piece->Rank;
            }

            BYTE len = 0;
            ar >> len;
            ar.Read(present_info_data_.item_name_, len);
            present_info_data_.item_name_[len]='\0';
            ar >> len;
            ar.Read(present_info_data_.send_char_name_, len);
            present_info_data_.send_char_name_[len]='\0';
            ar >> len;
            ar.Read(present_info_data_.send_message_, len);
            present_info_data_.send_message_[len]='\0';

        }break;
    case eSHOP_SERIALIZE_LOAD:
        {   

            //ar << m_ServerKey;
            ar << present_info_data_.gift_guid_;
            //ar << m_ItemID;
            ar << present_info_data_.price_guid_;

            ar << present_info_data_.state_;
            ar << present_info_data_.send_date_time_;

            ar << present_info_data_.number_of_pieces_;
            for (BYTE i = 0 ; i < present_info_data_.number_of_pieces_; ++i)
            {
                const ItemPiece& piece = m_ItemPieces[i];
                ar << piece.ItemCode;
                ar << piece.Amount;
                ar << piece.UseTime;
                ar << piece.Enchant;
                ar << piece.Rank;
            }

            BYTE len = 0;
            len = (BYTE)strlen(present_info_data_.item_name_);
            ar << len;
            ar.Write(present_info_data_.item_name_, len);
            len = (BYTE)strlen(present_info_data_.send_char_name_);
            ar << len;
            ar.Write(present_info_data_.send_char_name_, len);
            len = (BYTE)strlen(present_info_data_.send_message_);
            ar << len;
            ar.Write(present_info_data_.send_message_, len);

        }break;
    default:
        ASSERT(NULL);
        break;
    }
}


//==================================================================================================
//===================================== PresentRecvBox =============================================
//==================================================================================================


PresentRecvBox::PresentRecvBox()
{
}

PresentRecvBox::~PresentRecvBox()
{
}
void PresentRecvBox::Release()
{
    FOREACH_CONTAINER(const GIFT_HASH::value_type& node, m_PresentList, GIFT_HASH)
    {
        PresentInfo* present_info = node;
        present_info->Release();
        PresentInfo::FREE(present_info);
    }
    /*std::for_each(m_PresentList.begin(), m_PresentList.end(), 
    compose1(bind1st(mem_fun<void,PresentRecvBox,PresentRecvBox::PresentInfo*>(&PresentRecvBox::PresentInfo::Test), 
    select2nd<GIFT_HASH::value_type>()), 
    select2nd<GIFT_HASH::value_type>()));*/
    m_PresentList.clear();
}

PresentRecvBox::PresentInfo* 
PresentRecvBox::FindPresent(GIFTGUID GiftGuid) const
{
    //GIFT_HASH::iterator it = m_PresentList.find(GiftGuid);
    //if (it != m_PresentList.end())
    //  return *it;
    //return NULL;
    FOREACH_CONTAINER(const GIFT_HASH::value_type& node, m_PresentList, GIFT_HASH)
    {
        PresentInfo* present_info = node;
        if (present_info->GetGiftGuid() == GiftGuid) {
            return present_info;
        }
    }
    return NULL;
}

BOOL PresentRecvBox::ErasePresent(GIFTGUID GiftGuid)
{
    /*GIFT_HASH::iterator it = m_PresentList.find(GiftGuid);
    if (it == m_PresentList.end())
    return false;
    m_PresentList.erase(it);
    return true;*/
    GIFT_HASH::iterator it = m_PresentList.begin(),
                        end = m_PresentList.end();
    for (; it != end ; ++it)
    {
        PresentInfo* present_info = *it;
        if (present_info->GetGiftGuid() == GiftGuid) {
            m_PresentList.erase(it);
            return true;
        }
    }
    return false;
}

void PresentRecvBox::SetList(ShopManager& mgr IN, DWORD Count, tblRecvGiftItem* pGiftList)
{
    Release();

    SYSTEMTIME systime;
    for (DWORD i = 0 ; i < Count ; ++i)
    {
        ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, pGiftList[i].dwState, BYTE));
        const tblRecvGiftItem& gift_list_node = pGiftList[i];
        //
        PresentInfo* new_present_info =  PresentInfo::ALLOC();
        new_present_info->Release();
        PresentInfo::PresentInfoData* const dest_data = &new_present_info->present_info_data_;
        //new_present_info->SetServerKey(pGiftList[i].);
        //new_present_info->SetDateTime(0);
        dest_data->gift_guid_ = gift_list_node.dwGiftItemID;
        //new_present_info->SetItemID(pGiftList[i].dwBuyItemID);
        new_present_info->SetPriceGuid(mgr, gift_list_node.dwPriceGuid);
        dest_data->state_ = static_cast<uint8_t>(gift_list_node.dwState);
        ::VariantTimeToSystemTime(gift_list_node.dbSendDate, &systime);
        util::GetDateTime_YYYYMMDDHHMMSS(systime.wYear, systime.wMonth, systime.wDay,
                                         systime.wHour, systime.wMinute, systime.wSecond,
                                         dest_data->send_date_time_);

        //new_present_info->SetCharName(pGiftList[i].);
        new_present_info->SetItemName(pGiftList[i].szItemName);
        new_present_info->SetSendCharName(pGiftList[i].szSendCharName);
        new_present_info->SetSendMessage(pGiftList[i].szSendMessage);
        //m_PresentList.insert(GIFT_HASH::value_type(new_present_info->GetGiftGuid(), new_present_info));
        m_PresentList.push_back(new_present_info);
    }
}


void PresentRecvBox::Serialize(ArchiveOfStream& ar, eSHOP_SERIALIZE se)
{
    switch (se)
    {
    case eSHOP_SERIALIZE_SAVE:
        {
            Release();

            DWORD ListCount=0;
            ar >> ListCount;
            for (BYTE x = 0 ; x < ListCount ; ++x)
            {
                PresentInfo* pNewPresentInfo = PresentInfo::ALLOC();
                pNewPresentInfo->Serialize(ar, se);
                m_PresentList.push_back(pNewPresentInfo);
            }
        }
        break;
    case eSHOP_SERIALIZE_LOAD:
        {   
            ar << (DWORD)m_PresentList.size();
            GIFT_HASH::iterator it = m_PresentList.begin(), end = m_PresentList.end();
            for (; it != end  ; ++it)
            {
                PresentInfo* present_info = *it;
                present_info->Serialize(ar, se);
            }
        }
        break;
    default:
        ASSERT(NULL);
        break;
    }
}



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
