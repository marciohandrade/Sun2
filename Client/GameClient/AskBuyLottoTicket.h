#pragma once
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
#include "askdialog.h"

class AskBuyLottoTicket :
	public AskDialog
{
public:

	AskBuyLottoTicket(InterfaceManager *pUIMan);
	virtual ~AskBuyLottoTicket(void);

	virtual VOID    Process(DWORD dwTick);

	VOID			Initialize();
    VOID			SetInfo(BOOL bAuto){bAutoNumber = bAuto;}

protected:
	virtual void	OnUpdateSolarDialog();
	virtual void    MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	OnShowWindow( BOOL val );

	VOID			SetTexts();

	VOID			DoIt();
	VOID			Cancel();
private:
    BOOL            bAutoNumber;

};
#endif//_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE