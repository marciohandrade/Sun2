#pragma once
#include "solardialog.h"


enum eTOOLTIP_TYPE
{
	TOOLTIP_TYPE_ITEM = 0,
	TOOLTIP_TYPE_ETC,

	MAX_TOOLTIP_TYPE
};

struct TOOLTIP_INFO
{
	eTOOLTIP_TYPE eToolTipType;

	union 
	{
		struct // ITEM
		{
			DWORD dwItemCode;
		};

		struct // ETC
		{
			TCHAR * szInfo;
		};
	};
};


class TooltipDialog :
	public SolarDialog
{
public:
	enum eTOOLTIP_POS
	{
		TOOLTIP_LIST,
		Dialog_Max
	};

public:
	TooltipDialog(void);
	virtual ~TooltipDialog(void);

	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	RECT			CalcClipRect(int x, int y);

	VOID			ShowToolTipInfo(int x, int y, const TOOLTIP_INFO & info);
	VOID			HideToolTipInfo();


protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	VOID			_showItemToolTip(DWORD dwItemCode);
	VOID			_showETCToolTip(TCHAR * szInfo);

private:		
	static WzID	    m_wzId[Dialog_Max];
};

