#ifndef _AUTHSERVER_QUERY_INTERFACES_H
#define _AUTHSERVER_QUERY_INTERFACES_H

#include <HanAuth/HanAuthQueryManager.h>

#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__

class HanAuthQueryCallback : public nsHanAuth::IHanAuthCallback
{
public:
    virtual void AuthResult(nsHanAuth::INDEX_ID_T query_slot_index, int hanAuth_result,
                            const char user_id[ID_MAX_LEN + 1] /* etc ... */);
};

extern HanAuthQueryCallback HanAuthQueryCallback_Global;

#endif //__NA000000_NORTH_AMERICA_HANGAME_CHANNELING__

#endif //_AUTHSERVER_QUERY_INTERFACES_H

