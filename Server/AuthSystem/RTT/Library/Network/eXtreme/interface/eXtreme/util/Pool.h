#ifndef  __eXtreme_Pool_H
#define  __eXtreme_Pool_H

#include <queue>

namespace  eXtreme
{
	template < class _alloc >
	class Pool
	{
	public:
				Pool()
				{
				}
				~Pool()
				{
				}

				bool			Create( int size )
				{
					if( Size() > 0 )
					{
						return false;
					}

					for(int i = 0 ; i < size ; i++ )
					{
						_alloc* _item = _new_dbg_ _alloc;
						m_queue.push( _item );
					}

					return true;
				}

				void			Destroy()
				{
					while( m_queue.empty() == false )
					{
						_alloc*	_item = m_queue.front();
						m_queue.pop();

						delete _item;
					}
				}

				void			IncSize( int inc )
				{
					for(int i = 0 ; i < inc ; i++ )
						m_queue.push( ( _alloc* )( _new_dbg_ _alloc ) );
				}

				int				Size()
				{
					return ( int )m_queue.size();
				}

				_alloc*			Pop()
				{
					if( Size() == 0 )
						return 0;

					_alloc* item = m_queue.front();
					m_queue.pop();

					return item;
				}

				void			Push( _alloc* t )
				{
					m_queue.push( t );
				}

	private:
				std::queue		< _alloc* >		m_queue;
	};
}

#endif