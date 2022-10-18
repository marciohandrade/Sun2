#pragma once

#define OUTPUT_NONE			0
#define	OUTPUT_FILE			1
#define	OUTPUT_MONITOR		2
#define OUTPUT_BOTH			3
#define MAX_TIME_SIZE		100

#define __WAVERIX_SUNLOG_REVISION__

//
//  <�⺻ ����>
//    CSunLog Revision
//    �̰��� ���� �������� ȣȯ�Ǹ鼭 ���ο� ��û��Ȳ�� �����ϱ� ���� ������ �׿� ���� ���� ����� �����̴�.
//    ��� �α��� �⺻ �������̽��� CSunLog::LOGGINGwithARG �̸�,
//    ��Ÿ �ٸ� �������� ó������ ������ִ� �������̽����� �����ȴ�.
//
//  <���� ȣȯ�� ���� �α� ������ �α� �ɼ��� ������>
//    LEVEL�� ���, �ڱ� ���ϸ� ��� �����Ѵ�.
//      FULL�� ���, { CRITICAL, MIDDLE, FULL }
//      MIDDLE�� ���, { CRITICAL, MIDDLE }
//      CRITICAL�� ���, { CRITICAL }
//    OPTION�� ���,
//      FILE�� ���Ͽ��� ����Ѵ�.
//      CONSOLE�� �ֿܼ��� ����Ѵ�.
//      BOTH�� ���, ���Ӱ� �־��� ���ڰ��� �ǰ�, �ش� ��ġ���� ����Ѵ�.
//        -> BOTH�� �����Ǿ� �ִٰ� ������, �α������ Ư�� ��ü�� �����ؼ� ����� �����ϴ�
//    CATEGORY�� ���, (�ű�)
//      �ش� �÷����� �����Ǿ� �ִ� ��쿡�� ����Ѵ�.
//
//  <���� ȣȯ�� ���� �������̽���>
//    <��ũ��> SUNLOG : TCHAR����� �ణ ������ ��ƾ�� ���ؼ� ó���ȴ�.
//    <��ũ��> SUNLOGCONN, ��Ÿ... : SUNLOG�� ���������̸�, ���� �ϳ��� �����ϴ� �������� ������ ���̴�.
//    <�޼ҵ�> InsertMessage : SUNLOGxxx�� ���� ��ũ�θ� ������� ���� ������ ������, �̸� ���� ���� �������̽�
//    <��������> '\n'�� ó��, �ϳ��� �α�� �����ٷ� �ѱ�� ��ƾ�� ���ԵǾ� �ִ�. ����ϴµ� �־� ���� �ٷ� �ѱ��
//    ��ƾ�� ���δ� �־ ��� �˾Ƽ� ���ش�.
//
//  <�ű� �ɼ��� Ȱ�� ���>
//    ITEM, TRADE ����� ���õ� ���ָ� ������ �α��� �� �ֵ��� �Ǿ� �ִ�.
//    �̵��� ���ó���� �����ϸ�, ������ ���� (SolarLog�� ����ϹǷ� *.wzl)�� ������ �ҷ� ������ �� �ִ�.
//
//  <�ű� ī�װ� �߰�>
//    ���� �˻� �ܾ� "<LOG CATEGORY INTERFACE>"�� SunLog.h, SunLog.cpp���� ã�� �� ��.
//    �߰��ؾ� �� ���� �������� ������, �ʿ��� ��� �߰��� �� �ִ�.
//    ��, ī�װ��� ���� ó�� ��� ���δ�
//    BIT fields�� ����(CSunLog::cCATE_MASK)�ϸ�, ������ ������ �۵��ǰ� �� ������ �� �ý��ۿ����� 32���� �Ѱ��� ���̴�.
//
//  <���� �۾��� ��>
//    ī�װ��� ���� ���� �Ǵ� ...
//    GuildServer���� �۾����� �ʾҴ�.

// Blah~ Blah~ VC++ Compiler�� �Ʒ��� �������� ���а� �Ǿ���. ������� �ʴ´�.
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
	//   eLOGLEVEL�� SunServer.ini�� ��ϵ� LOG_LEVEL�� ��ġ���� �����ϴ�.
	//   <BOUNDARY> 1<= (value) <=3
	enum eLOGLEVEL {};
	// <Descrition>
	//   eLOGOPTION�� SunServer.ini�� ��ϵ� LOG_OPTION�� ��ġ���� �����ϴ�.
	//   <BOUNDARY> 1<= (value) <=3
	enum eLOGOPTION {};
	// <Descrition>
	//   eLOGCATEGORY�� �ű� �߰��� ������ ����� �α��� ������ �з��ϱ� �����̴�.
	//   ���Ϸ� ���� �α׿��� �� ���ַ� ������ �� �ֵ��� ���� ���еȴ�.
	//   Init�� ���� �� ���ֵ鿡 ���� ����ũ�� ���� �� ���� ���̳�, Ư���� ���� �������� �ʴ���.
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

	// ���� ���� ȣȯ��
	inline bool	InsertMessage( TCHAR* pszMsg, BYTE byLogLevel = 3 )
	{	return TRUE == LOGGING( this, CSunLog::cOUTPUT_BOTH, (eLOGLEVEL)byLogLevel, CSunLog::cCATE_DEFAULT, pszMsg );	}

	// Reload
	VOID	SetLogLevel( BYTE lv );


	// <DESCRIPTION>
	//   �� �����ӽÿ� ���� �ð� ���� �˻縦 �����Ѵ�.
	//   1) ���� Ÿ�Ӿƿ� üũ
	//   2) ��¥ ���濡 ���� ����, �����ִ� ���� ���� ���� ó���� �����Ѵ�. 
	//   2-1) �̰��, ������ ����Ǵ� ����� ������ �ذ��ϱ� ���� ������ ���� �̸� Flush�� �����ϵ��� �Ѵ�.
	VOID	Update();
	// <DESCRIPTION>
	//   ���� �ӽ� ���ۿ� ������ ������ ������ ��½�Ų��. (�ܼ�+����)
	VOID	Flush( eLOGOPTION eLogOPT = CSunLog::cOUTPUT_BOTH );

	typedef BOOL (*WriteMessage)( const CSunLog* pSunLog, const TCHAR* pMsg, va_list arglist );
	VOID	RegisterHandler( WriteMessage fnFileLogger );

	//
	//
	//
	//======================================================================================================
	//  �������ʹ� CSunLog��ü���� ����ϴ� ������ ó���������̸�, �̰��� ����ϱ� ���� INTERFACE�� �ƴϴ�.
	//  <WARNING> ������� ����.
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

	// �ϳ��� ���μ����� �ϳ��� �����ؾ��ϴ� SUNLOG ������
	static DWORD			g_dwSUNLOG_INIT;
	static CSunLogInfo*		g_pSUNLOG;

private:
	// <METHODs>
	//   ��� ���͸�

#ifdef __SUNLOG_UNUSED_SECT
	//
	// <RETURN>
	//   buffer <- CSunLog::m_strFileNameWithTime
	//   size <- sizeof(CSunLog::m_strFileNameWithTime)
	TCHAR*	GetCurrentFileNameWithTime( const TCHAR* defFileName );
#endif
	//
	// LEVEL�� ���, �ڱ� ���ϸ� ��� �����Ѵ�.
	//   FULL�� ���, CRITICAL+MIDDLE+FULL
	//   MIDDLE�� ���, CRITICAL+MIDDLE
	//   CRITICAL�� ���, CRITICAL
	// OPTION�� ���,
	//   FILE�� ���Ͽ��� ����Ѵ�.
	//   CONSOLE�� �ֿܼ��� ����Ѵ�.
	//   BOTH�� ���, ���Ӱ� �־��� ���ڰ��� �ǰ�, �ش� ��ġ���� ����Ѵ�.
	// CATEGORY�� ���,
	//   �ش� �÷����� �����Ǿ� �ִ� ��쿡�� ����Ѵ�.
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

//�α� ���� ��� ��� �÷���.
private:
	BOOL			m_bFileOutDelay;

public:
	VOID			SetFileOutDelayOption( BOOL bDelay ){ m_bFileOutDelay = bDelay; }
};

#include <SunLog.Interfaces.h>
//#include <SecureLog.h>

