
#pragma once

#include <QuestCondition.h>
#include <QuestConditionInfo_Concrete.h>

// TODO : QuestConditonInfo의 하위 클래스를 가지는 개별 클래스 선언과 구현들

BEGIN_DECL_QCONDITION( QUEST_CONDITION_REPEAT )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_REPEAT )
	virtual RC::eQUEST_RESULT CanAccept( void * pvParam );
END_DECL_QCONDITION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION( QUEST_CONDITION_CHARLV )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_CHARLV )
	virtual RC::eQUEST_RESULT CanAccept( void * pvParam );
	virtual RC::eQUEST_RESULT CanComplete( void * pvParam );
END_DECL_QCONDITION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION( QUEST_CONDITION_CHARCLASS )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_CHARCLASS )
	RC::eQUEST_RESULT CanAccept( void * pvParam );
END_DECL_QCONDITION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

BEGIN_DECL_QCONDITION( QUEST_CONDITION_CHAO )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_CHAO )
	virtual RC::eQUEST_RESULT CanAccept( void * pvParam );
END_DECL_QCONDITION

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION( QUEST_CONDITION_QUEST )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_QUEST )
	virtual RC::eQUEST_RESULT CanAccept( void * pvParam );
	virtual RC::eQUEST_RESULT CanComplete( void * pvParam );
END_DECL_QCONDITION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION( QUEST_CONDITION_ITEM )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_ITEM )
	virtual RC::eQUEST_RESULT CanAccept( void * pvParam );
	virtual RC::eQUEST_RESULT CanComplete( void * pvParam );
END_DECL_QCONDITION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION( QUEST_CONDITION_CLASSITEM )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_CLASSITEM )
	virtual RC::eQUEST_RESULT CanAccept( void * pvParam );
	virtual RC::eQUEST_RESULT CanComplete( void * pvParam );
END_DECL_QCONDITION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 완료 조건에만 있음
BEGIN_DECL_QCONDITION( QUEST_CONDITION_MONEY )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_MONEY )
	virtual RC::eQUEST_RESULT CanAccept( void * pvParam );
	virtual RC::eQUEST_RESULT CanComplete( void * pvParam );
END_DECL_QCONDITION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 완료 조건에만 있음
BEGIN_DECL_QCONDITION( QUEST_CONDITION_KILLMONSTER )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_KILLMONSTER )
	virtual void Init( const QUESTPART_PROGR * pStream );
	virtual void Clear();
	virtual void CopyOut( QUESTPART_PROGR & OUT rStream );
	virtual BOOL Event( eQUEST_EVENT evt, QUEST_MSG * pMsg );
	virtual RC::eQUEST_RESULT CanComplete( void * pvParam );	

	__ARRAY_PROPERTY( BYTE, 5, CurMonsterNum )
END_DECL_QCONDITION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION( QUEST_CONDITION_MISSION )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_MISSION )
	virtual RC::eQUEST_RESULT CanAccept( void * pvParam );
	virtual RC::eQUEST_RESULT CanComplete( void * pvParam );
END_DECL_QCONDITION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 완료 조건에만 있음
BEGIN_DECL_QCONDITION( QUEST_CONDITION_AREA )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_AREA )
    virtual BOOL Event( eQUEST_EVENT evt, QUEST_MSG * pMsg );
    virtual RC::eQUEST_RESULT CanComplete( void * pvParam );

enum { UNKNOWN_AREA = 0, SEARCHED_AREA = 1 }; 
enum { Area1 = 0, Area2, Area3, Max_Area}; 
        
    virtual void Init( const QUESTPART_PROGR * pStream );
    virtual void Clear();
    virtual void CopyOut( QUESTPART_PROGR & OUT rStream );

    __ARRAY_PROPERTY( BYTE, Max_Area, CurSearchedArea )
END_DECL_QCONDITION

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BEGIN_DECL_QCONDITION( QUEST_CONDITION_ITEM_SPACE )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_ITEM_SPACE )
	virtual RC::eQUEST_RESULT CanAccept( void * pvParam );
	virtual RC::eQUEST_RESULT CanComplete( void * pvParam );
END_DECL_QCONDITION
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
BEGIN_DECL_QCONDITION( QUEST_CONDITION_MISSION_CLEAR)
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_MISSION_CLEAR)
    virtual void Init( const QUESTPART_PROGR * pStream );
	virtual void Clear();
	virtual void CopyOut( QUESTPART_PROGR & OUT rStream );
	virtual BOOL Event( eQUEST_EVENT evt, QUEST_MSG * pMsg );
	virtual RC::eQUEST_RESULT CanComplete( void * pvParam );	

	__ARRAY_PROPERTY( BYTE, 2, CurClearNum )
END_DECL_QCONDITION
#endif // _NA_006607_20130402_ADD_QUESTTYPE
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#ifdef _DEVTAIYO_QUEST_TRIGGER_WORK
// 완료 조건에만 있음
BEGIN_DECL_QCONDITION( QUEST_CONDITION_TRIGGER )
__DECL_CUSTOMPOOL_PTR( QUEST_CONDITION_TRIGGER )
	virtual void Init( const QUESTPART_PROGR * pStream );
	virtual void Clear();
	virtual BOOL Event( eQUEST_EVENT evt, QUEST_MSG * pMsg );
	virtual RC::eQUEST_RESULT CanComplete( void * pvParam );

	__ARRAY_PROPERTY( BOOL, QUEST_CONDITION_TRIGGER_INFO::_CHECK_NUM, Satisfy )
END_DECL_QCONDITION

#endif // _DEVTAIYO_QUEST_TRIGGER_WORK
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
BEGIN_DECL_QCONDITION(QUEST_CONDITION_CREATE_PORTAL)
__DECL_CUSTOMPOOL_PTR(QUEST_CONDITION_CREATE_PORTAL)  
    virtual RC::eQUEST_RESULT CanAccept( void* pvParam );  
END_DECL_QCONDITION

#ifdef _NA_003027_20111013_HONOR_SYSTEM
BEGIN_DECL_QCONDITION(QUEST_CONDITION_DAILY_QUEST)
__DECL_CUSTOMPOOL_PTR(QUEST_CONDITION_DAILY_QUEST)
    virtual RC::eQUEST_RESULT CanAccept( void* pvParam );      
END_DECL_QCONDITION
#endif //_NA_003027_20111013_HONOR_SYSTEM

