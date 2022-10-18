#pragma once
#include "Const.h"
#include "CharacterDef.h"
#include "PlayerDef.h"
#include "AppearanceManager.h"
#include "SetItemOptionInfoParser.h"
#include "HeroHeightInfoParser.h"

struct BASE_NPCINFO;

class Character;
class EquipmentContainer;

struct IEquipmentRestriction;


class cCharacterBaseData
{
private:
    eCHAR_TYPE m_Class;         // Ŭ���� Ÿ��
    GenderType::Value m_Gender; // ����

    int m_HeightVariation;      // Ű ����
    int m_PlayerVariation[PLAYER_VARIATION_MAX];      // �Ӹ� �� �� ����

    BOOL m_IsHideHelmet;        // ���ǥ�ÿ���
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    BOOL m_IsHideWing;          // ����ǥ�ÿ���
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    BOOL m_IsHideCostume;       // �ڽ�Ƭ ǥ�� ����
    BOOL m_IsHideCostumeToSave; // ����ǥ�����϶� �ӽ�����.
#endif

public:
    eCHAR_TYPE GetClass() { return m_Class; }
    GenderType::Value GetGender() { return m_Gender; }
    int GetHeightVariation() const { return m_HeightVariation; }
    int GetPartVariation(int part_index);

    void SetClass(eCHAR_TYPE character_class) { m_Class = character_class; }
    void SetGender(GenderType::Value gender) { m_Gender = gender; }
    void SetHeightVariation(int variation);
    void SetPartVariation(int part_index, int variation);


    BOOL IsHideHelmet() { return m_IsHideHelmet; }
    void SetHideHelmet(BOOL IsHide) { m_IsHideHelmet = IsHide; }
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    BOOL IsHideWing() { return m_IsHideWing; }
    void SetHideWing(BOOL IsHide) { m_IsHideWing = IsHide; }
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    BOOL isHideCostume() { return m_IsHideCostume; }
    void SetHideCostume(BOOL isHide) { m_IsHideCostume = isHide;}

    BOOL isHideCostumetoAction() { return m_IsHideCostumeToSave; }
    void SetHideCostumetoAction(BOOL isHide) { m_IsHideCostumeToSave = isHide;}
#endif


protected:
    cCharacterBaseData();
};

class cPlayerItemEffect
{
private:
    ITEM_EFFECT_PARAM m_ItemEffectParameter[eITEM_EFFECT_TYPE_MAX];// �������������Ʈ����

    void GetItemEffectParam(int index, ITEM_EFFECT_PARAM& effect_param);
    void SetItemEffectParam(int index, ITEM_EFFECT_PARAM& effect_param);
    void ClearItemEffectParam();
    void ClearItemEffectParam(int index);


public:
    void AddItemEffect( Character* pCharacter, const BASE_ITEMINFO* pItemInfo, eITEM_EFFECT_TYPE EffectType );
    BOOL CreateWasteItemEffect( Character* pCharacter, DWORD ItemCode);
    void RefreshRenderEffectItem( Character* pCharacter, SLOTCODE* ItemCodes, int ItemCount);

    BOOL RemoveItemEffectByItemCode( DWORD ItemCode );

protected:
    cPlayerItemEffect();

    void Destroy();
    void Initialize();

    void AddItemEffect( Character* pCharacter, const BASE_ITEMINFO* pItemInfo );
    BOOL IsExistItemEffect(DWORD ItemCode);

    void UpdateItemEffectPos( Character* pCharacter );
    void RemoveItemEffect(eITEM_EFFECT_TYPE EffectType );


};

class cPlayerSetItemEffect
{
private:
    sFullSetEffectInfo m_FullsetEffectInfo[MAX_SETITEM_EFFECT_NUM]; // Ǯ������Ʈ��������
    sFullSetChangeItemInfo m_FullsetChangeItemInfo;                 // Ǯ��ü��������������

    std::vector<sAttachEffectForSetItem> m_FullsetEffectAttachData; // Ǯ������Ʈ��������

    BOOL m_ShowFullsetEffect;

public:

    void ProcessFullsetEffect( Character* pCharacter);
    void RefreshFullsetItemInfo( Character* pCharacter );
    void ReleaseFullsetEffect();
    void ShowFullsetEffect(BOOL IsShow);

private:
    void GetFullsetEffectInfo(int index, OUT sFullSetEffectInfo& effect_info);
    void SetFullsetEffectInfo(int index, IN const sFullSetEffectInfo& effect_info);

    void ClearFullsetEffectInfo();

    bool IsSetitemEquipmentSlot( Character* pCharacter, int equipment_slot_index);
    eKIND_OF_EQUIPED_SETITEM CheckKindOfSetItem(Character* pCharacter, OUT WORD& setitem_optioninfo_setgroupcode);

    void ChangeFullsetResource( Character* pCharacter, WORD setitem_optioninfo_setgroupcode );
    void RestoreFullsetResource( Character* pCharacter );

    void RefreshFullsetEffect( Character* pCharacter, eKIND_OF_EQUIPED_SETITEM kind_of_equiped_setitem, WORD setitem_optioninfo_setgroupcode);

    BOOL GetAttachEffectForSetItemPos( Character* pCharacter, OUT WzVector& Attach_Position);

protected:
    cPlayerSetItemEffect();

    void RefreshFullsetEffect( Character* pCharacter );
};

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
class cPlayerFPFullEffect
{
    enum FP_FULL_EFFECT_H
    {
        LEFT_FP_FULL_EFFECT_H,
        RIGHT_FP_FULL_EFFECT_H,
        MAX_FP_FULL_EFFECT_H,
    };
public:
    void CreateFPFullEffect(Character* character_ptr);
    void ProcessFPFullEffect(Character* character_ptr);
    void DestroyFPFullEffect();

private:
    HANDLE fp_full_effect_handle_[MAX_FP_FULL_EFFECT_H];
    
protected:
    cPlayerFPFullEffect();
    ~cPlayerFPFullEffect();
};
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
