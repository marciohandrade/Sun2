#pragma  once

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

class Handler_CG_FREE_PVP
{
public:
    Handler_CG_FREE_PVP()
    {
    }
    ~Handler_CG_FREE_PVP()
    {
    }
    
    void OnCG_ProcessPacket(User* user, MSG_BASE* packet, WORD size);    

    void OnCG_FREE_PVP_ROOM_LIST_SYN(User* user, MSG_BASE* packet, WORD size);
    void OnCG_FREE_PVP_CREATE_ROOM_SYN(User* user, MSG_BASE* packet, WORD size);
    void OnCG_FREE_PVP_JOIN_ROOM_SYN(User* user, MSG_BASE* packet, WORD size);
};

#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP