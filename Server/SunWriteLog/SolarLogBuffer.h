#pragma once
#include <vector>

///////////...///////////...///////////...///////////...///////////...///////////...///////////...///////////...
// Log의 가변데이터를 위한 버퍼 -- by shaketake 2008 - 11 - 03
// 딴데다 써도 되긴함...
///////////...///////////...///////////...///////////...///////////...///////////...///////////...///////////...

typedef int int32;
typedef unsigned char	uint8;

DLLEXPORTT_EMPLATE template class DLL_DECLARE std::allocator<uint8>;
DLLEXPORTT_EMPLATE template class DLL_DECLARE std::vector<uint8, std::allocator<uint8> >;

class DLL_DECLARE SolarLogBuffer
{
public:
	const static size_t DEFAULT_SIZE = 0x1000;

	// constructor
	SolarLogBuffer(): _rpos(0), _wpos(0)
	{
		_storage.reserve(DEFAULT_SIZE);
	}
	// constructor
	SolarLogBuffer(size_t res): _rpos(0), _wpos(0)
	{
		_storage.reserve(res);
	}
	// copy constructor
	SolarLogBuffer(const SolarLogBuffer &buf): _rpos(buf._rpos), _wpos(buf._wpos), _storage(buf._storage) { }

	void clear()
	{
		_storage.clear();
		_rpos = _wpos = 0;
	}

	SolarLogBuffer &operator<<(int32 value)
	{
		char temp[10] = {0,};
		itoa(value, temp, 10);
		*this << temp;
		return *this;
	}

	SolarLogBuffer &operator<<(const std::string &value)
	{
		append((uint8 const *)value.c_str(), value.length());
		append((uint8)0);
		return *this;
	}
	SolarLogBuffer &operator<<(const char *str)
	{
		append((uint8 const *)str, str ? strlen(str) : 0);
		append((uint8)0);
		return *this;
	}

	SolarLogBuffer &operator>>(std::string& value)
	{
		value.clear();
		while (rpos() < size())
		{
			char c=read<char>();
			if (c==0)
				break;
			value+=c;
		}
		return *this;
	}

	size_t rpos() const { return _rpos; }
	size_t rpos(size_t rpos_)
	{
		_rpos = rpos_;
		return _rpos;
	};

	size_t wpos() const { return _wpos; }
	size_t wpos(size_t wpos_)
	{
		_wpos = wpos_;
		return _wpos;
	}

	///////////...///////////...///////////...///////////...///////////...///////////...////////
	template <typename T> T read()
	{
		T r=read<T>(_rpos);
		_rpos += sizeof(T);
		return r;
	};
	template <typename T> T read(size_t pos) const
	{
		T val = *((T const*)&_storage[pos]);
		return val;
	}

	void read(uint8 *dest, size_t len)
	{
		memcpy(dest, &_storage[_rpos], len);
		_rpos += len;
	}

	const uint8 *contents() const { return &_storage[0]; }

	size_t size() const { return _storage.size(); }
	bool empty() const { return _storage.empty(); }

	void resize(size_t newsize)
	{
		_storage.resize(newsize);
		_rpos = 0;
		_wpos = size();
	};
	void reserve(size_t ressize)
	{
		if (ressize > size()) _storage.reserve(ressize);
	};

	///////////...///////////...///////////...///////////...///////////...///////////...////////
	template <typename T> void append(T value)
	{
		append((uint8 *)&value, sizeof(value));
	}

	void append(const std::string& str)
	{
		append((uint8 const*)str.c_str(),str.size() + 1);
	}
	void append(const char *src, size_t cnt)
	{
		return append((const uint8 *)src, cnt);
	}
	template<class T>
		void append(const T *src, size_t cnt)
	{
		return append((const uint8 *)src, cnt*sizeof(T));
	}
	void append(const uint8 *src, size_t cnt)
	{
		if (!cnt) return;
		if (_storage.size() < _wpos + cnt)
			_storage.resize(_wpos + cnt);
		memcpy(&_storage[_wpos], src, cnt);
		_wpos += cnt;
	}
	void append(const SolarLogBuffer& buffer)
	{
		if(buffer.size()) append(buffer.contents(),buffer.size());
	}


protected:
	size_t _rpos, _wpos;
	std::vector<uint8> _storage;
};
