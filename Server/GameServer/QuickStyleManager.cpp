#include "StdAfx.h"
#include ".\QuickStyleManager.h"

/*
// CHANGES, f100826.2L, removed

#include ".\SlotManager.h"
#include ".\SCQuickStyleSlot.h"
#include ".\SCQuickStyleSlotContainer.h"
#include "StyleQuickRegistInfoParser.h"
#include "SCSkillSlot.h"
#include "Player.h"
#include "SCSkillSlotContainer.h"
#include "SkillInfoParser.h"

#include <SlotInsertResultConvert.h>

QuickStyleManager::QuickStyleManager(void)
{
}

QuickStyleManager::~QuickStyleManager(void)
{
}

VOID QuickStyleManager::Release()
{
}

VOID QuickStyleManager::Init( SlotManager * pMGR )
{
	m_pPlayer			= pMGR->m_pPlayer;
	m_ppSlotContainer	= pMGR->m_pSlotContainer;
	m_pSlotMGR			= pMGR;
}

SCQuickStyleSlotContainer * QuickStyleManager::GetQuickStyleSlotContainer()
{
	return (SCQuickStyleSlotContainer *)m_pSlotMGR->GetSlotContainer(SI_STYLE);
}

VOID QuickStyleManager::SerializeStyleStream( POSTYPE pos, STYLESTREAM * IN pStream, eSERIALIZE eType )
{
	if( eType == SERIALIZE_LOAD )
	{
		SCQuickStyleSlot & rSlot = static_cast<SCQuickStyleSlot &>(GetQuickStyleSlotContainer()->GetSlot(pos));
		rSlot.CopyOut(*pStream);
	}
	else if( eType == SERIALIZE_STORE )
		((SCQuickStyleSlotContainer*)GetQuickStyleSlotContainer())->InsertSlot( pos, *pStream );
}

RC::eSTYLE_RESULT QuickStyleManager::Link( SLOTCODE code, POSTYPE toPos )
{
	if( !m_pSlotMGR->ValidState() ) 
		return RC::RC_STYLE_INVALIDSTATE;
	if( !m_pSlotMGR->ValidPos(  SI_STYLE,  toPos, FALSE ) )
		return RC::RC_STYLE_INVALIDPOS;

	SCSlotContainer * pContainer = GetQuickStyleSlotContainer();
	if( !pContainer->IsEmpty(toPos) )
		return RC::RC_STYLE_INVALIDPOS;

	// 어떤 것을 올릴 수 있는지에 대한 체크 필요!

	SCQuickStyleSlot slot;
	slot.SetOrgCode(code);
	pContainer->InsertSlot(toPos, slot);

	return RC::RC_STYLE_SUCCESS;
}

RC::eSTYLE_RESULT QuickStyleManager::Unlink( POSTYPE atPos )
{
	if( !m_pSlotMGR->ValidState() ) 
		return RC::RC_STYLE_INVALIDSTATE;
	if( !m_pSlotMGR->ValidPos(  SI_STYLE,  atPos ) )
		return RC::RC_STYLE_INVALIDPOS;

	SCSlotContainer * pContainer = GetQuickStyleSlotContainer();
	pContainer->DeleteSlot( atPos, NULL );

	return RC::RC_STYLE_SUCCESS;
}

RC::eSTYLE_RESULT QuickStyleManager::Move( POSTYPE fromPos, POSTYPE toPos )
{
	if( !m_pSlotMGR->ValidState() ) 
		return RC::RC_STYLE_INVALIDSTATE;
	if( !m_pSlotMGR->ValidPos(  SI_STYLE,  fromPos ) )
		return RC::RC_STYLE_INVALIDPOS;
	if( !m_pSlotMGR->ValidPos(  SI_STYLE,  toPos, FALSE ) )
		return RC::RC_STYLE_INVALIDPOS;

	SCSlotContainer *pContainer = GetQuickStyleSlotContainer();

	SCQuickStyleSlot FromSlot;
	pContainer->DeleteSlot( fromPos, &FromSlot );
	if( FALSE != pContainer->IsEmpty( toPos ) )
	{
		// 한 방향 이동
		RC::eSLOT_INSERT_RESULT rt = pContainer->InsertSlot( toPos, FromSlot );
		if( RC::RC_INSERT_SLOT_SUCCESS != rt )
		{
			pContainer->InsertSlot( fromPos, FromSlot );
			return SlotInsertResultConverter::ConvertToeStyleResult(rt);
		}
	}
	else
	{
		// 교환
		SCQuickStyleSlot ToSlot;
		pContainer->DeleteSlot( toPos, &ToSlot );

		RC::eSLOT_INSERT_RESULT rt = pContainer->InsertSlot( fromPos, ToSlot );
		if( RC::RC_INSERT_SLOT_SUCCESS != rt )
		{
			//rollback
			pContainer->InsertSlot( fromPos, FromSlot );
			pContainer->InsertSlot( toPos, ToSlot );
			return SlotInsertResultConverter::ConvertToeStyleResult(rt);
		}
		rt = pContainer->InsertSlot( toPos, FromSlot );
		if( RC::RC_INSERT_SLOT_SUCCESS != rt )		
		{
			//rollback
			pContainer->DeleteSlot( fromPos, NULL );
			pContainer->InsertSlot( fromPos, FromSlot );
			pContainer->InsertSlot( toPos, ToSlot );
			return SlotInsertResultConverter::ConvertToeStyleResult(rt);
		}
	}

	return RC::RC_STYLE_SUCCESS;
}

class QuickStyleUpdate
{
	QuickStyleManager*				m_pStyleManager;
	BASE_StyleQuickRegistInfo*	m_pStyleQuickInfo;

public:
	QuickStyleUpdate( QuickStyleManager *pStyleManager, BASE_StyleQuickRegistInfo *pStyleQuickInfo ) : 
	  m_pStyleManager(pStyleManager), m_pStyleQuickInfo(pStyleQuickInfo) {}

	VOID operator () ( SCSlot* pSlot )
	{
		SCSkillSlot *pSkillSlot = (SCSkillSlot*)pSlot;
		ROOT_SKILLINFO *pBaseSkillInfo = SkillInfoParser::Instance()->GetInfo( pSkillSlot->GetCode() );
		if( !pBaseSkillInfo )
		{
			SUNLOG( eCRITICAL_LOG, "[QuickStyleUpdate] SkillCode[%d] SkillInfo is NULL!", pSkillSlot->GetCode() );
			return;
		}

		if( pSkillSlot->IsSKill() )	return;
		const BASE_STYLEINFO *pBaseStyleInfo = pSkillSlot->GetStyleInfo();

		for( BYTE i = 0; i < MAX_STYLE_REGIST_COUNT; i++ )
		{
			if( m_pStyleQuickInfo->m_wStyleClassCode[i] != pBaseStyleInfo->m_SkillClassCode )	continue;

			// 스타일을 퀵 스타일 슬롯에 등록한다.
			RC::eSTYLE_RESULT rt = m_pStyleManager->Link( pSkillSlot->GetCode(), i );
			break;
		}
	}
};

VOID QuickStyleManager::UpdateStyleSlot()
{
	BASE_StyleQuickRegistInfo *pStyleQuickInfo = StyleQuickRegistInfoParser::Instance()->GetStyleQuickRegistInfo( m_pPlayer->GetCharType(), m_pPlayer->GetWeaponKind() );
	SCSkillSlotContainer *pSkillSlotContainer = (SCSkillSlotContainer *)m_pSlotMGR->GetSlotContainer(SI_SKILL);

	// 스타일 퀵 초기화
	GetQuickStyleSlotContainer()->ClearAll();

	// 습득한 스타일들 중에서 pStyleQuickInfo에 정보가 있는 것만 등록
	if( pStyleQuickInfo )
	{
		QuickStyleUpdate Opr( this, pStyleQuickInfo );
		pSkillSlotContainer->ForEachSlot( Opr );
	}
}

*/