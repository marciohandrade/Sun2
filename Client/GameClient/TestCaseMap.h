#pragma once
#include "testcase.h"
#include "BattleScene.h"

class TestCaseMap :
	public TestCase
{
public:
	TestCaseMap(void);
	virtual ~TestCaseMap(void);

	virtual BOOL Init();							//	초기화, 로딩
	virtual BOOL Process( DWORD dwTick );			//	메인 계산
	virtual BOOL Render( DWORD dwTick );			//	메인 랜더링
	virtual void Release();							//	종료
	virtual void ParsePacket( MSG_BASE * pMsg );	//  네트워크 패킷 처리

	virtual void RenderText();
	virtual void ProcessKeyInput(DWORD dwTick);

	virtual BOOL EntMessage(WORD wMessage, int iParam1, int iParam2, int iParam3 = 0, int iParam4 = 0);
};
