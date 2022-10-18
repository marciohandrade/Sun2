#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



#pragma once
#ifndef SOLAR_SHOP_PRESENT_RECVBOX_H
#define SOLAR_SHOP_PRESENT_RECVBOX_H
//==================================================================================================
/// PresentRecvBox class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2007. 1. 19
    @remark
        - 받은 선물함
    @note
        - 
    @history 
        - 
*/
//==================================================================================================

#include <list>

class ArchiveOfStream;
class ShopManager;

//==================================================================================================

class PresentRecvBox
{
public:
    class PresentInfo; // internal class
    typedef std::list<PresentRecvBox::PresentInfo*> GIFT_HASH;
    //
    PresentRecvBox();
    virtual ~PresentRecvBox();

    void SetList(ShopManager& mgr IN, DWORD Count, tblRecvGiftItem* pGiftList);
    void Serialize(ArchiveOfStream& ar, eSHOP_SERIALIZE se);
    void Release();
    PresentRecvBox::PresentInfo* FindPresent(GIFTGUID GiftGuid) const;
    BOOL ErasePresent(GIFTGUID GiftGuid);

    GIFT_HASH& GetHash() { return m_PresentList; }
private:
    GIFT_HASH m_PresentList;
};

//==================================================================================================

class PresentRecvBox::PresentInfo
{
    __DECL_CUSTOMPOOL_PTR(PresentInfo)
public:
    enum ePRESENT_STATE
    {
        ePS_WAIT    = 0,    ///> 대기
        ePS_ACCEPT,         ///> 선물 승인
        ePS_REJECT,         ///> 선물 거부
        ePS_DELETE          ///> 선물 삭제
    };
    struct ItemPiece
    {
        WORD    ItemCode;
        BYTE    Amount;
        DWORD   UseTime;
        BYTE    Enchant;
        BYTE    Rank;
    }; // m_ItemPieces[MAX_PACKAGE_ITEM_SIZE];
public:
    PresentInfo();
    ~PresentInfo();

    void Release();
    void Serialize(ArchiveOfStream& ar, eSHOP_SERIALIZE se);

    const char* GetItemName() const { return present_info_data_.item_name_; }
    const char* GetSendCharName() const { return present_info_data_.send_char_name_; }
    const char* GetSendMessage() const { return present_info_data_.send_message_; }

    void SetItemName(const char* p);
    void SetSendCharName(const char* p);
    void SetSendMessage(const char* p);

    // f100906.1L
    const ItemPiece* GetItemPiece(uint8_t piece_index) const;
    // {
    WORD  GetItemCode(BYTE piece_idx) const { return m_ItemPieces[piece_idx].ItemCode; }
    BYTE  GetAmount(BYTE piece_idx) const { return m_ItemPieces[piece_idx].Amount; }
    DWORD GetUseTime(BYTE piece_idx) const { return m_ItemPieces[piece_idx].UseTime; }
    BYTE  GetEnchant(BYTE piece_idx) const { return m_ItemPieces[piece_idx].Enchant; }
    BYTE  GetRank(BYTE piece_idx) const { return m_ItemPieces[piece_idx].Rank; }
    // }
    void    SetPriceGuid(ShopManager& mgr IN, PRICEGUID guid);
    PRICEGUID GetPriceGuid() { return present_info_data_.price_guid_; }
    //
    DWORD GetServerKey() const { return present_info_data_.server_key_; }
    GIFTGUID GetGiftGuid() const { return present_info_data_.gift_guid_; }
    uint8_t GetState() const { return present_info_data_.state_; }
    uint8_t GetPieceCount() const { return present_info_data_.number_of_pieces_; }
    const DWORD64& GetSendDateTime() const { return present_info_data_.send_date_time_; }
    // compatible interface
    void SetState(uint8_t state) { present_info_data_.state_ = state; }
protected:
    struct PresentInfoData {
        DWORD server_key_;  //< 어디에 사용?
        GIFTGUID gift_guid_;    //< GiftItemID
        PRICEGUID price_guid_;  //< PriceGuid
        uint8_t state_; //< 상태
        uint8_t number_of_pieces_;  //< ItemPiece의 개수
        DWORD64 send_date_time_;    //< double  m_dbSendDate;에서 변환 : YYYYMMDDHHMMSS
        char item_name_[MAX_ITEM_NAME_LENGTH + 1];  //< 아이템 이름
        char send_char_name_[MAX_CHARACTER_NAME_LENGTH + 1]; //< 보낸 사용자 캐릭터 이름
        char send_message_[MAX_GIFT_MESSAGE_LENGTH + 1];    //< 보낸 메세지
    } present_info_data_;
public:
    // to compatible access
    ItemPiece m_ItemPieces[MAX_PACKAGE_ITEM_SIZE];
private:
    friend class PresentRecvBox;
};

inline PresentRecvBox::PresentInfo::~PresentInfo() {
}

inline void PresentRecvBox::PresentInfo::Release() {
    ZeroMemory(&present_info_data_, sizeof(present_info_data_));
}

// f100906.1L
inline const PresentRecvBox::PresentInfo::ItemPiece*
PresentRecvBox::PresentInfo::GetItemPiece(uint8_t piece_index) const
{
    if (present_info_data_.number_of_pieces_ > piece_index) {
        return &m_ItemPieces[piece_index];
    }
    return NULL;
}

inline void PresentRecvBox::PresentInfo::SetItemName(const char* p)
{
    strncpy(present_info_data_.item_name_, p, _countof(present_info_data_.item_name_));
    present_info_data_.item_name_[_countof(present_info_data_.item_name_) - 1] = '\0';
}

inline void PresentRecvBox::PresentInfo::SetSendCharName(const char* p)
{
    strncpy(present_info_data_.send_char_name_, p, _countof(present_info_data_.send_char_name_));
    present_info_data_.send_char_name_[_countof(present_info_data_.send_char_name_) - 1] = '\0';
}

inline void PresentRecvBox::PresentInfo::SetSendMessage(const char* p)
{
    strncpy(present_info_data_.send_message_, p, _countof(present_info_data_.send_message_));
    present_info_data_.send_message_[_countof(present_info_data_.send_message_) - 1] = '\0';
}


#endif //SOLAR_SHOP_PRESENT_RECVBOX_H



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
