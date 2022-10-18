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
    //  0. ������������
    //  1. �������̻�Ŭ����
    //  2. �����¾�ȭŬ����
    //  3. �����°�ȭŬ����
    //  4. ���Ư������Ŭ����
    //  5. �����̻�Ŭ����(1����)
    //  6. ���¾�ȭŬ����(1����)
    //  7. ���°�ȭŬ����(1����)
    //  8. Ư������Ŭ����(1����)
    //  9. 2������Ŭ����(�����̻�, ���¾�ȭ)

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
        // �����̻�, ���¾�ȭ �� 1���� Ŭ����
        if (target->GetStatusManager()->StopStatusByStateType(eSTATE_TYPE_ABNORMAL, 1) > 0)
            return true;

        if (target->GetStatusManager()->StopStatusByStateType(eSTATE_TYPE_WEAKENING, 1) > 0)
            return true;
    }
    //_NA_20100503_MAGIC_SKILL_SAINTAID_CANCLE_STAT_IS_CHANGE
    else if (ability_info->m_iOption1 == 10)  
    {  
        //��� �����̻�� ��� ���� ��ȭ�� ���� �Ѵ�.  
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
