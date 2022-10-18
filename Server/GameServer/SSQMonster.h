#pragma once

#include "Monster.h"


//��������������������������������������������������������������������������������������������������������������������������������
//	<GameSSQPolicy>
//
class SSQCharacterStatusInGame
{
public:	typedef DWORD		POLICY_T;
		enum eFLAG {
			CLEARED						= 0,
			CHECKED_DEAD				= 1<< 0,	//	�ѹ� �׾������� ����
			CHECKED_DEAD_TIMER_EXPIRED	= 1<<10,	//	CHECKED_DEAD_TIMER_EVENT�� ���� ������ Ÿ�̸��� ���� ����
			CHECKED_DEAD_TIMER			= 1<<11,	//	�̺�Ʈ Ÿ�̸� ���� ���� ��) ���� �� �ʿ��� �̺�Ʈ(�÷��̾�:�ɱ��) ������ ����
		};

		//	(WAVERIX)(NOTE)
		//	- SSQ Character���� IsDead�� �˻����� ���� �ʿ䰡 �ִ�. �ٽ� ��Ƴ� ����� ���� ���� ���...

public:
	const	POLICY_T&		FIELD;

private:	POLICY_T		m_Field;		// FIELD

public:		SSQCharacterStatusInGame() : FIELD(m_Field), m_Field( CLEARED ) {}
	inline	VOID			Clear() { m_Field = CLEARED; }
	inline	VOID			SetField( const POLICY_T flags ) { m_Field = flags; }
	inline	POLICY_T		GetField() const { return m_Field; }
	inline	BOOLEAN			IsChecked( const eFLAG flag ) const { return !!(m_Field & flag); }

	inline	VOID			AddField( const POLICY_T flag )		{ m_Field |= flag; }
	inline	VOID			RemoveField( const POLICY_T flag )	{ m_Field &= ~flag; }
    __DISABLE_COPY(SSQCharacterStatusInGame);
};


//��������������������������������������������������������������������������������������������������������������������������������
//	<SSQMonster>
//
class SSQMonster : public Monster
{
public:		SSQMonster();
public:
	virtual	~SSQMonster();

	virtual	VOID			InitMonster( WORD NPCCode, BYTE byMoveType, DWORD dwMoveAreaID );
protected:
	virtual	BOOL			Init();
	virtual	VOID			Release();
public:
	virtual	VOID			OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle=-1 );
	virtual	VOID			OnLeaveField();
    //{designed by __NA_001290_20090525_SHIELD_SYSTEM
    // TODO(WAVERIX)
	virtual VOID            OnEnterSector();
    virtual	VOID			UpdateCalcRecover( BOOL bHPUpdate, BOOL bMPUpdate, BOOL bSDUpdate );
    //}
    virtual	VOID			OnDead();

public:		SSQCharacterStatusInGame	ControlStatus;

};
