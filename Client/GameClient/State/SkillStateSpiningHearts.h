#pragma once
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

class StateSkill;
class SkillStateSpiningHearts : public StateSkill
{
public:
    SkillStateSpiningHearts(void);
    ~SkillStateSpiningHearts(void);
public:
    STATECHANGE::TYPE  OnStart(Object* owner_ptr, DWORD cur_time);
    STATEPROCESS::TYPE OnProcess(Object* owner_ptr, DWORD cur_time, DWORD tick);
    STATECHANGE::TYPE  OnEnd(Object* owner_ptr, DWORD cur_time);

private:
    int hit_count_;
};
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
