#include "stdafx.h"

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
#include "NPCReplaceInfoParser.h"

//namespace local
//{
//    template < typename KEY_T, typename LOW_T, typename HIGH_T >
//    KEY_T MakePairKey(const LOW_T low, const HIGH_T high)
//    {
//        BOOST_STATIC_ASSERT(sizeof(LOW_T) <= (sizeof(KEY_T)/2));
//        BOOST_STATIC_ASSERT(sizeof(HIGH_T) <= (sizeof(KEY_T)/2));
//        return (KEY_T)(low + (high << ((sizeof(KEY_T)/2)*8)));
//    }
//}

NPCReplaceInfoParser::NPCReplaceInfoParser()
{

}

NPCReplaceInfoParser::~NPCReplaceInfoParser()
{

}

BOOL NPCReplaceInfoParser::LoadScript( eSCRIPT_CODE script_code, BOOL is_reload )
{
    switch (script_code)
    {
        case SCRIPT_NPCREPLACE_INFO:
            return _Load(is_reload);
    }

    return FALSE;
}

BOOL NPCReplaceInfoParser::_Load(BOOL is_reload)
{
    __UNUSED(is_reload);

    int row_size = GetRowSize();

    for (int row_index = 0; row_index< row_size; ++row_index)
    {
        MAPCODE mapcode = (MAPCODE)GetDataDWORD("MapCode", row_index);
        MONSTERCODE monstercode_old = (MONSTERCODE)GetDataDWORD("OldNPCCode", row_index);
        MONSTERCODE monstercode_new = (MONSTERCODE)GetDataDWORD("NewNPCCode", row_index);

        if(!(npc_replaceinfo_map_.insert(NPCReplaceInfoMap::value_type( \
                MakePairKey(mapcode, monstercode_old), monstercode_new))).second)
		{
			SUNLOG(eCRITICAL_LOG, _T("%s found duplicated key : old monster_code = %d, row_index = %d"), \
                __FUNCTION__, monstercode_old, row_index);
		}
    }

    return TRUE;
}

void NPCReplaceInfoParser::Release()
{

}

DWORD NPCReplaceInfoParser::GetReplaceNPCCode( const MAPCODE mapcode, const MONSTERCODE monstercode ) const
{
    const NPCReplaceInfoMap::const_iterator it = \
        npc_replaceinfo_map_.find(MakePairKey(mapcode, monstercode));

    return (it == npc_replaceinfo_map_.end() ? 0 : it->second);
}

#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE