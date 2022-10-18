#ifndef  __WBANetwork_Guard_H
#define  __WBANetwork_Guard_H

namespace WBANetwork
{
	template <class T>
	class Guard
	{
	public:
		Guard(T& lock) : lock_(&lock)
		{
			lock_->Lock();
		}

		~Guard()
		{
			lock_->Unlock();
		}

	private:
		T*		lock_;
	};
}

#endif