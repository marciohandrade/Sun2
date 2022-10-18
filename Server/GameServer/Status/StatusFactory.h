#pragma once
#ifndef GAMESERVER_STATUS_FACTORY_H
#define GAMESERVER_STATUS_FACTORY_H

//==================================================================================================

class AbilityStatus;    class WoundStatus;      class StunStatus;
class AbsorbStatus;     class CureStatus;       class BlindStatus;
class MagicShieldStatus;class FearStatus;       class SPBonusStatus;
class ChaosStatus;      class WindShieldStatus; class MirrorStatus;
class ThrustStatus;     class SleepStatus;      class LowHPStatus;
class BonusDamageStatus;class DragonTransStatus;class SummonStatus;

class BaseStatus;       class StyleThrust;      class StyleStun;
class StyleDown;        class RecoverStatus;    class SteelStatus;

class IncompetenceStatus; class IntensifySummon;

class SuctionStatus;        class ChangeAttrStatus; class TransparentStatus;
class VitalSuctionStatus;   class BattleStatus;     class EnchantPoisonStatus;

//==================================================================================================

class StatusFactory
{
public:
    struct IStatusAllocator {
        virtual ~IStatusAllocator() = 0;
        virtual BaseStatus* Allocate() const = 0;
        virtual void Deallocate(BaseStatus* ptr) const = 0;
    };
    template<class StatusType_>
    struct StatusAllocNode;
    //
    enum eStatusPoolIndex;

    //
    virtual ~StatusFactory();
    // util::Singletone interfaces
    static StatusFactory* Instance();

    void Release();

    BaseStatus* AllocStatus(eCHAR_STATE_TYPE state_id, BOOL is_ability = true);
    void FreeStatus(BaseStatus* status);

    void DisplayPoolInfo();


private:
    //            <eCHAR_STATE_TYPE, link_index>
    typedef STLX_HASH_MAP<uint16_t, uint16>   STATUS_ROUTING_TABLE;
    //
    StatusFactory();
    static StatusFactory* CreateInstance();
    static bool SetupMappingTable(StatusFactory* factory);
    //
    STATUS_ROUTING_TABLE status_routing_table_;
    static StatusFactory* status_factory_static_;
};

//==================================================================================================
// implements
//
// interfaces for util::Singleton<>
inline StatusFactory* StatusFactory::Instance()
{
    if (status_factory_static_) {
        return status_factory_static_;
    }
    return CreateInstance();
}

#endif //GAMESERVER_STATUS_FACTORY_H
