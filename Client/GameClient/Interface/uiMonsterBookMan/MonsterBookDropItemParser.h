#pragma once

#ifdef _NA_007086_20140318_MONSTERBOOK
#include <Singleton.h>

typedef STLX_VECTOR<DWORD> MONSTERBOOK_DROPITEM_LIST;  // 아이템코드 리스트

class MonsterBookDropItemParser : public util::Singleton<MonsterBookDropItemParser>, public IParser
{
    typedef STLX_HASH_MAP<DWORD, MONSTERBOOK_DROPITEM_LIST*> MONSTERBOOK_DROP_ITEM_GROUP;    // 타입별로 묶은 리스트들
    typedef STLX_HASH_MAP<DWORD, MONSTERBOOK_DROP_ITEM_GROUP*> MONSTERBOOK_DROP_ITEM_CATEGORY; // 몬스터별로 묶은 그룹들

public:
    MonsterBookDropItemParser();
    ~MonsterBookDropItemParser();
    virtual void Release() {}
    virtual	BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    MONSTERBOOK_DROPITEM_LIST* GetMonsterBookDropItem(const DWORD monster_code, const DWORD item_type);
    BOOL IsMonsterDrop(const DWORD monster_code);
    BOOL SearchMonsterDrop(DWORD monster_code, TCHAR* item_name);

private:
    void Unload();
    MONSTERBOOK_DROP_ITEM_CATEGORY data_table_;
};

#endif //_NA_007086_20140318_MONSTERBOOK