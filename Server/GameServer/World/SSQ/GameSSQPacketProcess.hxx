#pragma once


class GameSSQUpdateCtrl;
class User;

namespace nsSSQ_Util {};
namespace GameSSQControlInfo
{
	struct SSQ_UPDATER_NODE;
};

using namespace nsSSQ;
using namespace nsSSQ_Util;
using namespace GameSSQControlInfo;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//

template< typename _DERIVED, typename _PACKET_T >
class BROADCASTER_MSG_CG : public nsSSQ_Util::xxxSender< _DERIVED, _PACKET_T >
{
public:
	typedef BROADCASTER_MSG_CG<_DERIVED, _PACKET_T>		this_type;

	inline packet_structure* const	GetPacket() { return m_pPacket; }

public:		VOID		ProcessAllUser() const
			{
				UserManager* const pUSRMGR = UserManager::Instance();
				const USER_HASH& rList = pUSRMGR->RefUserList();
				const WORD size = m_pPacket->GetSize();
				FOREACH_CONTAINER( const USER_HASH::value_type& rNode, rList, USER_HASH )
				{
					User* const pUser = rNode.second;
					pUser->SendPacket( m_pPacket, size );
				}
				//pUSRMGR->Foreach( &this_type::SendPacket, this );
				//printf( "%p\t%u\n"
				//	, const_cast<packet_structure*>( m_pPacket )
				//	, m_PacketSize
				//	);
			}
};


class BROADCASTER_MSG_CG_STATUS_SSQ_INFO_BRD
	: public BROADCASTER_MSG_CG< BROADCASTER_MSG_CG_STATUS_SSQ_INFO_BRD, MSG_CG_STATUS_SSQ_INFO_BRD >
{
};

template< typename _DERIVED, typename _PACKET_T >
class UNICASTER_BETWEEN_SERVER : public nsSSQ_Util::xxxSender< _DERIVED, _PACKET_T >
{
public:
	typedef UNICASTER_BETWEEN_SERVER<_DERIVED, _PACKET_T>		this_type;

	inline packet_structure* const	GetPacket() { return m_pPacket; }

public:		BOOL		SendPacket( ServerSession* const pServerSession, const KEYTYPE KeyOfSSQ ) const
			{
				m_pPacket->m_dwKey = KeyOfSSQ;
				pServerSession->SendPacket( m_pPacket, m_pPacket->GetSize() );
				//printf( "%p\t%u\n"
				//	, const_cast<packet_structure*>( m_pPacket )
				//	, m_PacketSize
				//	);
			}
};

class BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD
	: public UNICASTER_BETWEEN_SERVER< BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD, MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD >
{
};

//template< typename _DRIVED, typename _PACKET >
//class BROADCASTER_IN_GAMESERVER : public Broadcaster< _DRIVED, _PACKET >
//{
//	typedef BROADCASTER_IN_GAMESERVER<_DRIVED, _PACKET>		this_type;
//public:
//	inline MSG_BASE*				GetPacketMsgHeader() { return m_pPacket; }
//	inline WORD						GetPacketMsgSize() { return m_PacketSize = m_pPacket->GetSize();; }
//
//	SSQ_MSG_SCT_HDR*				m_pMsgHeader;
//	WAR_TAG_P						m_pOffset;
//public:
//	inline VOID						Reset()
//	{
//		new (this) this_type();
//		m_pOffset = MakeInit_SSQPacketStream( m_pMsgHeader );
//	}
//
//	inline SSQ_MSG_SCT_HDR*&		RefMsgHeader() { return m_pMsgHeader; }
//	inline WAR_TAG_P&				RefMsgOffset() { return m_pOffset; }
//};
//
class BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD
	: public UNICASTER_BETWEEN_SERVER< BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD, MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD >
{
};


class BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_INFO_BRD
	: public UNICASTER_BETWEEN_SERVER< BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_INFO_BRD, MSG_CG_ZONE_EX_SSQ_ROOM_INFO_BRD>
{
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<DEFINITION #1>
//
struct SSQUserInfoHanlder
{
	DWORD						SeqIndex;	// UserKey <-> Idx
	DWORD						UserKey;
	SSQInfo*					pSSQInfo;
	User*						pUser;
};

//	SSQInfo들을 처리하는 루틴... Update, Serializable Data Container
struct SSQInfoHanlder
{
	//typedef STLX_VECTOR
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<GameWarControlManager>
//
class SSQMsgRouterInGame : public WarMsgRouter<SSQMsgRouterInGame, SSQUserInfoHanlder>
{
	friend struct WarMsgRouter<SSQMsgRouterInGame, SSQUserInfoHanlder>;
	struct GAME_SSQ_MSG_ROUTER_IMPL_Info : sun_type_info<SSQMsgRouterInGame> { };

public:
	typedef GAME_SSQ_MSG_ROUTER_IMPL_Info			router_info;

	BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD&		m_msgCMD_AG;
	BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD&	m_msgCMD_CG;
	BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_INFO_BRD&	m_msgBRD_CG;

	GameSSQUpdateCtrl*								m_pUpdateCtrl;

public:		SSQMsgRouterInGame()
				: m_msgCMD_AG(*new BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD)
				, m_msgCMD_CG(*new BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD)
				, m_msgBRD_CG(*new BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_INFO_BRD)
				, m_pUpdateCtrl( 0 )
			{
				m_msgCMD_AG.Reset();
				m_msgCMD_CG.Reset();
				m_msgBRD_CG.Reset();
			}
			~SSQMsgRouterInGame()
			{
				delete &m_msgCMD_AG;
				delete &m_msgCMD_CG;
				delete &m_msgBRD_CG;
				m_pUpdateCtrl = NULL;
			}

public:
	inline	VOID					SetUpdateCtrl( GameSSQUpdateCtrl* const pCtrl ) { m_pUpdateCtrl = pCtrl; }
	inline	GameSSQUpdateCtrl*		GetUpdateCtrl() { return m_pUpdateCtrl; }
public:
	inline	BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD&		GetCMD_AG() { return m_msgCMD_AG; }
	inline	BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD&	GetCMD_CG() { return m_msgCMD_CG; }
	inline	BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_INFO_BRD&	GetBRD_CG() { return m_msgBRD_CG; }

private:	VOID			SlotUpdate();
public:		VOID			SlotClear( User* const pUser );
            //__NA000000_090716_HIGH_LOAD_BALANCED_NETWORK_TUNEUP__
public:		VOID			Update( const SSQInfo::INDEX_T IDofSSQ );
			BOOLEAN			Routing( const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header );

//private:	VOID				NodeUpdate( const sNODE& rNode )
//			{
//				//ISSQHandler* const pISSQHandler = sNODE::GetStorage( rNode );
//				//if( FALSE == pISSQHandler->Update() )
//				//{
//				//	ReleaseNode( rNode );
//				//}
//			}

#undef SSQ_MODE_NODE
#define SSQ_MODE_NODE( protocol )									\
	BOOL	On##protocol( const nsSSQ::SSQ_MSG_##protocol& rMsg ) {	\
		printf( "OnRecv " #protocol "\n" );	\
		BYTE* ptr = (BYTE*)&rMsg;			\
		BYTE* ptrend = ptr+sizeof(rMsg);	\
		for( ; ptr != ptrend ; ++ptr )		\
			printf( "[%03u]", *ptr );		\
		puts("");							\
		return TRUE;						\
	}

public:
	////━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//// Server<->Server (A<->G) SSQ_MESSAGE_HANDLERs
	//SSQ_MSG_HANDLER_IMPL(	SSQ_S2S_GAMBLE_START		)
	//{
	//	WAVERIX_DEBUGGING_OUTPUTnV( 0 );
	//	GameInstanceDungeon* const pDungeonSSQ = m_pUpdateCtrl->GetSSQField();
	//	// 당첨자 계산 및 바로 Client들로 공지 - 에니메이션 연출 후 결과 보여주는 형태로 클라 구성

	//	// send to players - 이건 GameServer에서 보내는 형태로 하자. CharName등등 정보 담아서
	//	{
	//		SSQ_MSG_SCT_HDR*& rHdrPtr = m_msgCMD_AG.RefMsgHeader();
	//		WAR_TAG_P& rpOffset = m_msgCMD_AG.RefMsgOffset();

	//		SSQ_MSG_GNM_GAMBLE_DRAWING msg0;
	//		rpOffset = SSQ_Serialize( msg0,		rpOffset, rHdrPtr->hdr );
	//	};

	//	return TRUE;
	//};

public:
    //
    __DISABLE_COPY(SSQMsgRouterInGame);
};

namespace GameSSQControlInfo
{
	typedef SSQMsgRouterInGame::router_info	router_info;
};

