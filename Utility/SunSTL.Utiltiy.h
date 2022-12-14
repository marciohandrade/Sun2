#ifndef __SUN_STL_UTILITY__
#define __SUN_STL_UTILITY__
#pragma once

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Generic Foreach Routines
//	Author : Waverix
//	Remark : �褘炾� 餌辨腎朝 瑞Щ縑 渠и 橾奩�音� 瑞ず菟檜棻. л熱 檜葷縑憮 斜 羲й擊 雖薑ц戲貊,
//			褒薯煎 餌辨й 唳辦 п渡 瑞Щ縑 渠и 貲撮蒂 撲貲и 楚ぷ蒂 舒堅 餌辨ж賊,
//			渦 らж啪 餌辨й 熱 氈擊 剪塭 晦渠л.
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
//		// 1) Wrapper朝 棻擠婁 偽擎 ⑽衝擊 鏃ж賊 脹棻.
//		template<typename _FN, typename _OBJ> inline
//		VOID	TriggerManagerEx::ForeachActiveTriggerWithDelete( _FN _func, const _OBJ& _left )
//		{
//			Foreach_IfTrueThenDeleteNode_SequentialDS(
//				m_listActiveTrigger,						// loop蒂 給 濠猿掘褻
//				m_listActiveTrigger.begin(),				// loop蒂 給 衛濛 iterator
//				m_listActiveTrigger.end(),					// loop蒂 謙猿й iterator
//				std::bind1st(std::mem_fun(_func), _left)	// 陝 喻萄縑 渠и 籀葬 function
//				);
//		}
//		// 2) 餌辨擎 棻擠婁 偽擎 ⑽衝擊 鏃ж賊 脹棻.
//		~ TriggerManagerEx::Update()
//		{
//			ForeachActiveTriggerWithDelete( _Update, this );
//		}
//		// 3) 陝 喻萄縑 渠и 籀葬 fucntion擎 棻擠婁 偽檜 籀葬ж賊 脹棻.
//		BOOL	TriggerManagerEx::_Update( sTRIGGER_LIST_NODE rPair )
//		{
//			BaseTrigger* pTrigger = rPair.second;
//			if( !pTrigger->Update() )
//			{
//				~~;
//				return TRUE;	// 薯剪й 唳辦,
//			}
//			return FALSE;		// 薯剪ж雖 彊擊 唳辦
//		}
//	Usage Spec2 :
//		// 1) Wrapper朝 棻擠婁 偽擎 ⑽衝擊 鏃ж賊 脹棻.
//		template<typename _FN, typename _OBJ, typename _FNforRELEASE, typename _OBJforRELEASE> inline
//		VOID	TriggerManagerEx::ForeachActiveTriggerWithReleaseDelete( _FN _func, const _OBJ& _left, _FNforRELEASE _func2, const _OBJforRELEASE& _left2 )
//		{
//			Foreach_IfTrueThenReleaseAndDeleteNode_SequentialDS(
//				m_listActiveTrigger,						// loop蒂 給 濠猿掘褻
//				m_listActiveTrigger.begin(),				// loop蒂 給 衛濛 iterator
//				m_listActiveTrigger.end(),					// loop蒂 謙猿й iterator
//				std::bind1st(std::mem_fun(_func), _left)	// 陝 喻萄縑 渠и 籀葬 function
//				std::bind1st(std::mem_fun(_func2), _left2)	// 匐餌 唸婁 TRUE衛 2廓簞 function ��轎
//				);
//		}
//		// 2) 餌辨擎 棻擠婁 偽擎 ⑽衝擊 鏃ж賊 脹棻.
//		~ TriggerManagerEx::Update()
//		{
//			ForeachActiveTriggerWithReleaseDelete( _Update, this, _Release, this );
//		}
//		// 3) 薯剪й 喻萄縑 渠и 蹺陛 ��轎 function擎 棻擠婁 偽檜 籀葬ж賊 脹棻.
//		ANY_TYPE	TriggerManagerEx::_Release( sTRIGGER_LIST_NODE rPair )
//		{
//			BaseTrigger* pTrigger = rPair.second;
//			FreeTrigger( pTrigger );
//			return ANY_TYPE; (VOIDんл)
//		}
//	Usage Operator:
//		// 詹幗縑 п渡 諼睡 蕾斬擊 碳ъй 唳辦 椰ぷ朝 棻擠婁 偽檜 還橾 熱朝 氈棻.
//		template<typename _FN, typename _FNforRELEASE, > inline
//		VOID	TriggerManagerEx::ForeachActiveTriggerWithReleaseDelete( _FN _func, _FNforRELEASE _func2 )
//		{
//			Foreach_IfTrueThenReleaseAndDeleteNode_SequentialDS(
//				m_listActiveTrigger,						// loop蒂 給 濠猿掘褻
//				m_listActiveTrigger.begin(),				// loop蒂 給 衛濛 iterator
//				m_listActiveTrigger.end(),					// loop蒂 謙猿й iterator
//				std::bind1st(std::mem_fun(_func), this)		// 陝 喻萄縑 渠и 籀葬 function
//				std::bind1st(std::mem_fun(_func2), this)	// 匐餌 唸婁 TRUE衛 2廓簞 function ��轎
//				);
//		}
//		// ж雖虜 瑞Щ縑 渠и 瞪僥瞳檣 Operator蒂 舒朝 啪 渦 謠擊 唳辦陛 氈棻.
//		// 蕨蒂 菟橫, 陝 喻萄蒂 籀葬й 陽, 歜衛 滲熱蒂 舒朝啪 в蹂й 唳辦...
//		// 1) ~.h縑 Operator蒂 摹樹и棻.
//		class TriggerManagerEx
//		{
//			~~;
//		private:	class Operator;
//					friend class Operator;
//		};
//		// 2) ~.cpp縑 Operator蒂 掘⑷и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Foreach_Default
//	(WAVERIX)(NOTE)
//		雖薑 彰嬪曖 瑞Щ蒂 熱чи棻.
//		1) 雖薑 彰嬪曖 瑞Щ蒂 熱чж貊 雖薑脹 詭模萄蒂 ��轎и棻.
//			��轎⑽衝 : VOID _OBJ::Method( *_ITERATOR )
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_Default( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcNodeProcess )
{
    __TOUCHED(_ds); //intercepted debugging factility
	while( _it != _itend )
		_funcNodeProcess(*_it++);
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Foreach_AnyConditionTrue
//	(WAVERIX)(NOTE)
//		雖薑 彰嬪曖 瑞Щ蒂 熱чж貊, 褻勒檜 ж釭塭紫 TRUE橾 唳辦 TRUE 奩�納挬�.
//		1) 雖薑 彰嬪曖 瑞Щ蒂 熱чж貊 雖薑脹 詭模萄蒂 ��轎и棻.
//			��轎⑽衝 : BOOL _OBJ::Method( *_ITERATOR )
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Foreach_AllConditionTrue
//	(WAVERIX)(NOTE)
//		雖薑 彰嬪曖 瑞Щ蒂 熱чж貊, 褻勒檜 賅舒 TRUE橾 唳辦 TRUE 奩�納挬�.
//		1) 雖薑 彰嬪曖 瑞Щ蒂 熱чж貊 雖薑脹 詭模萄蒂 ��轎и棻.
//			��轎⑽衝 : BOOL _OBJ::Method( *_ITERATOR )
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Foreach_IfTrueThenFuncCall
//	(WAVERIX)(NOTE)
//		雖薑 彰嬪曖 瑞Щ蒂 熱чж貊, 褻勒檜 霤橾 唳辦 蹺陛 詭模萄蒂 褒чи棻.
//		1) 雖薑 彰嬪曖 瑞Щ蒂 熱чж貊 雖薑脹 詭模萄蒂 ��轎и棻.
//			��轎⑽衝 : BOOL _OBJ::Method( *_ITERATOR )
//		2) ��轎脹 л熱 唸婁縑 評塭 碟晦и棻.
//			TRUE	: 蹺陛 蛔煙脹 詭模萄蒂 ��轎и棻
//				��轎⑽衝 : anytype _OBJ::Method( *_ITERATOR )
//			FALSE	: 棻擠 喻萄煎 剩橫除棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Foreach_IfTrueThenContinue
//	(WAVERIX)(NOTE)
//		雖薑 彰嬪曖 瑞Щ蒂 熱чж貊, 褻勒檜 霤橾 唳辦 啗樓 瑞Щ蒂 褒чи棻.
//		1) 雖薑 彰嬪曖 瑞Щ蒂 熱чж貊 雖薑脹 詭模萄蒂 ��轎и棻.
//			��轎⑽衝 : BOOL _OBJ::Method( *_ITERATOR )
//		2) ��轎脹 л熱 唸婁縑 評塭 碟晦и棻.
//			TRUE	: 棻擠 喻萄煎 剩橫除棻.
//			FALSE	: 瑞Щ蒂 謙猿и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Foreach_IfTrueThenBreak
//	(WAVERIX)(NOTE)
//		雖薑 彰嬪曖 瑞Щ蒂 熱чж貊, 褻勒檜 霤橾 唳辦 瑞Щ蒂 謙猿и棻.
//		1) 雖薑 彰嬪曖 瑞Щ蒂 熱чж貊 雖薑脹 詭模萄蒂 ��轎и棻.
//			��轎⑽衝 : BOOL _OBJ::Method( *_ITERATOR )
//		2) ��轎脹 л熱 唸婁縑 評塭 碟晦и棻.
//			TRUE	: 瑞Щ蒂 謙猿и棻.
//			FALSE	: 棻擠 喻萄煎 剩橫除棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Foreach_IfTrueThenDeleteNode
//	(WAVERIX)(NOTE)
//		雖薑 彰嬪曖 瑞Щ蒂 熱чж貊, 褻勒檜 霤橾 唳辦 п渡 喻萄蒂 薯剪и棻.
//		1) 雖薑 彰嬪曖 瑞Щ蒂 熱чж貊 雖薑脹 詭模萄蒂 ��轎и棻.
//			��轎⑽衝 : BOOL _OBJ::Method( *_ITERATOR )
//		2) ��轎脹 л熱 唸婁縑 評塭 碟晦и棻.
//			TRUE	: п渡 喻萄蒂 餉薯 �� 棻擠 喻萄煎 剩橫除棻.
//			FALSE	: 棻擠 喻萄煎 剩橫除棻.
//	(WAVERIX)(WARNING)
//		檜 煎霜擎 п渡 л熱 頂睡縑憮 滌紫煎 delete, insert蛔擊 熱чж朝 唳辦縑 薑鼻瞳檣 翕濛擊 100%爾濰ж雖 跤и棻.
//		橾欽 濠晦 濠褐擊 薯剪ж朝 唳辦朝 瞰渠煎 Яп撿 и棻.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenDeleteNode( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete )
{
	while( _it != _itend )
		(!_funcIsDelete(*_it)) ? ++_it : _it = _ds.erase( _it );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Foreach_IfTrueThenReleaseAndDeleteNode
//	(WAVERIX)(NOTE)
//		雖薑 彰嬪曖 瑞Щ蒂 熱чж貊, 褻勒檜 霤橾 唳辦 п渡 喻萄蒂 薯剪и棻.
//		1) 雖薑 彰嬪曖 瑞Щ蒂 熱чж貊 雖薑脹 詭模萄蒂 ��轎и棻.
//			��轎⑽衝 : BOOL _OBJ::Method( *_ITERATOR )
//		2) ��轎脹 л熱 唸婁縑 評塭 碟晦и棻.
//			TRUE	: п渡 喻萄蒂 餉薯 �� 棻擠 喻萄煎 剩橫除棻.
//			FALSE	: 棻擠 喻萄煎 剩橫除棻.
//	(WAVERIX)(WARNING)
//		檜 煎霜擎 п渡 л熱 頂睡縑憮 滌紫煎 delete, insert蛔擊 熱чж朝 唳辦縑 薑鼻瞳檣 翕濛擊 100%爾濰ж雖 跤и棻.
//		橾欽 濠晦 濠褐擊 薯剪ж朝 唳辦朝 瞰渠煎 Яп撿 и棻.
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

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Foreach_IfTrueThenDeleteNode_Sequential_DS
//	(WAVERIX)(NOTE)
//		雖薑 彰嬪曖 瑞Щ蒂 熱чж貊, 褻勒檜 霤橾 唳辦 п渡 喻萄蒂 薯剪и棻.
//		1) 雖薑 彰嬪曖 瑞Щ蒂 熱чж貊 雖薑脹 詭模萄蒂 ��轎и棻.
//			��轎⑽衝 : BOOL _OBJ::Method( *_ITERATOR )
//		2) ��轎脹 л熱 唸婁縑 評塭 碟晦и棻.
//			TRUE	: п渡 喻萄蒂 餉薯 �� 棻擠 喻萄煎 剩橫除棻.
//			FALSE	: 棻擠 喻萄煎 剩橫除棻.
//	(WAVERIX)(WARNING)
//		檜 煎霜擎 п渡 л熱 頂睡縑憮 滌紫煎 delete, insert蛔擊 熱чж朝 唳辦縑 薑鼻瞳檣 翕濛擊 100%爾濰ж雖 跤и棻.
//		橾欽 濠晦 濠褐擊 薯剪ж朝 唳辦朝 瞰渠煎 Яп撿 и棻.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenDeleteNode_SequentialDS( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete )
{
	Foreach_IfTrueThenDeleteNode( _ds, _it, _itend, _funcIsDelete );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Foreach_IfTrueThenReleaseAndDeleteNode_Sequential_DS
//	(WAVERIX)(NOTE)
//		雖薑 彰嬪曖 瑞Щ蒂 熱чж貊, 褻勒檜 霤橾 唳辦 п渡 喻萄蒂 薯剪и棻.
//		1) 雖薑 彰嬪曖 瑞Щ蒂 熱чж貊 雖薑脹 詭模萄蒂 ��轎и棻.
//			��轎⑽衝 : BOOL _OBJ::Method( *_ITERATOR )
//		2) ��轎脹 л熱 唸婁縑 評塭 碟晦и棻.
//			TRUE	: п渡 喻萄蒂 餉薯 �� 棻擠 喻萄煎 剩橫除棻.
//			FALSE	: 棻擠 喻萄煎 剩橫除棻.
//	(WAVERIX)(WARNING)
//		檜 煎霜擎 п渡 л熱 頂睡縑憮 滌紫煎 delete, insert蛔擊 熱чж朝 唳辦縑 薑鼻瞳檣 翕濛擊 100%爾濰ж雖 跤и棻.
//		橾欽 濠晦 濠褐擊 薯剪ж朝 唳辦朝 瞰渠煎 Яп撿 и棻.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenReleaseAndDeleteNode_SequentialDS( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete, _OBJFUNC _funcRelease )
{
	Foreach_IfTrueThenReleaseAndDeleteNode( _ds, _it, _itend, _funcIsDelete, _funcRelease );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Foreach_IfTrueThenDeleteNode_TreeDS
//	(WAVERIX)(NOTE)
//		雖薑 彰嬪曖 瑞Щ蒂 熱чж貊, 褻勒檜 霤橾 唳辦 п渡 喻萄蒂 薯剪и棻.
//		1) 雖薑 彰嬪曖 瑞Щ蒂 熱чж貊 雖薑脹 詭模萄蒂 ��轎и棻.
//			��轎⑽衝 : BOOL _OBJ::Method( *_ITERATOR )
//		2) ��轎脹 л熱 唸婁縑 評塭 碟晦и棻.
//			TRUE	: п渡 喻萄蒂 餉薯 �� 棻擠 喻萄煎 剩橫除棻.
//			FALSE	: 棻擠 喻萄煎 剩橫除棻.
//	(WAVERIX)(WARNING)
//		檜 煎霜擎 п渡 л熱 頂睡縑憮 滌紫煎 delete, insert蛔擊 熱чж朝 唳辦縑 薑鼻瞳檣 翕濛擊 100%爾濰ж雖 跤и棻.
//		橾欽 濠晦 濠褐擊 薯剪ж朝 唳辦朝 瞰渠煎 Яп撿 и棻.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenDeleteNode_TreeDS( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete )
{
	Foreach_IfTrueThenDeleteNode( _ds, _it, _itend, _funcIsDelete );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	Foreach_IfTrueThenReleaseAndDeleteNode_TreeDS
//	(WAVERIX)(NOTE)
//		雖薑 彰嬪曖 瑞Щ蒂 熱чж貊, 褻勒檜 霤橾 唳辦 п渡 喻萄蒂 薯剪и棻.
//		1) 雖薑 彰嬪曖 瑞Щ蒂 熱чж貊 雖薑脹 詭模萄蒂 ��轎и棻.
//			��轎⑽衝 : BOOL _OBJ::Method( *_ITERATOR )
//		2) ��轎脹 л熱 唸婁縑 評塭 碟晦и棻.
//			TRUE	: п渡 喻萄蒂 餉薯 �� 棻擠 喻萄煎 剩橫除棻.
//			FALSE	: 棻擠 喻萄煎 剩橫除棻.
//	(WAVERIX)(WARNING)
//		檜 煎霜擎 п渡 л熱 頂睡縑憮 滌紫煎 delete, insert蛔擊 熱чж朝 唳辦縑 薑鼻瞳檣 翕濛擊 100%爾濰ж雖 跤и棻.
//		橾欽 濠晦 濠褐擊 薯剪ж朝 唳辦朝 瞰渠煎 Яп撿 и棻.
template<typename _STLDS, typename _ITERATOR, typename _OBJFUNC>
inline VOID		Foreach_IfTrueThenReleaseAndDeleteNode_TreeDS( _STLDS& _ds, _ITERATOR _it, _ITERATOR _itend, _OBJFUNC _funcIsDelete, _OBJFUNC _funcRelease )
{
	Foreach_IfTrueThenReleaseAndDeleteNode( _ds, _it, _itend, _funcIsDelete, _funcRelease );
}


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<SyncIterator>
//		Loop 晦遽, iterator曖 鼠唸撩擊 爾濰ж晦 嬪и Wrapper贗楚蝶
//	(WAVERIX)(WARNING)
//		檜勘 噙晦 瞪縑 掘褻瞳戲煎 偃摹й 熱 氈擊雖曖 罹睡蒂 顫瞪ж塭.
//		檜匙擎 僥薯陛 氈朝 掘褻縑憮 僥薯薄擊 撿晦ж雖 彊紫煙 爾諫ж晦 嬪п憮 堅寰脹 匙橾 閨檜棻.
//	<Use Case>
//		棻擠婁 偽擎 鼻�窕� 嫦儅й 陽 餌辨п紫 寰瞪ж紫煙 掘撩ц棻.
//		-	Loop寰縑憮 Insert/Delete陛 綴廓�� 橾橫陪棻. (濠晦 濠褐擊 餉薯й 熱 氈擊 雖 賅艇棻.)
//		-	Loop寰縑憮 翕橾и 濠猿掘褻縑 渠п loop陛 傳 嫦儅и棻. (檜 萃 賅舒陛 Insert/Delete陛 嫦儅й 唳辦)
//		-	Insert/Delete陛 嫦儅ж朝 loop縑憮 subloop嫦儅衛, п渡 loop陛 Insert/Delete嫦儅ж雖 彊戲賊 橾奩 瑞Щ煎紫 陛棟
//	<Sample>
//		typedef SyncIterator<STATUS_TABLE_IT>		SyncStatusIterator;
//		typedef SyncStatusIterator::Transaction		SyncTransaction;
//
//		// loop蒂 紫朝 衛薄
//		SyncTransaction trans( m_SyncIterator );	// loop蒂 給晦瞪 奩萄衛 transaction擊 勘橫塭.
//		SyncStatusIterator::Iterator it = m_SyncIterator.SetIterator( m_StatusTable.begin() ); // create wrap_iterator
//		STATUS_TABLE_IT itend = m_StatusTable.end();
//		for( ; it!=itend ; ++it )
//		{
//			BaseStatus* pStatus = it->second;
//			...
//
//		// 瑞Щ 寰 remove (嬴楚 sample擎 key煎 ゎ陛й 陽)
//		if( m_SyncIterator.IsTransaction() && (m_SyncIterator.CurrentIterator()->first == eStateID) )
//			m_SyncIterator.EraseCurrentIterator( m_StatusTable );
//		else
//			m_StatusTable.erase( eStateID );
//
//		// iterator煎 薯剪й 熱 氈棻賊 (m_SyncIterator.CurrentIterator() == deliterator)煎 ゎ陛ж塭
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
	bool						m_bTransaction[TRANSACTION_DEPTH];	// loop transaction 罹睡 っ滌辨
	iterator_type				m_CurIterators[TRANSACTION_DEPTH];	// loop transaction衛 餌辨腎朝 iterator
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<ADDIN-UTILITIES>
//
#include "SunSTL.UtiltiyTMP.h"
//
//

#endif //__SUN_STL_UTILITY__













