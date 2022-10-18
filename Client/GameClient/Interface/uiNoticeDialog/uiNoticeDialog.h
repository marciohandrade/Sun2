#pragma once
#include "uibase.h"
#include "FTextRender.h"

class uiNoticeMan;

class uiNoticeDialog: public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_NOTICE_TEXT,
		DLGPOS_CLOSE_BTN,

		DIALOG_MAX,
	};

public:
	uiNoticeDialog(InterfaceManager *pUIMan);
	virtual ~uiNoticeDialog();

public:
	virtual void Init(CDrawBase* pDrawBase);
    virtual void Release();

	bool ShowNoticeMessage(const TCHAR* pNoticeMessage, int DisplayTime);
	virtual void OnShowWindow( BOOL val );
	virtual void MessageProc( SI_MESSAGE * pMessage );

private:
	virtual void OnRenderSolarDialog();
	virtual void OnUpdateSolarDialog();

private:
	uiNoticeMan* m_pNoticeMan;

	static WzID m_wzId[DIALOG_MAX];
	TCHAR m_szNoticeMessage[MAX_NOTICE_LEN+1];
	CCtrlStaticWZ* m_pNoticeTextControl;
	CCtrlButtonWZ* m_pCloseButton;

	util::Timer m_Timer;
	FTextRender m_FtextRender;
	
	DWORD m_dwNoticeTime;
	float m_fFadeValue;
	bool m_bCloseForNextMessage;
};

