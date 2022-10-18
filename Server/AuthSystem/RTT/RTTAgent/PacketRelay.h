#pragma once

#include <eXtreme/util/Stream.h>
#include <eXtreme/threading/Mutex.h>

#include <queue>

class PacketRelay
{
	Singleton( PacketRelay );

private:
	typedef	struct StreamInfo_tag
	{
		StreamInfo_tag( bool isOver )
			: over( isOver )
		{
			if( over == true )
				stream = new eXtreme::Stream( 8192 );
			else
				stream = new eXtreme::Stream();
		}

		~StreamInfo_tag()
		{
			delete stream;
		}

		bool				over;
		eXtreme::Stream*	stream;
	} StreamInfo;

public:
			void					Initialize();
			void					Destroy();

			void					Clear();

			bool					PostPacket( void* buffer, int size );
			eXtreme::Stream*		PopPosted();


private:
			std::queue		< StreamInfo* >		m_pool;
			std::queue		< StreamInfo* >		m_poolOver;
			std::queue		< StreamInfo* >		m_queueInfo;

			eXtreme::Mutex						m_mutexPool;
			eXtreme::Mutex						m_mutexQueue;
};