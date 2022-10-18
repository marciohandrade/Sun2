#include "stdafx.h"
#include "./DBUnit.h"
//
#include "SolarDatabaseInDBP.h"
#include "DBProxyServerEx.h"
#include "PacketHandler/PacketHandler.h"

//==================================================================================================

DBUnit::DBUnit()
{
    ZeroMemory(pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
}

DBUnit::~DBUnit()
{
}

bool DBUnit::Init(DWORD db_unit_id, int db_connector_index,
                  eSERVER_TYPE server_type, DWORD session_index)
{
    ZeroMemory(pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
    unit_state_.value = unit_state_.eState_Init;
    db_connector_index_ = static_cast<uint8_t>(db_connector_index);
    db_unit_id_ = static_cast<uint16_t>(db_unit_id);
    server_type_ = static_cast<uint8_t>(server_type);
    session_index_ = session_index;
    //
    return true;
}

bool DBUnit::RequestRelease()
{
    if (number_of_queries_ != 0) {
        unit_state_.value = unit_state_.eState_Closing;
        return false;
    }
    ZeroMemory(pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
    return true;
}


//==================================================================================================
