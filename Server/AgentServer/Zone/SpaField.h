#pragma once
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM

#include "ZoneInterface.h"

class SpaField : public ZoneInterface
{
    _DECLARE_POOL(SpaField)
public:
    SpaField() {}
    virtual ~SpaField() {}

    VOID Create(KEYTYPE key, MAPCODE MapCode, ServerSession* pServerSession);
    virtual VOID ReadyUser( User * pUser, eSEND_TYPE type, MAPCODE FieldCode = 0,
        DWORD dwAreaID = 0, WzVector * pwvStartPos = NULL, bool bFirstJoin = false );

    virtual eZONETYPE GetType() { return eZONETYPE_SPA_FIELD; }

protected:
    virtual void On_create();
    virtual void On_readyuser(User* user);
    virtual void On_enteruser(User* user);
    virtual void On_leaveuser(User* user);

private:
};
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM