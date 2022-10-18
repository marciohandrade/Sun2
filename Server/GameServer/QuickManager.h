#pragma once

//=======================================================================================================================
/// QuickManager class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2004. 8. 11~
	@history
			- 2006/2/14(발렌타인데이^^/) : SerializeQuickTotalInfo()함수 추가, stream관련 함수 template화
*/
//=======================================================================================================================

class SCSlotContainer;
class SCQuickSlotContainer;
class SlotManager;
class Player;
class SCQuickSlot;

class QuickManager
{
public:
	QuickManager(void);
	~QuickManager(void);

	VOID					Init( SlotManager * pMGR );
	VOID					Release();

	SCQuickSlotContainer *	GetQuickSlotContainer();
	BOOL					ValidPos( SLOTIDX atIndex, POSTYPE atPos, BOOL bEmptyCheck = TRUE );
	BOOL					ValidState();
	BOOL					IsEmpty( POSTYPE pos );

	template< class QUICK_TOTAL_INFO_TYPE >
	VOID					SerializeQuickTotalInfo( QUICK_TOTAL_INFO_TYPE & QuickInfo, eSERIALIZE eType );
	template< class QUICKSTREAM_TYPE >
	VOID					SerializeQuickStream( POSTYPE pos, QUICKSTREAM_TYPE * IN pStream, eSERIALIZE eType = SERIALIZE_LOAD );

	RC::eQUICK_RESULT		LinkSkill( SLOTCODE code, POSTYPE toPos );
	RC::eQUICK_RESULT		LinkItem( POSTYPE orgPos, POSTYPE toPos );
	RC::eQUICK_RESULT		Unlink( POSTYPE atPos );
	RC::eQUICK_RESULT		Move( POSTYPE fromPos, POSTYPE toPos );
	RC::eQUICK_RESULT		UnlinkByCode( SLOTIDX atIndex, SLOTCODE Code );

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    RC::eQUICK_RESULT       ChangeSkill( SLOTCODE old_skill_code, SLOTCODE new_skill_code );
#endif
    // TODO f100825.3L, need a validation test whether a quick slot has a correctly link.
    // maybe I think a below method's name which need change to UpdateValidateLink()
    void UpdateOriginalItems();
    // 대상 퀵슬롯의 연결된 아이템 정보를 갱신한다.
    void UpdateLinkedItem(const POSTYPE target_quick_slot_position, 
                          const SLOTIDX new_linked_item_container_type, 
                          const POSTYPE new_linked_item_position);
private:
    // CHANGES f100825.2L, remove an unused method
	//VOID _IteratingOriginalItem( SCSlotContainer * pContainer, SCQuickSlot & rQuickSlot, SLOTCODE ItemCode );

	Player *				m_pPlayer;
	SlotManager *			m_pSlotMGR;
	SCSlotContainer **		m_ppSlotContainer;

};


// stream관련 template 구현부
#include "QuickManager_Stream.hpp"
