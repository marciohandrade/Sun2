///////////////////////////////////////////////////////////////////////
//  Crash Handler / Win32 Application crash handling service.
//  Version 0.1
//
//  Note : Using Runtime library is MT and MTd
//---------------------------------------------------------------------
//  Built in Server Engine part, Research & Development, Webzen

#ifndef  __CrashHandler_H
#define  __CrashHandler_H

#if !defined( _MT )
	#error You must link against multi-threaded libraries when using CrashHandler.
#endif


#define  EXCEPTION_FILTER		LPTOP_LEVEL_EXCEPTION_FILTER


//=================================================================================================
///  응용 프로그램의 Crash된 상황을 핸들링하여 디버깅이 가능하도록 관련 정보를 보고한다.
/**
	@author
			Server Engine part, Research & Development, Webzen
	@since
			2005. 2. 7
	@remarks
			응용 프로그램의 Unhandled Exception을 받아내어 오류가 발생된 상황을 디버깅 할 수 
			있도록 Code 수준의 정보를 생성한다.
			Crash Handler는 이 정보를 Stack trace와 Mini Dump 두 가지로 제공한다.
			- Stack Trace의 경우, Text로 반환되며 Crash된 당시 함수의 호출된 상황을 간략히
			보여준다.
			- Mini Dump의 경우, dmp파일로 저장되며 Crash된 당시 Call stack 및 각 변수들의
			상태 등 자세한 정보를 제공한다. 이 것은 Visual Studio를 통해 디버깅을 할 수 있다.

			- CrashHandler.h를 include하기 전에 앞서 Windows.h를 include해야 한다.

	@note

			- 1) 오류 발생
			여기에서 정의하는 오류는 Unhandled Exception이다.
			일반적으로 SEH를 사용하여 각종 예외를 처리할 수 있지만 SEH가 적용되지 않은 코드에서
			오류가 발생할 경우 그 수준에 따라 O/S는 응용 프로그램의 동작을 멈추고 프로세스를
			종료한다. 오류로부터 발생된 예외를 Handling하지 않았기 때문이다.
			이처럼 예외에 대한 핸들링을 하지 못해 발생하는 예외를 오류로 가정한다.


			- 2) 환경 설정
			Crash Handler를 사용하기 위해서는 Project Setting을 설정해야 한다.
			주의해야할 부분은 Release에서도 최적화 옵션을 Disable로 설정해야한다는점이다.
			Debugging 정보의 생성을 위해서 최적화 옵션을 포기하는데, 만일 최적화 옵션이 꼭
			필요하다면 Crash Handler에서는 정확한 오류를 보고할 수 없다.


			- 3) 속도 저하
			Crash Handler는 프로그램이 동작하는 동안 계속 감시하고 있지 않는다. 이름 그대로
			오류를 핸들링하는 작업이며 모니터링을 하지 않기 때문에 이 모듈을 사용하더라도
			응용프로그램에 영향을 미치지않는다.


			- 4) pdb 파일의 배포
			Crash Handler는 디버깅을 위해 Debug 혹은 Release에서도 pdb 파일을 필요로한다.
			여기에서는 다음 두 가지 사항에 유의해야한다.

			1. Stack Walker
			Stack Walker의 작업이 요청된 시점에서 소스코드 수준의 Call stack을
			기록한다. 때문에 응용 프로그램을 배포할 때 pdb를 함께 배포해야한다.


			2. Mini Dump
			Mini Dump의 작업이 요청된 시점에서 단지 메모리를 백업 받는 정도의 작업을 수행한다.
			다시말해 Mini Dump에는 디버깅 정보가 포함되지않지만 덤프파일인 *.dmp 를 사용해
			디버깅을 시도하기 위해서는 pdb파일이 필요하다. 이 것은 End User가 디버깅을
			시도하지 않기 때문에 pdb를 배포할 필요가 없음을 의미한다.


			* Mini Dump에는 Stack Walker의 내용이 포함되어있으므로 굳이 Stack Walker를 사용할
			필요가 없다.


			- 5) Run vs Debug Run
			프로그램을 실행하는 방법은 Ctrl + F5( Run )과 F5( Debug Run ), 두 가지 방법이 있다.
			Debug Run으로 실행할 경우 Top level exception filter를 개발 환경(VC++)에서 차지하기
			때문에 CrashHandler는 스스로 동작하지 못한다. 이 때 CrashHandler를 사용하기 위해서
			SEH를 직접 사용하여 CrashHandler를 호출하는 방법을 사용해야한다.
*/
//=================================================================================================
class CrashHandler
{
public:
	typedef	void				( *ProcResult )( LPCTSTR stackWalkLog, LPCTSTR dumpFileName );
	typedef	LPCTSTR				( *CallbackDmpName )();

public:
	static	void				Install( bool stackWalk, CallbackDmpName cb, ProcResult procResult );
	static	void				Uninstall();

	static	LONG	__stdcall	HandleException( PEXCEPTION_POINTERS exceptionInfo );

private:
			CrashHandler()	{}
			~CrashHandler()	{}

	static	void				GetExceptionCode( char* errorMsg, DWORD errorType );


	static	EXCEPTION_FILTER	m_prevFilter;

	static	bool				m_stackWalk;
	static	CallbackDmpName		m_cbDmpName;
	static	ProcResult			m_procResult;
};

#endif