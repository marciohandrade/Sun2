//=======================================================================================================================
/** StartScene
	@author
			유재영
	@since
			
	@remarks
*/

#pragma once
#include "scenebase.h"

#define START_TEST_WORK			TRUE		// 싱글모드시 Test UI Scene으로 시작.

class StartScene : public SceneBase
{
	int		m_iRenderCount;

	BOOL   m_bInitialized;

	BOOL   m_bInitInterface;
	BOOL   m_bRenderLoadImg;

public:
	StartScene(void);
	virtual ~StartScene(void);

	void Activate();						//	활성화 되었을 때
	BOOL Init();							//	초기화, 로딩
	BOOL Process( DWORD dwTick );			//	메인 계산
	BOOL Render( DWORD dwTick );			//	메인 랜더링
	void Release();							//	종료
	void ParsePacket( MSG_BASE * pMsg );	//  네트워크 패킷 처리
	virtual void        IncreaseLoadingProcess();

	// 텍스트 출력
protected:

	void RenderText( void );

	void DecideScene();

	void loadGameInfo();
};

extern StartScene g_StartScene;
