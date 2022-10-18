#ifndef TESTUISCENE_EXT02_H
#define TESTUISCENE_EXT02_H

#pragma once
#include "testuiscene.h"
#include "FTextRender.h"

class TestUIScene_Ext02 :
	public SceneBase
{
public:
	TestUIScene_Ext02(void);
	virtual ~TestUIScene_Ext02(void);

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
	FTextRender		m_TextRender;
	TCHAR			m_szTest[512];
};

extern TestUIScene_Ext02 g_TestUIScene_Ext02;

#endif 
// TESTUISCENE_EXT02_H