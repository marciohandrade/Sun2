#pragma once

// 모든 프로젝트 및 공유 폴더 위치가 고정이라는 전제하에 설정되는 상대 위치를 통한 Include
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<PROPERTIES>
//		@author : waverix
//		@remark :
//			PROPERTY<_TYPE>
//				= 단일 멤버 IO 처리용, complex타입 지원
//			PROPERTY_ARRAY<_TYPE, _INITVAL, _ARRAYCNT>
//				= 배열 멤버 IO 처리용, complex타입 미지원 (complex타입을 사용하기 위해서는 포인터와 같은 타입으로 사용할 것
//		@notes :
//				- 07/10/09 - 기존보다 형식 검사를 강화하고, 프로퍼티로서의 성격을 강화시켰다.
//							무수한 인터페이스와 필드들로 점철되는 매머드 객체로 사용하기 보다는
//							이런식으로 분리시켜 나가는 편이 이후 유지보수, 모듈화에 더 도움이 될 것이다.
//				- 07/10/09 - <주의> PROPERTY에서 GetRef, operator _TYPE& 는 complex타입 지원을 위해 const성질이 없다.
//							<주의>	PROPERTY_ARRAY에서 GetPtr는 const로서의 성질을 지키는 것이 안전하다. <읽기 전용>
//				- 07/10/09 - <주의> operator = 계열은 구현하지 않았습니다.
//							할당에 관련해서는 순수 타입 연산과 거의 동일해지는 문제 때문에 중대한 오류를 범할 수 있습니다.
//							<주의> 다음의 연산은 허용됩니다. 반대의 연산은 허용되지 않습니다.
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
//				// STATES는 미리 선언한 대로 0으로 초기화 되어 있음.
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
// 유니코드용 함수들
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
//   타입 캐스팅시 발생 가능한 타입 오버플로를 검사하기 위한 로직
//   검사 가능한 타입 최대치 = 64bit 자료형 (__int64) --or-- (long long)
// <Arguments>
//   F_TYPE: 변수의 원래 타입
//   F_VALUE: 타입 오버플로를 검사하고자 하는 변수
//   T_TYPE: 변환시키고자 하는 타입
// <Returns>
//   BOOL
//     TRUE: 타입 오버플로가 발생하기 않았다
//     FALSE: 타입 오버플로가 발생했다
// <Ex>
//   <기존1> DWORD dwValue;
//   <기존2> WORD wValue = (WORD)dwValue;
//
//   <변경1> DWORD dwValue;
//   <변경2> if( TRUE == SAFE_NUMERIC_TYPECAST(DWORD, dwValue, WORD) )
//   <변경3>   WORD wValue = (WORD)dwValue;
//   <변경4> else // ERROR HANDLING
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
// - 반올림과 관련해서 H/W 환경, compile option 또는 debug|release의 차이와 무관하게
// 항상 동일한 결과를 보인다.
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

  #ifdef _SUNGAME_VER //client side에서 사용하는 매크로 (유저배포용)
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

	// ASSERT 걸고싶으면 개인 디파인으로 거세요 제발
	#define ASSERT(expr)    __noop
    //#define ASSERT(expr)    FlowControl::FCAssert(##expr)

    //#define ASSERTMSG(expr,m)         if(!(FlowControl::FCAssert(expr))) { printf(m); }
    // 릴리즈시에도 Assert가 걸릴 수 있게 break
    // 스크립트 오류시에
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

    // 릴리즈시에도 Assert가 걸릴 수 있게 break
    // 스크립트 오류시에
    #define FASSERT(expr)   { if (FlowControl::FCAssert(##expr) == false) { __debugbreak(); }; }

#else //!defined(_SERVER) || !defined(_DEBUG)

    #define StartMemoryLeakCheck()  __noop
    #define BreakPointOnMemoryLeak  __noop
    #define BREAK(expr)             __noop

  #ifdef _SUNGAME_VER //client side에서 사용하는 매크로 (유저배포용)
    #define ASSERT(expr)        __noop
    #define ASSERTMSG(expr,m)   __noop
    #define FASSERT(expr)       { if (!(expr)) { __debugbreak(); }; }
  #else
    #define ASSERT(expr)        FlowControl::FCAssert(##expr)
    // 릴리즈시에도 Assert가 걸릴 수 있게 break
    // 스크립트 오류시에
    #define FASSERT(expr)   { if (FlowControl::FCAssert(##expr) == false) { __debugbreak(); }; }
  #endif

    #define DISPMSG         __noop
    #define DEBUG_CODE(c)   __noop
#endif

//==================================================================================================
// @history
// - |~10/02/22|지완 추가 
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
#ifdef _SUNGAME_VER //client side에서 사용하는 매크로 (유저배포용)
    #define SUNLOG      __noop
    #define CONSOLELOG  __noop
    #define _TRACE      __noop
#else
    #define SUNLOG MessageOut
    #define CONSOLELOG ConsoleOut
    #define _TRACE DebugOut
#endif
//==================================================================================================
