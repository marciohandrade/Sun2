//=======================================================================================================================
/** StartScene
	@author
			유재영
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

	BOOL Init( void );						//	초기화, 로딩
	void Release( void );					//	종료

	void OnRecvCG_SYNC( MSG_BASE * pMsg );
};



extern	LobbyScene		g_LobbyScene;