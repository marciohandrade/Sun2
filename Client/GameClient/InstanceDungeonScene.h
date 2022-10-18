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

	virtual BOOL	Init( void );						//	초기화, 로딩
	virtual BOOL	Process( DWORD dwTick );			//	메인 계산
	virtual BOOL	Render( DWORD dwTick );				//	메인 랜더링
	virtual void	Release( void );					//	종료

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
	// 패킷 관련
	virtual void	OnRecvCG_SYNC( MSG_BASE * pMsg );
	virtual void	OnRecvCG_STATUS( MSG_BASE * pMsg );
	virtual void	OnRecvCG_InstanceDungeon( MSG_BASE * pMsg );

	virtual void	ParsePacket( MSG_BASE * pMsg );

private:

	//	칼버스 이외에 다른 인던 타입이 늘어나면 배열로 하던 멀로 하던 니꼴리는대로 늘리세요^_^
	//	아직 서버쪽에서 정해준게 하나밖에 없어서 그냥 이대로 두었답니다. ^_^ - BY KTW
	BYTE		m_byJoinState;
	DWORD		m_dwStartCheckTime;
	DWORD		m_dwStartProcessTick;

};

extern InstanceDungeonScene		g_InstanceDungeonScene;

