#pragma once

class Object;

namespace STATE
{
	//!type
	enum TYPE
	{	/// pcState	
		NONE = -1,		///<NextState가 비어있다는 걸 표시하는 상태.CurState에 이 값이 들어갈 일은 없다.
		IDLE = 0,		///<평상시(가만히 서있는)의 상태
		SPAWN,			///<맨 처음 등장시의 상태
		MOVE,			///<이동시의 상태
        KEYBOARDMOVE,	/// 키보드 이동
		ATTACK,			///<공격시의 상태, Skill관련하여 바뀔 가능성 있음
		JUMP,			///<Jump!
		DEATH,			///<죽음상태
		ANIMATION,		///<이모티콘 동작용( 현재는 동작 확인 용으로도 쓰이고 있음)
		DAMAGE,			///<데미지 상태
		AIR,			///<띄우기
		DOWN,			///<자빠지기
		SKILL,			///<스킬
		GETUP,			///<일어나기
		EVENTJUMP,
		SIT,
		SPECIALMOVE,
		REVIVE,
		KEYBOARDJUMP,
		EMOTICON,
        ATTACKJUMP,    ///<점프공격
        RUNAWAY,       ///<(럭키몬스터)도망친다

		//pet
		PET_EAT_ACTION_STATE,	//Pet먹는 애니
		PET_IDLE,				//펫 꼼지락

		/// nonPcState
		TRIGGER_STATE,	///<맵오브젝트 트리거 엑션 
		ITEM_DROP_STATE,///<아이템 드롭
		// << npc
		NPC_IDLE_STATE,		///<NPC가 평상시(가만히 서있는)의 상태
		NPC_SWING_STATE,	///<NPC가 가끔 하는짓
		NPC_RESPONSE_STATE, ///<NPC가 대답	
		NPC_SPECIAL_ACT_STATE, //NPC의 특별한 행동(행동 조건을 클라에서 정한다)
		NPC_SPECIAL_ACT_LOOP_STATE, //NPC의 특별한 행동(행동 조건을 클라에서 정한다)(루프)
		
		TRANSFORM,
		EARTH_QUAKE, //지진 떨어짐 
		FALL_DOWN,
		AIR_FALL_DOWN,
#ifdef _YJ_FISHING_SYSTEM_000933
		FISHING_THROW,
		FISHING_STAND,
		FISHING_BITE,
		FISHING_SUCCESS,
		FISHING_FAILED,
#endif //_YJ_FISHING_SYSTEM_000933

		KNOCKBACK,
		KNOCKBACK2,
		KNOCKBACK3,
		FIRECRACKER,

		COLLECT,

		SUMMON_RIDER,
		RIDER_TURN,

        ETHERIA_EXTRACT,
        TRIGGER_ANIMATION_CONTROL,

        //! 2011.2.28 / i4u4me / 날개
        WINGS_SPREAD,
        WINGS_FURL,
        RIDER_SPREAD,
        RIDER_FURL,
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		STATUE_STAND,  /// 동상 stand
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		MAX
	};
};

//!StateBase::OnProcess() 에서 리턴
namespace STATEPROCESS
{
	enum TYPE
	{
		HOLD,		///<현재 상태를 유지하라
		SUSPEND,	///<다음상태가 NONE이 아니라면 바꾼다
		END,		///<다음상태가 NONE이면 IDLE로, NONE이 아니면 그 상태로 상태를 바꿔준다
	};
};

//!StateBase::OnStart(), OnEnd() 에서 리턴
namespace STATECHANGE
{
	enum TYPE
	{
		SUCCESS,	///<상태변화 성공
		FAILED,		///<상태변화 실패
		OUTOFRANGE,	///<사정거리 벗어남(아직 구현이 완료되어있지 않음)
	};
};

//!모든 상태 클래스의 기본클래스가 되는 Abstract Class
class StateBase
{
public:
	StateBase() {}
	virtual ~StateBase() {}

	//!각 상태의 Enum값을 리턴해줌 (각각의 클래스가 overriding 해줘야함)
	virtual int GetStateKind() PURE;

	//!이 상태로 이전이 가능한지를 판단하여 리턴해줌
	/*!
	Object::SetNextState() 내부에서 이 함수를 호출해줌
	여기서 TRUE를 리턴하면 NextState에 등록이 됨
	\sa Object::SetNextState
	*/
	virtual BOOL CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState) PURE;

    //! 상태
    virtual bool CanTransite(StateBase* from_state) { return true; }

	//!각 상태가 시작될때 호출이 됨
	/*!
    Object::CommitStateChange()에서 호출이 됨
	\return STATECHANGE::SUCCESS, FAILED, OUTOFRANGE 중 하나를 리턴
	\sa Object::CommitStateChange(), Object::StateProcess(), STATECHANGE::TYPE
	*/
	virtual STATECHANGE::TYPE OnStart(Object* pOwner, DWORD CurTime) { return STATECHANGE::SUCCESS; }

	//!각 상태별로 Process를 돌릴때 호출됨
	/*!
	Object::Process() 에서 호출이 됨
	\return STATEPROCESS::HOLD, SUSPEND, END 중 하나를 리턴
	\sa Object::StateProcess(),  STATEPROCESS::TYPE
	*/
	virtual STATEPROCESS::TYPE OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)  { return STATEPROCESS::SUSPEND; }


	//!각 상태가 끝날때 호출이 됨
	/*!
	Object::CommitStateChange()에서 호출이 됨
	\return STATECHANGE::SUCCESS, FAILED, OUTOFRANGE 중 하나를 리턴
	\sa STATECHANGE::TYPE, Object::CommitStateChange(), Object::StateProcess()
	*/
	virtual STATECHANGE::TYPE OnEnd(Object* pOwner, DWORD CurTime)  { return STATECHANGE::SUCCESS; }

	virtual void              Finalize(Object *pOwner) {}
	virtual void              Initialize(Object *pOwner) {}
	virtual void              OnForceChange(Object *pOwner) {}

    // 특정 상태에게 특정 행동을 지시하기위한 인터페이스.
    // 스킬 상태일경우, 캐릭터를 통해 현재 어떤 상태인지 식별이 할 방법이 없다보니 특정 스킬상태일때 명령을 내리기위한 방법이 없음
    // 일단은 디펜스모션일경우 이 함수가 호출되면 상태를 종료하는데에만 사용되는정도
    virtual void ProcessExtraCommand() {}
};

