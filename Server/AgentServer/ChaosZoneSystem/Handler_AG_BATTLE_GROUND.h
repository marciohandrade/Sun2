#pragma  once
class GameServerSession;
class Handler_AG_BATTLE_GROUND
{
public:
    Handler_AG_BATTLE_GROUND()
    {
    }
    ~Handler_AG_BATTLE_GROUND()
    {
    }
    
    void OnAG_ProcessPacket(GameServerSession* server_session, MSG_BASE* packet, WORD size);
protected:
    void OnAG_BATTLE_GROUND_LEAVE_CMD(GameServerSession* server_session, MSG_BASE* packet, WORD size);
    void OnAG_BATTLE_GROUND_CAN_RESERVATION_ACK(GameServerSession* server_session, MSG_BASE* packet, WORD size);
};