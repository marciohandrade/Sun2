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
		+ ���� �Ѹ� �Ѹ��� �ֺ� ���Ϳ� �ִ� �����鿡�� ������ ��Ŷ
		+ �������� : 1. �ڽ��� ObjetKeyrkey�� �ʿ� (m_dwPlayerKey)
							 2. ���� �ȿ� �ִ� ��� ����������׸� ������ ��� 
							   ���� ����ȣ �����ʿ�
						     3. ���� �ȿ� �ִ� ��� �÷��̾����׸� ������ ��� 
							     GuildGuid = INVALID_GUILDGUID�� ���� 
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
		+ ��Ƽ�ȿ� �Ҽӵ� �������� ������ ��Ŷ
		+ ����� member_in_party�� ȣ���ϴ� ��Ŷ�� �ϳ��ۿ� ������
		  ���� ����ϴ� ��Ŷ�� �� �þ �� ���� 
	    + �� ���� ����� �Ϲ�ȭ �۾��� �ʿ�
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
		+ ���� �Ѹ� �Ѹ��� ������ ��Ŷ
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
				//	����� ������ �ٲ��ֽñ�...__NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO04__
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
		+ TODO : Operator() ���� ������ Ư��ȭ
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