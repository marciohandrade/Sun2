#pragma once
#ifndef SOLARTRIGGER_ACTION_INFO_H
#define SOLARTRIGGER_ACTION_INFO_H
//==================================================================================================
/// ActionInfo's class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2004. 12. 29
    @remark
        - 액션 트리거(ActionInfo class)을 상속받은 각종 액션 트리거 타입 클래스들 집합
    @note
        - 
    @history 
        - ~2011. 07. 20
        - 2011. 07. 20, waverix, code improvement
*/
//==================================================================================================
#include "ConditionInfo.h"
#include "TriggerCommon.h"

//-----------------------------------------------------------------------
//
// 최상위 클래스
//
//-----------------------------------------------------------------------
class ActionInfo
{
public:
    ActionInfo(eACTION_TYPE wType);
    virtual ~ActionInfo();

    eACTION_TYPE TriggerType() const;
private:
    eACTION_TYPE trigger_type_;
};

inline ActionInfo::ActionInfo(eACTION_TYPE trigger_type)
    : trigger_type_(trigger_type)
{
}

inline ActionInfo::~ActionInfo()
{
}

inline eACTION_TYPE ActionInfo::TriggerType() const
{
    return trigger_type_;
}

//-----------------------------------------------------------------------
//
// 하위 클래스 선언 정의문
//
//-----------------------------------------------------------------------
#define BEGIN_DECL_ACTION(ConcreteAction) \
    class ConcreteAction##_INFO : public ActionInfo \
    { \
    public: \
        ConcreteAction##_INFO() : ActionInfo(e##ConcreteAction) {} \
        virtual ~ConcreteAction##_INFO() {}

#define END_DECL_ACTION \
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
    CANMOVE,
    CANNOTMOVE,
};      //< ACTION_CHANGE_PATHTILE의 Attribute 속성 정보 

}; //end of namespace

/*
BEGIN_DECL_ACTION(ACTION_CHANGE_SWITCH)
    __PROPERTY(BYTE, SwitchID)
    __PROPERTY(INT, Value)
END_DECL_ACTION;
*/
BEGIN_DECL_ACTION(ACTION_CHANGE_OBJECTANI)
    __PROPERTY(INT, ObjectID);
    __PROPERTY(DWORD, WzAniID);
    __PROPERTY(BYTE, Direction);// 방향(정방향 0, 역방향 1) 
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_CHANGE_PATHTILE)
    __PROPERTY(INT, TileID);
    __PROPERTY(BYTE, Attribute);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_CHANGE_OBJECTSTATE)
public:enum { APPEAR, DISAPPEAR, REMOVE, };
    __PROPERTY(INT, ObjectID);
    __PROPERTY(BYTE, State);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_PLAY_EVENT)
    __PROPERTY(INT, EventID);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_SET_ISATTACK)
public:
    enum { 
        TARGET_TYPE_USER = 0,
        TARGET_TYPE_MONSTER, 
        TARGET_TYPE_ALL,
        CAN_NOT_ATTACK = 0,
        CAN_ATTACK
    };
    __PROPERTY(BYTE, TargetType);// 공격대상 종류(0 : 유저, 1: 몬스터, 2 : 둘다)
    __PROPERTY(BYTE, IsAttack);  // 공격 가능 여부(0 : 공격불가능, 1 : 공격가능)

END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_PORTAL_PLAYER)
public:
    enum {
        _MAX_MAP_NAME_LENGTH = 128, 
        RELATEDVILLAGE  = 0, 
        VILLAGECODE     = 1,
        FIELDCODE       = 2,
        TARGET_RETURN   = 3, //__NA001310_BATTLEZONE_PORTAL_RETURN_TARGET_MOVE_SUPPORT__
    };
    __PROPERTY(INT, MapCode);
    __PROPERTY(INT, AreaID);
    __PROPERTY(BYTE, IsParty);  // 이동시킬 대상 (0 : 개인 1: 파티)
    __PROPERTY(INT, Type);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_SET_OBJECTTHRUST)
    __PROPERTY(BYTE, State);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_APPLY_DAMAGE)
public:
    enum {
        TARGET_FIELD    = 0,
        TARGET_PERSONAL = 1,
    };

    __PROPERTY(BYTE, DamageType);
    __PROPERTY(BYTE, TargetType);
    __PROPERTY(FLOAT, Damage);
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_REFLECT_DAMAGE)
    __PROPERTY(INT, ObjectID);
    __PROPERTY(FLOAT, Damage);
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CREATE_MONSTER)
    __PROPERTY(INT, MonsterCode);
    __PROPERTY(INT, AreaID);
    __PROPERTY(BYTE, IsParty);
    __PROPERTY(INT, CreateCount);           // 몬스터 생성 갯수
    __PROPERTY(INT, DelayTime);         // 생성 시간 간격
    __PROPERTY(INT, LoopCount);         // 몬스터 생성 회수
    __PROPERTY(BYTE, CreateDestroy);    // 몬스터 생성/제거
    __PROPERTY(INT, Type);                  // 타입 (리더 냐 꼬붕이냐)
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_DISPLAY_MSGBOX)
    enum eENUM
    {
        TARGET_PERSONAL         = 0,
        TARGET_FIELD            = 1,
        TARGET_OFFENCE          = 2,
        TARGET_DEFENCE          = 3,
    };

    __PROPERTY(INT, TextID);
    __PROPERTY(BYTE, Target);       // 0:개인 1:전체 2:공격 3:수비
    __PROPERTY(INT, DisplayWindow); // 출력할 창
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_REMOVE_ITEM)
    __PROPERTY(INT, ItemCode);
    __PROPERTY(INT, ItemNum);   // 아이템 개수
END_DECL_ACTION;

/*
BEGIN_DECL_ACTION(ACTION_PRESERVER)
END_DECL_ACTION;
*/

BEGIN_DECL_ACTION(ACTION_REWARD_PLAYER)
public:enum { INVENTORY=0, DROPFIELD=1, };
    __PROPERTY(INT, ItemCode);
    __PROPERTY(INT, Count);
    __PROPERTY(BYTE, IsParty);
    __PROPERTY(BYTE, IsDrop);   // 주는방식(0 : 인벤에 넣어서,1 바닥에 떨굼)
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_CLEAR_MISSION)
    __PROPERTY(BYTE, ResultType);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_PORTAL_RANDOM)
public: enum { _MAX_AREA_ID_NUM = 8, };
    __PROPERTY(INT, MapID);
    __ARRAY_PROPERTY(INT, _MAX_AREA_ID_NUM, AreaID);
    __PROPERTY(BYTE, Random);               // 랜덤 순차적으로
    __PROPERTY(BYTE, Target);               // (0: 개인 1: 파티)
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_NPC_RANDOM_AREA_MOVE)
public: enum { _MAX_AREA_ID_NUM = 8, };
    __ARRAY_PROPERTY(INT, _MAX_AREA_ID_NUM, AreaID);
    __PROPERTY(BYTE, MoveOrWarp);           // (0: move 1: warp)
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_ACTIVATE_TRIGGER)
    __PROPERTY(DWORD, WzTriggerID);
    __PROPERTY(BYTE, Flag); // 0 : 활성, 1 : 비활성
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_AREA_DAMAGE)
    __PROPERTY(DWORD, ObjectIndex);
    __PROPERTY(FLOAT, Range);
    __PROPERTY(BYTE, Target);       // 목표 (개인/파티)
    __PROPERTY(BYTE, DamageType);   // 데미지 타입
    __PROPERTY(FLOAT, Damage);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_OPERATE_SWITCH)
    __PROPERTY(BYTE, SwitchID);
    __PROPERTY(BYTE, OperationType);
    __PROPERTY(INT, Argument);  // 매개변수
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_OPERATE_SWITCH_EX)
	__PROPERTY(BYTE, Target);               // (0: 개인 1: 파티)
	__PROPERTY(BYTE, SwitchID);
	__PROPERTY(BYTE, OperationType);
	__PROPERTY(INT, Argument);  // 매개변수
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_OPERATE_VARIABLE)
    __PROPERTY(INT, VarID);
    __PROPERTY(BYTE, OperationType);
    __PROPERTY(INT, Argument);  // 매개변수
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_ROAD_CONNECT); // 길 연결을 설정/해제 한다
    __PROPERTY(INT, AreaIndex);
    __PROPERTY(BYTE, CanGo);    // 갈 수 있다=0, 없다=1
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CREATE_DIR_MONSTER)
    __PROPERTY(INT, MonsterCode);
    __PROPERTY(INT, AreaID);
    __PROPERTY(BYTE, IsParty);
    __PROPERTY(INT, CreateCount);   // 몬스터 생성 갯수
    __PROPERTY(INT, DelayTime);     // 생성 시간 간격
    __PROPERTY(INT, LoopCount);     // 몬스터 생성 회수
    __PROPERTY(FLOAT, DirX);        // x방향
    __PROPERTY(FLOAT, DirY);        // y방향
    __PROPERTY(FLOAT, DirZ);        // z방향
    __PROPERTY(INT, Type);          // Type[0]
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CHANGE_LIGHT)
    __PROPERTY(INT, LightIndex);// 이 인덱스의 라이트를
    __PROPERTY(INT, DelayTime); // 이만큼의 시간 후에
    __PROPERTY(BYTE, Status);   // 0:끈다 1:켠다 2:토글한다
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CHANGE_OBJECT_TYPE)
    __PROPERTY(INT, ObjectID);
    __PROPERTY(INT, DelayTime);
    __PROPERTY(INT, ObjectType);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_QUEST_SATISFACTION)
    __PROPERTY(ULONG, QuestID);
    __PROPERTY(LONG, ScriptCode);
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_QUEST_GRANT)
    __PROPERTY(ULONG, QuestID);
    __PROPERTY(BYTE, PartyState);
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_QUEST_SETUP)
    __PROPERTY(ULONG, QuestID);
    __PROPERTY(BYTE, Completed);
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_USE_CATEGORY)
    enum { _MAX_USE_CATEGORY_NAME = 128 };
    __PROPERTY(INT, FieldID);
    __ARRAY_PROPERTY(TCHAR, _MAX_USE_CATEGORY_NAME, CategoryName)
END_DECL_ACTION;

// 기존에 존재하는 액션 트리거
//BEGIN_DECL_ACTION(ACTION_CREATE_MONSTER)
//__END_DECL_ACTION

BEGIN_DECL_ACTION(ACTION_HOLD_CHARACTER)
    __PROPERTY(BYTE, Who);
    __PROPERTY(INT, HoldingTime);
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CREATE_MONSTER_GROUP)
    __PROPERTY(INT, GroupID);
    __PROPERTY(INT, AreaID);
    __PROPERTY(BYTE, PartyState);
    __PROPERTY(INT, GroupCount);
END_DECL_ACTION;

// <THINKING & WARNING><Waverix>
//  나타남/사라짐
//  나타나게/사라지게
//  --> 이 두가지는 서로 반대 값으로 해당 속성을 표시하게 된다. 주의할 것
//  ShowOrHide의 명칭 수정할 필요가 있다.

// 유닛 나타남/사라짐 (서버에서는 몬스터 생성/제거와 동일)
BEGIN_DECL_ACTION(ACTION_SHOW_OR_HIDE_UNIT)
    enum { SHOW=0,HIDE=1,};
    __PROPERTY(WzID, MonsterUnitID);
    __PROPERTY(INT, IsHide); //0: 나타나게, 1:사라지게
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_SHOW_GUIDE_MSGBOX)
    __PROPERTY(LONG, HelpIndex);
END_DECL_ACTION;

// 메시지 박스의 나타남/사라짐
BEGIN_DECL_ACTION(ACTION_SHOW_OR_HIDE_MSGBOX)
    enum { SHOW=1,HIDE=0,};
    __PROPERTY(INT, TextID);
    __PROPERTY(BYTE, IsParty);
    __PROPERTY(INT, DisplayWindow); // 출력할 창
    __PROPERTY(INT, IsShow);        //1: 나타나게, 0:사라지게
END_DECL_ACTION;


// 몬스터 랜덤 스폰
BEGIN_DECL_ACTION(ACTION_RANDOM_MONSTER_SPAWN)
    __PROPERTY(INT, MonsterID_1);           // 1-5 몬스터
    __PROPERTY(INT, MonsterRate_1);     // 1-5 몬스터 생성확률
    __PROPERTY(INT, MonsterID_2);
    __PROPERTY(INT, MonsterRate_2);
    __PROPERTY(INT, MonsterID_3);
    __PROPERTY(INT, MonsterRate_3);
    __PROPERTY(INT, MonsterID_4);
    __PROPERTY(INT, MonsterRate_4);
    __PROPERTY(INT, MonsterID_5);
    __PROPERTY(INT, MonsterRate_5);
    __PROPERTY(INT, AreaID);                // 몬스터 생성 지역
    __PROPERTY(BYTE, IsParty);          // 개인 ? 파티 ?
    __PROPERTY(INT, MinCreateMonsterCnt);   // 최소 몬스터 생성 갯수
    __PROPERTY(INT, MaxCreateMonsterCnt);   // 최소 몬스터 생성 갯수
    __PROPERTY(INT, RegenTime);         // 리젠 시간 간격
    __PROPERTY(INT, MaxRegenCount);     // 몬스터 총 리젠 회수
    __PROPERTY(FLOAT, DirX);                // x방향
    __PROPERTY(FLOAT, DirY);                // y방향
    __PROPERTY(FLOAT, DirZ);                // z방향
END_DECL_ACTION;

//#ifdef __NA_000104_20070123_TOP_OF_TRIAL
BEGIN_DECL_ACTION(ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR)
    __PROPERTY(INT, Floor); // 저주의 탑 층 수
END_DECL_ACTION;
//#endif //

//////////////////////////////////////////////////////////////////////////
BEGIN_DECL_ACTION(ACTION_RANDOM_MAP_MOVE)   // 랜덤 맵 이동
public:
    enum eENUM
    {
        _MAX_FIELDCODE_ID       = 10,       // 필드 코드 배열 크기
        RNDMODE_RANDOM          = 0,
        RNDMODE_SEQUENCIAL      = 1,
        TARGET_PERSONAL         = 0,
        TARGET_PARTY            = 1,
        RETMAP_VILLAGE          = 0,
        RETMAP_STARTINGVILLAGE  = 1,
        RETMAP_BATTLEMAP        = 2,
    };
    __ARRAY_PROPERTY(INT, _MAX_FIELDCODE_ID, FieldID);
    __PROPERTY(BYTE, Random);               // 랜덤, 순차적으로
    __PROPERTY(WzID, AreaID);               // 영역 ID
    __PROPERTY(BYTE, Target);               // (0: 개인 1: 파티)
    __PROPERTY(INT, Type);              // (0: 마을, 1:귀속마을(?), 2:배틀맵
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_RANDOM_AREA_MOVE) // 유저를 영역 이동 시킨다
public:
    enum eENUM
    {
        _MAX_AREA_ID            = 8,
        TARGET_PERSONAL         = 0,
        TARGET_PARTY            = 1,
        PROC_EACH               = 0,
        PROC_SAMENESS           = 1,
    };
    __ARRAY_PROPERTY(INT, _MAX_AREA_ID, AreaIDX);   // 이동시킬 영역 IDs
    __PROPERTY(BYTE, Target);   // (0: 개인 1: 파티)
    __PROPERTY(BYTE, ProcType); // (0: 개별, 1: 동일)
END_DECL_ACTION;
//////////////////////////////////////////////////////////////////////////


BEGIN_DECL_ACTION(ACTION_REMOVE_MONSTER)// 봉인.
    __PROPERTY(INT, MonsterCode);       // 몬스터코드
    __PROPERTY(BYTE, IsExpItemDrop);    // 경험치및아이템드럽 지급 (0: 안한다. 1: 한다)
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_ADD_EXP)
    __PROPERTY(BYTE, IsParty);  // 개인이냐 파티냐 (0 : 개인, 1:파티)
    __PROPERTY(DWORD, Exp);     // Exp 주는 경험치.
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_MONSTER_STAT_UP_OR_DOWN)
public: 
    enum
    {
        eOPERATION_MINUS = 0,                   // "-"
        eOPERATION_PLUS  = 1,                   // "+"
    };

     __PROPERTY(INT, MonsterCode);  // 몬스터코드
     __PROPERTY(WORD, AbilityID);   // 어빌리티 ID
     __PROPERTY(int, AbilityParameter1); // 타입설정 : 0. ??, 1. 수치, 2. 현재치 대비 비율, 3. 최대치 대비 비율, 4. 상대 방어력, 5. 수치가 비율값인 경우(이동속도율)
     __PROPERTY(int, AbilityParameter2); // 수치/비율값
     __PROPERTY(BYTE, OperationType);   // 연산자 1 : + , 0 : - 
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_RANDUM_TRIGER_ON_OR_OFF)
public:
    enum 
    { 
        _TRIGGER_STATE_OFF  = 0,
        _TRIGGER_STATE_ON   = 1,
        _MAX_TRIGGER_ID     = 8 
    };      

    __ARRAY_PROPERTY(DWORD, _MAX_TRIGGER_ID, WzTriggerID);
    __PROPERTY(BYTE, TriggerState);

END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_PLAY_MUSIC)
public: 
    enum
    {
        TARGET_PERSONAL         = 0,
        TARGET_FIELD            = 1,
    };

    __PROPERTY(BYTE, Target);                   // (0: 개인 1: 필드전체)
    __PROPERTY(INT, MusCode);                   // (뮤직코드)
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_DOWN_OBJETC_HP)
public:                                             // 맵 오브젝트의 HP를 %로 깍는다. (map_object 에 연결된 NPC)
    __PROPERTY(DWORD, dwObjKey);                    // mapobjectkey
    __PROPERTY(BYTE,  btHPPercentage);          // 하락 수치
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CHANGE_ATTACK_KIND)      // 봉인. 
public:                                             // 타인이 자신을 공격가능[불가능] 하게 만든다. (map_object 에 연결된 NPC)
    enum
    {
        ATTACK_IMPOSSIBILITY = 0,
        ATTACK_POSSIBILITY   = 1,
    };

    __PROPERTY(DWORD, dwObjKey);                    // mapobjectkey
    __PROPERTY(BYTE,  btPossibility);               // 0 : 공격 불가, 1 : 공격 가능
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_ACTIVE_STAMP)
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_ATTACK_OBJECTKIND)       // ACTION_CHANGE_ATTACK_KIND 확장
public:                                             //[몬스터/유닛/오브젝트]를 공격 할 수 [있게/없게] 만든다.
    __PROPERTY(BYTE,  byObjectType);                // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // 타입에 따라 변화.
    __PROPERTY(BYTE,  byAttack);                    // eACTION_ATTACK_TYPE
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_ACTIVE_SKILL_OBJECTKIND)
public:                                             //[몬스터/유닛/오브젝트]가 [스킬ID]스킬을 발동시킨다.
    __PROPERTY(BYTE,  byObjectType);                // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // 타입에 따라 변화.
    __PROPERTY(DWORD, dwSkillCode);             // 발동 스킬 코드
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_SHOW_MESSAGEBOX_OBJECTKIND)
public:                                             //[몬스터/유닛/오브젝트]의 위치에 [메시지코드]메시지를 보여준다.
    __PROPERTY(BYTE,  byObjectType);                // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // 타입에 따라 변화.
    __PROPERTY(DWORD, dwMessageCode);               // 출력할 메시지코드
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_SET_ATTACK_STATE_OBJECTKIND)
public:                                             //[몬스터/유닛]가 [오브젝트]를 [공격/멈춤]한다.
    __PROPERTY(BYTE,  byObjectType);                // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // 타입에 따라 변화.
    __PROPERTY(BYTE,  byDestObjectType);            // 대상 eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwDestObjectKey);         // 대상 타입에 따라 변화.
    __PROPERTY(BYTE,  byAttack);                    // eACTION_ATTACK_TYPE
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_DESTROY_OBJECTKIND)      // ACTION_REMOVE_MONSTER 확장
public:                                             // [몬스터/유닛/오브젝트]를 제거하며 보상을 [지급/미지급]한다.
    enum
    {
        NOT_REWARD = 0,
        REWARD     = 1,
    };

    __PROPERTY(BYTE,  byObjectType);                // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // 타입에 따라 변화.
    __PROPERTY(BYTE,  byReward);                    // 경험치및아이템드럽 지급 (0: 안한다. 1: 한다)
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CREATE_MONSTER_LINKED_MAPOBJECT)     
public:                                            
    __PROPERTY(DWORD,  dwObjectKey);               // 오브젝트ID
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CONTROL_TIMER)
public:   
    __PROPERTY(DWORD, dwTime);               // 시간 (ms), 생성시 초기시간, 파괴시 유지 시간(delay)
    __PROPERTY(BYTE,  byControlType);          // 변동 eTIMER_CONTROL_TYPE
END_DECL_ACTION;                                // (0:역방향 생성(감소), 1:정방향 생성(증가), 2:파괴)

//_NA_0_20100705_DOMINATION_EVENT
BEGIN_DECL_ACTION(ACTION_DO_COMMAND)
public:   
    enum
    {
        END_REGEN_EVENT = 0,
        MAX_COMMAND_TYPE,
    };

    __PROPERTY(BYTE,  byCommandType);          // 메시지 종류 
END_DECL_ACTION;                               

// added by _NA_0_20100817_HELLON_LAST_TRIGGER
BEGIN_DECL_ACTION(ACTION_MOVE_CONTROL)     
public:   
    __PROPERTY(BYTE,  byObjectType);                // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // 타입에 따라 변화.
    __PROPERTY(BYTE,  byMove);                  // eACTION_MOVE_TYPE
END_DECL_ACTION;                               

// added by _NA_0_20100817_HELLON_LAST_TRIGGER
BEGIN_DECL_ACTION(ACTION_ANIMATION_CONTROL)     
public:   
    enum
    {
        LOOP = 0,
        ONCE = 1,
    };

    __PROPERTY(BYTE,  byObjectType);        // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);         // 타입에 따라 변화.
    __PROPERTY(DWORD, dwAnimationCode1);    // 동작 코드1
    __PROPERTY(BYTE,  byLoop1);             //동작 코드1 의 (루프/1회) 여부       
    __PROPERTY(DWORD, dwAnimationCode2);    // 동작 코드2
    __PROPERTY(BYTE,  byLoop2);             //동작 코드2 의 (루프/1회) 여부      
END_DECL_ACTION;

// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
// 몬스터/유닛/오브젝트에게 데미지를 적용한다.
BEGIN_DECL_ACTION(ACTION_NPC_APPLY_DAMAGE)
public:
    __PROPERTY(BYTE, ObjectType); // 객체 종류
    __PROPERTY(DWORD, ObjectKey); // 객체 종류에 따라 변경
    __PROPERTY(INT, Damage); // 고정형 데미지
END_DECL_ACTION;

//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
//미션내 채집 트리거
BEGIN_DECL_ACTION(ACTION_COMPLETE_COLLECTION)
public:
    enum 
    { 
        kMaxCollectObject = 10,
    };      
    __ARRAY_PROPERTY(CODETYPE, kMaxCollectObject, ObjectKey); // 객체 종류에 따라 변경
    __PROPERTY(BYTE, EnableType); // 활성화여부(0:비활성, 1:활성)
END_DECL_ACTION;

// _NA_000000_20130104_ADD_ACTION_TRIGGER_APPLY_HPMPSD
// [개인/전체]에게 [HP/MP/SD]를 [수치]만큼 [증가/감소]를 적용
BEGIN_DECL_ACTION(ACTION_APPLY_HPMPSD)
public:
    enum
    {
        kPerson = 0,
        kField = 1,
        kTeam = 2
    };
    enum
    {
        kHP = 0,
        KMP = 1,
        KSD = 2
    };
    enum
    {
        kIncrease = 0,
        kDecrease = 1
    };
    __PROPERTY(BYTE, TargetType); // TargetType
    __PROPERTY(int, TeamKey); // 증가/감소
    __PROPERTY(BYTE, PointType); // HP/MP/SD
    __PROPERTY(DWORD, Figure);   // 수치
    __PROPERTY(BYTE, ApplyType); // 증가/감소
END_DECL_ACTION;

// _NA_006658_20130422_ADD_ACTION_TRIGGER_CHANGE_OBJECT_HP
// [ObjectKey]오브젝트의 HP를 [비율/수치] [숫자]만큼 [내린다/올린다]
BEGIN_DECL_ACTION(ACTION_CHANGE_OBJECT_HP)
public:
    enum
    {
        kPersentage = 0,
        kValue = 1,
    };
    enum
    {
        kDecrease = 0,
        kIncrease = 1
    };
    __PROPERTY(DWORD, Objectkey);   // 대상 오브젝트키
    __PROPERTY(BYTE, CalcType);     // 비율/수치
    __PROPERTY(DWORD, Figure);      // 값
    __PROPERTY(BYTE, ApplyType);    // 증감여부
END_DECL_ACTION;

// _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
// 레이더맵(미니맵)을 변경한다.
BEGIN_DECL_ACTION(ACTION_CHANGE_RADAR_MAP)  
public:
    __PROPERTY(WzID, SubMapID); // 서브맵ID
END_DECL_ACTION;


// 대상의 범위내에 있는 플레이어에게 상태를 지속시간동안 부여한다
BEGIN_DECL_ACTION(ACTION_ATTACH_STATE_RANGE)  
public:
    __PROPERTY(BYTE,  byObjectType);            // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // 타입에 따라 변화.
    __PROPERTY(FLOAT, fRange);                  // 범위
    __PROPERTY(DWORD, dwStateCode);             // 부여할 상태코드
    __PROPERTY(DWORD, dwTimeDuration);          // 상태의 지속시간
    __PROPERTY(BYTE,  byApplyType);             // 0:부여한다, 1:제거한다
END_DECL_ACTION;

// [파티/개인]의 HP를 [비율/수치] [숫자]만큼 [내린다/올린다]
BEGIN_DECL_ACTION(ACTION_CHANGE_PLAYER_HP)  
public:
    enum
    {
        kTargetParty = 0,
        kTargetPerson,
    };
    enum
    {
        kPersentage = 0,
        kValue = 1,
    };
    enum
    {
        kDecrease = 0,
        kIncrease = 1
    };
    __PROPERTY(BYTE, TargetType);   // 타겟타입
    __PROPERTY(BYTE, CalcType);     // 비율/수치 figure의 타입
    __PROPERTY(DWORD, Figure);      // 비율/값
    __PROPERTY(BYTE, ApplyType);    // 증감여부
END_DECL_ACTION;


#endif //SOLARTRIGGER_ACTION_INFO_H