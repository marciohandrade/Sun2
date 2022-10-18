#include <stdio.h>
#include <Windows.h>
#include <DbgHelp.h>

#include "CrashHandler.h"
#include "MiniDump.h"

#pragma comment(lib, "dbghelp.lib")

CrashHandler::ProcResult		CrashHandler::m_procResult	= 0;
CrashHandler::CallbackDmpName	CrashHandler::m_cbDmpName	= 0;
			  bool				CrashHandler::m_stackWalk	= false;

			  EXCEPTION_FILTER	CrashHandler::m_prevFilter	= 0;


//=================================================================================================
/**
	@remarks
			응용 프로그램에 CrashHandler를 설치한다. 이 함수가 실행되면 이후 발생되는 모든
			Unhandled Exception은 CrashHandler에서 받아 처리한다.
	@note
			이 함수를 호출한 후 Uninstall 없이 Install을 다시 호출할 수 있다. 이 경우 현재의
			설정만 바뀌게 되며, Memory leak 등 다른 문제의 발생 우려는 없다.
*/
//=================================================================================================
void CrashHandler::Install( bool stackWalk, CallbackDmpName cb, ProcResult procResult )
{
	m_prevFilter	= SetUnhandledExceptionFilter( HandleException );

	m_stackWalk		= stackWalk;
	m_cbDmpName		= cb;
	m_procResult	= procResult;
}

//=================================================================================================
/**
	@remarks
			이 함수가 실행되면 응용 프로그램에서 발생하는 Unhandle Exception을 더 이상
			Crash Handler에서 받아 처리하지 않는다. Install 함수가 실행되지 이전에 설정된
			Top level Unhandled Exception handler로 설정을 되돌린다.
*/
//=================================================================================================
void CrashHandler::Uninstall()
{
	if( m_prevFilter != 0 )
		SetUnhandledExceptionFilter( m_prevFilter );

	m_stackWalk		= false;
	m_cbDmpName		= 0;
	m_procResult	= 0;
}

//=================================================================================================
/**
	@remarks
			Install 함수가 이전에 실행되었다면, 응용 프로그램에서 오류가 발생할 경우 이 함수가
			호출된다. Install은 이 함수를 Top level Exception Filter로 등록한다.
	@note
			이 함수를 응용프로그램에서 강제로 호출할 수 있다. 이 경우 함수의 인자로는
			GetExceptionInformation()을 사용하여 입력할 수 있으며, NULL은 허용하지 않는다.
*/
//=================================================================================================
LONG __stdcall CrashHandler::HandleException( PEXCEPTION_POINTERS exceptionInfo )
{
	HANDLE	handleProcess	= GetCurrentProcess();
	HANDLE	handleThread	= GetCurrentThread();
	char	outputString[8192], lineBuffer[1024], temp[1024];
	LPCTSTR	dmpFileName = 0;
	BOOL	ret;
	int		i;


	if( exceptionInfo == 0 )
		return 0;


	::ZeroMemory( outputString, 8192 );
	::ZeroMemory( lineBuffer, 1024 );
	::ZeroMemory( temp, 1024 );


	//  First of all, just make minidump
	if( m_cbDmpName != 0 )
	{
		dmpFileName = ( *m_cbDmpName )();

		MiniDump::GenerateMiniDump( exceptionInfo, dmpFileName );
	}


	//  and next, back tracing call-stack
	SymSetOptions( SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_FAIL_CRITICAL_ERRORS );

	if( SymInitialize( handleProcess, 0, TRUE ) == TRUE )
	{
		//  Time stamp
		SYSTEMTIME	systemTime;

		GetLocalTime( &systemTime );
		sprintf( lineBuffer, "%4d.%2d.%2d  %d:%d:%d.%d\n\n",
			systemTime.wYear, systemTime.wMonth, systemTime.wDay,
			systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds );

		strcpy( outputString, lineBuffer );


		sprintf( lineBuffer, "Process Handle : %#x\t\t Thread Handle : %#x\r\n",
					handleProcess, handleThread );
		strcat( outputString, lineBuffer );

		//  Exception type
		GetExceptionCode( temp, exceptionInfo->ExceptionRecord->ExceptionCode );
		sprintf( lineBuffer, "Type : %s\n", temp );
		strcat( outputString, lineBuffer );


		//  Register information
		CONTEXT &context = *exceptionInfo->ContextRecord;

		sprintf( lineBuffer, "eax: %#08x ebx: %#08x\r\n", context.Eax, context.Ebx );
		strcat( outputString, lineBuffer );
		sprintf( lineBuffer, "ecx: %#08x edx: %#08x\r\n", context.Ecx, context.Edx );
		strcat( outputString, lineBuffer );
		sprintf( lineBuffer, "esi: %#08x edi: %#08x\r\n", context.Esi, context.Edi );
		strcat( outputString, lineBuffer );
		sprintf( lineBuffer, "ebp: %#08x esp: %#08x\r\n", context.Ebp, context.Esp );
		strcat( outputString, lineBuffer );
		strcat( outputString, "\r\n" );

		STACKFRAME stackFrame = { 0, };

		stackFrame.AddrPC.Offset	= context.Eip;
		stackFrame.AddrPC.Mode		= AddrModeFlat;
		stackFrame.AddrStack.Offset	= context.Esp;
		stackFrame.AddrStack.Mode	= AddrModeFlat;
		stackFrame.AddrFrame.Offset	= context.Ebp;
		stackFrame.AddrFrame.Mode	= AddrModeFlat;

		//  Call stack trace
		if( m_stackWalk == true )
		{
			for( i = 0 ; i < 512 && stackFrame.AddrPC.Offset ; i++ )
			{
				ret = StackWalk( IMAGE_FILE_MACHINE_I386, handleProcess, handleThread, &stackFrame, &context,
									0, SymFunctionTableAccess, SymGetModuleBase, 0 );
				if( ret != FALSE )
				{
					DWORD64			disp64 = 0;
					char			symbol[1024] = { 0, };
					PSYMBOL_INFO	symbolInfo = ( PSYMBOL_INFO )symbol;

					symbolInfo->MaxNameLen		= 512 - sizeof( SYMBOL_INFO );
					symbolInfo->SizeOfStruct	= sizeof( SYMBOL_INFO ) + symbolInfo->MaxNameLen - 1;

					ret = SymFromAddr( handleProcess, stackFrame.AddrPC.Offset, &disp64, symbolInfo );
					if( ret == TRUE )
					{
						IMAGEHLP_LINE64	errorLine	= { sizeof( IMAGEHLP_LINE64 ), 0, };
						IMAGEHLP_LINE64	symbolLine	= { sizeof( IMAGEHLP_LINE64 ), 0, };
						DWORD			disp		= 0;
						BOOL			ret1, ret2;

						ret1 = SymGetLineFromAddr64( handleProcess, stackFrame.AddrPC.Offset, &disp, &errorLine );
						ret2 = SymGetLineFromAddr64( handleProcess, stackFrame.AddrPC.Offset - ( DWORD )disp64,
														&disp, &symbolLine );
						if( ret1 == TRUE && ret2 == TRUE )
						{
							sprintf( lineBuffer, "%s() + %d line [addr=%#x]\r\n",
										symbolInfo->Name, errorLine.LineNumber - symbolLine.LineNumber,
										symbolInfo->Address );
							strcat( outputString, lineBuffer );

							sprintf( lineBuffer, "-%s %d line\r\n", errorLine.FileName, errorLine.LineNumber );
							strcat( outputString, lineBuffer );
						}
						else
						{
							sprintf( lineBuffer, "%s() + offset : 0x%04x\r\n",
										symbolInfo->Name, errorLine.LineNumber - symbolLine.LineNumber );
							strcat( outputString, lineBuffer );
							strcat( outputString, "(No line information...)\r\n" );
						}
					}
					else  //  SymFromAddr is failed
					{
						sprintf( lineBuffer, "0x%08x Address Lookup Error\r\n", stackFrame.AddrPC.Offset );
						strcat( outputString, lineBuffer );
						sprintf( lineBuffer, "-SymFromAddr() returned error : %d\r\n", GetLastError() );
						strcat( outputString, lineBuffer );
					}
				}
				else
					break;

				strcat( outputString, "\r\n" );
			}
		}

		SymCleanup( handleProcess );
	}

	if( m_procResult != 0 )
		( *m_procResult )( outputString, dmpFileName );

	return EXCEPTION_EXECUTE_HANDLER;
}

void CrashHandler::GetExceptionCode( char* message, DWORD type )
{
	switch( type )
	{
	case EXCEPTION_ACCESS_VIOLATION:
		sprintf( message, "Access violation" );
		break;

	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		sprintf( message, "Array bounds exceeded" );
		break;

	case EXCEPTION_BREAKPOINT:
		sprintf( message, "Break point" );
		break;

	case EXCEPTION_DATATYPE_MISALIGNMENT:
		sprintf( message, "Data type misalignment" );
		break;

	case EXCEPTION_FLT_DENORMAL_OPERAND:
		sprintf( message, "Float denormal operand" );
		break;

	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		sprintf( message, "Float divide by zero" );
		break;

	case EXCEPTION_FLT_INEXACT_RESULT:
		sprintf( message, "Float inexact result" );
		break;

	case EXCEPTION_FLT_INVALID_OPERATION:
		sprintf( message, "Float invalid operation" );
		break;

	case EXCEPTION_FLT_OVERFLOW:
		sprintf( message, "Float overflow" );
		break;

	case EXCEPTION_FLT_STACK_CHECK:
		sprintf( message, "Float stack check" );
		break;

	case EXCEPTION_FLT_UNDERFLOW:
		sprintf( message, "Float underlow" );
		break;

	case EXCEPTION_ILLEGAL_INSTRUCTION:
		sprintf( message, "Illegal instruction" );
		break;

	case EXCEPTION_IN_PAGE_ERROR:
		sprintf( message, "In page error" );
		break;

	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		sprintf( message, "Integer divide by zero" );
		break;

	case EXCEPTION_INT_OVERFLOW:
		sprintf( message, "Integer overflow" );
		break;

	case EXCEPTION_INVALID_DISPOSITION:
		sprintf( message, "Invalid disposition" );
		break;

	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		sprintf( message, "Noncontinuable exception" );
		break;

	case EXCEPTION_PRIV_INSTRUCTION:
		sprintf( message, "Privileged instruction" );
		break;

	case EXCEPTION_SINGLE_STEP:
		sprintf( message, "Single step" );
		break;

	case EXCEPTION_STACK_OVERFLOW:
		sprintf( message, "Stack overflow" );
		break;
	}
}