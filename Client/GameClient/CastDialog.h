//=======================================================================================================================
/// CastDialog
/**
	@author
			지완
	@since

	@remarks
			-임시 캐스트 타임 표시  		
*/


#pragma once
#include "solardialog.h"

class Object;

class CastDialog : public SolarDialog
{
public:
	enum eCAST_POS
	{
		CAST_PIC_GAUGE = 0, 
		CAST_TEXT = 1,
		

		Dialog_Max
	};

    enum eCAST_TYPE
    {
        DO_Etheria = 0,
        DO_MAX,
    };

public:
	CastDialog(void);
	virtual ~CastDialog(void);

	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );


	VOID SetGage( float fHPPercent );
	VOID SetText(TCHAR* szMessage);

    void SetCastType(BYTE Type) { Cast_Type_ = Type; }
    BYTE GetCastType() { return Cast_Type_; }

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	virtual VOID	OnShowWindow( BOOL val );
	
 
private:
	DWORD			m_dwTargetKEY;
	static WzID	    m_wzId[Dialog_Max];
	
	TCHAR			m_szResultInfo[512];

    float			m_fGaugeSize;
    BYTE             Cast_Type_;
};

