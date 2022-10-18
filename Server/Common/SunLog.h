#pragma once

#define OUTPUT_NONE			0
#define	OUTPUT_FILE			1
#define	OUTPUT_MONITOR		2
#define OUTPUT_BOTH			3
#define MAX_TIME_SIZE		100

#define __WAVERIX_SUNLOG_REVISION__

//
//  <기본 설명>
//    CSunLog Revision
//    이것은 기존 버전과의 호환되면서 새로운 요청상황을 수용하기 위한 로직과 그에 따라 새로 설계된 버전이다.
//    모든 로그의 기본 인터페이스는 CSunLog::LOGGINGwithARG 이며,
//    기타 다른 세부적인 처리들을 대신해주는 인터페이스들이 제공된다.
//
//  <기존 호환을 위한 로그 레벨과 로그 옵션의 재정의>
//    LEVEL의 경우, 자기 이하를 모두 포함한다.
//      FULL인 경우, { CRITICAL, MIDDLE, FULL }
//      MIDDLE인 경우, { CRITICAL, MIDDLE }
//      CRITICAL인 경우, { CRITICAL }
//    OPTION의 경우,
//      FILE은 파일에만 기록한다.
//      CONSOLE은 콘솔에만 기록한다.
//      BOTH인 경우, 새롭게 주어진 인자값에 의거, 해당 위치에만 기록한다.
//        -> BOTH로 설정되어 있다고 할지라도, 로깅시점에 특정 매체만 선택해서 기록이 가능하다
//    CATEGORY의 경우, (신규)
//      해당 플랙값이 설정되어 있는 경우에만 허용한다.
//
//  <기존 호환을 위한 인터페이스들>
//    <매크로> SUNLOG : TCHAR기반의 약간 변형된 루틴을 통해서 처리된다.
//    <매크로> SUNLOGCONN, 기타... : SUNLOG와 마찬가지이며, 이후 하나로 통합하는 방향으로 유도될 것이다.
//    <메소드> InsertMessage : SUNLOGxxx와 같은 매크로를 사용하지 않은 경우들이 있으며, 이를 위한 제공 인터페이스
//    <포멧제어> '\n'의 처리, 하나의 로깅당 다음줄로 넘기는 루틴이 포함되어 있다. 사용하는데 있어 다음 줄로 넘기는
//    루틴의 여부는 있어도 없어도 알아서 해준다.
//
//  <신규 옵션의 활용 방안>
//    ITEM, TRADE 등등의 관련된 범주를 포함해 로깅할 수 있도록 되어 있다.
//    이들의 블록처리도 가능하며, 쓰여진 파일 (SolarLog를 사용하므로 *.wzl)을 엑셀로 불러 구분할 수 있다.
//
//  <신규 카테고리 추가>
//    다음 검색 단어 "<LOG CATEGORY INTERFACE>"를 SunLog.h, SunLog.cpp에서 찾아 볼 것.
//    추가해야 할 것이 무엇인지 나오며, 필요한 경우 추가할 수 있다.
//    단, 카테고리에 대한 처리 허용 여부는
//    BIT fields에 기준(CSunLog::cCATE_MASK)하며, 고정된 값으로 작동되고 그 개수는 현 시스템에서는 32개가 한계일 것이다.
//
//  <이후 작업할 일>
//    카테고리의 동적 설정 또는 ...
//    GuildServer쪽은 작업되지 않았다.

// Blah~ Blah~ VC++ Compiler는 아래의 구문에서 실패가 되었다. 사용하지 않는다.
#define SUNLOG_ENUM_TO_STRING(name)	((TCHAR*)CSunLog::s##name())

#define ENUM_VALUE_LOGLEVEL(type, name, value)			\
	static const type	c##name = (eLOGLEVEL)(value);
//	static const DWORD	sc##name = (DWORD)(__int64)_T(#name)

//static const TCHAR*

#define ENUM_VALUE_LOGOPTION(type, name, value)			\
	static const type	c##name = (eLOGOPTION)(value);
//	static const DWORD	sc##name = (DWORD)(__int64)_T(#name)

#define ENUM_VALUE_CATEGORY(type, name, value)			\
	static const type	c##name = (eLOGCATEGORY)(value);	\
	inline static TCHAR* sc##name() { return _T(#name); }
//	static const DWORD	sc##name = (DWORD)(__int64)_T(#name)


//  <LOG CATEGORY INTERFACE>
#define SUNLOG_LOGGER_PER_OPTION(where, option)						\
	inline BOOL LOG( const eLOGLEVEL logLEVEL, const eLOGCATEGORY category, const TCHAR* pszMsg, va_list args )\
	{																								\
		CSunLog* pSunLog = CONTAINING_RECORD(this, CSunLog, where);									\
		BOOL bRET = LOGGINGwithARG( pSunLog, option, logLEVEL, category, pszMsg, args );			\
		va_end(args);																				\
		return bRET;																				\
	}

//  <LOG CATEGORY INTERFACE>
#define SUNLOG_LOGGER_PER_LEVEL(where, option, level)									\
	inline BOOL LOG( const eLOGCATEGORY category, const TCHAR* pszMsg, va_list args )	\
	{																					\
		CSunLog* pSunLog = CONTAINING_RECORD(this, CSunLog, where);						\
		BOOL bRET = LOGGINGwithARG( pSunLog, option, level, category, pszMsg, args );	\
		va_end(args);																	\
		return bRET;																	\
	}

//  <LOG CATEGORY INTERFACE>
#define SUNLOG_CATEGORY_OPTION_INTERFACE(name, catename)					\
	inline BOOL name( const eLOGLEVEL logLEVEL, const TCHAR* pszMsg, ... )	\
	{																		\
		va_list args;														\
		va_start(args, pszMsg);												\
		return LOG( logLEVEL, catename, pszMsg, args );						\
	}

//  <LOG CATEGORY INTERFACE>
#define SUNLOG_CATEGORY_LEVEL_INTERFACE(name, catename)		\
	inline BOOL name( const TCHAR* pszMsg, ... )			\
	{														\
		va_list args;										\
		va_start(args, pszMsg);								\
		return LOG( catename, pszMsg, args );				\
	}

class SolarLog;

class CSunLog
{
public:
	// <CONSTANTs>

	// <Descrition>
	//   eLOGLEVEL은 SunServer.ini에 등록된 LOG_LEVEL의 수치값과 동일하다.
	//   <BOUNDARY> 1<= (value) <=3
	enum eLOGLEVEL {};
	// <Descrition>
	//   eLOGOPTION은 SunServer.ini에 등록된 LOG_OPTION의 수치값과 동일하다.
	//   <BOUNDARY> 1<= (value) <=3
	enum eLOGOPTION {};
	// <Descrition>
	//   eLOGCATEGORY는 신규 추가된 것으로 남기는 로그의 성격을 분류하기 위함이다.
	//   파일로 남는 로그에는 이 범주로 구분할 수 있도록 탭이 구분된다.
	//   Init을 통해 이 범주들에 대한 마스크를 씌울 수 있을 것이나, 특별한 툴은 제공되지 않느다.
	//   <BOUNDARY> Bits 1<= (value) <=16
	enum eLOGCATEGORY {};
	enum eLOGCATEGORYGROUP {};

	ENUM_VALUE_LOGOPTION(eLOGOPTION, OUTPUT_NONE,		OUTPUT_NONE);
	ENUM_VALUE_LOGOPTION(eLOGOPTION, OUTPUT_FILE,		OUTPUT_FILE);
	ENUM_VALUE_LOGOPTION(eLOGOPTION, OUTPUT_CONSOLE,	OUTPUT_MONITOR);
	ENUM_VALUE_LOGOPTION(eLOGOPTION, OUTPUT_BOTH,		OUTPUT_BOTH);
	ENUM_VALUE_LOGOPTION(eLOGOPTION, OUTPUT_MASK,		cOUTPUT_NONE|cOUTPUT_FILE|cOUTPUT_CONSOLE);

	ENUM_VALUE_LOGLEVEL(eLOGLEVEL, CRITICAL_LOG,	eCRITICAL_LOG);
	ENUM_VALUE_LOGLEVEL(eLOGLEVEL, MIDDLE_LOG,		eMIDDLE_LOG);
	ENUM_VALUE_LOGLEVEL(eLOGLEVEL, FULL_LOG,		eFULL_LOG);
	ENUM_VALUE_LOGLEVEL(eLOGLEVEL, DEV_LOG,			eDEV_LOG);
	ENUM_VALUE_LOGLEVEL(eLOGLEVEL, LEVEL_MASK,		cCRITICAL_LOG|cMIDDLE_LOG|cFULL_LOG|cDEV_LOG);

	//  <LOG CATEGORY INTERFACE>
	ENUM_VALUE_CATEGORY(eLOGCATEGORY, CATE_DEFAULT,		1<<0);
	ENUM_VALUE_CATEGORY(eLOGCATEGORY, CATE_CONNECTION,	1<<1);
	ENUM_VALUE_CATEGORY(eLOGCATEGORY, CATE_WAREHOUSE,	1<<2);
	ENUM_VALUE_CATEGORY(eLOGCATEGORY, CATE_TRADE,		1<<3);
	ENUM_VALUE_CATEGORY(eLOGCATEGORY, CATE_ITEM,		1<<4);
	ENUM_VALUE_CATEGORY(eLOGCATEGORY, CATE_MISSION,		1<<5);
	ENUM_VALUE_CATEGORY(eLOGCATEGORY, CATE_ABUSE,		1<<6);
	ENUM_VALUE_CATEGORY(eLOGCATEGORY, CATE_DATARELOAD,	1<<30);
	//  <LOG CATEGORY INTERFACE>
	ENUM_VALUE_CATEGORY(eLOGCATEGORY, CATE_MASK,	\
		cCATE_DEFAULT | cCATE_CONNECTION | cCATE_WAREHOUSE | cCATE_TRADE | cCATE_ITEM | cCATE_MISSION \
		| cCATE_DATARELOAD | cCATE_ABUSE );

	CSunLog( VOID );
	~CSunLog( VOID );

public:
	//WriteMessage 
	// <METHODs><INTERFACEs>
	virtual BOOL		Init(
							const BYTE byLogOption,
							const BYTE byLogLevel,
							const TCHAR* pszFilePath,
							const TCHAR* pszFileName,
							BOOL bUseSolarLog = TRUE
							);
	inline static BOOL	LOGGING(
							CSunLog* pSunLog,
							const eLOGOPTION logOpt,
							const eLOGLEVEL logLEVEL,
							const eLOGCATEGORY category,
							const TCHAR* pszMsg
							)
	{	return LOGGINGwithARG( pSunLog, logOpt, logLEVEL, category, pszMsg, (va_list)NULL );	}

	static BOOL LOGGINGwithARG(
							CSunLog* pSunLog,
							const eLOGOPTION logOpt,
							const eLOGLEVEL logLEVEL,
							const eLOGCATEGORY category,
							const TCHAR* pszMsg,
							va_list vlist
		);

	// 이전 버전 호환용
	inline bool	InsertMessage( TCHAR* pszMsg, BYTE byLogLevel = 3 )
	{	return TRUE == LOGGING( this, CSunLog::cOUTPUT_BOTH, (eLOGLEVEL)byLogLevel, CSunLog::cCATE_DEFAULT, pszMsg );	}

	// Reload
	VOID	SetLogLevel( BYTE lv );


	// <DESCRIPTION>
	//   매 프레임시에 서버 시간 변경 검사를 수행한다.
	//   1) 버퍼 타임아웃 체크
	//   2) 날짜 변경에 따른 문제, 남아있는 버퍼 비우기 등의 처리를 수행한다. 
	//   2-1) 이경우, 파일이 변경되는 경우의 문제를 해결하기 위해 파일의 경우는 미리 Flush를 수행하도록 한다.
	VOID	Update();
	// <DESCRIPTION>
	//   현재 임시 버퍼에 쓰여진 내용을 강제로 출력시킨다. (콘솔+파일)
	VOID	Flush( eLOGOPTION eLogOPT = CSunLog::cOUTPUT_BOTH );

	typedef BOOL (*WriteMessage)( const CSunLog* pSunLog, const TCHAR* pMsg, va_list arglist );
	VOID	RegisterHandler( WriteMessage fnFileLogger );

	//
	//
	//
	//======================================================================================================
	//  다음부터는 CSunLog객체에서 사용하는 별도의 처리로직들이며, 이것을 사용하기 위한 INTERFACE가 아니다.
	//  <WARNING> 사용하지 말것.
	//
protected:
	// <CONSTANTs><TYPEs>
	static const DWORD	c_FileLogDataSize			= 1024*3;	//  3KB
	static const DWORD	c_FileLogDataSizeLimit		= 1024*2;	//  2KB
	static const DWORD	c_ConsoleLogDataSize		= 1024*8;	//  8KB
	static const DWORD	c_ConsoleLogDataSizeLimit	= 1024*7;	//  7KB
	static const DWORD	c_ConsoleLogInterval		= 500;		// 500ms
	static const DWORD	c_FileLogInterval			= 5000;		// 5000ms

	struct sLOG_HANDLER
	{
		WriteMessage		fn_FILE_LOGGER;
		WriteMessage		fn_CONSOLE_LOGGER;
	};
public:
	class CSunLogInfo
	{
	public:
		HANDLE				m_hConsole;
		//sLOG_HANDLER		m_Handler;
		WriteMessage		m_fnCONSOLE_LOGGER;

		struct sLOGGING_INFO
		{
			eLOGOPTION		Option;
			eLOGLEVEL		Level;
			eLOGCATEGORY	Mask;
		} LOGGING_INFO;

		struct sCONSOLE_LOG
		{
			DWORD			Written;
			TCHAR*			QueuedStream;		// Aligned Buffer by c_ConsoleLogDataSize
		} CONSOLE_LOG;
		CRITICAL_SECTION	m_csConsoleLog;
		DWORD				m_dwConsoleTimeout;

		CSunLogInfo();
		~CSunLogInfo();
	};

	// 하나의 프로세스에 하나만 유지해야하는 SUNLOG 정보들
	static DWORD			g_dwSUNLOG_INIT;
	static CSunLogInfo*		g_pSUNLOG;

private:
	// <METHODs>
	//   기능 필터링

#ifdef __SUNLOG_UNUSED_SECT
	//
	// <RETURN>
	//   buffer <- CSunLog::m_strFileNameWithTime
	//   size <- sizeof(CSunLog::m_strFileNameWithTime)
	TCHAR*	GetCurrentFileNameWithTime( const TCHAR* defFileName );
#endif
	//
	// LEVEL의 경우, 자기 이하를 모두 포함한다.
	//   FULL인 경우, CRITICAL+MIDDLE+FULL
	//   MIDDLE인 경우, CRITICAL+MIDDLE
	//   CRITICAL인 경우, CRITICAL
	// OPTION의 경우,
	//   FILE은 파일에만 기록한다.
	//   CONSOLE은 콘솔에만 기록한다.
	//   BOTH인 경우, 새롭게 주어진 인자값에 의거, 해당 위치에만 기록한다.
	// CATEGORY의 경우,
	//   해당 플랙값이 설정되어 있는 경우에만 허용한다.
	BOOL	CheckControl(
					const CSunLogInfo::sLOGGING_INFO* pINFO,
					const eLOGLEVEL logLEVEL,
					const eLOGOPTION logOPT,
					const eLOGCATEGORY logCATE )
	{
        __UNUSED(logOPT);
		if( pINFO->Level < logLEVEL )
			return FALSE;

		if( (pINFO->Mask&logCATE) != logCATE )
			return FALSE;

		return TRUE;
	}

	//BOOL	CheckHandler( CSunLog::sLOG_HANDLER& logHandler );
	BOOL	CheckHandler( WriteMessage& pfnColsoleLogHandler, WriteMessage& pfnFileLogHandler );

private:
	enum ePREFIX { PREFIX_NONE=0, PREFIX_WRITE_TIME=1, PREFIX_CATEGORY=2, };
	// METHODs for Handler
	// <RETURN>
	//   written position : INT (0.ret<MAX)
	//   -1 : error
	static INT	MakePrefixFormat( const ePREFIX ePrefix, TCHAR* buffer, const DWORD dwMaxWritableLen, const eLOGCATEGORY cate = cCATE_DEFAULT );
public:
	static INT	WriteMessage_IO( HANDLE hWriter, const BYTE* stream, const DWORD dwLength );
	// Handlers
	static BOOL WriteMessage_Console( const CSunLog* pSunLog, const TCHAR* pMsg, va_list vlist );
	static BOOL WriteMessage_Console2( const CSunLog* pSunLog, const TCHAR* pMsg, va_list vlist );
	static BOOL WriteMessage_Console3( const CSunLog* pSunLog, const TCHAR* pMsg, va_list vlist );
	static BOOL WriteMessage_File( const CSunLog* pSunLog, const TCHAR* pMsg, va_list vlist );

protected:
	// <FIELDs>
	SolarLog*			m_pMainLogger;
	WriteMessage		m_fnFILE_LOGGER;
	struct sFILE_LOG
	{
		DWORD			Written;
		TCHAR*			QueuedStream;		// Aligned Buffer by c_FileLogDataSize
	} FILE_LOG;
	CRITICAL_SECTION	m_csFileLog;
	DWORD				m_dwFileTimeout;

public:
	sFILE_LOG&			GetFILE_LOG() { return FILE_LOG; }
	//DWORD				m_dwWrittenFile;
	//TCHAR*				m_pFileQueuedData;		// Aligned Buffer by c_FileLogDataSize
	//DWORD				m_dwWrittenConsole;
	//TCHAR*				m_pConsoleQueuedData;	// Aligned Buffer by c_ConsoleLogDataSize

//로그 삽입 즉시 출력 플러그.
private:
	BOOL			m_bFileOutDelay;

public:
	VOID			SetFileOutDelayOption( BOOL bDelay ){ m_bFileOutDelay = bDelay; }
};

#include <SunLog.Interfaces.h>
//#include <SecureLog.h>

