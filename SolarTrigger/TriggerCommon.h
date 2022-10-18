#ifndef _SOLARTRIGGER_COMMON_H
#define _SOLARTRIGGER_COMMON_H
#pragma once

#include <UtilityCommon.h>
#include <MemoryPoolFactory.h>

//--------------------------------------------------------------------------------------------------
// ���� Ʈ���� Ÿ�� 
// 3D���������� Ÿ�� ���� ����� ���
// 1. enum eCONDITION_TYPE�� �����Ѵ�.
// 2. ConditionInfo�� ��ӹ޴� ���� Ŭ������ �����Ѵ�.

enum eCONDITION_TYPE
{
	eCONDITION_ALWAYS				= 0,	// �׻�                         
	eCONDITION_COMPARE_SWITCH		= 1,	// ����ġ ��                  
	eCONDITION_CLICK_OBJECT			= 2,	// ������Ʈ Ŭ��                
	eCONDITION_DAMAGE_OBJECT		= 3,	// ������Ʈ ������ ����         
	eCONDITION_COUNT_NPCKILL		= 4,	// ���� ��� ������           
	eCONDITION_PASS_TIME			= 5,	// �����ð��� �帧
	eCONDITION_ENTER_AREA			= 6,	// Ư�� ������ ��
	eCONDITION_HAVE_ITEM			= 7,	// �������� �����ϰ� �ִ�
	eCONDITION_MULTI_SWITCH			= 8,	// ���� ����ġ ��
	eCONDITION_MISSION_POINT		= 9,	// �̼� ����Ʈ ����
	eCONDITION_COMPARE_VARIABLE		= 10,	// ���� ��
	eCONDITION_QUEST_PROGRESS		= 11,	// Quest �Ϸ� üũ Ʈ����
	eCONDITION_CHECK_CLASS_ID		= 12,	// ���� Ŭ���� ����(���� Ʈ����)
	eCONDITION_NPCKILL_FOR_UNITID	= 13,	// ���� ������ ������
	eCONDITION_LIVE_PARTY_MEMBER	= 14,	// ��Ƽ�� ����/��� ���¶��
//_NA001385_20090924_DOMINATION_TRIGGER
    eCONDITION_CHECK_OBJECT_HP	    = 15,	// Mapobject�� HP��ġ�� %�����ϰ��
    eCONDITION_DESTROY_OBJECT   	= 16,	// mapobject�� �ı� �Ǿ��ٸ�.
//_NA_0_20100222_UNIT_TRIGGERS_ADD
                                            // 17�� ���� ���� ����
    eCONDITION_CHECK_OBJECTKIND_HP  = 18,   // [UnitID,Mapobject]�� HP��ġ�� %�����ϰ��
    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
    eCONDITION_NPC_ENTER_AREA       = 19,   // NPC�� Ư�� ������ ���� ��

    //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
    eCONDITION_COMPLETE_COLLECTION       = 20,   // �̼ǳ� ���� Ʈ����

    // _NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER
    eCONDITION_SERVER_EVENT         = 21,   // �����̺�Ʈ Ʈ����

    // _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM
    eCONDITION_CHECK_TEAM           = 22,   // �� üũ(���� Ʈ����)
	eCONDITION_COMPARE_SWITCH_EX	= 23,	// ����ġ(Ȯ����) ��
    eCONDITION_MONSTER_ENTER_AREA   = 24,   // ���Ͱ� Ư�� ������ ���� ��

    eCONDITION_MAX,	// ���� ��������� ����	(define ���ο� ���� �ʴ´�. �۾��� ���� ���)
};

//--------------------------------------------------------------------------------------------------

enum eACTION_TYPE
{
	eACTION_CHANGE_OBJECTANI		= 1001,	// ������Ʈ �ִϸ��̼� ����                             (����-����X)(Ŭ��-����O)
	eACTION_CHANGE_PATHTILE			= 1002,	// ��ã�� Ÿ�� �Ӽ� ����                                (����-����O)(Ŭ��-����O)
	eACTION_CHANGE_OBJECTSTATE		= 1003,	// ������Ʈ ���� ����                                   (����-����X)(Ŭ��-����O)
	eACTION_PLAY_EVENT				= 1004,	// �̺�Ʈ �÷���                                        (����-����O)(Ŭ��-����O)
	eACTION_SET_ISATTACK			= 1005,	// ���� ����/�Ұ� ����                                  (����-����O)(Ŭ��-����X)
	eACTION_PORTAL_PLAYER			= 1006,	// �÷��̾� ��Ż                                        (����-����O)(Ŭ��-����X)
	eACTION_SET_OBJECTTHRUST		= 1007,	// ������Ʈ �б� ����                                   (����-����X)(Ŭ��-����X) X
	eACTION_APPLY_DAMAGE			= 1008,	// ������ ����                                          (����-����X)(Ŭ��-����X) X
	eACTION_REFLECT_DAMAGE			= 1009,	// ������ �ݻ�                                          (����-����X)(Ŭ��-����X) X
	eACTION_CREATE_MONSTER			= 1010,	// ���� ����                                          (����-����O)(Ŭ��-����O)
	eACTION_DISPLAY_MSGBOX			= 1011,	// �޼��� �ڽ� ���                                     (����-����O)(Ŭ��-����X)
	eACTION_REMOVE_ITEM				= 1012,	// �κ��丮 ������ ����                                 (����-����O)(Ŭ��-����X)
	eACTION_REWARD_PLAYER			= 1014,	// �÷��̾� ����                                        (����-����O)(Ŭ��-����X)
	eACTION_CLEAR_MISSION			= 1015,	// �̼� ���                                            (����-����O)(Ŭ��-����O)
	eACTION_PORTAL_RANDOM			= 1016,	// ���� ��Ż                                            (����-����O)(Ŭ��-����X)
	eACTION_ACTIVATE_TRIGGER		= 1017,	// Ʈ���� Ȱ�� ��Ȱ��                                   (����-����O)(Ŭ��-����O)
	eACTION_AREA_DAMAGE				= 1018,	// ����������                                           (����-����O)(Ŭ��-����X)
	eACTION_OPERATE_SWITCH			= 1019,	// ����ġ ����                                          (����-����O)(Ŭ��-����X)
	eACTION_OPERATE_VARIABLE		= 1020,	// ����ġ ���� ����                                     (����-����O)(Ŭ��-����X)
	eACTION_ROAD_CONNECT			= 1021,	// �� ������ ����/�����Ѵ�                              (����-����O)(Ŭ��-����O)
	eACTION_CREATE_DIR_MONSTER		= 1022, // ���͸� �����Ѵ�(����)                              (����-����O)(Ŭ��-����O)
	eACTION_CHANGE_LIGHT			= 1023,	// ����Ʈ ������ �����Ѵ�                               (����-����X)(Ŭ��-����O)
	eACTION_CHANGE_OBJECT_TYPE		= 1024, // ������Ʈ ���¸� �ٲ۴�                               (����-����X)(Ŭ��-����O)
	eACTION_RANDOM_MAP_MOVE			= 1025, // ���� �� �̵�                                         (����-����O)(Ŭ��-����X)
	eACTION_DROP_TRESUREBOX			= 1026, // ���� ���� ����߸���                                 (����-����X)(Ŭ��-����X) X

	//eACTION_CREATE_MONSTER,
	eACTION_QUEST_SATISFACTION		= 1027,	// ����Ʈ ���� ����                                     (����-����X)(Ŭ��-����X) X
	eACTION_QUEST_GRANT				= 1028,	// ����Ʈ �ο�                                          (����-����O)(Ŭ��-����X)
	eACTION_QUEST_SETUP				= 1029,	// ����Ʈ �Ϸ�/���� ����                                (����-����O)(Ŭ��-����X)
	//
	eACTION_USE_CATEGORY			= 1031,	// (�ʵ�)�� (ī�װ�)�� ����Ѵ�                       (����-����X)(Ŭ��-����X) X
	eACTION_HOLD_CHARACTER			= 1032,	// (Character)�� �̵��� (�ð�)���� ���´�               (����-����X)(Ŭ��-����X) X
	eACTION_CREATE_MONSTER_GROUP	= 1033,	// ���ͱ׷� ����                                      (����-����X)(Ŭ��-����X) X
	eACTION_SHOW_GUIDE_MSGBOX		= 1034,	// �ȳ��޽����ڽ� ���                                  (����-����X)(Ŭ��-����O)
	eACTION_SHOW_OR_HIDE_UNIT		= 1035,	// ���� ��Ÿ��/�����                                   (����-����X)(Ŭ��-����X) X
	eACTION_SHOW_OR_HIDE_MSGBOX		= 1036,	// �޽��� �ڽ��� ��Ÿ��/�����                          (����-����X)(Ŭ��-����O)
	eACTION_RANDOM_AREA_MOVE		= 1038,	// ������ ���� �̵� ��Ų��                              (����-����O)(Ŭ��-����X)       
	eACTION_RANDOM_MONSTER_SPAWN    = 1039,	// ���� ���� ����                                     (����-����O)(Ŭ��-����O)
	eACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR = 1040, //                                                (����-����O)(Ŭ��-����X)
	eACTION_REMOVE_MONSTER			= 1041, //                                                      (����-����O)(Ŭ��-����X)        
	eACTION_ADD_EXP					= 1042, //                                                      (����-����O)(Ŭ��-����X)
	eACTION_MONSTER_STAT_UP_OR_DOWN	= 1043,	// ������ �ɷ�ġ�� �ϳ��� ����/���� ��Ų��.           (����-����O)(Ŭ��-����X)
	eACTION_RANDUM_TRIGER_ON_OR_OFF	= 1044,	// �ش� Ʈ������ �ϳ��� on/off ��Ų��.                  (����-����O)(Ŭ��-����X)
    //__NA001140_20080128_SOUND_TRIGGER_ADD
	eACTION_PLAY_MUSIC				= 1045,	// ���� ������ ����Ѵ�.                              (����-����O)(Ŭ��-����X)    
//_NA001385_20090924_DOMINATION_TRIGGER
    eACTION_DOWN_OBJETC_HP			= 1046,	// �� ������Ʈ�� HP�� ��´�.                           (����-����O)(Ŭ��-����X)    
    eACTION_CHANGE_ATTACK_KIND      = 1047, // �ڽ��� ���ݰ���[�Ұ���] �ϰ� �����.                 (����-����O)(Ŭ��-����O)
    eACTION_ACTIVE_STAMP            = 1048, // ���� ���� ���·� �����(���׸� �ı�) ���ι���        (����-����O)(Ŭ��-����X)
                                            // Ȱ��ȭ ���� ����ȭ�� ���� �̺�Ʈ�� ���
                                            // ���� eCONDITION_DESTROY_OBJECT ���� �Ͻ� ����
    //_NA_0_20100222_UNIT_TRIGGERS_ADD
    eACTION_ATTACK_OBJECTKIND       = 1049,     // [����/����/������Ʈ]�� ���� �� �� [�ְ�/����] ����� (Ư�� ���� ��ü�� �÷��̾�� ���� ������ �ʰ� �����.)
    eACTION_ACTIVE_SKILL_OBJECTKIND = 1050,     // [����/����/������Ʈ]�� [��ųID]��ų�� �ߵ���Ų��.
    eACTION_SHOW_MESSAGEBOX_OBJECTKIND  = 1051, // [����/����/������Ʈ]�� ��ġ�� [�޽����ڵ�]�޽����� �����ش�.
    eACTION_SET_ATTACK_STATE_OBJECTKIND = 1052, // [����/����]�� [����]�� [����/����]�Ѵ�.
    eACTION_DESTROY_OBJECTKIND = 1053,          // [����/����/������Ʈ]�� �����ϸ� ������ [����/������]�Ѵ�.
    eACTION_CREATE_MONSTER_LINKED_MAPOBJECT = 1054, // [������Ʈ]�� �����Ѵ�. (MapObject�� ����� Npc ����)
// _NA_0_20100525_ADD_TIMER_ACTION_TRIGGER
    eACTION_CONTROL_TIMER           = 1055,     // �ð踦 �����ش�.
//_NA_0_20100616_DOMINATION_EVENT
    eACTION_DO_COMMAND              = 1056,     // Ÿ�Կ� �ش� �ϴ� ������ ����(Ʈ���Ÿ� ���� �޽��� ����)
//_NA_0_20100817_HELLON_LAST_TRIGGER
    eACTION_MOVE_CONTROL            = 1057,
    eACTION_ANIMATION_CONTROL       = 1058,
    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
    eACTION_NPC_APPLY_DAMAGE        = 1059, // ����/����/������Ʈ���� �������� �����Ѵ�.
    //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
    eACTION_COMPLETE_COLLECTION     = 1060, //�̼ǳ� ä�� Ʈ����
    //_NA_000000_20130104_ADD_ACTION_TRIGGER_APPLY_HPMPSD
    eACTION_APPLY_HPMPSD            = 1061, // [����/��ü]���� [HP/MP/SD]�� [��ġ]��ŭ [����/����]�� ����
    //_NA_006658_20130422_ADD_ACTION_TRIGGER_CHANGE_OBJECT_HP
    eACTION_CHANGE_OBJECT_HP        = 1062, // [ObjectKey]������Ʈ�� HP�� [����/��ġ] [����]��ŭ [������/�ø���]
    //_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
    eACTION_CHANGE_RADAR_MAP        = 1063, // ���̴���(�̴ϸ�)�� �����Ѵ�.
    eACTION_ATTACH_STATE_RANGE      = 1064, // ����� �������� ���¸� ���ӽð����� �ο��Ѵ�.
    eACTION_CHANGE_PLAYER_HP        = 1065, // [��Ƽ/����]�� HP�� [����/��ġ] [����]��ŭ [������/�ø���]
	eACTION_OPERATE_SWITCH_EX		= 1066,	// ����ġ ����(Ȯ����)
    eACTION_NPC_RANDOM_AREA_MOVE    = 1067,	// NPC ���� ���� �̵�
	
    eACTION_MAX,							// ���� ��������� ����	(define ���ο� ���� �ʴ´�. �۾��� ���� ���)
};

//--------------------------------------------------------------------------------------------------
// �� ����
enum eCOMPARE_TYPE
{
	eCOMPARE_EQUAL = 0,				// "=="
	eCOMPARE_MORE,					// ">"
	eCOMPARE_LESS,					// "<"
	eCOMPARE_LESSTHAN,				// "<="
	eCOMPARE_MORETHAN,				// ">="
	eCOMPARE_NOTEQUAL,				// "!="
};	

// ���� ����
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
    eTIMER_CREATE_DECREASE = 0,    //  Ÿ�̸� ������ �ð��� ����.
    eTIMER_CREATE_INCREASE = 1,    //  Ÿ�̸� ������ �ð��� ����.
    eTIMER_DESTROY         = 2,    //  ���� �ð� ui ������ �ı�.
};


// ������� �ʴ� ��
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
