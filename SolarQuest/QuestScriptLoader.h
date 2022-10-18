#pragma once
//=======================================================================================================================
/// QuestScriptLoader class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 3. 23
	@remarks
			- 퀘스트 스크립트를 로딩하여 각각의 Conditon, Action을 생성하고 
			  QuestInfo를 완성하여 QuestInfoManager에 넣어주는 클래스
	@history
			- 2006.4.20 : 연합퀘스트를 위한 Rebuilding()함수 추가
*/
//=======================================================================================================================


class QuestInfo;
class QuestInfoManager;
class QuestActionInfo;
class QuestConditionInfo;
struct QuestScriptRecode;

#if defined(_SERVER)
class IEventRegisterForNpcKillByQuest	// 좀더 일반화 시켜야 하는데...
{
public:
	virtual			~IEventRegisterForNpcKillByQuest() {}
	virtual	BOOL	RegisterNpcKillInfo(WORD/*QCODE*/ questCode, MONSTERCODE npcCode) = 0;
};
#else
class IEventRegisterForNpcKillByQuest;
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
class QuestScriptLoader : public IParser
{
public:
	QuestScriptLoader(void);
	~QuestScriptLoader(void);

	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
			BOOL			LoadQuestInfo( BOOL bReload );
			VOID			SetQuestInfoManager( QuestInfoManager* pQuestInfoManager ){ m_pQuestInfoManager = pQuestInfoManager; }
			VOID			SetEventRegister( IEventRegisterForNpcKillByQuest*	pEventRegister){ m_pEventRegister = pEventRegister; }
	virtual VOID			Release();

protected:
	VOID	Optimizing( QuestInfo* pInfo );
	BOOL	Validating( QuestConditionInfo* pInfo );
	BOOL	Validating( QuestActionInfo* pInfo );
#if !defined(_NA_004157_20120409_QUEST_BANDWITH_INCREASING)
	VOID	Rebuilding( QuestInfoManager& rMNGR );
#endif
	//VOID	Case( util::SolarFileReader & sr, QuestInfo* pInfo, BYTE type );

    //{__NA_0_20090407_QUESTSCRIPT_LOAD_MODIFY
	BOOL		_Validating( const QuestScriptRecode& Recode );
	QuestInfo*	_newQuestInfo( const QuestScriptRecode& Recode );

	BOOL AddAcceptAction( const QuestScriptRecode& Recode, QuestInfo* pInfo );
	BOOL AddAbandonAction( const QuestScriptRecode& Recode, QuestInfo* pInfo );
	BOOL AddCompleteAction( const QuestScriptRecode& Recode, QuestInfo* pInfo );
	BOOL AddAcceptCondition( const QuestScriptRecode& Recode, QuestInfo* pInfo );
	BOOL AddCompleteCondition( const QuestScriptRecode& Recode, QuestInfo* pInfo );
    //}

private:
	QuestInfoManager*					m_pQuestInfoManager;
	IEventRegisterForNpcKillByQuest*	m_pEventRegister;
};
