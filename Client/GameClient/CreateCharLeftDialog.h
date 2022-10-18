#pragma once
#include "uiBase.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"

class InterfaceManager;

class CreateCharLeftDialog :
	public uiBase
{
public:

	enum CCL_CONTROL
	{
		CCL_CHAR_NAME = 0,
		CCL_SCROLL,
		CCL_LIST,
		CCL_CHAR_STR,
		CCL_CHAR_DEX,
		CCL_CHAR_CON,
		CCL_CHAR_INT,
		CCL_CHAR_SPR,

		CCL_HAIR_BACKWARD,
		CCL_HAIR_FOWARD,

		CCL_FACE_BACKWARD,
		CCL_FACE_FOWARD,

		CCL_HEIGHT_BACKWARD,
		CCL_HEIGHT_FOWARD,

		CCL_TEXT_HAIR,
		CCL_TEXT_FACE,
		CCL_TEXT_HEIGHT,


		CCL_MAX,
	};
	
	CreateCharLeftDialog(InterfaceManager * pUIman);
	virtual ~CreateCharLeftDialog(void);
public:
	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	OnUpdateSolarDialog();
    virtual void    OnRenderSolarDialog();

	virtual VOID	OnShowWindow( BOOL val );

	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	void    SetStat(BYTE byClass);
	void    GetCharName(TCHAR *szName);

	void    PutItemText(CCtrlListWZ *pList,TCHAR *szText,int iLength);

	void    InitText();

	void    UpdateVariationText();

	void	updateMouseWheel();
    BYTE    GetMouseWheelStatus(const RECT* rc=NULL);

private:
	static	WzID	m_wzId[CCL_MAX];
	FTextParser				m_TextParser;
	FTextSentence			m_TextSentence;
	FTextRender				m_TextRender;

	sScrollInfo				m_VscrInfo;

};
