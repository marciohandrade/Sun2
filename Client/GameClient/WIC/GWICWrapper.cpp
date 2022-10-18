#include "SunClientPrecompiledHeader.h"
#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING

#include "GWICWrapper.h"
#include "GACTMgr.h"
#include "GACTFactory.h"
//  For Finding WIC Window In GameChuLauncher
#define CHARGE_WND_HANDLE       "D7CD782E-A5B6-471d-9564-72F87BA18BDF"

// For Sending GameClient's HWND to GameChuLauncher.
#define GET_MSG_HWND            "2D67AF8D-88E6-4407-83C7-155ACF425B82"
    
GWICWrapper::GWICWrapper()
{
    m_hGame = NULL;
    m_hWIC = NULL;

    m_pACTMgr = NULL;
    m_pACTFactory = NULL;

    m_bInitialized = FALSE;
}

GWICWrapper::~GWICWrapper(void)
{
    Clear();
}

BOOL GWICWrapper::Initialize()
{
    m_hWIC = FindWIC(); 

    if (m_hWIC == NULL)
    {
        MessageBox(NULL, "Failed to Find WIC", NULL, MB_OK);
        return FALSE;
    }

    m_mapRegisteredCBEvent.clear();

    m_pACTMgr = new GACTMgr(this);
    m_pACTFactory = new GACTFactory(this, m_pACTMgr);

    m_bInitialized = TRUE;

    return TRUE;
}

void GWICWrapper::Clear()
{
    m_hGame = NULL;
    m_hWIC  = NULL;

    ClearRCBF();

    if (m_pACTMgr)
    {
        delete m_pACTMgr;
        m_pACTMgr = NULL;
    }

    if (m_pACTFactory)
    {
        delete m_pACTFactory;
        m_pACTFactory = NULL;
    }
}

void GWICWrapper::ClearRCBF()
{
    TRCBF_POS pos = m_mapRegisteredCBEvent.begin();
    TRCBF_POS end = m_mapRegisteredCBEvent.end();

    HANDLE hEvent = NULL;
    while( pos != end )
    {
        hEvent = pos->second;

        if (hEvent)
            ::CloseHandle(hEvent);

        pos++;
    }
    
    m_mapRegisteredCBEvent.clear();
}

HWND GWICWrapper::GetHWIC()            
{
    if (m_hWIC == NULL)
        m_hWIC = FindWIC();

    return m_hWIC;
}

HWND GWICWrapper::GetHGame()            
{
    return m_hGame;
}

void GWICWrapper::SetHGame(HWND hGame)            
{
    m_hGame = hGame;
}

BOOL GWICWrapper::TransferMyHWND()
{
    string sGUID = GET_MSG_HWND;
    LRESULT lResult = SendMsg(sGUID, m_hGame);
    HWND hWIC = (HWND)(lResult);
    BOOL bSuccess = FALSE;

    if ((m_hWIC == hWIC) && (m_hWIC != NULL))
        bSuccess =  TRUE;

    m_bInitialized = bSuccess;
    return bSuccess;
}

BOOL GWICWrapper::InvokeChargeCash()
{
    string sID   = CHARGE_CASH;
    string sCBID = CHARGE_CASH_CALLBACK;

    return Invoke(sID, sCBID, "");
}

BOOL GWICWrapper::InvokeCash()
{
    string sID   = CASH_REQ;
    string sCBID = CASH_REQ_CALLBACK;

    return Invoke(sID, sCBID, "");
}

BOOL GWICWrapper::InvokeBuyItem(const string& sValue)
{
    string sID   = BUY_ITEM;
    string sCBID = BUY_ITEM_CALLBACK;

    return Invoke(sID, sCBID, sValue);
}

BOOL GWICWrapper::InvokeGift(const string& sValue)
{
    string sID   = CHARGE_GIFT;
    string sCBID = CHARGE_GIFT_CALLBACK;

    return Invoke(sID, sCBID, sValue);
}

BOOL GWICWrapper::InvokeClan(const string& sValue)
{
    string sID   = CLAN;
    string sCBID = CLAN_CALLBACK;

    return Invoke(sID, sCBID, sValue);
}

BOOL GWICWrapper::InvokeSendMsg(const string& sValue)
{
    string sID   = SENDMSG;
    string sCBID = SENDMSG_CALLBACK;

    return Invoke(sID, sCBID, sValue);
}

BOOL GWICWrapper::Invoke(const string& sID, const string& sCBID, const string& sValue)
{
    if ((CheckFunction(sID) == TRUE) && (IsProcessing() == FALSE))
    {
        // 1. Create ACT ..
        IACT* pACT = m_pACTFactory->CreateACT(sID, sCBID, sValue);
        if (pACT == NULL)
        {
            return FALSE;
        }

        // 2. Execute
        LRESULT lResult = m_pACTMgr->Execute(pACT);

        if (lResult != 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}

//
LRESULT GWICWrapper::SendMsg( IACT* pACT )
{
    if (m_hGame == NULL && m_hWIC == NULL)
        return 0;

    LRESULT lResult = 0;
    string sValue = pACT->GetRequest();
    if (sValue.length() > 0)
    {
        COPYDATASTRUCT stData;
        CHAR szTemp[MAX_PATH];

        sprintf(szTemp, "%s=%s\0", pACT->GetID().c_str(), pACT->GetRequest().c_str());
        stData.dwData = 0;		
        stData.cbData = (DWORD)strlen(szTemp) +1;			
        stData.lpData = (PVOID)szTemp;

        lResult = ::SendMessageA(m_hWIC, WM_COPYDATA, (WPARAM)m_hGame, (LPARAM)(&stData));				
    }

    return lResult;
}

LRESULT GWICWrapper::SendMsg(const string& sID, HWND hWnd)
{
    if ( m_hGame == NULL && m_hWIC == NULL )
        return 0;

	COPYDATASTRUCT stData;
	stData.dwData = 0;
	stData.lpData = (LPVOID)sID.c_str();
    stData.cbData = (DWORD)sID.length() + 1;
	
	return ::SendMessage(m_hWIC, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)(&stData));
}

BOOL GWICWrapper::CheckFunction(const string& sID)
{
    HANDLE hEvent = ::OpenEventA(EVENT_ALL_ACCESS | EVENT_MODIFY_STATE,FALSE, sID.c_str());
    if (hEvent == NULL)
        return FALSE;

    CloseHandle(hEvent);
    return TRUE;
}

BOOL GWICWrapper::IsProcessing()
{
    if (m_pACTMgr->GetCount() > 0)
        return TRUE;

    return FALSE;
}

HANDLE GWICWrapper::CreateCBEvent(const string& sCBID)
{
    HANDLE hEvent = NULL;

    hEvent = FindRegisteredCBEvent(sCBID);
    if (hEvent == NULL)
    {
        hEvent = ::CreateEventA(NULL, FALSE, FALSE, sCBID.c_str());

        if (hEvent)
            RegisterCBEvent(sCBID, hEvent);
    }

     return hEvent;
}

void GWICWrapper::DestroyCBEvent(const string& sCBID)
{
    if (sCBID.size() == 0)
        return;

    HANDLE hEvent = FindRegisteredCBEvent(sCBID);
    if (hEvent != NULL)
    {
        ::CloseHandle(hEvent);
        this->UnRegisterCBEvent(sCBID);
    }
}

string GWICWrapper::FindRegisteredCBID(HANDLE hCBEvent)
{
    if (hCBEvent == NULL)
        return "";

    TRCBF_POS pos = m_mapRegisteredCBEvent.begin();
    TRCBF_POS end = m_mapRegisteredCBEvent.end();

    HANDLE hEvent = NULL;
    while (pos != end)
    {
        hEvent = pos->second;

        if (hEvent == hCBEvent)
            return pos->first;

        pos++;
    }
    
    return "";
}

HANDLE GWICWrapper::FindRegisteredCBEvent(const string& sCBID)
{
    if (sCBID.size() == 0)
        return NULL;
    
    TRCBF_POS pos = m_mapRegisteredCBEvent.begin();
    TRCBF_POS end = m_mapRegisteredCBEvent.end();

    string sID = "";
    while (pos != end)
    {
        sID = pos->first;
        if (sID == sCBID)
            return pos->second;

        pos++;
    }
    
    return NULL;
}

HANDLE GWICWrapper::RegisterCBEvent(const string& sCBID, HANDLE hEvent)
{
    TRCBF_POS pos = m_mapRegisteredCBEvent.find(sCBID);
    if (pos == m_mapRegisteredCBEvent.end())
    {
        m_mapRegisteredCBEvent.insert(std::make_pair(sCBID, hEvent));
    }

    return hEvent;
}

BOOL GWICWrapper::UnRegisterCBEvent(const string& sCBID)
{
    TRCBF_POS pos = m_mapRegisteredCBEvent.find(sCBID);
    if (pos != m_mapRegisteredCBEvent.end())
    {
        m_mapRegisteredCBEvent.erase(pos);
        return TRUE;
    }
    return FALSE;
}

HWND GWICWrapper::FindWIC()                                 
{
    return ::FindWindowA(NULL, CHARGE_WND_HANDLE);
}

//
LRESULT GWICWrapper::OnCopyData(WPARAM wParam, LPARAM lParam)
{
    IACT* pACT = NULL;
    pACT = m_pACTMgr->GetACT(0);

    if (pACT && lParam)
    {
        COPYDATASTRUCT *pData= (COPYDATASTRUCT *)lParam;
        pACT->SetCallBackResult((LPCSTR)(pData->lpData));

        //
        if (this->FindRegisteredCBEvent( pACT->GetCBID() ) == NULL)
            m_pACTMgr->CallBack(pACT);
    }
    
    return 0;
}

void GWICWrapper::CheckCBEvent()
{
    if (m_bInitialized != TRUE)
    {
        return;
    }

    IACT* pACT = m_pACTMgr->GetACT(0);

    if (pACT)
    {
        string sCBID  = pACT->GetCBID();
        HANDLE hEvent = FindRegisteredCBEvent(sCBID);
        if (hEvent)
            if (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0)
                TerminateInvoke(hEvent);
    }
}

//
void GWICWrapper::TerminateInvoke(HANDLE hEvent)
{
    IACT* pACT = m_pACTMgr->GetACT(0);

    if (pACT == NULL)
        return;

    string sCBID = FindRegisteredCBID(hEvent);
    if ( sCBID == pACT->GetCBID() )
    {
        m_pACTMgr->CallBack(pACT);
    }
}

#endif //_APPLY_JAPAN_GAMECHU_CHANNELING
