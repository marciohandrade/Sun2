#pragma once
#include "askdialog.h"

class AskNPCPortal :
	public AskDialog
{
public:
	AskNPCPortal(InterfaceManager *pUIMan);
	virtual ~AskNPCPortal(void);

	enum eNPC_PORTAL_PACKET_STATUS 
	{
		NONE		= 0,
		MOVE_PORTAL	= 1,
	};
  
	VOID			Initialize();

public:
	VOID			SetMoveVillage( MAPCODE MapCode, MONEY fare);

protected:
    virtual void	OnUpdateSolarDialog();
    virtual void    MessageProc( SI_MESSAGE * pMessage );
    virtual VOID	OnShowWindow( BOOL val );

	VOID			SetTexts();

	VOID			DoIt();
	VOID			Cancel();

private:
	MAPCODE			m_MapCode;
	MONEY			m_Fare;


};
