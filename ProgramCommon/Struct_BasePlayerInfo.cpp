#include "stdafx.h"
#include "Struct_BasePlayerInfo.h"

namespace nsSlot {
;

void InventoryConfigSwitcher::SyncFromConfigure()
{
    ZeroMemory(&table_, sizeof(table_));
    //
    const SetType* src_it = configures;
    Switches* tab_it = table_.tabs,
        * const tab_end = &table_.tabs[_countof(table_.tabs)];
    for ( ; tab_it != tab_end; ++tab_it, ++src_it)
    {
        const SetType tab_value = *src_it;
        tab_it->checked[eSwitch_All] = (tab_value == this->eInsertRule_All);
        if (tab_it->checked[eSwitch_All] == false)
        {
            for (int i = 0; i < kMaxNumberOfInsertRules; ++i) {
                tab_it->checked[i] = (tab_value & (1 << i)) != 0;
            }
        }
    }
    BOOST_STATIC_ASSERT(sizeof(bool) == 1 && 1 == true);
}

bool InventoryConfigSwitcher::Toggle(int tab_index, eSwitchValue switch_value)
{
    if ((tab_index < _countof(table_.tabs)) == false) {
        return false;
    }
    if ((switch_value < eSwitch_Upperbound) == false) {
        return false;
    }
    SetType tab_fields = configures[tab_index];
    Switches* const switches = &table_.tabs[tab_index];
    if (switch_value == eSwitch_All && tab_fields == eInsertRule_All) {
        assert(switches->checked[eSwitch_All] != false);
        return false; // it has to no actions if the previous status already 'all' checked
    }

    if (switch_value == eSwitch_All) {
        tab_fields = eInsertRule_All;
    }
    else {
        tab_fields ^= 1 << switch_value;
    }

    if (tab_fields == 0) {
        tab_fields = eInsertRule_All;
    }

    if (tab_fields == eInsertRule_All)
    {
        ZeroMemory(switches->checked, sizeof(switches->checked));
        switches->checked[eSwitch_All] = true;
    }
    else if (configures[tab_index] == eInsertRule_All)
    {
        ZeroMemory(switches->checked, sizeof(switches->checked));
        tab_fields = 1 << switch_value;
        switches->checked[switch_value] = true;
    }
    else
    {
        for (int index = 0; index < this->kMaxNumberOfInsertRules; ++index) {
            switches->checked[index] = (tab_fields & (1 << index)) != 0;
        }
    }
    if (configures[tab_index] == tab_fields) {
        return true;
    }
    configures[tab_index] = tab_fields;
    changed_ = true;
    table_.changed_tabs[tab_index] = true;
    return true;
}

}; //end of namespace
/*
BOOL
BASE_PLAYERINFO_LOG::PlayerInfoConverter( int iFromVersion IN, BYTE * pFromStream IN,
											int & iToVersion OUT, BYTE * pToStream OUT )
{	
	const int iLastestVersion = STREAM_BASE_PLAYERINFO_VERSION;
	
	if( iFromVersion > iLastestVersion || iFromVersion < 1 ) return FALSE;	

	switch( iFromVersion )
	{
	case STREAM_BASE_PLAYERINFO_VERSION:
		iToVersion = iLastestVersion;
		memcpy( pToStream, pFromStream, sizeof(BASE_PLAYERINFO_LOG) );
		break;
	default:
		// 3 이전의 버전은 제공하지 않습니다. why - 컨버터 제작 전이라 이전 자료구조 모름
        ASSERT(FALSE);
		return FALSE;
		break;
	}
	return TRUE;
}
*/
