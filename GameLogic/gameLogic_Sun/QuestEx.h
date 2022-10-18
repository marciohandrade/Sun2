#ifndef __QUESTEX_H__
#define __QUESTEX_H__

#include <ScriptCode.IParser.h>

#include <Struct.h>
#include <SolarFileReader.h>
#include <QuestScriptLoader.h>
#include <QuestInfoManager.h>
#include <QuestManager.h>

typedef struct _QUEST_INFO
{
	TCHAR pszName [256];
	TCHAR pszCategoryName [256];
	DWORD m_ExpireTime;
	DWORD m_QState;
	BYTE m_Num;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    BYTE is_daily;
    DWORD group_code;
#endif
} QUEST_INFO;

typedef std::map<QCODE, QUEST_INFO> QUEST_PRESET;

class QuestInfoManager2 : public QuestInfoManager
{
public:
	QuestInfoManager2(void) {}
	virtual ~QuestInfoManager2(void) {}

	BOOL Load( char * pszFileName );
	VOID Unload(){ReleaseEx();} // ko ReleaseEx
	
	QuestScriptLoader m_ScriptLoader;

	

	QUEST_PRESET m_QuestPreset;
private:
};

class QuestManager2 : public QuestManager
{
public:
	QuestManager2(QuestInfoManager* pmgr) : QuestManager(pmgr) {}

//	void GetQuest(QuestInfo* pQuest);

protected:
	// 하위 클래스에서 구현을 해야 하는 부분
	virtual QuestCondition * CreateQuestCondition( QuestConditionInfo * pInfo ) {return NULL;}
	virtual QuestAction * CreateQuestAction( QuestActionInfo * pInfo )  {return NULL;}
	virtual void DestroyQuestCondition( QuestCondition * pCondition ) {}
	virtual void DestroyQuestAction( QuestAction * pAction )  {}

	// 퀘스트 완료, 실패에 따른 이벤트 알림
	virtual void OnEvent( eQUEST_EVENT evt, QUEST_MSG * pMsg, Quest * pQuest ) {}
};

#endif //__QUESTEX_H__