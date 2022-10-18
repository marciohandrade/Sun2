#pragma once

#include <Function/ObjectPoolRouter.h>

class UserSession;

enum eBEHAVE_TRANSACTION
{
	//����������������������������������������������������������������������������������
	//
	// <FLAG>
	TR_END				= ( 1<<1 ),
	TR_BEGIN			= ( 1<<2 ),

	//
	// <CONTROL>
	TR_CONTROL_RANGE	= ( 0x0000000F ),
	TR_FIELD_RANGE		= ( 0x7FFFFFF0 ),
	TR_STARTIDX			= ( 4 ),
	//
	//����������������������������������������������������������������������������������

	// ����
	TR_CREATEHUNTING					= ( TR_BEGIN | ( 1<<TR_STARTIDX) ),
	TR_JOINHUNTING						= ( TR_BEGIN | ( 2<<TR_STARTIDX) ),
	TR_LEAVEHUNTING						= ( TR_BEGIN | ( 3<<TR_STARTIDX) ),

	// ����â
	TR_BACKTOSELECT						= ( TR_BEGIN | ( 4<<TR_STARTIDX) ),

	// �κ�
	TR_CREATELOBBY						= ( TR_BEGIN | ( 5<<TR_STARTIDX) ),
	TR_JOINLOBBY						= ( TR_BEGIN | ( 6<<TR_STARTIDX) ),
	TR_LEAVELOBBY						= ( TR_BEGIN | ( 7<<TR_STARTIDX) ),

	// �̼�
	TR_CREATEMISSION					= ( TR_BEGIN | ( 8<<TR_STARTIDX) ),
	TR_JOINMISSION						= ( TR_BEGIN | ( 9<<TR_STARTIDX) ),
	TR_LEAVEMISSION						= ( TR_BEGIN | (10<<TR_STARTIDX) ),

	// PVP
	TR_CREATEPVP						= ( TR_BEGIN | (11<<TR_STARTIDX) ),
	TR_JOINPVP							= ( TR_BEGIN | (12<<TR_STARTIDX) ),
	TR_LEAVEPVP							= ( TR_BEGIN | (13<<TR_STARTIDX) ),

	// ��������
	TR_CREATECHUNTING					= ( TR_BEGIN | (14<<TR_STARTIDX) ),
	TR_JOINCHUNTING						= ( TR_BEGIN | (15<<TR_STARTIDX) ),
	TR_LEAVECHUNTING					= ( TR_BEGIN | (16<<TR_STARTIDX) ),

	//
	TR_LEAVE_LOBBY_FOR_HUNTING			= ( TR_BEGIN | (17<<TR_STARTIDX) ),
	TR_LEAVE_LOBBY_FOR_MISSION			= ( TR_BEGIN | (18<<TR_STARTIDX) ),
	TR_LEAVE_LOBBY_FOR_PVP				= ( TR_BEGIN | (19<<TR_STARTIDX) ),
	TR_LEAVE_LOBBY_FOR_CHUNTING			= ( TR_BEGIN | (20<<TR_STARTIDX) ),
	TR_LEAVE_HUNTING_FOR_VILLAGE		= ( TR_BEGIN | (21<<TR_STARTIDX) ),
	TR_LEAVE_MISSION_FOR_VILLAGE		= ( TR_BEGIN | (22<<TR_STARTIDX) ),
	TR_LEAVE_PVP_FOR_VILLAGE			= ( TR_BEGIN | (23<<TR_STARTIDX) ),
	TR_LEAVE_CHUNTING_FOR_VILLAGE		= ( TR_BEGIN | (24<<TR_STARTIDX) ),
	TR_LEAVE_HUNTING_FOR_HUNTING		= ( TR_BEGIN | (25<<TR_STARTIDX) ),
	//
	TR_JOIN_VILLAGE						= ( TR_BEGIN | (26<<TR_STARTIDX) ),
	TR_CHARACTER_LIST					= ( TR_BEGIN | (27<<TR_STARTIDX) ),
	TR_CHARACTER_CREATION				= ( TR_BEGIN | (28<<TR_STARTIDX) ),
	TR_CHARACTER_DESTRUCTION			= ( TR_BEGIN | (29<<TR_STARTIDX) ),
	TR_LEAVE_MISSION_FOR_LOBBY			= ( TR_BEGIN | (30<<TR_STARTIDX) ),
	TR_CHARACTER_RECOVERY				= ( TR_BEGIN | (31<<TR_STARTIDX) ),

	//�δ�
	TR_CREATE_INSTANCE_DUNGEON			= ( TR_BEGIN | (32<<TR_STARTIDX) ),	
	TR_LEAVE_FIELD_FOR_INSTANCE			= ( TR_BEGIN | (33<<TR_STARTIDX) ),
	TR_LEAVE_INSTANCE_DUNGEON			= ( TR_BEGIN | (34<<TR_STARTIDX) ),	

	TR_LEAVE_FIELD_FOR_ROOM				= ( TR_BEGIN | (35<<TR_STARTIDX) ),
	TR_LEAVE_FIELD_FOR_SSQFIELD			= ( TR_BEGIN | (36<<TR_STARTIDX) ),

#ifdef __NA_001044_ADD_CUSTOM_UI
	TR_CHARACTER_IDCHECK				= ( TR_BEGIN | (37<<TR_STARTIDX) ),
#endif
    TR_QUERY_PORTAL_UNTIL_GAMESERVER_ACK= ( TR_BEGIN | (38<<TR_STARTIDX) ),
    TR_LEAVE_FIELD_FOR_ROOM_BY_PORTAL   = ( TR_BEGIN | (39<<TR_STARTIDX) ),
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    TR_QUERY_SERVER_SELECT_TRANSACTION  = ( TR_BEGIN | (40<<TR_STARTIDX) ),
#endif
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    TR_LEAVE_FIELD_FOR_PARTY_MISSION    = ( TR_BEGIN | (41<<TR_STARTIDX) ),
    TR_LEAVE_FIELD_FOR_PARTY_PVP        = ( TR_BEGIN | (42<<TR_STARTIDX) ),
    TR_LEAVE_FIELD_FOR_PARTY_INSTANCEDUNGEON = ( TR_BEGIN | (43<<TR_STARTIDX) ),
    TR_LEAVE_FIELD_FOR_PARTY_SSQ        = ( TR_BEGIN | (44<<TR_STARTIDX) ),
};

//����������������������������������������������������������������������������������
//	<Pool State>
enum eUSER_STATE	 
{	
	POOL_STATE,						//< ������ Ǯ�� �ִ� ����
	INIT_STATE,						//< ������ ������ ���� TempUser->User�� �� ����
	CHARACTER_LIST_STATE,			//< ĳ���͸���Ʈ�� �޾ƿ� ����, ĳ���� ����, ĳ���� ���� ó��
};

// Object Pool ���� ���� �ε����� ����� ���� Ÿ�� ������
enum eUSER_TYPE
{
	UT_TEMP_USER		= OBJTYPE_TEMPUSER,
	UT_USER				= OBJTYPE_USER,
};

// PacketHandler�� ���� ������
enum eUSER_PARSER_TYPE
{
	UPT_USER,
	UPT_TEMP_USER,
	UPT_USER_MAX,
};


