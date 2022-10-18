#pragma once
#include "battlescene.h"


class TestUIScene_Ext01;

class FieldScene :public BattleScene
{
public:
	friend TestUIScene_Ext01;

	FieldScene(void);
	virtual ~FieldScene(void);

	virtual BOOL		Init( void );					//	�ʱ�ȭ, �ε�
	virtual BOOL		Process( DWORD dwTick );		//	���� ���
	virtual void		Release( void );				//	����

	virtual void		ParsePacket( MSG_BASE * pMsg );	//  ��Ʈ��ũ ��Ŷ ó��
	virtual void		OnRecvCG_STATUS( MSG_BASE * pMsg );

	RELATIONKIND::TYPE  GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy );

protected:
	void				verifyDeadMessageBox();

public:
	BOOL				m_bPendingResurrection;
	DWORD				m_dwRenderTime;

};


extern FieldScene		g_FieldScene;

