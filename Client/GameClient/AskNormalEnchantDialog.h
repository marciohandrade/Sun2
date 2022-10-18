//------------------------------------------------------------------------------
/**
    (하|중)급 인첸트 할것인지 물어보는 창.
    @author
    -유재영
    @since
    -2005.07.29 
    @remarks
*/

#pragma once
#include "askdialog.h"


class AskNormalEnchantDialog :
	public AskDialog
{
public:
	enum eENCHANT_PACKET_STATUS 
	{
		NONE	= 0,
		ENCHANT	= 1,
	};

	AskNormalEnchantDialog(InterfaceManager *pUIMan);
	virtual ~AskNormalEnchantDialog(void);


    virtual VOID    Process(DWORD dwTick);

protected:
    virtual void	OnUpdateSolarDialog();
    virtual void    MessageProc( SI_MESSAGE * pMessage );
    virtual VOID	OnShowWindow( BOOL val );

	VOID			SetTexts();

public:
	VOID			Enchant();
	VOID			Cancel();

	VOID			SetEnchantInfo(CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos);
	VOID			SetEnchantType(eNormalEnchantType eType);

private:
	CODETYPE			m_EnchantItemCode;
	eNormalEnchantType	m_eNormalEnchantType;
	POSTYPE				m_TargetPos;
	POSTYPE				m_CrystalPos;
	DWORD				m_PacketEnchantStatus;
};
