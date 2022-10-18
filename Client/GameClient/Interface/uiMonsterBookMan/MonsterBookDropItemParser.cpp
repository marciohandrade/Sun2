#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007086_20140318_MONSTERBOOK

#include "uiMonsterBookMan/MonsterBookDropItemParser.h"
#include "ItemInfoParser.h"
#include "GameFunc.h"
#include "InterfaceManager.h"

MonsterBookDropItemParser::MonsterBookDropItemParser()
{
}
MonsterBookDropItemParser::~MonsterBookDropItemParser()
{
    Unload();
}

void MonsterBookDropItemParser::Unload()
{
    MONSTERBOOK_DROP_ITEM_CATEGORY::iterator itr = data_table_.begin();
    MONSTERBOOK_DROP_ITEM_CATEGORY::iterator itr_end = data_table_.end();
    for (; itr != itr_end; ++itr)
    {
        MONSTERBOOK_DROP_ITEM_GROUP* item_group = itr->second;
        MONSTERBOOK_DROP_ITEM_GROUP::iterator itr2 = item_group->begin();
        MONSTERBOOK_DROP_ITEM_GROUP::iterator itr_end2 = item_group->end();
        for (; itr2 != itr_end2; ++itr2)
        {
            MONSTERBOOK_DROPITEM_LIST* item_list = itr2->second;
            item_list->clear();
            delete item_list;
        }
        item_group->clear();
        delete item_group;
    }
    data_table_.clear();
}

MONSTERBOOK_DROPITEM_LIST* MonsterBookDropItemParser::GetMonsterBookDropItem(
    const DWORD monster_code, const DWORD item_type)
{
    MONSTERBOOK_DROP_ITEM_CATEGORY::iterator itr = data_table_.find(monster_code);
    if (itr != data_table_.end())
    {
        MONSTERBOOK_DROP_ITEM_GROUP* drop_item_group = itr->second;
        MONSTERBOOK_DROP_ITEM_GROUP::iterator itr2 = drop_item_group->find(item_type);
        if (itr2 != drop_item_group->end())
        {
            return itr2->second;
        }
    }

    return NULL;
}
BOOL MonsterBookDropItemParser::IsMonsterDrop(const DWORD monster_code)
{
    MONSTERBOOK_DROP_ITEM_CATEGORY::iterator itr = data_table_.find(monster_code);
    if (itr != data_table_.end())
    {
        MONSTERBOOK_DROP_ITEM_GROUP* drop_item_group = itr->second;
        MONSTERBOOK_DROP_ITEM_GROUP::iterator group_itr     = drop_item_group->begin();
        MONSTERBOOK_DROP_ITEM_GROUP::iterator group_itr_end = drop_item_group->end();
        for ( ; group_itr != group_itr_end; ++group_itr)
        {
            MONSTERBOOK_DROPITEM_LIST* item_list = group_itr->second;
            for (size_t i = 0; i < item_list->size(); ++i)
            {
                if ((*item_list)[i] == monster_code)
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}
BOOL MonsterBookDropItemParser::SearchMonsterDrop(DWORD monster_code, TCHAR* item_name)
{
    MONSTERBOOK_DROP_ITEM_CATEGORY::iterator itr = data_table_.find(monster_code);
    if (itr != data_table_.end())
    {
        MONSTERBOOK_DROP_ITEM_GROUP* drop_item_group = itr->second;
        MONSTERBOOK_DROP_ITEM_GROUP::iterator group_itr     = drop_item_group->begin();
        MONSTERBOOK_DROP_ITEM_GROUP::iterator group_itr_end = drop_item_group->end();
        for ( ; group_itr != group_itr_end; ++group_itr)
        {
            MONSTERBOOK_DROPITEM_LIST* item_list = group_itr->second;
            for (size_t i = 0; i < item_list->size(); ++i)
            {
                const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo((*item_list)[i]);
                if (item_info == NULL)
                {
                    continue;
                }
                TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
                GameFunc::GetItemName(NULL, item_info, string, INTERFACE_STRING_LENGTH-1);
                g_InterfaceManager.Trim(string);
                std::string name_string = string;
                if (name_string.find(item_name, 0) != -1)
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}
BOOL MonsterBookDropItemParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    if (script_code != SCRIPT_MONSTER_BOOK_DROP_ITEM)
    {
        return FALSE;
    }

    int row_size = GetRowSize();

    for (int row = 0; row < row_size; ++row)
    {
        BYTE group_number       = GetDataBYTE("GroupNum", row);
        DWORD monster_code      = GetDataDWORD("Ncode", row);


        MONSTERBOOK_DROP_ITEM_CATEGORY::iterator itr = data_table_.find(monster_code);
        if (itr == data_table_.end())
        {
            // 몬스터코드로 나눠진 상위카테고리. 없으면 생성
            MONSTERBOOK_DROP_ITEM_GROUP* item_group(new MONSTERBOOK_DROP_ITEM_GROUP);

            data_table_.insert(MONSTERBOOK_DROP_ITEM_CATEGORY::value_type(monster_code, item_group));

            itr = data_table_.find(monster_code);
        }

        MONSTERBOOK_DROP_ITEM_GROUP* drop_item_group = itr->second;

        // 아이템 타입으로 나눠진 아이템그룹(하위카테고리)에 추가
        // 한 row에 최대 50개까지 추가
        TCHAR colum_name[INTERFACE_STRING_LENGTH] = {0,};
        DWORD item_code = 0;
        for (int i = 1; i <= 50; ++i)
        {
            Snprintf(colum_name, INTERFACE_STRING_LENGTH-1, "IC%d", i);
            item_code = GetDataDWORD(colum_name, row);

            if (item_code == 0)
            {
                continue;
            }

            const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
            if (item_info == NULL)
            {
                continue;
            }

            MONSTERBOOK_DROP_ITEM_GROUP::iterator itr2 = drop_item_group->find(item_info->m_wType);
            if (itr2 == drop_item_group->end())
            {
                // 아이템타입으로 나눠진 하위카테고리. 없으면 생성
                MONSTERBOOK_DROPITEM_LIST* item_list(new MONSTERBOOK_DROPITEM_LIST);

                drop_item_group->insert(MONSTERBOOK_DROP_ITEM_GROUP::value_type(item_info->m_wType, item_list));

                itr2 = drop_item_group->find(item_info->m_wType);
            }

            MONSTERBOOK_DROPITEM_LIST* item_list = itr2->second;
            
            // 중복 안된거면 추가
            bool is_find = false;
            for (size_t j = 0; j < item_list->size(); ++j)
            {
                if ((*item_list)[j] == item_code)
                {
                    is_find = true;
                }
            }
            if (is_find == false)
            {
                item_list->push_back(item_code);
            }
        }
    }

    return TRUE;
}

#endif //_NA_007086_20140318_MONSTERBOOK