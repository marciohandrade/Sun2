#ifndef TESTUISCENE_EXT01_H
#define TESTUISCENE_EXT01_H

#pragma once
#include "testuiscene.h"
#include "FTextRender.h"

class TestCase;

class TestUIScene_Ext01 :
	public TestUIScene
{
public:
	TestUIScene_Ext01(void);
	virtual ~TestUIScene_Ext01(void);

	virtual void Activate();						//	활성화 되었을 때
	virtual BOOL Init();							//	초기화, 로딩
	virtual BOOL Process( DWORD dwTick );			//	메인 계산
	virtual BOOL Render( DWORD dwTick );			//	메인 랜더링
	virtual void Release();							//	종료
	virtual void ParsePacket( MSG_BASE * pMsg );	//  네트워크 패킷 처리

	virtual void RenderText();
	virtual void ProcessKeyInput(DWORD dwTick);

private:
	BOOL			m_bScanMode;
	TCHAR			m_szTest[512];
};

extern TestUIScene_Ext01 g_TestUIScene_Ext01;

#endif 
// TESTUISCENE_EXT01_H