#include "StdAfx.h"
#include ".\cancelstatusability.h"
#include "StatusManager.h"
#include "StateInfoParser.h"

// implmented by __NA_S00015_20080828_NEW_HOPE_SHADOW

CancelStatusAbility::CancelStatusAbility(void)
{
}

CancelStatusAbility::~CancelStatusAbility(void)
{
}

bool CancelStatusAbility::Execute(Character* target, BYTE* pMsg, WORD& wMsgSize)
{
    //  base_ability_info_->m_iOption1
    //  0. 개별상태지정
    //  1. 모든상태이상클리어
    //  2. 모든상태약화클리어
    //  3. 모든상태강화클리어
    //  4. 모든특수상태클리어
    //  5. 상태이상클리어(1개만)
    //  6. 상태약화클리어(1개만)
    //  7. 상태강화클리어(1개만)
    //  8. 특수상태클리어(1개만)
    //  9. 2개상태클리어(상태이상, 상태약화)

    Ability::Execute(target, pMsg, wMsgSize);

    eSTATE_TYPE         eStateType;
    BYTE                byStopCount = 0;

    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();

    if (ability_info->m_iOption1 == 0)
    {
        BaseStatus* pStatus = NULL;

        if (ability_info->m_iOption2 != 0)
        {
            pStatus = target->GetStatusManager()->FindStatus((eCHAR_STATE_TYPE)ability_info->m_iOption2);
            if (pStatus)
            {
                BASE_STATEINFO* pInfo = StateInfoParser::Instance()->GetStateInfo(pStatus->GetStateID());
                if (pInfo && pInfo->m_byIconKind != 2)
                    pStatus->StopStatus();
            }
        }

        if (ability_info->m_iParam[0] != 0)
        {
            pStatus = target->GetStatusManager()->FindStatus((eCHAR_STATE_TYPE)ability_info->m_iParam[0]);
            if (pStatus)
            {
                BASE_STATEINFO* pInfo = StateInfoParser::Instance()->GetStateInfo(pStatus->GetStateID());
                if (pInfo && pInfo->m_byIconKind != 2)
                    pStatus->StopStatus();
            }
        }

        if (ability_info->m_iParam[1] != 0)
        {
            pStatus = target->GetStatusManager()->FindStatus((eCHAR_STATE_TYPE)ability_info->m_iParam[1]);
            if (pStatus)
            {
                BASE_STATEINFO* pInfo = StateInfoParser::Instance()->GetStateInfo(pStatus->GetStateID());
                if (pInfo && pInfo->m_byIconKind != 2)
                    pStatus->StopStatus();
            }
        }

        return true;
    }
    else if (ability_info->m_iOption1 <= 4)
    {
        if (ability_info->m_iOption1 == 1)
            eStateType = eSTATE_TYPE_ABNORMAL;
        else if (ability_info->m_iOption1 == 2)
            eStateType = eSTATE_TYPE_WEAKENING;
        else if (ability_info->m_iOption1 == 3)
            eStateType = eSTATE_TYPE_STRENGTHENING;
        else if (ability_info->m_iOption1 == 4)
            eStateType = eSTATE_TYPE_SPECIALITY;

        byStopCount = 0;

        target->GetStatusManager()->StopStatusByStateType(eStateType, byStopCount);
    }
    else if (ability_info->m_iOption1 <= 8)
    {
        if (ability_info->m_iOption1 == 5)
            eStateType = eSTATE_TYPE_ABNORMAL;
        else if (ability_info->m_iOption1 == 6)
            eStateType = eSTATE_TYPE_WEAKENING;
        else if (ability_info->m_iOption1 == 7)
            eStateType = eSTATE_TYPE_STRENGTHENING;
        else if (ability_info->m_iOption1 == 8)
            eStateType = eSTATE_TYPE_SPECIALITY;

        byStopCount = 1;

        target->GetStatusManager()->StopStatusByStateType(eStateType, byStopCount);
    }
    else if (ability_info->m_iOption1 == 9)
    {
        // 상태이상, 상태약화 중 1개만 클리어
        if (target->GetStatusManager()->StopStatusByStateType(eSTATE_TYPE_ABNORMAL, 1) > 0)
            return true;

        if (target->GetStatusManager()->StopStatusByStateType(eSTATE_TYPE_WEAKENING, 1) > 0)
            return true;
    }
    //_NA_20100503_MAGIC_SKILL_SAINTAID_CANCLE_STAT_IS_CHANGE
    else if (ability_info->m_iOption1 == 10)  
    {  
        //모든 상태이상과 모든 상태 약화를 제거 한다.  
        byStopCount = 0;  

        target->GetStatusManager()->StopStatusByStateType(eSTATE_TYPE_ABNORMAL, byStopCount);  
        target->GetStatusManager()->StopStatusByStateType(eSTATE_TYPE_WEAKENING, byStopCount);  
    }  
    else
    {
        SUNLOG(eCRITICAL_LOG, "[CancelStatusAbility::Execute] Invalid Option1 = %d", ability_info->m_iOption1);
        return false;
    }



    return true;
}
