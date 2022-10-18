#include "SunClientPrecompiledHeader.h"
#include "uiherogaugeman.h"
#include "interfacemanager.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#include "HeroGaugeDialog2.h"
#include "HeroGaugeDialog2Control.h"
#else
#include "HeroGaugeDialog.h"
#endif


const DWORD IM_HERO_GAUGE_MANAGER::HERO_GAUGE_DLG	= StrToWzID("0210");
const DWORD IM_HERO_GAUGE_MANAGER::MAGAZINE__DLG	= StrToWzID("0291");
const DWORD IM_HERO_GAUGE_MANAGER::HERO_GAUGE_DLGCONTROL	= StrToWzID("0211");

uiHeroGaugeMan::uiHeroGaugeMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pHGauge = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void	uiHeroGaugeMan::OnInitialize()
{
#ifdef _NA_000000_20130114_RENEWER_UI
	 m_pHGauge = new HeroGaugeDialog2;
	 m_pHGauge_Control = new HeroGaugeDialog2Control;
#else
	 m_pHGauge = new HeroGaugeDialog;
#endif
   

    if( !g_InterfaceManager.LoadInterface( 
        m_pHGauge, 
        "Data\\Interface\\21_HP.iwz", 
        StrToWzID("0210"),
        FALSE ) )
    {

    }

    m_pHGauge->SetDialogResourceName( "Data\\Interface\\21_HP.iwz" );
    m_pHGauge->SetDialogKey( StrToWzID("0210") );

#ifdef _NA_000000_20130114_RENEWER_UI
	if( !g_InterfaceManager.LoadInterface( 
		m_pHGauge_Control, 
		"Data\\Interface\\21_1_Level.iwz", 
		StrToWzID("0211"), 
		FALSE ) )
	{

	}

	m_pHGauge_Control->SetDialogResourceName( "Data\\Interface\\21_1_Level.iwz" );
	m_pHGauge_Control->SetDialogKey( StrToWzID("0211") );

	m_pHGauge->SetParentdialog(m_pHGauge_Control);
#endif

}
