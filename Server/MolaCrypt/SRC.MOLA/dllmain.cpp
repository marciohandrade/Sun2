// AreaConquest.cpp : DLL ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
//#include "CryptManager.h"

// (WAVERIX_TEST) �ӽ÷� ������ ���̸�, �̰��� �ڵ忡�� �ʱ�ȭ�� �� �ֵ��� ����.
#if SERVER_CODE

static BOOL bG = CryptManager::Init1st();

#endif //

//static BOOL bG2 = CryptManager::InitSecureInfoParser();

#ifdef _USRDLL
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
#endif //_USRDLL

