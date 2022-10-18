#include "stdafx.h"
#include "PacketRelay.h"

#include <eXtreme/threading/Guard.h>

PacketRelay::PacketRelay()
{
}

PacketRelay::~PacketRelay()
{
}

void PacketRelay::Initialize()
{
	StreamInfo*	info;
	int			i;

	for( i = 0 ; i < 1000 ; i++ )
	{
		info = new StreamInfo( false );

		m_pool.push( info );
	}

	for( i = 0 ; i < 100 ; i++ )
	{
		info = new StreamInfo( true );

		m_poolOver.push( info );
	}
}

void PacketRelay::Destroy()
{
	while( m_pool.empty() == false )
	{
		delete m_pool.front();
		m_pool.pop();
	}

	while( m_poolOver.empty() == false )
	{
		delete m_poolOver.front();
		m_poolOver.pop();
	}
}

void PacketRelay::Clear()
{
	eXtreme::Guard	< eXtreme::Mutex >	guardPool( m_mutexPool );
	eXtreme::Guard	< eXtreme::Mutex >	guardQueue( m_mutexQueue );
	StreamInfo*		info;

	while( m_queueInfo.size() > 0 )
	{
		info = m_queueInfo.front();
		m_queueInfo.pop();

		info->stream->Clear();
		if( info->over == true )
			m_poolOver.push( info );
		else
			m_pool.push( info );
	}
}

bool PacketRelay::PostPacket( void* buffer, int size )
{
	StreamInfo*		info;

	{
		eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutexPool );
		int				i;

		if( size > STREAM_DEFAULT_SIZE )
		{
			//  stream pool이 부족하면 15% 증가시킨다.
			if( m_poolOver.size() == 0 )
			{
				for( i = 0 ; i < 15 ; i++ )
				{
					info = new StreamInfo( true );
					m_poolOver.push( info );
				}
			}

			info = m_poolOver.front();
			m_poolOver.pop();
		}
		else
		{
			//  stream pool이 부족하면 1.5% 증가시킨다.
			if( m_pool.size() == 0 )
			{
				for( i = 0 ; i < 1500 ; i++ )
				{
					info = new StreamInfo( false );
					m_pool.push( info );
				}
			}

			info = m_pool.front();
			m_pool.pop();
		}

		info->stream->Clear();
		if( info->stream->Write( buffer, size ) == false )
		{
			OutputDebug( "[PacketRelay::PostPacket] stream write error(size=%d)\n", size );
			assert( false );

			return false;
		}
	}

	{
		eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutexQueue );

        m_queueInfo.push( info );
	}

	return true;
}

eXtreme::Stream* PacketRelay::PopPosted()
{
	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutexQueue );
	StreamInfo*		info;

	if( m_queueInfo.size() == 0 )
		return 0;

	info = m_queueInfo.front();
	m_queueInfo.pop();


	if( info->over == true )
		m_poolOver.push( info );
	else
		m_pool.push( info );

	return info->stream;
}