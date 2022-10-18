#pragma once

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
class Handler_CG_GOLDRUSH
{
public:
    Handler_CG_GOLDRUSH() {}
    ~Handler_CG_GOLDRUSH() {}

    void OnCG_ProcessPacket(User* user, MSG_BASE* packet, WORD size);
protected:
    void OnCG_GOLDRUSH_RESERVATION_SYN(User* user, MSG_BASE* packet, WORD size);
    void OnCG_GOLDRUSH_RESERVATION_CANCEL_SYN(User* user, MSG_BASE* packet, WORD size);
    void OnCG_GOLDRUSH_ABNORMAL_EXIT_USER_REMOVE_SYN(User* user, MSG_BASE* packet, WORD size);
};
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH