
#pragma once

#include <PacketStruct_SM.h>

#include "PacketHandler.h"
#include "Sessions/ServerSessionManager.h"

class MasterServerSession;

#define HandlerFromMasterServer_DECL( fnName )		\
	static VOID On##fnName( MasterServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HandlerFromMasterServer_IMPL( fnName )		\
	VOID HandlerFromMasterServer::On##fnName( MasterServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )


class HandlerFromMasterServer
{
public:
	static VOID		RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance );

private:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();
private:
	//------------------------------------------------------------------------------
	// <PACKET HANDLERs>
	// Declaration & Implementation
	//
	HandlerFromMasterServer_DECL( SM_CONNECTION_HEARTBEAT );
	//
	//HandlerFromMasterServer_DECL( SM_CONNECTION_CONNECTION_SYN );
	//HandlerFromMasterServer_DECL( SM_CONNECTION_CONNECTION_ACK );
	//HandlerFromMasterServer_DECL( SM_CONNECTION_CONNECTION_NAK );
	//HandlerFromMasterServer_DECL( SM_CONNECTION_DISCONNECT_CMD );
	//HandlerFromMasterServer_DECL( SM_CONNECTION_PHASE_SHIFT_CMD );
	//HandlerFromMasterServer_DECL( SM_CONNECTION_BLOCK_CMD );
	//
	HandlerFromMasterServer_DECL( SM_SERVER_INFO_DESTINATION_KEY_SYN );
	HandlerFromMasterServer_DECL( SM_SERVER_INFO_DESTINATION_KEY_ACK );
	HandlerFromMasterServer_DECL( SM_SERVER_INFO_DESTINATION_KEY_NAK );
	HandlerFromMasterServer_DECL( SM_SERVER_INFO_QUERY_SYN );
	HandlerFromMasterServer_DECL( SM_SERVER_INFO_QUERY_ACK );
	HandlerFromMasterServer_DECL( SM_SERVER_INFO_QUERY_NAK );
	HandlerFromMasterServer_DECL(SM_SERVER_INFO_AUTHENTICATION_SYN);
	HandlerFromMasterServer_DECL(SM_SERVER_INFO_AUTHENTICATION_ACK);
	HandlerFromMasterServer_DECL(SM_SERVER_INFO_AUTHENTICATION_NAK);	
	HandlerFromMasterServer_DECL(SM_SERVER_INFO_CACHEITEM_REPORT_NTF);	
	//HandlerFromMasterServer_DECL( SM_CRYPTOGRAPHY_KEY );
	//HandlerFromMasterServer_DECL( SM_CRYPTOGRAPHY_ACK );
	//HandlerFromMasterServer_DECL( SM_CRYPTOGRAPHY_NAK );
	//HandlerFromMasterServer_DECL( SM_CRYPTOGRAPHY_CMD );
	HandlerFromMasterServer_DECL( SM_CRYPTOGRAPHY_PACKET_ENCODED );
	HandlerFromMasterServer_DECL( SM_CRYPTOGRAPHY_PACKET_ENCODING );
	//HandlerFromMasterServer_DECL( SM_CRYPTOGRAPHY_SNK_KEY );
	//HandlerFromMasterServer_DECL( SM_CRYPTOGRAPHY_SNK_CMD );
	HandlerFromMasterServer_DECL( SM_CRYPTOGRAPHY_SNK_ACK );
	HandlerFromMasterServer_DECL( SM_CRYPTOGRAPHY_SNK_NAK );
	//
	HandlerFromMasterServer_DECL( SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN );
	//HandlerFromMasterServer_DECL( SM_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK );
	//HandlerFromMasterServer_DECL( SM_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK );
	//HandlerFromMasterServer_DECL( SM_DATA_TRANSFER_SENDDATA_CMD );
	HandlerFromMasterServer_DECL( SM_DATA_TRANSFER_SENDDATA_ACK );
	HandlerFromMasterServer_DECL( SM_DATA_TRANSFER_SENDDATA_NAK );

	//------------------------------------------------------------------------------
	// <METHODs for HANDLER>
	//

	//------------------------------------------------------------------------------
	// <WorkLoad Interfaces>
	//
private:
	//class PROCESS_SM_DATA_TRANSFER_SENDDATA_CMD : public IWorkNode
	//{
	//private:
	//	static const DWORD	c_dwMaxTokenSize =
	//		MSG_SM_DATA_TRANSFER_SENDDATA_CMD::SEND_INFO_TOKEN_INFO::MAX_SENDDATA_CMD_SEND_INFO_TOKEN_SIZE;
	//public:
	//	PROCESS_SM_DATA_TRANSFER_SENDDATA_CMD();
	//	~PROCESS_SM_DATA_TRANSFER_SENDDATA_CMD();

	//public:
	//	ServerSession*		GetServerSession() { return m_pServerSession; }
	//	VOID				SetServerSession( ServerSession* pSession );

	//	// pSTREAM은 내부에서 해제하지 않는다. 이것은 공유하는 버퍼일 가능성이 있다. 이후 Delegator처리로?
	//	BOOL	SetInformation( const BYTE* pSTREAM, const DWORD dwBufSize, const DWORD dwCRC, const WORD wScriptNo );
	//public:
	//	// <INTERFACE><IMPLEMENTATION>
	//	virtual BOOL	DoInit();
	//	virtual BOOL	DoProcess();
	//	virtual BOOL	IsComplete();
	//	virtual BOOL	DoRelease();
	//private:
	//	ServerSession*				m_pServerSession;
	//	GLOBAL_SERVER_KEY			m_GLOBAL_SERVER_KEY;	// ServerSession* 동기화 확인 목적
	//	BOOL						m_bHeadSended;
	//	DWORD						m_dwCompleteCount;
	//	DWORD						m_dwProcessed;
	//	BYTE*						m_pSTREAM;
	//	MSG_SM_DATA_TRANSFER_SENDDATA_CMD::SEND_INFO_TOKEN_INFO_HEADER*
	//								m_pTOKEN_INFO_HEADER;
	//	MSG_SM_DATA_TRANSFER_SENDDATA_CMD::SEND_INFO_FILE_INFO
	//								m_FILE_INFO;
	//};
};


