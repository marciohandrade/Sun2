#include "StdAfx.h"
#include ".\quickmanager.h"
#include ".\SlotManager.h"
#include ".\SCQuickSlot.h"
#include ".\SCQuickSlotContainer.h"

#include <SlotInsertResultConvert.h>

QuickManager::QuickManager(void)
{
}

QuickManager::~QuickManager(void)
{
}

VOID QuickManager::Release()
{
}

VOID QuickManager::Init( SlotManager * pMGR )
{
	m_pPlayer			= pMGR->player_;
	m_ppSlotContainer	= pMGR->slot_container_list_;
	m_pSlotMGR			= pMGR;
}

BOOL QuickManager::IsEmpty( POSTYPE pos )
{
	return m_pSlotMGR->IsEmpty( SI_QUICK, pos );
}

SCQuickSlotContainer * QuickManager::GetQuickSlotContainer()
{
	return (SCQuickSlotContainer *)m_pSlotMGR->GetSlotContainer(SI_QUICK);
}

BOOL QuickManager::ValidPos( SLOTIDX atIndex, POSTYPE atPos, BOOL bEmptyCheck )
{
	return m_pSlotMGR->ValidPos(atIndex, atPos, bEmptyCheck);
}

BOOL QuickManager::ValidState()
{
	return m_pSlotMGR->ValidState();
}
//
//VOID QuickManager::SerializeQuickStream( POSTYPE pos, QUICKSTREAM * IN pStream, eSERIALIZE eType )
//{
//	if( eType == SERIALIZE_LOAD )
//	{
//		SCQuickSlot & rSlot = static_cast<SCQuickSlot &>(GetQuickSlotContainer()->GetSlot(pos));
//		rSlot.CopyOut(*pStream);
//	}
//	else if( eType == SERIALIZE_STORE )
//		((SCQuickSlotContainer*)GetQuickSlotContainer())->InsertSlot( pos, *pStream );
//}

VOID QuickManager::UpdateOriginalItems()
{
	// 현재 <060526> return 이후의 코드를 사용하지 않는다.
	return;
	//SCQuickSlotContainer * pQuickContainer = GetQuickSlotContainer();
	//SCSlotContainer * pInvenContainer = m_pSlotMGR->GetSlotContainer(SI_INVENTORY);

	//for( POSTYPE pos = 0 ; pos < pQuickContainer->GetMaxSlotNum() ; ++pos )
	//{
	//	if( !pQuickContainer->IsEmpty(pos) )
	//	{
	//		SCQuickSlot & rQuickSlot = (SCQuickSlot &)pQuickContainer->GetSlot(pos);
	//		if( rQuickSlot.GetOrgSlotIndex() == SI_INVENTORY && rQuickSlot.GetOrgPos() != 255 )
	//		{
	//			SCItemSlot & rItemSlot = (SCItemSlot &)pInvenContainer->GetSlot( rQuickSlot.GetOrgPos() );
	//			if( rItemSlot.GetCode() != rQuickSlot.GetOrgCode() )
	//			{
	//				_IteratingOriginalItem( pInvenContainer, rQuickSlot, rQuickSlot.GetOrgCode() );
	//			}
	//		}
	//	}
	//}
}
/*
// CHANGES f100825.2L, remove an unused method
VOID QuickManager::_IteratingOriginalItem( SCSlotContainer * pContainer, SCQuickSlot & rQuickSlot, SLOTCODE ItemCode )
{
	for( POSTYPE pos = 0 ; pos < pContainer->GetMaxSlotNum() ; ++pos )
	{
		if( !pContainer->IsEmpty(pos) )
		{
			SCItemSlot & rSlot = (SCItemSlot &)pContainer->GetSlot(pos);
			if( rSlot.GetItemCode() == ItemCode )
			{
				SCQuickSlotContainer * pQuickContainer = GetQuickSlotContainer();
				pQuickContainer->UpdateQuickSlot( rQuickSlot.GetOrgSlotIndex(), rQuickSlot.GetOrgPos(), rQuickSlot.GetOrgCode(),
													rQuickSlot.GetOrgSlotIndex(), pos, rQuickSlot.GetOrgCode() );
				break;
			}
		}
	}
}
*/

void QuickManager::UpdateLinkedItem(const POSTYPE target_quick_slot_position, 
                                    const SLOTIDX new_linked_item_container_type, 
                                    const POSTYPE new_linked_item_position)
{
    if (ValidState() == FALSE)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|상태 검사에 실패했습니다.|캐릭터이름 = %s|"), 
               __FUNCTION__, 
               m_pPlayer->GetCharName());
        return;
    }

    if (ValidPos(SI_QUICK, target_quick_slot_position, TRUE) == FALSE)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|퀵슬롯 위치 검사에 실패했습니다.|퀵슬롯위치 = %d, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               target_quick_slot_position, 
               m_pPlayer->GetCharName());
        return;
    }

    if (ValidPos(new_linked_item_container_type, new_linked_item_position, TRUE) == FALSE)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|연결된 아이템 위치 검사에 실패했습니다.|연결된아이템컨테이너 = %d, 연결된아이템위치 = %d, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               new_linked_item_container_type, 
               new_linked_item_position, 
               m_pPlayer->GetCharName());
        return;
    }

    // 연결된 아이템 위치 범위를 검사한다.
    SCSlotContainer* const new_linked_item_container = 
        m_pSlotMGR->GetSlotContainer(new_linked_item_container_type);
    const POSTYPE new_linked_item_max_slot_num = new_linked_item_container->GetMaxSlotNum();
    if (new_linked_item_position >= new_linked_item_max_slot_num)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|연결된 아이템 위치 검사에 실패했습니다.|연결된아이템컨테이너 = %d, 연결된아이템위치 = %d, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               new_linked_item_container_type, 
               new_linked_item_position, 
               m_pPlayer->GetCharName());
        return;
    }

    // 이전 위치를 삭제하고
    SCQuickSlotContainer* const quick_slot_container = GetQuickSlotContainer();
    if (quick_slot_container->DeleteSlot(target_quick_slot_position, NULL) == FALSE)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|갱신 대상 퀵슬롯 삭제에 실패했습니다.|퀵슬롯위치 = %d, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               target_quick_slot_position, 
               m_pPlayer->GetCharName());
        return;
    }

    // 새 링크 정보를 만들어서
    SCQuickSlot new_quick_slot;
    {
        const SCSlot& new_linked_item = 
            static_cast<const SCSlot&>(new_linked_item_container->GetSlot(new_linked_item_position));
        new_quick_slot.SetOrgPos(new_linked_item_position);
        new_quick_slot.SetOrgCode(new_linked_item.GetCode());
        new_quick_slot.SetOrgSlotIndex(new_linked_item_container_type);
    }
    
    // 이전 위치에 삽입한다.
    if (quick_slot_container->InsertSlot(target_quick_slot_position, new_quick_slot) != 
        RC::RC_INSERT_SLOT_SUCCESS)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|갱신 대상 퀵슬롯 삽입에 실패했습니다.|퀵슬롯위치 = %d, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               target_quick_slot_position, 
               m_pPlayer->GetCharName());
        return;
    }
}

RC::eQUICK_RESULT QuickManager::LinkSkill( SLOTCODE code, POSTYPE toPos )
{
	if( !ValidState() ) 
		return RC::RC_QUICK_INVALIDSTATE;
	if( !ValidPos(  SI_QUICK,  toPos, FALSE ) )
		return RC::RC_QUICK_INVALIDPOS;
	
	SCSlotContainer * pContainer = GetQuickSlotContainer();
	if( !pContainer->IsEmpty(toPos) )
	{
		// 덮어쓰기
		pContainer->DeleteSlot( toPos, NULL );
	}

	// 어떤 것을 올릴 수 있는지에 대한 체크 필요!

	SCQuickSlot slot;
	slot.SetOrgCode(code);
	slot.SetOrgSlotIndex(SI_SKILL);
	pContainer->InsertSlot(toPos, slot);

	return RC::RC_QUICK_SUCCESS;
}

RC::eQUICK_RESULT QuickManager::LinkItem( POSTYPE orgPos, POSTYPE toPos )
{
	if( !ValidState() ) 
		return RC::RC_QUICK_INVALIDSTATE;
	if( !ValidPos(  SI_INVENTORY,  orgPos ) )
		return RC::RC_QUICK_INVALIDPOS;
	if( !ValidPos(  SI_QUICK,  toPos, FALSE ) )
		return RC::RC_QUICK_INVALIDPOS;

	//유료화 탭의 아이템도 링크 할 수 있다.
	POSTYPE MaxSlotNum = m_pSlotMGR->GetSlotContainer(SI_INVENTORY)->GetMaxSlotNum();
	if( orgPos >= MaxSlotNum )
		return RC::RC_QUICK_INVALIDPOS;

	SCSlotContainer * pContainer = GetQuickSlotContainer();
	if( !pContainer->IsEmpty(toPos) )
	{
		// 덮어쓰기
		pContainer->DeleteSlot( toPos, NULL );
	}


	// 어떤 것을 올릴 수 있는지에 대한 체크 필요!

	SCSlotContainer * pOrgContainer = m_pSlotMGR->GetSlotContainer(SI_INVENTORY);
	SCSlot & rOrgSlot = (SCSlot & )pOrgContainer->GetSlot(orgPos);

	SCQuickSlot slot;
	slot.SetOrgPos(orgPos);
	slot.SetOrgCode(rOrgSlot.GetCode());
	slot.SetOrgSlotIndex(SI_INVENTORY);

	RC::eSLOT_INSERT_RESULT rt = pContainer->InsertSlot(toPos, slot);
	ASSERT( rt == RC::RC_INSERT_SLOT_SUCCESS );

	return RC::RC_QUICK_SUCCESS;
}

RC::eQUICK_RESULT QuickManager::Unlink( POSTYPE atPos )
{
	if( !ValidState() ) 
		return RC::RC_QUICK_INVALIDSTATE;
	if( !ValidPos(  SI_QUICK,  atPos ) )
		return RC::RC_QUICK_INVALIDPOS;

	SCSlotContainer * pContainer = GetQuickSlotContainer();
	pContainer->DeleteSlot( atPos, NULL );

	return RC::RC_QUICK_SUCCESS;
}

RC::eQUICK_RESULT QuickManager::Move( POSTYPE fromPos, POSTYPE toPos )
{
	if( !ValidState() ) 
		return RC::RC_QUICK_INVALIDSTATE;
	if( !ValidPos(  SI_QUICK,  fromPos ) )
		return RC::RC_QUICK_INVALIDPOS;
	if( !ValidPos(  SI_QUICK,  toPos, FALSE ) )
		return RC::RC_QUICK_INVALIDPOS;

	SCSlotContainer * pContainer	= GetQuickSlotContainer();

	SCQuickSlot FromSlot;
	pContainer->DeleteSlot( fromPos, &FromSlot );
	if( FALSE != pContainer->IsEmpty( toPos ) )
	{
		// 한 방향 이동
		RC::eSLOT_INSERT_RESULT rt = pContainer->InsertSlot( toPos, FromSlot );
		if( RC::RC_INSERT_SLOT_SUCCESS != rt )
		{
			pContainer->InsertSlot( fromPos, FromSlot );
			return SlotInsertResultConverter::ConvertToeQuickResult(rt);
		}
	}
	else
	{
		// 교환
		SCQuickSlot ToSlot;
		pContainer->DeleteSlot( toPos, &ToSlot );

		RC::eSLOT_INSERT_RESULT rt = pContainer->InsertSlot( fromPos, ToSlot );
		if( RC::RC_INSERT_SLOT_SUCCESS != rt )
		{
			//rollback
			pContainer->InsertSlot( fromPos, FromSlot );
			pContainer->InsertSlot( toPos, ToSlot );
			return SlotInsertResultConverter::ConvertToeQuickResult(rt);
		}
		rt = pContainer->InsertSlot( toPos, FromSlot );
		if( RC::RC_INSERT_SLOT_SUCCESS != rt )
		{
			//rollback
			pContainer->DeleteSlot( fromPos, NULL );
			pContainer->InsertSlot( fromPos, FromSlot );
			pContainer->InsertSlot( toPos, ToSlot );
			return SlotInsertResultConverter::ConvertToeQuickResult(rt);
		}
	}

	return RC::RC_QUICK_SUCCESS;
}

RC::eQUICK_RESULT QuickManager::UnlinkByCode( SLOTIDX atIndex, SLOTCODE Code )
{
	if( !ValidState() ) 
		return RC::RC_QUICK_INVALIDSTATE;

	if( atIndex != SI_INVENTORY &&
		atIndex != SI_EQUIPMENT &&
		atIndex != SI_SKILL)
	{
		SUNLOG( eCRITICAL_LOG, "UnlinkByCode() : Index (%u)", atIndex );
		return RC::RC_QUICK_INVALIDSLOTIDX;
	}

	SCSlotContainer * pQuickSlotContainer = GetQuickSlotContainer();
	if( pQuickSlotContainer->GetSlotNum() > 0 )
	{
		for( POSTYPE i = 0 ; i < pQuickSlotContainer->GetMaxSlotNum() ; ++i )
		{
			if( pQuickSlotContainer->IsEmpty(i) )
				continue;

			SCQuickSlot *pQuickSlot = (SCQuickSlot*)&(pQuickSlotContainer->GetSlot(i));
			if( !pQuickSlot )
				continue;

			if( pQuickSlot->GetOrgSlotIndex() != atIndex)
				continue;

			if( pQuickSlot->GetOrgCode() != Code )
				continue;

			pQuickSlotContainer->DeleteSlot( i, NULL );
			break;
		}
	}

	return RC::RC_QUICK_SUCCESS;
}

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
RC::eQUICK_RESULT QuickManager::ChangeSkill( SLOTCODE old_skill_code, SLOTCODE new_skill_code )
{
    if( !ValidState() ) 
        return RC::RC_QUICK_INVALIDSTATE;

    SCSlotContainer * pQuickSlotContainer = GetQuickSlotContainer();
    if( pQuickSlotContainer->GetSlotNum() > 0 )
    {
        for( POSTYPE i = 0 ; i < pQuickSlotContainer->GetMaxSlotNum() ; ++i )
        {
            if( pQuickSlotContainer->IsEmpty(i) )
                continue;

            SCQuickSlot *pQuickSlot = (SCQuickSlot*)&(pQuickSlotContainer->GetSlot(i));
            if( !pQuickSlot )
                continue;

            if( pQuickSlot->GetOrgSlotIndex() != SI_SKILL)
                continue;

            if( pQuickSlot->GetOrgCode() != old_skill_code )
                continue;

            pQuickSlotContainer->DeleteSlot( i, NULL );

            SCQuickSlot slot;
            slot.SetOrgCode(new_skill_code);
            slot.SetOrgSlotIndex(SI_SKILL);
            pQuickSlotContainer->InsertSlot(i, slot);

            return RC::RC_QUICK_SUCCESS;
        }
    }

    return RC::RC_QUICK_FAILED;
}
#endif