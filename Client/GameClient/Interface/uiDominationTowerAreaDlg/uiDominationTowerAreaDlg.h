#pragma once
#include "uibase.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"


class uiDominationTowerAreaDlg: public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_GUILDNAME_TEXT,		
		DLGPOS_GUILDMARK_PIC,

		DIALOG_MAX,
	};

public:
	uiDominationTowerAreaDlg(InterfaceManager *pUIMan);
	virtual ~uiDominationTowerAreaDlg();

public:
	virtual void Init(CDrawBase* pDrawBase);
	virtual void OnShowWindow(BOOL val);
	virtual void OnRenderSolarDialog();

	bool CheckShowDialog();

	// TODO:투명도 세팅함수작성 - 그에 따른 로직 작성
	void SetTransparency(BYTE bAlpha)
	{
		m_bAlpha = bAlpha;
		GetDialogWZ()->SetTransparency((float)m_bAlpha/255.0f);
	}
	void SetGuildName(TCHAR* pGuildName);

private:

private:
	static WzID m_wzId[DIALOG_MAX];
	CCtrlStaticWZ* m_pGuildNameText;	
	CCtrlPictureWZ* m_pGuildMarkPic;	

	BYTE m_bAlpha;
};
