#ifndef  __eXtreme_Mutex_H
#define  __eXtreme_Mutex_H

#include <eXtreme/threading/Guard.h>

namespace eXtreme
{
	class Mutex
	{
	public:
				Mutex();
				~Mutex();


				bool		TryLock();
				void		Lock();
				void		Unlock();

	private:
				CRITICAL_SECTION		m_cs;
	};
}

#endif