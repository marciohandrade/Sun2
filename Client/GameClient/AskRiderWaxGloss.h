#pragma once

#include "askdialog.h"

class AskRiderWaxGloss:
	public AskDialog
{
public:
	AskRiderWaxGloss(InterfaceManager *pUIMan);
	virtual ~AskRiderWaxGloss(void);

	virtual VOID    Process(DWORD dwTick);
	VOID			Initialize();
    VOID			SetRiderInfo(CODETYPE ItemCode, POSTYPE RiderPartsPos, POSTYPE WaxItemPos);

protected:
	virtual void	OnUpdateSolarDialog();
	virtual void    MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	OnShowWindow( BOOL val );

    VOID			SetTexts();
	VOID			DoIt();
	VOID			Cancel();
	
private:
	CODETYPE		m_ItemCode;
    POSTYPE			m_RiderPartsPos;
    POSTYPE			m_WaxItemPos;

};
