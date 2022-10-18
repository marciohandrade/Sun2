#pragma once

#include <PacketStruct_AG.h>
#include "GameServerEx.h"
#include <GameOptionParser.h>

extern GameServerEx*	g_pGameServer;

namespace Generic
{
	//	<Description>
	//		불필요한 매크로 제거 목적으로 일괄 변경 처리
	//	<Arguments>
	//		pPlayer : not null -> pPlayer 기준 처리
	//		NULL -> 일반 처리
	//	<Sample>
	//		100레벨 퀘스트 클리어 여부 : T1, F1
	//		CheckMaxLevel( NULL, 100 ) -> FALSE
	//		CheckMaxLevel( NULL, 110 ) -> TRUE
	//		CheckMaxLevel( (Player*), 100 ) -> T1:FALSE, F1:TRUE
	//		CheckMaxLevel( (Player*), 110 ) -> T1:TRUE, F1:TRUE(X)
	BOOL		CheckMaxLevel( Player* IN pPlayer, const INT level );

	//	<Description>
	//		불필요한 매크로 제거 목적으로 일괄 변경 처리
	//	<Arguments>
	//		pPlayer : not null -> pPlayer 기준 처리
	//		NULL -> 일반 처리
	//		GetMaxLevel( NULL ) -> 110 (현재 정의된 최고 레벨)
	//		GetMaxLevel( (Player*) ) -> T1:110, F1:100
	LEVELTYPE	GetMaxLevel( Player* IN pPlayer );
#if SUN_CODE_BACKUP
    // NOTE: f110601.1L, remove unused section
	//	TEMPLATE FUNCTIONs
	class SendPacketBroadcaster
	{
	public:		typedef MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN		BROADCASTER;
	private:	BROADCASTER*	m_pMSG_BRD;
				DWORD*			m_pUserStart;
				DWORD			m_ExceptKey;

	public:		SendPacketBroadcaster() : m_pMSG_BRD(0), m_pUserStart(0), m_ExceptKey(0) {}
				VOID				Init( MSG_BASE_FORWARD* pMsg , WORD wSize, DWORD dwExceptObjectKey = 0 )
				{
					m_pMSG_BRD = &MSG_ONEtoN_BROADCASTER::InstanceOfBase();
					GetBroadcastMsg().Init( pMsg, wSize );
					m_pUserStart = GetBroadcastMsg().GetUsersStartPtr();
					m_ExceptKey = dwExceptObjectKey;
				}

				VOID				InitByArray( BYTE** ppMsg, WORD* pwBrdPacketSize, DWORD dwMsgArrayCnt, DWORD dwExceptObjectKey = 0 )
				{
					m_pMSG_BRD = &MSG_ONEtoN_BROADCASTER::InstanceOfBase();
					GetBroadcastMsg().InitByArray( ppMsg, pwBrdPacketSize, dwMsgArrayCnt );
					m_pUserStart = GetBroadcastMsg().GetUsersStartPtr();
					m_ExceptKey = dwExceptObjectKey;
				}

				inline BROADCASTER&	GetBroadcastMsg() { return *m_pMSG_BRD; }

				template<typename _PLAYER_OBJ_PAIR>
				VOID				ProcAddUser( _PLAYER_OBJ_PAIR rPair )
				{
					Player* pPlayer = rPair.second;
					if( !m_ExceptKey || (pPlayer->GetObjectKey() != m_ExceptKey) )
					{
						m_pMSG_BRD->AddUser( m_pUserStart, pPlayer->GetObjectKey() );
					}
				}

				//	특수 타입, 직접적으로 많이 사용하는 경우를 감안한 지원
				inline VOID			ProcAddUserPLAYER_HASH( PLAYER_HASH_PAIR rPair )
				{	ProcAddUser( rPair );	}

				VOID				SendPacketToAll()
				{
					g_pGameServer->SendToServer( AGENT_SERVER, m_pMSG_BRD, GetBroadcastMsg().GetSendingSize() );
				}
	};
#endif //SUN_CODE_BACKUP
};

