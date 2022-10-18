//=======================================================================================================================
/**  
@author 
		jjumbo
@since 
		2007. 12. 11

@remarks
		����.
		�׽�Ʈ ����� ������ TEST_MODULE_START(0~9) ��ũ�� �̴�.
		�׽�Ʈ ����� �б�� TEST_MODULE_ELSE ��ũ�� �̴�. ( m_bTest �� True OR False ������)
		�׽�Ʈ ����� ���� TEST_MODULE_END ��ũ�� �̴�.
		�׽�Ʈ ����� TEST_MODULE_FORCE_END(0~9) ��ũ�δ� m_bTest�� ������ False ��Ų��.(�ݺ� �Ǽ� ����Ǵ� ���� ���´�)
		�׽�Ʈ ����� Ȱ��ȭ/ ��Ȱ��ȭ�� ������ NUMPAD + 0~9 ��ư�̴�.
		�׽�Ʈ ����� �⺻���� TRUE�̴�. 

		�׽�Ʈ ����� VK_ADD(Ű���� ���� ������ + ��ư)�� �������μ� ȭ�鿡 Status ������ �����ٰ����� �����Ѵ�.

���� 
		�׽�Ʈ ����� �������� ���ǹǷ�, �׽�Ʈ �� ���� �ؾ� �Ѵ�.( CheckIn ����)
*/
//=======================================================================================================================

#pragma  once
#include "Singleton.h"

#ifdef MAKE_TEST_MODULE_MACRO


#define START_TEST_MODULE(iTestModule, ModuleName) \
	TestModule::Instance()->SetModuleName(iTestModule, #ModuleName); \
	if( TestModule::Instance()->GetTestMode(iTestModule) ) { \

#define END_TEST_MODULE } 
#define ELSE_TEST_MODULE } else { 
#define FORCE_END_TEST_MODULE(iTestCase) TestModule::Instance()->SetTestMode(iTestCase, FALSE); }


class TestModule : public Singleton<TestModule>
{
	struct stModuleStatus
	{
		bool			bTest;
		util::_tstring	tszModuleName;
	};

#define NUM_TEST_CASE 10
#define UPDATE_TICK	200
	

public:
    enum 
    {
        TestMode_Show_Damage_info = 9,
    };

	TestModule();
	~TestModule();

	void	SetTestMode(int iTestCase, bool val);
	void	ToggleTestMode(int iTestCase);
	bool	GetTestMode(int iTestCase);
	void	ProceeInput(DWORD dwTick);
	void	TestStatusTextOut();
	void	SetModuleName(int iTestCase, TCHAR * pModuleName);
	
protected:
	bool	invariant(int iTestCase);

private:
	DWORD				m_dwUpdateTick;
	bool				m_bStatusTextOut;
	stModuleStatus		m_stModuleState[NUM_TEST_CASE];

};

#endif //MAKE_TEST_MODULE_MACRO