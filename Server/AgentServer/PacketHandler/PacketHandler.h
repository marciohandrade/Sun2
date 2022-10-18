#pragma once

#include <FunctionMap.h>

//
// PACKET STRUCTUREs
#include <PacketStruct_CG.h>
#include <PacketStruct_AM.h>
#include <PacketStruct_AG.h>
#include <PacketStruct_AW.h>
#include <PacketStruct_AD.h>
#include <PacketStruct_AZ.h>
#include <SunAuth/AuthProtocol_GA.h>
//
#include <ServerSessions/ServerSessionEx.h>
#include <ServerSessions/TempServerSessionEx.h>
#include <ServerSessions/MasterServerSession.h>
#include <ServerSessions/AuthAgentServerSession.h>
#include <ServerSessions/DBProxyServerSession.h>

#include <ServerSessions/WorldServerSession.h>
#include <ServerSessions/GameServerSession.h>
#include <ServerSessions/BattleServerSession.h>
#include <ServerSessions/FieldServerSession.h>

//
#ifdef _DEBUG
#	include <CPShowMSG.h>
#define __NA000000_081104_REGISTERED_PROTOCOL_NAMES__
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
struct MSG_BASE;
class UserSession;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#pragma pack(push,1)
#pragma warning(push)
#pragma warning(disable : 4201)
// Agent 기준용
struct	PACKET_CONTROL
{
	static const DWORD	MAX_PACKET_SIZE = 16384;

	union uPACKET_STRUCTURE
	{
		BYTE	PacketStream[PACKET_CONTROL::MAX_PACKET_SIZE];

		struct
		{
			DWORD	dwUserKey;
			BYTE	pC2APacketPtr[1];
		};

		struct
		{
			WORD									wBLANK;
			union
			{
				MSG_CG_CRYPTOGRAPHY_PACKET_ENCODING	pMSG_ENCODING[1];
				MSG_CG_CRYPTOGRAPHY_PACKET_ENCODED	pMSG_ENCODED[1];
			};
		} BASE_PK;

		struct
		{
			DWORD					dwUserKey;
			MSG_BASE_INTERNAL		pMSG_BASE_INTERNAL[1];
		} FORWARD;

		struct 
		{
			MSG_BASE				pMSG_BASE[1];
			MSG_BASE_FORWARD		pMSG_BASE_FORWARD[1];
		} BASE;
	};

	static uPACKET_STRUCTURE		sRECV_PACKET_STRUCTURE;
	static uPACKET_STRUCTURE		sSEND_PACKET_STRUCTURE;

	// TempUser, User(no crypt), MSG_BASE root
	inline static BYTE*		GetPacketPtr_C2A() { return (BYTE*)sRECV_PACKET_STRUCTURE.pC2APacketPtr; }
	// TempUser, User(no crypt), MSG_BASE root
	inline static BYTE*		GetPacketPtr_A2C() { return (BYTE*)sSEND_PACKET_STRUCTURE.pC2APacketPtr; }

	// User(crypt)
	inline static MSG_CG_CRYPTOGRAPHY_PACKET_ENCODED* GetPacketPtr_C2A_ENCODED()
	{ return sRECV_PACKET_STRUCTURE.BASE_PK.pMSG_ENCODED; }
	// User(crypt)
	inline static MSG_CG_CRYPTOGRAPHY_PACKET_ENCODING* GetPacketPtr_A2C_ENCODING()
	{ return sSEND_PACKET_STRUCTURE.BASE_PK.pMSG_ENCODING; }

	inline static MSG_BASE*	GetPacketPtr_BASE_C2A() { return sRECV_PACKET_STRUCTURE.BASE.pMSG_BASE; }
	inline static MSG_BASE*	GetPacketPtr_BASE_A2C() { return sSEND_PACKET_STRUCTURE.BASE.pMSG_BASE; }
	inline static MSG_BASE_FORWARD*	GetPacketPtr_FORWARD_C2A() { return sRECV_PACKET_STRUCTURE.BASE.pMSG_BASE_FORWARD; }
	inline static MSG_BASE_FORWARD*	GetPacketPtr_FORWARD_A2C() { return sSEND_PACKET_STRUCTURE.BASE.pMSG_BASE_FORWARD; }

	// Agent Server -> 서버팀 관리 서버들
	inline static BYTE*		GetPacketPtr_A2GDWMZ() { return (BYTE*)&(sSEND_PACKET_STRUCTURE.FORWARD); }
	// 서버팀 관리 서버들 -> Agent Server
	inline static BYTE*		GetPacketPtr_GDWMZ2A() { return (BYTE*)&(sRECV_PACKET_STRUCTURE.FORWARD); }
	// Agent Server -> Login F, Auth, Wops
	inline static BYTE*		GetPacketPtr_A2LUO() { return (BYTE*)sSEND_PACKET_STRUCTURE.FORWARD.pMSG_BASE_INTERNAL; }
	// Login F, Auth, Wops -> Agent Server
	inline static BYTE*		GetPacketPtr_LUO2A() { return (BYTE*)sRECV_PACKET_STRUCTURE.FORWARD.pMSG_BASE_INTERNAL; }

	static BOOL		m_bShowPacketStream;
#ifdef _DEBUG
	static const CPShowFNTable*	m_CPShowHandler;
#endif
	static BOOL		GetShowPacketStream() { return m_bShowPacketStream; }
	static VOID		SetShowPacketStream( BOOL bSet )
	{
		m_bShowPacketStream = bSet;
#ifdef _DEBUG
		m_CPShowHandler = GetCPShowHandler();
		m_CPShowHandler->fnInit( C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, CATEGORY_ODD_NUMBER, PROTOCOL_ODD_NUMBER );
#endif
	}
	static VOID		ShowPacketStream( MSG_BASE* pBASE, WORD wSize, CHAR* pPrefix );
};
#pragma warning(pop)
#pragma pack(pop)


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	TYPE DEFINITION
typedef VOID (*PH_fnHandler)( ServerSession* pSession, MSG_BASE* pMsg, WORD wSize );
typedef VOID (*PH_fnCAHandler)( UserSession* pSession, MSG_BASE* pMsg, WORD wSize );

struct sPACKET_HANDLER_INFO
{
	WORD			wCATEGORYnPROTOCOL;
	ePHASE_SHIFT	ePHASE_SHIFT_T;
	PH_fnHandler	pfnHandler;
};

struct FUNC_GENERIC : public util::BASE_FUNC
{
	ePHASE_SHIFT	m_PHASE_SHIFT_T;
	PH_fnHandler	m_fnHandler;
};

class PacketHandler;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
class ProtocolMatrix
{
#pragma warning(push)
#pragma warning(disable : 4201)
public:	struct sNode
		{
			union {
				WORD			wPROTOCOLnCATEGORY;
				struct {
					BYTE		bPROTOCOL;	// 일반과 역순... 연속 스트림 얻기 위함.
					BYTE		bCATEGORY;	// MAKEWORD(pMsg->m_byProtocol, pMsg->m_byCategory)
				};
			};
			ePHASE_SHIFT		ePHASE_SHIFT_T;
			PH_fnCAHandler		m_fnHandler;
		#ifdef __NA000000_081104_REGISTERED_PROTOCOL_NAMES__
			const CHAR*			m_ProtocolName;
		#endif
		};
#pragma warning(pop)
		typedef util::ARRAY_TYPEINFO< sNode, 0, 65536 >		array_t;
public:

public:		ProtocolMatrix();
public:
    const sNode* Find(BYTE category, BYTE protocol) const;
    bool RegisterFowardHandler(PH_fnCAHandler handler) { m_FowardHandler = handler; }
    bool RegisterAllDefault(const ePHASE_SHIFT phase, PH_fnCAHandler handler);
    bool RegisterHandler(const sPACKET_HANDLER_INFO* IN pInfo);
    bool RegisterHandlerRange(const BYTE category, const ePHASE_SHIFT phase, PH_fnCAHandler handler);
#ifdef __NA000000_081104_REGISTERED_PROTOCOL_NAMES__
    void RegisterHandlerName(const BYTE category, const BYTE protocol, const CHAR* const protocol_name);
#endif

private:	array_t::array_type		m_Array;
			ePHASE_SHIFT			m_DefaultPhase;
			PH_fnCAHandler			m_DefaultHandler;
			PH_fnCAHandler			m_FowardHandler;
			friend class PacketHandler;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
typedef Delegator<PacketHandler>					PACKET_HANDLER_DELEGATOR;
typedef Delegator<PacketHandler>*					PACKET_HANDLER_DELEGATOR_P;

typedef Delegator<util::FunctionMap>				FUNCTION_MAP_DELEGATOR;
typedef Delegator<FUNC_GENERIC>						FUNC_GENERIC_DELEGATOR;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
class PacketHandler
{
public:
	PacketHandler();
	~PacketHandler();

	static VOID				RegisterHandler();
	static VOID				ReleaseHandler();

	static PACKET_HANDLER_DELEGATOR	GetHandler()	{ return ms_pInstance; }

    const FUNC_GENERIC* GetServerProtocolBinding(eSERVER_TYPE server_type, BYTE category, BYTE protocol);
    const ProtocolMatrix::sNode* GetClientProtocolBinding(BYTE category, BYTE protocol) const;

	//-------------------------------------------------------------------------------------
	// 패킷 파싱 <서버 간 연결>
	inline BOOL		CanDoProcess( FUNC_GENERIC_DELEGATOR& funcDelegator );

	BOOL			ParsePacket( eSERVER_TYPE serverType, ServerSession* pSession, MSG_BASE* pMsg, WORD wSize )
	{
		return _ParsePacket( serverType, pSession, pMsg, wSize );
	}

	BOOL			ParsePacket( eSERVER_TYPE serverType, ServerSession* pSession, MSG_BASE_INTERNAL* pMsg, WORD wSize )
	{
		return _ParsePacket( serverType, pSession, pMsg, wSize );
	}

	//-------------------------------------------------------------------------------------
	// 패킷 파싱 <서버 간 연결>
	BOOL			ParsePacketCA( eUSER_PARSER_TYPE userType, User* pSession, MSG_BASE* pMsg, WORD wSize )
	{
		return _ParsePacketCA( userType, pSession, pMsg, wSize );
	}

	BOOL			ParsePacketCA( eUSER_PARSER_TYPE userType, User* pSession, MSG_BASE_INTERNAL* pMsg, WORD wSize )
	{
		return _ParsePacketCA( userType, pSession, pMsg, wSize );
	}

private:
	BOOL			_ParsePacket( eSERVER_TYPE serverType, ServerSession* pSession, MSG_BASE* pMsg, WORD wSize );
	BOOL			_ParsePacket( eSERVER_TYPE serverType, ServerSession* pSession, MSG_BASE_INTERNAL* pMsg, WORD wSize );
	BOOL			_ParsePacketCA( eUSER_PARSER_TYPE userType, User* pSession, MSG_BASE* pMsg, WORD wSize );
	BOOL			_ParsePacketCA( eUSER_PARSER_TYPE userType, User* pSession, MSG_BASE_INTERNAL* pMsg, WORD wSize );
	// 또 있다면 현(060815) 설계상 Connection 계열일 것이다.
	//-------------------------------------------------------------------------------------

public:
	class Phase
	{
	private:
		ePHASE_SHIFT	m_Phase;
	public:
		Phase() : m_Phase(PHASE_NOACTION) {}

		BOOL		Shift( ePHASE_SHIFT	phase );
		BOOL		RegisterHandler( sPACKET_HANDLER_INFO* pINFOs, WORD wCounts );
	};

public:
	template<eSERVER_TYPE serverType>
	BOOL			RegisterHandler_( sPACKET_HANDLER_INFO* pINFOs )
	{
		return _RegisterHandler( serverType, pINFOs );
	}

	template<eUSER_PARSER_TYPE userType>
	BOOL			RegisterHandlerCA_( sPACKET_HANDLER_INFO* pINFOs )
	{
		return _RegisterHandlerCA( userType, pINFOs );
	}

    bool RegisterFowardHandlerGroupCG(eUSER_PARSER_TYPE user_parse_type, BYTE category);
#ifdef _NA000000_CW_PROTOCOL_UNIFICATION
    bool RegisterFowardHandlerGroupCW(eUSER_PARSER_TYPE user_parse_type, BYTE category);
#endif
#ifdef __NA000000_081104_REGISTERED_PROTOCOL_NAMES__
	BOOLEAN			RegisterHandlerNames();
	const CHAR*		GetProtocolName( const BYTE category, const BYTE protocol );
#endif

private:
	BOOL			_RegisterHandler( eSERVER_TYPE serverType, sPACKET_HANDLER_INFO* pINFOs );
	BOOL			_RegisterHandlerCA( eUSER_PARSER_TYPE userType, sPACKET_HANDLER_INFO* pINFOs );

private:
	VOID			_Initialize();

	//FUNC_HASH_DELEGATOR		GetHandler( ServerSessionEx* pSession );

private:
	VOID			_LockInit() { m_bLock = FALSE; }
	BOOL			_IsLock() { return m_bLock; }
	VOID			_Locking() { m_bLock = TRUE; }
	VOID			_UnLocking() { m_bLock = FALSE; }

	//
	VOID			_CALockInit() { m_bCALock = FALSE; }
	BOOL			_IsCALock() { return m_bCALock; }
	VOID			_CALocking() { m_bCALock = TRUE; }
	VOID			_CAUnLocking() { m_bCALock = FALSE; }

private:
	//
	static PACKET_HANDLER_DELEGATOR		ms_pInstance;

	BOOL						m_bLock;
	BOOL						m_bCALock;
	// <COMMON HANDLER CONTAINER>
#ifdef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC 
    ProtocolMatrix              m_ProtocolMatrix;
#else
    ProtocolMatrix*				m_pProtocolMatrix;
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
	FUNCTION_MAP_DELEGATOR		m_FuncMaps[MAX_SERVER];
};


