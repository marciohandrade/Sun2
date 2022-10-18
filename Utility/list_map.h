#pragma once
#if SUN_CODE_BACKUP
/*---------------------------------------------------------------------------------------

	@Author : Waverix
	@Date : 2007-05-12
	@Reference
		- map
	@Remark 
		- 시작 동기는 재식씨가 SolarTrigger 구현시 요구된 검색과 순차 처리가 가능한
		자료구조가 요구됨을 알게 되어서이며, 이에 관련한 자료구조는 B-tree 계열이 필요하게
		되었음. 혹시나 해서 stl 찾아봤지만 역시나 없어서 map과 direct accessable linked list
		를 사용해 구현함
		- map 사용방법과 동일하며, 추가적인 인터페이스가 몇가지 있다.
		- 순차검색용 data_iterator가 존재하며, 이에 관련한 interface존재

	@Warning
		- 기본은 map을 따르기 때문에 순차검색용은 제한을 가지는 부분이 있다. 범위 연산과
		같은 것은 map기준으로 수행해야 한다
		- 값을 추가하는 경우, map은 본래의 로직대로 sorting되며, 순차리스트는 입력 순서에
		의한 FIFO 구조를 가진다.
	@Prototypes
		-> seq_node
		-> list_map (map상속)

	@Samples
		struct TestStruct
		{
			DWORD	dwA;
			DWORD	dwB;
		};

		typedef seq_node<DWORD, TestStruct>	LIST_MAP_DATA;
		typedef list_map<DWORD, TestStruct>	LIST_MAP;
		typedef LIST_MAP::iterator			LIST_MAP_IT;

		// Data 추가 관련
		static VOID Test3_LIST_MAP_SAMPLE2()
		{
			LIST_MAP listMap;

			TestStruct sA;

			// Data 추가 관련
			for( DWORD i=10 ; i<20 ; ++i )
			{
				DWORD ii = 0x409944F3*i + (0x1933F793+i);

				sA.dwA = ii;
				sA.dwB = i;

				LIST_MAP_IT it = listMap.insert( listMap.end(), make_pair( ii, LIST_MAP_DATA(ii, sA) ) );
				TestStruct& rV = it->second.second;
				// ...
			}

			for( DWORD i=10 ; i<20 ; ++i )
			{
				DWORD ii = 0x409944F3*i + (0x1933F793+i);

				TestStruct& rV = listMap[ii];
				sA.dwA = ii;
				sA.dwB = i;

				// ...
			}

			// map 기본 iterator 사용 예제
			LIST_MAP::iterator itAend = listMap.end();
			for( LIST_MAP::iterator it = listMap.begin() ; it != itAend ; ++it )
			{
				TestStruct& rV = it->second.second;
				printf ( "RV %u\n" , rV.dwB );
			}

			// list_map 전용 순차 data_iterator 사용 예제
			LIST_MAP::data_iterator itBend = listMap.seq_end();
			for( LIST_MAP::data_iterator it = listMap.seq_begin() ; it != itBend ; ++it )
			{
				TestStruct& rV = it->second;
				printf ( "RV %u\n" , rV.dwB );
			}
		}

//---------------------------------------------------------------------------------------
*/


#pragma once

#include "LinkedListCS.h"

#include <map>
#include <list>
//hash_map

namespace util
{
	template< class _Kty, class _Ty >
	struct seq_node : __LList
	{
		_Kty	first;
		_Ty		second;

		seq_node() { util::LList::Init( this ); }
		seq_node( _Kty k ) : first(k) { util::LList::Init( this ); }
		seq_node( const _Kty k, const _Ty& v ) : first(k), second(v) { util::LList::Init( this ); }
		~seq_node() { util::LList::Delete( this ); util::LList::Init( this ); }
	};


	template<
		class _Kty,
		class _Ty,
		class _Pr = less<_Kty>,
		class _Alloc = allocator< pair< const _Kty, seq_node< _Kty, _Ty > > >
		>
    class list_map : public std::map<_Kty, seq_node<_Kty, _Ty>, _Pr, _Alloc>
	{
	public:

		typedef seq_node<_Kty, _Ty>			_MyData;
		typedef list_map<_Kty, _Ty, _Pr, _Alloc>		_Myt;
		typedef map<_Kty, _MyData, _Pr, _Alloc>			_Mybase;

		typedef seq_node<_Kty, _Ty>			_node_type;
		typedef _Ty										mapped_type;

		class data_iterator;
	public:
		list_map()
			: _Mybase(key_compare(), allocator_type())
		{	// construct empty map from defaults
		}

		explicit list_map(const key_compare& _Pred)
			: _Mybase(_Pred, allocator_type())
		{	// construct empty map from comparator
		}

		list_map(const key_compare& _Pred, const allocator_type& _Al)
			: _Mybase(_Pred, _Al)
		{	// construct empty map from comparator and allocator
		}

		template<class _Iter>
		list_map(_Iter _First, _Iter _Last)
			: _Mybase(key_compare(), allocator_type())
		{	// construct map from [_First, _Last), defaults
			for (; _First != _Last; ++_First)
				this->insert(*_First);
		}

		template<class _Iter>
		list_map(_Iter _First, _Iter _Last,	const key_compare& _Pred)
			: _Mybase(_Pred, allocator_type())
		{	// construct map from [_First, _Last), comparator
			for (; _First != _Last; ++_First)
				this->insert(*_First);
		}

		template<class _Iter>
		list_map(_Iter _First, _Iter _Last, const key_compare& _Pred, const allocator_type& _Al)
			: _Mybase(_Pred, _Al)
		{	// construct map from [_First, _Last), comparator, and allocator
			for (; _First != _Last; ++_First)
				this->insert(*_First);
		}

		mapped_type& operator[](const key_type& _Keyval)
		{	// find element matching _Keyval or insert with default mapped
			iterator _Where = this->lower_bound(_Keyval);
			if (_Where == this->end() || this->comp(_Keyval, this->_Key(_Where._Mynode())))
				_Where = this->insert(_Where, value_type(_Keyval, _node_type(_Keyval)) );
			return ((*_Where).second).second;
		}

		iterator insert(const value_type& _Val)
		{	// insert a {key, mapped} value
			iterator& itF = _Mybase::find( _Val.first );
			if( itF != end() )
				return itF;

			iterator& rIT = (_Mybase::insert(_Val).first);
			util::LList::AddPrev( rIT->second, &_m_SeqListEntry );
			return rIT;
		}

		iterator insert(iterator _Where, const value_type& _Val)
		{	// insert a {key, mapped} value, with hint
			iterator& itF = _Mybase::find( _Val.first );
			if( itF != end() )
				return itF;

			iterator& rIT = (_Mybase::insert(_Where, _Val));
			util::LList::AddPrev( &(rIT->second), &_m_SeqListEntry );
			return rIT;
		}

		template<class _Iter>
		void insert(_Iter _First, _Iter _Last)
		{	// insert [_First, _Last), arbitrary iterators
			for (; _First != _Last; ++_First)
				insert(*_First);
		}

		iterator erase(iterator _Where)
		{	// erase element at _Where
			return _Mybase::erase( _Where );
		}

		iterator erase(iterator _First, iterator _Last)
		{	// erase [_First, _Last)
			return _Mybase::erase( _First, _Last );
		}

		size_type erase(const key_type& _Keyval)
		{	// erase and count all that match _Keyval
			return _Mybase::erase( _Keyval );
		}

		void erase(const key_type *_First, const key_type *_Last)
		{	// erase all that match array of keys [_First, _Last)
			return _Mybase::erase( _First, _Last );
		}

		// data_iterator 기준으로 next iterator를 반환한다.
		data_iterator erase( data_iterator _DataWhere )
		{	// erase element at _DataWhere
			key_type key = _DataWhere._m_pNode->first;
			data_iterator ret = (_MyData*)_DataWhere._m_pNode->next;
			_Mybase::erase(key);
			return ret;
		}

		data_iterator		seq_begin()
		{
			return data_iterator((_MyData*)(_m_SeqListEntry.next));
		}

		data_iterator		seq_end()
		{
			return data_iterator((_MyData*)&(_m_SeqListEntry));
		}

		class data_iterator
		{	// iterator for mutable _Seq LinkedList
			friend class list_map;
		public:
			typedef _MyData* pointer;
			typedef _MyData& reference;

			data_iterator() : _m_pNode(NULL)
			{	// construct with null node pointer
			}

			data_iterator( const _MyData* pNode ) : _m_pNode((_MyData*)pNode)
			{
			}

			reference operator*() const
			{	// return designated value
				return (*_m_pNode);
			}

			pointer operator->() const
			{	// return pointer to class object
				return &(*_m_pNode);
			}

			data_iterator& operator++()
			{	// preincrement
				_m_pNode = (_MyData*)_m_pNode->next;
				return (*this);
			}

			data_iterator operator++(int)
			{	// postincrement
				data_iterator _Tmp = *this;
				_m_pNode = (_MyData*)_m_pNode->next;
				return (_Tmp);
			}

			data_iterator& operator--()
			{	// predecrement
				_m_pNode = (_MyData*)_m_pNode->prev;
				return (*this);
			}

			data_iterator operator--(int)
			{	// postdecrement
				data_iterator _Tmp = *this;
				_m_pNode = (_MyData*)_m_pNode->prev;
				return (_Tmp);
			}

			bool operator==(const data_iterator& _Right) const
			{	// test for iterator equality
				return (_m_pNode == _Right._m_pNode);
			}

			bool operator!=(const data_iterator& _Right) const
			{	// test for iterator inequality
				return (!(*this == _Right));
			}
		protected:
			_MyData*			_m_pNode;
		};

	protected:
		_MyData				_m_SeqListEntry;
	};
}; // namespace util

#endif //SUN_CODE_BACKUP