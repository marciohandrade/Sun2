//=======================================================================================================================
/** EquipmentContainer
	@author
			유재영
	@since
			
	@remarks
*/


#ifndef __EQUIPMENTCONTAINER_H__
#define __EQUIPMENTCONTAINER_H__

#pragma once

#include <SCItemSlotContainer.h>

typedef map<int,int> USE_TOOLTIP_CTRL_EQUIP_WINDOW;

class CControlWZ;

#include "./ItemAttrCalculatorEP2.h"

class EquipmentContainer : public SCItemSlotContainer, public IEquipmentRestriction
{
public:
	//////////////////////////////////////////////////////////////////////////
	enum eEQUIP_POS
	{
		EQUIP_WEAPON1 = 0,	//eEQUIPCONTAINER_WEAPON = 0,	
		EQUIP_ARMOR,		//eEQUIPCONTAINER_ARMOR,		
		EQUIP_PROTECTOR,	//eEQUIPCONTAINER_PROTECTOR,	
		EQUIP_HELMET,		//eEQUIPCONTAINER_HELMET,		
		EQUIP_PANTS,		//eEQUIPCONTAINER_PANTS,		
		EQUIP_BOOTS,		//eEQUIPCONTAINER_BOOTS,		
		EQUIP_GLOVE,		//eEQUIPCONTAINER_GLOVE,		
		EQUIP_BELT,			//eEQUIPCONTAINER_BELT,			
		EQUIP_SHIRTS,		//eEQUIPCONTAINER_SHIRTS,		
		EQUIP_RING1,		//eEQUIPCONTAINER_RING1,		
		EQUIP_RING2,		//eEQUIPCONTAINER_RING2,		
		EQUIP_NECKLACE,		//eEQUIPCONTAINER_NECKLACE,		
		EQUIP_BOW,			//eEQUIPCONTAINER_BOW, // 안쓰지만 순서 맞춰서 넣어두자. 
		EQUIP_SACCESSORY1,	//eEQUIPCONTAINER_SACCESSORY1,
		EQUIP_SACCESSORY2,  //eEQUIPCONTAINER_SACCESSORY2,
		EQUIP_SACCESSORY3,	//eEQUIPCONTAINER_SACCESSORY3,
		EQUIP_CHARGE1,		//eEQUIPCONTAINER_CHARGE1,
		EQUIP_CHARGE2,		//eEQUIPCONTAINER_CHARGE2,
		EQUIP_CHARGE3,		//eEQUIPCONTAINER_CHARGE3,
		EQUIP_CHARGE4,		//eEQUIPCONTAINER_CHARGE4
		EQUIP_CHARGE5,		//eEQUIPCONTAINER_CHARGE5
#ifdef _GS_GSP_REMOVE_PCROOM_SLOT
		//no operation
#else
		EQUIP_PC_ROOM1,		//eEQUIPCONTAINER_PC_ROOM1
		EQUIP_PC_ROOM2,		//eEQUIPCONTAINER_PC_ROOM2
		EQUIP_PC_ROOM3,		//eEQUIPCONTAINER_PC_ROOM3
#endif//_GS_GSP_REMOVE_PCROOM_SLOT
		EQUIP_PIC_CHARGE4,
		EQUIP_PIC_CHARGE5,

#ifdef  _NA_000000_20130114_RENEWER_UI
		EQUIP_PICTURE_P004,
		EQUIP_PICTURE_P005,
		EQUIP_PICTURE_P006,
		EQUIP_PICTURE_P007,
		EQUIP_PICTURE_P008,
		EQUIP_PICTURE_P009,
#else
#ifdef _NA_002935_20110704_ITEM_SCORE
        EQUIP_SCORE_BG,
        EQUIP_SCORE_NUMBER1,
        EQUIP_SCORE_NUMBER2,
        EQUIP_SCORE_NUMBER3,
        EQUIP_SCORE_NUMBER4,
        EQUIP_SCORE_NUMBER5,
#endif //_NA_002935_20110704_ITEM_SCORE
#endif
		EQUIP_EXIT,			
		EQUIP_CHARACTER_VIEW,

		EQUIP_TXT_TITLE,

#ifdef  _NA_000000_20130114_RENEWER_UI
		EQUIP_HIDE_HELMET,
		EQUIP_HIDE_HELMET_P010,
#endif
							
		EQUIP_MAX,					
	};	
	//////////////////////////////////////////////////////////////////////////

	enum 
	{ 
		EQUIP_SLOT_START = EQUIP_WEAPON1,
#ifdef _GS_GSP_REMOVE_PCROOM_SLOT
		EQUIP_SLOT_END = EQUIP_CHARGE5 +1,
#else
		EQUIP_SLOT_END = EQUIP_PC_ROOM3 +1,
#endif//_GS_GSP_REMOVE_PCROOM_SLOT

	};
#ifdef  _NA_000000_20130114_RENEWER_UI
#else
#ifdef _NA_002935_20110704_ITEM_SCORE
    enum
    { 
        EQUIP_SCORE_START = EQUIP_SCORE_BG,
        EQUIP_SCORE_END   = EQUIP_SCORE_NUMBER5 +1,
    };
#endif //_NA_002935_20110704_ITEM_SCORE
#endif

public:
	EquipmentContainer();
	virtual ~EquipmentContainer();

	void					Release();

	// HERO를 제외한 나머지 사람의 장착 아이템은 Serial(클라이언트 자체 발급), Pos, Code를 제외한 나머지 정보는 필요없다!!!
	virtual VOID						UpdateSlot( POSTYPE AtPos,SCSlot & rSlot  );

	virtual RC::eSLOT_INSERT_RESULT		InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );	//< 슬롯에 객체 추가
	virtual BOOL						DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );		// 슬롯의 객체 삭제

    // 단순 슬롯 삽입
    virtual void JustInsertSlot(POSTYPE insert_position, SCSlot& slot_info);
    // 단순 슬롯 제거
    virtual bool JustDeleteSlot(POSTYPE delete_position, SCSlot* slot_info_ptr);

    virtual void SetDeleteChargeSlot(Character* character_ptr, POSTYPE delete_position);

    //파츠인덱스를 통해 이큅위치얻기
    int GetEquipPosByParts(POSTYPE parts_position);

    SCItemSlot* GetItemSlot(POSTYPE at_pos);

    //------------------------------------------------------------------------------
    //! Returns:   bool
    //! Qualifier: 아이템 타입으로 해당 아이템이 창착되어 있는지 확인
    //! Parameter: eITEMTYPE item_type
    //! Parameter: OUT SLOTCODE* item_code : 장착되어 있는 아이템 코드 
    //------------------------------------------------------------------------------
    bool IsEquipItem(eITEMTYPE item_type, OUT SLOTCODE* item_code);

    void SetOwnerKey(DWORD dwKey);

    // 실제 슬롯정보관련한 위치
	POSTYPE					GetRealSlotPos( POSTYPE pos );
	// 인터페이스 컨트롤 위치.
	POSTYPE					GetRelativeSlotPos( POSTYPE pos );

	void					RenderTooltip( DWORD DialogKey );

	USE_TOOLTIP_CTRL_EQUIP_WINDOW	m_UseTooltipCtrls;
	int				m_iTooltipStringCode;
	int				getTooltipString(POSTYPE pos);

    virtual SCItemSlotContainer* GetEquipmentSlotContainer();
    virtual ns_formula::EquipSlotsManageInfo* GetEquipmentManageInfo();
    virtual bool CheckEquipDefaultValidationWithoutStats(POSTYPE at_pot, const SCItemSlot& IN check_slot);
    bool CanEquip(POSTYPE at_pos, const SCItemSlot& IN check_slot);
    bool ValidLimitStats(const SCItemSlot& item_slot) const;
    bool OnChangedEquipItemDurability(const SCItemSlot& item_slot);

#ifdef _NA_000000_20130114_RENEWER_UI
	void SetChangeBackgroundIsClass(eCHAR_TYPE charater_type);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
protected:
	virtual CControlWZ *	getControl( POSTYPE /*AtPos*/ ){ return NULL; }
	POSTYPE					getCtrlIDToPos( WzID wzId );

	POSTYPE					m_TabNum;

	static WzID				m_wzId[EQUIP_MAX];

	DWORD                   m_dwOwnerKey;
	
    ns_formula::EquipSlotsManageInfo equip_slot_manage_info_;
};

#endif // __EQUIPMENTCONTAINER_H__