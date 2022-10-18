#include "StdAfx.h"
#include ".\gameserversession.h"
#include "PacketHandler.h"

__IMPL_CUSTOMPOOL_PTR( GameServerSession )
GameServerSession::GameServerSession(void)
{
}

GameServerSession::~GameServerSession(void)
{
}
VOID GameServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_GAME, this, (MSG_BASE *)pMsg, wSize );
}

//
//class PacketHandler
//{
//	// HandlerSet 을 구성
//	HandlerSet * operator() ( enum eTYPE )
//	{
//		return m_hash[eTYPE];
//	}
//
//
//	bool loadfile();
//
//	bool register( eTYPE, cate, pro, function_ptr );
//
//	m_hash[eTYPE];
//};
//
//class HandlerSet
//{
//	void Parse( ServerSession *, pMsg, wSize );
//	std::hash_map<(cate,protocol), 함수> m_set;
//};
//
//void HandlerSet::Parse( ServerSession * pSession, pMsg, wSize )
//{
//	function_ptr = m_set.find(cate,pro);
//	function_ptr( pSession, pMsg, wSize );
//
//	if( 생명주기가다하면 )
//	{
//		m_set.erase( it );
//	}
//
//}
//
//class HandlerGroup
//{
//	HandlerGroup( func ) : m_cur_function_ptr(func)
//	void operator( ServerSession *, pMsg, wSize )
//	{
//		m_cur_function_ptr( session, PMSG, wSize );
//		if( 주기 ) return false;
//		if( bNext? )
//		{
//			m_cur_function_ptr = m_cur_function_ptr->next;
//			if( NULL == m_cur_function_ptr ) 
//		}
//	}
//	m_cur_function_ptr;
//	std::list<(cate,protocol), 함수> m_list;
//};
