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

	virtual void Activate();						//	Ȱ��ȭ �Ǿ��� ��
	virtual BOOL Init();							//	�ʱ�ȭ, �ε�
	virtual BOOL Process( DWORD dwTick );			//	���� ���
	virtual BOOL Render( DWORD dwTick );			//	���� ������
	virtual void Release();							//	����
	virtual void ParsePacket( MSG_BASE * pMsg );	//  ��Ʈ��ũ ��Ŷ ó��

	virtual void RenderText();
	virtual void ProcessKeyInput(DWORD dwTick);

private:
};

extern TestUIScene_Ext03 g_TestUIScene_Ext03;

#endif 
// TESTUISCENE_EXT03_H