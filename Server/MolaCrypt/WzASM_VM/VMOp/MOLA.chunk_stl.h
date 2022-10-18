#pragma once

#if SUN_CODE_BACKUP

#ifndef __BLOCKALLOCATOR_IN_MOLA_H
#define __BLOCKALLOCATOR_IN_MOLA_H

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <DEFINITIONs>
#define VERSION_BLOCKALLOCATOR_IN_MOLA	0x00010004

#include <stddef.h>
#include <assert.h>

#undef BLOCKALLOCATOR_MSVC_5_6
#if defined(_MSC_VER) && _MSC_VER<1300
#	define BLOCKALLOCATOR_MSVC_5_6
#endif

#include <vector>
#include <set>
#include <list>
#include <map>
#include <hash_map>

#include "MOLA.SharedMemoryPool.h"

using namespace std;
using namespace stdext;

namespace molautil
{
#pragma pack( push, 4 )
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// <CHUNK ALLOCATOR>
//
	template<typename T, typename N=T>
	class sBLK_ALLOCATOR
	{
#ifdef BLOCKALLOCATOR_MSVC_5_6
		typedef N node_type;
#else
		typedef T node_type;
#endif

	public:
		typedef SIZE_T		size_type;
		typedef ptrdiff_t	difference_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef T			value_type;

#ifndef BLOCKALLOCATOR_MSVC_5_6
		template<typename Other>
		struct rebind
		{
			typedef sBLK_ALLOCATOR<Other> other;
		};
#endif

		pointer				address(reference x) const			{ return &x; }
		const_pointer		address(const_reference x) const	{ return &x; }

		sBLK_ALLOCATOR()
		{
		}

#ifdef BLOCKALLOCATOR_MSVC_5_6
		sBLK_ALLOCATOR(const sBLK_ALLOCATOR&)
		{
		}
#else
		template<typename T1, typename N1>
		sBLK_ALLOCATOR(const sBLK_ALLOCATOR<T1, N1>&)
		{
		}
#endif

		sBLK_ALLOCATOR<T, N>& operator = ( const sBLK_ALLOCATOR<T, N>& )
		{
			return *this;
		}

#ifndef BLOCKALLOCATOR_MSVC_5_6
		pointer allocate( size_type n, const VOID* hint = 0 )
		{
			//assert(n==1);
			//SIZE_T sz1 = sizeof(T);
			//printf( "%u\t%u\n", n, sz1 );
			return reinterpret_cast<pointer>( MOLASharedMemoryPool::MemoryAllocate(sizeof(T)*n) );
		}
#endif

		VOID deallocate( VOID* p, size_type n )
		{
			MOLASharedMemoryPool::MemoryDeAllocate( p );
		}

		VOID construct( pointer p, const T& val )
		{
			new ((VOID *)p) T(val);
		}

		VOID destroy( pointer p )
		{
			p->T::~T();
		}

		size_type max_size() const
		{
			size_type n = (size_type)(-1)/sizeof(node_type);
			return ( 0<n ? n : 1 );
		}

		bool operator == ( const sBLK_ALLOCATOR<T, N>& r ) const
		{
			return this == &r;
		}

		bool operator != ( const sBLK_ALLOCATOR<T, N>& r ) const
		{
			return this != &r;
		}

		bool operator < ( const sBLK_ALLOCATOR<T, N> & ) const;
		bool operator > ( const sBLK_ALLOCATOR<T, N> & ) const;

	private:
	};


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<chunk_list>
//		chunk memory version of list
//
//#pragma warning(disable:4786)
//#include <list>

	template< typename T >
	class chunk_vector : public vector< T, sBLK_ALLOCATOR< T > >
	{
		typedef typename vector< T, sBLK_ALLOCATOR< T > > super;

	public:
		typedef typename super::allocator_type		allocator_type;
		typedef typename super::size_type			size_type;
		typedef typename super::const_iterator		const_iterator;

		explicit chunk_vector( const allocator_type& al = allocator_type() ) : super(al)
		{
		}

		explicit chunk_vector(
			size_type n, const T& v = T(),
			const allocator_type& al = allocator_type()
			) : super( n, v, al )
		{
		}

		chunk_vector(
			const_iterator first, const_iterator last,
			const allocator_type& al = allocator_type()
			) : super( first, last, al )
		{
		}
	};

	template<typename T>
	struct chunk_list_node
	{
		VOID*	p1;
		VOID*	p2;
		T		t;
	};

	template< typename T >
	class chunk_list : public list<T, sBLK_ALLOCATOR< T, chunk_list_node<T> > >
	{
		typedef typename list
			<
				T,
				sBLK_ALLOCATOR< T, chunk_list_node<T> >
			> super;

	public:
		typedef typename super::allocator_type		allocator_type;
		typedef typename super::size_type			size_type;
		typedef typename super::const_iterator		const_iterator;

		explicit chunk_list( const allocator_type& al = allocator_type() ) : super(al)
		{
		}

		explicit chunk_list(
							size_type n, const T& v = T(),
							const allocator_type& al = allocator_type()
							) : super( n, v, al )
		{
		}

		chunk_list(
							const_iterator first, const_iterator last,
							const allocator_type& al = allocator_type()
							) : super( first, last, al )
		{
		}
	};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<chunk_set>
//		chunk memory version of set
//
//#pragma warning(disable:4786)
//#include <set>

	template <typename T>
	struct chunk_set_node
	{
		VOID	*p1, *p2, *p3;
		T		t;
		INT		i;
	};

	template < typename Key, typename Pred = less<Key> >
	class chunk_set : public set< Key, Pred, sBLK_ALLOCATOR< Key, chunk_set_node<Key> > >
	{
		typedef typename set< Key, Pred, sBLK_ALLOCATOR< Key, chunk_set_node<Key> > > super;

	public:
		typedef typename super::allocator_type	allocator_type;
		typedef typename super::value_type		value_type;

		explicit
		chunk_set(
					const Pred& comp = Pred(),
					const allocator_type& al = allocator_type()
					) : super( comp, al )
		{
		}

		chunk_set(
					const value_type* first, const value_type* last, const Pred& comp = Pred(),
					const allocator_type& al = allocator_type()
					) : super( first, last, comp, al )
		{
		}
	};


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<chunk_multiset>
//		chunk memory version of multiset
//
//#pragma warning(disable:4786)
//#include <set>

	template <typename T> struct chunk_multiset_node
	{
		VOID	*p1, *p2, *p3;
		T		t;
		INT		i;
	};


	template< typename Key, typename Pred=less<Key> >
	class chunk_multiset : public multiset< Key, Pred, sBLK_ALLOCATOR< Key, chunk_multiset_node<Key> > >
	{
		typedef typename multiset< Key, Pred, sBLK_ALLOCATOR< Key, chunk_multiset_node<Key> > > super;

	public:
		typedef typename super::allocator_type allocator_type;
		typedef typename super::value_type     value_type;

		explicit
		chunk_multiset(
						const Pred& comp=Pred(),
						const allocator_type& al=allocator_type()
						) : super(comp,al)
		{
		}

		chunk_multiset(
						const value_type* first,const value_type* last, const Pred& comp=Pred(),
						const allocator_type& al=allocator_type()
						) : super( first, last, comp, al )
		{
		}
	};



//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<chunk_map>
//		chunk memory version of map
//
//#pragma warning(disable:4786)
//#include <map>

	template <typename Key, typename T>
	struct chunk_map_node
	{
		VOID				*p1, *p2, *p3;
		pair<Key,T>			t;
		INT					i;
	};

	template< typename Key, typename T, typename Pred = less<Key> >
	class chunk_map : public map< Key, T, Pred, sBLK_ALLOCATOR< T, chunk_map_node<Key, T> > >
	{
		typedef typename map< Key, T, Pred, sBLK_ALLOCATOR< T, chunk_map_node<Key,T> > > super;

	public:
		typedef typename super::allocator_type	allocator_type;
		typedef typename super::value_type		value_type;

		explicit chunk_map(
								const Pred& comp = Pred(),
								const allocator_type& al = allocator_type()
								) : super( comp, al )
		{
		}

		chunk_map(
						const value_type *first, const value_type *last, const Pred& comp = Pred(),
						const allocator_type& al = allocator_type()
						) : super( first, last, comp, al )
		{
		}
	};


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<chunk_multimap>
//		chunk memory version of multimap
//
//#pragma warning(disable:4786)
//#include <map>

	template< typename Key, typename T >
	struct chunk_multimap_node
	{
	VOID            *p1, *p2, *p3;
	pair<Key,T>		t;
	INT				i;
	};

	template< typename Key, typename T, typename Pred=less<Key> >
	class chunk_multimap : public multimap< Key, T, Pred, sBLK_ALLOCATOR<T, chunk_multimap_node<Key, T> > >
	{
		typedef typename multimap< Key, T, Pred, sBLK_ALLOCATOR< T, chunk_multimap_node<Key, T> > > super;

	public:
		typedef typename super::allocator_type allocator_type;
		typedef typename super::value_type     value_type;

		explicit chunk_multimap(
								const Pred& comp=Pred(),
								const allocator_type& al=allocator_type()
								) : super(comp, al)
		{
		}

		chunk_multimap(
								const value_type* first, const value_type* last, const Pred& comp=Pred(),
								const allocator_type& al=allocator_type()
								) : super( first, last, comp, al )
		{
		}
	};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<chunk_hash_map>
//		chunk memory version of hash_map
//
//#pragma warning(disable:4786)
//#include <map>

	template< typename Key, typename T >
	struct chunk_hash_map_node
	{
		VOID            *p1, *p2, *p3;
		pair<Key,T>		t;
		INT				i;
	};
//typename _Tr = _STDEXT hash_compare<_Kty, less<_Kty> >
	template< typename Key, typename T, typename Pred=_STDEXT hash_compare<Key, less<Key> > >
	class chunk_hash_map : public hash_map< Key, T, Pred, sBLK_ALLOCATOR<T, chunk_hash_map_node<Key, T> > >
	{
		typedef typename hash_map< Key, T, Pred, sBLK_ALLOCATOR< T, chunk_hash_map_node<Key, T> > > super;

	public:
		typedef typename super::allocator_type allocator_type;
		typedef typename super::value_type     value_type;

		explicit chunk_hash_map(
			const Pred& comp=Pred(),
			const allocator_type& al=allocator_type()
			) : super(comp, al)
		{
		}

		chunk_hash_map(
			const value_type* first, const value_type* last, const Pred& comp=Pred(),
			const allocator_type& al=allocator_type()
			) : super( first, last, comp, al )
		{
		}
	};


#pragma pack(pop)
}; // namespace molautil

#elif VERSION_BLOCKALLOCATOR_IN_MOLA!=0x00010004
#	error You have included two BLOCKALLOCATOR.H with different version numbers
#endif

#endif //SUN_CODE_BACKUP