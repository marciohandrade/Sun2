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
	// T			: Ÿ��
	// _checksum	: true�̸�, �޸𸮰� ����Ǿ��� �� checksum�� �ΰ� �˻���. ���� ���� but ����
	// _replicaCount: ���� ���� �����ؼ� ���� ������ �� �� ������� (����Ʈ 11 = ���� 1�� + ���� 10��)
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
			// note: empty() �˻����� ������ �����Ⱚ�� ����� �Ǿ� crash
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
				// index ����
				renewIndex();

				for (int i = 0; i < _replicaCount; i++)
				{
					T* p = (T*)Alloc(sizeof(T));	// �����Ⱚ ��
					if (i == index_ || (_checksum && i == check_))
					{
						// ��ȣȭ
						*p = val;
						Encrypt(seed_ + i, (char *)p, sizeof(T));
					}

					map_[i] = p;
				}
			}
			else
			{
				renewIndex();

				T tmp[_replicaCount];	// ������ ��
				for (int i = 0; i < _replicaCount; ++i)
				{
					T* p = map_[i];

					if (i == index_ || (_checksum && i == check_))
					{
						// ��ȣȭ
						*p = val;	// ���� ������ ����
						Encrypt(seed_ + i, (char *)p, sizeof(T));
					}
					else
					{
						// ������ �� set, warning ����
						*p = tmp[i];
					}
				}
			}
		}

		inline T Get()
		{
			if (empty())
			{
				// note: T�� primitive type�� ����
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

				// checksum �˻�
				if (val != check)
				{
					// todo: ��ŷ!!
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
			// seed ����
			seed_ = GetTickCount();

			// _replicaCount�� 1�̸� index�ٲٴ� �� �ǹ� ����.
			if (_replicaCount <= 1)
			{
				return;
			}

			// ���� ���� ����� index
			index_ = rand() % _replicaCount;

			if (_checksum)
			{
				// üũ���� index
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
		unsigned int index_;		// ���� ���� ����� index
		unsigned int check_;		// ���� ���� ����Ǿ��� �� ������ checksum�� ����ִ� index
		T* map_[_replicaCount];
#ifdef _DEBUG
		T debugVal_;				// correct if _DEBUG is defined
#endif
	};
}
