#pragma once
#ifndef __PROGRAMCOMMON_NPC_ATTRIBUTES_H
#define __PROGRAMCOMMON_NPC_ATTRIBUTES_H

#include "Attribute.h"

class SCNPC;
struct BASE_NPCINFO;

//==================================================================================================

class NPCAttributes : public Attributes
{
public :
    NPCAttributes();
    virtual ~NPCAttributes();

    void SetOwner(SCNPC* owner);
    void SetCharInfo(const BASE_NPCINFO* npc_info,
                     int recover_hp, int recover_mp, int recover_sd);

    virtual void Clear(); // Attributes interface
    virtual void Update(); // Attributes interface
    virtual void UpdateEx(); // Attributes interface

#if defined(_SERVER)
    // CHANGES: f110413.1L, added interface to support light recovery quantity calculation for server
    // NOTE: HP, MP and SD recovery have busy update processes.
    // if server is to avoid busy updating, detach the recovery parts from full update calling.
    // the usage of this method is calling on changed recoveries in run-time service
    // not to initialize like the 'SetCharInfo'
    void UpdateChangedRecoveries(
        bool hp_update, int hp_recovery_quantity,
        bool mp_update, int mp_recovery_quantity,
        bool sd_update, int sd_recovery_quantity);
#endif
private:
    void UpdateAttackPower();
    void UpdateDefense();

public :
    SCNPC* owner_;
};

//==================================================================================================


inline void NPCAttributes::Clear()
{
    RegisterAll();
}

inline void NPCAttributes::UpdateEx()
{
}

#endif //__PROGRAMCOMMON_NPC_ATTRIBUTES_H