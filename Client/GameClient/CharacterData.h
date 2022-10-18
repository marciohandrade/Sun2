#pragma once

#include "CharacterVariation.h"

class cCharacterData
    : public cCharacterBaseData
    , public cPlayerItemEffect
    , public cPlayerSetItemEffect
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    , public cPlayerFPFullEffect
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
{
private:
    EquipmentContainer* m_EquipmentContainer;       // 이큅컨테이너
    bool m_RealEquipmentFlag[MAX_EQUIPMENT_SLOT_NUM];  // (활성화된)장비착용플래그

    HANDLE m_DestroyShieldEffectHandle;                  // 쉴드파괴이펙트

    bool m_IsInitialized;

protected:
    cCharacterData();
    virtual ~cCharacterData();

public:


    eCHAR_TYPE GetImageClass();
    DWORD GetClassCode();

    bool GetRealEquipFlag(int index) const;
    void SetRealEquipFlag(int index, bool equip_flag);

    EquipmentContainer* GetEquipmentContainer() const { return m_EquipmentContainer; }

protected:
    void Initialize();
    void Release();

    bool IsCharacterDataCreated() { return m_IsInitialized; }

    void SetEquipmentContainer(EquipmentContainer* equip_container) { m_EquipmentContainer = equip_container; }

    float GetHeightVariationValue();

    HANDLE GetDestroyShieldEffect() { return m_DestroyShieldEffectHandle; }
    void SetDestroyShieldEffect(HANDLE effect_handle) { m_DestroyShieldEffectHandle = effect_handle; }
};