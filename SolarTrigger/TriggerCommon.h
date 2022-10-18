#ifndef _SOLARTRIGGER_COMMON_H
#define _SOLARTRIGGER_COMMON_H
#pragma once

#include <UtilityCommon.h>
#include <MemoryPoolFactory.h>

//--------------------------------------------------------------------------------------------------
// 조건 트리거 타입 
// 3D엔진팀에서 타입 값을 변경될 경우
// 1. enum eCONDITION_TYPE을 변경한다.
// 2. ConditionInfo를 상속받는 하위 클래스를 변경한다.

enum eCONDITION_TYPE
{
	eCONDITION_ALWAYS				= 0,	// 항상                         
	eCONDITION_COMPARE_SWITCH		= 1,	// 스위치 비교                  
	eCONDITION_CLICK_OBJECT			= 2,	// 오브젝트 클릭                
	eCONDITION_DAMAGE_OBJECT		= 3,	// 오브젝트 데미지 입음         
	eCONDITION_COUNT_NPCKILL		= 4,	// 몬스터 사냥 마리수           
	eCONDITION_PASS_TIME			= 5,	// 일정시간이 흐름
	eCONDITION_ENTER_AREA			= 6,	// 특정 영역에 들어감
	eCONDITION_HAVE_ITEM			= 7,	// 아이템을 소유하고 있다
	eCONDITION_MULTI_SWITCH			= 8,	// 다중 스위치 비교
	eCONDITION_MISSION_POINT		= 9,	// 미션 포인트 만족
	eCONDITION_COMPARE_VARIABLE		= 10,	// 변수 비교
	eCONDITION_QUEST_PROGRESS		= 11,	// Quest 완료 체크 트리거
	eCONDITION_CHECK_CLASS_ID		= 12,	// 유저 클래스 설정(의존 트리거)
	eCONDITION_NPCKILL_FOR_UNITID	= 13,	// 몬스터 유닛을 잡으면
	eCONDITION_LIVE_PARTY_MEMBER	= 14,	// 파티원 생존/사망 상태라면
//_NA001385_20090924_DOMINATION_TRIGGER
    eCONDITION_CHECK_OBJECT_HP	    = 15,	// Mapobject의 HP수치가 %이하일경우
    eCONDITION_DESTROY_OBJECT   	= 16,	// mapobject가 파괴 되었다면.
//_NA_0_20100222_UNIT_TRIGGERS_ADD
                                            // 17번 맵툴 여유 공간
    eCONDITION_CHECK_OBJECTKIND_HP  = 18,   // [UnitID,Mapobject]의 HP수치가 %이하일경우
    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
    eCONDITION_NPC_ENTER_AREA       = 19,   // NPC가 특정 영역에 들어갔을 때

    //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
    eCONDITION_COMPLETE_COLLECTION       = 20,   // 미션내 수집 트리거

    // _NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER
    eCONDITION_SERVER_EVENT         = 21,   // 서버이벤트 트리거

    // _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM
    eCONDITION_CHECK_TEAM           = 22,   // 팀 체크(의존 트리거)
	eCONDITION_COMPARE_SWITCH_EX	= 23,	// 스위치(확장형) 비교
    eCONDITION_MONSTER_ENTER_AREA   = 24,   // 몬스터가 특정 영역에 들어갔을 때

    eCONDITION_MAX,	// 에러 검출용으로 쓰임	(define 내부에 넣지 않는다. 작업시 값만 상승)
};

//--------------------------------------------------------------------------------------------------

enum eACTION_TYPE
{
	eACTION_CHANGE_OBJECTANI		= 1001,	// 오브젝트 애니메이션 변경                             (서버-구현X)(클라-구현O)
	eACTION_CHANGE_PATHTILE			= 1002,	// 길찾기 타일 속성 변경                                (서버-구현O)(클라-구현O)
	eACTION_CHANGE_OBJECTSTATE		= 1003,	// 오브젝트 상태 변경                                   (서버-구현X)(클라-구현O)
	eACTION_PLAY_EVENT				= 1004,	// 이벤트 플레이                                        (서버-구현O)(클라-구현O)
	eACTION_SET_ISATTACK			= 1005,	// 공격 가능/불가 설정                                  (서버-구현O)(클라-구현X)
	eACTION_PORTAL_PLAYER			= 1006,	// 플레이어 포탈                                        (서버-구현O)(클라-구현X)
	eACTION_SET_OBJECTTHRUST		= 1007,	// 오브젝트 밀기 설정                                   (서버-구현X)(클라-구현X) X
	eACTION_APPLY_DAMAGE			= 1008,	// 데미지 적용                                          (서버-구현X)(클라-구현X) X
	eACTION_REFLECT_DAMAGE			= 1009,	// 데미지 반사                                          (서버-구현X)(클라-구현X) X
	eACTION_CREATE_MONSTER			= 1010,	// 몬스터 생성                                          (서버-구현O)(클라-구현O)
	eACTION_DISPLAY_MSGBOX			= 1011,	// 메세지 박스 출력                                     (서버-구현O)(클라-구현X)
	eACTION_REMOVE_ITEM				= 1012,	// 인벤토리 아이템 제거                                 (서버-구현O)(클라-구현X)
	eACTION_REWARD_PLAYER			= 1014,	// 플레이어 보상                                        (서버-구현O)(클라-구현X)
	eACTION_CLEAR_MISSION			= 1015,	// 미션 결과                                            (서버-구현O)(클라-구현O)
	eACTION_PORTAL_RANDOM			= 1016,	// 랜덤 포탈                                            (서버-구현O)(클라-구현X)
	eACTION_ACTIVATE_TRIGGER		= 1017,	// 트리거 활성 비활성                                   (서버-구현O)(클라-구현O)
	eACTION_AREA_DAMAGE				= 1018,	// 범위데미지                                           (서버-구현O)(클라-구현X)
	eACTION_OPERATE_SWITCH			= 1019,	// 스위치 연산                                          (서버-구현O)(클라-구현X)
	eACTION_OPERATE_VARIABLE		= 1020,	// 스위치 설정 변경                                     (서버-구현O)(클라-구현X)
	eACTION_ROAD_CONNECT			= 1021,	// 길 연결을 설정/해제한다                              (서버-구현O)(클라-구현O)
	eACTION_CREATE_DIR_MONSTER		= 1022, // 몬스터를 생성한다(방향)                              (서버-구현O)(클라-구현O)
	eACTION_CHANGE_LIGHT			= 1023,	// 라이트 설정을 변경한다                               (서버-구현X)(클라-구현O)
	eACTION_CHANGE_OBJECT_TYPE		= 1024, // 오브젝트 상태를 바꾼다                               (서버-구현X)(클라-구현O)
	eACTION_RANDOM_MAP_MOVE			= 1025, // 랜덤 맵 이동                                         (서버-구현O)(클라-구현X)
	eACTION_DROP_TRESUREBOX			= 1026, // 보물 상자 떨어뜨리기                                 (서버-구현X)(클라-구현X) X

	//eACTION_CREATE_MONSTER,
	eACTION_QUEST_SATISFACTION		= 1027,	// 퀘스트 조건 만족                                     (서버-구현X)(클라-구현X) X
	eACTION_QUEST_GRANT				= 1028,	// 퀘스트 부여                                          (서버-구현O)(클라-구현X)
	eACTION_QUEST_SETUP				= 1029,	// 퀘스트 완료/실패 설정                                (서버-구현O)(클라-구현X)
	//
	eACTION_USE_CATEGORY			= 1031,	// (필드)의 (카테고리)를 사용한다                       (서버-구현X)(클라-구현X) X
	eACTION_HOLD_CHARACTER			= 1032,	// (Character)의 이동을 (시간)동안 막는다               (서버-구현X)(클라-구현X) X
	eACTION_CREATE_MONSTER_GROUP	= 1033,	// 몬스터그룹 생성                                      (서버-구현X)(클라-구현X) X
	eACTION_SHOW_GUIDE_MSGBOX		= 1034,	// 안내메시지박스 출력                                  (서버-구현X)(클라-구현O)
	eACTION_SHOW_OR_HIDE_UNIT		= 1035,	// 유닛 나타남/사라짐                                   (서버-구현X)(클라-구현X) X
	eACTION_SHOW_OR_HIDE_MSGBOX		= 1036,	// 메시지 박스의 나타남/사라짐                          (서버-구현X)(클라-구현O)
	eACTION_RANDOM_AREA_MOVE		= 1038,	// 유저를 영역 이동 시킨다                              (서버-구현O)(클라-구현X)       
	eACTION_RANDOM_MONSTER_SPAWN    = 1039,	// 몬스터 랜덤 스폰                                     (서버-구현O)(클라-구현O)
	eACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR = 1040, //                                                (서버-구현O)(클라-구현X)
	eACTION_REMOVE_MONSTER			= 1041, //                                                      (서버-구현O)(클라-구현X)        
	eACTION_ADD_EXP					= 1042, //                                                      (서버-구현O)(클라-구현X)
	eACTION_MONSTER_STAT_UP_OR_DOWN	= 1043,	// 몬스터의 능력치중 하나를 증가/감소 시킨다.           (서버-구현O)(클라-구현X)
	eACTION_RANDUM_TRIGER_ON_OR_OFF	= 1044,	// 해당 트리거중 하나를 on/off 시킨다.                  (서버-구현O)(클라-구현X)
    //__NA001140_20080128_SOUND_TRIGGER_ADD
	eACTION_PLAY_MUSIC				= 1045,	// 사운드 파일을 재생한다.                              (서버-구현O)(클라-구현X)    
//_NA001385_20090924_DOMINATION_TRIGGER
    eACTION_DOWN_OBJETC_HP			= 1046,	// 맵 오브젝트의 HP를 깍는다.                           (서버-구현O)(클라-구현X)    
    eACTION_CHANGE_ATTACK_KIND      = 1047, // 자신을 공격가능[불가능] 하게 만든다.                 (서버-구현O)(클라-구현O)
    eACTION_ACTIVE_STAMP            = 1048, // 각인 가능 상태로 변경시(에테르 파괴) 각인물의        (서버-구현O)(클라-구현X)
                                            // 활성화 상태 동기화를 위한 이벤트로 사용
                                            // 현재 eCONDITION_DESTROY_OBJECT 상태 일시 동작
    //_NA_0_20100222_UNIT_TRIGGERS_ADD
    eACTION_ATTACK_OBJECTKIND       = 1049,     // [몬스터/유닛/오브젝트]를 공격 할 수 [있게/없게] 만든다 (특정 몬스터 객체가 플레이어에게 공격 당하지 않게 만든다.)
    eACTION_ACTIVE_SKILL_OBJECTKIND = 1050,     // [몬스터/유닛/오브젝트]가 [스킬ID]스킬을 발동시킨다.
    eACTION_SHOW_MESSAGEBOX_OBJECTKIND  = 1051, // [몬스터/유닛/오브젝트]의 위치에 [메시지코드]메시지를 보여준다.
    eACTION_SET_ATTACK_STATE_OBJECTKIND = 1052, // [몬스터/유닛]가 [몬스터]를 [공격/멈춤]한다.
    eACTION_DESTROY_OBJECTKIND = 1053,          // [몬스터/유닛/오브젝트]를 제거하며 보상을 [지급/미지급]한다.
    eACTION_CREATE_MONSTER_LINKED_MAPOBJECT = 1054, // [오브젝트]를 생성한다. (MapObject에 연결된 Npc 생성)
// _NA_0_20100525_ADD_TIMER_ACTION_TRIGGER
    eACTION_CONTROL_TIMER           = 1055,     // 시계를 보여준다.
//_NA_0_20100616_DOMINATION_EVENT
    eACTION_DO_COMMAND              = 1056,     // 타입에 해당 하는 동작을 실행(트리거를 통한 메시지 전달)
//_NA_0_20100817_HELLON_LAST_TRIGGER
    eACTION_MOVE_CONTROL            = 1057,
    eACTION_ANIMATION_CONTROL       = 1058,
    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
    eACTION_NPC_APPLY_DAMAGE        = 1059, // 몬스터/유닛/오브젝트에게 데미지를 적용한다.
    //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
    eACTION_COMPLETE_COLLECTION     = 1060, //미션내 채집 트리거
    //_NA_000000_20130104_ADD_ACTION_TRIGGER_APPLY_HPMPSD
    eACTION_APPLY_HPMPSD            = 1061, // [개인/전체]에게 [HP/MP/SD]를 [수치]만큼 [증가/감소]를 적용
    //_NA_006658_20130422_ADD_ACTION_TRIGGER_CHANGE_OBJECT_HP
    eACTION_CHANGE_OBJECT_HP        = 1062, // [ObjectKey]오브젝트의 HP를 [비율/수치] [숫자]만큼 [내린다/올린다]
    //_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
    eACTION_CHANGE_RADAR_MAP        = 1063, // 레이더맵(미니맵)을 변경한다.
    eACTION_ATTACH_STATE_RANGE      = 1064, // 대상의 범위내에 상태를 지속시간동안 부여한다.
    eACTION_CHANGE_PLAYER_HP        = 1065, // [파티/개인]의 HP를 [비율/수치] [숫자]만큼 [내린다/올린다]
	eACTION_OPERATE_SWITCH_EX		= 1066,	// 스위치 연산(확장형)
    eACTION_NPC_RANDOM_AREA_MOVE    = 1067,	// NPC 랜덤 영역 이동
	
    eACTION_MAX,							// 에러 검출용으로 쓰임	(define 내부에 넣지 않는다. 작업시 값만 상승)
};

//--------------------------------------------------------------------------------------------------
// 비교 종류
enum eCOMPARE_TYPE
{
	eCOMPARE_EQUAL = 0,				// "=="
	eCOMPARE_MORE,					// ">"
	eCOMPARE_LESS,					// "<"
	eCOMPARE_LESSTHAN,				// "<="
	eCOMPARE_MORETHAN,				// ">="
	eCOMPARE_NOTEQUAL,				// "!="
};	

// 연산 종류
enum eOPERATION_TYPE
{
	eOPERATION_EQUAL = 0,				// "="
	eOPERATION_PLUS,					// "+"
	eOPERATION_MINUS,					// "-"
	eOPERATION_MULTIPLE,				// "*"
	eOPERATION_DIVIDE,					// "/"
};

// _NA_0_20100222_UNIT_TRIGGERS_ADD
enum eOBJECTCODE_TYPE
{ 
    NPCCODE_TYPE = 0, 
    UNITID_TYPE  = 1, 
    MAPNPC_TYPE  = 2, 
    NON_TYPE     = 3,   // _NA_0_20100817_HELLON_LAST_TRIGGER
};     

enum eACTION_ATTACK_TYPE
{
    ATTACK_IMPOSSIBILITY = 0,
    ATTACK_POSSIBILITY   = 1,
};

//_NA_0_20100817_HELLON_LAST_TRIGGER
enum eACTION_MOVE_TYPE
{
    MOVE_IMPOSSIBILITY = 0,
    MOVE_POSSIBILITY   = 1,
};

//_NA_0_20100525_ADD_TIMER_ACTION_TRIGGER
enum eTIMER_CONTROL_TYPE
{
    eTIMER_CREATE_DECREASE = 0,    //  타이머 생성후 시간값 감소.
    eTIMER_CREATE_INCREASE = 1,    //  타이머 생성후 시간값 증가.
    eTIMER_DESTROY         = 2,    //  일정 시간 ui 유지후 파괴.
};


// 사용하지 않는 값
#define INVALID_SWITCH_ID_VALUE		255

//--------------------------------------------------------------------------------------------------
#if !defined(_SERVER)

template<typename _TYPE>
class TRIGGER_POOL
{
public:
    enum {
        eDEFUALT_TRIGGER_POOL_SIZE = 10,
    };

    TRIGGER_POOL(INT iSize = eDEFUALT_TRIGGER_POOL_SIZE)
    {
    #ifndef _SERVER
        m_pool.Initialize(iSize);
    #else
        m_pool.Initialize(iSize, iSize, "TRIGGER_POOL", AssertionLogger4Pool_Server);
    #endif //
    }

    ~TRIGGER_POOL()
    {
        m_pool.Release();
    }

    _TYPE * Alloc()
    {
        return (_TYPE*)m_pool.Alloc();
    }

    void Free(_TYPE* pType)
    {
        m_pool.Free(pType);
    }

private:
    util::CMemoryPoolFactory<_TYPE> m_pool;
};

#define __DECLARE_POOL(CLASSNAME)                     \
    private:                                            \
        static TRIGGER_POOL<CLASSNAME>      m_Pool;     \
    public:                                             \
        static CLASSNAME* Create()                      \
        {                                               \
            return m_Pool.Alloc();                      \
        }                                               \
        static void Destroy(CLASSNAME* pObject)       \
        {                                               \
            m_Pool.Free(pObject);                       \
        }

#define __IMPL_POOL(CLASSNAME)                        \
        TRIGGER_POOL<CLASSNAME> CLASSNAME::m_Pool;

//--------------------------------------------------------------------------------------------------
#elif defined(_SERVER)

#define __DECLARE_POOL(class_name)                      \
    public:                                             \
    static class_name* Create() {                       \
        return TAllocNew(class_name);                   \
    }                                                   \
    static void Destroy(class_name* object_ptr) {       \
        TAllocDelete(class_name, object_ptr);           \
    }

#define __IMPL_POOL(class_name)


#endif
//--------------------------------------------------------------------------------------------------

#endif //_SOLARTRIGGER_COMMON_H
