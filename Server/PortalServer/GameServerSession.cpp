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
//	// HandlerSet �� ����
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
//	std::hash_map<(cate,protocol), �Լ�> m_set;
//};
//
//void HandlerSet::Parse( ServerSession * pSession, pMsg, wSize )
//{
//	function_ptr = m_set.find(cate,pro);
//	function_ptr( pSession, pMsg, wSize );
//
//	if( �����ֱⰡ���ϸ� )
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
//		if( �ֱ� ) return false;
//		if( bNext? )
//		{
//			m_cur_function_ptr = m_cur_function_ptr->next;
//			if( NULL == m_cur_function_ptr ) 
//		}
//	}
//	m_cur_function_ptr;
//	std::list<(cate,protocol), �Լ�> m_list;
//};
