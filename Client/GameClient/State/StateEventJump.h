/**

    이벤트 점프 상태

    \SA STATE::JUMP

    @author
        -박준태

    @since

    @remarks
        -2005.06.08 소스 정리(지완)
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
	DWORD		m_dwTotalTime;		// 목적지까지 도착하는데 걸리는 전체 시간
	float		m_fDownHeight;		// 아래로 내려가야 하는 높이
	WzVector	m_vDist;			// 목적지 좌표와 현재 좌표간의 차이
	float		m_fVelocity_X;		// 점프를 시작할 때의 초기 속도
	float		m_fVelocity_Y; 
};
