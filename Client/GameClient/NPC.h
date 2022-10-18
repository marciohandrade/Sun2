#pragma once
//------------------------------------------------------------------------------
/**

    월드 상의 NONE PLAYER CHARICTER 특성을 정의 
    @author
		유재영
    @since

    @remarks
        -2005.06.07 소스 정리(지완)
        - 
*/

#include "Character.h"

struct BASE_NPCImageInfo;
class SolarDialog;

// 임시적으로 오브젝트 키를 셋 해놓는다.
#define NPC_OBJECT_KEY_START			(NPC_OBJECT_KEY)
#define NPC_OBJECT_KEY_END				(NONCHARACTER_OBJECT_KEY-1)

#define NPC_SWING_DEFAULT_DELAY_TERM	5000	// (NPC_SWING_DEFAULT_DELAY_TERM/1000)초
#define NPC_SWING_RANDOM_DELAY_TERM		13000	// (NPC_SWING_DEFAULT_DELAY_TERM/1000)초

#define MAX_NPC_ROUTE                   10

enum eNPCVOICEKIND
{
	eNPCVOICEKIND_NONE = 0,
	eNPCVOICEKIND_WAIT = 1,
	eNPCVOICEKIND_MEET = 2,
	eNPCVOICEKIND_LEAVE = 3,
};

enum eNPCMOVETYPE
{
	eNPCMOVETYPE_NONE = 0,
	eNPCMOVETYPE_PATROL = 1,
	eNPCMOVETYPE_RANDOMMOVE = 2,
	
};

class NPC : public Character
{
    struct NPCQuestInfo
    {
        int max_quest_repeat_;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        bool is_have_daily_quest_;
        bool is_have_guild_quest_;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
        bool is_have_challenge_quest_;
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST
    };

public:
	NPC();
	virtual ~NPC();

	virtual BOOL	Create( DWORD dwObjectKey, DWORD dwID = 0 );
	virtual void	Destroy( void );
	virtual WzID    GetCurrentAttackAnimation();
	virtual WzID    GetCurrentIdleAnimation();
	virtual WzID    GetCurrentMoveAnimation();
	virtual WzID    GetCurrentDeathAnimation();
	virtual WzID    GetCurrentDamageAnimation(BOOL bRight,BOOL bCritical);
	WzID			GetCurrentResponseAnimation();
    WzID            GetCurrentQuestAcceptAnimation();
    WzID            GetCurrentQuestCompleteAnimation();
	WzID			GetCurrentSwingAnimation();
	void			SetMonsterInfo(BASE_NPCINFO *pInfo);
	BASE_NPCINFO *	GetMonsterInfo();

	virtual BOOL    Render();

	virtual void	SetExtraInfo( EXTRA_NPCINFO	* pExtraInfo);
	EXTRA_NPCINFO *	GetExtraInfo();

	virtual  void   RenderInfo();	
    virtual  void   InitMeshHeight();

	void			InitDelaySwingTick() { m_iDelaySwingTick = 0; }
	void			SetDelaySwingTick(DWORD dwTick);
	int				ProcessDelaySwingTick(DWORD dwTick);

    // 이름 설정 
    virtual void	SetName( const tstring& Name );

    virtual void		RestoreObjectName();


	// 스텟 관련
	virtual void		SetExp(EXPTYPE dwExp);
	virtual EXPTYPE		GetExp();
	virtual EXPTYPE		GetNextExp();

	virtual void		SetLevel(LEVELTYPE LV);
	virtual LEVELTYPE	GetLevel();

	virtual DWORD		GetMaxHP();
	virtual DWORD		GetMaxMP();

    void				RenderHPGuage( void );

	virtual BOOL		Process( DWORD dwTick );
	virtual BOOL        ProcessNPCVoice( DWORD dwTick);
	void				ProcessNPCMove(DWORD dwTick);

	VOID				SetOpenDialog(SolarDialog * pDlg);

	void                PlayMeetSound();
	void                PlayWaitSound();
	void                PlayLeaveSound();

	void                SetMoveType(int iType);
	void                SetRouteSize(int iSize);
	void                SetRoutePos(int iIndex,WzVector vPos);
	WzVector            GetNextRoutePos();   
    
	void				SetMaxQuestRepeat(int max_quest_repeat);
	int					GetMaxQuestRepeat() { return npc_quest_info_.max_quest_repeat_; }
	void				ClearQuestInfo();
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    void                SetHaveDailyQuest() { npc_quest_info_.is_have_daily_quest_ = true; }
    void                SetHaveGuildQuest() { npc_quest_info_.is_have_guild_quest_ = true; }
    bool                IsHaveDailyQuest()  { return npc_quest_info_.is_have_daily_quest_; }
    bool                IsHaveGuildQuest()  { return npc_quest_info_.is_have_guild_quest_; }
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
    void                SetHaveChallengeDailyQuest() { npc_quest_info_.is_have_challenge_quest_ = true; }
    bool                IsHaveChallengeDailyQuest() { return npc_quest_info_.is_have_challenge_quest_; } 
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST

    BASE_NPCImageInfo* GetNpcImageInfo();
    BASE_NPCINFO* GetNpcInfo(){ return &m_NPCInfo; }
	static void SetNpcVoiceIndex(int VoiceIndex)	{	NPC::m_sVoiceIndex = VoiceIndex; }
	static int GetNpcVoiceIndex()					{	return NPC::m_sVoiceIndex; }

#ifdef _NA_008185_20150402_COSTUME_ITEM_FOR_NPC
    virtual void SetCostumeItemCode( SLOTCODE code );
    virtual SLOTCODE GetCostumeItemCode();
#endif//_NA_008185_20150402_COSTUME_ITEM_FOR_NPC
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	virtual void SetDecorateCostumeItemCode( SLOTCODE code );
	virtual SLOTCODE GetDecorateCostumeItemCode();
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

private:
	void				closeOpenedDialog();

protected:
	DWORD				CalDelaySwingTick(DWORD dwTick);
	void                InitMoveInfo();
	int                 m_iMoveType;
	int                 m_iNumRoute;
	int                 m_iCurDest;
	WzVector            m_vRoute[MAX_NPC_ROUTE];
	int                 m_iMoveWaitDelay;

private:
	BASE_NPCINFO        m_NPCInfo;
	EXTRA_NPCINFO		m_ExtraInfo;

	int					m_iDelaySwingTick;
	SolarDialog *		m_pOpenDialogByNPC;
	
	int                 m_iWaitSoundDelay;
	static int			m_sVoiceIndex;

    NPCQuestInfo        npc_quest_info_;

#ifdef _NA_008185_20150402_COSTUME_ITEM_FOR_NPC
    SLOTCODE            npc_costume_item_;
#endif //_NA_008185_20150402_COSTUME_ITEM_FOR_NPC
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	SLOTCODE            npc_decorate_costume_item_;
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

public:
    static void InitNpcImageInfo(Character* character_ptr, BASE_NPCINFO* npc_info);
};

inline void NPC::SetMaxQuestRepeat(int max_quest_repeat)
{
    if (1 != npc_quest_info_.max_quest_repeat_)
    {
        npc_quest_info_.max_quest_repeat_ = max_quest_repeat;
    }
}

inline void NPC::ClearQuestInfo()
{
    npc_quest_info_.max_quest_repeat_ = 0;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    npc_quest_info_.is_have_daily_quest_ = false;
    npc_quest_info_.is_have_guild_quest_ = false;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
    npc_quest_info_.is_have_challenge_quest_ = false;
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST
}
