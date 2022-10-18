#include "SunClientPrecompiledHeader.h"


#ifdef MAKE_TEST_MODULE_MACRO

TestModule::TestModule() :m_dwUpdateTick(0), m_bStatusTextOut(FALSE)
{
	for( int i = 0; i < NUM_TEST_CASE; i++)
	{
		m_stModuleState[i].bTest = TRUE;
	}
}

TestModule::~TestModule()
{

}

bool TestModule::invariant(int iTestCase) 
{
	if( iTestCase <0 || iTestCase >= NUM_TEST_CASE )
		return false;
	else 
		return true;
}

void TestModule::ProceeInput(DWORD dwTick)
{
	m_dwUpdateTick += dwTick;

	if( m_dwUpdateTick > UPDATE_TICK )
	{
		for ( int i = 0 ; i < NUM_TEST_CASE; i++)
		{
			if( GetAsyncKeyState(VK_NUMPAD0 +i)  )
			{
				ToggleTestMode(i);
			}
		}
		m_dwUpdateTick = 0;

		if( GetAsyncKeyState(VK_ADD))
		{
			m_bStatusTextOut ^= 1;
		}
	}
}

void TestModule::TestStatusTextOut()
{
	if( !m_bStatusTextOut)
		return;

	int iStartY = 200;
	int iStartX = 200; 
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( "mn12" );

	WzColor wzTextColor = WzColor_RGB( 0x0F, 255, 0x0f ) ;
	for( int i = 0; i < NUM_TEST_CASE; i++)
	{

		iStartY += 20;
		if( m_stModuleState[i].bTest )
			Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, _T("%s") ,"TRUE");
		else
			Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, _T("%s") ,"FALSE");
		g_pSunRenderer->x_pManagerTextOut->DrawText(szMessage, iStartX , iStartY, wzTextColor);

		sprintf(szMessage, "%d: ModuleName = %s" ,i, m_stModuleState[i].tszModuleName.c_str());
		g_pSunRenderer->x_pManagerTextOut->DrawText(szMessage, iStartX + 50, iStartY, wzTextColor);
		
	}

}

void TestModule::ToggleTestMode(int iTestCase) 
{
	if( !invariant(iTestCase) )
		return;
	m_stModuleState[iTestCase].bTest ^= 1;
}

bool TestModule::GetTestMode(int iTestCase) 
{
	if( !invariant(iTestCase) )
		return FALSE;
	return m_stModuleState[iTestCase].bTest;
}

void TestModule::SetModuleName(int iTestCase, TCHAR * pModuleName)
{
	if( !invariant(iTestCase) )
		return;

	m_stModuleState[iTestCase].tszModuleName = pModuleName;
}

void TestModule::SetTestMode(int iTestCase, bool val)
{
	if( !invariant(iTestCase) )
		return;

	m_stModuleState[iTestCase].bTest = val;
}

#endif // MAKE_TEST_MODULE_MACRO