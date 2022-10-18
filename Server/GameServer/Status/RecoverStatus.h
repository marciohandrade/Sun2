#pragma once

#include "EtcStatus.h"


class RecoverStatus : public EtcStatus
{
public:
    RecoverStatus() : 
        regen_hp_(0), regen_mp_(0), use_pet_(false)
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        , fp_maintain_time_(0)
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    {
    }
    ~RecoverStatus() 
    {
    }

    virtual void Init(Character* owner, WORD state_id, int application_time, int period);
    virtual void Start();
    virtual void Execute();
    virtual void SetRegenInfo(int regen_hp = 0, int regen_mp = 0) 
    { 
        regen_hp_ = regen_hp; 
        regen_mp_ = regen_mp; 
    }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    virtual void SetFPMaintainTime(int maintain_time) 
    {
        fp_maintain_time_ = maintain_time;
    }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    virtual BOOL CanRemove()
    {
        return IsUseItem() ? TRUE : FALSE;
    }
    
    void SetUsePet(const bool use_pet)
    { 
        use_pet_ = use_pet; 
    }
    
private:
    bool IsUsePet() const
    { 
        return use_pet_; 
    }
    bool IsHPRecover() const
    { 
        switch (GetStateID())
        {
        case eCHAR_STATE_ETC_AUTO_RECOVER_HPMP:
        case eCHAR_STATE_ETC_AUTO_RECOVER_HP:
        case eCHAR_STATE_ETC_ITEM_RECOVER_HP:
        case eCHAR_STATE_ETC_ITEM_RECOVER_HP_INSTANTLY:
        	return true;
        }
        return false;
    }
    bool IsUseItem() const
    {
        switch (GetStateID())
        {
        case eCHAR_STATE_ETC_ITEM_RECOVER_HP:
        case eCHAR_STATE_ETC_ITEM_RECOVER_HP_INSTANTLY:
        case eCHAR_STATE_ETC_ITEM_RECOVER_MP:
            return true;
        }
        return false;
    }
    bool IsMPRecover() const
    {
        switch (GetStateID())
        {
        case eCHAR_STATE_ETC_AUTO_RECOVER_HPMP:
        case eCHAR_STATE_ETC_AUTO_RECOVER_MP:
        case eCHAR_STATE_ETC_ITEM_RECOVER_MP:
            return true;
        }
        return false;
    }
    bool IsSDRecover() const
    { 
        switch (GetStateID())
        {
        case eCHAR_STATE_ETC_AUTO_RECOVER_SD:
            return true;
        }
        return false;
    }
    bool IsIgnoreReserveHP() const;

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    bool IsFPRecover() const
    {
        switch (GetStateID())
        {
        case eCHAR_STATE_ETC_AUTO_RECOVER_FP:
            return true;
        }
        return false;
    }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

private:
    int regen_hp_;
    int regen_mp_;
    bool use_pet_;

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    bool mode_decrease_fp_;     // FP감소상태여부 플래그

    int fp_maintain_time_;      // FP가 감소되지않고 유지되는 시간
    int status_period_;         // 회복(감소)주기
    int status_execute_counter_; // 해당 상태의 Execute()함수가 호출된 횟수
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
};