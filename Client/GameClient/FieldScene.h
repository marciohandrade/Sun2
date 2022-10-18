#pragma once
#include "battlescene.h"


class TestUIScene_Ext01;

class FieldScene :public BattleScene
{
public:
	friend TestUIScene_Ext01;

	FieldScene(void);
	virtual ~FieldScene(void);

	virtual BOOL		Init( void );					//	초기화, 로딩
	virtual BOOL		Process( DWORD dwTick );		//	메인 계산
	virtual void		Release( void );				//	종료

	virtual void		ParsePacket( MSG_BASE * pMsg );	//  네트워크 패킷 처리
	virtual void		OnRecvCG_STATUS( MSG_BASE * pMsg );

	RELATIONKIND::TYPE  GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy );

protected:
	void				verifyDeadMessageBox();

public:
	BOOL				m_bPendingResurrection;
	DWORD				m_dwRenderTime;

};


extern FieldScene		g_FieldScene;

