//=======================================================================================================================
/** 소켓 채울것인지 묻는 창
	@author
			유재영
	@since
			
	@remarks
*/

#pragma once

#include "askdialog.h"

class AskSocketFillDialog :
	public AskDialog
{
public:
	enum eENCHANT_PACKET_STATUS 
	{
		NONE		= 0,
		FILL_SOCKET	= 1,
	};

	AskSocketFillDialog(InterfaceManager *pUIMan);
	virtual ~AskSocketFillDialog(void);

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


	VOID			SetInfo(CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos);
	
private:
	CODETYPE			m_ItemCode;
	POSTYPE				m_TargetPos;
	POSTYPE				m_CrystalPos;
	DWORD				m_PacketStatus;
};