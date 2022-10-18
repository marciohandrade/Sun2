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
	FTextRender		m_TextRender;
	TCHAR			m_szTest[512];
};

extern TestUIScene_Ext02 g_TestUIScene_Ext02;

#endif 
// TESTUISCENE_EXT02_H