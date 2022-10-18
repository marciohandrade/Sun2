#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "ProhibitionWordParser.h"

#include "InventorySlotContainer.h"

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionPet) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionPet::UsePetItem(POSTYPE ItemPos)
{
    SCSlotContainer * pContainer = root_->GetItemSlotContainer(SI_INVENTORY);

    if (!root_->ValidState()) 
        return RC::RC_ITEM_INVALIDSTATE;

    if (!root_->ValidPos(SI_INVENTORY, ItemPos))
        return RC::RC_ITEM_INVALIDPOS;  

    SCItemSlot & rPetItemSlot = (SCItemSlot &)pContainer->GetSlot(ItemPos);
    if (!rPetItemSlot.GetItemInfo())
        return RC::RC_ITEM_NOINFO;

    //펫 아이템인가?
    if (!rPetItemSlot.IsPetItem() || (rPetItemSlot.GetNum() == 0))
        return RC::RC_ITEM_INVALID_TYPE;

    RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(rPetItemSlot.GetItemInfo());
    if (rcResult != RC::RC_ITEM_SUCCESS)
        return rcResult;

    RC::ePET_SUMMON_RESULT rt = root_->player_->m_PetManager.UsePetItem(rPetItemSlot);  
    switch (rt)
    {
    case RC::RC_PET_SUMMON_SUMMON_SUCCESS:
        GAMELOG->LogItem(ITEM_USE, root_->player_, &rPetItemSlot);
        return RC::RC_ITEM_SUMMON_SUMMON_SUCCESS;
        break;
    case RC::RC_PET_SUMMON_RETURN_SUCCESS:
        return RC::RC_ITEM_SUMMON_RETURN_SUCCESS;
        break;  
    case RC::RC_PET_SUMMON_ALREADY_SUMMONED:
        return RC::RC_ITEM_SUMMON_ALREADY_SUMMONED;
        break;
    case RC::RC_PET_SUMMON_NOT_PET_ITEM:
        return RC::RC_ITEM_SUMMON_NOT_PET_ITEM;
        break;
    case RC::RC_PET_SUMMON_NOT_SUMMONED_PET:
        return RC::RC_ITEM_SUMMON_NOT_SUMMONED_PET;
        break;
    case RC::RC_PET_SUMMON_MISMATCH_PET_ITEM:
        return RC::RC_ITEM_SUMMON_MISMATCH_PET_ITEM;
        break;
    case RC::RC_PET_CAN_NOT_SUMMON_FULLNESS_ZERO:
        return RC::RC_ITEM_CAN_NOT_SUMMON_FULLNESS_ZERO;
    default:
        return RC::RC_ITEM_FAILED;
    }
}

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
//Use패킷
RC::eITEM_RESULT nsSlot::ItemFunctionPet::UsePetFeed(SCItemSlot& rItemSlot)
{
    if (!root_->player_->m_PetManager.AlreadySummonedPet())
        return RC::RC_ITEM_NOT_SUMMONED_PET;

    const BASE_ITEMINFO* const pInfo = rItemSlot.GetItemInfo();
    if (!pInfo)
        return RC::RC_ITEM_FAILED;

    root_->player_->m_PetManager.IncreaseFullness(pInfo->m_wMissile);
    root_->player_->m_PetManager.IncreaseIntimacy((BYTE)pInfo->m_MaterialCode); 

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionPet::UseChangePetNameCard(
    POSTYPE ItemPos, const util::StrArrayForPetName& pet_name)
{
    if (!root_->player_->m_PetManager.AlreadySummonedPet())
        return RC::RC_ITEM_NOT_SUMMONED_PET;

    SCSlotContainer * pContainer = root_->GetItemSlotContainer(SI_INVENTORY);
    if (!root_->ValidState()) 
        return RC::RC_ITEM_INVALIDSTATE;

    if (!root_->ValidPos(SI_INVENTORY, ItemPos))
        return RC::RC_ITEM_INVALIDPOS;

    SCItemSlot & rPetNameItemSlot = (SCItemSlot &)pContainer->GetSlot(ItemPos);
    if (!rPetNameItemSlot.GetItemInfo())
        return RC::RC_ITEM_NOINFO;

    //쿨타임 체크
    RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(rPetNameItemSlot.GetItemInfo());
    if (RC::RC_ITEM_SUCCESS != rcResult)
        return rcResult;

    //펫 이름 설정 아이템인가?
    if (rPetNameItemSlot.GetItemInfo()->m_wType != eITEMTYPE_PET_NAME_CARD ||
        (rPetNameItemSlot.GetNum() == 0))
    {
        return RC::RC_ITEM_INVALID_TYPE;
    };
    if (strlen(pet_name) > 0)
    {
        ProhibitionWordParser* const banned_word_filter = ProhibitionWordParser::Instance();
        util::cSTRING_REF strPetName(pet_name, MAX_PET_NAME_LENGTH);
        if (banned_word_filter->CheckFilter(pet_name) == false ||
            banned_word_filter->CheckFilterName(pet_name) == false)
        {
            return RC::RC_ITEM_CANNOT_USE_PET_NAME;
        }
    }

    root_->player_->m_PetManager.SetPetName(pet_name);

    WORD wCoolTimeType = rPetNameItemSlot.GetItemInfo()->m_wCoolTimeType;
    root_->player_->ResetUseItem(wCoolTimeType);

    //로그
    const TCHAR* addInfo = NULL;
    SCItemSlot* petItemSlot = root_->player_->m_PetManager.GetUsedPetSlot();
    if (petItemSlot != NULL) {
        TCHAR buf[MAX_ADDINFO_SIZE];
        _sntprintf(buf, MAX_ADDINFO_SIZE, "PetItemCode:%u,PetItemSerial:%u", \
            petItemSlot->GetItemInfo()->m_Code, petItemSlot->GetWorldSerial());
        buf[MAX_ADDINFO_SIZE - 1] = '\0';
        addInfo = buf;
    }
    GAMELOG->LogItem(ITEM_USE, root_->player_, &rPetNameItemSlot, 0, 1, NULL, NULL, addInfo);

    //(유료 아이템) 삭제
    root_->Lose(pContainer, ItemPos, 1);

    return RC::RC_ITEM_SUCCESS;
}

