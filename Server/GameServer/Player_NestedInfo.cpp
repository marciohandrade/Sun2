#include "stdafx.h"
#include "./Player.h"
#include "PlayerRider.h"
#include "./TriggerManagerEx.h"

//==================================================================================================
//  <Player_NestedInfo.hxx>
//  -   Player Internal Function Classes
//
namespace ns_object {
;
//==================================================================================================
//
void
SPECIAL_RENDER_EFFECT::Clear()
{
    if (m_pList) {
        TAllocDelete(RENDER_EFFECT_ITEM_LIST, m_pList);
    };
    m_rFIELD = eSPECIAL_RENDER_EFFECT_NONE;
    m_pList = NULL;
}

void
SPECIAL_RENDER_EFFECT::AddEffectItem(const SLOTCODE effectItemCode, const DWORD update_count)
{
    if (m_pList == NULL) { // copy on write
        m_pList = TAllocNew(RENDER_EFFECT_ITEM_LIST);
    };
    RENDER_EFFECT_ITEM_LIST& rList = *m_pList;
    COUNT_T& rCount = rList[effectItemCode];
    rCount = update_count;
}

void
SPECIAL_RENDER_EFFECT::RemoveEffectItem(const SLOTCODE effectItemCode, const DWORD update_count)
{
    if(m_pList == 0)
        return;

    RENDER_EFFECT_ITEM_LIST& rList = *m_pList;
    RENDER_EFFECT_ITEM_LIST::iterator itFind = rList.find(effectItemCode);
    if(itFind != rList.end())
    {
        if(!update_count)
        {
            rList.erase(itFind);
            if(rList.empty())
            {
                TAllocDelete(RENDER_EFFECT_ITEM_LIST, m_pList);
                m_pList = NULL;
            }
            return;
        }
        COUNT_T& rCount = itFind->second;
        rCount = update_count;
    }
}

DWORD
SPECIAL_RENDER_EFFECT::SerializeRenderInfo(eSPECIAL_RENDER_EFFECT& OUT rOutBitRender,
                                           SLOTCODE* const OUT pOutItemArray, const DWORD upperbound)
{
    rOutBitRender = m_rFIELD;

    if(m_pList == 0)
        return 0;

    DWORD count = 0;
    RENDER_EFFECT_ITEM_LIST& rList = *m_pList;
    FOREACH_CONTAINER(const RENDER_EFFECT_ITEM_NODE& rNode, rList, RENDER_EFFECT_ITEM_LIST)
    {
        if(upperbound <= count)
            return count;   // '<' 조건에 만족하는 것은 critical bug
        SLOTCODE itemCode = rNode.first;
        pOutItemArray[count++] = itemCode;
    }
    return count;
}

//==================================================================================================
// CHANGES: f110103.3L, add zone transaction information to support rollback
PlayerZoneTransaction::PlayerZoneTransaction(Player* player)
    : player_(player)
{
    ZeroMemory(&transaction_data_, sizeof(transaction_data_));
}

void PlayerZoneTransaction::GetData(Data* const transaction_data) const
{
    if (IsTransaction()) {
        *transaction_data = transaction_data_;
    }
    else {
        ZeroMemory(transaction_data, sizeof(*transaction_data));
    }
}

void PlayerZoneTransaction::BeginTransaction(const Data& transaction_data)
{
    assert(IsTransaction() == false);
    transaction_data_ = transaction_data;
}

void PlayerZoneTransaction::EndTransaction()
{
    ZeroMemory(&transaction_data_, sizeof(transaction_data_));
}

void PlayerZoneTransaction::RollbackTransaction()
{
    assert(IsTransaction());
    if (player_ == NULL) {
        return;
    };
    if (transaction_data_.consumed_money) {
        player_->PlusMoney(transaction_data_.consumed_money);
    }
    ZeroMemory(&transaction_data_, sizeof(transaction_data_));
}
//

};  // end namespace ns_object
//
//==================================================================================================

bool IVehicleEvent::OnEvent(Player* player, const IVehicleEvent& event)
{
    bool success_rider_event = player->Rider.OnEvent(event);
    bool success_wings_event = player->Wings.OnEvent(event);

    return (success_rider_event && success_wings_event);
}

//==================================================================================================
// PlayerAttributeLinker
//  NOTE, f100827.4L, this is an event linker
// that is to make connection between a Player and a PlayerAttribute
// after a SCPlayer class split from a Player class.
namespace ns_object {
;

PlayerAttributeLinker::PlayerAttributeLinker(Player* player)
    : player_(player)
    , char_info_(player->GetCharInfo())
{
}

PlayerAttributeLinker::~PlayerAttributeLinker()
{
}

BASE_PLAYERINFO* PlayerAttributeLinker::GetCharacterInfo() const
{
    return char_info_;
}

eCHAR_TYPE PlayerAttributeLinker::GetCharType() const
{
    return static_cast<eCHAR_TYPE>(char_info_->m_byClassCode);
}

LEVELTYPE PlayerAttributeLinker::GetLevel() const
{
    return char_info_->m_LV; 
}

DWORD PlayerAttributeLinker::GetWeaponKind() const
{
    return player_->GetWeaponKind();
}

void PlayerAttributeLinker::SetCharInfo(BASE_PLAYERINFO* char_info)
{
    CopyMemory(char_info_, char_info, sizeof(*char_info_));
    player_->OnSetCharInfo();
}

//==================================================================================================

ChangedValuesNotifier::ChangedValuesNotifier(Player* input_player)
    : player(input_player)
    , max_hp(player->GetMaxHP())
    , max_mp(player->GetMaxMP())
    , max_sd(player->GetMaxSD())
{
}

ChangedValuesNotifier::~ChangedValuesNotifier()
{
    if (max_hp != player->GetMaxHP()) {
        player->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedHP);
    };
    if (max_mp != player->GetMaxMP()) {
        player->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedMP);
    };
    if (max_sd != player->GetMaxSD()) {
        player->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedSD);   
    }
};

}; //end of namespace
//
//==================================================================================================

