#pragma once
#ifdef _NA_008334_20150608_SONNENSCHEIN

#define Handler_CG_SONNENSCHEIN_DECL(p) \
    VOID On##p(User* user, MSG_BASE* packet, WORD wSize)
#define Handler_CG_SONNENSCHEIN_IMPL(p) \
    VOID Handler_CG_SONNENSCHEIN::On##p(User* user, MSG_BASE* packet, WORD wSize)

class Handler_CG_SONNENSCHEIN
{
public:
    Handler_CG_SONNENSCHEIN() {}
    ~Handler_CG_SONNENSCHEIN() {}

    void OnCG_ProcessPacket(User* user, MSG_BASE* packet, WORD size);

private:
    Handler_CG_SONNENSCHEIN_DECL(CG_SONNENSCHEIN_CREATE_SYN);
    Handler_CG_SONNENSCHEIN_DECL(CG_SONNENSCHEIN_ENTER_CHECK_SYN);
    Handler_CG_SONNENSCHEIN_DECL(CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN);
};


#endif //_NA_008334_20150608_SONNENSCHEIN