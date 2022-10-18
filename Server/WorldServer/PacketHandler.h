#pragma once

#include <Singleton.h>
#include <FunctionMap.h>
#include <PacketStruct_CW.h>

//struct MSG_BASE;
class User;
class GameDBProxySession;
class AccountDBProxySession;
class GameServerSession;
class BattleServerSession;
class FieldServerSession;
class AgentServerSession;
class MasterServerSession;
class GuildServerSession;

#pragma warning(push)
#pragma warning(disable : 4201)
#pragma pack(push,1)
// World 기준용
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
				MSG_CW_CRYPTOGRAPHY_PACKET_ENCODING	pMSG_ENCODING[1];
				MSG_CW_CRYPTOGRAPHY_PACKET_ENCODED	pMSG_ENCODED[1];
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
	inline static MSG_CW_CRYPTOGRAPHY_PACKET_ENCODED* GetPacketPtr_C2A_ENCODED()
	{ return sRECV_PACKET_STRUCTURE.BASE_PK.pMSG_ENCODED; }
	// User(crypt)
	inline static MSG_CW_CRYPTOGRAPHY_PACKET_ENCODING* GetPacketPtr_A2C_ENCODING()
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
};
#pragma pack(pop)
#pragma warning(pop)

class PacketHandler : public util::Singleton<PacketHandler>
{
	typedef VOID (*fnHandler_CW)( User *pUser, MSG_BASE *pMsg, WORD wSize );
	typedef VOID (*fnHandler_DW)( GameDBProxySession *pGameDBProxySession, MSG_BASE *pMsg, WORD wSize );
	typedef VOID (*fnHandler_TW)( AccountDBProxySession *pAccountDBProxySession, MSG_BASE *pMsg, WORD wSize );
	typedef VOID (*fnHandler_GW)( GameServerSession *pGameServerSession, MSG_BASE *pMsg, WORD wSize );
	typedef VOID (*fnHandler_BW)( BattleServerSession *pBattleServerSession, MSG_BASE *pMsg, WORD wSize );
	typedef VOID (*fnHandler_FW)( FieldServerSession *pFieldServerSession, MSG_BASE *pMsg, WORD wSize );
	typedef VOID (*fnHandler_AW)( AgentServerSession *pAgentServerSession, MSG_BASE *pMsg, WORD wSize );
	typedef VOID (*fnHandler_MW)( MasterServerSession *pMasterServerSession, MSG_BASE *pMsg, WORD wSize );
	typedef VOID (*fnHandler_WZ)( GuildServerSession *pGuildServerSession, MSG_BASE *pMsg, WORD wSize );

public:
	PacketHandler();
	~PacketHandler();

	BOOL			RegisterPackets();

	BOOL			ParsePacket_CW( User *pUser, MSG_BASE *pMsg, WORD wSize );
	BOOL			ParsePacket_DW( GameDBProxySession *pGameDBProxySession, MSG_BASE *pMsg, WORD wSize );
	BOOL			ParsePacket_TW( AccountDBProxySession *pAccountDBProxySession, MSG_BASE *pMsg, WORD wSize );
	BOOL			ParsePacket_GW( GameServerSession *pGameServerSession, MSG_BASE *pMsg, WORD wSize );
	BOOL			ParsePacket_BW( BattleServerSession *pBattleServerSession, MSG_BASE *pMsg, WORD wSize );
	BOOL			ParsePacket_FW( FieldServerSession *pFieldServerSession, MSG_BASE *pMsg, WORD wSize );
	BOOL			ParsePacket_AW( AgentServerSession *pAgentServerSession, MSG_BASE *pMsg, WORD wSize );
	BOOL			ParsePacket_MW( MasterServerSession *pMasterServerSession, MSG_BASE *pMsg, WORD wSize );
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
    BOOL ParsePacket_WZ(GameDBProxySession* server_session, MSG_BASE* msg, WORD msg_size);
#else
	BOOL			ParsePacket_WZ( GuildServerSession *pGuildServerSession, MSG_BASE *pMsg, WORD wSize );
#endif
private:
	BOOL			RegClientPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_CW fnHandler );
	BOOL			RegGameDBProxyPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_DW fnHandler );
	BOOL			RegAccountDBProxyPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_TW fnHandler );
	BOOL			RegGameServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_GW fnHandler );
	BOOL			RegBattleServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_BW fnHandler );
	BOOL			RegFieldServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_FW fnHandler );
	BOOL			RegAgentServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_AW fnHandler );
	BOOL			RegMasterServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_MW fnHandler );
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
    bool RegGuildServerPacket(BYTE category, BYTE protocol, PacketHandler::fnHandler_DW fnHandler);
#else
	BOOL			RegGuildServerPacket( BYTE category, BYTE protocol, PacketHandler::fnHandler_WZ fnHandler );
#endif
	struct FUNC_CW : public util::BASE_FUNC
	{
		fnHandler_CW	m_fnHandler;
	};
	struct FUNC_DW : public util::BASE_FUNC
	{
		fnHandler_DW	m_fnHandler;
	};
	struct FUNC_TW : public util::BASE_FUNC
	{
		fnHandler_TW	m_fnHandler;
	};
	struct FUNC_GW : public util::BASE_FUNC
	{
		fnHandler_GW	m_fnHandler;
	};
	struct FUNC_BW : public util::BASE_FUNC
	{
		fnHandler_BW	m_fnHandler;
	};
	struct FUNC_FW : public util::BASE_FUNC
	{
		fnHandler_FW	m_fnHandler;
	};
	struct FUNC_AW : public util::BASE_FUNC
	{
		fnHandler_AW	m_fnHandler;
	};
	struct FUNC_MW : public util::BASE_FUNC
	{
		fnHandler_MW	m_fnHandler;
	};
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	struct FUNC_WZ : public util::BASE_FUNC
	{
		fnHandler_WZ	m_fnHandler;
	};
#endif

	util::FunctionMap		*m_pFuncMap_CW;
	util::FunctionMap		*m_pFuncMap_DW;
	util::FunctionMap		*m_pFuncMap_TW;
	util::FunctionMap		*m_pFuncMap_GW;
	util::FunctionMap		*m_pFuncMap_BW;
	util::FunctionMap		*m_pFuncMap_FW;
	util::FunctionMap		*m_pFuncMap_AW;
	util::FunctionMap		*m_pFuncMap_MW;
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	util::FunctionMap		*m_pFuncMap_WZ;
#endif
};
