#pragma once

#ifdef _NA_20100307_BETAKEY_SYSTEM

#include <BetaKey.h>
#include <EnchantParser.h>

class BetaKeyImpl
{
public:
#ifdef _NA_002050_20110216_ADD_GENDER
    //배타기로 선택된 캐릭터를 생성할수 있는지 검사한다.
    static bool CanCreateCharacter(BetaKeyInfo* const betakey_info, BYTE class_code, BYTE gender);
#endif
    //베타키를 가진 사용자가 소요되는 인첸비용을 계산한다.
    static MONEY GetEnchantMoney(BetaKeyInfo* const betakey_info, const sBETAKEY* const enchant_betakey_info, const MONEY enchant_money);
    //베타키를 가진 사용자가 소요되는 포탈 비용을 계산한다.
    static DWORD GetPortalMoney(BetaKeyInfo* const betakey_info, sMAP_ENTERENCE_PORTAL* const portal_Info);
};

#endif //_NA_20100307_BETAKEY_SYSTEM