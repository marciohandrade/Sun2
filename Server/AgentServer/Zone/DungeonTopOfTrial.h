#pragma once

//  <DungeonTopOfTrial>
#include "InstanceDungeon.h"

class DungeonTopOfTrial : public InstanceDungeon
{
public:
    DungeonTopOfTrial();
    virtual ~DungeonTopOfTrial();
    virtual eINSTANCE_DUNGEON_KIND GetDungeonType() { return INDUN_KIND_TOP_OF_TRIAL; }

    virtual RC::eROOM_RESULT CanCreate(User* pUser, MAPCODE mapCode, eZONETYPE eZoneType,
                                       eZONEPUBLIC ePublic, BASE_ROOMINFO& IN BaseRoomInfo,
                                       BOOL CheckOpenTime = TRUE);
    virtual VOID    SendJoinNAKPacket(User* pUser);
    virtual VOID    SendJoinACKPacket(User* pUser);
    __DISABLE_COPY(DungeonTopOfTrial);
};

