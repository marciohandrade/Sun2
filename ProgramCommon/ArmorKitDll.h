// 다음 ifdef 블록은 DLL에서 내보내기하는 작업을 쉽게 해 주는 매크로를 만드는 
// 표준 방식입니다. 이 DLL에 들어 있는 파일은 모두 명령줄에 정의된 ARMORKITDLL_EXPORTS 기호로
// 컴파일되며, 동일한 DLL을 사용하는 다른 프로젝트에서는 이 기호를 정의할 수 없습니다.
// 이렇게 하면 소스 파일에 이 파일이 들어 있는 다른 모든 프로젝트에서는 
// ARMORKITDLL_API 함수를 DLL에서 가져오는 것으로 보고,
// 이 DLL은 해당 매크로로 정의된 기호가 내보내지는 것으로 봅니다.
#ifdef ARMORKITDLL_EXPORTS
#define ARMORKITDLL_API __declspec(dllexport)
#else
#define ARMORKITDLL_API __declspec(dllimport)
#endif


#include "akcommon.h"
#include "CArmorKit.h"



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class ARMORKITDLL_API CArmorKitDll {
private:
	CArmorKit *m_ArmorKit;
public:
	CArmorKitDll(void);
	~CArmorKitDll(void);

	CArmorKit *CreateArmorKitObj(void)
	{
		return new CArmorKit;
	}



	VOID	SetActionFlags( DWORD dwSetActionFlags );	// 액션 플래그를 설정하는 함수		
	VOID	SetActionMode ( DWORD dwSetActionMode, DWORD dwFrequency );	// 액션모드를 설정하는 함수	
	void SetUseFileLog(const BOOL Value);
	INT		Init(LPFN_USERCALLBACK lpfnUserCallBack, TCHAR	*lpVersion);	// Armorkit을 초기화하는 함수, 버전 지정 기능 추가
	BOOL	Start() ;									// Armorkit을 구동시키는 함수
	VOID	Release() ;									// Armorkit의 자원을 해제시키는 함수

	INT		SetPatternList(TCHAR *list, int length);
	INT		GenerateSid( int seed, char *sid );         // Server와의 헬쓰체크 위한 함수

};
ARMORKITDLL_API int AKDLL_GetTouchValue(int seed);


