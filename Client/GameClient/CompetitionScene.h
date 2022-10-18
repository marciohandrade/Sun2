#pragma once
#include "battlescene.h"

enum
{
    eCOMPETITION_SCENE_NONE = 0,
    eCOMPETITION_SCENE_START_READY,
    eCOMPETITION_SCENE_START,
    eCOMPETITION_SCENE_COMPETITON,
    eCOMPETITION_SCENE_END,
};

enum
{
    eCOMPETITION_RENDER_NONE = 0,
    eCOMPETITION_RENDER_WIN,
    eCOMPETITION_RENDER_LOSE,
    eCOMPETITION_RENDER_TIE,
};

class compStatus
{
public:
    compStatus()
    { clear(); }

    void clear()
    {
        m_dwTickStart = 0;
        m_status = eCOMPETITION_SCENE_NONE;
        m_renderstatus = eCOMPETITION_RENDER_NONE;
    }

    DWORD               m_dwTickStart;
    DWORD               m_dwTickCompStart;
    SHORT               m_status;
    SHORT               m_renderstatus;
};

class compWin
{
public:
    compWin()
    { clear(); }

    void clear()
    {
		m_wWinPoint = 0;
       	m_i64Exp = 0;
        m_CurMoney = 0;
		m_PlusPVPPoint = 0;
		m_wKillPoint = 0;
		m_wOtherKillPoint = 0;
        ZeroMemory (&m_ItemInfo, sizeof (INVENTORY_TOTAL_INFO));
    }

	WORD					m_wWinPoint;			// 승리 포인트
    
	EXPTYPE					m_i64Exp;

    MONEY					m_CurMoney;				// 현재 하임
    INVENTORY_TOTAL_INFO	m_ItemInfo;				// 획득 아이템
	WORD					m_PlusPVPPoint;			// 획득 PVP 포인트
    WORD					m_wKillPoint;			// 현재 킬포인트

	WORD					m_wOtherKillPoint;		// 상대팀 킬 포인트
};

class compLose
{
public:
    compLose()
    {}
};

class TestUIScene_Ext01;

class CompetitionScene :public BattleScene
{
public:
	friend TestUIScene_Ext01;

	CompetitionScene(void);
	virtual ~CompetitionScene(void);

	virtual BOOL		Init( void );					//	초기화, 로딩
    virtual BOOL        Render( DWORD dwTick );			//	메인 랜더링
	virtual BOOL		Process( DWORD dwTick );		//	메인 계산
	virtual void		Release( void );				//	종료
    virtual void	    ParsePacket( MSG_BASE * pMsg );	
	virtual void		ProcessKeyInput( DWORD dwTick );

    void                OnUsePotion(BASE_ITEMINFO *pItemInfo);
    void				ConfirmDeadMessageBox();
    void                ResetProcessDeathMessage();
	void				ConfirmDeadMessageBoxBeforeStart();

    virtual void        DoSomethingPacket(BYTE byProtocol);

	RELATIONKIND::TYPE  GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy );

private:
    void                renderTimer();
    void                renderWin();
    void                renderLose();

private:
    compStatus          m_statusInfo;
    compWin             m_winInfo;
    compLose            m_loseInfo;

	WORD				m_wMyTeamKey;

    bool                m_bDeathMessage;
    DWORD               m_dwDeathStartTime;
    bool                m_ProcessDeathMessage;

	RECT				m_baseRect;
};

extern CompetitionScene		g_CompetitionScene;
