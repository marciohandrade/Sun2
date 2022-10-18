#pragma once

#include "uibase.h"

class uiSSQDiceWindow : public uiBase
{

public:
	uiSSQDiceWindow( InterfaceManager * pUIMan );
	~uiSSQDiceWindow( void );

	virtual void Init( CDrawBase * pDrawBase );
	virtual void Release();
	virtual void OnRenderSolarDialog();
	virtual void Process( DWORD dwTick );	
	virtual void OnShowWindow( BOOL val );
	virtual void MessageProc( SI_MESSAGE * pMessage );

	const RECT GetViewRect( void );

private:
	void RenderDice( void );

private:
	CWzUnit *		m_pDiceUnit;
	CWzUnitDraw *	m_pDiceUnitDraw;

	DWORD			m_dwShowTick;
};