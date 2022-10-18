#pragma once

#ifdef _NA_0_20080402_GUILD_DATASTRUCT_CHANGE

#include "ObjectDefine.h"
#include "Party/GameParty.h"
#include "Party/GamePartyManager.h"

namespace GuildUtil
{
	
	namespace Cast
	{
		struct member_to_all  {};
		struct members_to_around {};
		struct member_in_party {};
		
		template<class CastType, class MsgType> class CastImpl{};
		
		/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		+ 길드원 한명 한명의 주변 섹터에 있는 유저들에게 보내는 패킷
		+ 제약조건 : 1. 자신의 ObjetKeyrkey가 필요 (m_dwPlayerKey)
							 2. 섹터 안에 있는 상대 길드멤버들한테만 보내는 경우 
							   상대방 길드번호 셋팅필요
						     3. 섹터 안에 있는 모든 플레이어한테만 보내는 경우 
							     GuildGuid = INVALID_GUILDGUID로 셋팅 
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		template<class MsgType>
		class CastImpl<members_to_around, MsgType>
		{
		public:
			CastImpl(MsgType* msg) : m_SendMsg(msg) {}
			void SendPacket(GUILDMEMBER_HASH::iterator::value_type& data, GUILDGUID GuildGuid)
			{
				GameGuildMember* pMember = data.second;
				if(pMember)
				{
					Player* pPlayer = PlayerManager::Instance()->FindPlayerByName(pMember->GetCharName());
					if(pPlayer)
					{
						m_SendMsg->m_dwPlayerKey = pPlayer->GetObjectKey();
						pPlayer->SendPacketAround(m_SendMsg, sizeof(*m_SendMsg), TRUE, GuildGuid);
					}
				}
			}
 
		private:
			MsgType* m_SendMsg;
		};

		/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		+ 파티안에 소속된 길드원에게 보내는 패킷
		+ 현재는 member_in_party를 호출하는 패킷이 하나밖에 없지만
		  추후 사용하는 패킷이 더 늘어날 수 있음 
	    + 그 때를 대비해 일반화 작업이 필요
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		template<class MsgType>
		class CastImpl<member_in_party, MsgType>
		{
		public:
			CastImpl(MsgType* msg) : m_SendMsg(msg) {}
			void SendPacket(GUILDMEMBER_HASH::iterator::value_type& data, GUILDGUID GuildGuid)
			{
				GameGuildMember* pMember = data.second;
				if(pMember)
				{
					Player* pPlayer = PlayerManager::Instance()->FindPlayerByName(pMember->GetCharName());
					if(pPlayer && pPlayer->GetPartyState().IsMember())
					{
						GameParty *pParty = GamePartyManager::Instance()->FindParty(pPlayer->GetPartyState().GetPartyKey());
						if(pParty && (pParty->FindGuildMember(GuildGuid)))
						{
							m_SendMsg->m_dwUserKey =pPlayer->GetUserKey(); 
							m_SendMsg->m_wPartykey = pPlayer->GetPartyState().GetPartyKey();
							m_SendMsg->m_bReason = eREASON_LEAVE_PARTY_BY_GUILDRELATION;
							g_pGameServer->SendToServer(AGENT_SERVER, m_SendMsg, sizeof(*m_SendMsg));
						}					
					}
				}
			}

		private:
			MsgType* m_SendMsg;
		};

		/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		+ 길드원 한명 한명에게 보내는 패킷
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		template<class MsgType>
		class CastImpl<member_to_all, MsgType>
		{
		public:
			CastImpl(MsgType* msg) : 
			  m_pSendMsg(msg) ,
			  m_rMsgBrd(MSG_ONEtoN_BROADCASTER::InstanceOfBase())
			{
				m_rMsgBrd = MSG_ONEtoN_BROADCASTER::InstanceOfBase();
				m_rMsgBrd.Init( m_pSendMsg, sizeof(*m_pSendMsg)) ;
				m_pStartPtr = m_rMsgBrd.GetUsersStartPtr();
			}

			~CastImpl()
			{
				g_pGameServer->SendToServer(AGENT_SERVER, &m_rMsgBrd, m_rMsgBrd.GetSendingSize());
			}

			void SendPacket(GUILDMEMBER_HASH::iterator::value_type& data, GUILDGUID GuildGuid)
			{
				GameGuildMember* pMember = data.second;
				//	적용시 적당히 바꿔주시길...__NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO04__
				BOOST_STATIC_ASSERT( FALSE );
				Player* pPlayer = PlayerManager::Instance()->FindPlayerByName(pMember->GetCharName());
				if(pPlayer)
				{
					m_rMsgBrd.AddUser(m_pStartPtr, pPlayer->GetUserGuid()) ;
				}
			}

		private:
			DWORD* m_pStartPtr;
			MsgType* m_pSendMsg;
			MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& m_rMsgBrd;
		};
		
		
		/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		+ TODO : Operator() 인자 개수로 특수화
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		template<class CastType, class MsgType> 
		class GuildCast
		{
		public:
			GuildCast(GUILDMEMBER_HASH* data, MsgType* msg, GUILDGUID guildguid) 
				: m_CastImpl(msg),
				  m_otherGuildGuid(guildguid),
				  m_pMemberList(data) 
			{}

		public:
			void excute()
			{
				for_each(m_pMemberList->begin(), m_pMemberList->end(), 
					boost::lambda::bind(&Impl::SendPacket,  &m_CastImpl,  boost::lambda::_1, m_otherGuildGuid));
			}

		private:
			typedef CastImpl<CastType, MsgType> Impl;
			GUILDMEMBER_HASH* m_pMemberList;
			GUILDGUID m_otherGuildGuid;
			Impl m_CastImpl;
		};
	}
}

#endif //_NA_0_20080402_GUILD_DATASTRUCT_CHANGE