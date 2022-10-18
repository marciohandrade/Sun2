#ifndef __SERVICE_APP_H__
#define __SERVICE_APP_H__


#pragma once

//#include <Windows.h>

/*

< ���� �����ϴ� �� >

	���� - ���࿡�� cmd�� ������Ѽ�..
	
		c:/> sc
		c:/> sc create 

	�ϸ� ������ ���ɴϴ�.

	ex)  sc create TestService binPath= d:\project\testservice.exe DisplayName= �׽�Ʈ����
		
		// ��ó�� �ϸ�.. TestService��� �̸����� testservice.exe�� ����ϰ�
		// ���� �����ڿ����� '�׽�Ʈ����'�� ���Դϴ�.

	����) �ɼǰ� �Ķ���� ���̴� ��ĭ�� ������մϴ�~

< ���� ���� >
	
	ex)  sc delete TestService


< ���� �����ϴ� �� >

	������ - �������� - ���� ������?�� �����
	����� ���� �̸����� ����-���� ���� �� �� �ֽ��ϴ�.
	�Ӽ����� ���� ���� �����ϸ� �����Ǿ������� ó���� ������ �� �ֽ��ϴ�.
	(!) ���۸Ű������� ������ �� �ֽ��ϴ�. --> WinMain���� ���°� �ƴ϶� ServiceMain(OnInit)���� ���ϴ�.
	�α׿� �ǿ��� '���񽺿� ����ũ�� ��ȣ �ۿ� ���'�� �Ѹ� �ܼ��� ���Դϴ�. --> �⺻ �ɼ����� �߰��߽��ϴ�.


< ���� ����� �ϴ� ��� >

	�� ���ó�� ���񽺸� ����ϰ�
	���� �����ڿ��� ����� ������ �Ӽ��� ��� �α׿� �ǿ��� '���񽺿� ����ũ�� ��ȣ �ۿ� ���'�� �մϴ�.
	
	�׸���, regedit�� ������� HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<����Ѽ����̸�>
	�� ������ ImagePath�� ����ȭ���� �����Ǿ� �ֽ��ϴ�.

	����ȭ�� ��ſ� visual studio�� ������Ѽ� ����� ���� �����ϰ� �� �� �ֽ��ϴ�.
	"C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE\devenv.exe" d:\project\testservice.exe /debugexe

	���� ���񽺸� �����ϸ� vs�� ����ǰ� ����� ���·� ������ �� �ְ� �˴ϴ�.
	����� �߿� '����� - ��� �ߴ�'�� ������ ��𼱰� ���߰���~



 < ���� ���� �ڵ� >

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
			// 0.5�ʿ� �ѹ��� .�� ����ϴ�. --> ���񽺴� �ܼ��� �����Ƿ� ����� ��忡���� ���Դϴ�.
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


< ���� ���� ���� >

D:\Sun\Server\_bin>sc create AgentServer binPath= d:\sun\server\_bin\AgentServer_d.exe DisplayName= "��Agent"
[SC] CreateService ����

D:\Sun\Server\_bin>sc create DBProxyServer binPath= d:\sun\server\_bin\DatabaseProxy_d.exe DisplayName= "��DBProxy"
[SC] CreateService ����

D:\Sun\Server\_bin>sc create GameServer binPath= d:\sun\server\_bin\GameServer_d.exe DisplayName= "��Game"
[SC] CreateService ����

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
	static	VOID		SetApplicationDirectory();							// ���α׷� ���� ��η� ����

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
