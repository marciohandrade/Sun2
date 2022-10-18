

#ifndef     __UIPASSWORDMAN_H__
#define     __UIPASSWORDMAN_H__
//------------------------------------------------------------------------------
//
//    작업자: 김태우
//    uiPasswordMan class
//------------------------------------------------------------------------------

#include "uiBaseMan.h"
#include "uiPassWordDef.h"

typedef void (*fn_Password)( bool bYes );

class uiPasswordDlg;

class uiPasswordMan : public uiBaseMan
{
public:
	uiPasswordMan(InterfaceManager *pUIMan);

	//--------------------------------------------------------------------------
	// init/release
	//
public:
    virtual void            OnInitialize();

	//--------------------------------------------------------------------------
	// procedure
	//
public:
	BOOL                    PasswordDlg(int iStringTableIndex, fn_Password _fn_password, int iMinCount, int iMaxCount, BOOL bShowPassChgBtn = FALSE, int iCalled = 0 );

	uiPasswordDlg*			m_pUIPasswordDlg;
	//--------------------------------------------------------------------------
	// member vars
	//
private:

    void                    _LoadUI(UINT uID);
};

//------------------------------------------------------------------------------
#endif  //  __UIPASSWORDMAN_H__

