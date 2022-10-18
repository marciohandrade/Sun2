#pragma once

#include "QuestDefine.h"
class QuestActionInfo
{
public:
	QuestActionInfo(eQUEST_ACTION_TYPE eType):m_Type(eType) {}
	virtual ~QuestActionInfo(void){}

	virtual BOOL DoAccept( void * pvParam ) { __UNUSED(pvParam); return FALSE; }
	virtual BOOL DoComplete( void * pvParam ) { __UNUSED(pvParam); return FALSE; }

private:

	__PROPERTY( eQUEST_ACTION_TYPE, Type )
};

//-----------------------------------------------------------------------
//
// 하위 클래스 선언 정의문
//
//-----------------------------------------------------------------------
#define	BEGIN_DECL_QACTION_INFO( ConcreteAction )						\
	class ConcreteAction##_INFO : public QuestActionInfo				\
	{																	\
	public:																\
		ConcreteAction##_INFO():QuestActionInfo(e##ConcreteAction){}	\
		virtual ~ConcreteAction##_INFO(){}

#define END_DECL_QACTION_INFO	\
	};
