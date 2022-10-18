//=======================================================================================================================
/**  
@author 
		jjumbo
@since 
		2007. 12. 11

@remarks
		사용법.
		테스트 모듈의 시작은 TEST_MODULE_START(0~9) 메크로 이다.
		테스트 모듈의 분기는 TEST_MODULE_ELSE 메크로 이다. ( m_bTest 가 True OR False 에따라)
		테스트 모듈의 끝은 TEST_MODULE_END 메크로 이다.
		테스트 모듈의 TEST_MODULE_FORCE_END(0~9) 메크로는 m_bTest를 강제로 False 시킨다.(반복 되서 실행되는 것을 막는다)
		테스트 모듈의 활성화/ 비활성화의 선택은 NUMPAD + 0~9 버튼이다.
		테스트 모듈의 기본값은 TRUE이다. 

		테스트 모듈의 VK_ADD(키보드 제일 오른쪽 + 버튼)을 누름으로서 화면에 Status 정보를 보여줄것인지 결정한다.

주의 
		테스트 모듈은 공동으로 사용되므로, 테스트 후 삭제 해야 한다.( CheckIn 금지)
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