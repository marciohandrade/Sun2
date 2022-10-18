#pragma once

#include "AbilityStatus.h"


// µå·¡°ï º¯½Å
class DragonTransStatus : public AbilityStatus
{
public:
	DragonTransStatus();
	~DragonTransStatus() {}

	virtual VOID		Start();
	virtual VOID		Execute() {}
	virtual VOID		End();

	eCHAR_TYPE			GetCharType()	{ return (eCHAR_TYPE)m_iOption2; }
    // CHANGES: f110428.6L, prevent a skipping previous style buff release process
    // because at this point, player is already transform.
    bool CheckStarting() const;
private:
    void ApplyAttrEx(bool application);
    //
    bool starting_state_;
};

inline DragonTransStatus::DragonTransStatus()
    : starting_state_(false)
{
}

// CHANGES: f110428.6L, prevent a skipping previous style buff release process
// because at this point, player is already transform.
inline bool DragonTransStatus::CheckStarting() const {
    return starting_state_;
};
