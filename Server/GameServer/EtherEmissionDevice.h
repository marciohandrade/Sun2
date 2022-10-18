#pragma once

#include "EtherBulletInfoParser.h"

class Skill;

class CEtherEmissionDevice
{
    enum AttackType
    {
        kNone = 0,
        kNormal	= 1,
        kSkill = 2
    };

    struct tsgEtherEmissionDeviceInfo
    {
        POSTYPE	bullet_slot_pos;
        SCItemSlot*	bullet_slot;
        SCItemSlot*	weapon_slot;
        tagEtherBulletInfo*	bullet_info;
        bool is_apply_option;
        int	combo_state;
        DWORD target_key;
        DWORD combo_tick;
        int	apply_attack_speed;

        tsgEtherEmissionDeviceInfo()
        {
            Init();
        }
        void Init()
        {
            bullet_slot_pos	= 0;
            bullet_slot	= NULL;
            weapon_slot	= NULL;
            bullet_info	= NULL;
            is_apply_option = false;
            combo_state	= tagEtherBulletInfo::eETHER_DEVICE_COMBO_NORMAL;
            target_key = 0;
            combo_tick = 0;
            apply_attack_speed = 0;
        }
    };

public:
    CEtherEmissionDevice();
    ~CEtherEmissionDevice();
        
    void Init(Player* const owner);
    bool ActiveBulletItem(const POSTYPE bullet_pos);
    bool InActiveBulletItem(const POSTYPE bullet_pos, const bool is_force = false);
    bool IsActive() const
    { 
        return is_active_; 
    }
    int	StartEtherEmissionDevice(Skill* const skill, const DWORD target_key);
    void EndEtherEmissionDevice(Skill* const skill);
    void Update();
    tagEtherBulletInfo*	GetBulletOption() const;
    POSTYPE	GetBulletItemPos() const;

private:
    bool SetBulletOption(const SCItemSlot& bullet_slot);
    AttackType CanApplyBulletSkill(const Skill* const skill) const;
    void ApplyOption();
    void RemoveOption();
    void ApplyCombo(const AttackType attack_type, const DWORD target_key);
    void IncreaseAttackSpeed();
    void RemoveAttackSpeed();
    Player* GetOwner() const;

private:
    bool is_active_;
    Player*	owner_;
    tsgEtherEmissionDeviceInfo device_info_;	
};