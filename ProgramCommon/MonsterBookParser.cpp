#include "StdAfx.h"
#include ".\MonsterBookParser.h"

#ifdef _NA_007086_20140318_MONSTERBOOK

MonsterBookParser::MonsterBookParser()
{
}

MonsterBookParser::~MonsterBookParser()
{
    FOREACH_CONTAINER(
        const MonsterBookContainer::value_type& monsterbook_info_pair, monster_book_container_, MonsterBookContainer)
    {
        MonsterBookInfo* monsterbook_info = monsterbook_info_pair.second;
        SAFE_DELETE(monsterbook_info);
    }

    monster_book_container_.clear();
}

BOOL MonsterBookParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    switch (script_code)
    {
    case SCRIPT_MONSTER_BOOK:
        return (LoadMonsterBook(!!is_reload) ? TRUE : FALSE);
    }
    return FALSE;
}

bool MonsterBookParser::LoadMonsterBook(const bool is_reload)
{
    __UNUSED(is_reload);

    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        MonsterBookInfo* monsterbook_info(new MonsterBookInfo);
        {
            GetData(row, monsterbook_info->index_, _T("Index"));
            GetData(row, monsterbook_info->main_category_, _T("MainCategory"));
            GetData(row, monsterbook_info->main_category_string_, _T("MainString"));
            GetData(row, monsterbook_info->sub_category_, _T("SubCategory"));
            GetData(row, monsterbook_info->sub_category_string_, _T("SubString"));
            GetData(row, monsterbook_info->monster_code_, _T("MonsterCode"));
            GetData(row, monsterbook_info->monster_icon_, _T("MonsterIcon"));
            GetData(row, monsterbook_info->ac_code_, _T("ACCode"));
        }

        MonsterBookContainer_Iterator iter = monster_book_container_.find(monsterbook_info->monster_code_);
        if (iter != monster_book_container_.end())
        {
            // 몬스터코드 중복됨
            SUNLOG(eDEV_LOG, "MonsterBook >> monstercode overlap, code  = %d",  monsterbook_info->monster_code_);
            SAFE_DELETE(monsterbook_info);
            continue;
        }

        monster_book_container_.insert(std::make_pair(monsterbook_info->monster_code_, monsterbook_info));
    }
    return true;
}

const MonsterBookInfo* MonsterBookParser::FindMonsterBookInfo(const DWORD monster_code) const
{
    const MonsterBookContainer::const_iterator iter = monster_book_container_.find(monster_code);
    if (iter != monster_book_container_.end())
    {
        return iter->second;
    }
    return NULL;
}

#endif //_NA_007086_20140318_MONSTERBOOK