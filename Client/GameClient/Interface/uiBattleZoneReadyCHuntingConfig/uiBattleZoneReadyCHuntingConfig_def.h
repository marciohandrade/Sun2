#ifndef     __UIBATTLEZONEREADYCHUNTINGCONFIG_DEFINE_H__
#define     __UIBATTLEZONEREADYCHUNTINGCONFIG_DEFINE_H__
//------------------------------------------------------------------------------

const int   c_iZONE_READY_CHUNTING_CONFIG = 5;

enum
{
    eBATTLE_ZONE_READY_CHUNTING_FIELD_TYPE_0 = 0,
    eBATTLE_ZONE_READY_CHUNTING_FIELD_TYPE_1 = 0,
    eBATTLE_ZONE_READY_CHUNTING_FIELD_TYPE_2 = 0,
    eBATTLE_ZONE_READY_CHUNTING_FIELD_TYPE_3 = 0,
    eBATTLE_ZONE_READY_CHUNTING_FIELD_TYPE_4 = 0,
};

class uicBattleZoneReadyCHunting_Config
{
public:
    uicBattleZoneReadyCHunting_Config()
    {
        clear();
    }

    void clear()
    {
		fieldType = eBATTLE_ZONE_READY_CHUNTING_FIELD_TYPE_0;
        vstrFieldNames.clear();

		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
		for(int i=0; i<c_iZONE_READY_CHUNTING_CONFIG; ++i)
		{
			ZeroMemory(szMessage, sizeof(szMessage));
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("Field %d"), i+1);
			vstrFieldNames.push_back(szMessage);
		}

        bonusType = eHUNTING_BONUS_TYPE_ITEM;
        diffcultType = eHUNTING_MONSTER_DIFFICULT_EASY;
        compRuleType = eCOMPETITION_RULE_DUAL_PARTY;
        compModeType = eCOMPETITION_MODE_TIME_ATTACK;

		fieldIndex = 0;
    }

    WORD        fieldType;
	WORD		fieldIndex;
    std::vector<util::_tstring> vstrFieldNames;

    WORD        bonusType;
    WORD        diffcultType;
    WORD        compRuleType;
    WORD        compModeType;
};

//------------------------------------------------------------------------------
#endif  //  __UIBATTLEZONEREADYCHUNTINGCONFIG_DEFINE_H__
