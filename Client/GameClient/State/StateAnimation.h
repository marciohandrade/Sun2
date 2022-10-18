/**

    ���ϸ��̼� ����
    \SA STATE::ANIMATION

    @author
        -������

    @since

    @remarks
        -2005.06.07 �ҽ� ����(����)
        - 
*/
#pragma once

class StateBase;
class StateOwner;

class StateAnimation :public StateBase
{
	StateAnimation(void);
public:
	~StateAnimation(void);
	__forceinline static StateAnimation* GetInstance()	{	static StateAnimation d;	return &d;	};

	virtual int			GetStateKind() {	return STATE::ANIMATION;	}
	virtual BOOL		CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
