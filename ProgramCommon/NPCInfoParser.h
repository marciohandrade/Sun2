#pragma once
#ifndef PROGRAMCOMMON_NPCINFOPARSER_H
#define PROGRAMCOMMON_NPCINFOPARSER_H

//==================================================================================================
// @history
//  ~10.06.05
//  10.06.06|waverix|fit in a coding style and changes a data structure related to the BASE_NPCINFO
//                   that unidimentional array has an O(1) time complexity about fixed size base key.
//
//==================================================================================================
#include <ScriptCode.IParser.h>
#include <SolarHashTable.h>
//#include <Singleton.h>
//#include <SolarFileReader.h>
//#include <vector>

struct BASE_NPCINFO;
struct EXTRA_NPCINFO;
//typedef util::SolarHashTable<BASE_NPCINFO *>::iterator  NPCINFO_ITERATOR;

//==================================================================================================

struct EXTRA_NPCINFO_SET
{
    typedef std::vector<EXTRA_NPCINFO*> Container;
    //
    EXTRA_NPCINFO_SET() { }
    ~EXTRA_NPCINFO_SET() { }

    Container::iterator GetBegin() {
        return extra_info_set_.begin();
    }
    Container::iterator GetEnd() {
        return extra_info_set_.end();
    }
    void Insert(EXTRA_NPCINFO* extra_info) {
        extra_info_set_.push_back(extra_info);
    }
    void Clear() {
        extra_info_set_.resize(0);
    }
    int GetSize() {
        static_cast<int>(extra_info_set_.size());
    }

    Container extra_info_set_; // Hash 에서 관리 되는것이라 지울 필요없음.
};

//==================================================================================================

class NPCInfoParser : public IParser
{
public:
    // interfaces for util::Singleton<>
    static NPCInfoParser* Instance();
    static void DestroyInstance();

    void Init(DWORD unused_arg1, DWORD unused_arg2 = 50);
    void Init(DWORD unused_arg1, char* pack_file_name, DWORD unused_arg2 = 50);
    // IParser interface implementation = { Init, Release, LoadScript }
    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);
    // (CHANGES) (f100606.2L) remove an unused section
    //void Reload();
    //void ReloadExtra();
    BASE_NPCINFO* GetNPCInfo(CODETYPE npc_code);
    const BASE_NPCINFO* GetNPCInfo(CODETYPE npc_code) const;
    EXTRA_NPCINFO_SET* GetExtraInfoSetFromFieldID(MAPCODE map_code,FIELDID field_id);
    EXTRA_NPCINFO*     GetExtraInfo(CODETYPE npc_code);
    // (f100606.2L) changed interfaces to support begin and end interface
    void SetFirst() const ;
    const BASE_NPCINFO* GetNext() const;
    //NPCINFO_ITERATOR Begin() { return npc_table_.begin(); }
    //NPCINFO_ITERATOR End()   { return npc_table_.end(); }

private:
    // (f100606.2L) add the item access facility which is supported an O(1) time complexity
    struct NpcTable {
        int number_of_total_npcs_;
        // iterator
        mutable const uint16_t* seq_total_it_;
        const uint16_t* seq_total_indexes_;
        const BASE_NPCINFO* seq_total_array_;
        BASE_NPCINFO* array_[USHRT_MAX];
    };
    //
    NPCInfoParser();
    ~NPCInfoParser();
    static NPCInfoParser* CreateInstance();

    BOOL _Load(BOOL reloaded);
    BOOL _LoadExtra(BOOL reloaded);
    bool _BuildReference(int added_number_of_nodes, bool is_extra_loading);
    bool _AddNpcInfo(CODETYPE npc_code, BASE_NPCINFO* npc_info);
    BOOL CheckValidity(BASE_NPCINFO* const npc_info);
    void Unload();
    void UnLoadExtra();

private:
    //----------------------------------------------------------------------------------------------
    // data fields...
    bool loaded_;
    util::SolarHashTable<EXTRA_NPCINFO*>* extra_npc_table_;
    // sequential stream array to support an extra npc information
    EXTRA_NPCINFO* extra_npc_info_array_;
    EXTRA_NPCINFO_SET extra_npc_info_set_;
    //util::SolarHashTable<BASE_NPCINFO*>   npc_table_;
    NpcTable npc_table_;
    //
    static NPCInfoParser* npc_info_parser_static_;
};


// interfaces for util::Singleton<>
inline NPCInfoParser* NPCInfoParser::Instance()
{
    if (npc_info_parser_static_) {
        return npc_info_parser_static_;
    }
    return CreateInstance();
}

inline BASE_NPCINFO* NPCInfoParser::GetNPCInfo(CODETYPE npc_code)
{
    // (CHANGES) (f100606.2L) changes a search algorithm
    if (npc_code == 0 || npc_code > USHRT_MAX) {
        return NULL;
    }
    --npc_code;
    BASE_NPCINFO* npc_info = npc_table_.array_[npc_code];
    return npc_info;
}

inline const BASE_NPCINFO* NPCInfoParser::GetNPCInfo(CODETYPE npc_code) const
{
    // (CHANGES) (f100606.2L) changes a search algorithm
    if (npc_code == 0 || npc_code > USHRT_MAX) {
        return NULL;
    }
    --npc_code;
    const BASE_NPCINFO* npc_info = npc_table_.array_[npc_code];
    return npc_info;
}

//NPCINFO_ITERATOR NPCInfoParser::Begin() { return npc_table_.begin(); }
//NPCINFO_ITERATOR NPCInfoParser::End()   { return npc_table_.end(); }

// changed interfaces to support begin and end interface
inline void NPCInfoParser::SetFirst() const
{
    npc_table_.seq_total_it_ = npc_table_.seq_total_indexes_;
}

// changed interfaces to support begin and end interface
inline const BASE_NPCINFO* NPCInfoParser::GetNext() const
{
    const uint16_t* end = npc_table_.seq_total_indexes_ + npc_table_.number_of_total_npcs_;
    if (npc_table_.seq_total_it_ >= end ||
        npc_table_.seq_total_it_ < npc_table_.seq_total_indexes_)
    {
        return NULL;
    }
    uint16_t index = *npc_table_.seq_total_it_++; // change iterator
    assert(npc_table_.array_[index]);
    return npc_table_.array_[index];
}

inline EXTRA_NPCINFO* NPCInfoParser::GetExtraInfo(CODETYPE npc_code)
{
    return extra_npc_table_->GetData(npc_code);
}

#endif //PROGRAMCOMMON_NPCINFOPARSER_H