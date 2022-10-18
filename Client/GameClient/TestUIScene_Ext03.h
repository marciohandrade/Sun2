#ifndef TESTUISCENE_EXT03_H
#define TESTUISCENE_EXT03_H

#pragma once
#include "testuiscene.h"

class TestUIScene_Ext03 :
	public SceneBase
{
public:
	TestUIScene_Ext03(void);
	virtual ~TestUIScene_Ext03(void);

	virtual void Activate();						//	활성화 되었을 때
	virtual BOOL Init();							//	초기화, 로딩
	virtual BOOL Process( DWORD dwTick );			//	메인 계산
	virtual BOOL Render( DWORD dwTick );			//	메인 랜더링
	virtual void Release();							//	종료
	virtual void ParsePacket( MSG_BASE * pMsg );	//  네트워크 패킷 처리

	virtual void RenderText();
	virtual void ProcessKeyInput(DWORD dwTick);

private:
};

extern TestUIScene_Ext03 g_TestUIScene_Ext03;

#endif 
// TESTUISCENE_EXT03_H