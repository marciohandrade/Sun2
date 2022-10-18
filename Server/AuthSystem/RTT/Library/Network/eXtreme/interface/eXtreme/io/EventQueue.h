#ifndef  __eXtreme_EventQueue_H
#define  __eXtreme_EventQueue_H

#include <eXtreme/net/Socket.h>

namespace eXtreme
{
	class EventHandler
	{
	public:
		virtual	~EventHandler()		{}

		virtual	void			HandleEvent( AsyncResult* result )		= 0;
		virtual	Socket*			GetHandle()								= 0;
		virtual	char*			GetBuffer()								= 0;
		virtual	int				GetBufferSize()							= 0;

///////////////////////////////////////////////////////////////////////////////////////
//				void			PrintTCPStatus();
///////////////////////////////////////////////////////////////////////////////////////


	protected:
				EventHandler()		{}
	};

	class EventQueue
	{
	public:
				EventQueue();
				~EventQueue();

				bool			Create( int threadCount );
				void			Close();

				bool			AddEvent( EventHandler*	handler );
				void			RemoveEvent( EventHandler* handler );

				EventHandler*	GetCompletionEvent( AsyncResult** result, int ms );
				void			PostCompletion( EventHandler* handler, AsyncResult* result );

	private:
				HANDLE			m_handleQueue;
	};
}

#endif