#pragma once
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

class StateSkill;
class SkillStateDarkPolingGrave : public StateSkill
{
public:
    SkillStateDarkPolingGrave(void);
    ~SkillStateDarkPolingGrave(void);
public:
    STATECHANGE::TYPE  OnStart(Object* owner_ptr, DWORD cur_time);
    STATEPROCESS::TYPE OnProcess(Object* owner_ptr, DWORD cur_time, DWORD tick);
};
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
