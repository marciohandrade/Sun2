#pragma  once

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
class GameServerSession;
class Handler_AG_FREE_PVP
{
public:
    Handler_AG_FREE_PVP()
    {
    }
    ~Handler_AG_FREE_PVP()
    {
    }

    void OnAG_ProcessPacket(GameServerSession* server_session, MSG_BASE* packet, WORD size);
protected:
    void OnAG_FREE_PVP_LEAVE_ROOM_CMD(GameServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnAG_FREE_PVP_ACTIVE_CMD(GameServerSession* server_session, MSG_BASE* packet, WORD size);
};

#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP