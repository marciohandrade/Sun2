#pragma once

#include <ResultCode.h>
#include <SCSlotHeader.h>
#include <SCItemSlot.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
struct sITEM_POLICY
{
	typedef __time64_t	TIME_SPEC;
	enum ePOLICY {
		ITEM_POLICY_NOT_DEFINED			= 0,
		ITEM_POLICY_NO_CONTROL			= ITEM_POLICY_NOT_DEFINED,	// 컨트롤할 정보 없음
		ITEM_POLICY_EXPIRATION_ITEM		= 1<<0,		// 캐시는 아니지만 기간제 아이템인 경우
		ITEM_POLICY_RENDER_EFFECT		= 1<<1,		// 특정 Item에 의한 Render Effect가 적용되는 경우 (eSPECIAL_RENDER_EFFECT)
		ITEM_POLICY_EXCLUSIVE_PAIR		= 1<<2,		// 캐릭터에 서로 다른 종류 아이템중 택일만 가능
	};

	ePOLICY				POLICY;
	union uSHARED_BLK
	{
		struct sEXPIRATION_ITEM
		{
			CODETYPE				ItemCode;
			TIME_SPEC				ExpiredTime;
		}							EXPIRATION_ITEM;

		struct sEXCLUSIVE_PAIR
		{
			CODETYPE				XOR_Item0;
			CODETYPE				XOR_Item1;
		}							EXCLUSIVE_PAIR;

		struct sRENDER_EFFECT
		{
			CODETYPE				ItemCode;
			eSPECIAL_RENDER_EFFECT	RenderEffect;
		}							RENDER_EFFECT;
	}								SHARED_BLK;

	typedef uSHARED_BLK::sEXPIRATION_ITEM		sEXPIRATION_ITEM;
	typedef uSHARED_BLK::sEXCLUSIVE_PAIR		sEXCLUSIVE_PAIR;
	typedef uSHARED_BLK::sRENDER_EFFECT			sRENDER_EFFECT;
	inline VOID					Clear() { POLICY = ITEM_POLICY_NOT_DEFINED; ZeroMemory( &SHARED_BLK, sizeof(SHARED_BLK) ); }
	inline sEXPIRATION_ITEM&	GetExpirationInfo() { return SHARED_BLK.EXPIRATION_ITEM; }
	inline sEXCLUSIVE_PAIR&		GetExclusivePairInfo() { return SHARED_BLK.EXCLUSIVE_PAIR; }
	inline sRENDER_EFFECT&		GetRenderEffectInfo() { return SHARED_BLK.RENDER_EFFECT; }
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<AppliedItemDependencyChain>
class AppliedItemDependencyChain : public IWorkNode
{
private:	static const DWORD								INTERVALforUPDATE		= 10*1000;
private:	typedef sITEM_POLICY::ePOLICY					ePOLICY;
			typedef sITEM_POLICY::sEXPIRATION_ITEM			sEXPIRATION_ITEM;
			typedef sITEM_POLICY::sEXCLUSIVE_PAIR			sEXCLUSIVE_PAIR;
			typedef sITEM_POLICY::sRENDER_EFFECT			sRENDER_EFFECT;
			typedef sITEM_POLICY::TIME_SPEC					TIME_SPEC;

private:	typedef std::pair<ePOLICY, sITEM_POLICY>				POLICY_NODE;
			typedef STLX_LIST<POLICY_NODE>					POLICY_LIST;
			typedef POLICY_LIST::iterator					POLICY_LIST_IT;

			typedef sITEM_POLICY::ePOLICY					ePOLICY_BITSET;	// 해당 코드에 관련된 모든 POLICY bitset
			typedef sITEM_POLICY*							sXOR_ITEM_PTR;	// XOR 속성을 가지는 경우에 할당됨
			typedef sITEM_POLICY*							sEXPIRED_PTR;	// 기간제 속성을 가지는 경우
			typedef sITEM_POLICY*							sEFFECT_PTR;	// 기간제 속성을 가지는 경우
			struct ITEMPOLICY_NODE
			{
				sEXPIRED_PTR		m_pTIME;
				sXOR_ITEM_PTR		m_pXOR;
				sEFFECT_PTR			m_pEFFECT;
				ITEMPOLICY_NODE(
					const sEXPIRED_PTR pExp=NULL,
					const sXOR_ITEM_PTR pXor=NULL,
					const sEFFECT_PTR pEffect=NULL
					) : m_pTIME(pExp), m_pXOR(pXor), m_pEFFECT(pEffect)
				{}
			};
			//typedef pair<sEXPIRED_PTR, sXOR_ITEM_PTR>		ITEMPOLICY_NODE;
			typedef std::pair<ePOLICY_BITSET, ITEMPOLICY_NODE>	ITEMCODE_LINK;	//
			typedef STLX_MAP<CODETYPE, ITEMCODE_LINK>		ITEMCODE_LIST;
			typedef ITEMCODE_LIST::iterator					ITEMCODE_LIST_IT;
			typedef std::pair<CODETYPE, ITEMCODE_LINK>			ITEMCODE_NODE;

private:	AppliedItemDependencyChain();
public:
	virtual	~AppliedItemDependencyChain();
	static	VOID					CreateInstance() { ms_pInstance ? 0 : new AppliedItemDependencyChain(); }
	static	VOID					DestoryInstance() { SAFE_DELETE(ms_pInstance); }
	inline
	static	AppliedItemDependencyChain*		Instance() { return ms_pInstance; }
	inline static WORK_KEY			GetWorkKey() { return ms_WorkKey; }
	inline static VOID				SetWorkKey( WORK_KEY wKey ) { ms_WorkKey = wKey; }
public:
	virtual	BOOL					DoInit();
	virtual	BOOL					DoProcess();
	virtual	BOOL					IsComplete();
	virtual	BOOL					DoRelease();
	// Control Interface
	virtual	VOID					OnMsg( sSIG_MSG* pSIG_MSG );

	inline	ePOLICY					GetItemDependencyInfo( const CODETYPE itemCode )
	{
		ITEMCODE_LIST_IT itFIND = m_ItemCodeChain.find(itemCode);
		return itFIND != m_ItemCodeChain.end() ? itFIND->second.first : sITEM_POLICY::ITEM_POLICY_NO_CONTROL;
	}
	inline	ePOLICY					GetItemDependencyInfo( const CODETYPE itemCode, ITEMCODE_LIST_IT& OUT rIT )
	{
		ITEMCODE_LIST_IT itFIND = m_ItemCodeChain.find(itemCode);
		rIT = itFIND;
		return itFIND != m_ItemCodeChain.end() ? itFIND->second.first : sITEM_POLICY::ITEM_POLICY_NO_CONTROL;
	}
	

	inline	BOOL					IsExpirationItem( const CODETYPE itemCode )
	{	return !!( sITEM_POLICY::ITEM_POLICY_EXPIRATION_ITEM&GetItemDependencyInfo(itemCode) );	}

	inline	BOOL					IsExclusiveItem( const CODETYPE itemCode )
	{	return !!( sITEM_POLICY::ITEM_POLICY_EXCLUSIVE_PAIR&GetItemDependencyInfo(itemCode) );	}

	inline	BOOL					IsRenderEffectItem( const CODETYPE itemCode )
	{	return !!( sITEM_POLICY::ITEM_POLICY_RENDER_EFFECT&GetItemDependencyInfo(itemCode) );	}

public:		CODETYPE				GetExclusiveItem( const CODETYPE itemCode );
			CODETYPE				GetExclusiveItem( const CODETYPE itemCode, ITEMCODE_LIST_IT& IN rIT );

			VOID					AddPolicyExpirationItem( const CODETYPE itemCode, const TIME_SPEC expiredTime );
			VOID					AddPolicyExclusivePair( const CODETYPE itemCode0, const CODETYPE itemCode1 );
			VOID					AddPolicyRenderEffectItem( const CODETYPE itemCode, const eSPECIAL_RENDER_EFFECT effect );
			VOID					RemovePolicyExpirationItem( const CODETYPE itemCode );
			VOID					RemovePolicyExclusivePair( const CODETYPE itemCode );
			VOID					RemovePolicyRenderEffectItem( const CODETYPE itemCode );

public:		BOOL					IsAcceptableInsert( Player* IN pPlayer, SCItemSlot& IN rItemSlot, const INT iAddNum );
			BOOL					IsExpiredItem( SCItemSlot& IN rItemSlot );
			BOOL					CheckAndApplyEffect( Player* IN pPlayer, SCItemSlot& IN rItemSlot, const BOOL bInserted, const INT changed_total_count );

private:	VOID					_AddPolicyExclusivePair( const CODETYPE itemCode, POLICY_LIST_IT itInserted );

private:	POLICY_LIST					m_PolicyChain;
			ITEMCODE_LIST				m_ItemCodeChain;
			ITimerBase					m_UpdateTimer;
			TIME_SPEC					m_CurrentTime;

	static	WORK_KEY					ms_WorkKey;
	static	AppliedItemDependencyChain*	ms_pInstance;
};


