#pragma once
#ifndef UTILITY_OBJKEY_GENERATOR_H
#define UTILITY_OBJKEY_GENERATOR_H

#include <UtilityCommon.h>
#include <chunk_stl.h>

#pragma pack(push)
#pragma pack()

namespace util {
;
//==================================================================================================
//	<CObjKeyGenerator>
//	-	Object들에 Key를 발급하고 회수하는 매니저
//	<History>
//	-	040906 : (ver1.0) Created
//	-	080519 : (ver1.1) DataStructure Changed & Bug Fix (by MjKim)
//	-	080520 : (ver1.2) Server Support by replacing datastructure (by Waverix)
//	-			ver1.1을 Server에서 사용하는데는 위험부담이 높아지는 관계로 자료구조 변경 (1.1의 목적도 반영)
//	<Used Version>
//	-	0x0100 - general
//	-	0x0110
//	-	0x0120
//  -   0x0130 - (ver1.3) change to an enhanced 'ReserveKey' to support client local handling
//
#define __OBJKEYGEN_VERSION_CTRL 0x0130
//

#if __OBJKEYGEN_VERSION_CTRL == 0x0130

//==================================================================================================
// version 0.1.3.0

namespace internal {
;

class ObjectKeyGeneratorBase
{
public:
    typedef ulong key_type;
    //static const key_type kMaxKeyBandwidth = (USHRT_MAX << 2); // (ULONG_MAX >> 2);
    static const key_type kMaxKeyBandwidth = (ULONG_MAX >> 2); // (ULONG_MAX >> 2);
    static const key_type kEmptyKey = 0;
    //
    ObjectKeyGeneratorBase();
    ObjectKeyGeneratorBase(key_type start_key, key_type end_key);
    ~ObjectKeyGeneratorBase();

    //
    bool Create(key_type start_key, key_type end_key);
    key_type GetKey();
    bool RestoreKey(key_type key);

    bool IsExistKey(key_type key) const;
    bool ReserveKey(key_type key);

    bool IsEmpty() const;
    size_t GetSize() const;

private:
    static const BOOLEAN kColorOfIssued = true;
    static const BOOLEAN kColorOfFree   = !kColorOfIssued;
#pragma pack(push, 1)
    struct AlignNode { BOOLEAN color_; };
#pragma pack(pop)
    //
    typedef STLX_DEQUE<AlignNode*> KeySet;   // Free Key Set
    //
    // data fields...
    bool initialized_;
    key_type lower_bound_;
    key_type upper_bound_;
    int number_of_free_;
    AlignNode* total_array_;
    KeySet free_key_set_;
};

//--------------------------------------------------------------------------------------------------
// implements

inline ObjectKeyGeneratorBase::ObjectKeyGeneratorBase()
    : initialized_(false),
      lower_bound_(0),
      upper_bound_(0),
      number_of_free_(0),
      total_array_(NULL)
{
}

inline ObjectKeyGeneratorBase::ObjectKeyGeneratorBase(key_type start_key, key_type end_key)
{
    Create(start_key, end_key);
}

inline bool ObjectKeyGeneratorBase::IsExistKey(key_type key) const
{
    if (lower_bound_ <= key && key < upper_bound_)
    {
        const key_type offset = key - lower_bound_;
        const AlignNode* align = &total_array_[offset];
        return (kColorOfFree == align->color_);
    }
    return false;
}

inline bool ObjectKeyGeneratorBase::IsEmpty() const
{
    return number_of_free_ <= 0;
}

inline size_t ObjectKeyGeneratorBase::GetSize() const
{
    return number_of_free_;
}

}; //end of nested namespace 'internal'


//==================================================================================================
// 

template<typename _KEYTYPE = DWORD>
class CObjKeyGenerator : public internal::ObjectKeyGeneratorBase
{
public:
    typedef _KEYTYPE key_type;
    // acceptable types = { uint8_t, uint16_t, uint32_t, ulong(DWORD) }
    BOOST_STATIC_ASSERT(\
        boost::is_integral<key_type>::value &&
        sizeof(key_type) <= sizeof(typename internal::ObjectKeyGeneratorBase::key_type) &&
        static_cast<key_type>(-1) > 0);
    //
    CObjKeyGenerator();
    CObjKeyGenerator(key_type start_key, key_type end_key);
    ~CObjKeyGenerator();

    //
    bool Create(key_type start_key, key_type end_key);
    key_type GetKey();
    bool RestoreKey(key_type key);

    bool IsExistKey(key_type key) const;
    bool ReserveKey(key_type key);

    bool IsEmpty() const;
    size_t GetSize() const;
};

//--------------------------------------------------------------------------------------------------
// implements

template<typename _KEYTYPE>
inline CObjKeyGenerator<_KEYTYPE>::CObjKeyGenerator()
{
}

template<typename _KEYTYPE>
inline CObjKeyGenerator<_KEYTYPE>::CObjKeyGenerator(key_type start_key, key_type end_key)
{
    ObjectKeyGeneratorBase::Create(start_key, end_key);
}

template<typename _KEYTYPE>
inline bool CObjKeyGenerator<_KEYTYPE>::Create(key_type start_key, key_type end_key)
{
    return ObjectKeyGeneratorBase::Create(start_key, end_key);
}

template<typename _KEYTYPE>
inline CObjKeyGenerator<_KEYTYPE>::~CObjKeyGenerator()
{
}

template<typename _KEYTYPE>
inline typename CObjKeyGenerator<_KEYTYPE>::key_type
CObjKeyGenerator<_KEYTYPE>::GetKey()
{
    return static_cast<key_type>(ObjectKeyGeneratorBase::GetKey());
}

template<typename _KEYTYPE>
inline bool CObjKeyGenerator<_KEYTYPE>::RestoreKey(key_type key)
{
    return ObjectKeyGeneratorBase::RestoreKey(key);
}

template<typename _KEYTYPE>
inline bool CObjKeyGenerator<_KEYTYPE>::IsExistKey(key_type key) const
{
    return ObjectKeyGeneratorBase::IsExistKey(key);
}

template<typename _KEYTYPE>
inline bool CObjKeyGenerator<_KEYTYPE>::ReserveKey(key_type key)
{
    return ObjectKeyGeneratorBase::ReserveKey(key);
}

template<typename _KEYTYPE>
inline bool CObjKeyGenerator<_KEYTYPE>::IsEmpty() const
{
    return ObjectKeyGeneratorBase::IsEmpty();
}

template<typename _KEYTYPE>
inline size_t CObjKeyGenerator<_KEYTYPE>::GetSize() const
{
    return ObjectKeyGeneratorBase::GetSize();
}

//
//==================================================================================================

#elif __OBJKEYGEN_VERSION_CTRL == 0x0120

//==================================================================================================
// version 0.1.2.0

	template<typename _KEYTYPE = DWORD>
	class CObjKeyGenerator
	{
	public:		CObjKeyGenerator( VOID );
				CObjKeyGenerator( const _KEYTYPE startKey, const _KEYTYPE endKey );
				~CObjKeyGenerator( VOID );
	public:
		//─────────────────────────────────────────
		//	<INTERFACEs>
	public:		VOID		Create( const _KEYTYPE startKey, const _KEYTYPE endKey );
				_KEYTYPE	GetKey( VOID );
				VOID		RestoreKey( const _KEYTYPE key );

				BOOL		IsExistKey( const _KEYTYPE key );
				BOOL		ReserveKey( const _KEYTYPE key );

				BOOL		IsEmpty( VOID );
				SIZE_T		GetSize( VOID );

	private:
		static const BOOLEAN	COLOR_ISSUE		= TRUE;
		static const BOOLEAN	COLOR_FREE		= FALSE;
		struct sALIGN { _KEYTYPE KEY; BOOLEAN COLOR; };
		typedef STLX_DEQUE<_KEYTYPE>	KeySet;		// Free Key Set
		typedef STLX_VECTOR<sALIGN>		TotalSet;	// Set for total managed key

	private:	_KEYTYPE 	m_Lowerbound;
				_KEYTYPE	m_Upperbound;
				KeySet		m_keySet;
				TotalSet	m_TotalKey;
	};

	template<typename _KEYTYPE>
		CObjKeyGenerator<_KEYTYPE>::CObjKeyGenerator( VOID )
	{
		m_Lowerbound = m_Upperbound = 0;
	}

	template<typename _KEYTYPE>
		CObjKeyGenerator<_KEYTYPE>::CObjKeyGenerator( const _KEYTYPE startKey, const _KEYTYPE endKey )
	{
		Create( startKey, endKey );
	}

	template<typename _KEYTYPE>
		VOID CObjKeyGenerator<_KEYTYPE>::Create( const _KEYTYPE startKey, const _KEYTYPE endKey )
	{
		ASSERT( (startKey < endKey) && m_keySet.empty() );
		m_Lowerbound = startKey;
		m_Upperbound = endKey + 1 ; // [lb, endKey] -> [lb, up)
		const _KEYTYPE range = m_Upperbound-m_Lowerbound;

		// deque는 reserve가 없어용
		m_TotalKey.reserve( range );

		sALIGN alignS = { 0, COLOR_FREE };
		for( _KEYTYPE key=m_Lowerbound ; key<m_Upperbound ; ++key )
		{
			alignS.KEY = key;
			m_TotalKey.push_back( alignS );
			m_keySet.push_back( key );
		}
	}

	template<typename _KEYTYPE>
		CObjKeyGenerator<_KEYTYPE>::~CObjKeyGenerator( VOID )
	{
		m_Lowerbound = m_Upperbound = 0;
	}

	template<typename _KEYTYPE>
		_KEYTYPE CObjKeyGenerator<_KEYTYPE>::GetKey( VOID )
	{
		if( m_keySet.empty() )
		{
			ASSERT( !"할당할 수 있는 키가 없습니다." );
			return (_KEYTYPE)0;
		}

		KeySet::iterator it = m_keySet.begin();

		const _KEYTYPE issue_key = *it;
		const _KEYTYPE index = issue_key-m_Lowerbound;
		sALIGN& rAlign = m_TotalKey[index];
		rAlign.COLOR = COLOR_ISSUE;

		m_keySet.pop_front();

		return issue_key;
	}

	template<typename _KEYTYPE>
		VOID CObjKeyGenerator<_KEYTYPE>::RestoreKey( const _KEYTYPE key )
	{
		if( m_Lowerbound<=key && key<m_Upperbound )
		{
			const _KEYTYPE index = key-m_Lowerbound;
			sALIGN& rAlign = m_TotalKey[index];
			if( COLOR_ISSUE == rAlign.COLOR )
			{
				rAlign.COLOR = COLOR_FREE;
				m_keySet.push_back( key );
				return;
			}
			ASSERT( (COLOR_ISSUE == rAlign.COLOR) );		// 이미 반환되거나...
			return;
		}
		ASSERT( (m_Lowerbound<=key && key<m_Upperbound) );	// 잘못된 키
	}

	template<typename _KEYTYPE>
		BOOL CObjKeyGenerator<_KEYTYPE>::IsExistKey( const _KEYTYPE key )
	{
		if( m_Lowerbound<=key && key<m_Upperbound )
		{
			const _KEYTYPE index = key-m_Lowerbound;
			sALIGN& rAlign = m_TotalKey[index];
			return ( COLOR_FREE == rAlign.COLOR );
		}
		return FALSE;
	}

	//	(NOTE) 이건 특별한 일 없으면 사용하지 않는 것이 좋다.
	template<typename _KEYTYPE>
		BOOL CObjKeyGenerator<_KEYTYPE>::ReserveKey( const _KEYTYPE key )
	{
		if( m_Lowerbound<=key && key<m_Upperbound )
		{
			const _KEYTYPE index = key-m_Lowerbound;
			sALIGN& rAlign = m_TotalKey[index];
			if( COLOR_FREE == rAlign.COLOR )
			{
				rAlign.COLOR = COLOR_ISSUE;
				KeySet::iterator itfind = find( m_keySet.begin(), m_keySet.end(), key );
				if( itfind != m_keySet.end() )
					m_keySet.erase( itfind );
				return TRUE;
			}
			ASSERT( (COLOR_FREE == rAlign.COLOR) );
		}
		ASSERT( (m_Lowerbound<=key && key<m_Upperbound) );
		return FALSE;
	}

	template<typename _KEYTYPE>
		BOOL CObjKeyGenerator<_KEYTYPE>::IsEmpty( VOID )
	{
		return ( m_keySet.empty() );
	}

	template<typename _KEYTYPE>
		SIZE_T CObjKeyGenerator<_KEYTYPE>::GetSize( VOID )
	{
		return m_keySet.size();
	}

//==================================================================================================

#else //__OBJKEYGEN_VERSION_CTRL < 0x0120
; //

#endif

}; // End of Namespace Util

#pragma pack(pop)

#endif //UTILITY_OBJKEY_GENERATOR_H
