#pragma once

#ifdef _NA_007086_20140318_MONSTERBOOK
#include <Singleton.h>


struct MonsterBookDropOrderInfo
{
    BYTE    index;
    DWORD   item_type;
    BYTE    category_group;
    DWORD   category_string;
    BYTE    show_type;

    BOOL    tree_unfold;
};


class MonsterBookDropOrderParser : public util::Singleton<MonsterBookDropOrderParser>, public IParser
{
    typedef STLX_HASH_MAP<BYTE, MonsterBookDropOrderInfo*> MONSTERBOOK_DROP_ORDER_MAP; // 이건 그냥 벡터로 해도 상관없을듯

public:
    MonsterBookDropOrderParser();
    ~MonsterBookDropOrderParser();
    virtual void Release() {}
    virtual	BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    MonsterBookDropOrderInfo* GetMonsterBookDropOrderInfo(const BYTE index);

private:
    void Unload();
    MONSTERBOOK_DROP_ORDER_MAP data_table_;
};

#endif //_NA_007086_20140318_MONSTERBOOK