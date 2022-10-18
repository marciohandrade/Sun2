#pragma once
#ifndef PROGRAMCOMMON_FATEITEMINFO_PARSER_H
#define PROGRAMCOMMON_FATEITEMINFO_PARSER_H

#include <Singleton.h>
#include <ScriptCode.IParser.h>
#include <ReliableRatioRandomizer.h>
#include <ReliableRatioRandomizerEX.hpp>

//==================================================================================================

#define MAX_FATE_DURA_RATIO_COUNT       4
#define MAX_FATE_ATK_DEF_RATIO_COUNT    4
// CHANGES: f110428.x, remove time specification about 365, and infinity
#define MAX_FATE_USING_TIME_RATIO_COUNT 3 // list = { 30老, 60老, 90老 }
#define MAX_FATE_SOCKET_COUNT           4   // 昏力 _NA_000251_20100727_SOCKET_SYSTEM


struct LUCKY_ITEM_INFO
{
    DWORD m_dwIndex;
    DWORD m_dwRefItemIndex;

    DWORD m_dwDuraRatio[MAX_FATE_DURA_RATIO_COUNT];
    WORD m_wDura_Min[MAX_FATE_DURA_RATIO_COUNT];
    WORD m_wDura_Max[MAX_FATE_DURA_RATIO_COUNT];
    DWORD m_dwFateDuraRatioSum;   

    DWORD m_dwAtkDefRatio[MAX_FATE_ATK_DEF_RATIO_COUNT];
    WORD m_wAtkDef_Min[MAX_FATE_ATK_DEF_RATIO_COUNT];
    WORD m_wAtkDef_Max[MAX_FATE_ATK_DEF_RATIO_COUNT];
    DWORD m_dwAtkDefRatioSum;

    DWORD m_dwUsingTimeRatio[MAX_FATE_USING_TIME_RATIO_COUNT];
    DWORD m_dwUsingTimeValue[MAX_FATE_USING_TIME_RATIO_COUNT];
    DWORD m_dwUsingTimeRatioSum;

    BOOL m_bCanUseEnchant;
    BOOL m_bCanUseSocket;

#if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
    DWORD m_dwSocketRatio[MAX_FATE_SOCKET_COUNT];
    DWORD m_dwSocketRatioSum;
#endif //
};

typedef const LUCKY_ITEM_INFO sFATE_ITEM_INFO;

//==================================================================================================

class FateItemInfoParser : public util::Singleton<FateItemInfoParser>, public IParser
{
public:
    FateItemInfoParser();
    virtual ~FateItemInfoParser();

    // IParser interfaces
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);
    virtual void Release();

    void Unload();

    sFATE_ITEM_INFO* GetFateItemInfoByIndex(DWORD lucky_index) const;
    sFATE_ITEM_INFO* GetFateItemInfoByRefItemCode(DWORD ref_item_code) const; 

    WORD GetRandomkeyFromFateDuraRatio(DWORD lucky_index);
    WORD GetRandomkeyFromFateAtkDefRatio(DWORD lucky_index);
    DWORD GetRandomkeyFromFateUsingTimeRatio(DWORD lucky_index);
#if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
    WORD GetRandomkeyFromFateSocketRatio(DWORD lucky_index);
#endif //

    void SetFirst() { fate_item_iterator_ = lucky_item_table_.begin(); }
    sFATE_ITEM_INFO* GetNext();

private:
    typedef STLX_HASH_MAP<DWORD, LUCKY_ITEM_INFO*> LUCKY_ITEM_INFO_HASH;
    typedef STLX_HASH_MAP<DWORD, ReliableRatioRandomizerEX<MAX_FATE_DURA_RATIO_COUNT>* >
        LUCKY_DURA_RATIO_HASH;
    typedef STLX_HASH_MAP<DWORD, ReliableRatioRandomizerEX<MAX_FATE_ATK_DEF_RATIO_COUNT>* >
        LUCKY_ATK_DEF_RATIO_HASH;
    typedef STLX_HASH_MAP<DWORD, ReliableRatioRandomizer<MAX_FATE_USING_TIME_RATIO_COUNT>* >
        LUCKY_USING_TIME_RATIO_HASH;
#if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
    typedef STLX_HASH_MAP<DWORD, ReliableRatioRandomizer<MAX_FATE_SOCKET_COUNT>* >
        LUCKY_SOCKET_RATIO_HASH;
#endif //

    WORD ValidCheck(LUCKY_ITEM_INFO* lucky_info);
    bool _Load(BOOL reloaded);

    LUCKY_ITEM_INFO_HASH lucky_item_table_;
    LUCKY_ITEM_INFO_HASH fate_item_ref_item_table_;
    LUCKY_ITEM_INFO_HASH::iterator fate_item_iterator_;

    LUCKY_DURA_RATIO_HASH fate_item_durability_selector_;
    LUCKY_ATK_DEF_RATIO_HASH fate_item_atkdef_selector_;

    LUCKY_USING_TIME_RATIO_HASH fate_item_time_selector_;
#if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
    LUCKY_SOCKET_RATIO_HASH fate_item_socket_selector_;
#endif
    //
    char debug_file_name_[MAX_PATH];
    //
    __DISABLE_COPY(FateItemInfoParser);
};

#endif //PROGRAMCOMMON_FATEITEMINFO_PARSER_H