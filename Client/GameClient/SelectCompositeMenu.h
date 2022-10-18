//=======================================================================================================================
/** SelectCompositeMenu
	@author
			유재영
	@desc

	@since
			
	@remarks
*/

#ifndef	_SELECTCOMPOSITEMENU_H_
#define _SELECTCOMPOSITEMENU_H_

#include "solardialog.h"

struct sSelectComposeInfo
{
	eITEMCOMPOSITECODE	eCompositeType;
	DWORD				dwStrID;
};



class SelectCompositeMenu : public SolarDialog
{
public:
	SelectCompositeMenu(void);
	virtual ~SelectCompositeMenu(void);

	// 기본 enum
	enum eSELECTMENU_POS
	{
		eSELECTMENU_SELECT1,
		eSELECTMENU_SELECT2,
		eSELECTMENU_SELECT3,
		eSELECTMENU_SELECT4,	
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#else
		eSELECTMENU_SELECT5,	
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		eSELECTMENU_BUTTON_EXIT,	
        eSELECTMENU_PICTURE_BACK,	

		Dialog_Max
	};

	enum
	{
		BEGIN_SELECT = eSELECTMENU_SELECT1,
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		MAX_SELECT	 = eSELECTMENU_SELECT4 + 1 - eSELECTMENU_SELECT1,
#else
		MAX_SELECT	 = eSELECTMENU_SELECT5 + 1 - eSELECTMENU_SELECT1,
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	};

	// 기본 virtual methods
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );

	virtual VOID	OnShowWindow(BOOL val);
	VOID			SetOpenNPC(CODETYPE code);

	void			SetDialogKey( DWORD DialogKey ) { m_DialogKey = DialogKey; }
	DWORD			GetDialogKey() { return m_DialogKey; }

	// 기본 proected methods
protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

protected:
	VOID			initCompositeMenuInfo();
	VOID			btnClickSelectMenu(POSTYPE pos);

	// 기본 attributes
private:
	DWORD							m_DialogKey;
	static WzID						m_wzId[Dialog_Max];

	TCHAR							m_szBuf[INTERFACE_STRING_LENGTH];

	CCtrlButtonWZ *					m_pBtnSelectMenu[MAX_SELECT];
	CCtrlStaticWZ *					m_pStaticHighOrLow[4];
	CCtrlStaticWZ *					m_pStaticKindOfComposite[4];
	CODETYPE						m_NPCCode;
	std::vector<sSelectComposeInfo> m_VecSelectComposeInfo;
};

SolarDialog * OpenCompositeFoward(eITEMCOMPOSITECODE eCode);

#endif
// _SELECTCOMPOSITEMENU_H_