#pragma once

#include <log4cxx/TplSingleton.h>
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>

#define LOG4CXX_DEBUG_SIMPLE(logger, message) { \
	if (logger->isDebugEnabled()) {\
	::log4cxx::StringBuffer oss; \
	oss << message; \
	logger->forcedLog(::log4cxx::Level::DEBUG, oss.str()); }}

#define _debug(message)	LOG4CXX_DEBUG_SIMPLE(_instance(TinyLogger)->GetLoggerPtr(), message)
#define _info(message)	LOG4CXX_INFO (_instance(TinyLogger)->GetLoggerPtr(), message)
#define _warn(message)	LOG4CXX_WARN (_instance(TinyLogger)->GetLoggerPtr(), message)
#define _error(message)	LOG4CXX_ERROR(_instance(TinyLogger)->GetLoggerPtr(), message)
#define _fatal(message)	LOG4CXX_FATAL(_instance(TinyLogger)->GetLoggerPtr(), message)

//#ifdef _DEBUG
//	#pragma comment(lib, "log4cxx_d.lib")
//#pragma comment(lib, "log4cxx.lib")
//#pragma comment(lib, "log4cxxs.lib")
//#pragma comment(lib, "ws2_32.lib")
//#else
//	#pragma comment(lib, "log4cxxs.lib")
//#endif

//namespace pharos
//{
	/*! 
		@author		guiun.oh
		@date		2006-10-26
		@class		TinyLogger
		@version	
		               customized by crenique. 2007.1.8
		@brief
		@bug
		@warning
	 */




	class TinyLogger
	{
	public:
		TinyLogger(void)
		{
		}

	public:	
		virtual ~TinyLogger(void)
		{
			release();
		}

		int initialize(char* szLoggerName, char* szLoggerConfigFile = NULL)
		{
			int nRet = 0;
			USES_CONVERSION;

			m_LoggerPtr = log4cxx::Logger::getLogger(_T(szLoggerName));

			// logger configuration
			if( NULL != szLoggerConfigFile ) 
			{
				log4cxx::PropertyConfigurator::configure(_T(szLoggerConfigFile));
			}
			else 
			{
				log4cxx::BasicConfigurator::configure();
			}		
			
			return nRet;
		}

		int release(void)
		{
			int nRet = 0;

		//	m_LoggerPtr->releaseRef();
		//	m_LoggerPtr = NULL;

			return nRet;
		}

		log4cxx::LoggerPtr GetLoggerPtr()
		{
			return m_LoggerPtr;
		}

	protected:
		log4cxx::LoggerPtr m_LoggerPtr;

	private:		
	};
//};
