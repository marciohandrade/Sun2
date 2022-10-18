#pragma once

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING

#include <string>
#include <list>

using namespace std;

class IACT;
class GWICWrapper;

class GACTMgr
{
public:
    GACTMgr(GWICWrapper* pWICWrapper);
    ~GACTMgr();

public:
    LRESULT Execute(IACT* pACT);
    void    CallBack(IACT* pACT);

public:
    LRESULT SendMsg(IACT* pACT);
    HANDLE  CreateCBEvent( const string& sCBID );
    void    DestroyCBEvent( const string& sCBID );

public:
    BOOL  RegisterACT(IACT* pACT);
    void  UnRegisterACT(const string& sID);

    IACT* GetACT(int nIndex);
    IACT* FindACT(const string& sID);
    int   GetCount();

    void  Clear();


private:
    typedef std::list<IACT*>   TACTPool;
    typedef TACTPool::iterator TIt;

    TACTPool                   m_lstACTPool;
    GWICWrapper*               m_pWICWrapper;
};


class IACT
{
public:
    virtual ~IACT() {};


public:
    virtual GACTMgr* GetACTMgr() = 0;
    virtual string   GetID() = 0;
    virtual string   GetCBID() = 0;
    virtual string   GetRequest() = 0;

    virtual string   GetCallBackResult() = 0;


    virtual void     SetRequest(const string& sRequest) = 0;
    virtual void     SetCallBackResult(const string& sResult) = 0;

public:
    virtual LRESULT  Execute() = 0;

    virtual BOOL     CallBack() = 0;
};


class GACT : public IACT
{
public:
    GACT(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest);
    virtual ~GACT() {};


public:
    virtual GACTMgr* GetACTMgr();
    virtual string   GetID();
    virtual string   GetCBID();

    virtual string   GetRequest();
    virtual string   GetCallBackResult();


    virtual void     SetACTMgr(GACTMgr* pACTMgr);
    virtual void     SetRequest(const string& sRequest);
    virtual void     SetCallBackResult(const string& sResult);


public:
    GACTMgr*         m_pACTMgr;

    string           m_sID;
    string           m_sCBID;
    string           m_sRequest;
    string           m_sCallBackResult;
};

#endif //_APPLY_JAPAN_GAMECHU_CHANNELING
