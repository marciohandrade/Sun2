#pragma once
#include "uiBaseMan.h"

struct IM_HERO_GAUGE_MANAGER
{
	static const DWORD HERO_GAUGE_DLG;
	static const DWORD MAGAZINE__DLG;
	static const DWORD HERO_GAUGE_DLGCONTROL;
};


#ifdef _NA_000000_20130114_RENEWER_UI
	class HeroGaugeDialog2;
	class HeroGaugeDialog2Control;
#else
	class HeroGaugeDialog;
#endif

class uiHeroGaugeMan : public uiBaseMan
{
public:
	uiHeroGaugeMan(InterfaceManager *pUIMan);

public:
    virtual void            OnInitialize();
    
private:
#ifdef _NA_000000_20130114_RENEWER_UI
	HeroGaugeDialog2 *		m_pHGauge;
	HeroGaugeDialog2Control* m_pHGauge_Control;
#else
	HeroGaugeDialog *		m_pHGauge;
#endif
};
