#include "stdafx.h"
#include "./ItemOptionHeader.h"

//==================================================================================================
//

namespace ns_formula {
;

static struct AttrTypeCheckNode
{
    bool check;
} attr_type_check_node[eATTR_MAX] = { 0, };

bool CheckSkipType(eATTR_TYPE attr_type, bool success)
{
    AttrTypeCheckNode* const check_node = &attr_type_check_node[attr_type];
    if (check_node->check) {
        return false;
    }
    if (success != false) {
        check_node->check = true;
    };
    return true;
}

bool ns_formula::CheckAttrValidity(eATTR_TYPE attr_type, int option_type, int option_value)
{
    __TOUCHED((attr_type, option_value));
    // (Ref.) enum eVALUE_TYPE
    // (Check Upperboundary)
    const int OptionTypeUpperbound = VALUE_TYPE_RATIO_VALUE + 1;
    BOOST_STATIC_ASSERT(OptionTypeUpperbound == 6);
    static const BOOLEAN OptionTypeAcceptField[OptionTypeUpperbound] = {
        1,  //[0] not defined + null        : ���� �������� Ȱ���.
        1,  //[1]VALUE_TYPE_VALUE           : ���� ��ġ
        1,  //[2]VALUE_TYPE_PERCENT_PER_CUR : ����ġ ��� ����
        1,  //[3]VALUE_TYPE_PERCENT_PER_MAX : �ִ�ġ ��� ����
        0,  //[4] not defined
    #ifdef _NA000000_VALUE_TYPE_SIMPLIFICATION
        0,  //[5]VALUE_TYPE_RATIO_VALUE     : EP2:unused
    #else
        1,  //[5]VALUE_TYPE_RATIO_VALUE     : ��ġ�� �������� ���(�̵��ӵ���)
    #endif
    };

    if(OptionTypeUpperbound <= option_type)
        return false;

    if(OptionTypeAcceptField[option_type] == 0)
        return false;
#if NATION_BUILD_VERSION < 1101
//#ifdef _NA000000_VALUE_TYPE_SIMPLIFICATION
    ;{
        static int value__ = 0;
        static const char* debug_string = NULL;
        bool report = false;
        if (option_type == VALUE_TYPE_RATIO_VALUE) {
            //if (CheckSkipType(attr_type, true)) {
                value__ += 3;// success;
                report = true;
            //};
        }
        if (option_type != VALUE_TYPE_RATIO_VALUE) {
            if (CheckSkipType(attr_type, false)) {
                value__ -= 1;// fail
            };
        }
        if (report)
        {
            SUNLOG(eCRITICAL_LOG, _T("%s\t%u\t%u\t%u"),
                   (option_type == VALUE_TYPE_RATIO_VALUE ? "VALUE_TYPE_RATIO_VALUE" : "fail"),
                   attr_type, option_type, option_value);
            return false;
        };
    };
//#endif
#if !defined(_NA000000_VALUE_TYPE_SIMPLIFICATION)
    // ����Ÿ���̾�߸� �ϴ� �ɼ� üũ. ������ �Ǵ� ���� �����ϴ��İ� ������ ��.
    // ex)
    //  - ���ݼ������� �⺻���� �����ϹǷ� { VALUE_TYPE_VALUE, VALUE_TYPE_PERCENT_PER_CUR }
    //  ��� ��� �����ϴ�.
    //  - ������ �����������̳� ���߰���Ȯ�� ���� �ɼ��� ������ �Ǵ� ���� ���µ�,
    //  VALUE_TYPE_PERCENT_PER_CUR�� ����� ��� �� ��ġ�� ������ 0�� �ȴ�.
    switch(attr_type)
    {   // g_ITEM_OPTION�� �ִ� �Ӽ�
    case eATTR_DEL_PHYSICAL_TARGET_DEFENSE_RATIO: // [R]
    case eATTR_DEL_MAGICAL_TARGET_DEFENSE_RATIO: // EP2:unused
    case eATTR_DEL_FIRE_TARGET_DEFENSE_RATIO: // [R]
    case eATTR_DEL_WATER_TARGET_DEFENSE_RATIO: // [R]
    case eATTR_DEL_EARTH_TARGET_DEFENSE_RATIO: // [R]
    case eATTR_DEL_WIND_TARGET_DEFENSE_RATIO: // [R]
    case eATTR_DEL_DARKNESS_TARGET_DEFENSE_RATIO: // [R]
    //----------------------------------------------------------------------------------------------
    case eATTR_ADD_SKILL_DAMAGE_RATIO: // [R]
    case eATTR_REFLECT_DAMAGE_RATIO: // [R]
    //----------------------------------------------------------------------------------------------
    case eATTR_BONUS_MONEY_RATIO: // [R]
    case eATTR_BONUS_EXP_RATIO: // [R]
    //----------------------------------------------------------------------------------------------
    case eATTR_AREA_ATTACK_RATIO: // [R]

    case eATTR_RESURRECTION_RATIO: // [R]
    case eATTR_DOUBLE_DAMAGE_RATIO: // [R]
    case eATTR_COPOSITE_INC_RATIO: // [R]
    case eATTR_BYPASS_DEFENCE_RATIO: // [R]
    //----------------------------------------------------------------------------------------------
    //_NA_001231_20081216_ADD_OPTION_KIND
    case eATTR_RESIST_HOLDING: // [R]
    case eATTR_RESIST_SLEEP: // [R]
    case eATTR_RESIST_POISON: // [R]
    case eATTR_RESIST_KNOCKBACK: // [R]
    case eATTR_RESIST_DOWN: // [R]
    case eATTR_RESIST_STUN: // [R]
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    // g_ITEM_OPTION�� ���� �Ӽ�(�ַ� ��ų��)
    case eATTR_DEL_ALL_TARGET_DEFENSE_RATIO: // EP2:unused
    case eATTR_DEL_MELEE_TARGET_DEFENSE_RATIO: // EP2:unused
    case eATTR_DEL_RANGE_TARGET_DEFENSE_RATIO: // EP2:unused
    case eATTR_DEL_DIVINE_TARGET_DEFENSE_RATIO: // EP2:unused
    case eATTR_DEL_MAGICAL_ALL_TARGET_DEFENSE_RATIO: //[ 95] [R] ���ݴ�� ����(���) ���� ������
    //----------------------------------------------------------------------------------------------
    case eATTR_ADD_RATIO_ARMOR_HARD_DAMAGE: // EP2:unused
    case eATTR_ADD_RATIO_ARMOR_MEDIUM_DAMAGE: // EP2:unused
    case eATTR_ADD_RATIO_ARMOR_SOFT_DAMAGE: // EP2:unused
    case eATTR_ADD_RATIO_ARMOR_SIEGE_DAMAGE: // EP2:unused
    case eATTR_ADD_RATIO_ARMOR_UNARMOR_DAMAGE: // EP2:unused
    case eATTR_ADD_ATTACK_INC_RATIO: // [R]
    case eATTR_ADD_DEFENSE_INC_RATIO: // [R]
    case eATTR_INCREASE_ENCHANT_RATIO:
        {
            if(option_type != VALUE_TYPE_RATIO_VALUE)
                return false;

            if(option_value < -100)
                return false;
        }
        break;
    default:
        {
            if(option_type == VALUE_TYPE_RATIO_VALUE)
                return false;
        }
        break;
    }
#endif //_NA000000_VALUE_TYPE_SIMPLIFICATION
#endif //NATION_BUILD_VERSION < 1101
    return true;
}

// NOTE: f110323.3L, this method is planned for item attribute calculation prefetching and
// restriction filter of stable item option calculations
bool ns_formula::CheckStatsRelatedOption(uint16_t option_index)
{
    BOOST_STATIC_ASSERT(eATTR_STR == 1 && eATTR_SPR <= 5); // = { 1,2,3,4,5 }
    bool validated = (option_index != 0 && option_index < _countof(g_ITEM_OPTION));
    if (validated == false) {
        return false;
    };
    eATTR_TYPE attr_type = g_ITEM_OPTION[option_index];
    bool matched = attr_type == eATTR_TYPE_INVALID ?        false
                 : attr_type <= eATTR_SPR ?                 true
                 : attr_type == eATTR_INCREASE_STAT_POINT ? true
                 : attr_type == eATTR_DECREASE_LIMIT_STAT ? true
                 :                                          false;
    return matched;
}

}; //end of namespace 'ns_formula'

//==================================================================================================
