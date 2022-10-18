#pragma once
#include "solardialog.h"
#include "LimitedMapPortParser.h"

class NPCPortalDialog : public SolarDialog
{
public:
	NPCPortalDialog(void);
	virtual ~NPCPortalDialog(void);

	// 기본 enum
	enum ePORTAL_POS
	{
		PORTAL_TITLE = 0,
		PORTAL_LIST,
		PORTAL_BTN_EXIT,
		PORTAL_BTN_OK,
		PORTAL_BTN_CANCEL,
		Dialog_Max
	};

	enum ePACKET_STATUS
	{
		NONE			= 0,
		eMOVE_VILLAGE	= 1,
	};

	// 기본 virtual methods
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );

	virtual VOID	OnShowWindow(BOOL val);

	//virtual void    UpdatePosition();

public:
	VOID			SetCurVillageCode( CODETYPE NPCCode, MAPCODE MapCode );
	VOID			RegisterPoralList();

	VOID			SEND_CG_ZONE_VILLAGE_MOVE_SYN(MAPCODE MapCode, MONEY Fare);

    MONEY           GetPendingMoney();
    void            ClearPendingMoney();

	// 기본 proected methods
protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

protected:
	VOID			BtnClick( POSTYPE pos );
	VOID			RefreshPortalList();
	VOID			popupAskPortal(MAPCODE MapCode, MONEY Fare);


private:
	CCtrlListWZ *				m_pListCtrl;
	CCtrlStaticWZ *				m_pTitleCtrl;
	const sMAPINFO *					m_pVillageMapInfo;

	std::vector<_sVILLAGEPORT>	m_vecPoralList;
	MAPCODE						m_PendingMapCode;
	MONEY						m_PendingFare;

	// 기본 attributes
private:
	static WzID				m_wzId[Dialog_Max];
};

//------------------------------------------------------------------------------
/**
*/
inline
MONEY
NPCPortalDialog::GetPendingMoney()
{
    return m_PendingFare;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
NPCPortalDialog::ClearPendingMoney()
{
    m_PendingFare = 0;
}
