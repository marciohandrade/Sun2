//=======================================================================================================================
/** StartScene
	@author
			���翵
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
	void Activate( void );					//	Ȱ��ȭ �Ǿ��� ��
	BOOL Init( void );						//	�ʱ�ȭ, �ε�
	BOOL Process( DWORD dwTick );			//	���� ���
	BOOL Render( DWORD dwTick );			//	���� ������
	void Release( void );					//	����

	virtual void ParsePacket( MSG_BASE * pMsg );	//  ��Ʈ��ũ ��Ŷ ó��
	virtual void OnRecvCG_STATUS( MSG_BASE * pMsg );
	virtual void OnRecvCG_MISSION( MSG_BASE * pMsg );
	void    InitMissionTime();
	
	virtual void	ProcessKeyInput();

	void	SetLeavePartyBan()			{ m_bLeave_Party_Ban = true;	}
	bool	IsLeavePartyBan()			{ return m_bLeave_Party_Ban;	}

	virtual RELATIONKIND::TYPE GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy );


protected:
	void	verifyDeadMessageBox();

	bool	m_bLeave_Party_Ban;		// �̼��� ��Ƽ���� �������


private:
};

extern MissionScene		g_MissionScene;