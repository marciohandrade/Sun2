#pragma once
//=============================================================================================================================
/// ZoneCommon.h
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2005. 12. 6
	@remark
		- 존에 관련된 공통된 정보
	@note
		- 
	@history 
		- 2008.02.06, Waverix, 개별 Pool정책에서 PoolRouter로 정책 변경
		- 2008.02.07, Waverix, MoveZone에 대한 형태, 권한에 따른 세부 구현 통합체계 구축
	@usage
		- 
*/
//=============================================================================================================================

enum eSEND_TYPE
{
	eSEND_SYN,
	eSEND_CMD,
	eSEND_NONE,
};

class User;

class ZoneInterface;	class RoomInterface;

class CharSelect;
class Village;			class HuntingField;		class LobbyRoom;
class MissionRoom;		class PVPRoom;			class CHuntingRoom;
class HuntingRoom;
class InstanceDungeon;
class DungeonTopOfTrial;
class DominationField;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
class SpaField;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#define _DECLARE_POOL( CLASSNAME )
#define _IMPL_POOL( CLASSNAME )

//─────────────────────────────────────────
//
template< eZONETYPE _ZTYPE, typename _TYPE >
struct zone_info : sun_type_info< _TYPE >
{
	static const eZONETYPE				value = _ZTYPE;
	typedef _TYPE						type;
	typedef ZoneInterface				base_type;
	static pointer downcast( base_type* pZone ) { return DOWNCASTER_SELECT<pointer>( pZone ); };
};

template< eINSTANCE_DUNGEON_KIND _ZTYPE, typename _TYPE >
struct indun_info : zone_info< eZONETYPE_MAX, _TYPE >
{
	static const eINSTANCE_DUNGEON_KIND	value = _ZTYPE;
};


//	MoveZone 이동 형태
enum eMOVE_ZONE_TYPE_FILTER
{
	MZTF_CHECK_SKIP					= 0,
	MZTF_STATIC_TO_STATIC			,		//	FIELD<->FIELD, FIELD<->VILLAGE
	MZTF_ROOM_TO_STATIC				,		//	ROOM->FIELD, ROOM->VILLAGE
	MZTF_VILLAGE_TO_LOBBY			,		//	VILLAGE->LOBBY, TUTORIAL->LOBBY
	MZTF_LOBBY_TO_VILLAGE			,		//	LOBBY->VILLAGE,	LOBBY->TUTORIAL
	MZTF_VILLAGE_TO_HUNTING			,		//	VILLAGE->HUNTING
	//MZTF_LOBBY_TO_ROOM			,		//	LOBBY->ROOM	(NORMAL) - use RoomMigrater
	MZTF_CHUNTING_TO_VILLAGE		,		//	CHUNTING->VILLAGE
	MZTF_HUNTING_TO_HUNTING			,		//	RELAY HUNTING
	MZTF_ROOM_TO_LOBBY				,		//	RELAY MISSION
	MZTF_STATIC_TO_ROOM				,		//	VILLAGE->ROOM, FIELD->ROOM
	MZTF_STATIC_TO_ROOM_FOR_UCUSER	,		//	VILLAGE->ROOM, FIELD->ROOM (for Unexpected Closing User Return to Party)
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<CheckMoveZonePolicy>
//	<Sample> - MoveZone
//		CheckMoveZonePolicy* const pCheckMoveZonePolicy = CheckMoveZonePolicy::AlignInfo(
//			MZTF_CHUNTING_TO_VILLAGE, pUser
//			//{		From		|			To		}
//			, eZONETYPE_CHUNTING,	eZONETYPE_VILLAGE	//	eZONETYPE
//			, GetKey(),				pVillage->GetKey()	//	ZoneKey
//			);
//		const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(
//			pCheckMoveZonePolicy, RoomArgument&
//			);
//
struct CheckMoveZonePolicyArgs
{
	CheckMoveZonePolicyArgs();
	CheckMoveZonePolicyArgs(
		User* const						pUser
		, const eZONETYPE				FromZoneType
		, const eZONETYPE				ToZoneType
		, const KEYTYPE					FromZoneKey
		, const KEYTYPE					ToZoneKey
		, ZoneInterface* const			FromZonePtr			// FromKey에 대한 Zone
		, ZoneInterface* const			ToZonePtr			// ToKey에 대한 Zone
		, const eZONETYPE				FromZoneTypeByObj	// FromZone의 실제 타입
		, const eZONETYPE				ToZoneTypeByObj		// ToZone의 실제 타입
		);

	User* const							pUser;
	const eZONETYPE						FromZoneType;
	const eZONETYPE						ToZoneType;
	const KEYTYPE						FromZoneKey;
	const KEYTYPE						ToZoneKey;
	ZoneInterface* const				FromZonePtr;		// FromKey에 대한 Zone
	ZoneInterface* const				ToZonePtr;			// ToKey에 대한 Zone
	const eZONETYPE						FromZoneTypeByObj;	// FromZone의 실제 타입
	const eZONETYPE						ToZoneTypeByObj;	// ToZone의 실제 타입
    __DISABLE_COPY(CheckMoveZonePolicyArgs);
};
//////////////////////////////////////////////////////////////////////////
class CheckMoveZonePolicy
{
protected:
	CheckMoveZonePolicy() : m_pImpl(0) {}
public:
	virtual RC::eROOM_RESULT	EnableCheck() = 0;

public:	// runtime checker - for single code instance
	static	CheckMoveZonePolicy*	_AlignInfo(
										const eMOVE_ZONE_TYPE_FILTER	moveType
										, User* const					pUser
										, const eZONETYPE				FromZoneType
										, const eZONETYPE				ToZoneType
										, const KEYTYPE					FromZoneKey
										, const KEYTYPE					ToZoneKey
										);
public:
	inline	bool					ValidCheck() { return ( m_pImpl && Args->pUser && Args->FromZonePtr && Args->ToZonePtr ); }
private:
	inline	VOID					_Clear();
private:	CheckMoveZonePolicy*		m_pImpl;

			friend class ZoneManager;
protected:	CheckMoveZonePolicyArgs		Args[1];
    __DISABLE_COPY(CheckMoveZonePolicy);
};


//─────────────────────────────────────────
//	<GOAL>
//	-	이곳 저곳에 난립하는 Zone 이동 컨트롤 제약 조건 검사 루틴들을 이곳으로 이전
//		시키기 위함이다.
#undef MOVE_ZONE_CTRL_NODE
#define MOVE_ZONE_CONTROL_LIST()								\
	MOVE_ZONE_CTRL_NODE( MZTF_CHECK_SKIP				)		\
	MOVE_ZONE_CTRL_NODE( MZTF_STATIC_TO_STATIC			)		\
	MOVE_ZONE_CTRL_NODE( MZTF_ROOM_TO_STATIC			)		\
	MOVE_ZONE_CTRL_NODE( MZTF_VILLAGE_TO_LOBBY			)		\
	MOVE_ZONE_CTRL_NODE( MZTF_LOBBY_TO_VILLAGE			)		\
	MOVE_ZONE_CTRL_NODE( MZTF_VILLAGE_TO_HUNTING		)		\
	MOVE_ZONE_CTRL_NODE( MZTF_CHUNTING_TO_VILLAGE		)		\
	MOVE_ZONE_CTRL_NODE( MZTF_HUNTING_TO_HUNTING		)		\
	MOVE_ZONE_CTRL_NODE( MZTF_ROOM_TO_LOBBY				)		\
	MOVE_ZONE_CTRL_NODE( MZTF_STATIC_TO_ROOM			)		\
	MOVE_ZONE_CTRL_NODE( MZTF_STATIC_TO_ROOM_FOR_UCUSER	)

template<eMOVE_ZONE_TYPE_FILTER	_MOVTYPE> struct MoveZonePolicy;

//	compile-time checker for specific declaration
#undef MOVE_ZONE_CTRL_NODE
#define MOVE_ZONE_CTRL_NODE( mztf )											\
	template<> struct MoveZonePolicy<mztf> {								\
		static CheckMoveZonePolicy*	AlignInfo(								\
			User* const			pUser										\
			, const eZONETYPE	FromZoneType, const eZONETYPE	ToZoneType	\
			, const KEYTYPE		FromZoneKey, const KEYTYPE		ToZoneKey	\
			)																\
		{ return CheckMoveZonePolicy::_AlignInfo( mztf, pUser, FromZoneType, ToZoneType, FromZoneKey, ToZoneKey ); }	\
	};

MOVE_ZONE_CONTROL_LIST();		// declare specific list

//─────────────────────────────────────────
//	<Sample><Specific-Implementation>
//
template< typename eMOVE_ZONE_TYPE_FILTER _MOVTYPE >
class CheckMoveZonePolicy_Impl : public CheckMoveZonePolicy
{
//private:
//	static const eMOVE_ZONE_TYPE_FILTER		movetype = _MOVTYPE;
//	static const eMOVE_ZONE_AUTHORITY		moveauth = _MOVAUTH;
//
//public:
//	virtual	RC::eROOM_RESULT	EnableCheck() = 0;
};


typedef util::cSTRING_BUFFER<MAX_ROOMTITLE_LENGTH>		ROOM_TITLE_BUFFER;
typedef util::cSTRING_BUFFER<MAX_ROOMPASSWORD_LENGTH>		ROOM_PASSWORD_BUFFER;

//	이후 존 관련 처리시 일일히 인자를 설정하지 말고 모으자.
//	일단 작업량이 많아지는 관계로 MoveZone에 필요한 인자만 모은다.
struct RoomArgument
{
	static const RoomArgument&	GetEmpty();
	RoomArgument(
				const eZONEPUBLIC RoomPublic		= eZONEPUBLIC_PUBLIC
				, const TCHAR* const pszRoomPWD		= NULL
				, const WORD wToFieldCode			= 0
				, const DWORD dwToAreaID			= 0
				, WzVector* const pwvStartPos		= NULL
				)
				: RoomExportLevel( RoomPublic )
				, ToFieldCode( wToFieldCode )
				, ToAreaID( dwToAreaID )
				, pStartPos( pwvStartPos )
	{
		if( pszRoomPWD ) Password.CopyFrom( pszRoomPWD );
	}

	const eZONEPUBLIC					RoomExportLevel;
	const FIELDCODE						ToFieldCode;
	const DWORD							ToAreaID;
	WzVector* const						pStartPos;
	ROOM_PASSWORD_BUFFER				Password;
    __DISABLE_COPY(RoomArgument);
};

//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

struct RoomArgumentForRoomSetup : public RoomArgument
{
    RoomArgumentForRoomSetup(
        const eZONEPUBLIC RoomPublic		= eZONEPUBLIC_PUBLIC
        , const TCHAR* const pszRoomPWD		= NULL
        , const WORD wToFieldCode			= 0
        , const DWORD dwToAreaID			= 0
        , WzVector* const pwvStartPos		= NULL
        )
        : RoomArgument(RoomPublic, pszRoomPWD, wToFieldCode, dwToAreaID, pwvStartPos)
        , m_pMapInfo(0), m_MaxLimitUserNum(0)
    {
        ZeroMemory(&m_RoomInfo, sizeof(m_RoomInfo));
    }

    BASE_ROOMINFO   m_RoomInfo;
    const sMAPINFO* m_pMapInfo;
    BYTE            m_MaxLimitUserNum;
    __DISABLE_COPY(RoomArgumentForRoomSetup);
};


class RoomInterface;

// RoomInterface에 존재하는 Room관련 제어 플랙 집합
// 초기화 위치: 생성자 | On_destroy
// 재설정 위치: ZoneManager 현 시점(090211) 기존 구조 포괄 포탈형 방 생성을 위한 플랙제어
//목적으로만 사용하므로 CreatePortal등지에서만 플랙값을 변경시키도록 했다. 차후 일반화
//시킬 필요 있음. 초기화 시키는 것이 바로 해당 타입의 Create(~~)라서 정리되면 수정하자.
class PolicymentForRoom
{
public:
    typedef DWORD   POLICY_T;
    enum eFlag {
        DISABLE_ALL             = 0,
        QUEST_PORTAL_CONTROL    = 1<<10,
    };

    static const DWORD MonsterAbilityChangableRoomsBit = (1 << eZONETYPE_MISSION)
                                                       | (1 << eZONETYPE_HUNTING)
                                                       | (1 << eZONETYPE_CHUNTING)
                                                       | (1 << eZONETYPE_INSTANCE);

    PolicymentForRoom() : FIELD(m_Field)
                        , m_Field(DISABLE_ALL)
                        , m_PortalKey(INVALID_PORTAL_KEY)
                        , m_PortalMasterKey(0)
    {}
    inline VOID Clear()
    {
        m_Field = DISABLE_ALL;
        m_PortalKey = INVALID_PORTAL_KEY;
        m_PortalMasterKey = 0;
    }

    inline PORTALKEY GetPortalKey() const { return m_PortalKey; }
    inline DWORD GetReservedPortalMaster() const { return m_PortalMasterKey; }
    inline VOID SetReservedPortalMaster(DWORD portalMaster) { m_PortalMasterKey = WORD(portalMaster); }

    const POLICY_T& FIELD;
private:
    inline VOID SetField(const POLICY_T flags) { m_Field = (POLICY_T)flags; }
    inline VOID SetPortalKey(PORTALKEY portalKey) { m_PortalKey = portalKey; }

    POLICY_T m_Field;
    PORTALKEY m_PortalKey;
    WORD m_PortalMasterKey;

    friend class RoomInterface;
    friend class ZoneManager;
    __DISABLE_COPY(PolicymentForRoom);
};
//}__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#include "ZoneInterface.h"

