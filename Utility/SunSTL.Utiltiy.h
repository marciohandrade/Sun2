#ifndef __SUN_STL_UTILITY__
#define __SUN_STL_UTILITY__
#pragma once

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Generic Foreach Routines
//	Author : Waverix
//	Remark : ���ϰ� ���Ǵ� ������ ���� �Ϲ�ȭ�� ��ƾ���̴�. �Լ� �̸����� �� ������ ����������,
//			������ ����� ��� �ش� ������ ���� ���� ������ ���۸� �ΰ� ����ϸ�,
//			�� ���ϰ� ����� �� ���� �Ŷ� �����.
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
//		// 1) Wrapper�� ������ ���� ������ ���ϸ� �ȴ�.
//		template<typename _FN, typename _OBJ> inline
//		VOID	TriggerManagerEx::ForeachActiveTriggerWithDelete( _FN _func, const _OBJ& _left )
//		{
//			Foreach_IfTrueThenDeleteNode_SequentialDS(
//				m_listActiveTrigger,						// loop�� �� �ڷᱸ��
//				m_listActiveTrigger.begin(),				// loop�� �� ���� iterator
//				m_listActiveTrigger.end(),					// loop�� ������ iterator
//				std::bind1st(std::mem_fun(_func), _left)	// �� ��忡 ���� ó�� function
//				);
//		}
//		// 2) ����� ������ ���� ������ ���ϸ� �ȴ�.
//		~ TriggerManagerEx::Update()
//		{
//			ForeachActiveTriggerWithDelete( _Update, this );
//		}
//		// 3) �� ��忡 ���� ó�� fucntion�� ������ ���� ó���ϸ� �ȴ�.
//		BOOL	TriggerManagerEx::_Update( sTRIGGER_LIST_NODE rPair )
//		{
//			BaseTrigger* pTrigger = rPair.second;
//			if( !pTrigger->Update() )
//			{
//				~~;
//				return TRUE;	// ������ ���,
//			}
//			return FALSE;		// �������� ���� ���
//		}
//	Usage Spec2 :
//		// 1) Wrapper�� ������ ���� ������ ���ϸ� �ȴ�.
//		template<typename _FN, typename _OBJ, typename _FNforRELEASE, typename _OBJforRELEASE> inline
//		VOID	TriggerManagerEx::ForeachActiveTriggerWithReleaseDelete( _FN _func, const _OBJ& _left, _FNforRELEASE _func2, const _OBJforRELEASE& _left2 )
//		{
//			Foreach_IfTrueThenReleaseAndDeleteNode_SequentialDS(
//				m_listActiveTrigger,						// loop�� �� �ڷᱸ��
//				m_listActiveTrigger.begin(),				// loop�� �� ���� iterator
//				m_listActiveTrigger.end(),					// loop�� ������ iterator
//				std::bind1st(std::mem_fun(_func), _left)	// �� ��忡 ���� ó�� function
//				std::bind1st(std::mem_fun(_func2), _left2)	// �˻� ��� TRUE�� 2��° function ȣ��
//				);
//		}
//		// 2) ����� ������ ���� ������ ���ϸ� �ȴ�.
//		~ TriggerManagerEx::Update()
//		{
//			ForeachActiveTriggerWithReleaseDelete( _Update, this, _Release, this );
//		}
//		// 3) ������ ��忡 ���� �߰� ȣ�� function�� ������ ���� ó���ϸ� �ȴ�.
//		ANY_TYPE	TriggerManagerEx::_Release( sTRIGGER_LIST_NODE rPair )
//		{
//			BaseTrigger* pTrigger = rPair.second;
//			FreeTrigger( pTrigger );
//			return ANY_TYPE; (VOID����)
//		}
//	Usage Operator:
//		// ����� �ش� �ܺ� ������ ������ ��� ���۴� ������ ���� ���� ���� �ִ�.
//		template<typename _FN, typename _FNforRELEASE, > inline
//		VOID	TriggerManagerEx::ForeachActiveTriggerWithReleaseDelete( _FN _func, _FNforRELEASE _func2 )
//		{
//			Foreach_IfTrueThenReleaseAndDeleteNode_SequentialDS(
//				m_listActiveTrigger,						// loop�� �� �ڷᱸ��
//				m_listActiveTrigger.begin(),				// loop�� �� ���� iterator
//				m_listActiveTrigger.end(),					// loop�� ������ iterator
//				std::bind1st(std::mem_fun(_func), this)		// �� ��忡 ���� ó�� function
//				std::bind1st(std::mem_fun(_func2), this)	// �˻� ��� TRUE�� 2��° function ȣ��
//				);
//		}
//		// ������ ������ ���� �������� Operator�� �δ� �� �� ���� ��찡 �ִ�.
//		// ���� ���, �� ��带 ó���� ��, �ӽ� ������ �δ°� �ʿ��� ���...
//		// 1) ~.h�� Operator�� �����Ѵ�.
//		class TriggerManagerEx
//		{
//			~~;
//		private:	class Operator;
//					friend class Operator;
//		};
//		// 2) ~.cpp�� Operator�� �����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Foreach_Default
//	(WAVERIX)(NOTE)
//		���� ������ ������ �����Ѵ�.
//		1) ���� ������ ������ �����ϸ� ������ �޼ҵ带 ȣ���Ѵ�.
//			ȣ������ : VOID _OBJ::Method( *_ITERATOR )
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_Default( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcNodeProcess )
{
    __TOUCHED(_ds); //intercepted debugging factility
	while( _it != _itend )
		_funcNodeProcess(*_it++);
}

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Foreach_AnyConditionTrue
//	(WAVERIX)(NOTE)
//		���� ������ ������ �����ϸ�, ������ �ϳ��� TRUE�� ��� TRUE ��ȯ�Ѵ�.
//		1) ���� ������ ������ �����ϸ� ������ �޼ҵ带 ȣ���Ѵ�.
//			ȣ������ : BOOL _OBJ::Method( *_ITERATOR )
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

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Foreach_AllConditionTrue
//	(WAVERIX)(NOTE)
//		���� ������ ������ �����ϸ�, ������ ��� TRUE�� ��� TRUE ��ȯ�Ѵ�.
//		1) ���� ������ ������ �����ϸ� ������ �޼ҵ带 ȣ���Ѵ�.
//			ȣ������ : BOOL _OBJ::Method( *_ITERATOR )
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

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Foreach_IfTrueThenFuncCall
//	(WAVERIX)(NOTE)
//		���� ������ ������ �����ϸ�, ������ ���� ��� �߰� �޼ҵ带 �����Ѵ�.
//		1) ���� ������ ������ �����ϸ� ������ �޼ҵ带 ȣ���Ѵ�.
//			ȣ������ : BOOL _OBJ::Method( *_ITERATOR )
//		2) ȣ��� �Լ� ����� ���� �б��Ѵ�.
//			TRUE	: �߰� ��ϵ� �޼ҵ带 ȣ���Ѵ�
//				ȣ������ : anytype _OBJ::Method( *_ITERATOR )
//			FALSE	: ���� ���� �Ѿ��.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Foreach_IfTrueThenContinue
//	(WAVERIX)(NOTE)
//		���� ������ ������ �����ϸ�, ������ ���� ��� ��� ������ �����Ѵ�.
//		1) ���� ������ ������ �����ϸ� ������ �޼ҵ带 ȣ���Ѵ�.
//			ȣ������ : BOOL _OBJ::Method( *_ITERATOR )
//		2) ȣ��� �Լ� ����� ���� �б��Ѵ�.
//			TRUE	: ���� ���� �Ѿ��.
//			FALSE	: ������ �����Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Foreach_IfTrueThenBreak
//	(WAVERIX)(NOTE)
//		���� ������ ������ �����ϸ�, ������ ���� ��� ������ �����Ѵ�.
//		1) ���� ������ ������ �����ϸ� ������ �޼ҵ带 ȣ���Ѵ�.
//			ȣ������ : BOOL _OBJ::Method( *_ITERATOR )
//		2) ȣ��� �Լ� ����� ���� �б��Ѵ�.
//			TRUE	: ������ �����Ѵ�.
//			FALSE	: ���� ���� �Ѿ��.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Foreach_IfTrueThenDeleteNode
//	(WAVERIX)(NOTE)
//		���� ������ ������ �����ϸ�, ������ ���� ��� �ش� ��带 �����Ѵ�.
//		1) ���� ������ ������ �����ϸ� ������ �޼ҵ带 ȣ���Ѵ�.
//			ȣ������ : BOOL _OBJ::Method( *_ITERATOR )
//		2) ȣ��� �Լ� ����� ���� �б��Ѵ�.
//			TRUE	: �ش� ��带 ���� �� ���� ���� �Ѿ��.
//			FALSE	: ���� ���� �Ѿ��.
//	(WAVERIX)(WARNING)
//		�� ������ �ش� �Լ� ���ο��� ������ delete, insert���� �����ϴ� ��쿡 �������� ������ 100%�������� ���Ѵ�.
//		�ϴ� �ڱ� �ڽ��� �����ϴ� ���� ����� ���ؾ� �Ѵ�.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenDeleteNode( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete )
{
	while( _it != _itend )
		(!_funcIsDelete(*_it)) ? ++_it : _it = _ds.erase( _it );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Foreach_IfTrueThenReleaseAndDeleteNode
//	(WAVERIX)(NOTE)
//		���� ������ ������ �����ϸ�, ������ ���� ��� �ش� ��带 �����Ѵ�.
//		1) ���� ������ ������ �����ϸ� ������ �޼ҵ带 ȣ���Ѵ�.
//			ȣ������ : BOOL _OBJ::Method( *_ITERATOR )
//		2) ȣ��� �Լ� ����� ���� �б��Ѵ�.
//			TRUE	: �ش� ��带 ���� �� ���� ���� �Ѿ��.
//			FALSE	: ���� ���� �Ѿ��.
//	(WAVERIX)(WARNING)
//		�� ������ �ش� �Լ� ���ο��� ������ delete, insert���� �����ϴ� ��쿡 �������� ������ 100%�������� ���Ѵ�.
//		�ϴ� �ڱ� �ڽ��� �����ϴ� ���� ����� ���ؾ� �Ѵ�.
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

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Foreach_IfTrueThenDeleteNode_Sequential_DS
//	(WAVERIX)(NOTE)
//		���� ������ ������ �����ϸ�, ������ ���� ��� �ش� ��带 �����Ѵ�.
//		1) ���� ������ ������ �����ϸ� ������ �޼ҵ带 ȣ���Ѵ�.
//			ȣ������ : BOOL _OBJ::Method( *_ITERATOR )
//		2) ȣ��� �Լ� ����� ���� �б��Ѵ�.
//			TRUE	: �ش� ��带 ���� �� ���� ���� �Ѿ��.
//			FALSE	: ���� ���� �Ѿ��.
//	(WAVERIX)(WARNING)
//		�� ������ �ش� �Լ� ���ο��� ������ delete, insert���� �����ϴ� ��쿡 �������� ������ 100%�������� ���Ѵ�.
//		�ϴ� �ڱ� �ڽ��� �����ϴ� ���� ����� ���ؾ� �Ѵ�.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenDeleteNode_SequentialDS( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete )
{
	Foreach_IfTrueThenDeleteNode( _ds, _it, _itend, _funcIsDelete );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Foreach_IfTrueThenReleaseAndDeleteNode_Sequential_DS
//	(WAVERIX)(NOTE)
//		���� ������ ������ �����ϸ�, ������ ���� ��� �ش� ��带 �����Ѵ�.
//		1) ���� ������ ������ �����ϸ� ������ �޼ҵ带 ȣ���Ѵ�.
//			ȣ������ : BOOL _OBJ::Method( *_ITERATOR )
//		2) ȣ��� �Լ� ����� ���� �б��Ѵ�.
//			TRUE	: �ش� ��带 ���� �� ���� ���� �Ѿ��.
//			FALSE	: ���� ���� �Ѿ��.
//	(WAVERIX)(WARNING)
//		�� ������ �ش� �Լ� ���ο��� ������ delete, insert���� �����ϴ� ��쿡 �������� ������ 100%�������� ���Ѵ�.
//		�ϴ� �ڱ� �ڽ��� �����ϴ� ���� ����� ���ؾ� �Ѵ�.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenReleaseAndDeleteNode_SequentialDS( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete, _OBJFUNC _funcRelease )
{
	Foreach_IfTrueThenReleaseAndDeleteNode( _ds, _it, _itend, _funcIsDelete, _funcRelease );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Foreach_IfTrueThenDeleteNode_TreeDS
//	(WAVERIX)(NOTE)
//		���� ������ ������ �����ϸ�, ������ ���� ��� �ش� ��带 �����Ѵ�.
//		1) ���� ������ ������ �����ϸ� ������ �޼ҵ带 ȣ���Ѵ�.
//			ȣ������ : BOOL _OBJ::Method( *_ITERATOR )
//		2) ȣ��� �Լ� ����� ���� �б��Ѵ�.
//			TRUE	: �ش� ��带 ���� �� ���� ���� �Ѿ��.
//			FALSE	: ���� ���� �Ѿ��.
//	(WAVERIX)(WARNING)
//		�� ������ �ش� �Լ� ���ο��� ������ delete, insert���� �����ϴ� ��쿡 �������� ������ 100%�������� ���Ѵ�.
//		�ϴ� �ڱ� �ڽ��� �����ϴ� ���� ����� ���ؾ� �Ѵ�.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenDeleteNode_TreeDS( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete )
{
	Foreach_IfTrueThenDeleteNode( _ds, _it, _itend, _funcIsDelete );
}

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Foreach_IfTrueThenReleaseAndDeleteNode_TreeDS
//	(WAVERIX)(NOTE)
//		���� ������ ������ �����ϸ�, ������ ���� ��� �ش� ��带 �����Ѵ�.
//		1) ���� ������ ������ �����ϸ� ������ �޼ҵ带 ȣ���Ѵ�.
//			ȣ������ : BOOL _OBJ::Method( *_ITERATOR )
//		2) ȣ��� �Լ� ����� ���� �б��Ѵ�.
//			TRUE	: �ش� ��带 ���� �� ���� ���� �Ѿ��.
//			FALSE	: ���� ���� �Ѿ��.
//	(WAVERIX)(WARNING)
//		�� ������ �ش� �Լ� ���ο��� ������ delete, insert���� �����ϴ� ��쿡 �������� ������ 100%�������� ���Ѵ�.
//		�ϴ� �ڱ� �ڽ��� �����ϴ� ���� ����� ���ؾ� �Ѵ�.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenReleaseAndDeleteNode_TreeDS( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete, _OBJFUNC _funcRelease )
{
	Foreach_IfTrueThenReleaseAndDeleteNode( _ds, _it, _itend, _funcIsDelete, _funcRelease );
}


//��������������������������������������������������������������������������������������������������������������������������������������������������
//	<SyncIterator>
//		Loop ����, iterator�� ���Ἲ�� �����ϱ� ���� WrapperŬ����
//	(WAVERIX)(WARNING)
//		�̰� ���� ���� ���������� ������ �� �������� ���θ� Ÿ���϶�.
//		�̰��� ������ �ִ� �������� �������� �߱����� �ʵ��� �����ϱ� ���ؼ� ��ȵ� ���� ���̴�.
//	<Use Case>
//		������ ���� ��Ȳ�� �߻��� �� ����ص� �����ϵ��� �����ߴ�.
//		-	Loop�ȿ��� Insert/Delete�� ����� �Ͼ��. (�ڱ� �ڽ��� ������ �� ���� �� �𸥴�.)
//		-	Loop�ȿ��� ������ �ڷᱸ���� ���� loop�� �� �߻��Ѵ�. (�� �� ��ΰ� Insert/Delete�� �߻��� ���)
//		-	Insert/Delete�� �߻��ϴ� loop���� subloop�߻���, �ش� loop�� Insert/Delete�߻����� ������ �Ϲ� �����ε� ����
//	<Sample>
//		typedef SyncIterator<STATUS_TABLE_IT>		SyncStatusIterator;
//		typedef SyncStatusIterator::Transaction		SyncTransaction;
//
//		// loop�� ���� ����
//		SyncTransaction trans( m_SyncIterator );	// loop�� ������ �ݵ�� transaction�� �ɾ��.
//		SyncStatusIterator::Iterator it = m_SyncIterator.SetIterator( m_StatusTable.begin() ); // create wrap_iterator
//		STATUS_TABLE_IT itend = m_StatusTable.end();
//		for( ; it!=itend ; ++it )
//		{
//			BaseStatus* pStatus = it->second;
//			...
//
//		// ���� �� remove (�Ʒ� sample�� key�� ���� ��)
//		if( m_SyncIterator.IsTransaction() && (m_SyncIterator.CurrentIterator()->first == eStateID) )
//			m_SyncIterator.EraseCurrentIterator( m_StatusTable );
//		else
//			m_StatusTable.erase( eStateID );
//
//		// iterator�� ������ �� �ִٸ� (m_SyncIterator.CurrentIterator() == deliterator)�� ���϶�
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
	bool						m_bTransaction[TRANSACTION_DEPTH];	// loop transaction ���� �Ǻ���
	iterator_type				m_CurIterators[TRANSACTION_DEPTH];	// loop transaction�� ���Ǵ� iterator
};

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	<ADDIN-UTILITIES>
//
#include "SunSTL.UtiltiyTMP.h"
//
//

#endif //__SUN_STL_UTILITY__













