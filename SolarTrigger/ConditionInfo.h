#pragma once
#ifndef SOLARTRIGGER_CONDITION_INFO_H
#define SOLARTRIGGER_CONDITION_INFO_H
//==================================================================================================
/// ConditionInfo's class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2004. 12. 29
	@remark
		- 컨디션 트리거(ConditionInfo class)을 상속받은 각종 컨디션 트리거 타입 클래스들 집합
	@note
		- 
	@history
        - ~2011. 07. 20
        - 2011. 07. 20, waverix, code improvement
*/
//==================================================================================================
#include "TriggerCommon.h"

//-----------------------------------------------------------------------
//
// 최상위 클래스
//
//-----------------------------------------------------------------------
class ConditionInfo
{
public:
    ConditionInfo(eCONDITION_TYPE trigger_type);
    virtual ~ConditionInfo();

    eCONDITION_TYPE TriggerType() const;
private:
    eCONDITION_TYPE trigger_type_;
};

inline ConditionInfo::ConditionInfo(eCONDITION_TYPE trigger_type)
    : trigger_type_(trigger_type)
{
}

inline ConditionInfo::~ConditionInfo()
{
}

inline eCONDITION_TYPE ConditionInfo::TriggerType() const
{
    return trigger_type_;
}

//-----------------------------------------------------------------------
//
// 하위 클래스 선언 정의문
//
//-----------------------------------------------------------------------
#define BEGIN_DECL_CONDITION(ConcreteCondition) \
    class ConcreteCondition##_INFO : public ConditionInfo \
    { \
    public: \
        ConcreteCondition##_INFO() : ConditionInfo(e##ConcreteCondition) {} \
        virtual ~ConcreteCondition##_INFO() {}

#define END_DECL_CONDITION \
    };

//-----------------------------------------------------------------------
//
// 하위 클래스
//
// Warnings :   ObjectID가 INT
//              ItemCode가 INT
// 
//-----------------------------------------------------------------------

namespace ns_trigger {
;

enum 
{
    PERSONAL = 0,
    PARTY = 1,
	ZONE = 2,
};
enum // NOTE: f110721.2L, unused type
{
    METHOD_GENERAL      = 0,
    METHOD_ACCUMULATE   = 1,
};

}; //end of namespace

BEGIN_DECL_CONDITION(CONDITION_ALWAYS)
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_COMPARE_SWITCH)
    __PROPERTY(BYTE, SwitchID);
    __PROPERTY(BYTE, OperationType);
    __PROPERTY(INT, Value);
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_COMPARE_SWITCH_EX)
	__PROPERTY(BYTE, Target);
	__PROPERTY(BYTE, SwitchID);
	__PROPERTY(BYTE, OperationType);
	__PROPERTY(INT, Value);
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_CLICK_OBJECT)
    __PROPERTY(INT, ObjectID);
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_DAMAGE_OBJECT)
    __PROPERTY(INT, ObjectID);
    __PROPERTY(FLOAT, Damage);
END_DECL_CONDITION;


BEGIN_DECL_CONDITION(CONDITION_COUNT_NPCKILL)
    __PROPERTY(INT, AreaID);
    __PROPERTY(INT, MonsterCode);   //< 몹 종류 몹 Code
    __PROPERTY(BYTE, IsParty);
    __PROPERTY(INT, KillCount);     //< 몇마리
    __PROPERTY(BYTE, Method);       //< 방법 = { 일반 , 누적 }
END_DECL_CONDITION;


BEGIN_DECL_CONDITION(CONDITION_PASS_TIME)
    __PROPERTY(INT, PassTime);
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_ENTER_AREA)
    __PROPERTY(INT, AreaID);
    __PROPERTY(INT, PlayerCount);
    __PROPERTY(BYTE, IsParty);
END_DECL_CONDITION;

// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
// NPC가 특정 지역에 들어갔을 때
BEGIN_DECL_CONDITION(CONDITION_NPC_ENTER_AREA)
    __PROPERTY(CODETYPE, AreaID);
    __PROPERTY(CODETYPE, NPCUnitID);
END_DECL_CONDITION;

//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
BEGIN_DECL_CONDITION(CONDITION_COMPLETE_COLLECTION)
public:
    __PROPERTY(CODETYPE, ObjectKey); // 객체 종류에 따라 변경
END_DECL_CONDITION;

//_NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER
BEGIN_DECL_CONDITION(CONDITION_SERVER_EVENT)
public:
    __PROPERTY(int, EventID); // 이벤트 종류에 따라 변경
END_DECL_CONDITION;

// _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM
BEGIN_DECL_CONDITION(CONDITION_CHECK_TEAM)
public:
    __PROPERTY(int, TeamKey); // 이벤트 종류에 따라 변경
END_DECL_CONDITION;



BEGIN_DECL_CONDITION(CONDITION_HAVE_ITEM)
    __PROPERTY(INT, ItemCode);
    __PROPERTY(INT, ItemNum);
END_DECL_CONDITION;


BEGIN_DECL_CONDITION(CONDITION_MULTI_SWITCH)
public:  enum { _MAX_SWITCH_ID_NUM = 10 };
    __ARRAY_PROPERTY(BYTE, _MAX_SWITCH_ID_NUM, SwitchID);
    __PROPERTY(BYTE, OperationType1);
    __PROPERTY(INT, Value1);
    __PROPERTY(BYTE, OperationType2);
    __PROPERTY(INT, Value2);
END_DECL_CONDITION;


BEGIN_DECL_CONDITION(CONDITION_MISSION_POINT)
    __PROPERTY(INT, MissionPoint);  // 이 포인트보다
    __PROPERTY(BYTE, Flag);         // 0:이상 1:이하 2:동등
END_DECL_CONDITION;

// [변수IDX] 가 [비교] [int]를 만족하면
BEGIN_DECL_CONDITION(CONDITION_COMPARE_VARIABLE)
    __PROPERTY(INT, VarID);
    __PROPERTY(BYTE, OperationType);
    __PROPERTY(INT, Value);
END_DECL_CONDITION;


// Quest 완료 체크 트리거, 있는것인지 확인할 것
BEGIN_DECL_CONDITION(CONDITION_QUEST_PROGRESS)
enum ePROGRESS { IN_PROGRESS, COMPLETED, DONTHAVE };
    __PROPERTY(/*QCODE*/ ULONG, QuestID);// Quest ID
    __PROPERTY(BYTE, Progress);          // (진행중, 완료한)
END_DECL_CONDITION;

// 유저 클래스 설정(의존 트리거)
// [USER CLASS ID] 이면, //0:모두, 1:버서커, 2:드래곤, 3:쉐도우, 4:발키리, 5:엘리멘탈리스트
BEGIN_DECL_CONDITION(CONDITION_CHECK_CLASS_ID)
    __PROPERTY(INT, ClassID);
END_DECL_CONDITION;


BEGIN_DECL_CONDITION(CONDITION_NPCKILL_FOR_UNITID)
    __PROPERTY(WzID, MonsterUnitID);
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_LIVE_PARTY_MEMBER)
public:
    enum{ MEMBER_DIE = 0, MEMBER_LIVE = 1 };
    __PROPERTY(BYTE, LiveFlag); // 0:사망 1:생존
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_CHECK_OBJECT_HP)
public:
    __PROPERTY(DWORD, dwObjKey);    // mapobject
    __PROPERTY(BYTE,  btHPPercentage);
END_DECL_CONDITION;


BEGIN_DECL_CONDITION(CONDITION_DESTROY_OBJECT)
public:
    __PROPERTY(DWORD, dwObjKey);
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_CHECK_OBJECTKIND_HP)
public:
    __PROPERTY(BYTE,  byObjectType);    // 대상의 타입(몬스터/유닛/오브젝트);
    __PROPERTY(DWORD, dwObjectKey);     // 대상 객체의 키값
    __PROPERTY(BYTE,  btHPPercentage);  // HP 조건
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_MONSTER_ENTER_AREA)
public:
    __PROPERTY(int, AreaID);    // 영역ID
    __PROPERTY(int, MonsterCode);  // 몬스터코드
    END_DECL_CONDITION;

#endif //SOLARTRIGGER_CONDITION_INFO_H