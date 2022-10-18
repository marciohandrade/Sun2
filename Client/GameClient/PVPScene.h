//=======================================================================================================================
/** PVP Scene(���� ��)
	@author
			���翵
	@since
			2005. 11. 18. - create
	@remarks
*/

#pragma once
#include "battlescene.h"
#include "PVPManager.h"


class PVPScene : public BattleScene
{
public:

	enum PVP_Packet_Status
	{
		PVP_PACKET_STATUS_NONE		= 0,
		PVP_PACKET_STATUS_PRESS_KEY = 1,
	};

	PVPScene(void);
	virtual ~PVPScene(void);

	virtual BOOL	Init( void );						//	�ʱ�ȭ, �ε�
	virtual BOOL	Process( DWORD dwTick );			//	���� ���
	virtual BOOL	Render( DWORD dwTick );				//	���� ������
	virtual void	Release( void );					//	����

	virtual void	ProcessKeyInput();

	virtual RELATIONKIND::TYPE GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy );

	
protected:
	VOID			initPVP();
	VOID			processInfo( DWORD dwTick );

	// PVP ���� ����
	VOID			showPVPInfoDialog();
	VOID			updatePVPInfo();

	VOID			InvincibleToAllUser();
	VOID			UnInvincibleToAllUser();


protected:
	// ��Ŷ ����
	virtual void	OnRecvCG_SYNC( MSG_BASE * pMsg );
	virtual void	OnRecvCG_STATUS( MSG_BASE * pMsg );
	virtual void	OnRecvCG_PVP( MSG_BASE * pMsg );
	
	virtual void	ParsePacket( MSG_BASE * pMsg );	

private:
	//PVP_PRESS_KEY_LEAVE_TIME
	util::Timer *	m_pPressAnykeyDelay;
	util::Timer *	m_pStartMatchDelay;
	PVPManager		m_PVPManager;
	BOOL			m_bCheckPressAnyKey;
	
};

extern PVPScene		g_PVPScene;
