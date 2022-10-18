#ifndef     __TEST_UI_SCENE_EXT00_H__
#define     __TEST_UI_SCENE_EXT00_H__
//------------------------------------------------------------------------------
/**
    @class TestUIScene

    User interface's test

    (C) 2005 ckbang
*/
#include "TestUIScene.h"

class TestUIScene_Ext00 :
	public SceneBase
{
public:
	TestUIScene_Ext00(void);
	virtual ~TestUIScene_Ext00(void);

	virtual void Activate();						//	활성화 되었을 때
	virtual BOOL Init();							//	초기화, 로딩
	virtual BOOL Process( DWORD dwTick );			//	메인 계산
	virtual BOOL Render( DWORD dwTick );			//	메인 랜더링
	virtual void Release();							//	종료
	virtual void ParsePacket( MSG_BASE * pMsg );	//  네트워크 패킷 처리

	virtual void RenderText();
	virtual void ProcessKeyInput(DWORD dwTick);
};

// global instance
extern TestUIScene_Ext00 g_TestUIScene_Ext00;

//------------------------------------------------------------------------------
#endif  // __TEST_UI_SCENE_EXT00_H__