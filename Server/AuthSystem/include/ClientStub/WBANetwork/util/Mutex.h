#ifndef  __WBANetwork_Mutex_H
#define  __WBANetwork_Mutex_H

namespace WBANetwork
{
	class Mutex
	{
	public:
		Mutex();
		~Mutex();

		bool	TryLock();
		void	Lock();
		void	Unlock();

	private:
		CRITICAL_SECTION		m_cs;
	};
}

#endif