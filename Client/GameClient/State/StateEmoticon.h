/**

    평상시 상태
    \SA STATE::EMOTICON

    @author
        -임대진

    @since

    @remarks
*/

#pragma once

class StateBase;

class StateEmoticon : public StateBase
{
	StateEmoticon(void);
public:
	~StateEmoticon(void);
	__forceinline static StateEmoticon* GetInstance()	{	static StateEmoticon d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::EMOTICON;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

protected:
	BOOL m_bSitting;
};
