// ���� ifdef ����� DLL���� ���������ϴ� �۾��� ���� �� �ִ� ��ũ�θ� ����� 
// ǥ�� ����Դϴ�. �� DLL�� ��� �ִ� ������ ��� ����ٿ� ���ǵ� ARMORKITDLL_EXPORTS ��ȣ��
// �����ϵǸ�, ������ DLL�� ����ϴ� �ٸ� ������Ʈ������ �� ��ȣ�� ������ �� �����ϴ�.
// �̷��� �ϸ� �ҽ� ���Ͽ� �� ������ ��� �ִ� �ٸ� ��� ������Ʈ������ 
// ARMORKITDLL_API �Լ��� DLL���� �������� ������ ����,
// �� DLL�� �ش� ��ũ�η� ���ǵ� ��ȣ�� ���������� ������ ���ϴ�.
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



	VOID	SetActionFlags( DWORD dwSetActionFlags );	// �׼� �÷��׸� �����ϴ� �Լ�		
	VOID	SetActionMode ( DWORD dwSetActionMode, DWORD dwFrequency );	// �׼Ǹ�带 �����ϴ� �Լ�	
	void SetUseFileLog(const BOOL Value);
	INT		Init(LPFN_USERCALLBACK lpfnUserCallBack, TCHAR	*lpVersion);	// Armorkit�� �ʱ�ȭ�ϴ� �Լ�, ���� ���� ��� �߰�
	BOOL	Start() ;									// Armorkit�� ������Ű�� �Լ�
	VOID	Release() ;									// Armorkit�� �ڿ��� ������Ű�� �Լ�

	INT		SetPatternList(TCHAR *list, int length);
	INT		GenerateSid( int seed, char *sid );         // Server���� �ﾲüũ ���� �Լ�

};
ARMORKITDLL_API int AKDLL_GetTouchValue(int seed);


