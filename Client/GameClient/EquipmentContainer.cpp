#include "SunClientPrecompiledHeader.h"
#include "equipmentcontainer.h"
#include "Player.h"
#include "ObjectManager.h"
#include "SlotKeyGenerator.h"
#include "ItemUnitRenderer.h"
#include "StringTableInfo.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "SCItemSlot.h"
#include "Hero.h"
#include "Interface/uiToolTipMan/uiToolTipMan.h"

WzID EquipmentContainer::m_wzId[EQUIP_MAX] = 
{ 
	StrToWzID("B003"),	//EQUIP_WEAPON1,	//eEQUIPCONTAINER_WEAPON = 0,
	StrToWzID("B013"),	//EQUIP_ARMOR,		//eEQUIPCONTAINER_ARMOR,
	StrToWzID("B015"),	//EQUIP_PROTECTOR,	//eEQUIPCONTAINER_PROTECTOR,
	StrToWzID("B001"),  //EQUIP_HELMET,		//eEQUIPCONTAINER_HELMET,
	StrToWzID("B005"),	//EQUIP_PANTS,		//eEQUIPCONTAINER_PANTS,
	StrToWzID("B006"),	//EQUIP_BOOTS,		//eEQUIPCONTAINER_BOOTS,
	StrToWzID("B012"),	//EQUIP_GLOVE,		//eEQUIPCONTAINER_GLOVE,
	StrToWzID("B004"),	//EQUIP_BELT,		//eEQUIPCONTAINER_BELT,
	StrToWzID("B002"),	//EQUIP_SHIRTS,		//eEQUIPCONTAINER_SHIRTS,
	StrToWzID("B008"),	//EQUIP_RING1,		//eEQUIPCONTAINER_RING1,
	StrToWzID("B009"),	//EQUIP_RING2,		//eEQUIPCONTAINER_RING2,
	StrToWzID("B011"),	//EQUIP_NECKLACE,	//eEQUIPCONTAINER_NECKLACE,
	StrToWzID("0000"),	//EQUIP_BOW,		//eEQUIPCONTAINER_BOW, // 안쓰지만 순서 맞춰서 넣어두자.
	StrToWzID("B018"),	//EQUIP_SACCESSORY1,//eEQUIPCONTAINER_SACCESSORY1,
	StrToWzID("B017"),	//EQUIP_SACCESSORY2,//eEQUIPCONTAINER_SACCESSORY2,
	StrToWzID("B016"),	//EQUIP_SACCESSORY3,//eEQUIPCONTAINER_SACCESSORY3,
	StrToWzID("B019"),	//EQUIP_CHARGE1,
	StrToWzID("B020"),	//EQUIP_CHARGE2,
	StrToWzID("B021"),	//EQUIP_CHARGE3,
	StrToWzID("BT00"),	//EQUIP_CHARGE4
	StrToWzID("BT01"),  //EQUIP_CHARGE5
#ifdef _GS_GSP_REMOVE_PCROOM_SLOT
	//no operation
#else
	StrToWzID("BT02"),	//EQUIP_PC_ROOM1
	StrToWzID("BT03"),	//EQUIP_PC_ROOM2
	StrToWzID("B010"),	//EQUIP_PC_ROOM3
#endif//_GS_GSP_REMOVE_PCROOM_SLOT
#ifdef  _NA_000000_20130114_RENEWER_UI
	StrToWzID("PI02"),	//EQUIP_PIC_CHARGE4	
	StrToWzID("P003"),	//EQUIP_PIC_CHARGE5
#else
	StrToWzID("PI01"),	//EQUIP_PIC_CHARGE4	
	StrToWzID("PI02"),	//EQUIP_PIC_CHARGE5
#endif

#ifdef  _NA_000000_20130114_RENEWER_UI
	StrToWzID("P004"),  //EQUIP_SCORE_BG,
	StrToWzID("P005"),  //EQUIP_SCORE_NUMBER1,
	StrToWzID("P006"),  //EQUIP_SCORE_NUMBER2,
	StrToWzID("P007"),  //EQUIP_SCORE_NUMBER3,
	StrToWzID("P008"),  //EQUIP_SCORE_NUMBER4,
	StrToWzID("P009"),  //EQUIP_SCORE_NUMBER5,
#else
#ifdef _NA_002935_20110704_ITEM_SCORE
    StrToWzID("P001"),  //EQUIP_SCORE_BG,
    StrToWzID("P002"),  //EQUIP_SCORE_NUMBER1,
    StrToWzID("P007"),  //EQUIP_SCORE_NUMBER2,
    StrToWzID("P008"),  //EQUIP_SCORE_NUMBER3,
    StrToWzID("P009"),  //EQUIP_SCORE_NUMBER4,
    StrToWzID("P010"),  //EQUIP_SCORE_NUMBER5,
#endif //_NA_002935_20110704_ITEM_SCORE
#endif

	StrToWzID("B999"),	
	StrToWzID("PI00"),

	StrToWzID("T999"),	//EQUIP_TXT_TITLE,

#ifdef  _NA_000000_20130114_RENEWER_UI
	StrToWzID("C000"),
	StrToWzID("P010"),//EQUIP_HIDE_HELMET_P010
#endif

};						

EquipmentContainer::EquipmentContainer()
{
	m_TabNum = 0;
	m_dwOwnerKey = 0;
	Init(MAX_EQUIPMENT_SLOT_NUM, SI_EQUIPMENT);

	m_UseTooltipCtrls.clear();
	m_iTooltipStringCode = 0;
	
	m_UseTooltipCtrls[EQUIP_WEAPON1] = eST_WEAPON_STRING_INDEX;
	m_UseTooltipCtrls[EQUIP_ARMOR] = eST_ARMOR_STRING_INDEX;
	m_UseTooltipCtrls[EQUIP_PROTECTOR] = eST_PROTECTOR_STRING_INDEX;
	m_UseTooltipCtrls[EQUIP_HELMET] = eST_HELMET_STRING_INDEX;
	m_UseTooltipCtrls[EQUIP_PANTS] = eST_PANTS_STRING_INDEX;
	m_UseTooltipCtrls[EQUIP_BOOTS] = eST_BOOTS_STRING_INDEX;
	m_UseTooltipCtrls[EQUIP_GLOVE] = eST_GLOVE_STRING_INDEX;
	m_UseTooltipCtrls[EQUIP_BELT] = eST_BELT_STRING_INDEX;
	m_UseTooltipCtrls[EQUIP_SHIRTS] = eST_SHIRTS_STRING_INDEX;
	m_UseTooltipCtrls[EQUIP_RING1] = eST_RING_STRING_INDEX;
	m_UseTooltipCtrls[EQUIP_RING2] = eST_RING_STRING_INDEX;
	m_UseTooltipCtrls[EQUIP_NECKLACE] = eST_NECKLACE_STRING_INDEX;
	m_UseTooltipCtrls[EQUIP_SACCESSORY1] = 40017;	// 특수 악세사리
	m_UseTooltipCtrls[EQUIP_SACCESSORY2] = 40017;
	m_UseTooltipCtrls[EQUIP_SACCESSORY3] = 40017;
	m_UseTooltipCtrls[EQUIP_CHARGE1] = 40022;	// 특수 악세사리
	m_UseTooltipCtrls[EQUIP_CHARGE2] = 40022;
	m_UseTooltipCtrls[EQUIP_CHARGE3] = 40022;
	m_UseTooltipCtrls[EQUIP_CHARGE4] = 40022;
	m_UseTooltipCtrls[EQUIP_CHARGE5] = 40022;
#ifdef _GS_GSP_REMOVE_PCROOM_SLOT
	//no operation
#else
	m_UseTooltipCtrls[EQUIP_PC_ROOM1] = 40027;	//PC방 전용 아이템
	m_UseTooltipCtrls[EQUIP_PC_ROOM2] = 40027;
	m_UseTooltipCtrls[EQUIP_PC_ROOM3] = 40027;
#endif//_GS_GSP_REMOVE_PCROOM_SLOT
}

EquipmentContainer::~EquipmentContainer()
{
	m_UseTooltipCtrls.clear();
	Release();
}

//-------------------------------------------------------------------------------------------
/**
*/
void
EquipmentContainer::Release()
{
	for ( POSTYPE pos = 0; pos < GetMaxSlotNum(); ++pos )
	{
		JustDeleteSlot(pos, NULL);
	}
}

//------------------------------------------------------------------------------
int	EquipmentContainer::getTooltipString(POSTYPE pos)
{
	USE_TOOLTIP_CTRL_EQUIP_WINDOW::iterator iter;

	iter = m_UseTooltipCtrls.find(pos);
	
	if (iter != m_UseTooltipCtrls.end())
	{
		return iter->second;

	}
	else
	{
		return 0;
	}
}


//------------------------------------------------------------------------------
POSTYPE EquipmentContainer::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < EQUIP_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
			return i;
	}

	return EQUIP_MAX;
}
//------------------------------------------------------------------------------
/** 파츠인덱스를 통해 이큅슬롯 인덱스 얻기
*/
int EquipmentContainer::GetEquipPosByParts(POSTYPE parts_position)
{
    switch (parts_position)
    {
        //특수악세사리0
    case BP_SPECIAL_ACCESSORY1:
        return EQUIP_SACCESSORY1;
        //특수악세사리1
    case BP_SPECIAL_ACCESSORY2:				
        return EQUIP_SACCESSORY2;
        //특수악세사리2
    case BP_SPECIAL_ACCESSORY3:	
        return EQUIP_SACCESSORY3;
        //무기
    case BP_WEAPON:
        return EQUIP_WEAPON1;
        //부츠
    case BP_BOOTS:
        return EQUIP_BOOTS;
        //바지
    case BP_PANTS:
        return EQUIP_PANTS;
        //가슴파츠
    case BP_ARMOR:
        return EQUIP_ARMOR;
        //투구
    case BP_HELM:
        return EQUIP_HELMET;
        //장갑
    case BP_GLOVE:
        return EQUIP_GLOVE;
    default:
        return EQUIP_MAX;
    }
}
//------------------------------------------------------------------------------
/** 단순 슬롯삽입
*/
void EquipmentContainer::JustInsertSlot(POSTYPE insert_position, SCSlot& slot_info)
{
    //slot_safe_code:안전한 슬롯체크 구문(삽입)
    if (ValidPos(insert_position) == false)
    {
        return;
    }

    // 추가
    RC::eSLOT_INSERT_RESULT slot_result = SCSlotContainer::InsertSlot(insert_position, slot_info);

    // 시리얼 발급 설정
    SCSlot& insert_slot = GetSlot(insert_position);

    DBSERIAL item_serial = g_SlotKeyGenerator.GetKey();
    insert_slot.SetSerial(item_serial);


    // 플레이어의 처리

    // 클라에서 따로 시리얼을 발급한다.
    Player* player_ptr = g_ObjectManager.GetPlayer(m_dwOwnerKey);
    if (player_ptr == NULL)
    {
        return;
    }

    //slot_safe_code:실제삽입 성공시에만 데이터처리
    if (slot_result != RC::RC_INSERT_SLOT_SUCCESS)
    {
        return;
    }

    player_ptr->SetRealEquipFlag(insert_position, true);


    if( player_ptr->IsMyHero() )
        g_pHero->ItemAttrCalculatorByItemslot(insert_slot, true, insert_position);


    if (insert_position == eEQUIPCONTAINER_WEAPON)
    {
        player_ptr->SetWeaponKind(player_ptr->GetWeaponKind());
    }

    player_ptr->OnUseItem(insert_slot.GetCode());
}

//------------------------------------------------------------------------------
/** 캐쉬 + PC_ROOM 슬롯 셋팅
*/
void EquipmentContainer::SetDeleteChargeSlot(Character* character_ptr, POSTYPE delete_position)
{
    if( delete_position==eEQUIPCONTAINER_CHARGE1||
        delete_position==eEQUIPCONTAINER_CHARGE2||
        delete_position==eEQUIPCONTAINER_CHARGE4||
        delete_position==eEQUIPCONTAINER_CHARGE5||
        delete_position==eEQUIPCONTAINER_CHARGE3)
    {
        if (IsEmpty(eEQUIPCONTAINER_CHARGE1) == FALSE)
        {
            SCSlot& slot_info = GetSlot(eEQUIPCONTAINER_CHARGE1);
            character_ptr->OnUseItem(slot_info.GetCode());
        }
        else if (IsEmpty(eEQUIPCONTAINER_CHARGE1) == FALSE)
        {
            SCSlot& slot_info = GetSlot(eEQUIPCONTAINER_CHARGE1);
            character_ptr->OnUseItem(slot_info.GetCode());
        }
        else if (IsEmpty(eEQUIPCONTAINER_CHARGE2) == FALSE)
        {
            SCSlot& slot_info = GetSlot(eEQUIPCONTAINER_CHARGE2);
            character_ptr->OnUseItem(slot_info.GetCode());
        }
        else if (IsEmpty(eEQUIPCONTAINER_CHARGE3) == FALSE)
        {
            SCSlot& slot_info = GetSlot(eEQUIPCONTAINER_CHARGE3);
            character_ptr->OnUseItem(slot_info.GetCode());
        }
        else if (IsEmpty(eEQUIPCONTAINER_CHARGE4) == FALSE)
        {
            SCSlot& slot_info = GetSlot(eEQUIPCONTAINER_CHARGE4);
            character_ptr->OnUseItem(slot_info.GetCode());
        }
        else if (IsEmpty(eEQUIPCONTAINER_CHARGE5) == FALSE)
        {
            SCSlot& slot_info = GetSlot(eEQUIPCONTAINER_CHARGE5);
            character_ptr->OnUseItem(slot_info.GetCode());
        }

    }

    if ((delete_position == eEQUIPCONTAINER_PC_ROOM1) ||
        (delete_position == eEQUIPCONTAINER_PC_ROOM2) ||
        (delete_position == eEQUIPCONTAINER_PC_ROOM3))
    {
        if (IsEmpty(eEQUIPCONTAINER_PC_ROOM1) == FALSE)
        {
            SCSlot& slot_info = GetSlot(eEQUIPCONTAINER_PC_ROOM1);
            character_ptr->OnUseItem(slot_info.GetCode());
        }
        else if(IsEmpty(eEQUIPCONTAINER_PC_ROOM2) == FALSE)
        {
            SCSlot& slot_info = GetSlot(eEQUIPCONTAINER_PC_ROOM2);
            character_ptr->OnUseItem(slot_info.GetCode());
        }
        else if(IsEmpty(eEQUIPCONTAINER_PC_ROOM3) == FALSE)
        {
            SCSlot& slot_info = GetSlot(eEQUIPCONTAINER_PC_ROOM3);
            character_ptr->OnUseItem(slot_info.GetCode());
        }
    }
}

//------------------------------------------------------------------------------
/** 단순 슬롯 제거
*/
bool EquipmentContainer::JustDeleteSlot(POSTYPE delete_position, SCSlot* slot_info_ptr)
{
    //slot_safe_code:안전한 슬롯체크 구문(삭제)
    if ((ValidPos(delete_position) == false) ||
        IsEmpty(delete_position))
    {
        return false;
    }

    // render unit 를 먼저 지운다.
    SCSlot& current_slot = GetSlot(delete_position);

    // 시리얼 회수
    g_SlotKeyGenerator.Restore((DWORD)current_slot.GetSerial());

    // 제거
    SCItemSlot temp_slot;
    if (SCSlotContainer::DeleteSlot(delete_position, &temp_slot) == FALSE) 
    {
        return false;
    }


    // 플레이어의 처리
    Player* player_ptr = g_ObjectManager.GetPlayer(m_dwOwnerKey);
    if (player_ptr == NULL)
    {
        return false;
    }

    player_ptr->OnUnUseItem(current_slot.GetCode());

    if (slot_info_ptr) 
    {
        static_cast<SCItemSlot*>(slot_info_ptr)->Copy(temp_slot);
    }

    if (player_ptr->GetRealEquipFlag(delete_position))
    {
        player_ptr->SetRealEquipFlag(delete_position, false);

        if( player_ptr->IsMyHero() )
            g_pHero->ItemAttrCalculatorByItemslot(temp_slot, false, delete_position);
    }
    
    if (delete_position == eEQUIPCONTAINER_WEAPON)
    {
        player_ptr->SetWeaponKind(player_ptr->GetWeaponKind());
        player_ptr->DestoryEtherWeaponEffect();
    }

    //charge+pcroom 슬롯 OnUse셋팅
    SetDeleteChargeSlot(player_ptr, delete_position);

    return true;
}

RC::eSLOT_INSERT_RESULT EquipmentContainer::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
    //slot_safe_code:안전한 슬롯체크 구문(삽입)
    if (ValidPos(AtPos) == false)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }


    // 추가
    RC::eSLOT_INSERT_RESULT slot_result = SCSlotContainer::InsertSlot( AtPos, rSlot );

    //slot_safe_code:실제삽입 성공시에만 데이터처리
    if (slot_result != RC::RC_INSERT_SLOT_SUCCESS)
    {
        return slot_result;
    }

    // 시리얼 발급 / 설정
    SCSlot & rSetSlot = GetSlot(AtPos);
    DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
    rSetSlot.SetSerial(dwSerial);


	// 클라에서 따로 시리얼을 발급한다.
	Player* player_ptr = g_ObjectManager.GetPlayer(m_dwOwnerKey);
	if (player_ptr == NULL)
    {
		return RC::RC_INSERT_SLOT_FAILED;
    }

    player_ptr->SetRealEquipFlag(AtPos, true);

    if( player_ptr->IsMyHero() )
        g_pHero->ItemAttrCalculatorByItemslot(rSetSlot, true, AtPos);

	if( AtPos == eEQUIPCONTAINER_WEAPON )
	{
		player_ptr->SetWeaponKind(player_ptr->GetWeaponKind());
	}
    player_ptr->UpdateSkillAttr();

    //용변신 & 외형과 상관없는슬롯위치에서는 갱신안함
    if ((player_ptr->IsTransForm() == false) &&
        (player_ptr->GetTransformData().GetTransFormProcessType() == TRANSFORM_PROCESS_NONE))
    {
        player_ptr->RefreshAppearRender();
    }
		
	player_ptr->OnUseItem(rSetSlot.GetCode());

	if(player_ptr->IsEtherDeviecBulletChargeing())
	{
		player_ptr->CreateEtherWeaponEffect(0,TRUE);
	}

    return slot_result;
}

BOOL EquipmentContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
    //safe_code:비어있거나 유효한위치가 아니라면 리턴한다
    if ((ValidPos(AtPos) == false) ||
        IsEmpty(AtPos))
    {
        return FALSE;
    }

	// render unit 를 먼저 지운다.
	SCSlot & rSlot = GetSlot(AtPos);

    // 시리얼 회수
	g_SlotKeyGenerator.Restore((DWORD)rSlot.GetSerial());

    // 제거
    SCItemSlot TmpDeleteItemSlot;
    if( !SCSlotContainer::DeleteSlot( AtPos, &TmpDeleteItemSlot ) ) 
    {
        return FALSE;
    }

    if( pSlotOut ) 
    {
        ((SCItemSlot*)pSlotOut)->Copy(TmpDeleteItemSlot);
    }

    // 플레이어일 경우 추가 처리
	Player* pPlayer = g_ObjectManager.GetPlayer(m_dwOwnerKey);
	if( pPlayer == NULL )
	{
		return FALSE;
	}
	
	pPlayer->OnUnUseItem(rSlot.GetCode());


	if( pPlayer->GetRealEquipFlag(AtPos) )
	{
		pPlayer->SetRealEquipFlag(AtPos, false);

        if( pPlayer->IsMyHero() )
            g_pHero->ItemAttrCalculatorByItemslot(TmpDeleteItemSlot, false, AtPos);		
	}


	if( AtPos == eEQUIPCONTAINER_WEAPON )
	{
		pPlayer->SetWeaponKind(pPlayer->GetWeaponKind());
		pPlayer->DestoryEtherWeaponEffect();
	}

	pPlayer->UpdateItemAttrCalculator();
	pPlayer->UpdateSkillAttr();

    //용변신 & 외형과 상관없는슬롯위치에서는 갱신안함
    if ((pPlayer->IsTransForm() == false) &&
        (pPlayer->GetTransformData().GetTransFormProcessType() == TRANSFORM_PROCESS_NONE))
    {
       pPlayer->RefreshAppearRender();
    }

	SetDeleteChargeSlot(pPlayer, AtPos);

	return TRUE;

}

VOID EquipmentContainer::UpdateSlot( POSTYPE AtPos,SCSlot & rSlot )
{
	Player* pPlayer = g_ObjectManager.GetPlayer(m_dwOwnerKey);
	if (!pPlayer)
	{
		return;
	}

	// render unit 를 먼저 지운다.
	SCSlot & slot = GetSlot(AtPos);
	slot.Copy(rSlot);


	pPlayer->UpdateSkillAttr();
	pPlayer->RefreshAppearRender();
}

// 실제 슬롯정보관련한 위치
POSTYPE	EquipmentContainer::GetRealSlotPos( POSTYPE pos )
{
	return m_TabNum + pos;
}

// 인터페이스 컨트롤 위치.
POSTYPE	EquipmentContainer::GetRelativeSlotPos( POSTYPE pos )
{
	// 확실한 
	return ((pos + m_TabNum) % (EQUIP_SLOT_END+1));
}

void EquipmentContainer::SetOwnerKey(DWORD dwOwnerKey)
{
	m_dwOwnerKey = dwOwnerKey;
}
//------------------------------------------------------------------------------ 
bool EquipmentContainer::IsEquipItem(eITEMTYPE item_type, OUT SLOTCODE* item_code)
{
    for (int i = 0; i < EQUIP_MAX; ++i)
    {
        SCSlot& slot = GetSlot(i);
        if (slot.GetSlotType() != ST_ITEM)
        {
            continue;
        }

        SCItemSlot& item_slot = static_cast<SCItemSlot&>(slot);
        const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
        if ((item_info != NULL) && (item_info->m_wType == item_type))
        {
            if (item_code != NULL)
            {
                *item_code = item_slot.GetCode();
            }
            return true;
        }
    }

    return false;
}

SCItemSlotContainer* EquipmentContainer::GetEquipmentSlotContainer()
{
    return this;
}

ns_formula::EquipSlotsManageInfo* EquipmentContainer::GetEquipmentManageInfo()
{
    return &equip_slot_manage_info_;
}

bool EquipmentContainer::CheckEquipDefaultValidationWithoutStats(POSTYPE at_pos, const SCItemSlot& IN check_slot)
{
    if (check_slot.CanEquip() && equip_slot_manage_info_.CanEquipmentsDependency(check_slot))
    {
        return true;
    }
    else
    {
        return false;
    }    
}

bool EquipmentContainer::CanEquip(POSTYPE at_pos, const SCItemSlot& IN check_slot)
{
    if (CheckEquipDefaultValidationWithoutStats(at_pos, check_slot) && ValidLimitStats(check_slot))
    {
        return true;
    }
    return false;
}

bool EquipmentContainer::ValidLimitStats(const SCItemSlot& item_slot) const
{
    PlayerAttributes& player_attributes = *(g_pHero->GetPlayerAttribute());
    if (ItemAttrCalculator::CanEquipmentValidStats(item_slot, player_attributes, g_pHero->GetLevel()) == false)
    {
        return false;
    }
    return true;
}

bool EquipmentContainer::OnChangedEquipItemDurability(const SCItemSlot& item_slot)
{
    const SLOTIDX slot_index = item_slot.GetSlotIdx();
    if (FlowControl::FCAssert(slot_index == SI_EQUIPMENT) == false)
    {
        return false;
    }
    const POSTYPE pos = item_slot.GetPos();
    if (FlowControl::FCAssert(pos < MAX_EQUIPMENT_SLOT_NUM) == false)
    {
        return false;
    }

    bool pc_room_status = (g_pHero->GetPCBangMode() != FALSE);
    ItemAttrCalculator Item_calculator(*(g_pHero->GetPlayerAttribute()), this);
    bool processed = Item_calculator.ChangeItemDurabilityStatus(item_slot, pos, pc_room_status, false, g_pHero->GetLevel());
    return processed;
}


//------------------------------------------------------------------------------ 
SCItemSlot* EquipmentContainer::GetItemSlot(POSTYPE at_pos)
{
    SCSlot& slot = SCSlotContainer::GetSlot(at_pos);

    if (slot.GetSlotType() != ST_ITEM)
    {
        return NULL;
    }
    SCItemSlot* item_slot = static_cast<SCItemSlot*>(&slot);
    if (item_slot->IsContainValidItem())
    {
        return item_slot;
    }
    return NULL;
}

void EquipmentContainer::RenderTooltip( DWORD DialogKey )
{
	if(m_iTooltipStringCode)
	{
		uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
		if(pUIMan)
		{
			pUIMan->RegisterTooltipWithStringCode(m_iTooltipStringCode);
			pUIMan->SetTooltipDialogID(DialogKey);
		}
	}
}

#ifdef _NA_000000_20130114_RENEWER_UI
void EquipmentContainer::SetChangeBackgroundIsClass( eCHAR_TYPE charater_type )
{
	switch (charater_type)
	{
	case eCHAR_BERSERKER:
		{
			getControl(EQUIP_PICTURE_P004)->ShowWindowWZ(WZ_SHOW);
			getControl(EQUIP_PICTURE_P005)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P006)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P007)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P008)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P009)->ShowWindowWZ(WZ_HIDE);
		}
		break;
	case eCHAR_DRAGON:
		{
			getControl(EQUIP_PICTURE_P004)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P005)->ShowWindowWZ(WZ_SHOW);
			getControl(EQUIP_PICTURE_P006)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P007)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P008)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P009)->ShowWindowWZ(WZ_HIDE);
		}
		break;
	case eCHAR_ELEMENTALIST:
		{
			getControl(EQUIP_PICTURE_P004)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P005)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P006)->ShowWindowWZ(WZ_SHOW);
			getControl(EQUIP_PICTURE_P007)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P008)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P009)->ShowWindowWZ(WZ_HIDE);
		}
		break;
	case eCHAR_MYSTIC:
		{
			getControl(EQUIP_PICTURE_P004)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P005)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P006)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P007)->ShowWindowWZ(WZ_SHOW);
			getControl(EQUIP_PICTURE_P008)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P009)->ShowWindowWZ(WZ_HIDE);
		}
		break;
	case eCHAR_SHADOW:
		{
			getControl(EQUIP_PICTURE_P004)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P005)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P006)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P007)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P008)->ShowWindowWZ(WZ_SHOW);
			getControl(EQUIP_PICTURE_P009)->ShowWindowWZ(WZ_HIDE);
		}
		break;
	case eCHAR_VALKYRIE:
		{
			getControl(EQUIP_PICTURE_P004)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P005)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P006)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P007)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P008)->ShowWindowWZ(WZ_HIDE);
			getControl(EQUIP_PICTURE_P009)->ShowWindowWZ(WZ_SHOW);
		}
		break;
	}
}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI