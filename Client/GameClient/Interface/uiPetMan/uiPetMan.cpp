#include "SunClientPrecompiledHeader.h"

#include "uipetman.h"
#include "InterfaceManager.h"
#include "uiPetNameSetting/uiPetNameSetting.h"
#include "uiPetGauge/uiPetGauge.h"


const DWORD IM_PET_MANAGER::PET_NAME_DLG = StrToWzID("0767");
const DWORD IM_PET_MANAGER::PET_GAUGE_DLG = StrToWzID("0214");

uiPetMan::uiPetMan(InterfaceManager * pUIMAN)
    :uiBaseMan(pUIMAN),
    m_pPetNameSetting(NULL)
{
}

//------------------------------------------------------------------------------
/**
*/
void uiPetMan::OnInitialize()
{
	m_pPetNameSetting = CreateUIDialog<uiPetNameSetting>(IM_PET_MANAGER::PET_NAME_DLG, 
                                                        "Data\\Interface\\76_7_Fetname.iwz", this);
	assert(m_pPetNameSetting);

    uiPetGauge* pet_gauge = CreateUIDialog<uiPetGauge>(IM_PET_MANAGER::PET_GAUGE_DLG, 
                                                        "Data\\Interface\\21_4_HP_pet.iwz", this);
    assert(pet_gauge);

}
