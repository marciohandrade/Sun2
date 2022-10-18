#pragma once
#ifdef _NA_008334_20150608_SONNENSCHEIN

#define Handler_AG_SONNENSCHEIN_DECL(p) \
    VOID On##p(GameServerSession* server_session, MSG_BASE* packet, WORD size)
#define Handler_AG_SONNENSCHEIN_IMPL(p) \
    VOID Handler_AG_SONNENSCHEIN::On##p(GameServerSession* server_session, MSG_BASE* packet, WORD size)

class GameServerSession;
//------------------------------------------------------------------------------------------------

class Handler_AG_SONNENSCHEIN
{
public:
    Handler_AG_SONNENSCHEIN() {}
    ~Handler_AG_SONNENSCHEIN() {}

    void OnAG_ProcessPacket(GameServerSession* server_session, MSG_BASE* packet, WORD size);

private:
    Handler_AG_SONNENSCHEIN_DECL(AG_SONNENSCHEIN_REQUEST_GUID_SYN);
    Handler_AG_SONNENSCHEIN_DECL(AG_SONNENSCHEIN_KICKUSER_CMD);
    Handler_AG_SONNENSCHEIN_DECL(AG_SONNENSCHEIN_ROOM_DESTROY_CMD);
    Handler_AG_SONNENSCHEIN_DECL(AG_SONNENSCHEIN_STAMP_COMPLETE_CMD);

    Handler_AG_SONNENSCHEIN_DECL(AG_SONNENSCHEIN_ENTER_CHECK_ACK);
};


#endif _NA_008334_20150608_SONNENSCHEIN