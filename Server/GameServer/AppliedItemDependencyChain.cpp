#include "stdafx.h"
#include "AppliedItemDependencyChain.h"
#include <SunEventInfoParser.h>
#include <ItemManager.h>

//����������������������������������������������������������������������������������������������
//	<AppliedItemDependencyChain>
WORK_KEY						AppliedItemDependencyChain::ms_WorkKey = 0;
AppliedItemDependencyChain*		AppliedItemDependencyChain::ms_pInstance = NULL;


AppliedItemDependencyChain::AppliedItemDependencyChain()
{
	ms_pInstance = this;
}

AppliedItemDependencyChain::~AppliedItemDependencyChain()
{
	ms_pInstance = NULL;
}

BOOL			AppliedItemDependencyChain::DoInit()
{
	//////////////////////////////////////////////////////////////////////////
	//	SunEvent�� ���� Item �������� ����
	SunEventInfoParser* pParser = SunEventInfoParser::Instance();
    // CHANGES: f110408.3L, added interface to support external control like a event.
    ItemInfoParser* const item_parser = ItemInfoParser::Instance();
	EVENTINFO_MAP& rMap = *pParser->GetEventInfoMap();

	EVENTINFO_MAP_ITER itend=rMap.end();
	for( EVENTINFO_MAP_ITER it=rMap.begin() ; it!=itend ; ++it )
	{
		const EVENT_INFO& rEventInfo = it->second;
		if( !rEventInfo.m_IsDeleteFlagForRewardedItem )
			continue;

		for( DWORD i=0 ; i<EVENT_INFO::_MAX_DELETE_ITEM_CODES ; ++i )
		{
			const CODETYPE itemCode = rEventInfo.m_DeleteRewaredItemCode[i];
			if( !itemCode )
				break;

			AddPolicyExpirationItem( itemCode, rEventInfo.m_end_time );
            // CHANGES: f110408.3L, this tag is used by ItemManager::UpdateExpirationItem
            item_parser->ChangeToTimeExpirationItem(itemCode);
		}					
	}

	//	<�縳�� �� ���� Item Pair>
	//		Ư�� HardCoding - ù ��ʴ� '������'�� ���� ���̸�, �ٷ� �տ� �����ϴ� ��ƾ���� �ϰ� ó���� �� �ִ�.
	//		������, �Ⱓ�� �������� ������ ��ũ��Ʈ �󿡼� �򰡰� ����������, �̰��� �׷��� ���ϴ�.
	//		���� �̷� ���ǵ��� �Ϲ�ȭ �� ��쿡 ������ �ڵ�ȭ ��Ű��� �ϰ�, �ϴ� Ư�� ó���� ��������.
    
#ifdef _NA_006624_20130409_CAN_OBTAIN_SOLOBADGE_COUPLEBADGE
    // 2013.04.09 �ΰ�� , ���� ������Ʈ�� �� ������ �Բ� �����Ͽ��� ������ ���� ������ �Բ� ���� �� �ֵ��� �� �Լ��� �ּ�ó��.
#else
	AddPolicyExclusivePair( eSPECIAL_RENDER_EFFECT_ADDIN_INFO_SOLO_BADGE, eSPECIAL_RENDER_EFFECT_ADDIN_INFO_COUPLE_BADGE );
#endif //_NA_006624_20130409_CAN_OBTAIN_SOLOBADGE_COUPLEBADGE

	//	<REF>	ItemInfoParser :: EffectItems for Rendering in Inventory
	//	-	eITEMTYPE_~~
	typedef ItemInfoParser::RENDER_EFFECT_ITEM_LIST		RENDER_EFFECT_ITEM_LIST;

	ItemInfoParser* const pItemInfoParser = ItemInfoParser::Instance();
	const RENDER_EFFECT_ITEM_LIST& rList = pItemInfoParser->RENDER_EFFECT_ITEMS;
	FOREACH_CONTAINER( const SLOTCODE itemCode, rList, RENDER_EFFECT_ITEM_LIST )
	{
		AddPolicyRenderEffectItem( itemCode, eSPECIAL_RENDER_EFFECT_NONE );
	}

	util::TimeSync::_time64(&m_CurrentTime);
	m_UpdateTimer.SetTimer(INTERVALforUPDATE);

	return TRUE;
}

BOOL			AppliedItemDependencyChain::DoProcess()
{
	if( m_UpdateTimer.IsExpired() )
	{
		util::TimeSync::_time64(&m_CurrentTime);
	}
	return TRUE;
}

BOOL			AppliedItemDependencyChain::IsComplete()
{
	return FALSE;
}

BOOL			AppliedItemDependencyChain::DoRelease()
{
	return TRUE;
}

VOID			AppliedItemDependencyChain::OnMsg( sSIG_MSG* pSIG_MSG )
{
	return;
}

CODETYPE		AppliedItemDependencyChain::GetExclusiveItem( const CODETYPE itemCode, ITEMCODE_LIST_IT& IN rIT )
{
	if( rIT == m_ItemCodeChain.end() )
		return 0;

	ITEMPOLICY_NODE& rNODE = rIT->second.second;
	sXOR_ITEM_PTR pXOR = rNODE.m_pXOR;
	if( !pXOR )
		return 0;

	sITEM_POLICY::sEXCLUSIVE_PAIR& rPAIR = pXOR->GetExclusivePairInfo();
	CODETYPE retCode = itemCode == rPAIR.XOR_Item0 ? rPAIR.XOR_Item1 : rPAIR.XOR_Item0;

	return retCode;
}

CODETYPE		AppliedItemDependencyChain::GetExclusiveItem( const CODETYPE itemCode )
{
	ITEMCODE_LIST_IT itFIND = m_ItemCodeChain.find(itemCode);
	return GetExclusiveItem( itemCode, itFIND );
}

VOID			AppliedItemDependencyChain::AddPolicyExpirationItem( const CODETYPE itemCode, const TIME_SPEC expiredTime )
{
	ITEMCODE_LIST_IT itFIND;
	const ePOLICY policy = GetItemDependencyInfo(itemCode, itFIND);
	if( !(sITEM_POLICY::ITEM_POLICY_EXPIRATION_ITEM&policy)  )
	{
		sITEM_POLICY rPolicy;
		rPolicy.Clear();
		rPolicy.POLICY = sITEM_POLICY::ITEM_POLICY_EXPIRATION_ITEM;
		sITEM_POLICY::sEXPIRATION_ITEM& rExpireItem = rPolicy.SHARED_BLK.EXPIRATION_ITEM;
		rExpireItem.ItemCode = itemCode;
		rExpireItem.ExpiredTime = expiredTime;

		POLICY_LIST_IT it = m_PolicyChain.insert( m_PolicyChain.end(), std::make_pair( rPolicy.POLICY, rPolicy ) );

		ITEMCODE_LIST_IT itFIND = m_ItemCodeChain.find(itemCode);
		if( itFIND == m_ItemCodeChain.end() )
		{
			ITEMCODE_LINK rLink = ITEMCODE_LINK( sITEM_POLICY::ITEM_POLICY_EXPIRATION_ITEM,
											ITEMPOLICY_NODE( &(it->second), NULL )
										);

			m_ItemCodeChain.insert( ITEMCODE_NODE( itemCode, rLink ) );
		}
		else
		{
			// �ٸ� ��å�� �����ϴ� ���.
			ITEMCODE_LINK& rLink = itFIND->second;
			ePOLICY_BITSET& rBit = rLink.first;
			ITEMPOLICY_NODE& rNODE = rLink.second;
			rNODE.m_pTIME = &(it->second);

			rBit = (ePOLICY_BITSET) ( (DWORD)rBit | (DWORD)sITEM_POLICY::ITEM_POLICY_EXPIRATION_ITEM );
		}
	}
}

VOID			AppliedItemDependencyChain::_AddPolicyExclusivePair( const CODETYPE itemCode, POLICY_LIST_IT itInserted )
{
	ITEMCODE_LIST_IT itFIND = m_ItemCodeChain.find( itemCode );
	if( itFIND == m_ItemCodeChain.end() )
	{
		ITEMCODE_LINK rLink = ITEMCODE_LINK( sITEM_POLICY::ITEM_POLICY_EXCLUSIVE_PAIR,
			ITEMPOLICY_NODE( NULL, &(itInserted->second) )
			);

		m_ItemCodeChain.insert( ITEMCODE_NODE( itemCode, rLink ) );
	}
	else
	{
		// �ٸ� ��å�� �����ϴ� ���.
		ITEMCODE_LINK& rLink = itFIND->second;
		ePOLICY_BITSET& rBit = rLink.first;
		ITEMPOLICY_NODE& rNODE = rLink.second;
		rNODE.m_pXOR = &(itInserted->second);

		rBit = (ePOLICY_BITSET) ( (DWORD)rBit | (DWORD)sITEM_POLICY::ITEM_POLICY_EXCLUSIVE_PAIR );
	}
}

VOID			AppliedItemDependencyChain::AddPolicyExclusivePair( const CODETYPE itemCode0, const CODETYPE itemCode1 )
{
	ITEMCODE_LIST_IT itFIND;
	const ePOLICY policy = GetItemDependencyInfo(itemCode0, itFIND);	// �ϳ��� �˻��ص� �� ���� �˻��ϴ� ���...
	if( !(sITEM_POLICY::ITEM_POLICY_EXCLUSIVE_PAIR&policy)  )
	{
		sITEM_POLICY rPolicy;
		rPolicy.Clear();
		rPolicy.POLICY = sITEM_POLICY::ITEM_POLICY_EXCLUSIVE_PAIR;
		sITEM_POLICY::sEXCLUSIVE_PAIR& rXorItem = rPolicy.SHARED_BLK.EXCLUSIVE_PAIR;
		rXorItem.XOR_Item0 = itemCode0;
		rXorItem.XOR_Item1 = itemCode1;

		POLICY_LIST_IT it = m_PolicyChain.insert( m_PolicyChain.end(), std::make_pair( rPolicy.POLICY, rPolicy ) );

		_AddPolicyExclusivePair( itemCode0, it );
		_AddPolicyExclusivePair( itemCode1, it );
	}
}

VOID			AppliedItemDependencyChain::AddPolicyRenderEffectItem( const CODETYPE itemCode, const eSPECIAL_RENDER_EFFECT effect )
{
	ITEMCODE_LIST_IT itFIND;
	const ePOLICY policy = GetItemDependencyInfo(itemCode, itFIND);
	if( !(sITEM_POLICY::ITEM_POLICY_RENDER_EFFECT&policy)  )
	{
		sITEM_POLICY rPolicy;
		rPolicy.Clear();
		rPolicy.POLICY = sITEM_POLICY::ITEM_POLICY_RENDER_EFFECT;
		sITEM_POLICY::sRENDER_EFFECT& rEffectItem = rPolicy.SHARED_BLK.RENDER_EFFECT;
		rEffectItem.ItemCode = itemCode;
		rEffectItem.RenderEffect = effect;

		POLICY_LIST_IT it = m_PolicyChain.insert( m_PolicyChain.end(), std::make_pair( rPolicy.POLICY, rPolicy ) );

		ITEMCODE_LIST_IT itFIND = m_ItemCodeChain.find(itemCode);
		if( itFIND == m_ItemCodeChain.end() )
		{
			ITEMCODE_LINK rLink = ITEMCODE_LINK( sITEM_POLICY::ITEM_POLICY_RENDER_EFFECT,
				ITEMPOLICY_NODE( NULL, NULL, &(it->second) )
				);

			m_ItemCodeChain.insert( ITEMCODE_NODE( itemCode, rLink ) );
		}
		else
		{
			// �ٸ� ��å�� �����ϴ� ���.
			ITEMCODE_LINK& rLink = itFIND->second;
			ePOLICY_BITSET& rBit = rLink.first;
			ITEMPOLICY_NODE& rNODE = rLink.second;
			rNODE.m_pEFFECT = &(it->second);

			rBit = (ePOLICY_BITSET) ( (DWORD)rBit | (DWORD)sITEM_POLICY::ITEM_POLICY_RENDER_EFFECT );
		}
	}
}

inline static BOOL		_IsMatchAddress( const sITEM_POLICY* item, const sITEM_POLICY* cmpbase )
{
	return (item==cmpbase);
}

VOID			AppliedItemDependencyChain::RemovePolicyExpirationItem( const CODETYPE itemCode )
{
	ITEMCODE_LIST_IT itFIND;
	const ePOLICY policy = GetItemDependencyInfo(itemCode, itFIND);
	if( sITEM_POLICY::ITEM_POLICY_EXPIRATION_ITEM&policy )
	{
		ITEMCODE_LINK& rLink = itFIND->second;
		ePOLICY_BITSET& rBit = rLink.first;
		ITEMPOLICY_NODE& rNODE = rLink.second;

		rBit = (ePOLICY_BITSET) ( (DWORD)rBit & (DWORD)(~sITEM_POLICY::ITEM_POLICY_EXPIRATION_ITEM) );
		const sEXPIRED_PTR pPtr = rNODE.m_pTIME;
		rNODE.m_pTIME = NULL;

		POLICY_LIST_IT it=m_PolicyChain.begin(), itend=m_PolicyChain.end();
		while( it!=itend )
			if( _IsMatchAddress( pPtr, &(it->second) ) )
			{
				m_PolicyChain.erase( it );
				break;
			}
	}
}

VOID			AppliedItemDependencyChain::RemovePolicyExclusivePair( const CODETYPE itemCode )
{
	ITEMCODE_LIST_IT itFIND;
	const ePOLICY policy = GetItemDependencyInfo(itemCode, itFIND);
	if( sITEM_POLICY::ITEM_POLICY_EXCLUSIVE_PAIR&policy )
	{
		ITEMCODE_LINK& rLink = itFIND->second;
		ePOLICY_BITSET& rBit = rLink.first;
		ITEMPOLICY_NODE& rNODE = rLink.second;

		rBit = (ePOLICY_BITSET) ( (DWORD)rBit & (DWORD)(~sITEM_POLICY::ITEM_POLICY_EXCLUSIVE_PAIR) );
		const sXOR_ITEM_PTR pPtr = rNODE.m_pXOR;
		rNODE.m_pXOR = NULL;

		POLICY_LIST_IT it=m_PolicyChain.begin(), itend=m_PolicyChain.end();
		while( it!=itend )
			if( _IsMatchAddress( pPtr, &(it->second) ) )
			{
				m_PolicyChain.erase( it );
				break;
			}
	}
}

VOID			AppliedItemDependencyChain::RemovePolicyRenderEffectItem( const CODETYPE itemCode )
{
	ITEMCODE_LIST_IT itFIND;
	const ePOLICY policy = GetItemDependencyInfo(itemCode, itFIND);
	if( sITEM_POLICY::ITEM_POLICY_RENDER_EFFECT&policy )
	{
		ITEMCODE_LINK& rLink = itFIND->second;
		ePOLICY_BITSET& rBit = rLink.first;
		ITEMPOLICY_NODE& rNODE = rLink.second;

		rBit = (ePOLICY_BITSET) ( (DWORD)rBit & (DWORD)(~sITEM_POLICY::ITEM_POLICY_RENDER_EFFECT) );
		const sEFFECT_PTR pPtr = rNODE.m_pEFFECT;
		rNODE.m_pEFFECT = NULL;

		POLICY_LIST_IT it=m_PolicyChain.begin(), itend=m_PolicyChain.end();
		while( it!=itend )
			if( _IsMatchAddress( pPtr, &(it->second) ) )
			{
				m_PolicyChain.erase( it );
				break;
			}
	}
}

BOOL			AppliedItemDependencyChain::IsAcceptableInsert( Player* IN pPlayer, SCItemSlot& IN rItemSlot, const INT iAddNum )
{
	// �������� ��å���� �������� �߰��� ���Ե� �� ���� ���̴�.
	const CODETYPE itemCode = rItemSlot.GetItemCode();

	ITEMCODE_LIST_IT itFIND;
	const ePOLICY policy = GetItemDependencyInfo(itemCode, itFIND);

	// �켱������ �Է� ���� ���θ� ���� �Ǵ��ؾ� �Ѵ�.
	if( sITEM_POLICY::ITEM_POLICY_EXCLUSIVE_PAIR&policy )
	{
		const CODETYPE cXOR_Item = GetExclusiveItem( itemCode, itFIND );
		if( cXOR_Item ) /* 0==cXOR_Item -> bug?*/
		{
			// �ϳ��� �����Ѵٸ�
			if( 0 != pPlayer->GetItemManager()->ExistItem( SI_INVENTORY, (SLOTCODE)cXOR_Item, 1 ) )
				return FALSE;
		}
	}

	return TRUE;
}

BOOL			AppliedItemDependencyChain::IsExpiredItem( SCItemSlot& IN rItemSlot )
{
	const CODETYPE itemCode = rItemSlot.GetItemCode();

	ITEMCODE_LIST_IT itFIND;
	const ePOLICY policy = GetItemDependencyInfo(itemCode, itFIND);
	if( sITEM_POLICY::ITEM_POLICY_EXPIRATION_ITEM&policy )
	{
		ITEMCODE_LINK& rLink = itFIND->second;
		ITEMPOLICY_NODE& rNODE = rLink.second;
		const sEXPIRED_PTR pPtr = rNODE.m_pTIME;
		const sEXPIRATION_ITEM& rInfo = pPtr->GetExpirationInfo();
		if( rInfo.ExpiredTime < m_CurrentTime )
			return TRUE;
	}
	return FALSE;
}

BOOL			AppliedItemDependencyChain::CheckAndApplyEffect( Player* IN pPlayer, SCItemSlot& IN rItemSlot, const BOOL bInserted, const INT changed_total_count )
{
	const CODETYPE itemCode = rItemSlot.GetItemCode();

	ITEMCODE_LIST_IT itFIND;
	const ePOLICY policy = GetItemDependencyInfo(itemCode, itFIND);
	if( sITEM_POLICY::ITEM_POLICY_RENDER_EFFECT&policy )
	{
		ITEMPOLICY_NODE& rNODE = itFIND->second.second;
		sEFFECT_PTR pEffect = rNODE.m_pEFFECT;
		if( pEffect )
		{
			sITEM_POLICY::sRENDER_EFFECT& rPAIR = pEffect->GetRenderEffectInfo();
			ns_object::SPECIAL_RENDER_EFFECT& rRENDER_EFFECT = pPlayer->RENDER_EFFECT;
			bInserted
				?		rRENDER_EFFECT.AddEffectItem( (SLOTCODE)itemCode, changed_total_count )
				:		rRENDER_EFFECT.RemoveEffectItem( (SLOTCODE)itemCode, changed_total_count )
				;
		}
	}

	return TRUE;
}

