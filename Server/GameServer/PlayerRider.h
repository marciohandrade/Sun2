#ifndef _GAMESERVER_PLAYER_RIDER_H
#define _GAMESERVER_PLAYER_RIDER_H
#pragma once

#include <StructInPacket.h>
#include <RidingInfoParser.h>

class SCItemSlot;
class Player;


//==================================================================================================

class PlayerRider
{
    // IPlayerRiderD real data block
    struct Container : public IPlayerRiderD::ContainerBase {
        RiderControl state;
        DWORD casting_start; // TickCount
        RiderControl enter_state; // move field synchronization (Field<->Battle)
        enum {
            eTrans_None             = 0,
            eTrans_SerializeStart   , //set by SyncPrepareRidingStatus
            eTrans_SyncRequested    , //set by SyncNotifyRiderSummon
        };
        BYTE transaction_mode;        //enter sync transaction
        KEYTYPE transaction_zone_key; //enter sync transaction
    };
public:
    enum eResult {
        eResult_Fail    = 0,// fail     Server -> Client(requester)
        eResult_1to1    ,   // success  Server -> Client(requester)
        eResult_1toN    ,   // success  Server -> Client(broadcast)
    };
    PlayerRider();
    PlayerRider(const IPlayerRiderD& r);
    ~PlayerRider();

    // need pre-check 'IsSummoned() != 0'
    BOOLEAN GetRiderItemStream(RENDER_ITEMSTREAMEX* item_stream);
    //----------------------------------------------------------------------------------------------
    // NOTE: f110411.2L, added the interface to support spread wings control on riding.
    // this method is implemented by _NA001955_110210_WING_COSTUME content.
    bool CanSpreadWings(const SCItemSlot& riding_item) const;
    // WARNING: don't use this method except specific control. this don't support various cases.
    bool ChangeFlyingState(bool flied);
    //----------------------------------------------------------------------------------------------

    eResult Route(const RiderControl& req_info,
                  RiderControl* result, RiderControlData* result_data);

    // casting start|cancel
    eResult SummonCasting(const RiderControl& casting_info,
                          RiderControl* result, RiderControlData* result_data);
    eResult Summon(const RiderControl& summon_info,
                   RiderControl* result, RiderControlData* result_data);
    eResult SummonRelease(const RiderControl& release_info,
                          RiderControl* result, RiderControlData* result_data);

    eResult Equip(const RiderControl& equip_info,
                  RiderControl* result, RiderControlData* result_data);
    eResult UnEquip(const RiderControl& unequip_info,
                    RiderControl* result, RiderControlData* result_data);

    // �ش� ����� Inventory���� ���� �����ϹǷ� PlayerRider ��ü�� ��� �Ǵ� ����̴�.
    static eResult WaxGloss(const RiderControl& wax_info,
                            RiderControl* result, RiderControlData* result_data,
                            Player* player);

    BOOLEAN CheckRule(const RiderControl& req_info,
                      RiderControl* result, RiderControlData* result_data, eResult* broad);

    // Support specific functions
    float GetScriptMoveSpeedRatio() const;
    BOOLEAN OnChangedOption(const nsSlot::RiderUpdateBits& update_bits);

    // for GM interface
    POSTYPE GetSummonedRiderItemPos() const;
    BOOLEAN ChangeToInstanceCasting() const; // casting�� ���� ������ �ð��� ������.

private:
    // (DESC) runtime check routine
    BOOLEAN IsValidCurrentContainedItemSlot() const;
    void ClearRiderItemInfo();
    void ClearGameControlInfo();
    struct RiderItemQueryResult {
        SCItemSlot* rider_item;
        BOOLEAN inventory_lock; // ���� ��û (packet)�� ���� ���͸��� ��Ŷ�ڵ鷯���� �̷�����.
                                // ���� ��� �κ����� ��� �����ϰ�, �ʿ��� ��� ���� ���� ����
    };
    RC::eITEM_RESULT GetRiderItem(POSTYPE request_item_pos, RiderItemQueryResult* query_result);
    // Optino ���� ����
    // result code ����� �� �ִ�. ex: RC_STATUS...?
    // (WARNING) �ش� ����� transaction�� �������Ѿ� �Ѵ�. ���� ���߿� �����ϸ� �뷫 ����
    BOOLEAN ApplyOption(const SCItemSlot& rider_item, const nsSlot::RiderUpdateBits& update_bits);
    BOOLEAN CancelOption(const SCItemSlot& rider_item, const nsSlot::RiderUpdateBits& update_bits);

private:
    const IPlayerRiderD& delegator_;
#pragma warning(push)
#pragma warning(disable : 4201)
    //----------------------------------------------------------------------------------------------
    struct {
        BOOLEAN is_transaction_;      // update field (enter/move) sync
        BOOLEAN is_applied_option_;   // is applied on player attribute (reserved)
    };
    //----------------------------------------------------------------------------------------------
    // assert (is_summoned_ != false)
    struct {
        POSTYPE item_pos_;              // summoned rider item pos
        bool flied_; // CHANGES: f110411.2L, added field to support flying enabled state on riding
        SCItemSlot* rider_item_;        // summoned rider item
        const sRiderInfo* rider_info_;  // summoned rider info 'RidingInfoParser'
        DBSERIAL rider_item_serial_;    // summoned rider item unique key - serial key
    };
    //----------------------------------------------------------------------------------------------
    struct {
        BOOLEAN hit_count_start_;
        WORD spawn_cancel_count_; // upperbound: sRiderInfo::spawn_cancel_count_
        DWORD cancel_timeout_;    // interval : sRiderInfo::cancel_count_timer_
    };
    //----------------------------------------------------------------------------------------------
#pragma warning(pop) // 4201
    friend class IPlayerRiderD;
    __DISABLE_COPY(PlayerRider);
};

//--------------------------------------------------------------------------------------------------
// Support specific functions

// �⺻ �ӵ� ������: 100�� ������ �ϴ� ���̴�. �̰��� ��ũ��Ʈ ���� �����̴�. Attributes�� ����� ��.
inline float
PlayerRider::GetScriptMoveSpeedRatio() const {
    return static_cast<float>(rider_info_->set_speed_);
}

inline void
PlayerRider::ClearRiderItemInfo()
{
    item_pos_ = INVALID_SLOT_POS;
    flied_ = false;
    rider_item_ = 0;
    rider_info_ = 0;
    rider_item_serial_ = TEMP_DBSERIAL_VALUE;
}

inline void
PlayerRider::ClearGameControlInfo()
{
    hit_count_start_ = 0;
    spawn_cancel_count_ = 0;
    cancel_timeout_ = 0;
}

// for GM interface
inline POSTYPE
PlayerRider::GetSummonedRiderItemPos() const
{
    return item_pos_;
}

#endif //_GAMESERVER_PLAYER_RIDER_H

