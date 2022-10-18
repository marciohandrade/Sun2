//=======================================================================================================================
/** 소켓 채울것인지 묻는 창
	@author
			유재영
	@since
			
	@remarks
*/

#pragma once
#ifndef _ASK_EXTRASTONE_USE_H_
#define _ASK_EXTRASTONE_USE_H_

#include "askdialog.h"

class AskExtraStoneUse :
	public AskDialog
{
public:
	enum eEXTRASTONE_PACKET_STATUS 
	{
		NONE		= 0,
		USE_EXTRASTONE	= 1,
	};

	AskExtraStoneUse(InterfaceManager *pUIMan);
	virtual ~AskExtraStoneUse(void);

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

#endif //_ASK_EXTRASTONE_USE_H_