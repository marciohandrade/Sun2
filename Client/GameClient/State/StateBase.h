#pragma once

class Object;

namespace STATE
{
	//!type
	enum TYPE
	{	/// pcState	
		NONE = -1,		///<NextState�� ����ִٴ� �� ǥ���ϴ� ����.CurState�� �� ���� �� ���� ����.
		IDLE = 0,		///<����(������ ���ִ�)�� ����
		SPAWN,			///<�� ó�� ������� ����
		MOVE,			///<�̵����� ����
        KEYBOARDMOVE,	/// Ű���� �̵�
		ATTACK,			///<���ݽ��� ����, Skill�����Ͽ� �ٲ� ���ɼ� ����
		JUMP,			///<Jump!
		DEATH,			///<��������
		ANIMATION,		///<�̸�Ƽ�� ���ۿ�( ����� ���� Ȯ�� �����ε� ���̰� ����)
		DAMAGE,			///<������ ����
		AIR,			///<����
		DOWN,			///<�ں�����
		SKILL,			///<��ų
		GETUP,			///<�Ͼ��
		EVENTJUMP,
		SIT,
		SPECIALMOVE,
		REVIVE,
		KEYBOARDJUMP,
		EMOTICON,
        ATTACKJUMP,    ///<��������
        RUNAWAY,       ///<(��Ű����)����ģ��

		//pet
		PET_EAT_ACTION_STATE,	//Pet�Դ� �ִ�
		PET_IDLE,				//�� ������

		/// nonPcState
		TRIGGER_STATE,	///<�ʿ�����Ʈ Ʈ���� ���� 
		ITEM_DROP_STATE,///<������ ���
		// << npc
		NPC_IDLE_STATE,		///<NPC�� ����(������ ���ִ�)�� ����
		NPC_SWING_STATE,	///<NPC�� ���� �ϴ���
		NPC_RESPONSE_STATE, ///<NPC�� ���	
		NPC_SPECIAL_ACT_STATE, //NPC�� Ư���� �ൿ(�ൿ ������ Ŭ�󿡼� ���Ѵ�)
		NPC_SPECIAL_ACT_LOOP_STATE, //NPC�� Ư���� �ൿ(�ൿ ������ Ŭ�󿡼� ���Ѵ�)(����)
		
		TRANSFORM,
		EARTH_QUAKE, //���� ������ 
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

        //! 2011.2.28 / i4u4me / ����
        WINGS_SPREAD,
        WINGS_FURL,
        RIDER_SPREAD,
        RIDER_FURL,
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		STATUE_STAND,  /// ���� stand
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		MAX
	};
};

//!StateBase::OnProcess() ���� ����
namespace STATEPROCESS
{
	enum TYPE
	{
		HOLD,		///<���� ���¸� �����϶�
		SUSPEND,	///<�������°� NONE�� �ƴ϶�� �ٲ۴�
		END,		///<�������°� NONE�̸� IDLE��, NONE�� �ƴϸ� �� ���·� ���¸� �ٲ��ش�
	};
};

//!StateBase::OnStart(), OnEnd() ���� ����
namespace STATECHANGE
{
	enum TYPE
	{
		SUCCESS,	///<���º�ȭ ����
		FAILED,		///<���º�ȭ ����
		OUTOFRANGE,	///<�����Ÿ� ���(���� ������ �Ϸ�Ǿ����� ����)
	};
};

//!��� ���� Ŭ������ �⺻Ŭ������ �Ǵ� Abstract Class
class StateBase
{
public:
	StateBase() {}
	virtual ~StateBase() {}

	//!�� ������ Enum���� �������� (������ Ŭ������ overriding �������)
	virtual int GetStateKind() PURE;

	//!�� ���·� ������ ���������� �Ǵ��Ͽ� ��������
	/*!
	Object::SetNextState() ���ο��� �� �Լ��� ȣ������
	���⼭ TRUE�� �����ϸ� NextState�� ����� ��
	\sa Object::SetNextState
	*/
	virtual BOOL CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState) PURE;

    //! ����
    virtual bool CanTransite(StateBase* from_state) { return true; }

	//!�� ���°� ���۵ɶ� ȣ���� ��
	/*!
    Object::CommitStateChange()���� ȣ���� ��
	\return STATECHANGE::SUCCESS, FAILED, OUTOFRANGE �� �ϳ��� ����
	\sa Object::CommitStateChange(), Object::StateProcess(), STATECHANGE::TYPE
	*/
	virtual STATECHANGE::TYPE OnStart(Object* pOwner, DWORD CurTime) { return STATECHANGE::SUCCESS; }

	//!�� ���º��� Process�� ������ ȣ���
	/*!
	Object::Process() ���� ȣ���� ��
	\return STATEPROCESS::HOLD, SUSPEND, END �� �ϳ��� ����
	\sa Object::StateProcess(),  STATEPROCESS::TYPE
	*/
	virtual STATEPROCESS::TYPE OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)  { return STATEPROCESS::SUSPEND; }


	//!�� ���°� ������ ȣ���� ��
	/*!
	Object::CommitStateChange()���� ȣ���� ��
	\return STATECHANGE::SUCCESS, FAILED, OUTOFRANGE �� �ϳ��� ����
	\sa STATECHANGE::TYPE, Object::CommitStateChange(), Object::StateProcess()
	*/
	virtual STATECHANGE::TYPE OnEnd(Object* pOwner, DWORD CurTime)  { return STATECHANGE::SUCCESS; }

	virtual void              Finalize(Object *pOwner) {}
	virtual void              Initialize(Object *pOwner) {}
	virtual void              OnForceChange(Object *pOwner) {}

    // Ư�� ���¿��� Ư�� �ൿ�� �����ϱ����� �������̽�.
    // ��ų �����ϰ��, ĳ���͸� ���� ���� � �������� �ĺ��� �� ����� ���ٺ��� Ư�� ��ų�����϶� ����� ���������� ����� ����
    // �ϴ��� ���潺����ϰ�� �� �Լ��� ȣ��Ǹ� ���¸� �����ϴµ����� ���Ǵ�����
    virtual void ProcessExtraCommand() {}
};

