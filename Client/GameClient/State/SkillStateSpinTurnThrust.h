#pragma once
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

class StateSkill;
class SkillStateSpinTurnThrust : public StateSkill
{
public:
    SkillStateSpinTurnThrust(void);
    ~SkillStateSpinTurnThrust(void);
public:
    STATECHANGE::TYPE  OnStart(Object* owner_ptr, DWORD cur_time);
    STATEPROCESS::TYPE OnProcess(Object* owner_ptr, DWORD cur_time, DWORD tick);

    void CreateIllusion(Character* character_ptr, DWORD live_time, DWORD disappear_time );

private:    
    BOOL is_move_;
};
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
