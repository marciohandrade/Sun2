#ifndef __MISC_H__
#define __MISC_H__

//=============================================================================================================================
// Miscellaneous header files
//	- ����� define, ������ Ű����, ������ typedef, ���ϰ� �۾��ϱ� ���� ���� ��ũ��
//=============================================================================================================================





#define ROUND_DOWN( Size, Amount )  ((DBLENGTH)(Size) & ~((Amount) - 1))
#define ROUND_UP(   Size, Amount ) (((DBLENGTH)(Size) +  ((Amount) - 1)) & ~((Amount) - 1))

// ��� ����!
// max�� RAND_MAX(32767)���� �ʴ´�.
#define random(min, max)                 ((rand()%(int)(((max) + 1)-(min)))+ (min)) 
#define BOUND_CHECK(minValue, value, maxValue)		max( minValue, min( value, maxValue) )
#define _RAND_MAX 0x7fff
// 0 <= min <= seed < max ��!
#define dRandom(min, max)				((((double)rand()/(((double)(_RAND_MAX)+(double)(1)))) * (max-min))+(min))
#define random_percent(success_percent) ( random(1,100) <= success_percent ? TRUE : FALSE )

#define sSWITCH(a)		{										\
							switch((a))		{					\
							case -1:		{__debugbreak();

#define CASE(a)			} break;								\
						case (a):	{

#define CASE2(a,b)		} break;								\
						case (a): case (b):	{

#define DEFAULT			}										\
						break;									\
						default:	{
#define eSWITCH			} break;	}	}




//--------------------------------------------------------------------------------
// start ���� �޸� Ǯ ���� �ڵ�
//--------------------------------------------------------------------------------
#include <MemoryPoolFactory.h>

template<class _Ct, int nCount=1000>
class CUSTOMPOOL
{
public:
	CUSTOMPOOL(){ m_Pool.Initialize( nCount );	}
	~CUSTOMPOOL(){ m_Pool.Release();	}
	_Ct * Alloc() { return m_Pool.Alloc(); }
	VOID Free( _Ct * p ) { return m_Pool.Free(p); }
public:
	util::CMemoryPoolFactory<_Ct> m_Pool;
};

#define __DECL_CUSTOMPOOL_EX(_ClassTypem, Count)						\
	private:															\
	static CUSTOMPOOL<_ClassType,Count> s_Pool;							\
	public:																\
	static _ClassType *	ALLOC() { return s_Pool.Alloc(); }				\
	static VOID FREE( _ClassType * pClass ) { s_Pool.Free( pClass ); }\
	static VOID	DisplayerPoolInfo()										\
	{																	\
		DISPMSG( "[%4u,%4u][band:%u node:%u]", s_Pool.m_Pool.GetPoolBasicSize(), s_Pool.m_Pool.GetPoolExtendSize(),	\
		s_Pool.m_Pool.GetNumberOfBands(), s_Pool.m_Pool.GetAvailableNumberOfTypes() );	\
	}

#define __DECL_CUSTOMPOOL(_ClassType)									\
	private:															\
	static CUSTOMPOOL<_ClassType> s_Pool;								\
	public:																\
	static _ClassType *	ALLOC() { return s_Pool.Alloc(); }				\
	static VOID FREE( _ClassType * pClass ) { s_Pool.Free( pClass ); }\
	static VOID	DisplayerPoolInfo()										\
	{																	\
		DISPMSG( "[%4u,%4u][band:%u node:%u]", s_Pool.m_Pool.GetPoolBasicSize(), s_Pool.m_Pool.GetPoolExtendSize(),	\
		s_Pool.m_Pool.GetNumberOfBands(), s_Pool.m_Pool.GetAvailableNumberOfTypes() );	\
	}

#define __IMPL_CUSTOMPOOL_EX(_ClassType, Count)							\
	CUSTOMPOOL<_ClassType,Count> _ClassType::s_Pool;
#define __IMPL_CUSTOMPOOL(_ClassType)									\
	CUSTOMPOOL<_ClassType> _ClassType::s_Pool;

//--------------------------------------------------------------------------------
// end �޸� Ǯ ���� �ڵ�
//-------------------------------------------------------------------------------

// _NA001385_20090924_DOMINATION_BASE
template<typename T>
inline bool Between(const T& value, const T& rangeStart, const T& rangeEnd)
{
    //if(rangeStart > rangeEnd)
    //{
    //    std::swap(rangeStart, rangeEnd);
    //}
    return (rangeStart <= value) && (value <= rangeEnd);
}


#endif // __MISC_H__