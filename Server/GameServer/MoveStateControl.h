#pragma once


class MoveStateControl
{
public:
	MoveStateControl() {}
	~MoveStateControl() {}

	VOID				Init( Character *pOwner, eCHAR_MOVE_STATE eMoveState );
	
	VOID				SetMoveState( eCHAR_MOVE_STATE eMoveState );
	eCHAR_MOVE_STATE	GetMoveState()	{ return m_eMoveState; }

	float				GetMoveSpeed();
    // CHANGES: f110805.2L
    float GetPlayerSpeedInMoving();

#ifdef _NA_0_20110803_PLAYER_MOVEMENT_SYNC
    float               GetLimitMaxSpeed();
#endif

private:
	Character*			m_pOwner;
	eCHAR_MOVE_STATE	m_eMoveState;
	float				m_fBaseMoveSpeed;
};













