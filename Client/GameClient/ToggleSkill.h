#pragma once
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

class ToggleSkill
{
public:
    ToggleSkill(void);
    ~ToggleSkill(void);

protected:
    std::vector<eCHAR_STATE_TYPE> toggle_state_array_;
    std::vector<SLOTCODE> toggle_skill_code_array_;

public:
    eCHAR_STATE_TYPE FindToggleSkillState( SLOTCODE skill_class_code );
    void             GetToggleSkillCodeArray( std::vector<SLOTCODE>& skill_code_array );

#ifdef _NA_008252_20150427_AURASKILL_PACKET_PENDING
public:
    BOOL GetPacketPending() { return send_packet_pending_; }
    void SetPacketPending() { send_packet_pending_ = TRUE; }
    void UnsetPacketPending() { send_packet_pending_ = FALSE; }

private:
    BOOL send_packet_pending_;
#endif //_NA_008252_20150427_AURASKILL_PACKET_PENDING
};
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
