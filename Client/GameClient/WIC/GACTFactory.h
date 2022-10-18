#pragma once

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING

#include "GACTMgr.h"

class GWICWrapper;

class GACTFactory
{
public:
    GACTFactory(GWICWrapper* pWICWrapper, GACTMgr* pACTMgr);
    ~GACTFactory(void);

public:
    IACT* CreateACT(const string& sID, const string& sCBID, const string& sRequest);

private:
    GWICWrapper* m_pWICWrapper;
    GACTMgr*     m_pACTMgr;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class GACTChargCash : public GACT
{
public:
    GACTChargCash(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest);

public:
    virtual LRESULT Execute();
    virtual BOOL    CallBack();
};

class GACTCash : public GACT
{
public:
    GACTCash(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest);

public:
    virtual LRESULT Execute();
    virtual BOOL    CallBack();
};

class GACTBuyItem : public GACT
{
public:
    GACTBuyItem(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest);

public:
    virtual LRESULT Execute();
    virtual BOOL    CallBack();
};

class GACTGift : public GACT
{
public:
    GACTGift(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest);

public:
    virtual LRESULT Execute();
    virtual BOOL    CallBack();
};

class GACTClan: public GACT
{
public:
    GACTClan(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest);

public:
    virtual LRESULT Execute();
    virtual BOOL    CallBack();
};

class GACTSendMsg: public GACT
{
public:
    GACTSendMsg(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest);

public:
    virtual LRESULT Execute();
    virtual BOOL    CallBack();
};

#endif //_APPLY_JAPAN_GAMECHU_CHANNELING
