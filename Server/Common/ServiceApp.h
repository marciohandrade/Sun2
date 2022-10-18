#ifndef __SERVICE_APP_H__
#define __SERVICE_APP_H__


#pragma once

//#include <Windows.h>

/*

< 서비스 생성하는 법 >

	시작 - 실행에서 cmd를 실행시켜서..
	
		c:/> sc
		c:/> sc create 

	하면 도움말이 나옵니다.

	ex)  sc create TestService binPath= d:\project\testservice.exe DisplayName= 테스트서비스
		
		// 위처럼 하면.. TestService라는 이름으로 testservice.exe를 등록하고
		// 서비스 관리자에서는 '테스트서비스'로 보입니다.

	주의) 옵션과 파라미터 사이는 한칸씩 띄워야합니다~

< 서비스 삭제 >
	
	ex)  sc delete TestService


< 서비스 실행하는 법 >

	제어판 - 관리도구 - 서비스 관리자?를 띄워서
	등록한 서비스 이름으로 시작-정지 등을 할 수 있습니다.
	속성에서 복구 탭을 선택하면 중지되었을때의 처리를 설정할 수 있습니다.
	(!) 시작매개변수도 지정할 수 있습니다. --> WinMain으로 가는게 아니라 ServiceMain(OnInit)으로 갑니다.
	로그온 탭에서 '서비스와 데스크톱 상호 작용 허용'을 켜면 콘솔이 보입니다. --> 기본 옵션으로 추가했습니다.


< 서비스 디버깅 하는 방법 >

	위 방법처럼 서비스를 등록하고
	서비스 관리자에서 등록한 서비스의 속성을 열어서 로그온 탭에서 '서비스와 데스크톱 상호 작용 허용'을 켭니다.
	
	그리고, regedit를 실행시켜 HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<등록한서비스이름>
	에 가보면 ImagePath에 실행화일이 설정되어 있습니다.

	실행화일 대신에 visual studio를 실행시켜서 디버깅 모드로 실행하게 할 수 있습니다.
	"C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE\devenv.exe" d:\project\testservice.exe /debugexe

	이제 서비스를 시작하면 vs가 실행되고 디버그 상태로 실행할 수 있게 됩니다.
	디버깅 중에 '디버그 - 모두 중단'을 누르면 어디선가 멈추겠죠~



 < 샘플 서비스 코드 >

 class TestServiceApp : public ServiceApp
{
public :
	TestServiceApp(const char* pServiceName)
	: ServiceApp(pServiceName)
	{
	}

	virtual ~TestServiceApp()
	{
	}

	virtual VOID	Update()
	{
		while (1)
		{
			// 0.5초에 한번씩 .을 찍습니다. --> 서비스는 콘솔이 없으므로 디버깅 모드에서만 보입니다.
			printf(".");
			Sleep(500);
		}
	}
};



__DECLARE_SERVICE( TestServiceApp, "TestService" );

void main( ) 
{ 	
	__START_SERVICE( TestServiceApp );
} 


< 예제 서비스 생성 >

D:\Sun\Server\_bin>sc create AgentServer binPath= d:\sun\server\_bin\AgentServer_d.exe DisplayName= "썬Agent"
[SC] CreateService 성공

D:\Sun\Server\_bin>sc create DBProxyServer binPath= d:\sun\server\_bin\DatabaseProxy_d.exe DisplayName= "썬DBProxy"
[SC] CreateService 성공

D:\Sun\Server\_bin>sc create GameServer binPath= d:\sun\server\_bin\GameServer_d.exe DisplayName= "썬Game"
[SC] CreateService 성공

*/

enum SERVER_MODE
{
	eCONSOLE_MODE = 0,
	eSERVICE_MODE,
};

class ServiceApp
{
public :
	ServiceApp(const char* pServiceName);
	virtual ~ServiceApp();

	VOID				StartService(LPSERVICE_MAIN_FUNCTION func, LPHANDLER_FUNCTION pCtrlHandler);

	VOID				HandleStart (DWORD argc, LPTSTR *argv);
	VOID				HandleControl (DWORD opcode);
	VOID				SetServiceName( const char* pszServiceName );

	virtual VOID		Update() = 0;

protected :
	
	virtual DWORD		OnInit (DWORD argc, LPTSTR *argv, DWORD *specificError)	{ argc; argv; specificError; return 0; }
	virtual VOID		OnPause()												{}
	virtual VOID		OnContinue()											{}
	virtual VOID		OnStop()												{}
	virtual VOID		OnInterrogate()											{}
	virtual VOID		OnShutDown()											{}

public :
	VOID				DebugOut(LPSTR String, DWORD Status);
	static	VOID		SetApplicationDirectory();							// 프로그램 실행 경로로 설정

protected :
	char					m_ServiceName[256];
	SERVICE_STATUS          m_ServiceStatus; 
	SERVICE_STATUS_HANDLE   m_ServiceStatusHandle; 

	LPHANDLER_FUNCTION		m_pServiceCtrlHandler;
};


#define __DECLARE_SERVICE( serviceClassName, serviceName )			\
																	\
		serviceClassName g_##serviceClassName( serviceName );		\
																	\
		VOID	Start##serviceClassName##Function (DWORD argc, LPTSTR *argv)	{ g_##serviceClassName.HandleStart(argc, argv); }	\
		VOID	Control##serviceClassName##Function (DWORD opcode)				{ g_##serviceClassName.HandleControl(opcode); }

#define __START_SERVICE( serviceClassName )					\
		g_##serviceClassName.StartService( (LPSERVICE_MAIN_FUNCTION)Start##serviceClassName##Function, (LPHANDLER_FUNCTION)Control##serviceClassName##Function );



#endif // __SERVICE_APP_H__
