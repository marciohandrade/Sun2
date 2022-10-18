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
			���� ���α׷��� CrashHandler�� ��ġ�Ѵ�. �� �Լ��� ����Ǹ� ���� �߻��Ǵ� ���
			Unhandled Exception�� CrashHandler���� �޾� ó���Ѵ�.
	@note
			�� �Լ��� ȣ���� �� Uninstall ���� Install�� �ٽ� ȣ���� �� �ִ�. �� ��� ������
			������ �ٲ�� �Ǹ�, Memory leak �� �ٸ� ������ �߻� ����� ����.
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
			�� �Լ��� ����Ǹ� ���� ���α׷����� �߻��ϴ� Unhandle Exception�� �� �̻�
			Crash Handler���� �޾� ó������ �ʴ´�. Install �Լ��� ������� ������ ������
			Top level Unhandled Exception handler�� ������ �ǵ�����.
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
			Install �Լ��� ������ ����Ǿ��ٸ�, ���� ���α׷����� ������ �߻��� ��� �� �Լ���
			ȣ��ȴ�. Install�� �� �Լ��� Top level Exception Filter�� ����Ѵ�.
	@note
			�� �Լ��� �������α׷����� ������ ȣ���� �� �ִ�. �� ��� �Լ��� ���ڷδ�
			GetExceptionInformation()�� ����Ͽ� �Է��� �� ������, NULL�� ������� �ʴ´�.
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