#pragma once
#include "uiBaseMan.h"

class uiPetNameSetting;
class InterfaceManager;

struct IM_PET_MANAGER
{
	static const DWORD PET_NAME_DLG;			
    static const DWORD PET_GAUGE_DLG;
};

class uiPetMan : public uiBaseMan
{
public:
	uiPetMan(InterfaceManager* pUIMAN);

public:
	virtual void            OnInitialize();
		
private:
	uiPetNameSetting*		m_pPetNameSetting;
};
