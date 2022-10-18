#include "stdafx.h"
#include "CPetManager.h"

//==================================================================================================
#include <SCItemSlot.h>
#include <PetInfoParser.h>
#include <SkillAttrCalculator.h>

//==================================================================================================
#include "GameZone.h"
#include "ItemManager.h"

#include "StatusManager.h"
#include "RecoverStatus.h"

//==================================================================================================
//  @history
//  -   091003|waverix|code arrangement
//                    |remove '__BUG_2536_RECOVER_HP_BUG'
//                    |remove '__NA_1131_PET_NAME_SETTING'
//                    |remove '__NA_0_PET_TIMER_PERIOD_CHANGE'

CPetManager::CPetManager()
{
}

CPetManager::~CPetManager()
{
}

void
CPetManager::Init(Player* pPlayer)
{
    m_pPlayer       = pPlayer;
    m_SummonedPet   = FALSE;
    m_PetItemPos    = 0;
    m_PetItemSerial = 0;

    m_pPetItemSlot  = NULL;
    m_pPetInfo      = NULL;
    m_pImagePetInfo = NULL;

    m_bAppliedBasicOption = FALSE;
    m_bAppliedAddOption   = FALSE;
    m_AppliedOptionLV     = MAX_PET_OPTION_EXTENT_COUNT;
    m_bMAXIntimacy        = FALSE;

    m_FiveMinTimer.DisableCheckTime();
    m_TenMinTimer.DisableCheckTime();
    m_MaxIntimacyTimer.DisableCheckTime();
}

void
CPetManager::Clear()
{
    m_SummonedPet   = FALSE;
    m_PetItemPos    = 0;
    m_PetItemSerial = 0;

    m_pPetItemSlot  = NULL;
    m_pPetInfo      = NULL;
    m_pImagePetInfo = NULL;

    m_bAppliedBasicOption = FALSE;
    m_bAppliedAddOption   = FALSE;
    m_AppliedOptionLV     = MAX_PET_OPTION_EXTENT_COUNT;
    m_bMAXIntimacy        = FALSE;

    BASE_PLAYERINFO* player_info = m_pPlayer->GetCharInfo();
    player_info->m_bSummonedPet = m_SummonedPet;
    player_info->m_bMAXIntimacy = m_bMAXIntimacy;
    player_info->m_PetItemPos   = m_PetItemPos;
    player_info->m_bMAXOptionRemainTime = m_MaxIntimacyTimer.GetRemainedTime();
}

//==================================================================================================

RC::ePET_SUMMON_RESULT
CPetManager::UsePetItem(SCItemSlot& rPetItemSlot)
{
    // (BUGFIX) (091007) (WAVERIX) 2nd level interface, occurred change status need pre-check 'IsPet'
    if(rPetItemSlot.IsPetItem() == 0)
        return RC::RC_PET_SUMMON_NOT_PET_ITEM;

    if(m_SummonedPet == FALSE && rPetItemSlot.IsPetLocked())
        rPetItemSlot.SetUserBlocked(FALSE);

    if(rPetItemSlot.IsPetLocked()) {
        // Return Pet
        return ReturnPet(&rPetItemSlot);
    }
    else {
        // Summon Pet
        return SummonPet(&rPetItemSlot);
    }
}

RC::ePET_SUMMON_RESULT
CPetManager::SummonPet(SCItemSlot* pPetItemSlot)
{
    if(m_SummonedPet)
        return RC::RC_PET_SUMMON_ALREADY_SUMMONED;
    // (CHANGES) (f100519.2L) changes condition check ordering
    if (!m_pPlayer) {
        return RC::RC_PET_SUMMON_RETURN_FAILED;
    }
    if (!pPetItemSlot->IsPetItem()) {
        return RC::RC_PET_SUMMON_NOT_PET_ITEM;
    }
    if (pPetItemSlot->CanSummonStatus() == false) {
        return RC::RC_PET_SUMMON_NOT_PET_ITEM;
    }

    GameZone *pZone = m_pPlayer->GetGameZonePtr();
    if(!pZone)  
        return RC::RC_PET_SUMMON_RETURN_FAILED;

    if(pZone->GetZoneType() != eZONETYPE_VILLAGE)
    {
        if(pPetItemSlot->GetPetInfo()->GetFullness() <= 0)
            return RC::RC_PET_CAN_NOT_SUMMON_FULLNESS_ZERO;
    }

#ifdef _NA_007854_20141127_MODIFY_PET_CHARGE_SUBTYPE
    if (pPetItemSlot->GetItemRuleInfo().charge_sub_type == eCHARGE_SUB_FIXED_QUANTITY)
    {
        if(!pPetItemSlot->StartQuantity())
            return RC::RC_PET_EXPIRED_CHARGE_ITEM;
    }
#else
    if(!pPetItemSlot->StartQuantity())
        return RC::RC_PET_EXPIRED_CHARGE_ITEM;
#endif //_NA_007854_20141127_MODIFY_PET_CHARGE_SUBTYPE
    

    pPetItemSlot->SetUserBlocked(TRUE); 
    m_SummonedPet   = TRUE; 
    m_PetItemSerial = pPetItemSlot->GetSerial();
    m_PetItemPos    = pPetItemSlot->GetPos();
    m_pPetItemSlot  = pPetItemSlot; 
    // (CHANGES) (f100524.2L) updates an pet image control to support an etheria
    PetInfoParser* const pet_parser = PetInfoParser::Instance();
    const BASE_ITEMINFO* item_info = m_pPetItemSlot->GetItemInfo();
    const BASE_ITEMINFO* image_item_info = m_pPetItemSlot->GetImageItemInfo();
    m_pPetInfo = pet_parser->GetPetInfo(item_info->m_wSkillCode);
    m_pImagePetInfo = m_pPetInfo;
    if (m_pPetInfo != NULL && item_info != image_item_info)
    {
        m_pImagePetInfo = pet_parser->GetPetInfo(image_item_info->m_wSkillCode);
        if (m_pImagePetInfo == NULL) {
            m_pPetInfo = NULL;
        }
    };
    //m_pPetInfo = PetInfoParser::Instance()->GetPetInfo(m_pPetItemSlot->GetItemInfo()->m_wSkillCode);
    if(!m_pPetInfo)
    {
        pPetItemSlot->SetUserBlocked(FALSE);
        Clear();
        return RC::RC_PET_SUMMON_RETURN_FAILED;
    }

    m_FiveMinTimer.SetTimer(1000 * 60 * 5);
    m_TenMinTimer.SetTimer(1000 * 60 * 10);

    if(GetFullness() > 0)
    {
        ApplyBasicOption();
        StartAddOption();
    }

    MSG_CG_ETC_PET_SUMMON_RETURN_BRD brd;
    brd.m_dwObjectKey   = m_pPlayer->GetObjectKey();
    brd.m_bIsSummon     = 1;
    brd.m_byFullness    = GetFullness();
    brd.m_Intimacy      = GetIntimacy();
    brd.m_bSetPetName   = IsSetPetName();
    if(brd.m_bSetPetName && GetPetName())
        memcpy(brd.m_PetName, GetPetName(), MAX_PET_NAME_LENGTH);
    brd.m_wPetIndex     = m_pImagePetInfo->m_wIndexCode;
    m_pPlayer->SendPacketAround(&brd, sizeof(brd), FALSE);

    return RC::RC_PET_SUMMON_SUMMON_SUCCESS;
}

RC::ePET_SUMMON_RESULT
CPetManager::ReturnPet(SCItemSlot* pPetItemSlot, BOOL bForce)
{
    if(!bForce)
    {
        if(m_SummonedPet == FALSE)
            return RC::RC_PET_SUMMON_NOT_SUMMONED_PET;

        if(pPetItemSlot->GetSerial() != m_PetItemSerial)
            return RC::RC_PET_SUMMON_MISMATCH_PET_ITEM;

        if(!m_pPlayer)
            return RC::RC_PET_SUMMON_RETURN_FAILED; 
    }

    MSG_CG_ETC_PET_SUMMON_RETURN_BRD brd;
    brd.m_dwObjectKey   = m_pPlayer->GetObjectKey();
    brd.m_bIsSummon     = 0;
    brd.m_wPetIndex     = m_pImagePetInfo->m_wIndexCode;

    m_pPlayer->SendPacketAround(&brd, sizeof(brd), !!bForce);
    //if(bForce) <- (WAVERIX) SendPacketAround
    //    m_pPlayer->SendPacket(&brd, sizeof(brd));

    pPetItemSlot->SetUserBlocked(FALSE);
    if(m_bMAXIntimacy || GetIntimacy() >= 100)
    {
        m_bMAXIntimacy = FALSE;
        DecreaseIntimacy(30);
        m_MaxIntimacyTimer.DisableCheckTime();
    }

    m_FiveMinTimer.DisableCheckTime();
    m_TenMinTimer.DisableCheckTime();

    CancelBasicOption();
    EndAddOption();

#ifdef _NA_007854_20141127_MODIFY_PET_CHARGE_SUBTYPE
    if (m_pPetItemSlot->GetItemRuleInfo().charge_sub_type == eCHARGE_SUB_FIXED_QUANTITY) {
        m_pPetItemSlot->EndQuantity();
    }
#else
    m_pPetItemSlot->EndQuantity();
#endif //_NA_007854_20141127_MODIFY_PET_CHARGE_SUBTYPE

    Clear();

    return RC::RC_PET_SUMMON_RETURN_SUCCESS;
}

RC::ePET_SUMMON_RESULT CPetManager::ReturnPet( BOOL bForce /*= FALSE*/ )
{
    if (AlreadySummonedPet() == FALSE) {
        return RC::RC_PET_SUMMON_NOT_SUMMONED_PET;
    }

    SCItemSlot* const pet_slot = GetUsedPetSlot();
    if (pet_slot == NULL) {
        return RC::RC_PET_SUMMON_RETURN_FAILED;
    }
    return ReturnPet(pet_slot, TRUE);
}

void
CPetManager::ApplyBasicOption()
{
    //이미 옵션 적용 중
    if(m_bAppliedBasicOption)
        return;

    if(!m_pPetItemSlot)
        return;

    if(!m_pPetItemSlot->GetItemInfo())
        return;

    //옵션 적용
    SkillAttrCalculator skill_calculator(m_pPlayer->GetAttr()); 

    StatusManager* const pStatusManager = m_pPlayer->GetStatusManager();
    RecoverStatus* pRecoverHPStatus =
        static_cast<RecoverStatus*>(pStatusManager->FindStatus(eCHAR_STATE_ETC_AUTO_RECOVER_HP));
    RecoverStatus* pRecoverHPMPStatus =
        static_cast<RecoverStatus*>(pStatusManager->FindStatus(eCHAR_STATE_ETC_AUTO_RECOVER_HPMP));

    const BASE_ITEMINFO* item_info = m_pPetItemSlot->GetItemInfo();

    for(int op = 0; op < _countof(item_info->m_wOptionIndex); ++op)
    {
        if(item_info->m_wOptionIndex[op] != 0 && 
           item_info->m_iOptionValue[op] != 0 &&
           item_info->m_wOptionIndex[op] < _countof(g_ITEM_OPTION))
        {
            skill_calculator.AddAttr(g_ITEM_OPTION[item_info->m_wOptionIndex[op]],
                                     item_info->m_byOptionType[op],
                                     item_info->m_iOptionValue[op]);

            if(g_ITEM_OPTION[item_info->m_wOptionIndex[op]] == eATTR_RECOVERY_HP)
            {
                if(pRecoverHPStatus)
                    pRecoverHPStatus->SetUsePet(true);
                if(pRecoverHPMPStatus)
                    pRecoverHPMPStatus->SetUsePet(true);
            }
        }
    }

    m_bAppliedBasicOption = TRUE;
}

void
CPetManager::CancelBasicOption()
{
    //기본 옵션이 적용중 아니면
    if(!m_bAppliedBasicOption)
        return;

    if(!m_pPetItemSlot)
        return;

    SkillAttrCalculator skill_calculator(m_pPlayer->GetAttr()); 

    StatusManager* const pStatusManager = m_pPlayer->GetStatusManager();
    RecoverStatus* pRecoverHPStatus =
        static_cast<RecoverStatus*>(pStatusManager->FindStatus(eCHAR_STATE_ETC_AUTO_RECOVER_HP));
    RecoverStatus* pRecoverHPMPStatus =
        static_cast<RecoverStatus*>(pStatusManager->FindStatus(eCHAR_STATE_ETC_AUTO_RECOVER_HPMP));

    const BASE_ITEMINFO* item_info = m_pPetItemSlot->GetItemInfo();

    for(int op = 0; op < _countof(item_info->m_wOptionIndex); ++op)
    {
        if(item_info->m_wOptionIndex[op] != 0 && 
           item_info->m_iOptionValue[op] != 0 &&
           item_info->m_wOptionIndex[op] < _countof(g_ITEM_OPTION))
        {
            skill_calculator.DelAttr(g_ITEM_OPTION[item_info->m_wOptionIndex[op]],
                                     item_info->m_byOptionType[op],
                                     item_info->m_iOptionValue[op]);
        }
    }

    if(pRecoverHPStatus)
        pRecoverHPStatus->SetUsePet(false);
    if(pRecoverHPMPStatus)
        pRecoverHPMPStatus->SetUsePet(false);

    m_bAppliedBasicOption = FALSE;
}

void
CPetManager::StartAddOption()
{
    if(m_bAppliedAddOption)
        return;

    m_bAppliedAddOption = TRUE;

    BYTE byOptionLV = GetOptionLVByIntimacy();

    ApplyAddOption(byOptionLV);
}

void
CPetManager::EndAddOption()
{
    if(!m_bAppliedAddOption)
        return;

    CancelAddOption();

    m_bAppliedAddOption = FALSE;
}

void
CPetManager::ApplyAddOption(BYTE byOptionLV)
{
    if(!m_bAppliedAddOption)
        return;

    if(!m_pPetInfo)
        return;

    if(byOptionLV >= MAX_PET_OPTION_EXTENT_COUNT)
    {
        ASSERT(FALSE);
        return;
    }

    //옵션 레벨 저장
    m_AppliedOptionLV = byOptionLV;

    //옵션 적용
    SkillAttrCalculator skill_calculator(m_pPlayer->GetAttr());

    StatusManager* const pStatusManager = m_pPlayer->GetStatusManager();
    RecoverStatus* pRecoverHPStatus = \
        static_cast<RecoverStatus*>(pStatusManager->FindStatus(eCHAR_STATE_ETC_AUTO_RECOVER_HP));
    RecoverStatus* pRecoverHPMPStatus = \
        static_cast<RecoverStatus*>(pStatusManager->FindStatus(eCHAR_STATE_ETC_AUTO_RECOVER_HPMP));

#ifdef __CN_PET_OPTION_APPLY_FLAG
    // (CHANGES) (100211) (WAVERIX) Pet누적 옵션의 기획과 구현의 상호 오해가 있었던 듯. 0의 조건 추가
    //  0 = 이전 레벨 옵션은 적용시키지 않고 해당 레벨의 옵션만 적용된다.
    // !0 = 해당 플랙의 설정이 있는 경우, 0%~부터 (설정된 것만) 순차 적용
    if (m_pPetInfo->m_eApplyOptionFlag != 0)
    {
        for (BYTE i = 0; i <= byOptionLV; ++i)
        {
            if (m_pPetInfo->CanUseOption(i) == false) {
                continue;
            }
            const sPetOptionInfo& pet_option = m_pPetInfo->m_PetOptionInfo[i];
            for (int op = 0 ; op < MAX_PET_OPTION_COUNT; ++op)
            {
                if (pet_option.m_wOptionKind[op] != 0    &&
                    pet_option.m_byValueType[op] != 0  &&
                    pet_option.m_wOptionKind[op] < _countof(g_ITEM_OPTION))
                {
                    skill_calculator.AddAttr(g_ITEM_OPTION[pet_option.m_wOptionKind[op]],
                                             pet_option.m_byValueType[op],
                                             pet_option.m_iValue[op]);

                    if (g_ITEM_OPTION[pet_option.m_wOptionKind[op]] == eATTR_RECOVERY_HP)
                    {
                        if (pRecoverHPStatus) {
                            pRecoverHPStatus->SetUsePet(true);
                        }
                        if (pRecoverHPMPStatus) {
                            pRecoverHPMPStatus->SetUsePet(true);
                        }
                    }
                }
            } //end 'for'
        } //end 'for'
    } //end if 'Cumulate' flag != 0
    else //if m_pPetInfo->m_eApplyOptionFlag == 0 then apply current level only
#endif
    {
        const sPetOptionInfo& pet_option = m_pPetInfo->m_PetOptionInfo[byOptionLV];
        for (int op = 0; op < MAX_PET_OPTION_COUNT; ++op)
        {
            if (pet_option.m_wOptionKind[op] != 0   &&
                pet_option.m_byValueType[op] != 0  &&
                pet_option.m_wOptionKind[op] < _countof(g_ITEM_OPTION))
            {
                skill_calculator.AddAttr(g_ITEM_OPTION[pet_option.m_wOptionKind[op]],
                                         pet_option.m_byValueType[op],
                                         pet_option.m_iValue[op]);

                if (g_ITEM_OPTION[pet_option.m_wOptionKind[op]] == eATTR_RECOVERY_HP)
                {
                    if (pRecoverHPStatus) {
                        pRecoverHPStatus->SetUsePet(true);
                    }
                    if (pRecoverHPMPStatus) {
                        pRecoverHPMPStatus->SetUsePet(true);
                    }
                }
            }
        } //end 'for' single loop
    }
}

void
CPetManager::CancelAddOption()
{
    if(!m_bAppliedAddOption)
        return;

    if(!m_pPetInfo)
        return;

    if(m_AppliedOptionLV >= MAX_PET_OPTION_EXTENT_COUNT)
        return;

    SkillAttrCalculator skill_calculator(m_pPlayer->GetAttr()); 

    StatusManager* const pStatusManager = m_pPlayer->GetStatusManager();
    RecoverStatus* pRecoverHPStatus = \
        static_cast<RecoverStatus*>(pStatusManager->FindStatus(eCHAR_STATE_ETC_AUTO_RECOVER_HP));
    RecoverStatus* pRecoverHPMPStatus = \
        static_cast<RecoverStatus*>(pStatusManager->FindStatus(eCHAR_STATE_ETC_AUTO_RECOVER_HPMP));

#ifdef __CN_PET_OPTION_APPLY_FLAG
    // (CHANGES) (100211) (WAVERIX) Pet누적 옵션의 기획과 구현의 상호 오해가 있었던 듯. 0의 조건 추가
    //  0 = 이전 레벨 옵션은 적용시키지 않고 해당 레벨의 옵션만 해제된다.
    // !0 = 해당 플랙의 설정이 있는 경우, 0%~부터 (설정된 것만) 순차 해제
    if (m_pPetInfo->m_eApplyOptionFlag != 0)
    {
        for (BYTE i = 0; i <= m_AppliedOptionLV; ++i)
        {
            if (m_pPetInfo->CanUseOption(i) == false) {
                continue;
            }
            const sPetOptionInfo& pet_option = m_pPetInfo->m_PetOptionInfo[i];
            for (int op = 0; op < MAX_PET_OPTION_COUNT; ++op)
            {
                if (pet_option.m_wOptionKind[op] != 0 &&
                    pet_option.m_byValueType[op] != 0 &&
                    pet_option.m_wOptionKind[op] < _countof(g_ITEM_OPTION))
                {
                    skill_calculator.DelAttr(g_ITEM_OPTION[pet_option.m_wOptionKind[op]],
                                             pet_option.m_byValueType[op],
                                             pet_option.m_iValue[op]);
                }
            } //end 'for'
        } //end 'for'
    }
    else //if m_pPetInfo->m_eApplyOptionFlag == 0 then release current level only
#endif
    {
        const sPetOptionInfo& pet_option = m_pPetInfo->m_PetOptionInfo[m_AppliedOptionLV];
        for (int op = 0 ; op < MAX_PET_OPTION_COUNT ; ++op)
        {
            if (pet_option.m_wOptionKind[op] != 0 &&
                pet_option.m_byValueType[op] != 0 &&
                pet_option.m_wOptionKind[op] < _countof(g_ITEM_OPTION))
            {
                skill_calculator.DelAttr(g_ITEM_OPTION[pet_option.m_wOptionKind[op]],
                                         pet_option.m_byValueType[op],
                                         pet_option.m_iValue[op]);
            }
        }
    }

    if(pRecoverHPStatus) {
        pRecoverHPStatus->SetUsePet(false);
    }
    if(pRecoverHPMPStatus) {
        pRecoverHPMPStatus->SetUsePet(false);
    }

    m_AppliedOptionLV   = MAX_PET_OPTION_EXTENT_COUNT;
}

BOOLEAN
CPetManager::IsFullnessMin()
{
    if(!IsValidPetItemSlot())
        return FALSE;

    if(m_pPetItemSlot->GetPetInfo()->GetFullness() > 0)
        return FALSE;

    return TRUE;
}

BYTE
CPetManager::GetOptionLVByIntimacy()
{
    if(!IsValidPetItemSlot())
        return MAX_PET_OPTION_EXTENT_COUNT;

    int intimacy = m_pPetItemSlot->GetPetInfo()->GetIntimacy();

    for(int i = 0 ; i < MAX_PET_OPTION_EXTENT_COUNT; i++)
    {
        if(m_pPetInfo->m_byIntimacyMin[i] <= intimacy && intimacy <= m_pPetInfo->m_byIntimacyMax[i])
            return i;
    }

    return MAX_PET_OPTION_EXTENT_COUNT;
}

void
CPetManager::Update()
{
    if(!IsValidPetItemSlot())
        return;

    if(!m_pPlayer)
        return;

    GameZone* pZone = m_pPlayer->GetGameZonePtr();
    if(!pZone)
        return;

    if(m_FiveMinTimer.IsExpired())
    {       
        if(pZone->GetZoneType() != eZONETYPE_VILLAGE)
        {
            DecreaseFullness(m_pPetInfo->m_byFullnessDown[0]);
        }       
    }

    if(m_TenMinTimer.IsExpired())
    {
        if(pZone->GetZoneType() == eZONETYPE_VILLAGE)
        {
            IncreaseFullness(m_pPetInfo->m_byFullnessUp);
        }
        else
        {
            IncreaseIntimacy(m_pPetInfo->m_byIntimacyUp);
        }
    }

    if(m_MaxIntimacyTimer.IsExpired())
    {
        m_bMAXIntimacy = FALSE;
        DecreaseIntimacy(30);
        m_MaxIntimacyTimer.DisableCheckTime();
    }

    if(GetFullness() <= 0 && m_SummonedPet && pZone->GetZoneType() != eZONETYPE_VILLAGE)
    {       
        ReturnPet(m_pPetItemSlot);
        MSG_CG_ETC_PET_RETURN_CMD cmd;
        cmd.m_ItemPos   = m_PetItemPos;
        m_pPlayer->SendPacket(&cmd, sizeof(cmd));
    }

    BASE_PLAYERINFO* player_info = m_pPlayer->GetCharInfo();
    player_info->m_bSummonedPet = m_SummonedPet;
    player_info->m_bMAXIntimacy = m_bMAXIntimacy;
    player_info->m_PetItemPos   = m_PetItemPos;
    player_info->m_bMAXOptionRemainTime = m_MaxIntimacyTimer.GetRemainedTime();
}

BOOL
CPetManager::IsValidPetItemSlot()
{
    if(!m_SummonedPet)
        return FALSE;

    if(!m_pPetItemSlot)
        return FALSE;

    if(!m_pPetInfo)
        return FALSE;

    if(m_pPetItemSlot->GetPos() != m_PetItemPos)
        return FALSE;

    if(m_pPetItemSlot->GetSerial() != m_PetItemSerial)
        return FALSE;

    return TRUE; 
}

void
CPetManager::IncreaseFullness(BYTE value)
{
    if(!IsValidPetItemSlot())
        return;

    int FullnessValue = m_pPetItemSlot->GetPetInfo()->GetFullness();
    if(FullnessValue >= 100)
        return;
    FullnessValue += value;
    m_pPetItemSlot->GetPetInfo()->SetFullness(FullnessValue);

    if(GetFullness() > 0)
    {
        ApplyBasicOption();
        StartAddOption();
    }

    MSG_CG_ETC_PET_STATUS_CHANGE_BRD brd;
    brd.m_dwObjectKey   = m_pPlayer->GetObjectKey();
    brd.m_ItemPos       = m_PetItemPos;
    brd.m_bIsFullness   = 1;
    brd.m_byValue       = m_pPetItemSlot->GetPetInfo()->GetFullness();

    m_pPlayer->SendPacketAround(&brd, sizeof(brd));
}

void
CPetManager::DecreaseFullness(BYTE value)
{
    if(!IsValidPetItemSlot())
        return;

    int FullnessValue = m_pPetItemSlot->GetPetInfo()->GetFullness();
    if(FullnessValue <= 0)
        return;
    FullnessValue -= value;
    m_pPetItemSlot->GetPetInfo()->SetFullness(FullnessValue);

    if(GetFullness() <= 0)
    {
        CancelBasicOption();
        EndAddOption();
    }

    MSG_CG_ETC_PET_STATUS_CHANGE_BRD brd;
    brd.m_dwObjectKey   = m_pPlayer->GetObjectKey();
    brd.m_ItemPos       = m_PetItemPos;
    brd.m_bIsFullness   = 1;
    brd.m_byValue       = m_pPetItemSlot->GetPetInfo()->GetFullness();

    m_pPlayer->SendPacketAround(&brd, sizeof(brd));
}

void
CPetManager::IncreaseIntimacy(BYTE value)
{
    if(!IsValidPetItemSlot())
        return;

    if(m_bMAXIntimacy)
        return;

    //기존 친밀도에 따른 옵션 레벨
    BYTE OldOptionLV = GetOptionLVByIntimacy();

    int IntimacyValue = m_pPetItemSlot->GetPetInfo()->GetIntimacy();
    if(IntimacyValue >= 100)
        return;
    IntimacyValue += value;
    m_pPetItemSlot->GetPetInfo()->SetIntimacy(IntimacyValue);

    //변경된 친밀도에 따른 옵션 레벨
    BYTE NewOptionLV = GetOptionLVByIntimacy();

    //옵션 변경
    if(OldOptionLV != NewOptionLV)
    {
        CancelAddOption();
        ApplyAddOption(NewOptionLV);
    }

    //친밀도 MAX시 30분간 유지  
    if(MAX_PET_FULLNESS <= IntimacyValue)
    {
        m_bMAXIntimacy = TRUE;
        m_MaxIntimacyTimer.SetTimer(1000 * 60 * 30);
    }

    MSG_CG_ETC_PET_STATUS_CHANGE_BRD brd;
    brd.m_dwObjectKey   = m_pPlayer->GetObjectKey();
    brd.m_ItemPos       = m_PetItemPos;
    brd.m_bIsFullness   = 0;
    brd.m_byValue       = m_pPetItemSlot->GetPetInfo()->GetIntimacy();

    m_pPlayer->SendPacketAround(&brd, sizeof(brd));
}

void
CPetManager::DecreaseIntimacy(BYTE value)
{
    if(!IsValidPetItemSlot())
        return;

    if(m_bMAXIntimacy)
        return;

    //기존 친밀도에 따른 옵션 레벨
    BYTE OldOptionLV = GetOptionLVByIntimacy();

    int IntimacyValue = m_pPetItemSlot->GetPetInfo()->GetIntimacy();
    if(IntimacyValue <= 0)
        return;
    IntimacyValue -= value;
    m_pPetItemSlot->GetPetInfo()->SetIntimacy(IntimacyValue);

    //변경된 친밀도에 따른 옵션 레벨
    BYTE NewOptionLV = GetOptionLVByIntimacy();


    //옵션 변경
    if(OldOptionLV != NewOptionLV)
    {
        CancelAddOption();
        ApplyAddOption(NewOptionLV);
    }

    MSG_CG_ETC_PET_STATUS_CHANGE_BRD brd;
    brd.m_dwObjectKey   = m_pPlayer->GetObjectKey();
    brd.m_ItemPos       = m_PetItemPos;
    brd.m_bIsFullness   = 0;
    brd.m_byValue       = m_pPetItemSlot->GetPetInfo()->GetIntimacy();

    m_pPlayer->SendPacketAround(&brd, sizeof(brd));
}

BYTE
CPetManager::GetFullness()
{
    if(!IsValidPetItemSlot())
        return 0;

    return m_pPetItemSlot->GetPetInfo()->GetFullness();
}

BYTE
CPetManager::GetIntimacy()
{
    if(!IsValidPetItemSlot())
        return 0;

    return m_pPetItemSlot->GetPetInfo()->GetIntimacy();
}

void
CPetManager::OnDead()
{
    if(!IsValidPetItemSlot())
        return;

    DecreaseFullness(m_pPetInfo->m_byFullnessDown[1]);
    DecreaseIntimacy(m_pPetInfo->m_byIntimacyDown[1]);
}

void
CPetManager::SetSummonInfo()
{
    if(m_SummonedPet)
        return;

    if(!m_pPlayer)
        return;

    Init(m_pPlayer);

    BASE_PLAYERINFO* pInfo = m_pPlayer->GetCharInfo(); 
    SCSlotContainer* pInventory = (SCSlotContainer*)m_pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);

    if(!pInfo)
        return;

    if(!pInventory)
        return;

    if(!pInfo->m_bSummonedPet)
        return;

    SCItemSlot & rPetItemSlot = (SCItemSlot &)pInventory->GetSlot(pInfo->m_PetItemPos);

    if(!rPetItemSlot.IsPetItem())
        return;

    rPetItemSlot.SetUserBlocked(TRUE);
    m_SummonedPet       = TRUE; 
    m_PetItemSerial     = rPetItemSlot.GetSerial();
    m_PetItemPos        = rPetItemSlot.GetPos();
    m_pPetItemSlot      = &rPetItemSlot;
    // (CHANGES) (f100524.2L) updates an pet image control to support an etheria
    PetInfoParser* const pet_parser = PetInfoParser::Instance();
    const BASE_ITEMINFO* item_info = m_pPetItemSlot->GetItemInfo();
    const BASE_ITEMINFO* image_item_info = m_pPetItemSlot->GetImageItemInfo();
    m_pPetInfo = pet_parser->GetPetInfo(item_info->m_wSkillCode);
    m_pImagePetInfo = m_pPetInfo;
    if (m_pPetInfo != NULL && item_info != image_item_info)
    {
        m_pImagePetInfo = pet_parser->GetPetInfo(image_item_info->m_wSkillCode);
        if (m_pImagePetInfo == NULL) {
            m_pPetInfo = NULL;
        }
    };
    //m_pPetInfo = PetInfoParser::Instance()->GetPetInfo(m_pPetItemSlot->GetItemInfo()->m_wSkillCode);

    if(!m_pPetInfo)
    {
        m_pPetItemSlot->SetUserBlocked(FALSE);
        Clear();
        return;
    }

    m_FiveMinTimer.SetTimer(1000 * 60 * 5);
    m_TenMinTimer.SetTimer(1000 * 60 * 10);

    if(GetFullness() > 0)
    {
        ApplyBasicOption();
        StartAddOption();
    }

    /*
    MSG_CG_ETC_PET_SUMMON_RETURN_BRD brd;
    brd.m_dwObjectKey   = m_pPlayer->GetObjectKey();
    brd.m_bIsSummon     = 1;
    brd.m_wPetIndex     = m_pImagePetInfo->m_wIndexCode;
    m_pPlayer->SendPacketAround(&brd, sizeof(brd), FALSE);
    */

    if(pInfo->m_bMAXIntimacy)
    {
        m_bMAXIntimacy  = TRUE;
        m_MaxIntimacyTimer.SetTimer(pInfo->m_bMAXOptionRemainTime);
    }

    m_pPetItemSlot->StartQuantity();
}

void
CPetManager::SetPetName(const char* pszPetName)
{
    if(!IsValidPetItemSlot())
        return;

    m_pPetItemSlot->GetPetInfo()->SetPetName(pszPetName);

    MSG_CG_ETC_PET_SET_NAME_BRD brd;
    brd.m_dwObjectKey   = m_pPlayer->GetObjectKey();
    memcpy(brd.m_PetName, m_pPetItemSlot->GetPetInfo()->GetPetName(), MAX_PET_NAME_LENGTH);
    m_pPlayer->SendPacketAround(&brd, sizeof(brd), FALSE);
}

const char*
CPetManager::GetPetName()
{
    if(!IsValidPetItemSlot())
        return NULL;

    return m_pPetItemSlot->GetPetInfo()->GetPetName();
}

BOOL
CPetManager::IsSetPetName()
{
    if(!IsValidPetItemSlot())
        return FALSE;

    return m_pPetItemSlot->GetPetInfo()->IsSetPetName();
}

