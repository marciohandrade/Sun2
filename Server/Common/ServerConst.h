


/** ServerConst.h : ���������� ����ϴ� DEFINE�� ����

*/

#ifndef __SERVER_CONST_H__
#define __SERVER_CONST_H__



// ���� ����
// �� ������ DBTable�� ������ ���������ʰ��� ����ȭ�� �̷�� ���� �Ѵ�.
// ������ ����!!!!!!!
// Related : Global.h�� GetServerType2String()
enum eSERVER_TYPE
{
	UNKNOWN_SERVER 			= 0,
	AGENT_SERVER			= 1,
	FIELD_SERVER			= 2,
	BATTLE_SERVER			= 3,
	GAME_DBPROXY			= 4,
	WORLD_SERVER			= 5,
	MASTER_SERVER			= 6,
	GUILD_SERVER            = 7,
	ACCOUNT_DBPROXY			= 8,
	TOURNAMENT_SERVER		= 9,
	AUTH_AGENT_SERVER		= 10,	// TODO: DB�� �ֱ�?
	PORTAL_SERVER			= 11,	//14,
	SOLARAUTH_SERVER		= 12,	//15,
	SHOP_SERVER				= 13,	//16,	

	OP_SERVER				= 25,	//11,   // WOPS(Webzen Online Portal Solution) Agent Server
	TEMP_SERVER				= 26,	//12,
	GAME_SERVER				= 27,	//< FIELD_SERVER�� BATTLE_SERVER�� ��Ī

	AUTH_SERVER				= 28,
	LOGIN_FRONT_SERVER		= 29,
	BILLING_SERVER			= 30,
	CLIENT_SERVER			= 31,
#ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION
    RANKING_SERVER          = GAME_DBPROXY,
#else
    RANKING_SERVER          = PORTAL_SERVER,
#endif
	MAX_SERVER				= 32,	
};

enum eSERVER_STATE
{
	SERVER_ABNORMAL_DISCONNECTED,
	SERVER_BOOTING,
	SERVER_NORMAL,
};

enum eDEBUGINFO_TYPE
{
	eDEBUGINFO_TYPE_INVALID,		// �� ���� ������� ����������� ������ ����
	eDEBUGINFO_TYPE_DAMAGE,			// ������ ���������
	eDEBUGINFO_TYPE_AGGRO,			// ��׷� ���������(��Ʋ���ڵ�)
	eDEBUGINFO_TYPE_MONSTERSKILL,	// ���� ��ų����
	eDEBUGINFO_TYPE_MAX,
};

enum eAUTH_LOGOUT_REASON
{
	AUTH_LOGOUT_NORMAL				= 0,			// ���� �α׾ƿ�
	AUTH_LOGOUT_AUTH_REQUEST		= 1,			// ���������� ��û���� ���� �α׾ƿ�
	AUTH_LOGOUT_USER_NOT_FOUND      = 2,			// �������� ���� ������ ã�� �� ���� ��� ����Agent�������� ���� �α׾ƿ�
	AUTH_LOGOUT_HACKING_USER        = 3,			// ���� ����� ��ŷ�� ��� ������ ���� ����Agent�������� ���� �α׾ƿ�
	AUTH_LOGOUT_WRONG_AUTHKEY       = 4,			// ����Ű�� ��ġ���� ���� ������ �������� ����Agent�������� ���� �α׾ƿ�
	AUTH_LOGOUT_WRONG_VERSION       = 5,			// ������ ��ġ���� �ʾ� ����Agent�������� ���� �α׾ƿ�
	AUTH_LOGOUT_ETC                 = 6,
};


enum eABUSE_TYPE
{
	eABUSE_FORCED_DISCONNECT		= 1,		// ��������(�������� 5ȸ�� �����)
	eABUSE_TEMPORARY_BLOCK			= 2,		// �ӽú��(Master�� �� �ִ� ���� ���)
	eABUSE_WORKPLACE_IP_BLOCK		= 3,		// �۾��� IP BLOCK
	eABUSE_DUPLICATE_ITEM_SERIAL	= 4,		// ������ �ߺ��ø��� �߻�
	eABUSE_CRASH_REPORT				= 5,		// Ŭ���̾�Ʈ ũ���� ����Ʈ
	eABUSE_LOGIC_VIOLATION			= 6,		// ��������(�������� ������ ������ġ�� �Ѿ�� ���������)
};

enum eABUSE_SUB_TYPE
{
	eABUSE_SUB_FORCED_DISCONNECT									= 0,		// ���¡���� ������������ ��������
	eABUSE_SUB_TEMPORARY_BLOCK										= 1,		// �������� ������������ �ӽú��(�����ͼ����� �� �ִ� ����)
	eABUSE_SUB_WORKPLACE_IP_BLOCK									= 2,		// �۾��� IP ��
	eABUSE_SUB_DUPLICATE_ITEM_SERIAL_EXSIT							= 3,		// ������ �ߺ� �ø��� ����
	eABUSE_SUB_DUPLICATE_ITEM_SERIAL_GENERATE						= 4,		// ������ �ߺ� �ø��� �߱�
	eABUSE_SUB_CRASH_REPORT											= 5,		// ũ���� ����Ʈ

	eABUSE_SUB_LOGIC_VIOLATION_ATTACK_SPEED							= 6,		// ���� ����
	eABUSE_SUB_LOGIC_VIOLATION_ATTACK_SEQUENCE						= 7,		// ���ݼ��� ����
	eABUSE_SUB_LOGIC_VIOLATION_ATTACK_RANGE							= 8,		// ���ݰŸ� ����
	eABUSE_SUB_LOGIC_VIOLATION_ATTACK_TYPE							= 9,		// ����Ÿ�� ����

	eABUSE_SUB_LOGIC_VIOLATION_INVALID_POSITION						= 10,		// ��ǥ �̻�
	eABUSE_SUB_LOGIC_VIOLATION_NOT_SYNC_POSITION					= 11,		// ����Ŭ�� ��ġ����ȭ ����
	eABUSE_SUB_LOGIC_VIOLATION_MOVE_RANGE							= 12,		// �̵��Ÿ� ����
	eABUSE_SUB_LOGIC_VIOLATION_MOVE_CRACKED_PACKET					= 13,		// �̵����� ũ���� ��Ŷ
	eABUSE_SUB_LOGIC_VIOLATION_MOVE_SPEED							= 14,		// �̼� ����

	eABUSE_SUB_LOGIC_VIOLATION_SECURITY_QUERY_DISCONNECT			= 15,		// �������� ����
	eABUSE_SUB_LOGIC_VIOLATION_HEARTBEAT_DISCONNECT					= 16,		// ��Ʈ��Ʈ ����
	eABUSE_SUB_LOGIC_VIOLATION_HACKSHIELD_DISCONNECT				= 17,		// �ٽ��� ����
	eABUSE_SUB_LOGIC_VIOLATION_UNEXPECTED_PACKET					= 18,		// �������� ���� ��Ŷ����
	eABUSE_SUB_LOGIC_VIOLATION_TEMPUSER_AUTH_DISCONNECT				= 19,		// TempUser ����
	eABUSE_SUB_LOGIC_VIOLATION_MISSION_REWARD						= 20,		// �̼Ǻ��� ����
	eABUSE_SUB_LOGIC_VIOLATION_LOADING_TIME_SHORT					= 21,		// �ε��ð� ����(�ε��ð��� �ʹ� ª�����)

	eABUSE_SUB_LOGIC_VIOLATION_WRONG_SECTOR_WORLDSVR				= 22,		// ���弭������ �߸��� ���� �ε������� ���� �α׸� ��°��
	eABUSE_SUB_LOGIC_VIOLATION_USER_NONE_WORLDSVR					= 23,		// ���Ӽ������� ������ �ִµ� ���弭������ ������ ���� ���
	eABUSE_SUB_LOGIC_VIOLATION_UNKNOWN_PROTOCOL						= 24,		// ���� ������ ��Ŷ ����(���̶�� ������ ������ ��Ŷ�� �����ϴ� ��츦 �� �� �ִ�.)
	eABUSE_SUB_LOGIC_VIOLATION_ASSERTION_FLOW						= 25,		// Ȯ�� �÷ο� ����
	eABUSE_SUB_LOGIC_VIOLATION_HACKSHIELD_INVALID_STATUS_DISCONNECT	= 26,		// �ٽ��� ���� V2 - (�� ���ɼ��� ������ �ٽ��� ��ġ ���ް� ���� �� �ִ� ���)
	eABUSE_SUB_MAX,
};

//MSG_DG_OTHER_EXIST_CHARNAME_SYN ����!
enum eCHARNAME_QUERY_ACTION_TYPE
{
	eCHARNAME_ACTION_SHOP_SEND_GIFT,	//< ������ �������� �����Ҷ�, ĳ�������翩�� üũ
    eCHARNAME_ACTION_DEFAULT,           // Added by __NA000000_SHOP_USERID_50_PASSING_PROC__
};

//__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
// ���� �� ���� ȹ�� ����
enum eHEIM_ACQUISITION_TYPE
{
    eHEIM_ACQUISITION_PICKUP,           // �ݱ�
    eHEIM_ACQUISITION_QUEST,           // ����
    eHEIM_ACQUISITION_MISSION,           // �̼�
};

// __KR_001355_20090608_EXP_ACQUISITION_NOTIFICATION
enum eEXP_ACQUISITION_TYPE
{
    eEXP_ACQUISITION_HUNTING,           //���
    eEXP_ACQUISITION_QUEST,            //����
    eEXP_ACQUISITION_MISSION,           //�̼�
};

//__NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION
// ���� �� ���� �Һ� ����
enum eHEIM_CONSUMPTION_TYPE
{
    eHEIM_CONSUMPTION_WAYPOINT,         // ��������Ʈ �̿�
    eHEIM_CONSUMPTION_RANKUP,           // ��ũ�� ���
    eHEIM_CONSUMPTION_COMPOSITION,      // ���� ���
    eHEIM_CONSUMPTION_REPAIR,           // ������ ���� ���
    eHEIM_CONSUMPTION_NPC_SHOP,         // NPC �������� ������ ���� (�籸�� ����)
    eHEIM_CONSUMPTION_ENCHANT,          // ��æ�� ���
};

// __KR_001355_20090805_CHECK_GROUPSYSTEM_MSG
//20090805 ���� �׷� �ý��ۿ��� Ŭ���̾�Ʈ�� �׷� ���� �������� �˸��°����� ����
enum eGAME_CONTENT_TYPE
{
    GAME_CONTENT_NORMAL                   = 1,    //�⺻ �ý���
    GAME_CONTENT_GROUPING                 = 2,    //�׷� �ý���
};

// NOTE: f101210.3L, moved from filtered eGM_STRING enumeration
enum eGM_STRING
{
    GMSTR_NONE = 0,
    //----------------------------------------------------------------------------------------------
    GMSTR_TURNON,
    GMSTR_TURNOFF,
    //----------------------------------------------------------------------------------------------
    GMSTR_MAX
};

#endif // __SERVER_CONST_H__
