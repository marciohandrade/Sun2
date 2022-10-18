#pragma once
//=======================================================================================================================
/// QuestManager_Concrete class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 3. 29
	@remarks
			- 로직에서 퀘스트를 다룰 때 사용하는 통로 클래스
	@description
			- QuestManager를 상속
			- QuestManager의 virtual VOID OnEvent( Quest* pQuest )를 구현 상속

	@scenario 
			1. DB로부터 (진행중and완료된)퀘스트의 정보를 받아서 Player마다 Quest객체를 QuestManager를 통해서 생성한다(CreateQuest())
			2. 초기화된 QuestManager_Concrete는 이벤트가 발생시에 로직의 이벤트(Event())를 호출해주고 몬스터 킬일 경우 클라이언트로 알려준다
			3. 클라이언트의 패킷의 요청에 따라 조건체크와 액션이 일어나고, 응답을 해준다
	@history
			- 
*/
//=======================================================================================================================

#include <QuestManager.h>
#include "QuestCondition_Concrete.h"
#include "QuestAction_Concrete.h"

class Quest;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<QuestManager_Concrete>
class QuestManager_Concrete : public QuestManager
{
public:		QuestManager_Concrete( QuestInfoManager* pmgr ) : QuestManager(pmgr) {}
	virtual ~QuestManager_Concrete() { Release(); }

//	DB로 부터 왔을 때
// 퀘스트 객체를 미리 만들어서 놓는다.Manager에 Quest를 등록해둔다.
// 그리고 MonsterKillCondition이 add될때 Establish한다?
// 
//			VOID	SerializeStream( QUEST_PROGR_TOTAL_INFO & IN rTotalInfo, eQUEST_SERIALIZE eType );

// 클라이언트에서만 호출해준다. 굳이 서버에서는 시간 체크는 부하를 줄 필요없다.
//			VOID Update();

// 로직에서 호출해준다.
//			VOID Event( eQUEST_EVENT evt, QUEST_MSG* pMsg );
//			VOID Event( QCODE QCode, eQUEST_EVENT evt, QUEST_MSG* pMsg );


// 처리완료 ------------------------------------------------------------------------------------
// 클라이언트로 부터 조건체크 패킷이 온 순간 처리해 준다
//			BOOL CanAccept( QCODE qCode, VOID* pvParam );
//			BOOL CanComplete( QCODE qCode, VOID* pvParam );

	static	VOID			Init( WORD wPoolSize );

			VOID			Init( Player* pPlayer );
			VOID			Release();

			VOID			SendQuestStream();
			//VOID			RecvQuestStream( MSG_CG_QUEST_STREAM_CMD* pcmd );
            BOOLEAN MakeInfoQuestNpcNaviStream(MSG_CG_QUEST_OBJECT_NAVI_POS_CMD* INOUT pMsg,
                                               QCODE questCode = 0);
            BOOLEAN OnEventNpcSummonQuestAbandon();
            inline void RegisterNpcSummonEvent(MONSTERCODE npcCode, CODETYPE npcKey);
protected:
			// 콜벡이 되는 부분
	virtual VOID			OnEvent( eQUEST_EVENT evt, QUEST_MSG* pMsg, Quest* pQuest );

			// 하위 클래스에서 구현을 해야 하는 부분
	virtual QuestCondition*	CreateQuestCondition( QuestConditionInfo* pInfo );
	virtual QuestAction*	CreateQuestAction( QuestActionInfo* pInfo );
	virtual VOID			DestroyQuestCondition( QuestCondition* pCondition );
	virtual VOID			DestroyQuestAction( QuestAction* pAction );

	inline	VOID			SetPlayer( Player* pPlayer ) { m_pPlayer = pPlayer; }
    //__NA000000_090320_v0902_LOGIC_CHANGE_ABOUT_QUEST__
    enum eNpcSummonInfo { eNpcSummonInfo_Code, eNpcSummonInfo_Key, eNpcSummonInfo_Count };
    typedef DWORD NpcSummonInfo[eNpcSummonInfo_Count];
private:
	static	VarMsg2			m_VarMsg;
	Player*					m_pPlayer;
    NpcSummonInfo m_NpcSummonInfo;
    //
	static util::PoolRouter<eQUEST_ACTION_MAX>  ms_PoolRouter;
};


// (BUGFIX) (WAVERIX) (091018) change type for 'remove mob key' event on 'mob summon' quest abandon
// 이론... 몬스터 키 대역은 50001~이다... 퀘스트 취소시 몬스터가 제거되지 않을 수도 있었다.
inline void
QuestManager_Concrete::RegisterNpcSummonEvent(MONSTERCODE npcCode, CODETYPE npcKey)
{
    m_NpcSummonInfo[eNpcSummonInfo_Code] = npcCode;
    m_NpcSummonInfo[eNpcSummonInfo_Key] = npcKey;
}

