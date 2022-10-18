//=======================================================================================================================
/** StartScene
	@author
			���翵
	@since
			
	@remarks
*/

#pragma once
#include "scenebase.h"

#define START_TEST_WORK			TRUE		// �̱۸��� Test UI Scene���� ����.

class StartScene : public SceneBase
{
	int		m_iRenderCount;

	BOOL   m_bInitialized;

	BOOL   m_bInitInterface;
	BOOL   m_bRenderLoadImg;

public:
	StartScene(void);
	virtual ~StartScene(void);

	void Activate();						//	Ȱ��ȭ �Ǿ��� ��
	BOOL Init();							//	�ʱ�ȭ, �ε�
	BOOL Process( DWORD dwTick );			//	���� ���
	BOOL Render( DWORD dwTick );			//	���� ������
	void Release();							//	����
	void ParsePacket( MSG_BASE * pMsg );	//  ��Ʈ��ũ ��Ŷ ó��
	virtual void        IncreaseLoadingProcess();

	// �ؽ�Ʈ ���
protected:

	void RenderText( void );

	void DecideScene();

	void loadGameInfo();
};

extern StartScene g_StartScene;
