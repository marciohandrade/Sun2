#pragma once

//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiWebDialogMan class
//------------------------------------------------------------------------------

#include "uiWebDialogMan_def.h"
#include "uiBaseMan.h"
#ifdef _KIKI_UI_WEB_DIALOG

class uiWebDialog;

class uiWebDialogMan : public uiBaseMan
{
public:
    uiWebDialogMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
	virtual void			OnInitialize();

private:
    BOOL                    _LoadUI();

    //--------------------------------------------------------------------------
    // functions
    //
public:
	void					ToggleWebDialog( void );

#ifdef _KIKI_WEB_AUTH	
	void					RequestLogin(TCHAR* pID, char* pszGLogKey);
#endif

	void					RequestUserID(TCHAR* pCharName);
	void					RequestGLOG();
	void					RequestBLOG(TCHAR* pCharID);

private:
	void					_loadURL();

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );
	virtual void            NetworkProcWorld( MSG_BASE * pMsg );

			void			SetURL( WCHAR * pURL );
			void			SetURL( char * pURL );
	inline	WCHAR *			GetURL( void )				{ return m_pURL; }
    
	
    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiWebDialog*			m_pUIWebDialog;
	WCHAR					m_pURL[MAX_PATH];
	WCHAR					m_pLoginURL[MAX_PATH];
	
	TCHAR					m_szAuthURL[MAX_PATH];
	TCHAR					m_szUserURL[MAX_PATH];
	WCHAR					m_pAuthURL[MAX_PATH];
	WCHAR					m_pGLOGURL[MAX_PATH];
	WCHAR					m_pUserURL[MAX_PATH];

	//BOOL					m_bFirst;
};

#endif