#pragma once

class AuraSkill
{
private:
    std::vector<eCHAR_STATE_TYPE> m_AuraStateArray;
    std::vector<SLOTCODE> m_AuraSkillCodeArray;

protected:
    AuraSkill();

public:
    eCHAR_STATE_TYPE FindAuraSkillState( SLOTCODE SkillClassCode );
    void GetAuraSkillCodeArray( std::vector<SLOTCODE>& Array );
};
