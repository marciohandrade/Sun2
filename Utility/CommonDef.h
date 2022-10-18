#pragma once

// ��� ������Ʈ �� ���� ���� ��ġ�� �����̶�� �����Ͽ� �����Ǵ� ��� ��ġ�� ���� Include
#include "../include/Global.Define.h"

#ifndef INOUT
#	define INOUT
#endif //


#define __DISABLE_COPY(className)					\
		private :									\
		className(const className&);				\
		void	operator = (const className&)

#define	__UNUSED(variable)		variable
// NOTE: to remove meanless warnings, this macro same as __UNUSED
#define __TOUCHED(variable)     variable

#define __INFINITE_LOOP(name)	BOOL b##name##Loop = TRUE;	while (b##name##Loop)

// NOTE: use this tag to disable warning LNK4221 when a code file has no public symbols
#define __EMPTY_FILE_TAG()  namespace { char __EmptyFile_DisableWarning_LNK4221_##__LINE__; }
#define COMPILE_ASSERTION_CHECK_METHOD_IMPL() namespace { void __ComplieAssertionChecker_##__LINE__()
#define COMPILE_ASSERTION_CHECK_METHOD_END() }

//=============================================================================================================================
// SafeDeleter
//=============================================================================================================================

#ifdef SAFE_DELETE
	#undef SAFE_DELETE
#endif	

#ifdef SAFE_DELETE_ARRAY
	#undef SAFE_DELETE_ARRAY
#endif

#define SAFE_DELETE( p)				{ if ( p) { delete ( p); ( p) = NULL; } }
#define SAFE_DELETE_ARRAY( p)		{ if ( p) { delete[] ( p); ( p) = NULL; } }
#define SAFE_RELEASE( p)			{ if ( p) { ( p)->Release(); ( p) = NULL; } }
#define	SAFE_RELEASENDELETE(p)				if (p) { (p)->Release(); delete (p); (p) = NULL; }

class SafeDeleter
{
public :
	template <class T>
	void	operator () (T*& ptr) const
	{
		SAFE_DELETE( ptr );
	}
};

class PairDeleter
{
public :
	template < class pair_type >
	void operator () ( pair_type & pair ) const
	{
		SAFE_DELETE( pair.second );
	}
};


template <class T> 
struct Param2Class
{
	typedef T ClassType;
};

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	<PROPERTIES>
//		@author : waverix
//		@remark :
//			PROPERTY<_TYPE>
//				= ���� ��� IO ó����, complexŸ�� ����
//			PROPERTY_ARRAY<_TYPE, _INITVAL, _ARRAYCNT>
//				= �迭 ��� IO ó����, complexŸ�� ������ (complexŸ���� ����ϱ� ���ؼ��� �����Ϳ� ���� Ÿ������ ����� ��
//		@notes :
//				- 07/10/09 - �������� ���� �˻縦 ��ȭ�ϰ�, ������Ƽ�μ��� ������ ��ȭ���״�.
//							������ �������̽��� �ʵ��� ��ö�Ǵ� �Ÿӵ� ��ü�� ����ϱ� ���ٴ�
//							�̷������� �и����� ������ ���� ���� ��������, ���ȭ�� �� ������ �� ���̴�.
//				- 07/10/09 - <����> PROPERTY���� GetRef, operator _TYPE& �� complexŸ�� ������ ���� const������ ����.
//							<����>	PROPERTY_ARRAY���� GetPtr�� const�μ��� ������ ��Ű�� ���� �����ϴ�. <�б� ����>
//				- 07/10/09 - <����> operator = �迭�� �������� �ʾҽ��ϴ�.
//							�Ҵ翡 �����ؼ��� ���� Ÿ�� ����� ���� ���������� ���� ������ �ߴ��� ������ ���� �� �ֽ��ϴ�.
//							<����> ������ ������ ���˴ϴ�. �ݴ��� ������ ������ �ʽ��ϴ�.
//								PROPERTY<DWORD>	autovariable;
//								DWORD tVal = autovariable;
//								DWORD& rVal = autovariable;
//		@history :
//			07/10/09 : designed by waverix
//		@sample :
//			class TestObject
//			{
//			public:		PROPERTY<DWORD>					OBJECT_KEY;
//						PROPERTY_ARRAY<BYTE, 0, 16>		STATES;
//			};
//
//			{
//				TestObject* pTestObject;
//				pTestObject->OBJECT_KEY.Set( 10 );
//				pTestObject->OBJECT_KEY.Get();
//
//				// STATES�� �̸� ������ ��� 0���� �ʱ�ȭ �Ǿ� ����.
//				pTestObject->STATES.SetValue( 10, 100 );
//				BYTE byResultIs_100 = pTestObject->STATES.Get( 10 );
//				pTestObject->STATES.SetValue( 16, 100 );	// overflow, no-action
//				BYTE byResultIs_0 = pTestObject->STATES.Get( 16 );
//			}
//			
class PROPERTY_BASE
{
public:		PROPERTY_BASE() {}
			~PROPERTY_BASE() {}
};

template<typename _TYPE>
class PROPERTY : public PROPERTY_BASE
{
private:	typedef _TYPE					node_type;
			typedef PROPERTY<node_type>		this_type;

public:		PROPERTY() {}
			explicit PROPERTY( const node_type& t ) : m_FIELD(t) {}
			~PROPERTY() {}

public:		inline operator const node_type& () const		{ return m_FIELD; }
			inline node_type	Get() const					{ return m_FIELD; }
			inline VOID			Set( node_type IN field )	{ m_FIELD = field; }

private:	node_type			m_FIELD;

private:	this_type& operator = ( const node_type& );
			//this_type operator = ( const this_type& );
};

template<typename _TYPE>
class PROPERTY<typename _TYPE*> : public PROPERTY_BASE
{
private:	typedef _TYPE					node_type;
			typedef node_type*				pointer;
			typedef node_type* const		pointer_r;
			typedef PROPERTY<pointer>		this_type;
public:		PROPERTY() {}
			explicit PROPERTY( const node_type& t ) : m_FIELD(t) {}
			~PROPERTY() {}

public:		inline operator const pointer_r () const		{ return m_FIELD; }
			inline pointer		Get() const					{ return m_FIELD; }
			inline VOID			Set( pointer IN field )		{ m_FIELD = field; }
			inline pointer_r 	operator-> ()				{ return m_FIELD; }
			inline operator bool() const					{ return !!m_FIELD; }
			inline bool operator == ( const this_type& rhs ) const	{ return m_FIELD == rhs.m_FIELD; }
			inline bool operator == ( const pointer& rhs ) const	{ return m_FIELD == rhs; }
			inline bool operator != ( const this_type& rhs ) const	{ return m_FIELD != rhs.m_FIELD; }
			inline bool operator != ( const pointer& rhs ) const	{ return m_FIELD != rhs; }

private:	pointer				m_FIELD;
private:	this_type& operator = ( const node_type& );
			this_type& operator = ( const pointer& );
};


template<typename _TYPE, typename _TYPE _INITVAL, typename DWORD _ARRAYCNT=1>
class PROPERTY_ARRAY : public PROPERTY_BASE
{
private:	typedef _TYPE											node_type;
			typedef PROPERTY_ARRAY<node_type, _INITVAL, _ARRAYCNT>	this_type;

public:		PROPERTY_ARRAY() { Clear(); }
			~PROPERTY_ARRAY() {}
private:	PROPERTY_ARRAY( const this_type& t );
			this_type& operator = ( const this_type& );

public:		inline const _TYPE*	GetPtr() const				{ return m_pFIELD; }
			inline _TYPE		Get( DWORD dwIDX ) const
			{
				if( _ARRAYCNT > dwIDX )
					return m_pFIELD[dwIDX];
#ifdef _DEBUG
				__debugbreak();
#endif
				return _INITVAL;
			}
			inline BOOL			Set( DWORD dwIDX, _TYPE IN field )
			{
				if( _ARRAYCNT > dwIDX )
				{
					m_pFIELD[dwIDX] = field;
					return TRUE;
				}
#ifdef _DEBUG
				__debugbreak();
#endif
				return FALSE;
			}

			inline VOID			Clear()
			{
                // (f100527.5L) change clear function
                _TYPE* it = m_pFIELD;
                for (DWORD loop = _ARRAYCNT; loop; --loop) {
                    *it++ = _INITVAL;
                }
			}

private:	_TYPE				m_pFIELD[_ARRAYCNT];
};

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	<PROPERTIES><OLD VERSION>
#define __PROPERTY( DataType, name )											\
		public :																\
			void		Set##name( DataType value )		{ m_##name = value; }	\
			DataType	Get##name() const				{ return m_##name; }	\
		protected :																\
			DataType	m_##name;

#define __REF_PROPERTY( DataType, name )										\
		public :																\
			void		Set##name( DataType value )		{ m_##name = value; }	\
			DataType	Get##name() const				{ return m_##name; }	\
			const DataType &	Get##name##Ref() const	{ return m_##name; }	\
			const DataType *	Get##name##Ptr() const	{ return &m_##name; }	\
			DataType &	_Get##name##Ref()		 { return m_##name; }			\
			DataType *	_Get##name##Ptr()		{ return &m_##name; }			\
			void	Set##name##Ref( const DataType & value ) { m_##name = value; }	\
			void	Set##name##Ptr( const DataType * value ) { m_##name = *value; }	\
		protected :																\
			DataType	m_##name;

#define __BOOL_PROPERTY( name )													\
		public :																\
			void	Set##name( bool bValue = true )		{ m_##name = bValue; }	\
			bool	Is##name() const					{ return m_##name; }	\
		protected :																\
			bool	m_##name;


#define __PTR_PROPERTY( DataType, name )										\
		public :																\
			void		Set##name( DataType* value )	{ m_p##name = value; }	\
			const DataType*	Get##name() const			{ return m_p##name; }	\
			DataType*	Get##name()						{ return m_p##name; }	\
		protected :																\
			DataType*	m_p##name;

// (CHANGES) (100220) (WAVERIX) changes index type, WORD to DWORD,\
// expected that invalid index has can't access fault
// (CHANGES) (100220) (WAVERIX) add boundary checker
#define __ARRAY_PROPERTY(DataType, num, name) \
        public: \
            void Set##name(const DWORD idx, DataType value) { \
                if (FlowControl::FCAssert(idx < _countof(m_p##name))) { \
                    m_p##name[idx] = value; \
                } \
            }  \
            DataType Get##name(const DWORD idx) { \
                if (FlowControl::FCAssert(idx < _countof(m_p##name))) { \
                    return m_p##name[idx]; \
                } \
                return DataType(); \
            } \
            DataType* Get##name##Ptr() { return m_p##name; } \
        protected: \
            DataType m_p##name[num];

#if defined( _UNICODE ) || defined( UNICODE )
/*
/////////////////////////////////////////////////////////////////////////////////////
// �����ڵ�� �Լ���
/////////////////////////////////////////////////////////////////////////////////////
	#define TSTRCPY					wcscpy
	#define TSTRNCPY				wcsncpy
	#define TATOL					_wtol
	#define TSPRINTF				swprintf
	#define TSTRLEN					wcslen	
#else

#if defined( _MBCS )
/////////////////////////////////////////////////////////////////////////////////////
// MBCS
/////////////////////////////////////////////////////////////////////////////////////
	#define TSTRCPY					_mbscpy
	#define TSTRNCPY				_mbsncpy
	#define TATOL					atol
	#define TSPRINTF				sprintf
	#define TSTRLEN					_mbslen
#else
/////////////////////////////////////////////////////////////////////////////////////
// SBCS
/////////////////////////////////////////////////////////////////////////////////////
	#define TSTRCPY					strcpy
	#define TSTRNCPY				strncpy
	#define TATOL					atol
	#define TSPRINTF				sprintf
	#define TSTRLEN					strlen
#endif
*/
#endif

// CHANGES: f101110.1L, changes the '_countof' macro.
// _countof helper : this macro brought from over vs2005 version
#if !defined(_countof)
 #if !defined(__cplusplus)
  #define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#else
 extern "C++"
 {
   template <typename _CountofType, size_t _SizeOfArray>
   char (*__countof_helper(_CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];
   #define _countof(_Array) sizeof(*__countof_helper(_Array))
 }
 #endif
#endif

// <Description>
//   Ÿ�� ĳ���ý� �߻� ������ Ÿ�� �����÷θ� �˻��ϱ� ���� ����
//   �˻� ������ Ÿ�� �ִ�ġ = 64bit �ڷ��� (__int64) --or-- (long long)
// <Arguments>
//   F_TYPE: ������ ���� Ÿ��
//   F_VALUE: Ÿ�� �����÷θ� �˻��ϰ��� �ϴ� ����
//   T_TYPE: ��ȯ��Ű���� �ϴ� Ÿ��
// <Returns>
//   BOOL
//     TRUE: Ÿ�� �����÷ΰ� �߻��ϱ� �ʾҴ�
//     FALSE: Ÿ�� �����÷ΰ� �߻��ߴ�
// <Ex>
//   <����1> DWORD dwValue;
//   <����2> WORD wValue = (WORD)dwValue;
//
//   <����1> DWORD dwValue;
//   <����2> if( TRUE == SAFE_NUMERIC_TYPECAST(DWORD, dwValue, WORD) )
//   <����3>   WORD wValue = (WORD)dwValue;
//   <����4> else // ERROR HANDLING
//
#define SAFE_NUMERIC_TYPECAST( F_TYPE, F_VALUE, T_TYPE )	\
	( (F_TYPE)(((T_TYPE)(0xFFFFFFFFFFFFFFFFLL))&F_VALUE) == F_VALUE )

#define MAKEDWORD(a, b)		((DWORD)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))


template<bool> struct CompileTimeChecker
{
	CompileTimeChecker(...);
};
template<> struct CompileTimeChecker<false> {};
#define STATIC_ASSERT( expr )	STATIC_ASSERTMSG( expr, CompileTime )
#define STATIC_ASSERTMSG( expr, msg )	\
		{ \
			class Assertion_For_##msg {}; \
			(void)sizeof(CompileTimeChecker<(expr)!=0>((Assertion_For_##msg()))); \
		}
//(void)sizeof(CompileTimeChecker<(expr)!=0>((Assertion_For_##msg())));

//==================================================================================================
// Logic Level calculator
namespace util {
;
// (NOTE) (WAVERIX) (080912)
// - �ݿø��� �����ؼ� H/W ȯ��, compile option �Ǵ� debug|release�� ���̿� �����ϰ�
// �׻� ������ ����� ���δ�.
inline int LL_Round(const double val)
{
    const int integral = (int)val;
    const double mantissa = val - integral;

    return mantissa < .5f ? integral
                          : integral + 1;
}

inline int LL_Ceil(const double val)
{
    const INT integral = (INT)val;
    const double matissa = val - integral;

    return matissa > .0f ? integral + 1
                         : integral;
}

}; //end of namespace

//==================================================================================================

#if defined(_DEBUG)

  #ifdef _SUNGAME_VER //client side���� ����ϴ� ��ũ�� (����������)
    #error "disabled configuration"
  #endif

    #include <crtdbg.h>
#if _MSC_VER >= 1500
    //#include <vld.h>
#endif
    
    #define _CRTDBG_MAP_ALLOC
    inline void StartMemoryLeakCheck(void)      { _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); }
    #define BreakPointOnMemoryLeak( iMemory)    _CrtSetBreakAlloc( iMemory)

    #include <assert.h>

    #define BREAK(expr)     _ASSERTE(##expr)

	// ASSERT �ɰ������ ���� ���������� �ż��� ����
	#define ASSERT(expr)    __noop
    //#define ASSERT(expr)    FlowControl::FCAssert(##expr)

    //#define ASSERTMSG(expr,m)         if(!(FlowControl::FCAssert(expr))) { printf(m); }
    // ������ÿ��� Assert�� �ɸ� �� �ְ� break
    // ��ũ��Ʈ �����ÿ�
    #define FASSERT(expr)   _ASSERTE(expr)
    #define DISPMSG         printf
    #define DEBUG_CODE(c)   c

#elif defined(_SERVER)

    #define StartMemoryLeakCheck()  __noop
    #define BreakPointOnMemoryLeak  __noop
    #define BREAK(expr)             __noop
    #define ASSERT(expr)            FlowControl::FCAssert(##expr)
    #define DISPMSG                 printf
    #define DEBUG_CODE(c)           c

    // ������ÿ��� Assert�� �ɸ� �� �ְ� break
    // ��ũ��Ʈ �����ÿ�
    #define FASSERT(expr)   { if (FlowControl::FCAssert(##expr) == false) { __debugbreak(); }; }

#else //!defined(_SERVER) || !defined(_DEBUG)

    #define StartMemoryLeakCheck()  __noop
    #define BreakPointOnMemoryLeak  __noop
    #define BREAK(expr)             __noop

  #ifdef _SUNGAME_VER //client side���� ����ϴ� ��ũ�� (����������)
    #define ASSERT(expr)        __noop
    #define ASSERTMSG(expr,m)   __noop
    #define FASSERT(expr)       { if (!(expr)) { __debugbreak(); }; }
  #else
    #define ASSERT(expr)        FlowControl::FCAssert(##expr)
    // ������ÿ��� Assert�� �ɸ� �� �ְ� break
    // ��ũ��Ʈ �����ÿ�
    #define FASSERT(expr)   { if (FlowControl::FCAssert(##expr) == false) { __debugbreak(); }; }
  #endif

    #define DISPMSG         __noop
    #define DEBUG_CODE(c)   __noop
#endif

//==================================================================================================
// @history
// - |~10/02/22|���� �߰� 
// - |10/02/22|waverix|add types which are extracted from chrominum 'basictypes.h'
//                    |Comments="to support coding style since 2010"|]
// - |10/08/13|waverix|moved this point from the 'define.h'
//
typedef unsigned long  ulong;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

// start reference : chrominum/basictypes.h, SkUserConfig.h
//typedef signed char         schar;
typedef signed char         int8;
typedef signed char         int8_t;
typedef short               int16;
typedef short               int16_t;
typedef int                 int32;
typedef int                 int32_t;
typedef long long           int64;
typedef long long           int64_t;

// NOTE: unsigned types are DANGEROUS in loops and other arithmetical
// places.  Use the signed types unless your variable represents a bit
// pattern (eg a hash value) or you really need the extra bit.  Do NOT
// use 'unsigned' to express "this value should always be positive";
// use assertions for this.

typedef unsigned char       uint8;
typedef unsigned char       uint8_t;
typedef unsigned short      uint16;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64;
typedef unsigned long long  uint64_t;
// end reference : chrominum/basictypes.h, SkUserConfig.h
//==================================================================================================
// configuration for default compiler level 4 warnings to disable
//
// C4201 : unnamed union/struct/class
// to allow like a WzVector
//      union {
//          struct { float x, y, z; };
//          float elements[3];
//      };
#pragma warning(disable:4201)
// C4127 : conditional expression is constant
// to allow like a 'ASSERT(!"can't entered")'
#pragma warning(disable:4127)
//
//==================================================================================================

// NOTE: f110512.6L
enum eSUN_LOG_LEVEL
{
    eCRITICAL_LOG   = 1,
    eMIDDLE_LOG     = 2,
    eFULL_LOG       = 3,
    eDEV_LOG        = 4
};

#ifdef _SERVER
extern bool MessageOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...);
extern bool ConsoleOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...);
#else
#include <GeneralFunction.h>

inline void MessageOut(eSUN_LOG_LEVEL, const TCHAR*, ...) {}
inline void ConsoleOut(eSUN_LOG_LEVEL, const TCHAR*, ...) {}
#endif

// NOTE: f110512.6L
#ifdef _SUNGAME_VER //client side���� ����ϴ� ��ũ�� (����������)
    #define SUNLOG      __noop
    #define CONSOLELOG  __noop
    #define _TRACE      __noop
#else
    #define SUNLOG MessageOut
    #define CONSOLELOG ConsoleOut
    #define _TRACE DebugOut
#endif
//==================================================================================================
