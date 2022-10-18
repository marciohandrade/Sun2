#pragma once

#include <boost/pool/pool.hpp>
#include "SunSTL.UtiltiyTMP.h"


// 고정형 크기 메모리 풀 사용
//#ifndef _DEBUG
#define USE_FIXED_SIZE_MEMORY_POOL
//#endif


namespace memory {

////////////////////////////////////////////////////////////////////////////////////////////////////
// 고정형 크기 메모리 풀
////////////////////////////////////////////////////////////////////////////////////////////////////

class NoParent
{
};

// 다형성 지원 버전
template <typename OBJECT_T, typename PARENT_T = NoParent>
class FixedSizeMemoryPool
{
public:
    FixedSizeMemoryPool() : 
        impl_(sizeof(OBJECT_T))
    {
    }
    ~FixedSizeMemoryPool()
    {
    }

public:
    void ReleaseAll()
    {
        impl_.release_memory();
    }

    void* Malloc()
    {
        return impl_.malloc();
    }
    void Free(void* const ptr)
    {
        impl_.free(ptr);
    }

    OBJECT_T* New()
    {
        OBJECT_T* alloc = (OBJECT_T*)impl_.malloc();
        if (alloc != NULL)
        {
            new(alloc) OBJECT_T();
        }
        return alloc;
    }
    void Delete(OBJECT_T* const ptr)
    {
        if (ptr != NULL)
        {
            ptr->~OBJECT_T();        
            impl_.free(ptr);
        }
    }
    void DeleteByParent(PARENT_T* const parent_ptr)
    {
        OBJECT_T* const ptr = dynamic_cast<OBJECT_T*>(parent_ptr);
        if (ptr == NULL)
        {
            return;
        }
        ptr->~OBJECT_T();        
        impl_.free(ptr);
    }

private:
    typedef boost::pool<> PoolImpl;
    PoolImpl impl_;

}; // FixedSizeMemoryPool

// 다형성 지원하지 않는 버전
template <typename OBJECT_T>
class FixedSizeMemoryPool<OBJECT_T, NoParent>
{
public:
    FixedSizeMemoryPool() : 
        impl_(sizeof(OBJECT_T))
    {
    }
    ~FixedSizeMemoryPool()
    {
    }

public:
    void ReleaseAll()
    {
        impl_.release_memory();
    }

    void* Malloc()
    {
        return impl_.malloc();
    }
    void Free(void* const ptr)
    {
        impl_.free(ptr);
    }

    OBJECT_T* New()
    {
        OBJECT_T* alloc = (OBJECT_T*)impl_.malloc();
        if (alloc != NULL)
        {
            new(alloc) OBJECT_T();
        }
        return alloc;
    }
    void Delete(OBJECT_T* const ptr)
    {
        if (ptr != NULL)
        {
            ptr->~OBJECT_T();        
            impl_.free(ptr);
        }
    }

private:
    typedef boost::pool<> PoolImpl;
    PoolImpl impl_;

}; // FixedSizeMemoryPool

} // memory


//==================================================================================================
extern void _SetSharedMemoryLeakDetect(void (*userhook)(DWORD));
extern BOOL TraceShareMemoryLeak(const char* const path_file_name);
extern void TestUnit_UsedLogOutput();
//==================================================================================================
//  <SharedMemoryPool>
//  <Usage>
//      Init - EntryPoint 재설정 후 첫번째 라인에 SharedMemoryPool::CreateInstance()를 호출해 주면 끝
//  <HISTORY>
//      07/09/07 - CheckIn (Waverix)
//      11/07/05 - upgrade to v0250
class SharedMemoryPool
{
public:
    static const ulong kVersion = 0x0250;
    //
    static void* MemoryAllocate(size_t sz, const char* name, const int line_no);
    static void  MemoryDeAllocate(void* ptr);
    static void* RedirectNew_MemoryAllocate(size_t size,
        const char* name, const int line_no, SharedMemoryPool*);
    static void  RedirectNew_MemoryDeallocate(void* ptr);
    // NOTE: f110708.5L, added the token validation routine
    static bool CheckValidAllocatedMemory(void* ptr);

    static bool  CreateInstance();
    static void  DestroyInstance();
    static bool CheckInstanced();
private:
    static const DWORD kInstancedTag = 0x3A000D12;
    //
    SharedMemoryPool();
    ~SharedMemoryPool();
    static SharedMemoryPool* Instance();
    //
    const DWORD instanced_tag_;
    void* inter_block_;
    //
    static int referenced_counter_s;
    friend class SharedMemoryRouter;
    friend BOOL TraceShareMemoryLeak(const char* const path_file_name);
    friend void TestUnit_UsedLogOutput();
    __DISABLE_COPY(SharedMemoryPool);
};

#define DOWNCASTER_SELECT	::util::downcaster_select
//#ifdef _DEBUG
//	#define DOWNCASTER_SELECT	dynamic_cast
//#else
//	#define DOWNCASTER_SELECT	static_cast
//#endif

template< typename _TYPE >
struct sun_type_info
{
	typedef _TYPE						value_type;
	typedef value_type*					pointer;
	typedef value_type* const			pointer_r;
	typedef const value_type*			const_pointer;
	typedef value_type&					reference;
};

//==================================================================================================

#if SUN_CODE_BACKUP
//==================================================================================================
// CHANGES: f110318.3L, reserved deletion

template< typename _TYPE >
struct TAllocator : sun_type_info< _TYPE >
{
	typedef TAllocator<value_type>		allocator_type;
	static pointer						Allocate()
	{
#if defined(_SERVER)
		return SharedMemoryPool::InstanceOf<value_type>();
#else
		return new value_type();
#endif
	}

	template< typename _DTYPE >
	static void							DeAllocate( _DTYPE* ptr )
	{
		// compile-time check routine
		static_cast<_DTYPE*>( reinterpret_cast<value_type*>(1) );

#if defined(_SERVER)
		SharedMemoryPool::DeleteOf< _DTYPE >( ptr ) ;
#else
		delete ptr;
#endif
	} 

	static void							DeAllocate( pointer ptr )
	{
#if defined(_SERVER)
		SharedMemoryPool::DeleteOf<value_type>( ptr );
#else
		delete ptr;
#endif
	}
};
#endif // SUN_CODE_BACKUP

//==================================================================================================

#define USING_SUN_SHARED_POOL   (1)
#if !defined(_MOLA_LIB)
    #define USING_SUN_SHARED_POOL_EXCEPT_MALLOC
#endif 

#if defined(_SERVER) || !defined(_SUNGAME_VER)
    #define USING_SUN_SHARED_POOL_REPORT (1)
#else
    #define USING_SUN_SHARED_POOL_REPORT (0)
#endif

//==================================================================================================

// CHANGES: f110318.3L, interfaces (like a TAllocNew, TAllocDelete) showing the below codes is made
// to replace 'TAllocator<T>' interface that is manipulated by using shared memory pool.
// because 'Apply Code Changes' function of the visual studio
// can't interpret template because of ambigous.
#if USING_SUN_SHARED_POOL

#ifdef USING_SUN_SHARED_POOL_EXCEPT_MALLOC

    #define TAllocMalloc(_size_) \
        ::malloc(_size_)

    #define TAllocFree(_ptr_) { \
        ::free(_ptr_); \
    }

#else
  #if USING_SUN_SHARED_POOL_REPORT
    #define TAllocMalloc(_size_) \
        SharedMemoryPool::MemoryAllocate(_size_, __FUNCTION__, __LINE__)
  #else
    #define TAllocMalloc(_size_) \
        SharedMemoryPool::MemoryAllocate(_size_, NULL, 0)
  #endif
    #define TAllocFree(_ptr_) { \
            SharedMemoryPool::MemoryDeAllocate(_ptr_); \
        }
#endif //USING_SUN_SHARED_POOL_ONLY_TALLOCNEW

//#ifdef USE_FIXED_SIZE_MEMORY_POOL
//
//    #define TAllocNew(OBJECT_T_) \
//        new (FixedSizeMemoryPool<##OBJECT_T_>::Instance().Malloc())##OBJECT_T_
//    
//    #define TAllocDelete(OBJECT_T_, INSTANCE_) { \
//        typedef sun_type_info<##OBJECT_T_>::value_type InstanceType; \
//        BOOST_STATIC_ASSERT((boost::is_same<InstanceType, ##OBJECT_T_ >::value)); \
//        static_cast<##OBJECT_T_*>(INSTANCE_)->InstanceType::~InstanceType(); \
//        FixedSizeMemoryPool<##OBJECT_T_>::Instance().Free(##INSTANCE_); }
//
//#else // USE_FIXED_SIZE_MEMORY_POOL

  #if USING_SUN_SHARED_POOL_REPORT
    #define TAllocNew(_type_)   \
        new (SharedMemoryPool::MemoryAllocate(sizeof(sun_type_info<##_type_>::value_type), \
            __FUNCTION__, __LINE__)) ##_type_
  #else
    #define TAllocNew(_type_)   \
        new (SharedMemoryPool::MemoryAllocate(sizeof(sun_type_info<##_type_>::value_type), \
            NULL, 0)) ##_type_
  #endif
    #define TAllocDelete(_type_, _instance_) { \
        typedef sun_type_info<##_type_>::value_type _typeof_instance_; \
        BOOST_STATIC_ASSERT((boost::is_same<_typeof_instance_, ##_type_ >::value));\
        static_cast<##_type_*>(_instance_)->_typeof_instance_::~_typeof_instance_(); \
        SharedMemoryPool::MemoryDeAllocate(_instance_); }

//#endif // USE_FIXED_SIZE_MEMORY_POOL

#else // USING_SUN_SHARED_POOL

    #define TAllocMalloc(_size_) \
        ::malloc(_size_)

    #define TAllocFree(_ptr_) { \
            ::free(_ptr_); \
        }

    #define TAllocNew(_type_)   \
        new sun_type_info<##_type_>::value_type

    #define TAllocDelete(_type_, _instance_) \
        {  static_cast<##_type_*>(_instance_); \
            typedef sun_type_info<##_type_>::value_type _typeof_instance_t; \
            delete (_instance_); }

#endif // USING_SUN_SHARED_POOL

//==================================================================================================
