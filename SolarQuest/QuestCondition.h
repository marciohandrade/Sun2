#pragma once

#include "QuestDefine.h"
#include "QuestStruct.h"

#include <ResultCode.h>

//==================================================================================================
//
class QuestManager;
class QuestCondition
{
public:
	QuestCondition(void);
	virtual ~QuestCondition(void);

	// 하위 클래스에서 개별적으로 구현한다
	virtual void Init( const QUESTPART_PROGR * pStream = NULL ) { __UNUSED(pStream); }
	virtual void Clear() {}
	virtual void CopyOut( QUESTPART_PROGR & OUT rStream ) { __UNUSED(&rStream); }
	virtual RC::eQUEST_RESULT CanAccept( void * pvParam ) { return RC::RC_QUEST_FAILED; }
	virtual RC::eQUEST_RESULT CanComplete( void * pvParam ) { return RC::RC_QUEST_FAILED; }
	virtual BOOL Event( eQUEST_EVENT evt, QUEST_MSG * pMsg ) { __UNUSED((evt, pMsg)); return FALSE; }
	virtual eQUEST_CONDITION_TYPE GetType() = 0;
	
	__PTR_PROPERTY(Quest, Quest)
	__PTR_PROPERTY(QuestManager, Manager)
};
//
//==================================================================================================

//-----------------------------------------------------------------------
//
// 하위 클래스 선언 정의문
//
//-----------------------------------------------------------------------
#define	BEGIN_DECL_QCONDITION( ConcreteCondition )							\
	class ConcreteCondition : public QuestCondition							\
	{																		\
	public:																	\
		ConcreteCondition(){}												\
		virtual ~ConcreteCondition(){}										\
		inline ConcreteCondition##_INFO * GetInfo() { return m_pInfo;}		\
		inline void SetInfo( ConcreteCondition##_INFO * p ) { m_pInfo=p;}	\
		virtual eQUEST_CONDITION_TYPE GetType() { return m_pInfo->GetType(); }	\
	private:																\
		ConcreteCondition##_INFO * m_pInfo;

#define END_DECL_QCONDITION	\
	};
