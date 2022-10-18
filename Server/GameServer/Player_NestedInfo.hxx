#ifndef __GAMESERVER_PLAYER_INL
    #error "Can't direct include this header file"
#endif

#include <SSQ/SSQHandlerBase.h>
#include "./World/TriggerLinkInGame.hxx"

//==================================================================================================

namespace ns_object {

//==================================================================================================
//  <SPECIAL_RENDER_EFFECT>
//
class SPECIAL_RENDER_EFFECT // PROPERTY
{
    typedef DWORD                                   COUNT_T;
    // Inventory에 있는 자료구조를 연결할 수 있도록 하자.
    typedef STLX_MAP<SLOTCODE, COUNT_T>             RENDER_EFFECT_ITEM_LIST;
    typedef RENDER_EFFECT_ITEM_LIST::value_type     RENDER_EFFECT_ITEM_NODE;
public:
    SPECIAL_RENDER_EFFECT(eSPECIAL_RENDER_EFFECT& rFIELD)
        : m_rFIELD(rFIELD)
        , m_pList(NULL)
    {}

public:
    inline eSPECIAL_RENDER_EFFECT AddEffect(eSPECIAL_RENDER_EFFECT addEffect) {
        return m_rFIELD = (eSPECIAL_RENDER_EFFECT)((DWORD)m_rFIELD | (DWORD)addEffect);
    }
    inline eSPECIAL_RENDER_EFFECT RemoveEffect(eSPECIAL_RENDER_EFFECT addEffect) {
        return m_rFIELD = (eSPECIAL_RENDER_EFFECT)((DWORD)m_rFIELD & (~(DWORD)addEffect));
    }
    inline eSPECIAL_RENDER_EFFECT GetEffect() { return m_rFIELD; }

public:
    void Clear();
    void AddEffectItem(const SLOTCODE effectItemCode, const DWORD update_count);
    void RemoveEffectItem(const SLOTCODE effectItemCode, const DWORD update_count);
    DWORD SerializeRenderInfo(eSPECIAL_RENDER_EFFECT& OUT rOutBitRender,
                              SLOTCODE* const OUT pOutItemArray, const DWORD upperbound);

private:
    eSPECIAL_RENDER_EFFECT&  m_rFIELD;
    RENDER_EFFECT_ITEM_LIST* m_pList;
    //
    __DISABLE_COPY(SPECIAL_RENDER_EFFECT);
};

//==================================================================================================
// CHANGES: f110103.3L, add zone transaction information to support rollback

class PlayerZoneTransaction
{
public:
    struct Data {
        MONEY consumed_money;
    };
    PlayerZoneTransaction();
    PlayerZoneTransaction(Player* player);
    ~PlayerZoneTransaction();

    bool IsTransaction() const;
    void GetData(Data* const transaction_data) const;
    void BeginTransaction(const Data& transaction_data);
    void EndTransaction();
    void RollbackTransaction();
    //
private:
    Player* player_;
    Data transaction_data_;
};

inline PlayerZoneTransaction::PlayerZoneTransaction() {
    ZeroMemory(this, sizeof(*this));
};

inline PlayerZoneTransaction::~PlayerZoneTransaction() {
};

inline bool PlayerZoneTransaction::IsTransaction() const
{
    bool exist = (transaction_data_.consumed_money != 0);
    return exist;
}

};  // end namespace ns_object
//
//==================================================================================================
//
//

class SSQHandlerInGame;

//==================================================================================================
//
class ISSQHandlerInGame
{
    __DISABLE_COPY(ISSQHandlerInGame);
public:
    ISSQHandlerInGame()
        : m_pHandler(0)
        , m_ItemCode(0)
    {}

    ~ISSQHandlerInGame();
public:
    inline static BOOLEAN IsSSQEventID(const DWORD eventID) {
        return SSQHandlerCommon::IsSSQEventID(eventID);
    }
    inline BOOLEAN IsValidState() const { return !!m_pHandler ; }
    inline SSQHandlerInGame* operator->() { return m_pHandler; }
    void Init();
    void Release();

    inline SLOTCODE GetUsedItemCode() { return m_ItemCode; }
    inline void SetUsedItemCode(const SLOTCODE itemCode) { m_ItemCode = itemCode; }
private:
    SSQHandlerInGame* m_pHandler;
    SLOTCODE          m_ItemCode;
};

//==================================================================================================
//__NA001390_090915_RIDING_SYSTEM__
class PlayerRider;

struct IVehicleEvent {
    enum eEvents {
        eEvent_None         = 0,
        eEvent_Hit          ,
        eEvent_EnterField   ,
        eEvent_LeaveField   ,
        //------------------------------------------------------------------------------------------
        eEvent_TimeExpired  ,   // post effect 'summon released' call by ItemManager::Update
        eEvent_CancelByHit  ,   // post effect 'summon released' internal link 'eEvent_Hit'
        eEvent_CancelByDead ,   // post effect 'summon released' internal link 'eEvent_Hit'
        //------------------------------------------------------------------------------------------
        eEvent_LinkEvent    ,   // NOTE: f110411.2L, added value to support special event handling.
        eEvent_ChangeEquips ,   // NOTE: f110520.3L, the changes of equipments may occur wings controls.
        //
        eEvent_Counts
    };

    struct None;
    struct Hit;
    struct EnterField;
    struct LeaveField;
    struct TimeExpired;
    struct LinkEvent;
    struct ChangeEquips;
    // data fields...
    BYTE event;
    // <<data_type>> not defined
    //---------------------------------------------------------------
    static bool OnEvent(Player* player, const IVehicleEvent& event);
};

struct IVehicleEvent::None : IVehicleEvent { None() { event = eEvent_None; } };
struct IVehicleEvent::Hit : IVehicleEvent { Hit() { event = eEvent_Hit; } };
struct IVehicleEvent::EnterField : IVehicleEvent { EnterField() { event = eEvent_EnterField; } };
struct IVehicleEvent::LeaveField : IVehicleEvent { LeaveField() { event = eEvent_LeaveField; } };
struct IVehicleEvent::TimeExpired : IVehicleEvent { TimeExpired() { event = eEvent_TimeExpired; } };
struct IVehicleEvent::LinkEvent : IVehicleEvent { LinkEvent() { event = eEvent_LinkEvent; } };
struct IVehicleEvent::ChangeEquips : IVehicleEvent { ChangeEquips() { event = eEvent_ChangeEquips; } };

//==================================================================================================
//
class IPlayerRiderD
{
public:
    struct ContainerBase { // 12Bytes(reserve:3Bytes)
        PlayerRider* handler;
        Player* player;
        BOOLEAN is_summoned;
    };
private:
    struct ContainerExport : ContainerBase {
        BYTE _block[28];
    };
public:
    IPlayerRiderD()
    {
        ZeroMemory(this, sizeof(*this));
    }
    IPlayerRiderD(const IPlayerRiderD& r)
    {
        CopyMemory(this, &r, sizeof(*this));
    }
    ~IPlayerRiderD();

    BOOLEAN IsValidState() const { return !!container_.handler; }

    void Init(Player* player);
    void Release();
    void Update(Player* player, DWORD delta_tick);

    PlayerRider& Get() { return *container_.handler; }
    PlayerRider* operator->() { return container_.handler; }
    const PlayerRider* operator->() const { return container_.handler; }

    //----------------------------------------------------------------------------------------------
    //  Sync Event
    // assert 'IsValidState() != false'
    BOOLEAN Command_SummonRelease(const IVehicleEvent& event);
    BOOLEAN Command_ItemSync(Player* player, const SCItemSlot& changed_item);
    // special commands : for field<->battle synchronization
    BOOLEAN SyncPrepareRidingStatus(KEYTYPE enter_zone_key); // serializing from DBStream : first status
    BOOLEAN SyncNotifyRiderSummon(const SCItemSlot& item_slot, Player* player); // serializing notification : OnUpdate
    BOOLEAN SyncUpdateRidingStatus(Player* player); // EnterField

    //----------------------------------------------------------------------------------------------
    // Battle Event
    bool OnEvent(const IVehicleEvent& event);
    // shared
    static void SetDefaultFailResult(const RiderControl& req_info,
                                     RiderControl* result, RiderControlData* result_data);

    BOOLEAN IsSummoned()
    {
        return container_.is_summoned;
    }

private:
    bool _OnEvent(const IVehicleEvent& event);

    ContainerExport container_;
    friend class PlayerRider;
};


inline void
IPlayerRiderD::SetDefaultFailResult(const RiderControl& req_info,
                                    RiderControl* result, RiderControlData* result_data)
{
    result->request = req_info.request;
    result->result = req_info.eRet_Failed;
    result_data->data_type = result_data->eDataType_None;
    BOOST_STATIC_ASSERT(result_data->eDataType_None == 0); //for clear memory stream
}

inline bool
IPlayerRiderD::OnEvent(const IVehicleEvent& event)
{
    if (container_.is_summoned == 0) {
        if (event.event != event.eEvent_Hit) {
            return false;
        }
    }
    return _OnEvent(event);
}

//==================================================================================================
// NOTE: f110315.2L, IPlayerWingsD delegater for player wings control accessor
// declared by _NA001955_110210_WING_COSTUME
class IPlayerWingsD
{
public:
    struct ContainerBase { // 12Bytes(reserve:3Bytes)
        ns_object::PlayerWings* handler;
    };
public:
    IPlayerWingsD()
    {
        ZeroMemory(this, sizeof(*this));
    }
    IPlayerWingsD(const IPlayerRiderD& r)
    {
        CopyMemory(this, &r, sizeof(*this));
    }
    ~IPlayerWingsD() {}

    bool IsValidState() const { return !!container_.handler; }

    void Init(Player* player);
    void Release();
    void Update(Player* player, const util::ServerTimeInfoPerFrame& time_info);

    ns_object::PlayerWings& Get() { return *container_.handler; }
    ns_object::PlayerWings* operator->() { return container_.handler; }
    const ns_object::PlayerWings* operator->() const { return container_.handler; }

    //----------------------------------------------------------------------------------------------
    //  Sync Event
    // assert 'IsValidState() != false'
    //----------------------------------------------------------------------------------------------
    // Battle Event
    // shared
    bool OnEvent(const IVehicleEvent& event);

    bool Command_Furl(const IVehicleEvent& event);

private:
    //
    bool _OnEvent(const IVehicleEvent& event);
    //
    ContainerBase container_;
    friend class ns_object::PlayerWings;
};

inline bool
IPlayerWingsD::OnEvent(const IVehicleEvent& event)
{
    if (container_.handler == NULL) {
        return false;
    }
    return _OnEvent(event);
}

//==================================================================================================
//
namespace nsSlot {
;

class EtheriaImpl;
// _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
// this has a different presentation from a complex riding casting
struct EtheriaControlOption
{
    bool doing_casting_;
    DWORD timeout_;
    void Clear() { ZeroMemory(this, sizeof(*this)); }
};

}; //end of namespace
//==================================================================================================

namespace ns_object {

class PlayerAttributeLinker : public IPlayerAttributeLinker
{
public:
    PlayerAttributeLinker() : player_(NULL), char_info_(NULL) {}
    PlayerAttributeLinker(Player* player);
    ~PlayerAttributeLinker();
    //
    virtual BASE_PLAYERINFO* GetCharacterInfo() const;
    virtual eCHAR_TYPE GetCharType() const;
    virtual LEVELTYPE GetLevel() const;
    virtual DWORD GetWeaponKind() const;
    virtual void SetCharInfo(BASE_PLAYERINFO* char_info);
    //
    Player* GetOwner() const;
private:
    Player* const player_;
    BASE_PLAYERINFO* const char_info_;
    //
    __DISABLE_COPY(PlayerAttributeLinker);
};

inline Player* PlayerAttributeLinker::GetOwner() const {
    return player_;
};

//==================================================================================================

struct ChangedValuesNotifier
{
    ChangedValuesNotifier(Player* input_player);
    ~ChangedValuesNotifier();

    Player* const player;
    const DWORD max_hp;
    const DWORD max_mp;
    const DWORD max_sd;
    //
    __DISABLE_COPY(ChangedValuesNotifier);
};

//==================================================================================================
}; //end of namespace
//==================================================================================================

