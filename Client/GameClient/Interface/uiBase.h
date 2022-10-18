#ifndef     __USER_INTERFACE_BASE_H__
#define     __USER_INTERFACE_BASE_H__
//------------------------------------------------------------------------------
/**
    @class uiBase

    User Interface Abstract Data Type

    (C) 2005 ckbang
*/
#include "uiBase_def.h"
#include "SolarDialog.h"

class uiBaseMan;
class SCSlotContainer;

#define SendMesaageUI(ParentUIHandle,messageID,lParam,wParam) ParentUIHandle->GetMessage(messageID,lParam,wParam)

class uiBase : public SolarDialog
{
private:
    struct ResourceInfo
    {
        char    m_szDialogResName[MAX_PATH];
        DWORD   m_dwDialogKey;
    };

    ResourceInfo m_ResourceInfo;

public:
    const char* GetDialogResourceName() { return m_ResourceInfo.m_szDialogResName; }
    DWORD GetDialogKey() { return m_ResourceInfo.m_dwDialogKey; }

    void SetDialogKey( DWORD dialog_key ) { m_ResourceInfo.m_dwDialogKey = dialog_key; }
    void SetDialogResourceName( const char* resource_name ) { StrnCopy( m_ResourceInfo.m_szDialogResName, resource_name, MAX_PATH ); }


public:
    uiBase(InterfaceManager * pUIMan = &g_InterfaceManager );
    virtual ~uiBase();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual VOID            Init(CDrawBase* pDrawBase);
    virtual VOID            Process(DWORD /*dwTick*/) {};
    virtual VOID            Release();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    InterfaceManager*      GetUIMan() { return &g_InterfaceManager; }
    CDrawBase*             GetDrawBase() { return g_pSunRenderer; }

    virtual SCSlotContainer * GetSlotContainer() {return NULL;}

    /// interface show/hide
    virtual bool            ShowInterface( BOOL val, BOOL showOff=FALSE );
	bool                    ShowInterfaceWithSound( BOOL val, BOOL showOff=FALSE );

    // network pending condition
    void                    OnPending();
    void                    OffPending();
    bool                    IsPending();

    void                    SetMan(uiBaseMan* baseMan);
    uiBaseMan*              GetMan();
	bool                    LoadInterface();
	BOOL					CloneInterface();
	void					SetClone( BOOL b ) { m_bClone = b; }
	BOOL					IsClone( void ) { return m_bClone; }

    virtual bool IsOverControl(CControlWZ* control);

private:

    //--------------------------------------------------------------------------
    // functions
    //
public:
    static BYTE GetMouseWheelStatus(const RECT* rc = NULL);

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
    // dialog lock
    void                    OnLock();
    void                    OffLock();
    bool                    IsLock();

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );         // Pure
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
	virtual void			GetMessage(DWORD messageID,LPARAM lParam,WPARAM wParam) {}

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiBaseMan*              m_pMan;
    CDrawBase *				m_pDraw;
  
    // safe-guard
    bool                    m_isLock;
    bool                    m_isPending;

    // resource check
	BOOL					m_bClone;
	
};

//------------------------------------------------------------------------------
/**
*/
inline
void
uiBase::SetMan(uiBaseMan* baseMan)
{
    assert (baseMan);
    m_pMan = baseMan;
}

//------------------------------------------------------------------------------
/**
*/
inline
uiBaseMan *
uiBase::GetMan()
{
    assert (m_pMan);
    return m_pMan;
}


//------------------------------------------------------------------------------
/**
*/
inline
void
uiBase::OnPending()
{
    this->m_isPending = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiBase::OffPending()
{
    this->m_isPending = false;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiBase::IsPending()
{
    return this->m_isPending;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiBase::OnLock()
{
    this->m_isLock = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiBase::OffLock()
{
    this->m_isLock = false;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiBase::IsLock()
{
    return this->m_isLock;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_BASE_H__
