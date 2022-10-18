//=======================================================================================================================
/** InstanceDungeon Scene
@author
@since
@remarks
*/


#pragma once

#include "battlescene.h"

class InstanceDungeonScene : public BattleScene
{
public:

	enum InstanceDungeon_Packet_Status
	{
		INSTANCEDUNGEON_PACKET_STATUS_NONE		= 0,
		INSTANCEDUNGEON_PACKET_STATUS_PRESS_KEY = 1,
	};

	InstanceDungeonScene(void);
	virtual ~InstanceDungeonScene(void);

	virtual BOOL	Init( void );						//	�ʱ�ȭ, �ε�
	virtual BOOL	Process( DWORD dwTick );			//	���� ���
	virtual BOOL	Render( DWORD dwTick );				//	���� ������
	virtual void	Release( void );					//	����

	virtual void	ProcessKeyInput();

#ifdef _NA_007477_20140805_RADAR_RAYER_MODIFY
#else
	void			initInstanceDungeon( MAPCODE MapCode );
    void			SetCurLayer(int iCurLayer);
protected:
    void			initRadarLayer( MAPCODE MapCode );
public:
#endif //_NA_007477_20140805_RADAR_RAYER_MODIFY

	void			SetJoinState(BYTE byState)						{ m_byJoinState = byState; }
	BYTE			GetJoinState()									{ return m_byJoinState; }
	void			SetStartCheckTime(DWORD dwStartCheckTime)		{m_dwStartCheckTime = dwStartCheckTime;}
	DWORD			GetStartCheckTime()		 	 					{return m_dwStartCheckTime;}
	void			SetStartProcessTick(DWORD dwStartProcessTick)	{m_dwStartProcessTick = dwStartProcessTick;}
	DWORD			GetStartProcessTick()							{return m_dwStartProcessTick;}
	void			Send_MSG_CG_ETC_GET_BATTLE_OPEN_TIME_SYN();			

	virtual			RELATIONKIND::TYPE GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy );

protected:
	void			processInfo( DWORD dwTick );
	void			DisplayTime();

protected:
	// ��Ŷ ����
	virtual void	OnRecvCG_SYNC( MSG_BASE * pMsg );
	virtual void	OnRecvCG_STATUS( MSG_BASE * pMsg );
	virtual void	OnRecvCG_InstanceDungeon( MSG_BASE * pMsg );

	virtual void	ParsePacket( MSG_BASE * pMsg );

private:

	//	Į���� �̿ܿ� �ٸ� �δ� Ÿ���� �þ�� �迭�� �ϴ� �ַ� �ϴ� �ϲø��´�� �ø�����^_^
	//	���� �����ʿ��� �����ذ� �ϳ��ۿ� ��� �׳� �̴�� �ξ���ϴ�. ^_^ - BY KTW
	BYTE		m_byJoinState;
	DWORD		m_dwStartCheckTime;
	DWORD		m_dwStartProcessTick;

};

extern InstanceDungeonScene		g_InstanceDungeonScene;

