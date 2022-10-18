#include "stdafx.h"

#ifdef _NA_20100307_BETAKEY_SYSTEM

#include "BetaKeyImpl.h"
#include "CharInfoParser.h"
#include "Const.h"

#ifdef _NA_002050_20110216_ADD_GENDER
bool BetaKeyImpl::CanCreateCharacter(BetaKeyInfo* const betakey_info, BYTE class_code, BYTE gender)
{
    INIT_CHARINFO* const init_char_Info = CharInfoParser::Instance()->GetInitCharInfo(class_code);

    if(init_char_Info)
    {
        INT32 betakey = 0;
        
        switch(gender)
        {
        case GenderType::kMale:     betakey = init_char_Info->m_MenBeytaKey; break;
        case GenderType::kFemale:   betakey = init_char_Info->m_WomanBeytaKey; break;
        }

        return betakey_info->CanAbilityContent(betakey);
    }
    return false;
}
#endif

MONEY BetaKeyImpl::GetEnchantMoney(BetaKeyInfo* const betakey_info, const sBETAKEY* const enchant_betakey_info, MONEY enchant_money)
{
    if (betakey_info->CanAbilityBetaKey(enchant_betakey_info->m_betakey)) {
        enchant_money = static_cast<MONEY>(enchant_money * enchant_betakey_info->m_pay_ratio);
    }
    return enchant_money;
}

DWORD BetaKeyImpl::GetPortalMoney(BetaKeyInfo* const betakey_info, sMAP_ENTERENCE_PORTAL* const portal_Info)
{
    DWORD potal_money = portal_Info->dwHeim;
    if (betakey_info->CanAbilityBetaKey(portal_Info->nBeytaKey)) {
        potal_money = static_cast<DWORD>(potal_money * portal_Info->fPayApp);
    }
    return potal_money;
}

#endif //_NA_20100307_BETAKEY_SYSTEM

