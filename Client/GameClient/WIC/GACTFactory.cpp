#include "SunClientPrecompiledHeader.h"

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING

#include "GACTFactory.h"
#include "GACTMgr.h"
#include "GWICWrapper.h"
#include "GDefine.h"

int AnsiToUTF16(const string& strAnsi, wstring& strUnicode)
{
    int len = ::MultiByteToWideChar(CP_ACP, 0, strAnsi.c_str(), -1, NULL, 0);
    if(len == 0)	return len;
    wchar_t* wbuf = new wchar_t[len];
    len = ::MultiByteToWideChar(CP_ACP, 0, strAnsi.c_str(), -1, wbuf, len);
    if(len != 0)	strUnicode = wbuf; 
    delete[] wbuf;
    return len;
}

///////////////////////////////////////////////////////////
wstring AnsiToUTF16(const string& strAnsi)
{
    wstring strUnicode = L"";

    AnsiToUTF16( strAnsi, strUnicode );

    return strUnicode;
}


GACTFactory::GACTFactory(GWICWrapper* pWICWrapper, GACTMgr* pACTMgr)
{
    m_pWICWrapper = pWICWrapper;
    m_pACTMgr     = pACTMgr;
}

GACTFactory::~GACTFactory(void)
{
    m_pWICWrapper = NULL;
    m_pACTMgr     = NULL;
}

IACT* GACTFactory::CreateACT(const string& sID, const string& sCBID, const string& sRequest)
{
    IACT* pACT = NULL;
    if ( sID == CHARGE_CASH )
        pACT = new GACTChargCash(m_pACTMgr, sID, sCBID, "");

    else if ( sID == CASH_REQ )
        pACT = new GACTCash(m_pACTMgr, sID, sCBID, "");

    else if ( sID == CHARGE_GIFT )
        pACT = new GACTGift(m_pACTMgr, sID, sCBID, sRequest);

    else if ( sID == BUY_ITEM )
        pACT = new GACTBuyItem(m_pACTMgr, sID, sCBID, sRequest);

    else if ( sID == CLAN )
        pACT = new GACTClan(m_pACTMgr, sID, sCBID, sRequest);

    else if ( sID == SENDMSG )
        pACT = new GACTClan(m_pACTMgr, sID, sCBID, sRequest);

    return pACT;
}

GACTChargCash::GACTChargCash(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest) 
: GACT(pACTMgr, sID, sCBID, sRequest)
{

}

LRESULT GACTChargCash::Execute()
{
    LRESULT lResult = 1;
    HANDLE  hEvent  = NULL;
	if (hEvent = ::OpenEventA(EVENT_ALL_ACCESS | EVENT_MODIFY_STATE, FALSE, m_sID.c_str()))
	{
		::SetEvent(hEvent);
		::CloseHandle(hEvent);

        m_pACTMgr->CreateCBEvent(m_sCBID);;
	}
    else
        lResult = 0;

    return lResult;
}

BOOL GACTChargCash::CallBack()
{
    return TRUE;
}

GACTCash::GACTCash(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest) 
: GACT(pACTMgr, sID, sCBID, "")
{
}

LRESULT GACTCash::Execute()
{
    LRESULT lResult = 1;
    HANDLE  hEvent  = NULL;
	if (hEvent = ::OpenEventA(EVENT_ALL_ACCESS | EVENT_MODIFY_STATE, FALSE, m_sID.c_str()))
	{
		::SetEvent(hEvent);
		::CloseHandle(hEvent);

        m_pACTMgr->CreateCBEvent(m_sCBID);
	}
    else
        lResult = 0;

    return lResult;
}

BOOL GACTCash::CallBack()
{
    return TRUE;
}

GACTBuyItem::GACTBuyItem(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest) 
: GACT(pACTMgr, sID, sCBID, sRequest)
{
}

LRESULT GACTBuyItem::Execute()
{
    LRESULT lResult = m_pACTMgr->SendMsg(this);
    if ( lResult != 0 )
    {
        // Create Event Kernel Object
        m_pACTMgr->CreateCBEvent(m_sCBID);
    }
    return lResult;
}

BOOL GACTBuyItem::CallBack()
{
    return TRUE;
}

GACTGift::GACTGift(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest) 
: GACT(pACTMgr, sID, sCBID, sRequest)
{
}

LRESULT GACTGift::Execute()
{
    LRESULT lResult = m_pACTMgr->SendMsg(this);
    if ( lResult != 0 )
    {
        // Create Event Kernel Object
        m_pACTMgr->CreateCBEvent(m_sCBID);
    }

    return lResult;
}

BOOL GACTGift::CallBack()
{
    return TRUE;
}

GACTClan::GACTClan(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest) 
: GACT(pACTMgr, sID, sCBID, sRequest)
{
}
LRESULT GACTClan::Execute()
{
    LRESULT lResult = m_pACTMgr->SendMsg(this);
    if ( lResult != 0 )
    {
        // Create Event Kernel Object
        m_pACTMgr->CreateCBEvent(m_sCBID);
    }

    return lResult;
}

BOOL GACTClan::CallBack()
{
    return TRUE;
}

GACTSendMsg::GACTSendMsg(GACTMgr* pACTMgr, const string& sID, const string& sCBID, const string& sRequest) 
: GACT(pACTMgr, sID, sCBID, sRequest)
{
}
LRESULT GACTSendMsg::Execute()
{
    LRESULT lResult = m_pACTMgr->SendMsg(this);
    if ( lResult != 0 )
    {
        // Create Event Kernel Object
        m_pACTMgr->CreateCBEvent(m_sCBID);
    }

    return lResult;
}

BOOL GACTSendMsg::CallBack()
{
     return TRUE;
}

#endif //_APPLY_JAPAN_GAMECHU_CHANNELING
