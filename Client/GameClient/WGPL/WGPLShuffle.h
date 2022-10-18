#pragma once

#include <map>
#include "WGPLValue.h"

namespace WGPL	// ngpl clone
{
	template<class T, bool _checksum = true, size_t _replicaCount = __rc>
	class Shuffle
	{
	public:
		Shuffle() : lasttick_(0), tickPeriod_(5000)
		{
			lasttick_ = GetTickCount();
		};
		Shuffle(const T& val) : lasttick_(0), tickPeriod_(5000)	// copy constructor
		{
			lasttick_ = GetTickCount();
			val_.Set(val);
		}
		Shuffle(Shuffle<T, _checksum, _replicaCount>& val) : lasttick_(0), tickPeriod_(5000)
		{
			lasttick_ = GetTickCount();
			val_.Set(val);
		}

		~Shuffle()
		{
			lasttick_ = 0;
		}

		// operator override
		operator T const ()
		{
			realloc();
			return val_.Get();
		}
		T operator=(const T val)
		{
			realloc();
			val_.Set(val);
			return val;
		}
		// note: 이게 없으면, Shuffle<> == Shuffle<>인 경우, memory deep copy가 안되서 crash
		T operator=(const Shuffle<T, _checksum, _replicaCount>& val)
		{
			realloc();

			// note: const를 뗘야함.
			Shuffle<T, _checksum, _replicaCount>& cval = const_cast<Shuffle<T, _checksum, _replicaCount>& >(val);
			val_.Set(T(cval));
			return cval;
		}
		T operator++()
		{
			realloc();
			T tmp = val_.Get();
			val_.Set(tmp + 1);
			return tmp + 1;
		}
		T operator++(int)
		{
			realloc();
			T tmp = val_.Get();
			val_.Set(tmp + 1);
			return tmp;
		}
		T operator--()
		{
			realloc();
			T tmp = val_.Get();
			val_.Set(tmp - 1);
			return tmp - 1;
		}
		T operator--(int)
		{
			realloc();
			T tmp = val_.Get();
			val_.Set(tmp - 1);
			return tmp;
		}
		T operator+=(const T& val)
		{
			realloc();
			T tmp = val_.Get() + val;
			val_.Set(tmp);
			return tmp;
		}
		T operator-=(const T& val)
		{
			realloc();
			T tmp = val_.Get() - val;
			val_.Set(tmp);
			return tmp;
		}
		T operator*=(const T& val)
		{
			realloc();
			T tmp = val_.Get() * val;
			val_.Set(tmp);
			return tmp;
		}
		T operator/=(const T& val)
		{
			realloc();
			T tmp = val_.Get() / val;
			val_.Set(tmp);
			return tmp;
		}
		T operator%=(const T& val)
		{
			realloc();
			T tmp = val_.Get() % val;
			val_.Set(tmp);
			return tmp;
		}
		T operator&=(const T& val)
		{
			realloc();
			T tmp = val_.Get() & val;
			val_.Set(tmp);
			return tmp;
		}
		T operator|=(const T& val)
		{
			realloc();
			T tmp = val_.Get() | val;
			val_.Set(tmp);
			return tmp;
		}
		T operator^=(const T& val)
		{
			realloc();
			T tmp = val_.Get() ^ val;
			val_.Set(tmp);
			return tmp;
		}

		// customizing allocation time set function
		void SetShuffleTime(unsigned long tick = 5000)
		{
			tickPeriod_ = tick;
			realloc();
		}

	private:
		inline void realloc()
		{
			unsigned long now = GetTickCount();
			unsigned long elpased = now - lasttick_;

			if (elpased >= tickPeriod_)
			{
				lasttick_ = now;

				// 재할당
				Value<T, _checksum, _replicaCount> tmp;
				tmp.Set(val_.Get());

				val_.Swap(tmp);
			}
		}

	private:
		Value<T, _checksum, _replicaCount> val_;
		unsigned long lasttick_;	// last tick
		unsigned long tickPeriod_;	// user set tick by set_minAllocTime
	};
}
