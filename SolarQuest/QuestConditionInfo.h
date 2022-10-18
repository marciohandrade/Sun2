#pragma once

#include "QuestDefine.h"
class QuestConditionInfo
{
public:
	QuestConditionInfo(eQUEST_CONDITION_TYPE eType):m_Type(eType) {}
	virtual ~QuestConditionInfo(void) {}

	virtual BOOL CanAccept( void * pvParam ) { __UNUSED(pvParam); return FALSE; }
	virtual BOOL CanComplete( void * pvParam ) { __UNUSED(pvParam); return FALSE; }

private:
	__PROPERTY( eQUEST_CONDITION_TYPE, Type )
};


//-----------------------------------------------------------------------
//
// ���� Ŭ���� ���� ���ǹ�
//
//-----------------------------------------------------------------------
#define	BEGIN_DECL_QCONDITION_INFO( ConcreteCondition )							\
	class ConcreteCondition##_INFO : public QuestConditionInfo					\
	{																			\
	public:																		\
		ConcreteCondition##_INFO():QuestConditionInfo(e##ConcreteCondition){}	\
		virtual ~ConcreteCondition##_INFO(){}

#define END_DECL_QCONDITION_INFO	\
	};
