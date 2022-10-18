#pragma once

#include "askdialog.h"

class AskEtherDeviceDialog :
	public AskDialog
{
public:
	enum eENCHANT_PACKET_STATUS 
	{
		NONE		= 0,
		FILL_SOCKET	= 1,
	};

	AskEtherDeviceDialog(InterfaceManager *pUIMan);
	virtual ~AskEtherDeviceDialog(void);

	virtual VOID    Process(DWORD dwTick);

	VOID			Initialize();

protected:
	virtual void	OnUpdateSolarDialog();
	virtual void    MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	OnShowWindow( BOOL val );

	VOID			SetTexts();

	VOID			DoIt();
	VOID			Cancel();

public:


	VOID			SetInfo(CODETYPE ItemCode, POSTYPE WeaponPos, POSTYPE DeviceItemPos);

private:
	CODETYPE			m_ItemCode;
	POSTYPE				m_WeaponPos;
	POSTYPE				m_DeviceItemPos;

};
