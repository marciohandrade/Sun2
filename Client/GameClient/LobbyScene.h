//=======================================================================================================================
/** StartScene
	@author
			���翵
	@since
			
	@remarks
*/

#pragma once

#include "scenebase.h"
#include "BattleScene.h"

class LobbyScene : public BattleScene
{
public:
	LobbyScene(void);
	virtual ~LobbyScene(void);

	BOOL Init( void );						//	�ʱ�ȭ, �ε�
	void Release( void );					//	����

	void OnRecvCG_SYNC( MSG_BASE * pMsg );
};



extern	LobbyScene		g_LobbyScene;