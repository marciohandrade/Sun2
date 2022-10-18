#pragma once

#include "SSQInfoStruct.h"

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	<WarMsgRouter>
//	-	Message Slot Controller for Routing
template< typename _DERIVED_T, typename _STORAGE_T >
struct WarMsgRouter
{
protected:
	typedef _DERIVED_T								sub_type;
	typedef _STORAGE_T								storage_type;
	
	typedef WarMsgRouter<sub_type, storage_type>	this_type;
    typedef sun_type_info<storage_type>             storage_info;

	typedef STLX_MAP< DWORD, storage_type* >		TARGET_LIST;

	//	<need implementation interfaces>
	//	BOOLEAN				Routing( const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header );

protected:	TARGET_LIST								m_Container;
public:
	inline	storage_type*	FindSlot( const DWORD id ) const
			{
				TARGET_LIST::const_iterator it = m_Container.find( id );
				return it != m_Container.end()
					?	it->second
					:	0
					;
			}

	inline	storage_type*	AlignNewSlot( const DWORD id )
			{
				//storage_type* const new_slot = storage_info::Allocate();
				//m_Container.insert( make_pair( id, new_slot ) );
				// <- ���� slot���� �Ǵ� �ʿ�.
				//return new_slot;
				storage_type*& rSlot = m_Container[id];
                if (rSlot == NULL) {
                    rSlot = TAllocNew(storage_info::value_type);
                }
				return rSlot;
			}

	inline	VOID			ReleaseSlot( const DWORD id )
			{
				TARGET_LIST::iterator it = m_Container.find( id );
				if( it != m_Container.end() )
				{
                    storage_type* storage = it->second;
                    TAllocDelete(storage_info::value_type, storage);
					m_Container.erase( it );
				}
			}

	//	<��ϸ� �߰�>
	inline	VOID			AddSlot( const DWORD id, storage_type* const pStorage )
			{
				storage_type*& rSlot = m_Container[id];
				// ASSERT( rSlot );�� ��찡 ������ ��������.
				if( !rSlot )
					rSlot = pStorage;
			}

	//	<��Ͽ����� ����>
	inline	VOID			RemoveSlot( const DWORD id )
			{
				m_Container.erase( id );
			}


public:		BOOL			OnRecv( const WAR_MSG_HDR* IN pStream )
			{
				WAR_MSG_HDR header = *pStream;
				WAR_TAG_P pIt = (WAR_TAG_P)(pStream+1);
				return static_cast<sub_type*>( this )->Routing( pIt, header );
			}

			BOOL			OnRecvSpecial( const WAR_MSG_HDR* IN pStream )
			{
				WAR_MSG_HDR header = *pStream;
				WAR_TAG_P pIt = (WAR_TAG_P)(pStream+1);
				return static_cast<sub_type*>( this )->RoutingSpecial( pIt, header );
			}
};


//	Client Packet Serializer
struct WarPacketAssembler
{
	static	WORD			Msg_Size;
	static	BYTE			Msg_Stream[4096];

	template< typename _MSG, typename _MSGSUB >
	static VOID				MakePacket( const _MSGSUB& rMsg )
	{
		typedef _MSG		message;
		typedef _MSGSUB		sub_message;

		message* const pMsgRoot = new (Msg_Stream) message;
		pMsgRoot->m_Header->size = 0;
		rMsg.SerializeTo( pMsgRoot->m_Header );

		Msg_Size = pMsgRoot->GetSize();
	}
};

