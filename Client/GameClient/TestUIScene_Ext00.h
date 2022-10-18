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

	virtual void Activate();						//	Ȱ��ȭ �Ǿ��� ��
	virtual BOOL Init();							//	�ʱ�ȭ, �ε�
	virtual BOOL Process( DWORD dwTick );			//	���� ���
	virtual BOOL Render( DWORD dwTick );			//	���� ������
	virtual void Release();							//	����
	virtual void ParsePacket( MSG_BASE * pMsg );	//  ��Ʈ��ũ ��Ŷ ó��

	virtual void RenderText();
	virtual void ProcessKeyInput(DWORD dwTick);
};

// global instance
extern TestUIScene_Ext00 g_TestUIScene_Ext00;

//------------------------------------------------------------------------------
#endif  // __TEST_UI_SCENE_EXT00_H__