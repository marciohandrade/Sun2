#pragma once
#ifndef UTILITY_LINKED_LIST_CSTYLE_H
#define UTILITY_LINKED_LIST_CSTYLE_H

//==================================================================================================
//
namespace util {
;
#pragma pack(push)
#pragma pack()
//
// C Style Linked List
//   <WARNING!> 사용방법이 익숙해지기 전까지는 사용하면 위험!
//

struct __LListBase {};

template<class T>
struct __LList : public util::__LListBase
{
    T* next;
    T* prev;
};

template<>
struct __LList<void> : public util::__LListBase
{
    __LList<void>* next;
    __LList<void>* prev;
};

typedef struct util::__LList<void>* PListNode;


struct LList
{
    static void Init(__LListBase* item);
    static bool IsEmpty(const __LListBase* item);
    static void AddNext(__LListBase* item, __LListBase* entry);
    static void AddPrev(__LListBase* item, __LListBase* entry);

    template<typename _TYPE_>
    static _TYPE_* Delete(_TYPE_* delitem);
    //
private:
    static void __Insert(__LListBase* item, __LListBase* prev, __LListBase* next);
    static void __Delete(__LListBase* prev, __LListBase* next);
};

#pragma pack(pop)
}; //end of namespace
//==================================================================================================
//
#define LC_RawLCast(__node__) static_cast<util::__LList<void>*>(__node__)

__forceinline void util::LList::__Insert(
    util::__LListBase* item, util::__LListBase* prev, util::__LListBase* next)
{
    LC_RawLCast(next)->prev  = LC_RawLCast(item);
    LC_RawLCast(item)->next  = LC_RawLCast(next);
    LC_RawLCast(item)->prev  = LC_RawLCast(prev);
    LC_RawLCast(prev)->next  = LC_RawLCast(item);
}

__forceinline void util::LList::__Delete(util::__LListBase* prev, util::__LListBase* next)
{
    LC_RawLCast(next)->prev = LC_RawLCast(prev);
    LC_RawLCast(prev)->next = LC_RawLCast(next);
}

__forceinline void util::LList::Init(util::__LListBase* item)
{
    LC_RawLCast(item)->next = LC_RawLCast(item);
    LC_RawLCast(item)->prev = LC_RawLCast(item);
}

__forceinline bool util::LList::IsEmpty(const util::__LListBase* item)
{
    return static_cast<const util::__LList<void>*>(item)->next == item;
}

__forceinline void util::LList::AddNext(util::__LListBase* item, util::__LListBase* entry)
{
    __Insert(item, entry, LC_RawLCast(entry)->next);
}

__forceinline void util::LList::AddPrev(util::__LListBase* item, util::__LListBase* entry)
{
    __Insert(item, LC_RawLCast(entry)->prev, entry);
}

template<typename _TYPE_>
_TYPE_* util::LList::Delete(_TYPE_* del_item)
{
    util::__LListBase* next_item = del_item->next;
    util::LList::__Delete(del_item->prev, next_item);
    Init(del_item);
    return static_cast<_TYPE_*>(next_item);
}

#undef LC_RawLCast
//==================================================================================================

#endif //UTILITY_LINKED_LIST_CSTYLE_H