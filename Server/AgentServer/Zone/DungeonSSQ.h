//==================================================================================================
//    <DungeonSSQ>

#include "InstanceDungeon.h"

struct SSQInfo;
class DungeonSSQ : public InstanceDungeon
{
public:
    DungeonSSQ();
    virtual ~DungeonSSQ();
    inline virtual RC::eROOM_RESULT CanCreate(User* pUser, // Not Used
                                              MAPCODE mapCode, eZONETYPE eZoneType,
                                              eZONEPUBLIC ePublic, BASE_ROOMINFO& IN baseRoomInfo,
                                              BOOL checkOpenTime = TRUE);
    virtual RC::eROOM_RESULT Create(User* pUser, MAPCODE mapCode, KEYTYPE instanceDungeonKey,
                                    ServerSession* pServerSession, BOOL useChargeItem = FALSE);

    virtual VOID On_create();
    virtual VOID On_destroy();
    virtual VOID On_enteruser(User* pUser);
    virtual VOID On_leaveuser(User* pUser);

    virtual BOOL    Process();
    VOID    SendJoinNAKPacket(User* pUser);
    VOID    SendJoinACKPacket(User* pUser);

    inline VOID SetClearProcess() { m_bCloseState = TRUE; }

protected:
    virtual RC::eROOM_RESULT SetBaseRoom(User* pUser, BASE_ROOMINFO& baseRoomInfo, MAPCODE mapCode);

private:
    BOOLEAN m_bCloseState;

    friend class WarControlManager;
    __DISABLE_COPY(DungeonSSQ);
};
