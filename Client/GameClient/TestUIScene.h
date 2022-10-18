#ifndef     __TEST_UI_SCENE_H__
#define     __TEST_UI_SCENE_H__
//------------------------------------------------------------------------------
/**
    @class TestUIScene

    User interface's test

    (C) 2005 ckbang, y2jinc
*/
#include "scenebase.h"
#include "IconImage.h"

#include "BattleScene.h"

class TestCase;

class TestUIScene :
	public BattleScene
{
public:
	TYPEDEF_MAP(TEST_CASE, SUN_STRING, TestCase *);

	TestUIScene(void);
	virtual ~TestUIScene(void);

	virtual void Activate();						//	활성화 되었을 때
	virtual BOOL Init();							//	초기화, 로딩
	virtual BOOL Process( DWORD dwTick );			//	메인 계산
	virtual BOOL Render( DWORD dwTick );			//	메인 랜더링
	virtual void Release();							//	종료
	virtual void ParsePacket( MSG_BASE * pMsg );	//  네트워크 패킷 처리

	virtual void RenderText( DWORD dwTick );
	virtual void ProcessKeyInput();

	virtual BOOL AttachTestCase(const SUN_STRING & strTestCaseName);
	virtual BOOL DeAttachTestCase(const SUN_STRING & strTestCaseName);

	virtual BOOL EntMessage(WORD wMessage, int iParam1, int iParam2, int iParam3 = 0, int iParam4 = 0);

	BOOL		 RenderMap( DWORD dwTick );

protected:
	TEST_CASE_MAP		m_TestCaseMap;
private:
	//	동영상 촬영용 프리 카메라
	WzVector			m_FreeCameraVector;
};

extern TestUIScene g_TestUIScene;

//------------------------------------------------------------------------------
#endif  // __TEST_UI_SCENE_H__