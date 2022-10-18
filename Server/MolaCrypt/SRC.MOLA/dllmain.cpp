// AreaConquest.cpp : DLL 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
//#include "CryptManager.h"

// (WAVERIX_TEST) 임시로 설정한 것이며, 이것은 코드에서 초기화할 수 있도록 하자.
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

