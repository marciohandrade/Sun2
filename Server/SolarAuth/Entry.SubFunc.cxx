
//static CHAR			g_szServiceName[MAX_PATH];

extern VOID CallBackCommand( CHAR* szMessage );

// Service
class SolarAuthServiceApp : public ServiceApp
{
public :
	SolarAuthServiceApp( const CHAR* pServiceName ) : ServiceApp(pServiceName) { };

	virtual DWORD		OnInit ( DWORD argc, LPTSTR* argv, DWORD* specificError );
	virtual VOID		Update();
	virtual VOID		OnStop();
	virtual VOID		OnShutDown();
};

//============================================================
//
extern "C"
{
	int WinMainCRTStartup();
}

int SunEntryPoint()
{
	SharedMemoryPool::CreateInstance();
	//_CrtSetAllocHook(SunAllocHook);
	return WinMainCRTStartup();
}


static int SunAllocHook(int nAllocType, void *pvData,
	size_t nSize, int nBlockUse, long lRequest,
	const unsigned char * szFileName, int nLine )
{
	__UNUSED(nAllocType);
	__UNUSED(pvData);
	__UNUSED(nBlockUse);
	__UNUSED(szFileName);
	__UNUSED(nLine);

	if (lRequest >= 1120551)	// 389862
	{
		//if (nSize == 32)
		{
			__asm int 3;
		}
	}


	return TRUE;
}


//
// <실행환경 설정 함수>
//
static BOOL		InitRuntimeEnvionment()
{
	// 서버 시작
	ServiceApp::SetApplicationDirectory();

	// 콘솔창 초기화
	CONSOLE_DESC cdesc;
	ZeroMemory( &cdesc, sizeof(cdesc) );
	cdesc.bUsingConsoleInput	= FALSE;
	cdesc.bUsingRedirect		= TRUE;

	BOOL bRet = AllocConsole();
	//if( !g_SimpleConsole.Init( &cdesc ) )
	//{
	//	ASSERT( !"Solar Console Initialization Failed!" );
	//	return FALSE;
	//}

	return TRUE;
}

