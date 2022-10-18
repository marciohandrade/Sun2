#include "SunClientPrecompiledHeader.h"
#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING

#include "GACTMgr.h"
#include "GWICWrapper.h"

GACTMgr::GACTMgr(GWICWrapper* pWICWrapper)
{
    m_pWICWrapper = pWICWrapper;
}

GACTMgr::~GACTMgr(void)
{
    Clear();

    m_pWICWrapper = NULL;
}

LRESULT GACTMgr::Execute(IACT* pACT)
{
    LRESULT lResult = pACT->Execute();

    if (lResult != 0)
        this->RegisterACT(pACT);
    else
        delete pACT;
    
    return lResult;
}

void GACTMgr::CallBack(IACT* pACT)
{
    if (pACT)
    {
        pACT->CallBack();
        this->UnRegisterACT( pACT->GetID() );
    }
}

LRESULT GACTMgr::SendMsg(IACT* pACT)
{
    if (m_pWICWrapper)
        return m_pWICWrapper->SendMsg(pACT);

    return 0;
}

HANDLE GACTMgr::CreateCBEvent(const string& sCBID)
{
    if (m_pWICWrapper)
        return m_pWICWrapper->CreateCBEvent(sCBID);

    return NULL;
}

void GACTMgr::DestroyCBEvent(const string& sCBID)
{
    if (m_pWICWrapper)
        return m_pWICWrapper->DestroyCBEvent(sCBID);
}

BOOL GACTMgr::RegisterACT(IACT* pACT)
{
    if (pACT == NULL)
        return FALSE;

    if (FindACT(pACT->GetID()) != NULL)
        return FALSE;

    m_lstACTPool.push_back(pACT);

    return TRUE;
}

void GACTMgr::UnRegisterACT(const string& sID)
{
    if (m_lstACTPool.size() <= 0)
	    return;

    TIt it = m_lstACTPool.begin();

    IACT* pACT = NULL;
    for (it; it!= m_lstACTPool.end(); it++)
    {
	    pACT = (*it);
	    if (pACT)
	    {
		    if (pACT->GetID() == sID)
		    {
		        delete pACT;
			    
                m_lstACTPool.erase(it);
			    break;
		    }
	    }
    }
}

IACT* GACTMgr::GetACT(int nIndex)
{
	if (m_lstACTPool.size() <= 0)
        return NULL;

    if (nIndex >= (int)m_lstACTPool.size())
        return NULL;

    UINT nCount = 0;

    TIt it = m_lstACTPool.begin();
    IACT*  pACT = NULL;
    for (it; it!= m_lstACTPool.end(); it++)
    {
	    pACT = (*it);
	    if (pACT)
        {
		    if (nCount == nIndex)
			    return pACT;
        }

        nCount++;
    }

    return NULL;
}

IACT* GACTMgr::FindACT(const string& sID)
{
    if (m_lstACTPool.size() <= 0)
	    return NULL;

    TIt it = m_lstACTPool.begin();

    IACT* pACT = NULL;
    for (it; it!= m_lstACTPool.end(); it++)
    {
	    pACT = (*it);
	    if (pACT)
        {
		    if (pACT->GetID() == sID)
			    return pACT;
        }
    }

    return NULL;
}

int GACTMgr::GetCount()
{
    return (int)m_lstACTPool.size();
}

void GACTMgr::Clear()
{
    if (m_lstACTPool.size() <= 0)
	    return;

    TIt it = m_lstACTPool.begin();

    IACT* pACT = NULL;
    while(it!= m_lstACTPool.end())
    {
        pACT = (*it);
        if (pACT)
	        delete pACT;

	    it = m_lstACTPool.erase(it);
    }
}


GACT::GACT(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest)
{
    m_pACTMgr  = pACTMgr;

    m_sID      = sID;
    m_sCBID    = sCBID;
    m_sRequest = sRequest;
}

GACTMgr* GACT::GetACTMgr()
{ 
    return m_pACTMgr;
}

string GACT::GetID()
{
    return m_sID;
}

string GACT::GetCBID()
{
    return m_sCBID;
}

string GACT::GetRequest()
{ 
    return m_sRequest;
}

string GACT::GetCallBackResult()
{ 
    return m_sCallBackResult;
}

void GACT::SetACTMgr(GACTMgr* pACTMgr)
{ 
    m_pACTMgr = pACTMgr; 
}

void GACT::SetRequest(const string& sRequest)      
{ 
    m_sRequest = sRequest;
}

void GACT::SetCallBackResult(const string& sResult)
{ 
    m_sCallBackResult = sResult; 
}

#endif //_APPLY_JAPAN_GAMECHU_CHANNELING
