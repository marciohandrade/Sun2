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
///  ���� ���α׷��� Crash�� ��Ȳ�� �ڵ鸵�Ͽ� ������� �����ϵ��� ���� ������ �����Ѵ�.
/**
	@author
			Server Engine part, Research & Development, Webzen
	@since
			2005. 2. 7
	@remarks
			���� ���α׷��� Unhandled Exception�� �޾Ƴ��� ������ �߻��� ��Ȳ�� ����� �� �� 
			�ֵ��� Code ������ ������ �����Ѵ�.
			Crash Handler�� �� ������ Stack trace�� Mini Dump �� ������ �����Ѵ�.
			- Stack Trace�� ���, Text�� ��ȯ�Ǹ� Crash�� ��� �Լ��� ȣ��� ��Ȳ�� ������
			�����ش�.
			- Mini Dump�� ���, dmp���Ϸ� ����Ǹ� Crash�� ��� Call stack �� �� ��������
			���� �� �ڼ��� ������ �����Ѵ�. �� ���� Visual Studio�� ���� ������� �� �� �ִ�.

			- CrashHandler.h�� include�ϱ� ���� �ռ� Windows.h�� include�ؾ� �Ѵ�.

	@note

			- 1) ���� �߻�
			���⿡�� �����ϴ� ������ Unhandled Exception�̴�.
			�Ϲ������� SEH�� ����Ͽ� ���� ���ܸ� ó���� �� ������ SEH�� ������� ���� �ڵ忡��
			������ �߻��� ��� �� ���ؿ� ���� O/S�� ���� ���α׷��� ������ ���߰� ���μ�����
			�����Ѵ�. �����κ��� �߻��� ���ܸ� Handling���� �ʾұ� �����̴�.
			��ó�� ���ܿ� ���� �ڵ鸵�� ���� ���� �߻��ϴ� ���ܸ� ������ �����Ѵ�.


			- 2) ȯ�� ����
			Crash Handler�� ����ϱ� ���ؼ��� Project Setting�� �����ؾ� �Ѵ�.
			�����ؾ��� �κ��� Release������ ����ȭ �ɼ��� Disable�� �����ؾ��Ѵٴ����̴�.
			Debugging ������ ������ ���ؼ� ����ȭ �ɼ��� �����ϴµ�, ���� ����ȭ �ɼ��� ��
			�ʿ��ϴٸ� Crash Handler������ ��Ȯ�� ������ ������ �� ����.


			- 3) �ӵ� ����
			Crash Handler�� ���α׷��� �����ϴ� ���� ��� �����ϰ� ���� �ʴ´�. �̸� �״��
			������ �ڵ鸵�ϴ� �۾��̸� ����͸��� ���� �ʱ� ������ �� ����� ����ϴ���
			�������α׷��� ������ ��ġ���ʴ´�.


			- 4) pdb ������ ����
			Crash Handler�� ������� ���� Debug Ȥ�� Release������ pdb ������ �ʿ���Ѵ�.
			���⿡���� ���� �� ���� ���׿� �����ؾ��Ѵ�.

			1. Stack Walker
			Stack Walker�� �۾��� ��û�� �������� �ҽ��ڵ� ������ Call stack��
			����Ѵ�. ������ ���� ���α׷��� ������ �� pdb�� �Բ� �����ؾ��Ѵ�.


			2. Mini Dump
			Mini Dump�� �۾��� ��û�� �������� ���� �޸𸮸� ��� �޴� ������ �۾��� �����Ѵ�.
			�ٽø��� Mini Dump���� ����� ������ ���Ե��������� ���������� *.dmp �� �����
			������� �õ��ϱ� ���ؼ��� pdb������ �ʿ��ϴ�. �� ���� End User�� �������
			�õ����� �ʱ� ������ pdb�� ������ �ʿ䰡 ������ �ǹ��Ѵ�.


			* Mini Dump���� Stack Walker�� ������ ���ԵǾ������Ƿ� ���� Stack Walker�� �����
			�ʿ䰡 ����.


			- 5) Run vs Debug Run
			���α׷��� �����ϴ� ����� Ctrl + F5( Run )�� F5( Debug Run ), �� ���� ����� �ִ�.
			Debug Run���� ������ ��� Top level exception filter�� ���� ȯ��(VC++)���� �����ϱ�
			������ CrashHandler�� ������ �������� ���Ѵ�. �� �� CrashHandler�� ����ϱ� ���ؼ�
			SEH�� ���� ����Ͽ� CrashHandler�� ȣ���ϴ� ����� ����ؾ��Ѵ�.
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