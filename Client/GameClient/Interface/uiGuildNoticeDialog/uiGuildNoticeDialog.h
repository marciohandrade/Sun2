#pragma once
#include "uibase.h"
#include "FTextRender.h"

class uiNoticeMan;

class uiGuildNoticeDialog: public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_NOTICE_TEXT,
		DLGPOS_GUILDMARK_PIC,
		DLGPOS_CLOSE_BTN,

		DIALOG_MAX,
	};

public:
	uiGuildNoticeDialog(InterfaceManager *pUIMan);
	virtual ~uiGuildNoticeDialog();

public:
	virtual void Init(CDrawBase* pDrawBase);
    virtual void Release();

	virtual void OnShowWindow( BOOL val );
	virtual void MessageProc( SI_MESSAGE * pMessage );

	bool ShowNoticeMessage(const TCHAR* pNoticeMessage, int DisplayTime);
	
private:
	virtual void OnRenderSolarDialog();
	virtual void OnUpdateSolarDialog();

private:
	uiNoticeMan* m_pNoticeMan;
	 
	static WzID m_wzId[DIALOG_MAX];
	TCHAR m_szNoticeMessage[MAX_MISSION_MESSAGE_LENGTH+1];
	CCtrlStaticWZ* m_pNoticeTextControl;
	CCtrlPictureWZ* m_pGuildMarkPicControl;
	CCtrlButtonWZ* m_pCloseButton;

	util::Timer m_Timer;
	FTextRender	m_FtextRender;
	DWORD m_dwNoticeTime;

	int nDisplayTime;
	float m_fFadeValue;
	bool m_bCloseForNextMessage;
};
