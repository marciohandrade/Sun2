#pragma  once

class Handler_CG_BATTLE_GROUND
{
public:
    Handler_CG_BATTLE_GROUND()
    {
    }
    ~Handler_CG_BATTLE_GROUND()
    {
    }
    
    void OnCG_ProcessPacket(User* user, MSG_BASE* packet, WORD size);

protected:
    void OnCG_BATTLE_GROUND_RESERVATION_SYN(User* user, MSG_BASE* packet, WORD size);
    void OnCG_BATTLE_GROUND_RESERVATION_CANCEL_SYN(User* user, MSG_BASE* packet, WORD size);
    void OnCG_BATTLE_GROUND_ABNORMAL_EXIT_USER_REMOVE_SYN(User* user, MSG_BASE* packet, WORD size);
#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
    void OnCG_BATTLE_GROUND_GET_OPEN_TIME_SYN(User* user, MSG_BASE* packet, WORD size);
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
};