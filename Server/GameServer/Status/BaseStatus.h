#pragma once


enum { BASE_EXPIRE_TIME_INFINITY = -1 };

class BaseStatus
{
public:
    BaseStatus() : 
        owner_(NULL), 
        state_id_(0), 
        period_(0), 
        application_time_(0), 
        excute_time_(0), 
        expire_time_(0),
        requested_remove_(false)
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        , is_pause_(false)
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    {
    }
    virtual ~BaseStatus() 
    {
    }

    virtual void Init(Character* owner, WORD state_id, int application_time, int period);
    virtual void Start() 
    {
    }
    virtual void Execute();
    virtual void End() 
    {
    }
    virtual BOOL Update(DWORD cur_time);

    WORD GetStateID() const
    {
        return state_id_;
    }
    void SetStateID(const WORD state_id)
    {
        state_id_ = state_id;
    }
    bool IsPeriodicStatus() const
    {
        enum { kBasePeriodTimeZero = 0 };
        return period_ != kBasePeriodTimeZero;
    }
    int GetPeriodTime() const
    {
        return period_;
    }
    int GetApplicationTime() const
    {
        return application_time_;
    }
    int GetLeavedTime() const;
    void SetExpiredTime(const DWORD expire_time) 
    { 
        expire_time_ = expire_time; 
    }
    void StopStatus()
    {
        requested_remove_ = true;
    }
    void CancelRequestStop()
    {
        requested_remove_ = false;
    }
    virtual BOOL IsNotifyStatus()    
    { 
        return FALSE; 
    }
    virtual bool IsAbilityStatus() const   
    { 
        return false; 
    }
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    virtual bool IsToggleFPStatus() const
    {
        return false;
    }
    virtual bool IsOverlapStatus() const
    {
        return false;
    }

    virtual bool StatusOverlap() { return false;}    // 중첩가능 상태 / 상태중첩 +1
    virtual WORD GetStatusOverlapCount() {return 0;} 

    VOID ResetStatus();

    void Pause() {is_pause_ = true; }
    void Resume() {is_pause_ = false; }
    bool IsPaused() {return is_pause_; }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    virtual BOOL CanRemove() 
    { 
        return TRUE; 
    }
    virtual void SetRegenInfo(int regen_hp = 0, int regen_mp = 0) 
    { 
        __UNUSED(regen_hp); __UNUSED(regen_mp); 
    }
    virtual BOOL SendStatusAddBRD();
    virtual BOOL SendStatusDelBRD();

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    virtual void SetFPMaintainTime(int maintain_time) 
    {
        __UNUSED(maintain_time);
    }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
protected:
    Character* GetOwner() const
    {
        if (owner_ != NULL && owner_->IsDeleted())
        {
            return NULL;
        }
        return owner_;
    }
    DWORD GetExpiredTime() const
    {
        return expire_time_;
    }    

private:
    bool IsExecuteTime(const DWORD cur_tick) const
    {
        return cur_tick >= excute_time_ && IsPeriodicStatus();
    }
    bool IsExpired(const DWORD cur_tick) const
    {
        return (expire_time_ != BASE_EXPIRE_TIME_INFINITY) && (expire_time_ <= cur_tick);
    }

private:
    Character* owner_;
    WORD state_id_;
    int period_; // 실행 주기
    int application_time_; // 유지 시간
    DWORD excute_time_; // 다음 실행 시간
    DWORD expire_time_; // 완전히 소멸하는 시간
    bool requested_remove_;
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    bool is_pause_;
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
};