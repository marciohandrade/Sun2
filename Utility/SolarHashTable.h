#pragma once
#ifndef UTILITY_SOLAR_HASH_TABLE_H
#define UTILITY_SOLAR_HASH_TABLE_H

//==================================================================================================
// SolarHashTable.h: interface for the SolarHashTable class.
// Hash Table Class version 1.0
// source code created by Min-Wook Kim (taiyo@webzen.co.kr)
// 2004-7-29
// 
//--------------------------------------------------------------------------------------------------
// @history
// 2004-07-27 : HashTable class was created with MemoryPool
// 2004-07-29 : added m_dwBucketKey
// 2004-12-21 : change class name, CHashTable to SolarHashTable
// 2005-04-26 : added class < KeyType=DWORD >
// 2005-08-08 : added find() method for iterator class 
// 2011-05-26 : waverix, change allocators
// 2011-05-28 : waverix, f110528.1L, upgrade to v0200
//  purpose: memory optimization + to avoid misunderstanding usage
//  specifications
//  support algorithm
//      'v < 0200': { list, hash, single-hash & multi-key buckets }
//      'v >=0200': { list, hash }
//      NOTE: f110528.1L, most custom usage cases are used to unique hash table.
//      furthermore, the multi-key inserting has caused everywhere enough trouble recently
//  changes (bucket order)
//      'v < 0200': the buckets order of a list same the added sequence like a list.push_back
//      'v >=0200': bidirectional order (it doesn't support bucket ordered iterating)
//      NOTE: f110528.1L, this usages are rare.
//  changes (hash bucket size)
//      'v < 0200': fixed. this size same initializing arguments
//      'v >=0210': variable
//--------------------------------------------------------------------------------------------------
// example)
//
// SolarHashTable<TTT> * hashTable = new SolarHashTable<TTT>;
// hashTable->Initialize(100);
// hashTable->RemoveAll();
// delete hashTable;

// caution) duplicated loop ( duplicated GetNext() traveling ) is bad usage
//==================================================================================================

//==================================================================================================
// version control
#if defined(_SERVER)
#define SOLAR_HASH_TABLE_VERSION  0x0210
#else
#define SOLAR_HASH_TABLE_VERSION  0x0210
#endif

//==================================================================================================

//==================================================================================================
//==================================================================================================
//==================================================================================================
#if SOLAR_HASH_TABLE_VERSION >= 0x0200
//==================================================================================================
//==================================================================================================
//==================================================================================================
#include "./UtilityCommon.h"
#include "./LinkedListCS.h"
#include "./MemoryPoolFactory.h" // don't used, but added for include dependencies

namespace util { namespace internal {
;
struct PrimeNode;

// @history
//  2011.05.28, waverix, f110528.1L, created. this class is based on generic algorithm
class WxHashTableBase
{
public:
    typedef boost::mpl::if_c<sizeof(void*) == sizeof(ulong), ulong,
            boost::mpl::if_c<sizeof(void*) == sizeof(int64), uint64, void> >::type
#if defined(_WIN64)
            ::type
#endif
            hash_type;
    //
    typedef uint8_t* complex_type;
    static const ulong kVersion = 0x0210;
    static const uint16_t kInitializedTag = uint16_t(('I' << 0) | ('h' << 8));
    //
    struct WxHashNode : public util::__LList<WxHashNode>
    {
        complex_type key;
        complex_type data;
    };
    typedef WxHashNode node_type;

    WxHashTableBase();
    explicit WxHashTableBase(ulong init_bucket_size);
    ~WxHashTableBase();
    // custom SolarHashTable compatible interface
    bool Initialize(ulong init_bucket_size);

    size_t size() const;
    node_type* find(complex_type key) const;
    node_type* begin() const;
    node_type* end() const;
    bool insert(complex_type key, complex_type data);
    // if the operation is success, the result is the next node also changed to empty table.
    node_type* erase(node_type* const it);
    bool erase(complex_type key);
    void clear();
    //
    node_type* climb_to_next(node_type*);
    node_type* climb_to_prev(node_type*);
    static void make_balanced_buckets(WxHashTableBase* cur_hash);
private:
    //
    uint16_t initialized_mark_;
    size_t number_of_nodes_;
    const PrimeNode* prime_node_;
    node_type** buckets_;
    node_type** end_bucket_;
    node_type list_root_;
    //
    __DISABLE_COPY(WxHashTableBase);
    WAVERIX_TEST_BLOCK_IN_CLASS(WxHashTableBase);
}; //end of class 'WxHashTableBase'

}}; //end of namespace 'util.internal'
//==================================================================================================
struct util::internal::PrimeNode
{
    util::internal::WxHashTableBase::hash_type prime;
    util::internal::WxHashTableBase::hash_type need_change_next;
};
//
inline size_t util::internal::WxHashTableBase::size() const
{
    return number_of_nodes_;
}

inline util::internal::WxHashTableBase::node_type*
util::internal::WxHashTableBase::begin() const
{
    return list_root_.next;
}

inline util::internal::WxHashTableBase::node_type*
util::internal::WxHashTableBase::end() const
{
    return const_cast<node_type*>(&list_root_);
}

inline util::internal::WxHashTableBase::node_type*
util::internal::WxHashTableBase::climb_to_next(node_type* it)
{
    node_type* next_it = it->next;
    if (next_it == it) {
        assert(!"must check these cases");
        next_it = &list_root_;
    };
    return next_it;
}

inline util::internal::WxHashTableBase::node_type*
util::internal::WxHashTableBase::climb_to_prev(node_type* it)
{
    node_type* prev_it = it->prev;
    if (prev_it == it) {
        assert(!"must check these cases");
        prev_it = &list_root_;
    };
    return prev_it;
}

//==================================================================================================
//==================================================================================================
namespace util {
;

//==================================================================================================

template<class _Type, class _KeyType = DWORD>
class SolarHashTable
{
public:
    typedef _KeyType key_type;
    typedef _Type data_type;
    typedef SolarHashTable<data_type, key_type> table_type;
    //
    class iterator;
    BOOST_STATIC_ASSERT(boost::is_pod<key_type>::value &&
                        boost::is_pod<data_type>::value &&
                        sizeof(key_type) <= sizeof(void*) &&
                        sizeof(data_type) <= sizeof(void*));
    //
    SolarHashTable();
    explicit SolarHashTable(const DWORD init_bucket_size);
    ~SolarHashTable();
    //
    key_type GetDataNum() const;
    bool Initialize(const DWORD init_bucket_size);
    bool Add(data_type data, key_type key);
    data_type GetData(key_type key) const;
    void Remove(key_type key);
    //data_type RemoveHead(); //< don't support anymore v0200
    void RemoveAll();
    //
    data_type GetHeadData() const;
    data_type GetTailData() const;
    // self-contained iterating support interfaces = {
    //  SetFirst, GetNext, SetLast(X), GetPrev(X)
    bool SetFirst();
    data_type GetNext() const;
    // don't support on v0200 anymore = {
    //  void SetBucketFirst(key_type key);
    //  data_type GetBucketNext() const;
    //  void RemoveCurBucketData();
    //}
    //
    iterator find(key_type key) const;
    iterator begin() const;
    iterator end() const;
    key_type size() const;
    bool insert(key_type key, data_type data);
    iterator erase(const iterator& it);
    bool erase(key_type key);
    void clear();
    //
private:
    typedef util::internal::WxHashTableBase base_type;
    //
    base_type* base_;
    mutable base_type::node_type* self_contained_iterator_;
    //
    friend class iterator;
    __DISABLE_COPY(SolarHashTable);
    WAVERIX_TEST_BLOCK_IN_CLASS(SolarHashTable);
}; //end of class 'SolarHashTable<,>'

//==================================================================================================

template<class _Type, class _KeyType>
class SolarHashTable<_Type, _KeyType>::iterator
{
public:
    iterator();
    iterator operator++() const;
    iterator operator++(int) const;

    bool operator!=(const iterator& it);
    bool operator==(const iterator& it);
    data_type operator*() const;
private:
    mutable base_type::node_type* ptr_;
    mutable base_type* table_; //reserved, to validate iteration integrity
    //
    friend class table_type;
};

//==================================================================================================
//==================================================================================================
}; //end of namespace
//==================================================================================================
#pragma warning(push)
#pragma warning(disable: 4311 4312)
//==================================================================================================
// iterator implements
template<class _Type, class _KeyType>
__forceinline util::SolarHashTable<_Type, _KeyType>::iterator::iterator()
    : ptr_(NULL)
    , table_(NULL)
{
}

template<class _Type, class _KeyType>
typename util::SolarHashTable<_Type, _KeyType>::iterator
__forceinline util::SolarHashTable<_Type, _KeyType>::iterator::operator++() const
{
    this->ptr_ = table_->climb_to_next(this->ptr_);
    this->table_ = table_;
    return *this;
}

template<class _Type, class _KeyType>
typename util::SolarHashTable<_Type, _KeyType>::iterator
__forceinline util::SolarHashTable<_Type, _KeyType>::iterator::operator++(int) const
{
    iterator it;
    it.ptr_ = this->ptr_;
    this->ptr_ = table_->climb_to_next(this->ptr_);
    this->table_ = table_;
    return *this;
}

template<class _Type, class _KeyType>
__forceinline bool util::SolarHashTable<_Type, _KeyType>::iterator::operator!=(const iterator& it)
{
    return (this->ptr_ != it.ptr_);
}

template<class _Type, class _KeyType>
__forceinline bool util::SolarHashTable<_Type, _KeyType>::iterator::operator==(const iterator& it)
{
    return (this->ptr_ == it.ptr_);
}

template<class _Type, class _KeyType>
typename util::SolarHashTable<_Type, _KeyType>::data_type
__forceinline util::SolarHashTable<_Type, _KeyType>::iterator::operator*() const
{
    return (this->ptr_ != NULL) ?
        reinterpret_cast<data_type>(this->ptr_->data) : data_type();
}

//==================================================================================================
// SolarHashTable implements
template<class _Type, class _KeyType>
util::SolarHashTable<_Type, _KeyType>::SolarHashTable()
    : base_(NULL)
    , self_contained_iterator_(NULL)
{
}

template<class _Type, class _KeyType>
util::SolarHashTable<_Type, _KeyType>::SolarHashTable(const DWORD init_bucket_size)
    : base_(TAllocNew(base_type)(init_bucket_size))
    , self_contained_iterator_(NULL)
{
}

template<class _Type, class _KeyType>
util::SolarHashTable<_Type, _KeyType>::~SolarHashTable()
{
    if (base_) {
        TAllocDelete(base_type, base_);
        base_ = NULL;
    };
    self_contained_iterator_ = NULL;
}
//
template<class _Type, class _KeyType>
typename typename util::SolarHashTable<_Type, _KeyType>::key_type
util::SolarHashTable<_Type, _KeyType>::GetDataNum() const
{
    return static_cast<key_type>(base_->size());
}

template<class _Type, class _KeyType>
bool util::SolarHashTable<_Type, _KeyType>::Initialize(const DWORD init_bucket_size)
{
    if (base_) {
        return false;
    };
    base_ = TAllocNew(base_type)(init_bucket_size);
    self_contained_iterator_ = NULL;
    return true;
}

template<class _Type, class _KeyType>
bool util::SolarHashTable<_Type, _KeyType>::Add(data_type data, key_type key)
{
    return base_->insert(base_type::complex_type(key), base_type::complex_type(data));
}

template<class _Type, class _KeyType>
typename util::SolarHashTable<_Type, _KeyType>::data_type
util::SolarHashTable<_Type, _KeyType>::GetData(key_type key) const
{
    base_type::node_type* found_it = base_->find(base_type::complex_type(key));
    if (found_it == base_->end()) {
        return data_type();
    };
    return data_type(found_it->data);
}

template<class _Type, class _KeyType>
void util::SolarHashTable<_Type, _KeyType>::Remove(key_type key)
{
    base_->erase(base_type::complex_type(key));
}

template<class _Type, class _KeyType>
void util::SolarHashTable<_Type, _KeyType>::RemoveAll()
{
    if (base_) {
        base_->clear();
    };
    self_contained_iterator_ = NULL;
}

//
template<class _Type, class _KeyType>
typename util::SolarHashTable<_Type, _KeyType>::data_type
util::SolarHashTable<_Type, _KeyType>::GetHeadData() const
{
    // NOTE: f110528.1L, it is different from custom logic.
    // the head of the custom has meant the first inserted node
    // while the updating logic will be meaning first node of hash-nodes like stdext::hash_map
    base_type::node_type* begin_it = base_->begin();
    if (begin_it == base_->end()) {
        return data_type();
    };
    return data_type(begin_it->data);
}

template<class _Type, class _KeyType>
typename util::SolarHashTable<_Type, _KeyType>::data_type
util::SolarHashTable<_Type, _KeyType>::GetTailData() const
{
    // NOTE: f110528.1L, it is different from custom logic.
    // the tail of the custom has meant the last inserted node
    // while the updating logic will be meaning last node of hash-nodes like stdext::hash_map
    base_type::node_type* end_it = base_->end();
    base_type::node_type* last_it = base_->climb_to_prev(end_it);
    if (last_it == end_it) {
        return data_type();
    };
    return data_type(last_it->data);
}

// self-contained iterating support interfaces = {
//  SetFirst, GetNext, SetLast(X), GetPrev(X)
template<class _Type, class _KeyType>
bool util::SolarHashTable<_Type, _KeyType>::SetFirst()
{
#if SUN_CODE_BACKUP
    // NOTE: f110531.7L, it's possible
    // that the iterated finding routine is stopped when condition satisfy.
    if (FlowControl::FCAssert(self_contained_iterator_ == NULL) == false) {
        assert(!"it does occurred nested loop!, it may caused dangerous and unexpected operation");
        return false;
    };
#endif
    //헤쉬 테이블의 BASE값에 대한 검사
    if(!base_)
    {
        return false;
    }

    base_type::node_type* begin_it = base_->begin();
    if (begin_it == base_->end()) {
        return false;
    };
    self_contained_iterator_ = begin_it;
    return true;
}

template<class _Type, class _KeyType>
typename util::SolarHashTable<_Type, _KeyType>::data_type
util::SolarHashTable<_Type, _KeyType>::GetNext() const
{
    if (self_contained_iterator_)
    {
        const base_type::node_type* end_it = base_->end();
        if (self_contained_iterator_ != end_it) {
            const data_type& data = data_type(self_contained_iterator_->data);
            self_contained_iterator_ = base_->climb_to_next(self_contained_iterator_);
            return data;
        };
        if (self_contained_iterator_ == end_it) {
            self_contained_iterator_ = NULL;
        };
    };
    return data_type();
}

//
template<class _Type, class _KeyType>
typename util::SolarHashTable<_Type, _KeyType>::iterator
__forceinline util::SolarHashTable<_Type, _KeyType>::find(key_type key) const
{
    iterator it;
    it.ptr_ = base_->find(base_type::complex_type(key));
    it.table_ = base_;
    return it;
}

template<class _Type, class _KeyType>
typename util::SolarHashTable<_Type, _KeyType>::iterator
__forceinline util::SolarHashTable<_Type, _KeyType>::begin() const
{
    iterator it;
    it.ptr_ = base_->begin();
    it.table_ = base_;
    return it;
};

template<class _Type, class _KeyType>
typename util::SolarHashTable<_Type, _KeyType>::iterator
__forceinline util::SolarHashTable<_Type, _KeyType>::end() const
{
    iterator it;
    it.ptr_ = base_->end();
    it.table_ = base_;
    return it;
}

template<class _Type, class _KeyType>
typename util::SolarHashTable<_Type, _KeyType>::key_type
__forceinline util::SolarHashTable<_Type, _KeyType>::size() const
{
    return (base_) ? static_cast<key_type>(base_->size()) : 0;
}

template<class _Type, class _KeyType>
bool util::SolarHashTable<_Type, _KeyType>::insert(key_type key, data_type data)
{
    return base_->insert(base_type::complex_type(key), base_type::complex_type(data));
}

template<class _Type, class _KeyType>
typename util::SolarHashTable<_Type, _KeyType>::iterator
util::SolarHashTable<_Type, _KeyType>::erase(const iterator& it)
{
    iterator _it;
    _it.ptr_ = base_->erase(it.ptr_);
    _it.table_ = base_;
    return _it;
}

template<class _Type, class _KeyType>
bool util::SolarHashTable<_Type, _KeyType>::erase(key_type key)
{
    return base_->erase(base_type::complex_type(key));
}

template<class _Type, class _KeyType>
__forceinline void util::SolarHashTable<_Type, _KeyType>::clear()
{
    if (base_) {
        base_->clear();
    };
    self_contained_iterator_ = NULL;
}
#pragma warning(pop)
//==================================================================================================
//==================================================================================================
//==================================================================================================
#else //SOLAR_HASH_TABLE_VERSION < 0x0200
//==================================================================================================
//==================================================================================================
//==================================================================================================

#include "MemoryPoolFactory.h"

namespace util
{

template <class T, class KeyType=DWORD >
class SolarHashTable
{	
	class HashBucket;

	class HashList
	{
	public:
		HashList *		pNext;
		HashList *		pPrv;
		HashBucket *	pBucket;
	};

	class HashBucket
	{
	public:
		KeyType			dwKey;
		T				TData;
		HashList *		pList;		// 전체 버킷리스트
		HashBucket *	pNext;		// 동일 키값을 가지는 버킷리스트에 대한 Next
		HashBucket *	pPrv;
		void SetData(T pvoid, KeyType key) { TData = pvoid;dwKey = key; pNext = NULL; pPrv = NULL; }
	};

	BOOL				m_bInited;
protected:
	DWORD				m_dwDataNum;
	DWORD				m_dwMaxBucketNum;
	HashBucket **		m_ppBucketTable;

	/// for iterator
	HashList *			m_pListTail;
	HashList * 			m_pListHead;
	HashList *			m_pListCur;
	
	/// for bucket iterator
	HashBucket *		m_pCurBucket;
	HashBucket*			m_pLastBucket;
	KeyType				m_dwBucketKey;
#if SOLAR_HASH_TABLE_VERSION < 0x0110
	// memorypool
	CMemoryPoolFactory<HashBucket>	* m_pHashBucketPool;
	CMemoryPoolFactory<HashList>	* m_pHashListPool;
#endif
public:
	typedef typename T	object_type;

	SolarHashTable()
	{
		m_dwDataNum			= 0;
		m_dwMaxBucketNum	= 0;
		m_ppBucketTable		= NULL;
		m_pListHead			= NULL;
		m_pListTail			= NULL;
		m_pListCur			= NULL;
		m_pCurBucket		= NULL;
		m_bInited			= FALSE;
#if SOLAR_HASH_TABLE_VERSION < 0x0110
		m_pHashListPool		= NULL;
		m_pHashBucketPool	= NULL;
#endif
	}

	SolarHashTable( DWORD dwMaxBucketNum )
	{
		m_dwDataNum			= 0;
		m_dwMaxBucketNum	= 0;
		m_ppBucketTable		= NULL;
		m_pListHead			= NULL;
		m_pListTail			= NULL;
		m_pListCur			= NULL;
		m_pCurBucket		= NULL;
#if SOLAR_HASH_TABLE_VERSION < 0x0110
		m_pHashListPool		= NULL;
		m_pHashBucketPool	= NULL;
#endif
		m_bInited			= FALSE;
		Initialize( dwMaxBucketNum );
	}
	
	virtual ~SolarHashTable()
	{
		RemoveAll();
		if (m_ppBucketTable)
		{
			delete [] m_ppBucketTable;
			m_ppBucketTable = NULL;
		}
#if SOLAR_HASH_TABLE_VERSION < 0x0110
		if(m_pHashListPool)
		{
			m_pHashListPool->Release();
			delete m_pHashListPool;
			m_pHashListPool = NULL;
		}
		if( m_pHashBucketPool )
		{
			m_pHashBucketPool->Release();
			delete m_pHashBucketPool;
			m_pHashBucketPool = NULL;
		}
#endif
	}
	
	inline KeyType		GetDataNum() { return m_dwDataNum; }
	BOOL				Initialize(DWORD dwMaxBucketNum)
	{
		if( m_bInited ) return FALSE;
#if SOLAR_HASH_TABLE_VERSION < 0x0110
		ASSERT( NULL == m_pHashBucketPool );
		ASSERT( NULL == m_pHashListPool );
#endif
		m_dwDataNum			= 0;
		m_dwMaxBucketNum	= dwMaxBucketNum;
		m_ppBucketTable		= NULL;
		m_pListHead			= NULL;
		m_pListTail			= NULL;
		m_pListCur			= NULL;
		m_pCurBucket		= NULL;
#if SOLAR_HASH_TABLE_VERSION < 0x0110
		m_pHashBucketPool	= new CMemoryPoolFactory<HashBucket>;
		m_pHashListPool		= new CMemoryPoolFactory<HashList>;
		m_pHashBucketPool->Initialize( dwMaxBucketNum, dwMaxBucketNum/2+1, NULL, NULL );
		m_pHashListPool->Initialize( dwMaxBucketNum, dwMaxBucketNum/2+1, NULL, NULL );
#endif
		m_ppBucketTable = new HashBucket *[m_dwMaxBucketNum];
		
		if( !m_ppBucketTable )
			return FALSE;
		
		memset(m_ppBucketTable, 0, sizeof(HashBucket *) * m_dwMaxBucketNum);
		m_bInited			= TRUE;
		return TRUE;
		
	}
	/// don't check for the same key
	BOOL				Add(T TData, KeyType dwKey)
	{
		KeyType index = dwKey % m_dwMaxBucketNum;
		
		HashBucket *	cur = NULL;
		HashBucket *	prv = NULL;
		//HashBucket *	next = NULL;
		
		
		if (!m_ppBucketTable[index])
		{
#if SOLAR_HASH_TABLE_VERSION >= 0x0110
			HashBucket* node = TAllocNew(HashBucket);
            node->SetData(TData, dwKey);
            m_ppBucketTable[index] = node;
            node->pList = addList(node);
#else
			m_ppBucketTable[index] = (HashBucket*)m_pHashBucketPool->Alloc();
			m_ppBucketTable[index]->SetData( TData, dwKey );
			m_ppBucketTable[index]->pList = addList(m_ppBucketTable[index]);
#endif
		}
		else 
		{
			cur = m_ppBucketTable[index];
			while (cur)
			{
				prv = cur;
				cur = cur->pNext;
			}
#if SOLAR_HASH_TABLE_VERSION >= 0x0110
			cur = prv->pNext = TAllocNew(HashBucket);
#else
			cur = prv->pNext = (HashBucket*)m_pHashBucketPool->Alloc();
#endif
			cur->SetData( TData, dwKey );
			cur->pPrv = prv;
			cur->pList = addList(cur);
			
		}
		m_dwDataNum++;
		
		return TRUE;
	}
	
	T					GetData(KeyType dwKey)
	{
		KeyType index = dwKey % m_dwMaxBucketNum;
		
		HashBucket* pBucket = m_ppBucketTable[index];
		
		while(pBucket)
		{
			if (pBucket->dwKey == dwKey)
			{
				return pBucket->TData;
			}
			pBucket = pBucket->pNext;
		}
		return NULL;
		
	}
	
	T					GetHeadData() { if( !m_pListHead ) return NULL; return m_pListHead->pBucket->TData;	}
	T					GetTailData() { if( !m_pListTail ) return NULL; return m_pListTail->pBucket->TData;	}
	
	/// Iterate List
	inline void			SetFirst() { m_pListCur = m_pListHead; }
	T			 		GetNext()
	{
		if (m_pListCur)
		{
			const T& TData = m_pListCur->pBucket->TData;
			m_pListCur = m_pListCur->pNext;
			return TData;
		}

		return NULL;		
	}

	inline void			SetLast() { m_pListCur = m_pListTail;	}
	T			 		GetPrev()
	{
		if (m_pListCur)
		{
			const T& TData = m_pListCur->pBucket->TData;
			m_pListCur = m_pListCur->pPrv;
			return TData;
		}

		return NULL;		
	}


	
	/// Iterate Bucket Chain List
	void				SetBucketFirst(KeyType dwKey)
	{
		KeyType index = dwKey % m_dwMaxBucketNum;			
		m_dwBucketKey = dwKey;
		m_pCurBucket = m_ppBucketTable[index];
		m_pLastBucket = NULL;
	}		
	T					GetBucketNext()
	{
		while(m_pCurBucket)
		{
			if (m_pCurBucket->dwKey == m_dwBucketKey)
			{
				T	TData = m_pCurBucket->TData;
				m_pLastBucket = m_pCurBucket;
				m_pCurBucket = m_pCurBucket->pNext;
				return TData;
			}
			m_pCurBucket = m_pCurBucket->pNext;
		}
		return NULL;
	}
	void				RemoveCurBucketData()
	{
		KeyType dwIndex = m_dwBucketKey%m_dwMaxBucketNum;
		HashBucket*	cur = m_pLastBucket;
		HashBucket*	prv = NULL;
		HashBucket*	next = NULL;

		prv = cur->pPrv;
		next = cur->pNext;
		if (!prv)
			m_ppBucketTable[dwIndex] = next;
		else 
			prv->pNext = next;

		if (next)
			next->pPrv = prv;

		removeList(cur->pList);
#if SOLAR_HASH_TABLE_VERSION >= 0x0110
        TAllocDelete(HashBucket, cur);
#else
		m_pHashBucketPool->Free(cur);
#endif
		cur = NULL;
		--m_dwDataNum;
	}

	
	/// delete bucket, list for (dwKey)
	void				Remove(KeyType dwKey)
	{
		KeyType dwIndex = dwKey % m_dwMaxBucketNum;
		
		HashBucket*	cur = m_ppBucketTable[dwIndex];
		HashBucket*	prv = NULL;
		HashBucket*	next = NULL;
		
		while (cur)
		{
			if (cur->dwKey == dwKey)
			{
				prv = cur->pPrv;
				next = cur->pNext;

				if(cur == m_pCurBucket)			/// for iterator sync
					m_pCurBucket = next;

				if (!prv)
					m_ppBucketTable[dwIndex] = next;
				else 
					prv->pNext = next;
				
				if (next)
					next->pPrv = prv;
				
				removeList(cur->pList);
#if SOLAR_HASH_TABLE_VERSION >= 0x0110
                TAllocDelete(HashBucket, cur);
#else
				m_pHashBucketPool->Free(cur);
#endif
				cur = NULL;
				--m_dwDataNum;
				return;
			}
			cur = cur->pNext;
		}
	}

	T					RemoveHead()
	{
		HashList*	cur = m_pListHead;
		if( !cur ) return NULL;
		m_pListHead = m_pListHead->pNext;
		T data = cur->pBucket->TData;
#if SOLAR_HASH_TABLE_VERSION >= 0x0110
        TAllocDelete(HashBucket, cur->pBucket);
        TAllocDelete(HashList, cur);
#else
		m_pHashBucketPool->Free(cur->pBucket);	//delete cur->pBucket;
		m_pHashListPool->Free(cur);				//delete cur;
#endif
		--m_dwDataNum;

		return data;
	}
	/// delete all of Bucket, List Node
	void				RemoveAll()
	{
		HashList*	cur = m_pListHead;
		HashList*	next = NULL;
		
		while (cur)
		{
			next = cur->pNext;
#if SOLAR_HASH_TABLE_VERSION >= 0x0110
            TAllocDelete(HashBucket, cur->pBucket);
            TAllocDelete(HashList, cur);
#else
			m_pHashBucketPool->Free(cur->pBucket);	//delete cur->pBucket;
			m_pHashListPool->Free(cur);				//delete cur;
#endif
			cur = next;
		}
		m_dwDataNum = 0;
		m_pListHead = NULL;
		
		if(m_ppBucketTable)
			memset(m_ppBucketTable, 0, sizeof(HashBucket *) * m_dwMaxBucketNum);
	}
	
	//------------------------------------------------------------------------------
	// iterator 
	//------------------------------------------------------------------------------
	class iterator
	{
		friend class SolarHashTable;
		HashList * _current;

	public:
		iterator():_current(NULL){}
		iterator( const iterator & itr ):_current(itr._current){}
		~iterator(){}

		iterator & operator++()
		{
			if( _current ) _current = _current->pNext;
			return *this;
		}
		iterator & operator++( int i )
		{
			if( _current ) _current = _current->pNext;
			return *this;
		}
		iterator & operator--()
		{
			if( _current ) _current = _current->pPrv;
			return *this;
		}
		iterator & operator--( int i )
		{
			if( _current ) _current = _current->pPrv;
			return *this;
		}

		BOOL operator!=( iterator & it )
		{
			return ( it._current != _current );
		}
		BOOL operator==( iterator & it )
		{
			return ( it._current == _current );
		}
		T operator*()
		{
			if( _current ) return _current->pBucket->TData;
			return NULL;
		}
	};

	iterator find( KeyType dwKey )
	{
		KeyType index = dwKey % m_dwMaxBucketNum;

		HashBucket* pBucket = m_ppBucketTable[index];

		while(pBucket)
		{
			if (pBucket->dwKey == dwKey)
			{
				break;
			}
			pBucket = pBucket->pNext;
		}
		iterator it;
		if( pBucket )
			it._current = pBucket->pList;
		else
			it._current = NULL;
		return it;
	}

	iterator begin()
	{
		iterator it;
		it._current = m_pListHead;
		return it;
	}
	iterator end()
	{
		iterator it;
		it._current = NULL;
		return it;
	}
	KeyType size()
	{
		return GetDataNum();
	}
	void erase( iterator & it )
	{
		Remove( it._current->pBucket->dwKey );
	}
	void clear()
	{
		RemoveAll();
		ASSERT( 0 == GetDataNum() );
	}
	
protected:
	HashList *				addList(HashBucket * pBucket)
	{
		HashList*	cur = NULL;
		//HashList*	prv = NULL;
		//HashList*	next = NULL;
		
		if (!m_pListHead)
		{
#if SOLAR_HASH_TABLE_VERSION >= 0x0110
			m_pListCur = m_pListHead = TAllocNew(HashList);
#else
			m_pListCur = m_pListHead = (HashList*)m_pHashListPool->Alloc();
#endif
			m_pListCur->pPrv	= NULL;
			m_pListCur->pNext	= NULL;
			
			m_pListTail = m_pListCur;
			m_pListCur->pBucket = pBucket;
			pBucket->pList = m_pListHead;
			return m_pListHead;
		}
		//else
		{
#if SOLAR_HASH_TABLE_VERSION >= 0x0110
			cur = m_pListTail->pNext = TAllocNew(HashList);
#else
			cur = m_pListTail->pNext = (HashList*)m_pHashListPool->Alloc();
#endif
			cur->pNext	= NULL;
			cur->pPrv	= m_pListTail;
			m_pListTail = cur;
			cur->pBucket = pBucket;
			pBucket->pList = cur;
			return cur;
		}
		//return NULL;
	}
	void				removeList(HashList * pList)
	{
		
	//	HashList*	cur = pList;
		HashList*	prv = pList->pPrv;
		HashList*	next = pList->pNext;
		
		
		if(pList == m_pListCur)		/// for iterator sync
			m_pListCur = next;
		
		if (prv)
			prv->pNext = next;
		else 
			m_pListHead = next;
		
		if (next)
			next->pPrv = prv;
		else
			m_pListTail = prv;

#if SOLAR_HASH_TABLE_VERSION >= 0x0110
        TAllocDelete(HashList, pList);
#else
		m_pHashListPool->Free(pList);
#endif
		pList = NULL;
	}
	
};


} /// namespace util

//==================================================================================================
//==================================================================================================
//==================================================================================================
#endif //SOLAR_HASH_TABLE_VERSION < 0x0200
//==================================================================================================
//==================================================================================================
//==================================================================================================


#endif //UTILITY_SOLAR_HASH_TABLE_H
