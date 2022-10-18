#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007086_20140318_MONSTERBOOK

#include "uiMonsterBookMan/MonsterBookDropOrderParser.h"

MonsterBookDropOrderParser::MonsterBookDropOrderParser()
{
}
MonsterBookDropOrderParser::~MonsterBookDropOrderParser()
{
    Unload();
}

void MonsterBookDropOrderParser::Unload()
{
    MonsterBookDropOrderInfo* drop_order_info = NULL;

    MONSTERBOOK_DROP_ORDER_MAP::iterator itr = data_table_.begin();
    MONSTERBOOK_DROP_ORDER_MAP::iterator itr_end = data_table_.end();
    for (; itr != itr_end; ++itr)
    {
        MonsterBookDropOrderInfo* drop_order_info = itr->second;
        delete drop_order_info;
    }
    data_table_.clear();
}

MonsterBookDropOrderInfo* MonsterBookDropOrderParser::GetMonsterBookDropOrderInfo(const BYTE index)
{
    MONSTERBOOK_DROP_ORDER_MAP::iterator itr = data_table_.find(index);
    if (itr != data_table_.end())
    {
        return itr->second;
    }

    return NULL;
}

BOOL MonsterBookDropOrderParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    if (script_code != SCRIPT_MONSTER_BOOK_DROP_ORDER_INFO)
    {
        return FALSE;
    }

    int row_size = GetRowSize();

    for (int row = 0; row < row_size; ++row)
    {
        BYTE    index           = GetDataBYTE("Index", row);
        DWORD   item_type       = GetDataDWORD("ItemType", row);
        BYTE    category_group  = GetDataBYTE("CategoryGroup", row);
        DWORD   category_string = GetDataDWORD("CategoryString", row);
        BYTE    show_type       = GetDataBYTE("ShowOption", row);

        MONSTERBOOK_DROP_ORDER_MAP::iterator itr = data_table_.find(index);

        if (itr == data_table_.end())
        {
            MonsterBookDropOrderInfo* drop_order_info = new MonsterBookDropOrderInfo;
            drop_order_info->index              = index;
            drop_order_info->item_type          = item_type;
            drop_order_info->category_group     = category_group;
            drop_order_info->category_string    = category_string;
            drop_order_info->show_type          = show_type;
            drop_order_info->tree_unfold        = TRUE;

            data_table_.insert(MONSTERBOOK_DROP_ORDER_MAP::value_type(index, drop_order_info));
        }
        else
        {
            // 몬스터코드 중복됨
            SUNLOG(eDEV_LOG, "MonsterBookDropOrder >> ItemType overlap, code  = %d",  item_type);
            continue;
        }
    }

    return TRUE;
}

#endif //_NA_007086_20140318_MONSTERBOOK