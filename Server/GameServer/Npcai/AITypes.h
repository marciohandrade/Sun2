#ifndef _AITYPES_H_
#define _AITYPES_H_

#include <ProgramCommon/WzDataType.h>


#pragma pack(push,1)

// (CHANGES) (091216) moved 'eBATTLE_POINT_TYPE' to 'AITypes.h'
/**

NPC�� BattleRecord*�� ���´�.
�������� ������ ���� �����̴�.

*/
//==================================================================================================
enum eBATTLE_POINT_TYPE
{
    BATTLE_POINT_CLASS,         // �÷��̾� Ŭ���� ��׷� ����Ʈ 
    BATTLE_POINT_FIRST,         // ���� ����Ʈ
    BATTLE_POINT_DISTANCE,      // �ּ� �Ÿ� ����Ʈ
    BATTLE_POINT_LEVEL,         // �ּ� ���� ����Ʈ
    BATTLE_POINT_HP,            // �ּ� ü�� ����Ʈ
    BATTLE_POINT_DAMAGE,        // ������ ����Ʈ
    BATTLE_POINT_ETC,           // ��Ÿ ����Ʈ (��, ���� �� �����Ƽ�� ���� ����Ʈ)
    BATTLE_POINT_TOTAL,         // ��ü ����Ʈ
    BATTLE_POINT_MAX
};


// Ÿ�� �˻� Ÿ��
enum eTARGET_SEARCH_TYPE
{
	eRARGET_SEARCH_NEAREST	= 0,	// ���� ����� �� �켱
	eRARGET_SEARCH_LOW_HP,			// ü���� ���� ���� ���
	eRARGET_SEARCH_LOW_LEVEL,		// ������ ���� ���� ���
	eRARGET_SEARCH_HIGH_MP,			// ������ ���� ���� ���
	eRARGET_SEARCH_LOW_HPRATIO,		// ü�� ������ ���� ���� ���
	eRARGET_SEARCH_CORPSE,			// ��ü
	eRARGET_SEARCH_NEAREST_NONPVP,	// ���� ����� �� �켱(�÷��̾� ����)

    // _NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    eRARGET_NOT_SEARCH,	            // �˻� ����(�İ�)
    eRARGET_SEARCH_NONTARGET_LOW_HP,// Ÿ���� ���� ���¿����� ü���� ���� ��� ����
    eRARGET_SEARCH_UNITID,          // �ش� ID ��� �ϳ��� ã�´�.          
    eRARGET_SEARCH_MAPOBJECTID,     // �ش� ID ��� �ϳ��� ã�´�.          
    eRARGET_SEARCH_MONSTERCODE,     // �ش� ���� �ڵ��� ���� ����� �� 
    eRARGET_SEARCH_NOT_SEARCH_AND_TARGET_CHNAGE,  // ���ʰ˻���, Ÿ�ٺ����� ���� �ʴ´�. //_NA001385_20090924_DOMINATION_MAPNPC_AI_BUG
};

enum
{
	ATTACK_ORDER_0_PERCENT		= 0,		// ���� ��ɱ� ����
	ATTACK_ORDER_100_PERCENT	= 1,		// 100�ۼ�Ʈ ���ݸ�� ����
	ATTACK_ORDER_50_PERCENT		= 2,		// 50�ۼ�Ʈ ���ݸ�� ����
};

enum 
{ 
	HELP_REQUEST_NOT			= 0,		// ������û ����
	HELP_REQUEST_HP50_PERCENT	= 1,		// HP�� 50�ۼ�Ʈ �����̸� ������û
};


// Group ���� Command Type
enum
{
	GROUP_CMD_TYPE_ATTACK,
	GROUP_CMD_TYPE_STOP_ATTACK,
};


// (CHANGES) 'ENUM_STATD_ID' moved to 'AITypes.h'
enum ENUM_STATD_ID
{
    STATE_ID_UNKNOWN    = 0,
    STATE_ID_WANDER,
    STATE_ID_IDLE,
    STATE_ID_TRACK,
    STATE_ID_ATTACK,
    STATE_ID_HELP,
    STATE_ID_THRUST,
    STATE_ID_DEAD,
    STATE_ID_FLYING,
    STATE_ID_KNOCKDOWN,
    STATE_ID_JUMP,
    STATE_ID_FALL_APART,
    STATE_ID_RETURN,
    STATE_ID_RETREAT,
    STATE_ID_RUNAWAY,
    STATE_ID_CHAOS,
    STATE_ID_SUMMON_IDLE,
    STATE_ID_PATROL,
    STATE_ID_SPAWN_IDLE,
    STATE_ID_STOP_IDLE,
    STATE_ID_TRACK_AREA,
    STATE_ID_BLIND,
    STATE_ID_PATHLIST,          //_NA_0_20100222_UNIT_TRIGGERS_PATHLIST
    STATE_ID_FOLLOW,
    STATE_ID_NO_TRANS,          //_NA001385_20090924_DOMINATION_MAPNPC_AI_BUG
    STATE_ID_DESTROY_OBJECT,     //_NA_0_20100514_NPC_DEAD_STATE_TIME
    STATE_ID_SPECIAL_DEAD_ACTION,
    STATE_ID_UPPERBOUND,
};

enum ENUM_AI_MSG_ID
{
	AI_MSG_ID_FORCE_ATTACK,
	AI_MSG_ID_ATTACKED,
	AI_MSG_ID_LEAVE_FIELD,
	AI_MSG_ID_HELP_REQUEST,
	AI_MSG_ID_THRUST,
	AI_MSG_ID_FLYING,
	AI_MSG_ID_KNOCKDOWN,
	AI_MSG_ID_STUN,
	AI_MSG_ID_GROUP_C0MMAND,
	AI_MSG_ID_LETSGO,
	AI_MSG_ID_ENEMY_FOUND,
	AI_MSG_ID_RUNAWAY,
	AI_MSG_ID_CHAOS,
	AI_MSG_ID_CHANGE_RETURNSTATE,
	AI_MSG_ID_COMMAND_FOLLOW,
	AI_MSG_ID_USE_SKILL,
	AI_MSG_ID_CHANGE_RETREATSTATE,
	AI_MSG_ID_COMMAND_NONPVP,
	AI_MSG_ID_BLIND,
	AI_MSG_ID_MAX						, //UPPER-BOUND
};

struct AI_MSG
{
	WORD		wSize;
	DWORD		dwMsgID;
	DWORD		dwDeliveryTick;
};

// NPC���� ���ݸ��
struct AI_MSG_FORCE_ATTACK : public AI_MSG
{
	AI_MSG_FORCE_ATTACK() { dwMsgID = AI_MSG_ID_FORCE_ATTACK; }
	DWORD			dwTargetKey;		// ���ݴ��
};

// NPC�� ���ݹ���
struct AI_MSG_ATTACKED : public AI_MSG
{
	AI_MSG_ATTACKED() { dwMsgID = AI_MSG_ID_ATTACKED; }
	DWORD			dwAttackerKey;		// ������ �ִ� ��
	DAMAGETYPE		wDamage;
};

struct AI_MSG_LEAVE_FIELD : public AI_MSG
{
	AI_MSG_LEAVE_FIELD() { dwMsgID = AI_MSG_ID_LEAVE_FIELD; }
	DWORD			dwObjectKey;
};

struct AI_MSG_HELP_REQUEST : public AI_MSG
{
	AI_MSG_HELP_REQUEST() { dwMsgID = AI_MSG_ID_HELP_REQUEST; }
	DWORD			dwAttackerKey;		// �����ؾ��� ���(���� ������ �ִ� ��)
	DWORD			dwTargetKey;		// Help�� ��û�ϴ� NPC
};

struct AI_MSG_THRUST : public AI_MSG
{
	AI_MSG_THRUST() { dwMsgID = AI_MSG_ID_THRUST; }
	BOOL			bDownAfterThrust;
};

struct AI_MSG_FLYING : public AI_MSG
{
	AI_MSG_FLYING() { dwMsgID = AI_MSG_ID_FLYING; }
	DWORD			dwFlyingTick;
};

struct AI_MSG_KNOCKDOWN : public AI_MSG
{
	AI_MSG_KNOCKDOWN() { dwMsgID = AI_MSG_ID_KNOCKDOWN; dwKnockDownTick = 0; }
	DWORD			dwKnockDownTick;
};

struct AI_MSG_STUN : public AI_MSG
{
	AI_MSG_STUN() { dwMsgID = AI_MSG_ID_STUN; dwStunTick = 0; }
	DWORD			dwStunTick;
};

struct AI_MSG_GROUP_C0MMAND : public AI_MSG
{
	AI_MSG_GROUP_C0MMAND() { dwMsgID = AI_MSG_ID_GROUP_C0MMAND; }
	BYTE			byType;				// ��� Ÿ��
	DWORD			dwTargetKey;		// ���ݴ��
};

struct AI_MSG_LETSGO : public AI_MSG
{
	AI_MSG_LETSGO() { dwMsgID = AI_MSG_ID_LETSGO; }
	WzVector		wvDestPos;
	BYTE			byMoveState;
};

struct AI_MSG_CHANGETORETURNSTATE : public AI_MSG
{
	AI_MSG_CHANGETORETURNSTATE() { dwMsgID = AI_MSG_ID_CHANGE_RETURNSTATE; }
};

struct AI_MSG_CHANGETORETREATSTATE : public AI_MSG
{
	AI_MSG_CHANGETORETREATSTATE() { dwMsgID = AI_MSG_ID_CHANGE_RETREATSTATE; }
	float			fMaxDistance;
	BOOL			bIdleAfterRetreat;	// ������ �� ������ ���ΰ�
};

struct AI_MSG_ENEMY_FOUND : public AI_MSG
{
	AI_MSG_ENEMY_FOUND() { dwMsgID = AI_MSG_ID_ENEMY_FOUND; }
	DWORD			dwTargetObjectKey;
};

struct AI_MSG_RUNAWAY : public AI_MSG
{
	AI_MSG_RUNAWAY() { dwMsgID = AI_MSG_ID_RUNAWAY; }
	DWORD			dwTargetKey;		// �����ľ��� ��� ������ƮŰ
	DWORD			dwRunAwayTime;		// ����ġ�� �ð�
	WORD			wStateID;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eABILITY        abilityID;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
};

struct AI_MSG_CHAOS : public AI_MSG
{
	AI_MSG_CHAOS() { dwMsgID = AI_MSG_ID_CHAOS; }
	DWORD			dwChaosTime;		// ȥ���� ���� �ð�
};

struct AI_MSG_COMMAND_FOLLOW : public AI_MSG
{
	AI_MSG_COMMAND_FOLLOW() { dwMsgID = AI_MSG_ID_COMMAND_FOLLOW; }
};

struct AI_MSG_USE_SKILL : public AI_MSG
{
	AI_MSG_USE_SKILL() { dwMsgID = AI_MSG_ID_USE_SKILL; }
	SLOTCODE		dwSkillCode;
	DWORD			dwTargetKey;
	WzVector		vTargetPos;
#ifdef _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY
    WORD            dwSelectedSkillDelay;
#endif // _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY
};

struct AI_MSG_COMMAND_NONPVP : public AI_MSG
{
	AI_MSG_COMMAND_NONPVP() { dwMsgID = AI_MSG_ID_COMMAND_NONPVP; }
};

struct AI_MSG_BLIND : public AI_MSG
{
	AI_MSG_BLIND() { dwMsgID = AI_MSG_ID_BLIND; }
	DWORD			dwBlindTime;		// ���սð�.
};

#pragma pack(pop)

#endif // _AITYPES_H_












