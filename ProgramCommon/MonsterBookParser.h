#pragma once

#ifndef PROGRAMCOMMON_MONSTERBOOK_MONSTERBOOKPARSER_H_
#define PROGRAMCOMMON_MONSTERBOOK_MONSTERBOOKPARSER_H_

#include <Singleton.h>
#include <ScriptCode.IParser.h>

#ifdef _NA_007086_20140318_MONSTERBOOK

class MonsterBookInfo
{
public:
    DWORD index_;               // �ε���
    WORD main_category_;        // ���� ī�װ�
    DWORD main_category_string_;// ���� ī�װ� ����
    WORD sub_category_;         // ���� ī�װ�
    DWORD sub_category_string_; // ���� ī�װ� ����

    DWORD monster_code_;    // ���� �ڵ�
    DWORD monster_icon_;    // ���� ������
    DWORD ac_code_;         // ac �ڵ�
    
#ifdef _SERVER
#else
    BOOL  active_;
    BOOL  is_new_;
    BOOL  tree_unfold_;
#endif //_SERVER

    MonsterBookInfo()
    {
        ::ZeroMemory(this, sizeof(*this));

#ifdef _SERVER
#else
        tree_unfold_ = TRUE;
#endif //_SERVER
    }
private:
    __DISABLE_COPY(MonsterBookInfo);
};

class MonsterBookParser : public util::Singleton<MonsterBookParser>, public IParser
{
public:
    typedef STLX_HASH_MAP<DWORD, MonsterBookInfo*> MonsterBookContainer;
    typedef MonsterBookContainer::iterator MonsterBookContainer_Iterator;


    MonsterBookParser();
    ~MonsterBookParser();

    virtual void Release() {}
    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    const MonsterBookInfo* FindMonsterBookInfo(DWORD monster_code) const;

    MonsterBookContainer_Iterator MonsterBookInfo_Begin() { return monster_book_container_.begin(); }
    MonsterBookContainer_Iterator MonsterBookInfo_End() { return monster_book_container_.end(); }
    int GetMonsterTotalCount() { return monster_book_container_.size(); }

private:
    bool LoadMonsterBook(const bool is_reload);

    MonsterBookContainer monster_book_container_;
    __DISABLE_COPY(MonsterBookParser);
};

#endif // _NA_007086_20140318_MONSTERBOOK
#endif // PROGRAMCOMMON_MONSTERBOOK_MONSTERBOOKPARSER_H_