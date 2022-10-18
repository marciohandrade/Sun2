#pragma once

#include <windows.h>
#include <map>
#include <vector>
#include <assert.h>
#include "WGPLCryptor.h"
#include "WGPLAllocator.h"

namespace WGPL
{
	static const size_t __rc = 11;

	//
	// T			: 타입
	// _checksum	: true이면, 메모리가 변경되었는 지 checksum을 두고 검사함. 보안 강력 but 느림
	// _replicaCount: 실제 변수 포함해서 더미 변수를 몇 개 만들건지 (디폴트 11 = 실제 1개 + 더미 10개)
	//
	template<class T, bool _checksum = true, size_t _replicaCount = __rc>
	class Value
	{
	public:
		Value()	: seed_(0), index_(0), check_(0)
		{
#ifdef _DEBUG
			memset(map_, 0, sizeof(map_));
#endif
		};
		~Value()
		{
			// note: empty() 검사하지 않으면 쓰레기값을 지우게 되어 crash
			if (!empty())
			{
				for (int i = 0; i < _replicaCount; i++)
				{
					Free(map_[i], sizeof(T));
				}
			}
		};

		inline bool empty()
		{
			return !seed_;
		}

		inline void Set(T val)
		{
#ifdef _DEBUG
			debugVal_ = val;
#endif

			if (empty())
			{
				// index 갱신
				renewIndex();

				for (int i = 0; i < _replicaCount; i++)
				{
					T* p = (T*)Alloc(sizeof(T));	// 쓰레기값 들어감
					if (i == index_ || (_checksum && i == check_))
					{
						// 암호화
						*p = val;
						Encrypt(seed_ + i, (char *)p, sizeof(T));
					}

					map_[i] = p;
				}
			}
			else
			{
				renewIndex();

				T tmp[_replicaCount];	// 쓰레기 값
				for (int i = 0; i < _replicaCount; ++i)
				{
					T* p = map_[i];

					if (i == index_ || (_checksum && i == check_))
					{
						// 암호화
						*p = val;	// 실제 값으로 셋팅
						Encrypt(seed_ + i, (char *)p, sizeof(T));
					}
					else
					{
						// 쓰레기 값 set, warning 무시
						*p = tmp[i];
					}
				}
			}
		}

		inline T Get()
		{
			if (empty())
			{
				// note: T는 primitive type만 가능
				return 0;
			}
			else
			{
				return getValue();
			}
		}

		inline void Swap(Value<T, _checksum, _replicaCount>& src)
		{
			swap(this->seed_, src.seed_);
			swap(this->index_, src.index_);
			swap(this->check_, src.check_);
#ifdef _DEBUG
			swap(this->debugVal_, src.debugVal_);
#endif

			// swap map
			T* tmp[_replicaCount];
			memcpy(tmp, this->map_, sizeof(tmp));
			memcpy(this->map_, src.map_, sizeof(tmp));
			memcpy(src.map_, tmp, sizeof(tmp));
		}

	private:
		inline T getValue()
		{
			assert(!empty());

			T val = getValue(index_);
#ifdef _DEBUG
			assert(debugVal_ == val);
#endif

			if (_checksum)
			{
				T check = getValue(check_);

				// checksum 검사
				if (val != check)
				{
					// todo: 해킹!!
					//std::cout << "val = " << val << ", check = " << check << std::endl;
					//printf("getValue: index_ = %u, check_ = %u\n", index_, check_);

                    if (_Callback_WGPL != NULL)
                    {
                        _Callback_WGPL();
                    }
                    else
                    {
                        throw;
                    }
				}
			}

			return val;
		}

		inline T getValue(unsigned int index)
		{
			T* p = map_[index];

			T val;
			memcpy(&val, p, sizeof(T));

			Decrypt(seed_ + index, (char *)&val, sizeof(T));

			return val;
		}

		inline void renewIndex()
		{
			// seed 저장
			seed_ = GetTickCount();

			// _replicaCount가 1이면 index바꾸는 게 의미 없음.
			if (_replicaCount <= 1)
			{
				return;
			}

			// 실제 값이 저장된 index
			index_ = rand() % _replicaCount;

			if (_checksum)
			{
				// 체크섬의 index
				check_ = index_ + 1;
				check_ %= _replicaCount;
			}

			assert(index_ != check_);
		}

		template<class _ST>
		inline void swap(_ST& a, _ST& b)
		{
			_ST tmp = a;
			a = b;
			b = tmp;
		}

	private:
		unsigned int seed_;
		unsigned int index_;		// 실제 값이 저장된 index
		unsigned int check_;		// 실제 값이 변경되었는 지 검증할 checksum이 들어있는 index
		T* map_[_replicaCount];
#ifdef _DEBUG
		T debugVal_;				// correct if _DEBUG is defined
#endif
	};
}
