#pragma once
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

class StateSkill;
class SkillStateBlairPang : public StateSkill
{
public:
    SkillStateBlairPang(void);
    ~SkillStateBlairPang(void);
public:
    STATECHANGE::TYPE OnStart(Object* owner_ptr, DWORD cur_time);
    STATEPROCESS::TYPE OnProcess(Object* owner_ptr, DWORD cur_time, DWORD tick);
};
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
