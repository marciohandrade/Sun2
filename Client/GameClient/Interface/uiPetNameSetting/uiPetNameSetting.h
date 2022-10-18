#pragma once

#include "uibase.h"

class uiPetMan;
      
class uiPetNameSetting : public uiBase
{
	enum ePET_NAME_POS
	{
		PET_NAME_EDIT=0,
		PET_NAME_BTN_OK,
		PET_NAME_BTN_CANCLE,
		PET_NAME_BTN_EXIT,
		PET_NAME_MAX, 
	};

	enum
	{
		PET_BTN_MAX = PET_NAME_BTN_EXIT - PET_NAME_BTN_OK + 1,
	};


	
public:
	
	uiPetNameSetting(InterfaceManager *pUIMan);
	virtual ~uiPetNameSetting(void);
	
	virtual VOID		Init( CDrawBase * pDrawBase );
	virtual VOID		Release();

	virtual VOID		MessageProc( SI_MESSAGE * pMessage );
	virtual void	    OnShowWindow( BOOL val );
	void				SetLockNameCard(POSTYPE pos,BOOL bLock=TRUE);
	void				SetPetName();

private:
	CControlWZ *		getControl( POSTYPE AtPos );
	POSTYPE				getCtrlIDToPos( WzID wzId );
	void				SendPetName();
	void				PetNameError(int iStringCode);
	void				InitControls();

private:
	BOOL				m_bInitDlg;							//dlg는 최초 OnShowWindow(0)한번호출된다 
	POSTYPE				m_InvenPos;
	BOOL				m_bWaitPacket;

	TCHAR				m_PetName[MAX_PET_NAME_LENGTH+1];
	uiPetMan*			m_pPetMan;
	static WzID			m_wzId[PET_NAME_MAX];
	CCtrlButtonWZ*		m_pPetBtnCtrl[PET_BTN_MAX];
	CCtrlDxIMEEditBox*	m_pNameEditCtrl;
};
//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiPetNameSetting::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < PET_NAME_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return PET_NAME_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiPetNameSetting::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < PET_NAME_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}
