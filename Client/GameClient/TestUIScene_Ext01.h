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

	virtual void Activate();						//	Ȱ��ȭ �Ǿ��� ��
	virtual BOOL Init();							//	�ʱ�ȭ, �ε�
	virtual BOOL Process( DWORD dwTick );			//	���� ���
	virtual BOOL Render( DWORD dwTick );			//	���� ������
	virtual void Release();							//	����
	virtual void ParsePacket( MSG_BASE * pMsg );	//  ��Ʈ��ũ ��Ŷ ó��

	virtual void RenderText();
	virtual void ProcessKeyInput(DWORD dwTick);

private:
	BOOL			m_bScanMode;
	TCHAR			m_szTest[512];
};

extern TestUIScene_Ext01 g_TestUIScene_Ext01;

#endif 
// TESTUISCENE_EXT01_H