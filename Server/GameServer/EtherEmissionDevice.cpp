#include "StdAfx.h"
#include "EtherEmissionDevice.h"

#include "ItemManager.h"
#include "Skill.h"
#include "SkillInfoParser.h"
#include "SCItemSlotContainer.h"
#include "SkillAttrCalculator.h"


CEtherEmissionDevice::CEtherEmissionDevice() : 
    is_active_(false), owner_(NULL)
{
}

CEtherEmissionDevice::~CEtherEmissionDevice()
{
    is_active_ = false;
    owner_ = NULL;
    device_info_.Init();
}

void CEtherEmissionDevice::Init(Player* const owner)
{
    is_active_ = false;
    owner_ = owner;	
    device_info_.Init();
}

bool CEtherEmissionDevice::ActiveBulletItem(const POSTYPE bullet_pos)
{
    Player* const owner = GetOwner();
    if (owner == NULL)
    {
        return false;
    }

    if (is_active_)
    {
        return false;
    }
    
    const ItemManager* const item_manager = owner->GetItemManager();
    if (item_manager == NULL)
    {
        return false;
    }    

    SCSlotContainer* const inven_container = item_manager->GetItemSlotContainer(SI_INVENTORY);
    if (inven_container == NULL)
    {
        return false;
    }

    SCSlotContainer* const equip_container = item_manager->GetItemSlotContainer(SI_EQUIPMENT);
    if (equip_container == NULL)
    {
        return false;
    }
    
    if (item_manager->ValidPos(SI_INVENTORY, bullet_pos) == false)
    {
        return false;
    }
        
    SCItemSlot& bullet_slot = (SCItemSlot&)inven_container->GetSlot(bullet_pos);
    if (bullet_slot.GetItemInfo() == NULL)
    {
        return false;
    }
    
    SCItemSlot& weapon_slot = (SCItemSlot&)equip_container->GetSlot(eEQUIPCONTAINER_WEAPON);
    if (weapon_slot.GetItemInfo() == NULL)
    {
        return false;
    }
        
    if (bullet_slot.GetItemInfo()->m_wType != eITEMTYPE_ETHER_BULLET || bullet_slot.GetNum() == 0)
    {
        return false;
    }
        
    if (weapon_slot.IsEquipEtherDevice() == false)
    {
        return false;
    }
        
    if ((!bullet_slot.GetLaxLock()->CanLock()) || (!weapon_slot.GetLaxLock()->CanLock()))
    {
        return false;
    }
        
    device_info_.Init();
    if (SetBulletOption(bullet_slot) == false)
    {
        return false;
    }
    device_info_.bullet_slot_pos = bullet_pos;
    device_info_.bullet_slot = &bullet_slot;
    device_info_.weapon_slot = &weapon_slot;
    is_active_ = true;

    bullet_slot.GetLaxLock()->EnterLock();
    weapon_slot.GetLaxLock()->EnterLock();

    owner->GetCharInfo()->m_bActiveEtherDevice = TRUE;
    owner->GetCharInfo()->m_byEtherBulletPos = bullet_pos;

    return true;
}

bool CEtherEmissionDevice::InActiveBulletItem(const POSTYPE bullet_pos, const bool is_force)
{
    Player* const owner = GetOwner();
    if (owner == NULL)
    {
        return false;
    }

    if (is_force == false)
    {
        if (is_active_ == false)
        {
            return false;
        }
            
        if (device_info_.bullet_slot_pos != bullet_pos)
        {
            return false;
        }
            
        if (device_info_.apply_attack_speed != 0) 
        {
            RemoveAttackSpeed();
        }

        if (device_info_.is_apply_option)
        {
            return false;
        }
    }
    
    if (device_info_.bullet_slot != NULL)
    {
        (device_info_.bullet_slot)->GetLaxLock()->LeaveLock();
    }    
    if (device_info_.weapon_slot != NULL)
    {
        (device_info_.weapon_slot)->GetLaxLock()->LeaveLock();	
    }
        
    device_info_.Init();
    is_active_ = false;
    
    owner->GetCharInfo()->m_bActiveEtherDevice = FALSE;
    owner->GetCharInfo()->m_byEtherBulletPos = INVALID_POSTYPE_VALUE;

    return true;
}

bool CEtherEmissionDevice::SetBulletOption(const SCItemSlot& bullet_slot)
{
    const Player* const owner = GetOwner();
    if (owner == NULL)
    {
        return false;
    }
    if (owner->GetItemManager() == NULL)
    {
        return false;
    }
    
    device_info_.bullet_info = 
        EtherBulletInfoParser::Instance()->GetEtherBulletInfoByItemCode(bullet_slot.GetItemCode());
    if (device_info_.bullet_info == NULL)
    {
        return false;
    }
        
    return true;
}

tagEtherBulletInfo*	CEtherEmissionDevice::GetBulletOption() const
{
    if (IsActive() == false)
    {
        return NULL;
    }
    return device_info_.bullet_info;
}

void CEtherEmissionDevice::ApplyOption()
{
    Player* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    if (device_info_.is_apply_option)
    {
        return;
    }

    int combo_index = device_info_.combo_state - 1;
    if (device_info_.combo_state == tagEtherBulletInfo::eETHER_DEVICE_COMBO_NORMAL)
    {
        combo_index = tagEtherBulletInfo::eETHER_DEVICE_COMBO_NORMAL;
    }
    
    SkillAttrCalculator skill_calc(owner->GetAttr());
    for (int i = 0; i < MAX_BULLET_OPTION_COUNT; ++i)
    {
        const tagEtherBulletInfo* const bullet_info = device_info_.bullet_info;
        if (bullet_info == NULL)
        {
            continue;
        }

        if (bullet_info->m_EtherBulletOption[combo_index].m_wOptionKind[i] != 0 && 
            bullet_info->m_EtherBulletOption[combo_index].m_iValue[i] != 0 &&
            bullet_info->m_EtherBulletOption[combo_index].m_wOptionKind[i] < _countof(g_ITEM_OPTION))
        {
            skill_calc.AddAttr(
                g_ITEM_OPTION[bullet_info->m_EtherBulletOption[combo_index].m_wOptionKind[i]],
                bullet_info->m_EtherBulletOption[combo_index].m_byValueType[i], 
                bullet_info->m_EtherBulletOption[combo_index].m_iValue[i]
            );
        }            
    }

    device_info_.is_apply_option = true;
}

void CEtherEmissionDevice::RemoveOption()
{
    Player* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    int combo_index = device_info_.combo_state - 1;
    if (device_info_.combo_state == tagEtherBulletInfo::eETHER_DEVICE_COMBO_NORMAL)
    {
        combo_index = tagEtherBulletInfo::eETHER_DEVICE_COMBO_NORMAL;
    }
    
    SkillAttrCalculator skill_calc(owner->GetAttr());
    for (int i = 0; i < MAX_BULLET_OPTION_COUNT; ++i)
    {
        const tagEtherBulletInfo* const bullet_info = device_info_.bullet_info;
        if (bullet_info == NULL)
        {
            continue;
        }

#ifdef _NA_008220_20150414_BUG_ETHERBULLET_ATTR
        if (bullet_info->m_EtherBulletOption[combo_index].m_wOptionKind[i] != 0 &&
            bullet_info->m_EtherBulletOption[combo_index].m_wOptionKind[i] < _countof(g_ITEM_OPTION))
        {
            skill_calc.ClearAttr(
                g_ITEM_OPTION[bullet_info->m_EtherBulletOption[combo_index].m_wOptionKind[i]],
                bullet_info->m_EtherBulletOption[combo_index].m_byValueType[i]
            );

        }  
#else
        if (bullet_info->m_EtherBulletOption[combo_index].m_wOptionKind[i] != 0 && 
            bullet_info->m_EtherBulletOption[combo_index].m_iValue[i] != 0 &&
            bullet_info->m_EtherBulletOption[combo_index].m_wOptionKind[i] < _countof(g_ITEM_OPTION))
        {
            skill_calc.DelAttr(
                g_ITEM_OPTION[bullet_info->m_EtherBulletOption[combo_index].m_wOptionKind[i]],
                bullet_info->m_EtherBulletOption[combo_index].m_byValueType[i], 
                bullet_info->m_EtherBulletOption[combo_index].m_iValue[i]
            );

        }  
#endif //_NA_008220_20150414_BUG_ETHERBULLET_ATTR         
    }

    device_info_.is_apply_option = false;
}

void CEtherEmissionDevice::ApplyCombo(const AttackType attack_type, const DWORD target_key)
{
    if (attack_type == kNone)
    {
        if (device_info_.apply_attack_speed > 0)
        {
            RemoveAttackSpeed();
        }
        device_info_.combo_state = tagEtherBulletInfo::eETHER_DEVICE_COMBO_NORMAL;
    }
    else if (attack_type == kSkill)
    {
        if (device_info_.apply_attack_speed > 0)
        {
            RemoveAttackSpeed();
        }
        device_info_.combo_state = tagEtherBulletInfo::eETHER_DEVICE_COMBO_NORMAL;
    }
    else if (attack_type == kNormal)
    {
        if (device_info_.apply_attack_speed > 0)
        {
            RemoveAttackSpeed();
        }
        if (target_key == device_info_.target_key)
        {
            if (device_info_.combo_state < tagEtherBulletInfo::eETHER_DEVICE_COMBO_MAX)
            {
                device_info_.combo_state++;
                IncreaseAttackSpeed();
            }
        }
        else
        {
            device_info_.combo_state = tagEtherBulletInfo::eETHER_DEVICE_COMBO_1;
            device_info_.target_key = target_key;
            IncreaseAttackSpeed();
        }
        device_info_.combo_tick = GetTickCount();
    }
}

void CEtherEmissionDevice::IncreaseAttackSpeed()
{
    Player* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    if (device_info_.combo_state == tagEtherBulletInfo::eETHER_DEVICE_COMBO_NORMAL)
    {
        return;
    }
    
    if (device_info_.apply_attack_speed > 0)
    {
        SkillAttrCalculator skill_calc(owner->GetAttr());	
        skill_calc.DelAttr(
            eATTR_ATTACK_SPEED, VALUE_TYPE_PERCENT_PER_CUR, device_info_.apply_attack_speed);
        device_info_.apply_attack_speed = 0;
    }
    
    if (device_info_.combo_state <= tagEtherBulletInfo::eETHER_DEVICE_COMBO_NORMAL || 
        device_info_.combo_state >= tagEtherBulletInfo::eETHER_DEVICE_COMBO_MAX)
    {
        return;
    }
        
    device_info_.apply_attack_speed = 
        device_info_.bullet_info->m_iAttackSpeed[device_info_.combo_state - 1];
    
    SkillAttrCalculator skill_calc(owner->GetAttr());	
    skill_calc.AddAttr( 
        eATTR_ATTACK_SPEED, VALUE_TYPE_PERCENT_PER_CUR, device_info_.apply_attack_speed);

    MSG_CG_STATUS_ETHER_ATTACK_SPEED_CMD cmd;	
    cmd.m_AttackSpeedValue = device_info_.apply_attack_speed;
    owner->SendPacket(&cmd, sizeof(cmd));
}

void CEtherEmissionDevice::RemoveAttackSpeed()
{
    Player* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    if (device_info_.apply_attack_speed == 0)
    {
        return;
    }
        
    SkillAttrCalculator skill_calc(owner->GetAttr());	
    skill_calc.DelAttr( 
        eATTR_ATTACK_SPEED, VALUE_TYPE_PERCENT_PER_CUR, device_info_.apply_attack_speed);

    MSG_CG_STATUS_ETHER_ATTACK_SPEED_CMD cmd;		
    cmd.m_AttackSpeedValue = 0;
    owner->SendPacket(&cmd, sizeof(cmd));

    device_info_.apply_attack_speed = 0;
}

Player* CEtherEmissionDevice::GetOwner() const
{
    if (obj_util::IsValid(owner_) == false)
    {
        return NULL;
    }
    return owner_;
}

CEtherEmissionDevice::AttackType CEtherEmissionDevice::CanApplyBulletSkill(
    const Skill* const skill) const
{
    const SCItemSlot* const bullet_item = device_info_.bullet_slot;
    if (bullet_item == NULL)
    {
        return kNone;
    }        
    if (bullet_item->GetItemInfo()->m_wType != eITEMTYPE_ETHER_BULLET || bullet_item->GetNum() == 0)
    {
        return kNone;
    }

    const eSKILL_TYPE skill_type = skill->GetSkillType();
    switch (skill_type)
    {
    case SKILL_TYPE_NORMAL:
    case SKILL_TYPE_NORMAL_AREA:
    case SKILL_TYPE_STYLE:
        return kNormal;
    }
    
    const SkillScriptInfo* const skill_info = skill->GetSkillBaseInfo();
    if (skill_info == NULL)
    {
        return kNone;
    }
    if (skill_info->m_byEtherBulletInfo == 0)
    {
        return kNone;
    }

#ifdef _NA_004605_20120417_ACTIVE_ETHER_BULLET_TO_SKILL
    return kSkill;
#else
    return kNone;
#endif
}

int CEtherEmissionDevice::StartEtherEmissionDevice(Skill* const skill, const DWORD target_key)
{
    const Player* const owner = GetOwner();
    if (owner == NULL)
    {
        return 0;
    }

    if (is_active_ == false)
    {
        return 0;
    }

    //_NA_008456_20150819_ETHER_BULLET_PVP_CHECK
    GameField* game_field = owner->GetField();
    if (game_field == NULL) {
        return 0;
    }
    Character* main_target = game_field->FindCharacter(target_key);
    if (main_target == NULL) {
        return 0;
    }

    if (main_target->GetObjectKey() != owner->GetObjectKey() &&
        main_target->IsEqualObjectKind(PLAYER_OBJECT) == TRUE &&
        GetBulletOption()->apply_pvp_damage_ == FALSE) 
    {
        //탄환이 동작하지 않는 조건
        //타겟이 플레이어인데, PvP탄환이 아닐 경우
        return 0;       
    }
    //----------------------------------------------------------

    const AttackType attack_type = CanApplyBulletSkill(skill);
    ApplyCombo(attack_type, target_key);
    if (attack_type == kNone)
    {
        return 0;
    }
        
    ApplyOption();
    
    skill->SetAppliedEtherBulletOption(TRUE);
    skill->SetAppliedEtherBulletInfo(device_info_.bullet_info);

    return device_info_.combo_state;
}

void CEtherEmissionDevice::EndEtherEmissionDevice(Skill* const skill)
{
    //조건체크------------------------------------------------------------------------------------
    Player* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    if (is_active_ == false)
    {
        return;	
    }
        
    if (device_info_.bullet_info == NULL)
    {
        return;
    }
        
    if (device_info_.apply_attack_speed != 0) 
    {
        RemoveAttackSpeed();
    }

    if (device_info_.is_apply_option == false)
    {
        return;
    }
    //추가된 옵션 제거 ----------------------------------------------------------------------------
    RemoveOption();

    skill->SetAppliedEtherBulletOption(FALSE);
    skill->SetAppliedEtherBulletInfo(NULL);

    //탄환 수량 1감소 (사용에 대한 처리) ----------------------------------------------------------
    ItemManager* const item_manager = owner->GetItemManager();
    if (item_manager == NULL)
    {
        return;
    }
        
    SCSlotContainer* const inven_container = item_manager->GetItemSlotContainer(SI_INVENTORY);
    if (inven_container == NULL)
    {
        return;
    }
        
    GAMELOG->LogItem(ITEM_USE, owner, device_info_.bullet_slot);

    const POSTYPE del_pos = device_info_.bullet_slot_pos;
    if ((device_info_.bullet_slot)->GetNum() == 1)
    {
        MSG_CG_ITEM_INACTIVE_ETHER_BULLET_CMD cmd;
        owner->SendPacket(&cmd, sizeof(cmd));

        InActiveBulletItem(0, true);
    }

    item_manager->Lose(inven_container, del_pos, 1);

    MSG_CG_ITEM_USE_CMD	cmd;
    cmd.m_ItemPos = del_pos;
    owner->SendPacket(&cmd, sizeof(cmd));
}

void CEtherEmissionDevice::Update()
{
    if (is_active_ == false)
    {
        return;
    }
        
    if (device_info_.combo_state >= tagEtherBulletInfo::eETHER_DEVICE_COMBO_1)
    {
        if (device_info_.combo_tick + 2000 < GetTickCount())
        {
            if (device_info_.apply_attack_speed > 0)
            {
                RemoveAttackSpeed();
            }
            device_info_.combo_state = tagEtherBulletInfo::eETHER_DEVICE_COMBO_NORMAL;
        }
    }
}

POSTYPE	CEtherEmissionDevice::GetBulletItemPos() const
{
    if (is_active_ == false)
    {
        return INVALID_POSTYPE_VALUE;
    }
    return device_info_.bullet_slot_pos;
}