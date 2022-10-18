#pragma once

//=============================================================================================================================
/// SolarAuthServer Common Definition class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 11
	@remark
	- SolarAuthServer
	@note
	@history 
	-  
*/
//=============================================================================================================================


// 임시 선언 파일
// 이후 Common/ProgramCommon등으로 이동될 구조 등등
#ifdef _DEBUG
#	define VIRTUAL_LOCK_INIT(cs)	InitializeCriticalSection(&(cs))
#	define VIRTUAL_LOCKING(cs)		EnterCriticalSection(&(cs))
#	define VIRTUAL_UNLOCKING(cs)	LeaveCriticalSection(&(cs))
#else
#	define VIRTUAL_LOCK_INIT(cs)
#	define VIRTUAL_LOCKING(cs)
#	define VIRTUAL_UNLOCKING(cs)
#endif

#pragma pack( push, 1 )

typedef BYTE	CODE_NATION_T;
typedef BYTE	CODE_REGION_T;
typedef BYTE	MODE_SERVER_T;
typedef BYTE	PADDING_T;
typedef DWORD	CRYPT_KEY_T;

struct eCODE_NATION
{
	static const CODE_NATION_T	NONE		= 0;
	static const CODE_NATION_T	GLOBAL		= 0;
	static const CODE_NATION_T	KOREAN		= 1;

	CHAR* ToString()
	{
		switch( value )
		{
		case eCODE_NATION::GLOBAL:
			return "GLOBAL";
		case eCODE_NATION::KOREAN:
			return "KOREAN";
		}
		return (CHAR*)NULL;
	}

	eCODE_NATION() : value(eCODE_NATION::NONE) {}
	eCODE_NATION( CODE_NATION_T code ) : value( code ) {}

	CODE_NATION_T	value;
};

struct eCODE_REGION
{
	static const CODE_REGION_T	NONE		= 0;
	static const CODE_REGION_T	R1			= 1;
	static const CODE_REGION_T	R2			= 2;

	CHAR* ToString()
	{
		switch( value )
		{
		case eCODE_REGION::NONE:
			return "NONE";
		case eCODE_REGION::R1:
			return "R1";
		case eCODE_REGION::R2:
			return "R2";
		}
		return (CHAR*)NULL;
	}

	eCODE_REGION() : value(eCODE_REGION::NONE) {}
	eCODE_REGION( CODE_REGION_T code ) : value( code ) {}

	CODE_REGION_T	value;
};

struct eMODE_SERVER
{
	static const MODE_SERVER_T	NONE				= 0;
	static const MODE_SERVER_T	SOLAR_AUTH_SERVER	= 1;
	static const MODE_SERVER_T	MASTER_SERVER		= 2;

	CHAR* ToString()
	{
		switch( value )
		{
		case eMODE_SERVER::NONE:
			return "NONE";
		case eMODE_SERVER::SOLAR_AUTH_SERVER:
			return "SOLAR_AUTH_SERVER";
		case eMODE_SERVER::MASTER_SERVER:
			return "MASTER_SERVER";
		}
		return (CHAR*)NULL;
	}

	eMODE_SERVER() : value(eMODE_SERVER::NONE) {}
	eMODE_SERVER( MODE_SERVER_T code ) : value( code ) {}

	CODE_REGION_T	value;
};

typedef struct sGLOBAL_SERVER_KEY
{
	// <FIELDs>
	// [0]:국가번호 [1]:지역번호 [2]:서버타입 [3]:패딩타입
	struct KEY
	{
		eCODE_NATION	m_NationCode;
		eCODE_REGION	m_RegionCode;
		eMODE_SERVER	m_ServerMode;
		PADDING_T		m_PadType;
	} m_KEY;

	sGLOBAL_SERVER_KEY( const DWORD key=0 )
	{
		//DWORD tmpKey = key;
		KEY stkKey;
		stkKey.m_NationCode	= (BYTE)((0x000000FF)&(key>>0));
		stkKey.m_RegionCode	= (BYTE)((0x000000FF)&(key>>8));
		stkKey.m_ServerMode	= (BYTE)((0x000000FF)&(key>>16));
		stkKey.m_PadType	= (BYTE)((0x000000FF)&(key>>24));
		// 검증...?
		m_KEY = m_KEY;
	}

	inline operator DWORD()
	{
		DWORD dwKey = 0;
		dwKey |= ((((DWORD)m_KEY.m_NationCode.value)&0x000000FF)<<0) ;
		dwKey |= ((((DWORD)m_KEY.m_RegionCode.value)&0x000000FF)<<8) ;
		dwKey |= ((((DWORD)m_KEY.m_ServerMode.value)&0x000000FF)<<16) ;
		dwKey |= ((((DWORD)m_KEY.m_PadType)&0x000000FF)<<24) ;
		return dwKey;
	}

	VOID Set( eCODE_NATION nationCode, eCODE_REGION regionCode, eMODE_SERVER serverMode, PADDING_T padType = 0 )
	{
		m_KEY.m_NationCode = nationCode;
		m_KEY.m_RegionCode = regionCode;
		m_KEY.m_ServerMode = serverMode;
		m_KEY.m_PadType = padType;
	}

	inline CODE_NATION_T	GetNationCode() { return m_KEY.m_NationCode.value; }
	inline CODE_REGION_T	GetRegionCode() { return m_KEY.m_RegionCode.value; }
	inline MODE_SERVER_T	GetServerMode() { return m_KEY.m_ServerMode.value; }
	inline PADDING_T		GetPaddingType() { return m_KEY.m_PadType; }

} GLOBAL_SERVER_KEY;

typedef struct sGLOBAL_SERVER_INFO
{
	GLOBAL_SERVER_KEY	m_GlobalServerKey;
	CRYPT_KEY_T			m_CryptKey;
	struct
	{
		// 이후 IPv6를 고려할까 말까? 일단은 공간확보만 하자.
		DWORD			m_dwIPAddress;
		DWORD			m_reservedIP1;
		DWORD			m_reservedIP2;
		DWORD			m_reservedIP3;
	};

	inline CRYPT_KEY_T&	CryptKey() { return m_CryptKey; }
	inline DWORD&		IPAddress() { return m_dwIPAddress; }
} GLOBAL_SERVER_INFO;

#pragma pack( pop )


template<typename T>
class Delegator
{
public:
	Delegator() : m_POINTER(NULL) {}
	Delegator( T* ptr ) : m_POINTER(ptr) {}
	~Delegator() { m_POINTER = NULL; }

	inline BOOL operator == ( PVOID ptr ) const { return m_POINTER == ptr; }
	inline BOOL operator != ( PVOID ptr ) const { return !(m_POINTER == ptr); }

	inline BOOL operator == ( const Delegator<T>& ref ) const { return ref.m_POINTER == m_POINTER; }
	inline BOOL operator != ( const Delegator<T>& ref ) const { return !(ref.m_POINTER == m_POINTER); }

	inline T* operator-> ()	{ return m_POINTER; }

	inline T& operator* ()	{ return *m_POINTER; }

	inline T* operator() ()	{ return m_POINTER; }

	inline T*& REFERENCE()	{ return m_POINTER; }

private:

	// <FIELDs>
	T*		m_POINTER;
};

struct CHAR_WRAPPER
{
	TCHAR*			m_pSTRING;

	CHAR_WRAPPER() : m_pSTRING(NULL) {}
	CHAR_WRAPPER( const TCHAR* pString ) : m_pSTRING((TCHAR*)pString) {}

	inline operator TCHAR* () { return m_pSTRING; }
};

//=======================================================================================================================
//
// <Name>
//   ePHASE_SHIFT
// <Description>
//   xxxSession들의 패킷 처리 허용에 관련한 상태 필드
//
//   PHASE_NOACTION : 다른 필드의 체크 유무와 상관없이 아무런 동작도 허용하지 않는다.
//     (CONNECTION계열 일부 제외)
//   PHASE_UNKNOWN : 최초 접속 단계이며, 인증받기 위한 절차를 수행하는 단계
//     (CONNECTION, SERVER_INFO을 절차를 밟아 단계적으로 허용해 간다.)
//   PHASE_EXCHANGE : 상대방을 신뢰할 수 있는 서버로 판단되었을 때 전환되는 단계로 암호화 관련 처리를 수행
//     (SE_CRYPTOGRAPHY 키교환 관련)
//   PHASE_SERVICE : 모든 인증과정이 완료된 단계로 마스터 서버가 요구하는 데이터를 전송할 수 있다.
//     (CONNECTION, SERVER_INFO, SE_CRYPTOGRAPHY, DATA_TRANSFER 등의 대부분이 허용된다. 필요에 따라 (시간, 상태, 등등...)
//     막혀있을 수도 있을 것이다.
// <Example>
//   PHASE_UNKNOWN|PHASE_EXCHANGE --> 해당 패킷 처리는 PHASE_UNKNOWN와 PHASE_EXCHANGE에 허용되어 있다.
//   PHASE_EXCHANGE|PHASE_SERVICE --> 해당 패킷 처리는 PHASE_EXCHANGE와 PHASE_SERVICE에 허용되어 있다.
//   PHASE_UNLIMITED == (PHASE_UNKNOWN|PHASE_EXCHANGE|PHASE_SERVICE) --> 모든 상태에서 패킷 처리 허용
//   PHASE_BLOCKED|xxx --> 해당 패킷은 허용되어있는 필드 체크 여부와 상관없이 일시적으로 허용하지 않는다.
//                        이것은 블록 (설정/해제)를 통해 기존 설정을 변경하지 않고 변경할 수 있다.
typedef enum ePHASE_SHIFT
{
	PHASE_NOACTION		= 0x00000001,			// 
	PHASE_UNKNOWN		= PHASE_NOACTION<<1,	// <접속 단계>
	PHASE_EXCHANGE		= PHASE_NOACTION<<2,	// <인증 단계>
	PHASE_SERVICE		= PHASE_NOACTION<<3,	// <서비스 단계>
	PHASE_BLOCKED		= PHASE_NOACTION<<4,	// <경우에 따른 불허 처리>
	PHASE_UNLIMITED		= (PHASE_UNKNOWN|PHASE_EXCHANGE|PHASE_SERVICE),
};



