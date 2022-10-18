#ifndef _PROGRAMCOMMON_PACKETSTRUCT_CHAOSZONESYSTEM_H_
#define _PROGRAMCOMMON_PACKETSTRUCT_CHAOSZONESYSTEM_H_

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "PacketStruct_CG.h"
#include "const.h"

//--------------------------------------------------------------------------------------------------
enum eCG_CHAOS_ZONE_ZONE_CG
{
    CG_CHAOS_ZONE_ZONE_JOIN_ACK,
    CG_CHAOS_ZONE_ZONE_LEAVE_ACK
};

struct MSG_CG_CHAOS_ZONE_ZONE : MSG_CG_CHAOS_ZONE_PACKET
{
    BYTE packet_type;
    MSG_CG_CHAOS_ZONE_ZONE()
    {
        category_type = kCategoryZone;
    }
};

struct MSG_CG_CHAOS_ZONE_ZONE_JOIN_ACK : MSG_CG_CHAOS_ZONE_ZONE
{
    MAPCODE map_code;
    ChaosZoneType chaos_zone_type;
    
    MSG_CG_CHAOS_ZONE_ZONE_JOIN_ACK()
    {
        packet_type = CG_CHAOS_ZONE_ZONE_JOIN_ACK;
    }
};


//--------------------------------------------------------------------------------------------------
enum eBATTLEGROUND_PACKET_TYPE_CG
{
    CG_BATTLE_GROUND_RESERVATION_SYN = 0, //���� �������� ��û
    CG_BATTLE_GROUND_RESERVATION_ACK, //���� �������� ����
    CG_BATTLE_GROUND_RESERVATION_BRD, //���� �������� BRD
    CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN, //���� ������� ��û
    CG_BATTLE_GROUND_RESERVATION_CANCEL_ACK, //���� ������� ����
    CG_BATTLE_GROUND_MATCHING_RESULT_BRD, //���� 1����Ī BRD
    
    CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_REMOVE_SYN, //������ �������� �� ���� ����
    CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_RETURN_SYN, //������ �������� �� ����

    CG_BATTLE_GROUND_TEAM_INFO_CMD, // ������ �뺸(����)
    CG_BATTLE_GROUND_TEAM_INFO_BRD, // ������ �뺸(�츮�� + ����� ���)

    CG_BATTLE_GROUND_RESURECTION_REFUSE_SYN, // ��Ȱ��� �źλ���
    CG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN, // ��Ȱ��� �ź� ���
    CG_BATTLE_GROUND_STATE_RESURECTION_CMD, // ��Ȱ���� ���(����+�ð�)

    CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD, // ���� ���� BRD
    CG_BATTLE_GROUND_STRONGPOINT_VICTORY_POINT_BRD, // ���� ���� BRD

    CG_BATTLE_GROUND_FINISH_RESULT_BRD, // ���� ���� ����
    CG_BATTLE_GROUND_TIME_INFO_CMD, // ���� �ð� ����

    CG_BATTLE_GROUND_GET_OPEN_TIME_SYN, // ���� ���½ð� ���
    CG_BATTLE_GROUND_GET_OPEN_TIME_ACK, // ���� ���½ð� �˷��ֱ�

    CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD,

    CG_BATTLE_GROUND_INITIATE_VOTE_SYN, // ���� �߹� ��ǥ ����
    CG_BATTLE_GROUND_INITIATE_VOTE_NAK, // ���� �߹� ��ǥ ���� ����
    CG_BATTLE_GROUND_INITIATE_VOTE_BRD, // ���� �߹� ��ǥ ���� BRD
    CG_BATTLE_GROUND_VOTE_STATUS_BRD,   // ���� ��ǥ ��Ȳ BRD
    CG_BATTLE_GROUND_VOTE_SYN,          // ���� ��ǥ
    CG_BATTLE_GROUND_VOTE_RESULT_BRD,   // ���� ��ǥ ���
    CG_BATTLE_GROUND_VANP_PLAYER_CMD,   // ���� ���� �߹� ���

};

struct MSG_CG_BATTLE_GROUND_PACKET : MSG_CG_CHAOS_ZONE_PACKET
{
    eBATTLEGROUND_PACKET_TYPE_CG packet_type;
    MSG_CG_BATTLE_GROUND_PACKET()
    {
        category_type = kCategoryBattleGround;
    }
};

//--------------------------------------------------------------------------------------------------
// CG Client <-> AgentServer
struct MSG_CG_BATTLE_GROUND_RESERVATION_SYN : MSG_CG_BATTLE_GROUND_PACKET
{
    MAPCODE map_code;   //>>���ڵ�
    bool is_healer;     //>>��������
    
    MSG_CG_BATTLE_GROUND_RESERVATION_SYN()
    {
        packet_type = CG_BATTLE_GROUND_RESERVATION_SYN;
        map_code = 0;
        is_healer = false;
    }
};
struct MSG_CG_BATTLE_GROUND_RESERVATION_ACK : MSG_CG_BATTLE_GROUND_PACKET
{
    RC::eBATTLE_GROUND_RESULT result_code;
    WORD battle_ground_reservation_key;

    MSG_CG_BATTLE_GROUND_RESERVATION_ACK()
    {
        packet_type = CG_BATTLE_GROUND_RESERVATION_ACK;
    }
};

struct MSG_CG_BATTLE_GROUND_RESERVATION_BRD : MSG_CG_BATTLE_GROUND_PACKET
{
    WORD current_reservation_memeber_count;
    MSG_CG_BATTLE_GROUND_RESERVATION_BRD()
    {
        current_reservation_memeber_count = 0;
        packet_type = CG_BATTLE_GROUND_RESERVATION_BRD;
    }
};


struct MSG_CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN : MSG_CG_BATTLE_GROUND_PACKET
{
    MSG_CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN()
    {
        packet_type = CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN;
    }
};

struct MSG_CG_BATTLE_GROUND_RESERVATION_CANCEL_ACK : MSG_CG_BATTLE_GROUND_PACKET
{
    RC::eBATTLE_GROUND_RESULT result_code;

    MSG_CG_BATTLE_GROUND_RESERVATION_CANCEL_ACK()
    {
        packet_type = CG_BATTLE_GROUND_RESERVATION_CANCEL_ACK;
    }
};

struct MSG_CG_BATTLE_GROUND_MATCHING_RESULT_BRD :  MSG_CG_BATTLE_GROUND_PACKET
{
    BYTE result_type;
    MAPCODE map_code;
    MSG_CG_BATTLE_GROUND_MATCHING_RESULT_BRD()
    {
        packet_type = CG_BATTLE_GROUND_MATCHING_RESULT_BRD;
        result_type = 0;
        map_code = 0;
    }
};

struct MSG_CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_REMOVE_SYN : MSG_CG_BATTLE_GROUND_PACKET
{
    MSG_CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_REMOVE_SYN()
    {
        packet_type = CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_REMOVE_SYN;
    }
};

struct MSG_CG_BATTLE_GROUND_TEAM_INFO_CMD : MSG_CG_BATTLE_GROUND_PACKET
{
    WORD team_key;
    MSG_CG_BATTLE_GROUND_TEAM_INFO_CMD()
    {
        packet_type = CG_BATTLE_GROUND_TEAM_INFO_CMD;
    }
};

struct MSG_CG_BATTLE_GROUND_TEAM_INFO_BRD : MSG_CG_BATTLE_GROUND_PACKET
{
    BYTE member_count;
    BATTLE_GROUND_TEAM_MEMBER_INFO member_list[20];

    MSG_CG_BATTLE_GROUND_TEAM_INFO_BRD()
    {
        packet_type = CG_BATTLE_GROUND_TEAM_INFO_BRD;
        member_count = 0;
        memset(member_list, 0, sizeof(member_list));
    }
};

struct MSG_CG_BATTLE_GROUND_RESURECTION_REFUSE_SYN : MSG_CG_BATTLE_GROUND_PACKET
{
    MSG_CG_BATTLE_GROUND_RESURECTION_REFUSE_SYN()
    {
        packet_type = CG_BATTLE_GROUND_RESURECTION_REFUSE_SYN;
    }
};

struct MSG_CG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN : MSG_CG_BATTLE_GROUND_PACKET
{
    MSG_CG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN()
    {
        packet_type = CG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN;
    }
};

struct MSG_CG_BATTLE_GROUND_STATE_RESURECTION_CMD : MSG_CG_BATTLE_GROUND_PACKET
{
    DWORD player_object_key;
    BattleGroundPlayerState player_state;
    DWORD resurection_time;
    MSG_CG_BATTLE_GROUND_STATE_RESURECTION_CMD()
    {
        player_object_key = 0;
        packet_type = CG_BATTLE_GROUND_STATE_RESURECTION_CMD;
        player_state = kPlayerStateNone;
        resurection_time = 0;
    }
};
struct MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD : MSG_CG_BATTLE_GROUND_PACKET
{
    BATTLE_GROUND_STRONGPOINT_INFO strongpoint_info;
    MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD()
    {
        packet_type = CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD;
        memset(&strongpoint_info, 0, sizeof(strongpoint_info));
    }
};

struct MSG_CG_BATTLE_GROUND_STRONGPOINT_VICTORY_POINT_BRD : MSG_CG_BATTLE_GROUND_PACKET
{
    BATTLE_GROUND_TEAM_SCORE_INFO team_score_info[2];

    MSG_CG_BATTLE_GROUND_STRONGPOINT_VICTORY_POINT_BRD()
    {
        packet_type = CG_BATTLE_GROUND_STRONGPOINT_VICTORY_POINT_BRD;
    }
};

struct MSG_CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_RETURN_SYN : MSG_CG_BATTLE_GROUND_PACKET
{
    DWORD strongpoint_count;
    BATTLE_GROUND_STRONGPOINT_INFO strongpoint_info[3]; // ����� 3�������� ���� �߰� ����..

    MSG_CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_RETURN_SYN()
    {
        packet_type = CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_RETURN_SYN;
        strongpoint_count = 0;
        memset(&strongpoint_info, 0, sizeof(strongpoint_info));
    }
};

struct MSG_CG_BATTLE_GROUND_FINISH_RESULT_BRD : MSG_CG_BATTLE_GROUND_PACKET
{
    ChaosZoneResultType result_type;
    MSG_CG_BATTLE_GROUND_FINISH_RESULT_BRD()
    {
        result_type = kResultNone;
        packet_type = CG_BATTLE_GROUND_FINISH_RESULT_BRD;
    }
};

struct MSG_CG_BATTLE_GROUND_TIME_INFO_CMD : MSG_CG_BATTLE_GROUND_PACKET
{
    DWORD elapsed_time;
    MAPCODE map_code;
    MSG_CG_BATTLE_GROUND_TIME_INFO_CMD()
    {
        packet_type = CG_BATTLE_GROUND_TIME_INFO_CMD;
        elapsed_time = 0;
        map_code = 0;
    }
};

// _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
struct MSG_CG_BATTLE_GROUND_GET_OPEN_TIME_SYN : MSG_CG_BATTLE_GROUND_PACKET
{
    MSG_CG_BATTLE_GROUND_GET_OPEN_TIME_SYN()
    {
        packet_type = CG_BATTLE_GROUND_GET_OPEN_TIME_SYN;
    }
};

struct MSG_CG_BATTLE_GROUND_GET_OPEN_TIME_ACK : MSG_CG_BATTLE_GROUND_PACKET
{
    JoinBattleGroundState join_state;
    DWORD join_time;
    MSG_CG_BATTLE_GROUND_GET_OPEN_TIME_ACK()
    {
        packet_type = CG_BATTLE_GROUND_GET_OPEN_TIME_ACK;
        join_state = kNone;
        join_time = INFINITE;
    }
};

// _NA_006577_BATTLE_GROUND_BATTLE_STATE_NOTIFY
struct MSG_CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD : MSG_CG_BATTLE_GROUND_PACKET
{
    TCHAR kill_payer_name[MAX_CLIENT_CHARNAME_LENGTH];
    TCHAR death_payer_name[MAX_CLIENT_CHARNAME_LENGTH];
    bool is_series_kill; // ����ų ����
    WORD series_kill_count; // ���� ų ��
    bool is_series_kill_block; // ����ų ���� ����
#ifdef _NA_006599_20130401_BATTLEGROUND_PKPOINT
    BYTE pk_point;
    BATTLE_GROUND_TEAM_SCORE_INFO team_score_info[2];
#endif

    MSG_CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD()
    {
        packet_type = CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD;
        memset(&kill_payer_name, 0, _countof(kill_payer_name));
        memset(&death_payer_name, 0, _countof(death_payer_name));
        is_series_kill = false;
        series_kill_count = 0;
        is_series_kill_block = false;
#ifdef _NA_006599_20130401_BATTLEGROUND_PKPOINT
        pk_point = 0;
#endif
    }
};

// _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
struct MSG_CG_BATTLE_GROUND_INITIATE_VOTE_SYN : MSG_CG_BATTLE_GROUND_PACKET
{
    TCHAR van_payer_name[MAX_CLIENT_CHARNAME_LENGTH];
    MSG_CG_BATTLE_GROUND_INITIATE_VOTE_SYN()
    {
        packet_type = CG_BATTLE_GROUND_INITIATE_VOTE_SYN;
        memset(&van_payer_name, 0, _countof(van_payer_name));
    }
};

struct MSG_CG_BATTLE_GROUND_INITIATE_VOTE_NAK : MSG_CG_BATTLE_GROUND_PACKET
{
    RC::eBATTLE_GROUND_RESULT result;
    MSG_CG_BATTLE_GROUND_INITIATE_VOTE_NAK()
    {
        packet_type = CG_BATTLE_GROUND_INITIATE_VOTE_NAK;
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
        result = RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL;
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    }
};

struct MSG_CG_BATTLE_GROUND_INITIATE_VOTE_BRD : MSG_CG_BATTLE_GROUND_PACKET
{
    TCHAR van_payer_name[MAX_CLIENT_CHARNAME_LENGTH];
    MSG_CG_BATTLE_GROUND_INITIATE_VOTE_BRD()
    {
        packet_type = CG_BATTLE_GROUND_INITIATE_VOTE_BRD;
        memset(&van_payer_name, 0, _countof(van_payer_name));
    }
};
struct MSG_CG_BATTLE_GROUND_VOTE_STATUS_BRD : MSG_CG_BATTLE_GROUND_PACKET
{
    TCHAR van_payer_name[MAX_CLIENT_CHARNAME_LENGTH];
    BYTE yes_count;
    BYTE no_count;
    DWORD remain_vote_time;
    MSG_CG_BATTLE_GROUND_VOTE_STATUS_BRD()
    {
        packet_type = CG_BATTLE_GROUND_VOTE_STATUS_BRD;
        memset(&van_payer_name, 0, _countof(van_payer_name));
        yes_count = 0;
        no_count = 0;
        remain_vote_time = 0;
    }
};

struct MSG_CG_BATTLE_GROUND_VOTE_SYN : MSG_CG_BATTLE_GROUND_PACKET
{
    bool vote_result;
    MSG_CG_BATTLE_GROUND_VOTE_SYN()
    {
        packet_type = CG_BATTLE_GROUND_VOTE_SYN;
        vote_result = true;
    }
};

struct MSG_CG_BATTLE_GROUND_VOTE_RESULT_BRD : MSG_CG_BATTLE_GROUND_PACKET
{
    bool vote_result;
    TCHAR van_payer_name[MAX_CLIENT_CHARNAME_LENGTH];
    MSG_CG_BATTLE_GROUND_VOTE_RESULT_BRD()
    {
        packet_type = CG_BATTLE_GROUND_VOTE_RESULT_BRD;
        vote_result = false;
        memset(&van_payer_name, 0, _countof(van_payer_name));
    }
};
struct MSG_CG_BATTLE_GROUND_VANP_PLAYER_CMD : MSG_CG_BATTLE_GROUND_PACKET
{
    MSG_CG_BATTLE_GROUND_VANP_PLAYER_CMD()
    {
        packet_type = CG_BATTLE_GROUND_VANP_PLAYER_CMD;
    }
};

//--------------------------------------------------------------------------------------------------
enum eFREEPVP_PACKET_TYPE_CG
{
    CG_FREE_PVP_ROOM_LIST_SYN = 0, // �� ����Ʈ
    CG_FREE_PVP_ROOM_LIST_ACK,
    
    CG_FREE_PVP_CREATE_ROOM_SYN, // �����
    CG_FREE_PVP_CREATE_ROOM_ACK,
    
    CG_FREE_PVP_JOIN_ROOM_SYN, // �� ���� 
    CG_FREE_PVP_JOIN_ROOM_COUNT_CMD,
    CG_FREE_PVP_JOIN_ROOM_ACK,

    CG_FREE_PVP_RESURRECTION_SYN, // ��Ȱ
};

struct MSG_CG_FREE_PVP_PACKET : MSG_CG_CHAOS_ZONE_PACKET
{
    eFREEPVP_PACKET_TYPE_CG packet_type;
    MSG_CG_FREE_PVP_PACKET()
    {
        category_type = kCategoryFreePVP;
    }
};

struct MSG_CG_FREE_PVP_ROOM_LIST_SYN : MSG_CG_FREE_PVP_PACKET
{
    MSG_CG_FREE_PVP_ROOM_LIST_SYN()
    {
        packet_type = CG_FREE_PVP_ROOM_LIST_SYN;
    }
};

struct MSG_CG_FREE_PVP_ROOM_LIST_ACK : MSG_CG_FREE_PVP_PACKET
{
    BYTE count_of_list;//���� ���޵Ǵ� ����Ʈ ��
    FreePVPRoomInfo free_pvp_room_info[100];
    
    MSG_CG_FREE_PVP_ROOM_LIST_ACK()
    {
        packet_type = CG_FREE_PVP_ROOM_LIST_ACK;
        count_of_list = 0;
        ZeroMemory(free_pvp_room_info,sizeof(free_pvp_room_info));
    }
    uint16_t GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(free_pvp_room_info);
        return static_cast<uint16_t>(MinSize + sizeof(free_pvp_room_info[0]) * count_of_list);
    }
};

struct MSG_CG_FREE_PVP_CREATE_ROOM_SYN : MSG_CG_FREE_PVP_PACKET
{
    BaseFreePVPRoomInfo base_free_pvp_info;
    MSG_CG_FREE_PVP_CREATE_ROOM_SYN()
    {
        packet_type = CG_FREE_PVP_CREATE_ROOM_SYN;
        ZeroMemory(&base_free_pvp_info, sizeof(base_free_pvp_info));
    }
};

struct MSG_CG_FREE_PVP_CREATE_ROOM_ACK : MSG_CG_FREE_PVP_PACKET
{
    RC::eFREE_PVP_RESULT result_code;
    MSG_CG_FREE_PVP_CREATE_ROOM_ACK()
    {
        packet_type = CG_FREE_PVP_CREATE_ROOM_ACK;
    }
};

struct MSG_CG_FREE_PVP_JOIN_ROOM_SYN : MSG_CG_FREE_PVP_PACKET
{
    KEYTYPE join_room_key; //Join�Ϸ��� �� Ű��(index)
    bool is_private_room; //��й� ����
    TCHAR password[MAX_ROOMPASSWORD_LENGTH+1]; //��Ƽ�� ��� ��ȣ
    MSG_CG_FREE_PVP_JOIN_ROOM_SYN()
    {
        packet_type = CG_FREE_PVP_JOIN_ROOM_SYN;
        join_room_key = 0;
        is_private_room = false;
    }
    uint16_t GetSize() const
    {
        const size_t MinSize = sizeof(*this) - sizeof(password);
        if (is_private_room == true)
        {
            return static_cast<uint16_t>(MinSize + sizeof(password[0]) * _countof(password));
        }
        else
        {
            return static_cast<uint16_t>(MinSize);
        }
    }
};

struct MSG_CG_FREE_PVP_JOIN_ROOM_COUNT_CMD : MSG_CG_FREE_PVP_PACKET
{
    MSG_CG_FREE_PVP_JOIN_ROOM_COUNT_CMD()
    {
        packet_type = CG_FREE_PVP_JOIN_ROOM_COUNT_CMD;
    }
};

struct MSG_CG_FREE_PVP_JOIN_ROOM_ACK : MSG_CG_FREE_PVP_PACKET
{
    RC::eFREE_PVP_RESULT result_code;
    FreePVPRoomInfo free_pvp_room_info;
    MSG_CG_FREE_PVP_JOIN_ROOM_ACK()
    {
        packet_type = CG_FREE_PVP_JOIN_ROOM_ACK;
        ZeroMemory(&free_pvp_room_info,sizeof(free_pvp_room_info));
    }
};

struct MSG_CG_FREE_PVP_RESURRECTION_SYN : MSG_CG_FREE_PVP_PACKET
{
    MSG_CG_FREE_PVP_RESURRECTION_SYN()
    {
        packet_type = CG_FREE_PVP_RESURRECTION_SYN;
    }
};

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
//--------------------------------------------------------------------------------------------------
enum eGOLDRUSH_PACKET_TYPE_CG
{
    CG_GOLDRUSH_RESERVATION_SYN = 0,       //���� �������� ��û
    CG_GOLDRUSH_RESERVATION_ACK,           //���� �������� ����
    CG_GOLDRUSH_RESERVATION_BRD,           //���� �������� BRD
    CG_GOLDRUSH_RESERVATION_CANCEL_SYN,    //���� ������� ��û
    CG_GOLDRUSH_RESERVATION_CANCEL_ACK,    //���� ������� ����

    CG_GOLDRUSH_TEAM_INFO_CMD, // �������뺸(����)
    CG_GOLDRUSH_TEAM_INFO_BRD, // �������뺸(�츮��+ ��������)

    CG_GOLDRUSH_ABNORMAL_EXIT_USER_REMOVE_SYN, //������ �������� �� ���� ����
    CG_GOLDRUSH_ABNORMAL_EXIT_USER_RETURN_SYN, //������ �������� �� ����

    CG_GOLDRUSH_STATE_RESURECTION_CMD, // ��Ȱ���¸��(����+�ð�)

    CG_GOLDRUSH_FINISH_RESULT_BRD, // ������п���

    CG_GOLDRUSH_TIME_INFO_CMD, // ����ð�����
    //CG_GOLDRUSH_GET_OPEN_TIME_SYN, // ������½ð����
   // CG_GOLDRUSH_GET_OPEN_TIME_ACK, // ������½ð��˷��ֱ�

   CG_GOLDRUSH_BATTLE_STATE_NOTIFY_BRD,
   CG_GOLDRUSH_MONSTERKILL_NOTIFY_BRD,
   CG_GOLDRUSH_TEAM_SCORE_INFO_BRD, // �� �������Ʈ �����, �˸� (1������ ������ ������)
   CG_GOLDRUSH_TOTAL_TEAM_POINT_BRD,

    CG_GOLDRUSH_INITIATE_VOTE_SYN, // ���� �߹� ��ǥ ����
    CG_GOLDRUSH_INITIATE_VOTE_NAK, // ���� �߹� ��ǥ ���� ����
    CG_GOLDRUSH_INITIATE_VOTE_BRD, // ���� �߹� ��ǥ ���� BRD
    CG_GOLDRUSH_VOTE_STATUS_BRD,   // ���� ��ǥ ��Ȳ BRD
    CG_GOLDRUSH_VOTE_SYN,          // ���� ��ǥ
    CG_GOLDRUSH_VOTE_RESULT_BRD,   // ���� ��ǥ ���
    CG_GOLDRUSH_VANP_PLAYER_CMD,   // ���� ���� �߹� ���
};
struct MSG_CG_GOLDRUSH_PACKET : MSG_CG_CHAOS_ZONE_PACKET
{
    eGOLDRUSH_PACKET_TYPE_CG packet_type;
    MSG_CG_GOLDRUSH_PACKET()
    {
        category_type = eCHAOSZONE_CATEGORY_GOLDRUSH;
    }
};

struct MSG_CG_GOLDRUSH_RESERVATION_SYN : MSG_CG_GOLDRUSH_PACKET
{
    MAPCODE map_code;   //>>���ڵ�
    bool is_healer;     //>>��������

    MSG_CG_GOLDRUSH_RESERVATION_SYN()
    {
        packet_type = CG_GOLDRUSH_RESERVATION_SYN;
        map_code = 0;
        is_healer = false;
    }
};
struct MSG_CG_GOLDRUSH_RESERVATION_ACK : MSG_CG_GOLDRUSH_PACKET
{
    RC::eBATTLE_GROUND_RESULT result_code;
    WORD goldrush_reservation_key;

    MSG_CG_GOLDRUSH_RESERVATION_ACK()
    {
        packet_type = CG_GOLDRUSH_RESERVATION_ACK;
    }
};

struct MSG_CG_GOLDRUSH_RESERVATION_BRD : MSG_CG_GOLDRUSH_PACKET
{
    WORD current_reservation_memeber_count;
    MSG_CG_GOLDRUSH_RESERVATION_BRD()
    {
        current_reservation_memeber_count = 0;
        packet_type = CG_GOLDRUSH_RESERVATION_BRD;
    }
};


struct MSG_CG_GOLDRUSH_RESERVATION_CANCEL_SYN : MSG_CG_GOLDRUSH_PACKET
{
    MSG_CG_GOLDRUSH_RESERVATION_CANCEL_SYN()
    {
        packet_type = CG_GOLDRUSH_RESERVATION_CANCEL_SYN;
    }
};

struct MSG_CG_GOLDRUSH_RESERVATION_CANCEL_ACK : MSG_CG_GOLDRUSH_PACKET
{
    RC::eBATTLE_GROUND_RESULT result_code;

    MSG_CG_GOLDRUSH_RESERVATION_CANCEL_ACK()
    {
        packet_type = CG_GOLDRUSH_RESERVATION_CANCEL_ACK;
    }
};

struct MSG_CG_GOLDRUSH_TEAM_INFO_CMD : MSG_CG_GOLDRUSH_PACKET
{
    WORD team_key;
    MSG_CG_GOLDRUSH_TEAM_INFO_CMD()
    {
        packet_type = CG_GOLDRUSH_TEAM_INFO_CMD;
    }
};

struct MSG_CG_GOLDRUSH_TEAM_INFO_BRD : MSG_CG_GOLDRUSH_PACKET
{
    BYTE member_count;
    GOLDRUSH_TEAM_MEMBER_INFO member_list[20];

    MSG_CG_GOLDRUSH_TEAM_INFO_BRD()
    {
        packet_type = CG_GOLDRUSH_TEAM_INFO_BRD;
        member_count = 0;
        memset(member_list, 0, sizeof(member_list));
    }
};

// �������������� ���� ��Ŷ
struct MSG_CG_GOLDRUSH_ABNORMAL_EXIT_USER_REMOVE_SYN : MSG_CG_GOLDRUSH_PACKET
{
    MSG_CG_GOLDRUSH_ABNORMAL_EXIT_USER_REMOVE_SYN()
    {
        packet_type = CG_GOLDRUSH_ABNORMAL_EXIT_USER_REMOVE_SYN;
    }
};

struct MSG_CG_GOLDRUSH_ABNORMAL_EXIT_USER_RETURN_SYN : MSG_CG_GOLDRUSH_PACKET
{
    // �� ���� �� �������Ʈ���� ������
    // ��巯�� ���� �� ���������� ���� ��Ŷ
    struct GoldrushTeamInfo
    {
        ChaosZoneTeam team_key;
        WORD total_gold_point;

        GoldrushTeamInfo() : team_key(kChaosZoneTeamNone), total_gold_point(0) {}
    };

    DWORD remained_time;    // (ms)
    GoldrushTeamInfo team_info[2];

    MSG_CG_GOLDRUSH_ABNORMAL_EXIT_USER_RETURN_SYN() : remained_time(0)
    {
        packet_type = CG_GOLDRUSH_ABNORMAL_EXIT_USER_RETURN_SYN;
    }
};

// ��Ȱ���� ��Ŷ
struct MSG_CG_GOLDRUSH_STATE_RESURECTION_CMD : MSG_CG_GOLDRUSH_PACKET
{
    DWORD player_object_key;
    BattleGroundPlayerState player_state;
    DWORD resurection_time;
    MSG_CG_GOLDRUSH_STATE_RESURECTION_CMD()
    {
        player_object_key = 0;
        packet_type = CG_GOLDRUSH_STATE_RESURECTION_CMD;
        player_state = kPlayerStateNone;
        resurection_time = 0;
    }
};

// ���� ���
struct MSG_CG_GOLDRUSH_FINISH_RESULT_BRD : MSG_CG_GOLDRUSH_PACKET
{
    ChaosZoneResultType result_type;
    MSG_CG_GOLDRUSH_FINISH_RESULT_BRD()
    {
        result_type = kResultNone;
        packet_type = CG_GOLDRUSH_FINISH_RESULT_BRD;
    }
};

// ���� �ð�����
struct MSG_CG_GOLDRUSH_TIME_INFO_CMD : MSG_CG_GOLDRUSH_PACKET
{
    DWORD elapsed_time;
    MAPCODE map_code;
    MSG_CG_GOLDRUSH_TIME_INFO_CMD()
    {
        packet_type = CG_GOLDRUSH_TIME_INFO_CMD;
        elapsed_time = 0;
        map_code = 0;
    }
};

// _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
/*
struct MSG_CG_GOLDRUSH_GET_OPEN_TIME_SYN : MSG_CG_GOLDRUSH_PACKET
{
    MSG_CG_GOLDRUSH_GET_OPEN_TIME_SYN()
    {
        packet_type = CG_GOLDRUSH_GET_OPEN_TIME_SYN;
    }
};

struct MSG_CG_GOLDRUSH_GET_OPEN_TIME_ACK : MSG_CG_GOLDRUSH_PACKET
{
    JoinBattleGroundState join_state;
    DWORD join_time;
    MSG_CG_GOLDRUSH_GET_OPEN_TIME_ACK()
    {
        packet_type = CG_GOLDRUSH_GET_OPEN_TIME_ACK;
        join_state = kNone;
        join_time = INFINITE;
    }
};
*/

struct MSG_CG_GOLDRUSH_BATTLE_STATE_NOTIFY_BRD : MSG_CG_GOLDRUSH_PACKET
{
    TCHAR kill_player_name[MAX_CLIENT_CHARNAME_LENGTH];
    TCHAR dead_player_name[MAX_CLIENT_CHARNAME_LENGTH];

    bool is_killer_monster; //ų�� ĳ���Ͱ� �����ΰ�? �����̸� killer_team�� ������ �����ʴ´�
    bool is_series_kill; // ����ų ����
    WORD series_kill_count; // ���� ų ��
    bool is_series_kill_block; // ����ų ���� ����

    WORD steal_gold;
    GOLDRUSH_TEAM_SCORE_INFO killer_team;
    GOLDRUSH_TEAM_SCORE_INFO dead_team;

    MSG_CG_GOLDRUSH_BATTLE_STATE_NOTIFY_BRD()
    {
        packet_type = CG_GOLDRUSH_BATTLE_STATE_NOTIFY_BRD;
        memset(&kill_player_name, 0, _countof(kill_player_name));
        memset(&dead_player_name, 0, _countof(dead_player_name));
        is_series_kill = false;
        is_killer_monster = false;
        series_kill_count = 0;
        is_series_kill_block = false;
        
        steal_gold = 0;
    }
};

struct MSG_CG_GOLDRUSH_MONSTERKILL_NOTIFY_BRD : MSG_CG_GOLDRUSH_PACKET
{
    TCHAR kill_player_name[MAX_CLIENT_CHARNAME_LENGTH];
    MONSTERCODE dead_monster_code;
    WORD get_goldpoint;

    bool is_boss_monster;

    GOLDRUSH_TEAM_SCORE_INFO killer_team;

    MSG_CG_GOLDRUSH_MONSTERKILL_NOTIFY_BRD() : is_boss_monster(false), get_goldpoint(0), dead_monster_code(0)
    {
        packet_type = CG_GOLDRUSH_MONSTERKILL_NOTIFY_BRD;
        memset(&kill_player_name, 0, _countof(kill_player_name));
    }
};

struct MSG_CG_GOLDRUSH_TEAM_SCORE_INFO_BRD : MSG_CG_GOLDRUSH_PACKET
{
    // 1������ ��������� ����
    // ���������� �÷��̾� ����, �÷��̾� �߹� ���� ������ ����
    WORD get_goldpoint; // �� �÷��̾ �޴� �������Ʈ
    GOLDRUSH_TEAM_SCORE_INFO team;

    MSG_CG_GOLDRUSH_TEAM_SCORE_INFO_BRD() : get_goldpoint(0) {
        packet_type = CG_GOLDRUSH_TEAM_SCORE_INFO_BRD;
    }
};

struct MSG_CG_GOLDRUSH_TOTAL_TEAM_POINT_BRD : MSG_CG_GOLDRUSH_PACKET
{
    // �� ���� �� �������Ʈ���� ������
    // ��巯�� ���� �� ���������� ���� ��Ŷ
    struct GoldrushTeamInfo
    {
        ChaosZoneTeam team_key;
        WORD total_gold_point;

        GoldrushTeamInfo() : team_key(kChaosZoneTeamNone), total_gold_point(0) {}
    };

    GoldrushTeamInfo team_info[2];

    MSG_CG_GOLDRUSH_TOTAL_TEAM_POINT_BRD() {
        packet_type = CG_GOLDRUSH_TOTAL_TEAM_POINT_BRD;
    }
};
// VOTE System
struct MSG_CG_GOLDRUSH_INITIATE_VOTE_SYN : MSG_CG_GOLDRUSH_PACKET
{
    TCHAR van_payer_name[MAX_CLIENT_CHARNAME_LENGTH];
    MSG_CG_GOLDRUSH_INITIATE_VOTE_SYN()
    {
        packet_type = CG_GOLDRUSH_INITIATE_VOTE_SYN;
        memset(&van_payer_name, 0, _countof(van_payer_name));
    }
};

struct MSG_CG_GOLDRUSH_INITIATE_VOTE_NAK : MSG_CG_GOLDRUSH_PACKET
{
    RC::eBATTLE_GROUND_RESULT result;
    MSG_CG_GOLDRUSH_INITIATE_VOTE_NAK()
    {
        packet_type = CG_GOLDRUSH_INITIATE_VOTE_NAK;
        result = RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL;
    }
};

struct MSG_CG_GOLDRUSH_INITIATE_VOTE_BRD : MSG_CG_GOLDRUSH_PACKET
{
    TCHAR van_payer_name[MAX_CLIENT_CHARNAME_LENGTH];
    MSG_CG_GOLDRUSH_INITIATE_VOTE_BRD()
    {
        packet_type = CG_GOLDRUSH_INITIATE_VOTE_BRD;
        memset(&van_payer_name, 0, _countof(van_payer_name));
    }
};
struct MSG_CG_GOLDRUSH_VOTE_STATUS_BRD : MSG_CG_GOLDRUSH_PACKET
{
    TCHAR van_payer_name[MAX_CLIENT_CHARNAME_LENGTH];
    BYTE yes_count;
    BYTE no_count;
    DWORD remain_vote_time;
    MSG_CG_GOLDRUSH_VOTE_STATUS_BRD()
    {
        packet_type = CG_GOLDRUSH_VOTE_STATUS_BRD;
        memset(&van_payer_name, 0, _countof(van_payer_name));
        yes_count = 0;
        no_count = 0;
        remain_vote_time = 0;
    }
};

struct MSG_CG_GOLDRUSH_VOTE_SYN : MSG_CG_GOLDRUSH_PACKET
{
    bool vote_result;
    MSG_CG_GOLDRUSH_VOTE_SYN()
    {
        packet_type = CG_GOLDRUSH_VOTE_SYN;
        vote_result = true;
    }
};

struct MSG_CG_GOLDRUSH_VOTE_RESULT_BRD : MSG_CG_GOLDRUSH_PACKET
{
    bool vote_result;
    TCHAR van_payer_name[MAX_CLIENT_CHARNAME_LENGTH];
    MSG_CG_GOLDRUSH_VOTE_RESULT_BRD()
    {
        packet_type = CG_GOLDRUSH_VOTE_RESULT_BRD;
        vote_result = false;
        memset(&van_payer_name, 0, _countof(van_payer_name));
    }
};
struct MSG_CG_GOLDRUSH_VANP_PLAYER_CMD : MSG_CG_GOLDRUSH_PACKET
{
    MSG_CG_GOLDRUSH_VANP_PLAYER_CMD()
    {
        packet_type = CG_GOLDRUSH_VANP_PLAYER_CMD;
    }
};
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

//----------------------------------------------------------------------------------------------
#ifdef _NA_008334_20150608_SONNENSCHEIN
enum eSONNENSCHEIN_PACKET_TYPE_CG
{
    CG_SONNENSCHEIN_CREATE_SYN = 0,
    CG_SONNENSCHEIN_CREATE_ACK,

    CG_SONNENSCHEIN_ENTER_CHECK_SYN,
    CG_SONNENSCHEIN_ENTER_CHECK_ACK,

    CG_SONNENSCHEIN_BEGIN_STAMP_BRD,
    CG_SONNENSCHEIN_CANCEL_STAMP_BRD,

    CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN,
    CG_SONNENSCHEIN_ENTERPLAYER_LIST_ACK,
};

struct MSG_CG_SONNENSCHEIN_PACKET : MSG_CG_CHAOS_ZONE_PACKET
{
    eSONNENSCHEIN_PACKET_TYPE_CG packet_type;
    MSG_CG_SONNENSCHEIN_PACKET()
    {
        category_type = eCHAOSZONE_CATEGORY_SONNENSCHEIN;
    }
};

struct MSG_CG_SONNENSCHEIN_CREATE_SYN : MSG_CG_SONNENSCHEIN_PACKET
{
    MSG_CG_SONNENSCHEIN_CREATE_SYN()
    {
        packet_type = CG_SONNENSCHEIN_CREATE_SYN;
    }
};

struct MSG_CG_SONNENSCHEIN_CREATE_ACK : MSG_CG_SONNENSCHEIN_PACKET
{
    MSG_CG_SONNENSCHEIN_CREATE_ACK()
    {
        packet_type = CG_SONNENSCHEIN_CREATE_ACK;
        result_code = RC::RC_SONNENSCHEIN_FAILED;
    }

    RC::eSONNENSCHEIN_RESULT result_code;
};

struct MSG_CG_SONNENSCHEIN_ENTER_CHECK_SYN : MSG_CG_SONNENSCHEIN_PACKET
{
    MSG_CG_SONNENSCHEIN_ENTER_CHECK_SYN()
    {
        packet_type = CG_SONNENSCHEIN_ENTER_CHECK_SYN;

        entrance_item_code = 0;
    }
    SLOTCODE entrance_item_code;
};

struct MSG_CG_SONNENSCHEIN_ENTER_CHECK_ACK : MSG_CG_SONNENSCHEIN_PACKET
{
    MSG_CG_SONNENSCHEIN_ENTER_CHECK_ACK()
    {
        packet_type = CG_SONNENSCHEIN_ENTER_CHECK_ACK;
        result_code = RC::RC_SONNENSCHEIN_FAILED;
        sonnenschein_mapcode = 0;
    }

    RC::eSONNENSCHEIN_RESULT result_code;
    MAPCODE sonnenschein_mapcode;
};

struct MSG_CG_SONNENSCHEIN_BEGIN_STAMP_BRD : MSG_CG_SONNENSCHEIN_PACKET
{
    MSG_CG_SONNENSCHEIN_BEGIN_STAMP_BRD()
    {
        packet_type = CG_SONNENSCHEIN_BEGIN_STAMP_BRD;

        stamp_id = 0;
    }

    WzID stamp_id;
};

struct MSG_CG_SONNENSCHEIN_CANCEL_STAMP_BRD : MSG_CG_SONNENSCHEIN_PACKET
{
    MSG_CG_SONNENSCHEIN_CANCEL_STAMP_BRD()
    {
        packet_type = CG_SONNENSCHEIN_CANCEL_STAMP_BRD;

        stamp_id = 0;
    }

    WzID stamp_id;
};

struct MSG_CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN : MSG_CG_SONNENSCHEIN_PACKET
{
    MSG_CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN()
    {
        packet_type = CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN;
    }
};

struct MSG_CG_SONNENSCHEIN_ENTERPLAYER_LIST_ACK : MSG_CG_SONNENSCHEIN_PACKET
{
    MSG_CG_SONNENSCHEIN_ENTERPLAYER_LIST_ACK()
    {
        packet_type = CG_SONNENSCHEIN_ENTERPLAYER_LIST_ACK;

        enter_player_count = 0;
        ZeroMemory(enter_player_list, sizeof(enter_player_list));
    }

    CHARGUID enter_player_list[MAX_GUILD_MEMBER_NUM];
    INT enter_player_count;
};
#endif //_NA_008334_20150608_SONNENSCHEIN

//----------------------------------------------------------------------------------------------
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
enum eONEDAYSGUILDMISSION_PACKET_TYPE_CG
{
	CG_ONEDAYSGUILDMISSION_CREATE_SYN = 0,
	CG_ONEDAYSGUILDMISSION_CREATE_ACK,

	CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN,
	CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK,

	CG_ONEDAYSGUILDMISSION_BEGIN_STAMP_BRD,
	CG_ONEDAYSGUILDMISSION_CANCEL_STAMP_BRD,

	CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN,
	CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_ACK,

	CG_ONEDAYSGUILDMISSION_TIME_STATE_INFO_BRD,											 // ����ð�����
	CG_ONEDAYSGUILDMISSION_ABNORMAL_EXIT_USER_RETURN_SYN,
	
	CG_ONEDAYSGUILDMISSION_PLAYER_INFO_BRD,

	CG_ONEDAYSGUILDMISSION_STATE_RESURECTION_CMD,
};

struct MSG_CG_ONEDAYSGUILDMISSION_PACKET : MSG_CG_CHAOS_ZONE_PACKET
{
	eONEDAYSGUILDMISSION_PACKET_TYPE_CG packet_type;
	MSG_CG_ONEDAYSGUILDMISSION_PACKET()
	{
		category_type = eCHAOSZONE_CATEGORY_ONEDAYSGUILDMISSION;
	}
};

struct MSG_CG_ONEDAYSGUILDMISSION_CREATE_SYN : MSG_CG_ONEDAYSGUILDMISSION_PACKET
{
	MSG_CG_ONEDAYSGUILDMISSION_CREATE_SYN()
	{
		packet_type = CG_ONEDAYSGUILDMISSION_CREATE_SYN;
	}
};

struct MSG_CG_ONEDAYSGUILDMISSION_CREATE_ACK : MSG_CG_ONEDAYSGUILDMISSION_PACKET
{
	MSG_CG_ONEDAYSGUILDMISSION_CREATE_ACK()
	{
		packet_type = CG_ONEDAYSGUILDMISSION_CREATE_ACK;
		result_code = RC::RC_ONEDAYSGUILDMISSION_FAILED;
	}

	RC::eONEDAYSGUILDMISSION_RESULT result_code;
};

struct MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN : MSG_CG_ONEDAYSGUILDMISSION_PACKET
{
	MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN()
	{
		packet_type = CG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN;

		entrance_item_code = 0;
	}

	SLOTCODE entrance_item_code;
};

struct MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK : MSG_CG_ONEDAYSGUILDMISSION_PACKET
{
	MSG_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK()
	{
		packet_type = CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK;
		result_code = RC::RC_ONEDAYSGUILDMISSION_FAILED;
		onedaysguildmissin_mapcode = 0;
	}

	RC::eONEDAYSGUILDMISSION_RESULT result_code;
	MAPCODE onedaysguildmissin_mapcode;
};

struct MSG_CG_ONEDAYSGUILDMISSION_BEGIN_STAMP_BRD : MSG_CG_ONEDAYSGUILDMISSION_PACKET
{
	MSG_CG_ONEDAYSGUILDMISSION_BEGIN_STAMP_BRD()
	{
		packet_type = CG_ONEDAYSGUILDMISSION_BEGIN_STAMP_BRD;

		stamp_id = 0;
	}

	WzID stamp_id;
};

struct MSG_CG_ONEDAYSGUILDMISSION_CANCEL_STAMP_BRD : MSG_CG_ONEDAYSGUILDMISSION_PACKET
{
	MSG_CG_ONEDAYSGUILDMISSION_CANCEL_STAMP_BRD()
	{
		packet_type = CG_ONEDAYSGUILDMISSION_CANCEL_STAMP_BRD;

		stamp_id = 0;
	}

	WzID stamp_id;
};

struct MSG_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN : MSG_CG_ONEDAYSGUILDMISSION_PACKET
{
	MSG_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN()
	{
		packet_type = CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_SYN;
	}
};

struct MSG_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_ACK : MSG_CG_ONEDAYSGUILDMISSION_PACKET
{
	MSG_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_ACK()
	{
		packet_type = CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_ACK;

		event_state_ = 0;
		remain_time_ = 0;

		stand_by_user_list_cnt = 0;
		ZeroMemory(stand_by_user_list, sizeof(stand_by_user_list));

		enter_player_count = 0;
		ZeroMemory(enter_player_list, sizeof(enter_player_list));
	}

	BYTE event_state_;
	DWORD remain_time_; //�и�������

	CHARGUID stand_by_user_list[MAX_GUILD_MEMBER_NUM];
	INT stand_by_user_list_cnt;

	CHARGUID enter_player_list[MAX_GUILD_MEMBER_NUM];
	INT enter_player_count;
};

struct MSG_CG_ONEDAYSGUILDMISSION_TIME_STATE_INFO_BRD : MSG_CG_ONEDAYSGUILDMISSION_PACKET
{
	BYTE event_state_;
	DWORD remain_time_; //�и�������

	MSG_CG_ONEDAYSGUILDMISSION_TIME_STATE_INFO_BRD()
	{
		packet_type = CG_ONEDAYSGUILDMISSION_TIME_STATE_INFO_BRD;
		event_state_ = 0;
		remain_time_ = 0;
	}
};

struct MSG_CG_ONEDAYSGUILDMISSION_ABNORMAL_EXIT_USER_RETURN_SYN : MSG_CG_ONEDAYSGUILDMISSION_PACKET
{
	DWORD remained_time; //�и�������

	MSG_CG_ONEDAYSGUILDMISSION_ABNORMAL_EXIT_USER_RETURN_SYN()
	{
		packet_type = CG_ONEDAYSGUILDMISSION_ABNORMAL_EXIT_USER_RETURN_SYN;
		remained_time = 0;
	}
};

struct MSG_CG_ONEDAYSGUILDMISSION_PLAYER_INFO_BRD : MSG_CG_ONEDAYSGUILDMISSION_PACKET
{
	BYTE infoflag;
	CHARGUID enter_player_guid;

	MSG_CG_ONEDAYSGUILDMISSION_PLAYER_INFO_BRD()
	{
		packet_type = CG_ONEDAYSGUILDMISSION_PLAYER_INFO_BRD;
		infoflag = 0;											// 0: none, 1: Enter, 2: Leave
		enter_player_guid = 0;
	}
};

struct MSG_CG_ONEDAYSGUILDMISSION_STATE_RESURECTION_CMD : MSG_CG_ONEDAYSGUILDMISSION_PACKET
{
	DWORD player_object_key;
	BattleGroundPlayerState player_state;
	DWORD resurection_time;
	MSG_CG_ONEDAYSGUILDMISSION_STATE_RESURECTION_CMD()
	{
		player_object_key = 0;
		packet_type = CG_ONEDAYSGUILDMISSION_STATE_RESURECTION_CMD;
		player_state = kPlayerStateNone;
		resurection_time = 0;
	}
};

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)


#endif //_PROGRAMCOMMON_PACKETSTRUCT_CHAOSZONESYSTEM_H_