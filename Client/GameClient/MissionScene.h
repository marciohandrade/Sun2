//=======================================================================================================================
/** StartScene
	@author
			유재영
	@since
			
	@remarks
*/

#pragma once
#include "scenebase.h"
#include "battlescene.h"
#include "MapArea.h"



class TestUIScene_Ext01;

class MissionScene : public BattleScene
{
public:
	friend TestUIScene_Ext01;

	MissionScene(void);
	virtual ~MissionScene(void);

public:
	// virtual methods
	void Activate( void );					//	활성화 되었을 때
	BOOL Init( void );						//	초기화, 로딩
	BOOL Process( DWORD dwTick );			//	메인 계산
	BOOL Render( DWORD dwTick );			//	메인 랜더링
	void Release( void );					//	종료

	virtual void ParsePacket( MSG_BASE * pMsg );	//  네트워크 패킷 처리
	virtual void OnRecvCG_STATUS( MSG_BASE * pMsg );
	virtual void OnRecvCG_MISSION( MSG_BASE * pMsg );
	void    InitMissionTime();
	
	virtual void	ProcessKeyInput();

	void	SetLeavePartyBan()			{ m_bLeave_Party_Ban = true;	}
	bool	IsLeavePartyBan()			{ return m_bLeave_Party_Ban;	}

	virtual RELATIONKIND::TYPE GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy );


protected:
	void	verifyDeadMessageBox();

	bool	m_bLeave_Party_Ban;		// 미션중 파티에서 강퇴당함


private:
};

extern MissionScene		g_MissionScene;