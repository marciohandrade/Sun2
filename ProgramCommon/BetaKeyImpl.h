#pragma once

#ifdef _NA_20100307_BETAKEY_SYSTEM

#include <BetaKey.h>
#include <EnchantParser.h>

class BetaKeyImpl
{
public:
#ifdef _NA_002050_20110216_ADD_GENDER
    //��Ÿ��� ���õ� ĳ���͸� �����Ҽ� �ִ��� �˻��Ѵ�.
    static bool CanCreateCharacter(BetaKeyInfo* const betakey_info, BYTE class_code, BYTE gender);
#endif
    //��ŸŰ�� ���� ����ڰ� �ҿ�Ǵ� ��þ����� ����Ѵ�.
    static MONEY GetEnchantMoney(BetaKeyInfo* const betakey_info, const sBETAKEY* const enchant_betakey_info, const MONEY enchant_money);
    //��ŸŰ�� ���� ����ڰ� �ҿ�Ǵ� ��Ż ����� ����Ѵ�.
    static DWORD GetPortalMoney(BetaKeyInfo* const betakey_info, sMAP_ENTERENCE_PORTAL* const portal_Info);
};

#endif //_NA_20100307_BETAKEY_SYSTEM