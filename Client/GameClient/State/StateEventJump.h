/**

    �̺�Ʈ ���� ����

    \SA STATE::JUMP

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
        - 
*/
#pragma once

class StateBase;

class StateEventJump :
	public StateBase
{
	StateEventJump(void);
public:
	~StateEventJump(void);
	__forceinline static StateEventJump* GetInstance()	{	static StateEventJump d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::EVENTJUMP;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

private:
	DWORD		m_dwTotalTime;		// ���������� �����ϴµ� �ɸ��� ��ü �ð�
	float		m_fDownHeight;		// �Ʒ��� �������� �ϴ� ����
	WzVector	m_vDist;			// ������ ��ǥ�� ���� ��ǥ���� ����
	float		m_fVelocity_X;		// ������ ������ ���� �ʱ� �ӵ�
	float		m_fVelocity_Y; 
};
