#pragma once

#include "QuestDefine.h"
#include "QuestStruct.h"

//==================================================================================================

class QuestManager;
class QuestAction
{
public:
	QuestAction(void);
	virtual ~QuestAction(void);

	virtual void DoAccept( void * pvParam ) { __UNUSED(pvParam); }
	virtual void DoComplete( void * pvParam ) { __UNUSED(pvParam); }
	virtual eQUEST_ACTION_TYPE GetType() = 0;

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
#define	BEGIN_DECL_QACTION( ConcreteAction )							\
	class ConcreteAction : public QuestAction							\
	{																	\
	public:																\
		ConcreteAction(){}												\
		virtual ~ConcreteAction(){}										\
		inline ConcreteAction##_INFO * GetInfo() { return m_pInfo;}		\
		inline void SetInfo( ConcreteAction##_INFO * p ) { m_pInfo=p;}	\
		virtual eQUEST_ACTION_TYPE GetType() { return m_pInfo->GetType(); }	\
	private:															\
		ConcreteAction##_INFO * m_pInfo;

#define END_DECL_QACTION	\
	};