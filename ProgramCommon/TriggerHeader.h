#ifndef __TRIGGER_HEADER_H__
#define __TRIGGER_HEADER_H__

#pragma once

#include <Struct.h>

#pragma pack(push,1)

enum eTRIGGER_TYPE
{
	eTRIGGER_CONDITION							= 0,	// 컨디션
	eTRIGGER_ACTION								= 1,	// 액션
};

// 조건 트리거 타입  => 3DEngine팀에서 트리거 에디터 작업이 이미 아래와 같은 아이디로 되어 있는 상태(2005.4.12)
enum eTRIGGER_CONDITION_TYPE
{
	eTRIGGER_ALWAYS_CONDITION					= 0,	// 항상
	eTRIGGER_COMPARE_SWITCH_CONDITION			= 1,	// 스위치 비교
	eTRIGGER_CLICK_OBJECT_CONDITION             = 2,	// 오브젝트 클릭
	eTRIGGER_DAMAGE_OBJECT_CONDITION            = 3,	// 오브젝트 데미지 입음
	eTRIGGER_COUNT_NPCKILL_CONDITION            = 4,	// 몬스터 사냥 마리수
	eTRIGGER_PASS_TIME_CONDITION                = 5,	// 일정시간이 흐름
	eTRIGGER_ENTER_AREA_CONDITION               = 6,	// 특정 영역에 들어감
	eTRIGGER_OBTAIN_ITEM_CONDITION              = 7,	// 아이템 획득
	eTRIGGER_MULTI_SWITCH_CONDITION             = 8,	// 다중 스위치 비교
	eTRIGGER_MISSION_POINT_CONDITION            = 9,	// 미션 포인트 만족
	eTRIGGER_VARIABLE_COMPARE_CONDITION         = 10,   // 변수비교
	eTRIGGER_TEST_QUEST_FLAG_CONDITION          = 11,   // 퀘스트 완료 체크
	eTRIGGER_USER_CONDITION                     = 12,   // 유저 직접 설정
	eTRIGGER_MONSTER_KILL	                    = 13,   // 몬스터 유닛을 잡으면
    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
    eTRIGGER_NPC_ENTER_AREA_CONDITION           = 14,   // NPC가 특정 영역에 들어갔다면
    //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
    eTRIGGER_COMPLETE_COLLECTION_CONDITION           = 15,   // 미션내 채집 트리거
};

// 행위 트리거 타입 => 3DEngine팀에서 트리거 에디터 작업이 이미 아래와 같은 아이디로 되어 있는 상태(2005.4.12)
enum eTRIGGER_ACTION_TYPE
{
	eTRIGGER_CHANGE_SWITCH_ACTION               = 1000,	// 스위치 변경
	eTRIGGER_CHANGE_OBJECTANI_ACTION            = 1001,	// 오브젝트 애니메이션 변경
	eTRIGGER_CHANGE_PATHTILE_ACTION             = 1002,	// 길찾기 타일 속성 변경
	eTRIGGER_CHANGE_OBJECTSTATE_ACTION          = 1003,	// 오브젝트 상태 변경
	eTRIGGER_PLAY_EVENT_ACTION                  = 1004,	// 이벤트 플레이
	eTRIGGER_SET_ISATTACK_ACTION                = 1005,	// 공격 가능/불가 설정
	eTRIGGER_PORTAL_PLAYER_ACTION               = 1006,	// 플레이어 포탈
	eTRIGGER_SET_OBJECTTHRUST_ACTION            = 1007,	// 오브젝트 밀기 설정
	eTRIGGER_APPLY_DAMAGE_ACTION                = 1008,	// 데미지 적용
	eTRIGGER_REFLECT_DAMAGE_ACTION              = 1009,	// 데미지 반사
	eTRIGGER_CREATE_MONSTER_ACTION              = 1010,	// 몬스터 생성
	eTRIGGER_DISPLAY_MSGBOX_ACTION              = 1011,	// 메세지 박스 출력
	eTRIGGER_REMOVE_ITEM_ACTION                 = 1012,	// 인벤토리 아이템 제거
	eTRIGGER_PRESERVER_ACTION                   = 1013,	// 지속
	eTRIGGER_REWARD_PLAYER_ACTION               = 1014,	// 플레이어 보상 
	eTRIGGER_CLEAR_MISSION_ACTION               = 1015,	// 미션 결과
	eTRIGGER_PORTAL_RANDOM_ACTION               = 1016,	// 랜덤 포탈
	eTRIGGER_ACTIVATE_TRIGGER_ACTION            = 1017,	// 트리거 활성 비활성
	eTRIGGER_AREA_DAMAGE_ACTION                 = 1018,	// 범위데미지
	eTRIGGER_OPERATE_SWITCH_ACTION              = 1019,	// 스위치 연산
	eTRIGGER_OPERATE_VARIABLE_ACTION            = 1020,	// 변수 연산
	eTRIGGER_AREA_CHANGE_PATHTILE_ACTION        = 1021,	// 영역 타일 속성변경 
	eTRIGGER_CREATE_MONSTER_WITH_DIRECTION      = 1022,	// 몬스터 생성 (방향)
	eTRIGGER_CHANGE_LIGHT                       = 1023,	// 광원 설정 변경
	eTRIGGER_CHANGE_OBJECT_TYPE                 = 1024,	// 오브젝트 상태를 바꾼다.
	eTRIGGER_RANDOMMAP_MOVE                     = 1025, // 랜덤맵 이동
	eTRIGGER_DROP_TREASURE                      = 1026, // 상자 떨구기
	eTRIGGER_QUEST_CONDITION_COMPLETE_ACTION    = 1027, // 퀘 조건 만족
	eTRIGGER_GIVE_QUEST_ACTION                  = 1028, // 퀘 주기
	eTRIGGER_SET_QUEST_COMPLETE_FLAG_ACTION     = 1029, // 퀘스트 성공 실패
	eTRIGGER_USER_ACTION                        = 1030, // 유저 액션
	eTRIGGER_USE_CATEGORY_ACTION                = 1031, // 카테고리 사용
	eTRIGGER_SET_CANNOT_MOVE_ACTION             = 1032, // 이동 불가 설정
	eTRIGGER_CREATE_MONSTER_GROUP_ACTION        = 1033, // 몬스터 그룹 생성
	eTRIGGER_NOTICE_MESSAGE_BOX_ACTION          = 1034, // 안내 메시지 박스
	eTRIGGER_SHOW_MONSTERUNIT_ACTION			= 1035, // 유닛 나타남 / 사라짐
	eTRIGGER_SHOW_MESSAGE_BOX_ACTION			= 1036, // 메시지 박스를 나타나게 / 사라지게 한다.
	eTRIGGER_SKILL_GENERATE_ACTION				= 1037, // 스킬발생 트리거.
	eTRIGGER_TERRITORY_MOVE_ACTION				= 1038, // 영역 이동 트리거.
	eTRIGGER_MON_RANDOM_SPAWN_ACTION			= 1039, // 몬스터 랜덤 스폰.
	eTRIGGER_SET_LAYER							= 1040, // [?]층을 보여준다.
};


// 연산 종류
enum eTRIGGER_OPERATION_TYPE
{
	eTRIGGER_OPERATION_EQUAL = 0,				// "=="
	eTRIGGER_OPERATION_MORE,					// ">"
	eTRIGGER_OPERATION_LESS,					// "<"
	eTRIGGER_OPERATION_LESSTHAN,				// "<="
	eTRIGGER_OPERATION_MORETHAN,				// ">="
	eTRIGGER_OPERATION_NOTEQUAL,				// "!="
};

#define	INVALID_SWITCH_ID_VALUE			255

//////////////////////////////////////////////////////////////////////////
// Trigger Struct Define Start
//////////////////////////////////////////////////////////////////////////

struct TRIGGER_TYPE
{
	WORD				m_wTriggerType;		// 트리거 종류
	int                 m_iIDIndex;
};

// 조건 트리거 : 무조건
struct TRIGGER_ALWAYS_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_ALWAYS_CONDITION() { m_wTriggerType = eTRIGGER_ALWAYS_CONDITION; }
};

// 조건 트리거 : 스위치 비교			=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_COMPARE_SWITCH_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_COMPARE_SWITCH_CONDITION() { m_wTriggerType = eTRIGGER_COMPARE_SWITCH_CONDITION; }
	BYTE				m_bySwitchID;			// 스위치 아이디
	BYTE				m_byOperationType;		// 연산종류
	int					m_iValue;				// 값
};

// 조건 트리거 : 오브젝트 선택
struct TRIGGER_CLICK_OBJECT_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_CLICK_OBJECT_CONDITION() { m_wTriggerType = eTRIGGER_CLICK_OBJECT_CONDITION; }
	int					m_iObjectID;			// 오브젝트 아이디
};

// 조건 트리거 : 오브젝트 데미지 입음
struct TRIGGER_DAMAGE_OBJECT_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_DAMAGE_OBJECT_CONDITION() { m_wTriggerType = eTRIGGER_DAMAGE_OBJECT_CONDITION; }
	int					m_iObjectID;			// 오브젝트 아이디
	float				m_fDamage;				// 데미지
};

// 조건 트리거 : 몬스터 죽인 일정 마리 죽음
struct TRIGGER_COUNT_NPCKILL_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_COUNT_NPCKILL_CONDITION() { m_wTriggerType = eTRIGGER_COUNT_NPCKILL_CONDITION; }
	int					m_iAreaID;				// 영역 아이디
	int					m_iMonsterType;			// 몬스터 종류
	BYTE				m_byIsParty;			// 파티여부(개인 0, 파티 1)
	int					m_iKillCount;			// 몬스터 사냥 마리수
	BYTE                m_byMethod;             // 방법 (0 : 일반 1 : 누적)
};

// 조건 트리거 : 일정시간 흐름			=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_PASS_TIME_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_PASS_TIME_CONDITION() { m_wTriggerType = eTRIGGER_PASS_TIME_CONDITION; }
	int					m_iPassTime;				// 흐르는 시간
};

// 조건 트리거 : 영역에 들어감
struct TRIGGER_ENTER_AREA_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_ENTER_AREA_CONDITION() { m_wTriggerType = eTRIGGER_ENTER_AREA_CONDITION; }
	int					m_iAreaID;				// 영역 아이디
	int					m_iEnterCount;			// 들어간 객체의 수
	BYTE                m_bIsParty;             // 파티? 개인?
};

// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
// NPC가 특정 영역에 들어갔다면
struct TRIGGER_NPC_ENTER_AREA_CONDITION : public TRIGGER_TYPE 
{
    TRIGGER_NPC_ENTER_AREA_CONDITION() : area_id(0), npc_unit_id(0)
    {
        m_wTriggerType = eTRIGGER_NPC_ENTER_AREA_CONDITION;
    }
    CODETYPE area_id;
    CODETYPE npc_unit_id;
};

//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
struct TRIGGER_COMPLETE_COLLECTION_CONDITION : public TRIGGER_TYPE 
{
    TRIGGER_COMPLETE_COLLECTION_CONDITION() : object_key(0)
    {
        m_wTriggerType = eTRIGGER_COMPLETE_COLLECTION_CONDITION;
    }
    CODETYPE object_key;
};

// 조건 트리거 : 아이템 획득			=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_OBTAIN_ITEM_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_OBTAIN_ITEM_CONDITION() { m_wTriggerType = eTRIGGER_OBTAIN_ITEM_CONDITION; }
	int					m_iItemCode;			// 아이템 코드
	int                 m_iNumber;
};

// 조건 트리거 : 다중 스위치
struct TRIGGER_MULTI_SWITCH_CONDITION : public TRIGGER_TYPE
{
	enum { _MAX_SWITCH_ID_NUM = 10	};
	TRIGGER_MULTI_SWITCH_CONDITION() { m_wTriggerType = eTRIGGER_MULTI_SWITCH_CONDITION; }

	BYTE				m_byaSwitchID[_MAX_SWITCH_ID_NUM];	// 스위치 아이디들
	BYTE				m_byOperationType1;					// 연산종류1
	int					m_iValue1;							// 값1
	BYTE				m_byOperationType2;					// 연산종류2
	int					m_iValue2;							// 값2
};

// 조건 트리거 : 미션 포인트 만족		=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_MISSION_POINT_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_MISSION_POINT_CONDITION() { m_wTriggerType = eTRIGGER_MISSION_POINT_CONDITION; }
	//int					m_iAreaID;				// 영역 아이디
	int					m_iMissionPoint;		// 만족시킬 미션 포인트
	BYTE                m_bCondition;
};

// 조건 트리거 : 변수 비교
struct TRIGGER_VARIABLE_COMPARE_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_VARIABLE_COMPARE_CONDITION() { m_wTriggerType = eTRIGGER_VARIABLE_COMPARE_CONDITION; }
	DWORD               m_dwVariableID;
	BYTE                m_bOperator;
	int                 m_iParam;
};

struct TRIGGER_TEST_QUEST_FLAG_CONDITION   : public TRIGGER_TYPE
{
	TRIGGER_TEST_QUEST_FLAG_CONDITION() { m_wTriggerType = eTRIGGER_TEST_QUEST_FLAG_CONDITION; }
	ULONG              m_ulQuestID;
	BYTE               m_lScriptCode;
	

};

struct TRIGGER_USER_CONDITION              : public TRIGGER_TYPE
{
	TRIGGER_USER_CONDITION() { m_wTriggerType = eTRIGGER_USER_CONDITION; }
	int               m_iUser;	
};

struct TRIGGER_MONSTER_KILL              : public TRIGGER_TYPE
{
	TRIGGER_MONSTER_KILL()	{ m_wTriggerType = eTRIGGER_MONSTER_KILL; }
	WzID			m_wID;			//	대상 유닛의 WzID
};


////////////////////////////////////////////////////////////////////////////////////////

// 행위 트리거 : 스위치 설정 변경		=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_CHANGE_SWITCH_ACTION : public TRIGGER_TYPE
{
	TRIGGER_CHANGE_SWITCH_ACTION() { m_wTriggerType = eTRIGGER_CHANGE_SWITCH_ACTION; }
	BYTE				m_bySwitchID;			// 스위치 아이디
	int					m_iValue;				// 값
};

// 행위 트리거 : 오브젝트 애니메이션 변경
struct TRIGGER_CHANGE_OBJECTANI_ACTION : public TRIGGER_TYPE
{
	enum eORDER { RIGHT = 0, REVERSE = 1, };
	TRIGGER_CHANGE_OBJECTANI_ACTION() { m_wTriggerType = eTRIGGER_CHANGE_OBJECTANI_ACTION; }
	int					m_iObjectID;			// 오브젝트 아이디
	DWORD				m_dwAniID;				// 애니메이션 아이디
	BYTE				m_byDirection;			// 방향(정방향 0, 역방향 1) 
};

// 행위 트리거 : 타일속성 변경
struct TRIGGER_CHANGE_PATHTILE_ACTION : public TRIGGER_TYPE
{
	enum eFLAG { ENABLE = 0, DISABLE = 1 };
	TRIGGER_CHANGE_PATHTILE_ACTION() { m_wTriggerType = eTRIGGER_CHANGE_PATHTILE_ACTION; }
	int					m_iTileID;				// 타일 아이디
	BYTE				m_byAttribute;			// 속성
};

// 행위 트리거 : 오브젝트 상태 변경		=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_CHANGE_OBJECTSTATE_ACTION : public TRIGGER_TYPE
{
	enum eSTATE { APPEAR = 0, DISAPPEAR = 1, REMOVE = 2 };
	TRIGGER_CHANGE_OBJECTSTATE_ACTION() { m_wTriggerType = eTRIGGER_CHANGE_OBJECTSTATE_ACTION; }
	int					m_iObjectID;			// 오브젝트 아이디
	BYTE				m_byState;				// 상태
};

// 행위 트리거 : 이벤트 플레이
struct TRIGGER_PLAY_EVENT_ACTION : public TRIGGER_TYPE
{
	TRIGGER_PLAY_EVENT_ACTION() { m_wTriggerType = eTRIGGER_PLAY_EVENT_ACTION; }
	int					m_iEventID;				// 이벤트 아이디
};

// 행위 트리거 : 공격여부 설정			=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_SET_ISATTACK_ACTION : public TRIGGER_TYPE
{
	TRIGGER_SET_ISATTACK_ACTION() { m_wTriggerType = eTRIGGER_SET_ISATTACK_ACTION; }
	BYTE				m_byTargetType;			// 공격대상 종류
	BYTE				m_byIsAttack;			// 공격 가능 여부
};

// 행위 트리거 : 플레이어 포탈
struct TRIGGER_PORTAL_PLAYER_ACTION : public TRIGGER_TYPE
{
	TRIGGER_PORTAL_PLAYER_ACTION() { m_wTriggerType = eTRIGGER_PORTAL_PLAYER_ACTION; }
	int                 m_iMapIndex;            // 맵번호
	int					m_iAreaID;				// 영역 아이디
	BYTE                m_byTarget;             // 이동시킬 대상 (0 : 개인 1: 파티)
	int                 m_iType;           
};

// 행위 트리거 : 오브젝트 밀기설정		=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_SET_OBJECTTHRUST_ACTION : public TRIGGER_TYPE
{
	TRIGGER_SET_OBJECTTHRUST_ACTION() { m_wTriggerType = eTRIGGER_SET_OBJECTTHRUST_ACTION; }
	BYTE				m_byState;
};

// 행위 트리거 : 데미지 적용			=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_APPLY_DAMAGE_ACTION : public TRIGGER_TYPE
{
	TRIGGER_APPLY_DAMAGE_ACTION() { m_wTriggerType = eTRIGGER_APPLY_DAMAGE_ACTION; }
	BYTE				m_byDamageType;
	BYTE				m_byTargetType;
	float				m_fDamage;
};

// 행위 트리거 : 데미지 반사
struct TRIGGER_REFLECT_DAMAGE_ACTION : public TRIGGER_TYPE
{
	TRIGGER_REFLECT_DAMAGE_ACTION() { m_wTriggerType = eTRIGGER_REFLECT_DAMAGE_ACTION; }
	int					m_iObjectID;
	float				m_fDamage;				// 데미지
};


// 행위 트리거 : 몬스터 생성
struct TRIGGER_CREATE_MONSTER_ACTION : public TRIGGER_TYPE
{
	TRIGGER_CREATE_MONSTER_ACTION() { m_wTriggerType = eTRIGGER_CREATE_MONSTER_ACTION; }
	int					m_iMonsterCode;			// 몬스터 코드
	int					m_iAreaID;				// 영역 아이디
	BYTE				m_byIsParty;			// 파티여부
	int					m_iCreateCount;			// 몬스터 생성 갯수
	int					m_iDelayTime;			// 생성 딜레이 시간
	int					m_iLoopCount;			// 몬스터 생성 회수
    BYTE                m_byCreateDelete;       // 생성/제거
	int					m_iType;				// 타입
	
};

// 행위 트리거 : 메세지 박스 출력
struct TRIGGER_DISPLAY_MSGBOX_ACTION : public TRIGGER_TYPE
{
	TRIGGER_DISPLAY_MSGBOX_ACTION() { m_wTriggerType = eTRIGGER_DISPLAY_MSGBOX_ACTION; }
	int					m_iTextID;				// 텍스트 아이디
	BYTE				m_byIsParty;			// 파티여부
	int					m_iDisplayWindow;		// 출력할 창
};

// 행위 트리거 : 인벤토리 아이템 제거		=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_REMOVE_ITEM_ACTION : public TRIGGER_TYPE
{
	TRIGGER_REMOVE_ITEM_ACTION() { m_wTriggerType = eTRIGGER_REMOVE_ITEM_ACTION; }
	int                m_iItemID;
	//BYTE               m_byAll; // (0: 아이템 하나만,1: 아이템 전부)
	int                m_iNumber; // 숫자
};

// 행위 트리거 : 지속				=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_PRESERVER_ACTION : public TRIGGER_TYPE
{
	TRIGGER_PRESERVER_ACTION() { m_wTriggerType = eTRIGGER_PRESERVER_ACTION; }
};

// 행위 트리거 : 플레이어 보상			=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_REWARD_PLAYER_ACTION : public TRIGGER_TYPE
{
	TRIGGER_REWARD_PLAYER_ACTION() { m_wTriggerType = eTRIGGER_REWARD_PLAYER_ACTION; }
	int					m_iItemCode;			// 아이템 코드
	int                 m_iItemCount;           // 아이템 갯수
	BYTE				m_byIsParty;			// 파티여부(개인 0, 파티 1)
	BYTE                m_byDrop;         // 주는방식(0 : 인벤에 넣어서,1 바닥에 떨굼)
};

// 행위 트리거 : 미션 결과
struct TRIGGER_CLEAR_MISSION_ACTION : public TRIGGER_TYPE
{
	TRIGGER_CLEAR_MISSION_ACTION() { m_wTriggerType = eTRIGGER_CLEAR_MISSION_ACTION; }
	BYTE				m_byResultType;
};

// 행위 트리거 : 랜덤 포탈			=> 현재 구현할 필요 없음(2005.04.12)
struct TRIGGER_PORTAL_RANDOM_ACTION : public TRIGGER_TYPE
{
	enum { _MAX_AREA_ID_NUM = 8	};
	TRIGGER_PORTAL_RANDOM_ACTION() { m_wTriggerType = eTRIGGER_PORTAL_RANDOM_ACTION; }
	int                 m_iMapID;
	int                 m_iAreaID[_MAX_AREA_ID_NUM];
	BYTE                m_byRandom; // 랜덤 순차적으로
	BYTE                m_byTarget; // (0: 개인 1: 파티)
};

// 행위 트리거 : 트리거 활성/비활성
struct TRIGGER_ACTIVATE_TRIGGER_ACTION : public TRIGGER_TYPE
{
	TRIGGER_ACTIVATE_TRIGGER_ACTION() { m_wTriggerType = eTRIGGER_ACTIVATE_TRIGGER_ACTION; }
	DWORD m_dwTriggerID; // 트리거 아이디
	BYTE  m_byFlag; // 활성/비활성

};

// 행위 트리거 : 범위 데미지 부여
struct TRIGGER_AREA_DAMAGE_ACTION : public TRIGGER_TYPE
{
	TRIGGER_AREA_DAMAGE_ACTION() { m_wTriggerType = eTRIGGER_AREA_DAMAGE_ACTION; }
	int   m_iObjectIndex; // 오브젝트 ID
	float m_fRange; // 범위
	BYTE  m_byTarget;   // 목표 (개인/파티)
	BYTE  m_byDamageType; // 데미지 타입
	float m_fDamage; // 데미지

};

// 행위 트리거 : 스위치 설정 변경
struct TRIGGER_OPERATE_SWITCH_ACTION : public TRIGGER_TYPE
{
	TRIGGER_OPERATE_SWITCH_ACTION() { m_wTriggerType = eTRIGGER_OPERATE_SWITCH_ACTION; }
	BYTE  m_bySwitchID; // 스위치아이디
	BYTE  m_byOperation; // 연산종류
	int   m_iArgument; // 매개변수

};

// 행위 트리거 : 스위치 설정 변경
struct TRIGGER_OPERATE_VARIABLE_ACTION : public TRIGGER_TYPE
{
	TRIGGER_OPERATE_VARIABLE_ACTION() { m_wTriggerType = eTRIGGER_OPERATE_VARIABLE_ACTION; }
	DWORD  m_dwVariableID; // 변수아이디
	BYTE  m_byOperation; // 연산종류
	int   m_iArgument; // 매개변수

};


// 행위 트리거 : 영역 타일 속성변경 
struct TRIGGER_AREA_CHANGE_PATHTILE_ACTION  : public TRIGGER_TYPE
{
	TRIGGER_AREA_CHANGE_PATHTILE_ACTION () { m_wTriggerType = eTRIGGER_AREA_CHANGE_PATHTILE_ACTION; }
	int   m_iAreaID;  //  영역아이디
	BYTE  m_byCanWalk; // 갈수있음 없음
	

};

// 행위 트리거 : 몬스터 생성 (방향)
struct TRIGGER_CREATE_MONSTER_WITH_DIRECTION : public TRIGGER_TYPE
{
	TRIGGER_CREATE_MONSTER_WITH_DIRECTION() { m_wTriggerType = eTRIGGER_CREATE_MONSTER_WITH_DIRECTION; }
	int m_iMonsterID; // 몹종류
	int m_iAreaID;
	BYTE m_byParty;
	int m_iMonsterCount;
	int m_iTimeInverval;
	int m_iTimeCount;
	WzVector m_vDir;
	int m_iType;

	
};

// 행위 트리거 : 광원 설정 변경
struct TRIGGER_CHANGE_LIGHT : public TRIGGER_TYPE
{
	enum eSTATE { OFF = 0, ON = 1, TOGGLE = 2, }; // 0:끈다 1:켠다 2:토글한다
	TRIGGER_CHANGE_LIGHT() { m_wTriggerType = eTRIGGER_CHANGE_LIGHT; }
	int   m_iLightIndex;
	int   m_iTime;
	BYTE  m_byCondition;

};

// 행위 트리거 : 오브젝트 상태를 바꾼다.
struct TRIGGER_CHANGE_OBJECT_TYPE : public TRIGGER_TYPE
{
	TRIGGER_CHANGE_OBJECT_TYPE() { m_wTriggerType = eTRIGGER_CHANGE_OBJECT_TYPE; }
	int m_iObjectID;
	int m_iTime;
	int m_iType;
	

};

struct TRIGGER_RANDOMMAP_MOVE : public TRIGGER_TYPE
{
	TRIGGER_RANDOMMAP_MOVE() { m_wTriggerType = eTRIGGER_RANDOMMAP_MOVE; }
	int   m_iMapIndex[10];
	BYTE  m_bMethod; // 랜덤 / 순차
	WzID  m_AreaID; // 영역아이디
	BYTE  m_bParty; // 개인 / 파티
	int   m_iType; // 타입

};

struct TRIGGER_DROP_TREASURE : public TRIGGER_TYPE
{
	TRIGGER_DROP_TREASURE() { m_wTriggerType = eTRIGGER_DROP_TREASURE; }
	int   m_iType;
	BYTE  m_bParty;
	BYTE  m_bInventory;

};

struct TRIGGER_QUEST_CONDITION_COMPLETE_ACTION  : public TRIGGER_TYPE  // 퀘 조건 만족
{
	TRIGGER_QUEST_CONDITION_COMPLETE_ACTION() { m_wTriggerType = eTRIGGER_QUEST_CONDITION_COMPLETE_ACTION; }
	ULONG              m_ulQuestID;
	long               m_lScriptCode;

};

struct TRIGGER_GIVE_QUEST_ACTION                : public TRIGGER_TYPE  // 퀘 주기
{
	TRIGGER_GIVE_QUEST_ACTION() { m_wTriggerType = eTRIGGER_GIVE_QUEST_ACTION; }
	ULONG              m_ulQuestID;
	BYTE               m_byParty; // 0 : 개인 1 : 파티
};

struct TRIGGER_SET_QUEST_COMPLETE_FLAG_ACTION   : public TRIGGER_TYPE  // 퀘스트 성공 실패
{
	TRIGGER_SET_QUEST_COMPLETE_FLAG_ACTION() { m_wTriggerType = eTRIGGER_SET_QUEST_COMPLETE_FLAG_ACTION; }

	ULONG              m_ulQuestID;
	BYTE               m_byFailed; // 0 : 완료 1 : 실패

};

struct TRIGGER_USER_ACTION                      : public TRIGGER_TYPE  // 유저 액션
{
	TRIGGER_USER_ACTION()                     { m_wTriggerType = eTRIGGER_USER_ACTION; }

	int               m_iUserID;
};
struct TRIGGER_USE_CATEGORY_ACTION              : public TRIGGER_TYPE  // 카테고리 사용
{
	TRIGGER_USE_CATEGORY_ACTION() { m_wTriggerType = eTRIGGER_USE_CATEGORY_ACTION; }

	int               m_iFieldID;
	char             m_szCategory[128];
};
struct TRIGGER_SET_CANNOT_MOVE_ACTION           : public TRIGGER_TYPE  // 이동 불가 설정
{
	TRIGGER_SET_CANNOT_MOVE_ACTION() { m_wTriggerType = eTRIGGER_SET_CANNOT_MOVE_ACTION; }

	BYTE              m_byTarget; // 0 : 유저 1 : 몹 2 : 둘다
	int               m_iTime;

};
struct TRIGGER_CREATE_MONSTER_GROUP_ACTION      : public TRIGGER_TYPE  // 몬스터 그룹 생성
{
	TRIGGER_CREATE_MONSTER_GROUP_ACTION() { m_wTriggerType = eTRIGGER_CREATE_MONSTER_GROUP_ACTION; }

	int               m_iGroupCode;
	int               m_iAreaID;
	BYTE              m_byParty; // 0 : 개인 1 : 파티
	int               m_iNumber;
};

struct TRIGGER_NOTICE_MESSAGE_BOX_ACTION        : public TRIGGER_TYPE  // 안내 메시지 박스
{
	TRIGGER_NOTICE_MESSAGE_BOX_ACTION() { m_wTriggerType = eTRIGGER_NOTICE_MESSAGE_BOX_ACTION; }
	long               m_lScriptCode;
};

struct TRIGGER_SHOW_MONSTERUNIT_ACTION          : public TRIGGER_TYPE  // 몬스터 유닛 나타나게 사라짐
{
	TRIGGER_SHOW_MONSTERUNIT_ACTION() { m_wTriggerType = eTRIGGER_SHOW_MONSTERUNIT_ACTION; }
	int                m_iUnitID;
};

struct TRIGGER_SHOW_MESSAGE_BOX_ACTION          : public TRIGGER_TYPE  // 메시지 나타나게 / 사라지게
{
	TRIGGER_SHOW_MESSAGE_BOX_ACTION() { m_wTriggerType = eTRIGGER_SHOW_MESSAGE_BOX_ACTION; }
	int					m_iTextID;				// 텍스트 아이디
	BYTE				m_byIsParty;			// 파티여부
	int					m_iDisplayWindow;		// 출력할 창
	int					m_iDisplaySwitch;		// 보여줄 것인가 말것인가?
};

struct TRIGGER_SKILL_GENERATE_ACTION			: public TRIGGER_TYPE
{
	TRIGGER_SKILL_GENERATE_ACTION() {m_wTriggerType = eTRIGGER_SKILL_GENERATE_ACTION; }
	WzID m_wzObject;
	int m_iSkill;
};

struct TRIGGER_TERRITORY_MOVE_ACTION			: public TRIGGER_TYPE
{
	TRIGGER_TERRITORY_MOVE_ACTION() { m_wTriggerType = eTRIGGER_TERRITORY_MOVE_ACTION; }
	int m_iArea[8];
	BYTE m_btWho;
	BYTE m_btRandom;
};

struct TRIGGER_MON_RANDOM_SPAWN_ACTION			: public TRIGGER_TYPE
{
	TRIGGER_MON_RANDOM_SPAWN_ACTION() { m_wTriggerType = eTRIGGER_MON_RANDOM_SPAWN_ACTION; }
	int		 m_iMonID[5];
	int		 m_iMonRate[5];

	int		 m_iArea;
	BYTE	 m_btWho;
	int		 m_iMonMinNum;
	int		 m_iMonMaxNum;
	int		 m_iTime;
	int		 m_iRegenNum;
	WzVector m_wvDir;
};

struct TRIGGER_SET_LAYER			: public TRIGGER_TYPE
{
	TRIGGER_SET_LAYER() { m_wTriggerType = eTRIGGER_SET_LAYER; }
	int					m_iLayer;				// 층 값
};

//////////////////////////////////////////////////////////////////////////
// Trigger  Struct Define End
//////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#endif // __TRIGGER_HEADER_H__
