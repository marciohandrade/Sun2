#pragma once

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH

class GameServerSession;
class Handler_AG_GOLDRUSH
{
public:
    Handler_AG_GOLDRUSH() {}
    ~Handler_AG_GOLDRUSH() {}

    void OnAG_ProcessPacket(GameServerSession* server_session, MSG_BASE* packet, WORD size);
protected:
    void OnAG_GOLDRUSH_LEAVE_CMD(GameServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnAG_GOLDRUSH_CAN_RESERVATION_ACK(GameServerSession* server_session, MSG_BASE* packet, WORD size);

};

#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH