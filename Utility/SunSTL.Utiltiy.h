#ifndef __SUN_STL_UTILITY__
#define __SUN_STL_UTILITY__
#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Generic Foreach Routines
//	Author : Waverix
//	Remark : 흔하게 사용되는 루프에 대한 일반화된 루틴들이다. 함수 이름에서 그 역할을 지정했으며,
//			실제로 사용할 경우 해당 루프에 대한 명세를 설명한 래퍼를 두고 사용하면,
//			더 편하게 사용할 수 있을 거라 기대함.
//	Spec & Usage :
//		VOID Foreach_Default									spec 1
//		BOOL Foreach_AnyConditionTrue							spec 1
//		BOOL Foreach_AllConditionTrue							spec 1
//		Foreach_IfTrueThenFuncCall								spec 2
//		Foreach_IfTrueThenContinue								spec 1
//		Foreach_IfTrueThenBreak									spec 1
//		Foreach_IfTrueThenDeleteNode_SequentialDS				spec 1
//		Foreach_IfTrueThenReleaseAndDeleteNode_SequentialDS		spec 2
//		Foreach_IfTrueThenDeleteNode_TreeDS						spec 1
//		Foreach_IfTrueThenReleaseAndDeleteNode_TreeDS			spec 2
//
//	Usage Spec1 :
//		// 1) Wrapper는 다음과 같은 형식을 취하면 된다.
//		template<typename _FN, typename _OBJ> inline
//		VOID	TriggerManagerEx::ForeachActiveTriggerWithDelete( _FN _func, const _OBJ& _left )
//		{
//			Foreach_IfTrueThenDeleteNode_SequentialDS(
//				m_listActiveTrigger,						// loop를 돌 자료구조
//				m_listActiveTrigger.begin(),				// loop를 돌 시작 iterator
//				m_listActiveTrigger.end(),					// loop를 종료할 iterator
//				std::bind1st(std::mem_fun(_func), _left)	// 각 노드에 대한 처리 function
//				);
//		}
//		// 2) 사용은 다음과 같은 형식을 취하면 된다.
//		~ TriggerManagerEx::Update()
//		{
//			ForeachActiveTriggerWithDelete( _Update, this );
//		}
//		// 3) 각 노드에 대한 처리 fucntion은 다음과 같이 처리하면 된다.
//		BOOL	TriggerManagerEx::_Update( sTRIGGER_LIST_NODE rPair )
//		{
//			BaseTrigger* pTrigger = rPair.second;
//			if( !pTrigger->Update() )
//			{
//				~~;
//				return TRUE;	// 제거할 경우,
//			}
//			return FALSE;		// 제거하지 않을 경우
//		}
//	Usage Spec2 :
//		// 1) Wrapper는 다음과 같은 형식을 취하면 된다.
//		template<typename _FN, typename _OBJ, typename _FNforRELEASE, typename _OBJforRELEASE> inline
//		VOID	TriggerManagerEx::ForeachActiveTriggerWithReleaseDelete( _FN _func, const _OBJ& _left, _FNforRELEASE _func2, const _OBJforRELEASE& _left2 )
//		{
//			Foreach_IfTrueThenReleaseAndDeleteNode_SequentialDS(
//				m_listActiveTrigger,						// loop를 돌 자료구조
//				m_listActiveTrigger.begin(),				// loop를 돌 시작 iterator
//				m_listActiveTrigger.end(),					// loop를 종료할 iterator
//				std::bind1st(std::mem_fun(_func), _left)	// 각 노드에 대한 처리 function
//				std::bind1st(std::mem_fun(_func2), _left2)	// 검사 결과 TRUE시 2번째 function 호출
//				);
//		}
//		// 2) 사용은 다음과 같은 형식을 취하면 된다.
//		~ TriggerManagerEx::Update()
//		{
//			ForeachActiveTriggerWithReleaseDelete( _Update, this, _Release, this );
//		}
//		// 3) 제거할 노드에 대한 추가 호출 function은 다음과 같이 처리하면 된다.
//		ANY_TYPE	TriggerManagerEx::_Release( sTRIGGER_LIST_NODE rPair )
//		{
//			BaseTrigger* pTrigger = rPair.second;
//			FreeTrigger( pTrigger );
//			return ANY_TYPE; (VOID포함)
//		}
//	Usage Operator:
//		// 멤버에 해당 외부 접근을 불허할 경우 랩퍼는 다음과 같이 줄일 수는 있다.
//		template<typename _FN, typename _FNforRELEASE, > inline
//		VOID	TriggerManagerEx::ForeachActiveTriggerWithReleaseDelete( _FN _func, _FNforRELEASE _func2 )
//		{
//			Foreach_IfTrueThenReleaseAndDeleteNode_SequentialDS(
//				m_listActiveTrigger,						// loop를 돌 자료구조
//				m_listActiveTrigger.begin(),				// loop를 돌 시작 iterator
//				m_listActiveTrigger.end(),					// loop를 종료할 iterator
//				std::bind1st(std::mem_fun(_func), this)		// 각 노드에 대한 처리 function
//				std::bind1st(std::mem_fun(_func2), this)	// 검사 결과 TRUE시 2번째 function 호출
//				);
//		}
//		// 하지만 루프에 대한 전문적인 Operator를 두는 게 더 좋을 경우가 있다.
//		// 예를 들어, 각 노드를 처리할 때, 임시 변수를 두는게 필요할 경우...
//		// 1) ~.h에 Operator를 선언한다.
//		class TriggerManagerEx
//		{
//			~~;
//		private:	class Operator;
//					friend class Operator;
//		};
//		// 2) ~.cpp에 Operator를 구현한다.
//		class TriggerManagerEx::Operator
//		{
//		public:		Operator( TriggerManagerEx* pMgr ) : m_pMGR(pMgr) {}
//		// routine process node
//		public:		BOOL	_Update( sTRIGGER_LIST_NODE rPair )
//					{
//						~~;
//					}
//
//					BOOL	_Release( sTRIGGER_LIST_NODE rPair )
//					{
//						RELEASE_COUNT.Set( RELEASE_COUNT+1 );
//					}
//		private:	TriggerManagerEx*	m_pMGR;
//		public:		PROPERTY<DWORD>		RELEASE_COUNT;
//		};
//

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Foreach_Default
//	(WAVERIX)(NOTE)
//		지정 범위의 루프를 수행한다.
//		1) 지정 범위의 루프를 수행하며 지정된 메소드를 호출한다.
//			호출형식 : VOID _OBJ::Method( *_ITERATOR )
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_Default( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcNodeProcess )
{
    __TOUCHED(_ds); //intercepted debugging factility
	while( _it != _itend )
		_funcNodeProcess(*_it++);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Foreach_AnyConditionTrue
//	(WAVERIX)(NOTE)
//		지정 범위의 루프를 수행하며, 조건이 하나라도 TRUE일 경우 TRUE 반환한다.
//		1) 지정 범위의 루프를 수행하며 지정된 메소드를 호출한다.
//			호출형식 : BOOL _OBJ::Method( *_ITERATOR )
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline BOOL		Foreach_AnyConditionTrue( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcCheckCondition )
{
	BOOL bRET = FALSE;
	while( _it != _itend )
	{
		if( _funcCheckCondition(*_it) )
			bRET = TRUE;

		++_it;
	}
	return bRET;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Foreach_AllConditionTrue
//	(WAVERIX)(NOTE)
//		지정 범위의 루프를 수행하며, 조건이 모두 TRUE일 경우 TRUE 반환한다.
//		1) 지정 범위의 루프를 수행하며 지정된 메소드를 호출한다.
//			호출형식 : BOOL _OBJ::Method( *_ITERATOR )
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline BOOL		Foreach_AllConditionTrue( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcCheckCondition )
{
	BOOL bRET = TRUE;
	while( _it != _itend )
	{
		if( !_funcCheckCondition(*_it) )
			bRET = FALSE;

		++_it;
	}
	return bRET;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Foreach_IfTrueThenFuncCall
//	(WAVERIX)(NOTE)
//		지정 범위의 루프를 수행하며, 조건이 참일 경우 추가 메소드를 실행한다.
//		1) 지정 범위의 루프를 수행하며 지정된 메소드를 호출한다.
//			호출형식 : BOOL _OBJ::Method( *_ITERATOR )
//		2) 호출된 함수 결과에 따라 분기한다.
//			TRUE	: 추가 등록된 메소드를 호출한다
//				호출형식 : anytype _OBJ::Method( *_ITERATOR )
//			FALSE	: 다음 노드로 넘어간다.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenFuncCall( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcCheckTrue, _OBJFUNC _funcIfTrueCase )
{
	while( _it != _itend )
	{
		if( _funcCheckTrue(*_it) )
			_funcIfTrueCase(*_it);
		++_it;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Foreach_IfTrueThenContinue
//	(WAVERIX)(NOTE)
//		지정 범위의 루프를 수행하며, 조건이 참일 경우 계속 루프를 실행한다.
//		1) 지정 범위의 루프를 수행하며 지정된 메소드를 호출한다.
//			호출형식 : BOOL _OBJ::Method( *_ITERATOR )
//		2) 호출된 함수 결과에 따라 분기한다.
//			TRUE	: 다음 노드로 넘어간다.
//			FALSE	: 루프를 종료한다.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenContinue( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsContinue )
{
	while( _it != _itend )
	{
		if( !_funcIsContinue(*_it) )
			break;
		++_it;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Foreach_IfTrueThenBreak
//	(WAVERIX)(NOTE)
//		지정 범위의 루프를 수행하며, 조건이 참일 경우 루프를 종료한다.
//		1) 지정 범위의 루프를 수행하며 지정된 메소드를 호출한다.
//			호출형식 : BOOL _OBJ::Method( *_ITERATOR )
//		2) 호출된 함수 결과에 따라 분기한다.
//			TRUE	: 루프를 종료한다.
//			FALSE	: 다음 노드로 넘어간다.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenBreak( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsBreak )
{
	while( _it != _itend )
	{
		if( _funcIsBreak(*_it) )
			break;
		++_it;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Foreach_IfTrueThenDeleteNode
//	(WAVERIX)(NOTE)
//		지정 범위의 루프를 수행하며, 조건이 참일 경우 해당 노드를 제거한다.
//		1) 지정 범위의 루프를 수행하며 지정된 메소드를 호출한다.
//			호출형식 : BOOL _OBJ::Method( *_ITERATOR )
//		2) 호출된 함수 결과에 따라 분기한다.
//			TRUE	: 해당 노드를 삭제 후 다음 노드로 넘어간다.
//			FALSE	: 다음 노드로 넘어간다.
//	(WAVERIX)(WARNING)
//		이 로직은 해당 함수 내부에서 별도로 delete, insert등을 수행하는 경우에 정상적인 동작을 100%보장하지 못한다.
//		일단 자기 자신을 제거하는 경우는 절대로 피해야 한다.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenDeleteNode( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete )
{
	while( _it != _itend )
		(!_funcIsDelete(*_it)) ? ++_it : _it = _ds.erase( _it );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Foreach_IfTrueThenReleaseAndDeleteNode
//	(WAVERIX)(NOTE)
//		지정 범위의 루프를 수행하며, 조건이 참일 경우 해당 노드를 제거한다.
//		1) 지정 범위의 루프를 수행하며 지정된 메소드를 호출한다.
//			호출형식 : BOOL _OBJ::Method( *_ITERATOR )
//		2) 호출된 함수 결과에 따라 분기한다.
//			TRUE	: 해당 노드를 삭제 후 다음 노드로 넘어간다.
//			FALSE	: 다음 노드로 넘어간다.
//	(WAVERIX)(WARNING)
//		이 로직은 해당 함수 내부에서 별도로 delete, insert등을 수행하는 경우에 정상적인 동작을 100%보장하지 못한다.
//		일단 자기 자신을 제거하는 경우는 절대로 피해야 한다.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenReleaseAndDeleteNode( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete, _OBJFUNC _funcRelease )
{
	while( _it != _itend )
	{
		if(!_funcIsDelete(*_it))
			++_it;
		else
		{
			_funcRelease( *_it );
			_it = _ds.erase( _it );
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Foreach_IfTrueThenDeleteNode_Sequential_DS
//	(WAVERIX)(NOTE)
//		지정 범위의 루프를 수행하며, 조건이 참일 경우 해당 노드를 제거한다.
//		1) 지정 범위의 루프를 수행하며 지정된 메소드를 호출한다.
//			호출형식 : BOOL _OBJ::Method( *_ITERATOR )
//		2) 호출된 함수 결과에 따라 분기한다.
//			TRUE	: 해당 노드를 삭제 후 다음 노드로 넘어간다.
//			FALSE	: 다음 노드로 넘어간다.
//	(WAVERIX)(WARNING)
//		이 로직은 해당 함수 내부에서 별도로 delete, insert등을 수행하는 경우에 정상적인 동작을 100%보장하지 못한다.
//		일단 자기 자신을 제거하는 경우는 절대로 피해야 한다.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenDeleteNode_SequentialDS( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete )
{
	Foreach_IfTrueThenDeleteNode( _ds, _it, _itend, _funcIsDelete );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Foreach_IfTrueThenReleaseAndDeleteNode_Sequential_DS
//	(WAVERIX)(NOTE)
//		지정 범위의 루프를 수행하며, 조건이 참일 경우 해당 노드를 제거한다.
//		1) 지정 범위의 루프를 수행하며 지정된 메소드를 호출한다.
//			호출형식 : BOOL _OBJ::Method( *_ITERATOR )
//		2) 호출된 함수 결과에 따라 분기한다.
//			TRUE	: 해당 노드를 삭제 후 다음 노드로 넘어간다.
//			FALSE	: 다음 노드로 넘어간다.
//	(WAVERIX)(WARNING)
//		이 로직은 해당 함수 내부에서 별도로 delete, insert등을 수행하는 경우에 정상적인 동작을 100%보장하지 못한다.
//		일단 자기 자신을 제거하는 경우는 절대로 피해야 한다.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenReleaseAndDeleteNode_SequentialDS( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete, _OBJFUNC _funcRelease )
{
	Foreach_IfTrueThenReleaseAndDeleteNode( _ds, _it, _itend, _funcIsDelete, _funcRelease );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Foreach_IfTrueThenDeleteNode_TreeDS
//	(WAVERIX)(NOTE)
//		지정 범위의 루프를 수행하며, 조건이 참일 경우 해당 노드를 제거한다.
//		1) 지정 범위의 루프를 수행하며 지정된 메소드를 호출한다.
//			호출형식 : BOOL _OBJ::Method( *_ITERATOR )
//		2) 호출된 함수 결과에 따라 분기한다.
//			TRUE	: 해당 노드를 삭제 후 다음 노드로 넘어간다.
//			FALSE	: 다음 노드로 넘어간다.
//	(WAVERIX)(WARNING)
//		이 로직은 해당 함수 내부에서 별도로 delete, insert등을 수행하는 경우에 정상적인 동작을 100%보장하지 못한다.
//		일단 자기 자신을 제거하는 경우는 절대로 피해야 한다.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenDeleteNode_TreeDS( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete )
{
	Foreach_IfTrueThenDeleteNode( _ds, _it, _itend, _funcIsDelete );
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Foreach_IfTrueThenReleaseAndDeleteNode_TreeDS
//	(WAVERIX)(NOTE)
//		지정 범위의 루프를 수행하며, 조건이 참일 경우 해당 노드를 제거한다.
//		1) 지정 범위의 루프를 수행하며 지정된 메소드를 호출한다.
//			호출형식 : BOOL _OBJ::Method( *_ITERATOR )
//		2) 호출된 함수 결과에 따라 분기한다.
//			TRUE	: 해당 노드를 삭제 후 다음 노드로 넘어간다.
//			FALSE	: 다음 노드로 넘어간다.
//	(WAVERIX)(WARNING)
//		이 로직은 해당 함수 내부에서 별도로 delete, insert등을 수행하는 경우에 정상적인 동작을 100%보장하지 못한다.
//		일단 자기 자신을 제거하는 경우는 절대로 피해야 한다.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenReleaseAndDeleteNode_TreeDS( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete, _OBJFUNC _funcRelease )
{
	Foreach_IfTrueThenReleaseAndDeleteNode( _ds, _it, _itend, _funcIsDelete, _funcRelease );
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SyncIterator>
//		Loop 기준, iterator의 무결성을 보장하기 위한 Wrapper클래스
//	(WAVERIX)(WARNING)
//		이걸 쓰기 전에 구조적으로 개선할 수 있을지의 여부를 타전하라.
//		이것은 문제가 있는 구조에서 문제점을 야기하지 않도록 보완하기 위해서 고안된 것일 뿐이다.
//	<Use Case>
//		다음과 같은 상황이 발생할 때 사용해도 안전하도록 구성했다.
//		-	Loop안에서 Insert/Delete가 빈번히 일어난다. (자기 자신을 삭제할 수 있을 지 모른다.)
//		-	Loop안에서 동일한 자료구조에 대해 loop가 또 발생한다. (이 둘 모두가 Insert/Delete가 발생할 경우)
//		-	Insert/Delete가 발생하는 loop에서 subloop발생시, 해당 loop가 Insert/Delete발생하지 않으면 일반 루프로도 가능
//	<Sample>
//		typedef SyncIterator<STATUS_TABLE_IT>		SyncStatusIterator;
//		typedef SyncStatusIterator::Transaction		SyncTransaction;
//
//		// loop를 도는 시점
//		SyncTransaction trans( m_SyncIterator );	// loop를 돌기전 반드시 transaction을 걸어라.
//		SyncStatusIterator::Iterator it = m_SyncIterator.SetIterator( m_StatusTable.begin() ); // create wrap_iterator
//		STATUS_TABLE_IT itend = m_StatusTable.end();
//		for( ; it!=itend ; ++it )
//		{
//			BaseStatus* pStatus = it->second;
//			...
//
//		// 루프 안 remove (아래 sample은 key로 평가할 때)
//		if( m_SyncIterator.IsTransaction() && (m_SyncIterator.CurrentIterator()->first == eStateID) )
//			m_SyncIterator.EraseCurrentIterator( m_StatusTable );
//		else
//			m_StatusTable.erase( eStateID );
//
//		// iterator로 제거할 수 있다면 (m_SyncIterator.CurrentIterator() == deliterator)로 평가하라
//		if( m_SyncIterator.IsTransaction() && (m_SyncIterator.CurrentIterator() == deliterator) )
//			m_SyncIterator.EraseCurrentIterator( m_StatusTable );
//		else
//			m_StatusTable.erase( deliterator );
//
template<typename _ITERATOR, DWORD _TRANSACTION_DEPTH=4>
class SyncIterator
{
	typedef _ITERATOR							iterator_type;
	static const DWORD							TRANSACTION_DEPTH = _TRANSACTION_DEPTH;

public:
	SyncIterator() : m_Depth(0) {
        ZeroMemory(m_bTransaction, sizeof(m_bTransaction));
        //fill_n( &m_bTransaction[0], (INT)(sizeof(m_bTransaction)/sizeof(m_bTransaction[0])), false );
    }

	//////////////////////////////////////////////////////////////////////////
	//	<UTILITY CLASS DEFINTIONs>
public:
	//////////////////////////////////////////////////////////////////////////
	//	<Transaction>
	class Transaction
	{
	public:		Transaction( SyncIterator& r ) : m_rSync(r)		{ m_rSync._EnterTransaction(); }
				~Transaction()									{ m_rSync._LeaveTransaction(); }
	private:	SyncIterator&							m_rSync;
	};
	friend class Transaction;

	//////////////////////////////////////////////////////////////////////////
	//	<Iterator>
	class Iterator
	{
	public:
		typedef _ITERATOR								iterator_type;
		typedef Iterator								this_type;
		typedef typename iterator_type::pointer			pointer;
		typedef typename iterator_type::reference		reference;
	private:
		friend class SyncIterator;
		Iterator( const iterator_type& r ) : m_rCurIterator(const_cast<iterator_type&>(r)) {}
	public:
		operator const iterator_type&()			{ return m_rCurIterator; }

		reference			operator*()			{ return *m_rCurIterator; }
		pointer				operator->() const	{ return m_rCurIterator.operator->(); }
		this_type&			operator++()		{ ++m_rCurIterator; return (*this); }
		this_type			operator++(int)		{ return m_rCurIterator++; }
		this_type&			operator--()		{ --m_rCurIterator; return (*this); }
		this_type			operator--(int)		{ return m_rCurIterator--; }
		BOOL				operator == ( const iterator_type& _Right ) const { return (m_rCurIterator == _Right); }
		BOOL				operator != ( const iterator_type& _Right ) const { return (!(m_rCurIterator == _Right)); }

		iterator_type&		m_rCurIterator;
	};

public:

	__forceinline bool			IsTransaction()		{ return !m_Depth ? false : m_bTransaction[m_Depth-1]; }
	__forceinline Iterator		SetIterator( iterator_type _it ) { return m_CurIterators[m_Depth-1] = _it; }
	//const Iterator				CurrentIterator()	{ return m_CurIterators[m_Depth-1]; }
	template<typename _TABLE>	VOID	EraseIterator( _TABLE& table, const iterator_type& itr )
	{
		if( 0 == m_Depth )
			table.erase(itr);
		else
		{
			iterator_type tmp = itr;
			iterator_type next = table.erase(itr);
			--next;
			replace( &m_CurIterators[0], &m_CurIterators[m_Depth], tmp, next );
		}
	}
private:
	__forceinline VOID			_LeaveTransaction()	{ m_bTransaction[--m_Depth] = false; }
	__forceinline bool			_EnterTransaction()
	{
		if(m_Depth < TRANSACTION_DEPTH)
			return m_bTransaction[m_Depth++] = true;
		else
		{
			__debugbreak();
			return false;
		}
	}

private:
	WORD						m_Depth;
	bool						m_bTransaction[TRANSACTION_DEPTH];	// loop transaction 여부 판별용
	iterator_type				m_CurIterators[TRANSACTION_DEPTH];	// loop transaction시 사용되는 iterator
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<ADDIN-UTILITIES>
//
#include "SunSTL.UtiltiyTMP.h"
//
//

#endif //__SUN_STL_UTILITY__













