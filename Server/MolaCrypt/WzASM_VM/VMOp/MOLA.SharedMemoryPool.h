#pragma once

#if SUN_CODE_BACKUP
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
class MOLASharedMemoryPool
{
public:
			template<typename _TYPE>
			static _TYPE*		InstanceOf()
			{
				PVOID pAllocPtr = MOLASharedMemoryPool::MemoryAllocate( sizeof(_TYPE) );
				return new (pAllocPtr) _TYPE;
			};

			template<typename _TYPE>
			static VOID			DeleteOf( _TYPE* ptr )
			{
				ptr->_TYPE::~_TYPE();
				MOLASharedMemoryPool::MemoryDeAllocate( ptr );
			}

			static PVOID				MemoryAllocate( SIZE_T sz );
			static VOID					MemoryDeAllocate( PVOID ptr );

			static VOID					CreateInstance();
			static VOID					DestroyInstance();
private:
			MOLASharedMemoryPool();
			~MOLASharedMemoryPool();

private:	static MOLASharedMemoryPool*	ms_pInstance;
			static INT						ms_iCounts;
			PVOID							m_pInterBlock;
};

#endif //SUN_CODE_BACKUP