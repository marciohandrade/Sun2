#pragma once
#ifndef PROGRAMCOMMON_PERKINFOPARSER_H
#define PROGRAMCOMMON_PERKINFOPARSER_H

//==================================================================================================
/// PerkInfoParser class
/**
    @author
        waverix
    @since
        2010/07/28
    @remarks
    @purpose
    @history
    - 2010/07/28|waverix|created by _NA002217_100728_EP2_PERK_SYSTEM_
*/
//==================================================================================================

#include <SCSlotStruct.h>

//==================================================================================================
// declaration
//
struct PerkInfo; // declared in 'Struct.h'
class PerkInfoParser;

struct PerkGroup
{
    static const int kMaxNumberOfPerkInfos = PerkInfo::kMaxPerkLevel;
    SLOTCODE group_code_;
    int16_t order_;
    SLOTCODE item_code_;
    int number_of_perk_infos_;
    const PerkInfo* perk_info_array_[kMaxNumberOfPerkInfos];
    //
    const PerkInfo* GetPerkInfoByExp(EXPTYPE exp) const;
};

//==================================================================================================
// declaration
//
class PerkInfoParser : public IParser
{
public:
    //                 PerkID.uid_
    typedef STLX_HASH_MAP<ulong, const PerkInfo*> PerkTable;
    //                PerkID.group_
    typedef STLX_HASH_MAP<ulong, PerkGroup> GroupTable;
    //                 PerkID.uid_
    typedef STLX_HASH_MAP<ushort, const PerkInfo*> CashPerkTable;
    // interfaces for util::Singleton<>
    static PerkInfoParser* Instance();
    static void DestroyInstance();

    PerkInfoParser();
    ~PerkInfoParser();

    void Unload();
    void Init(char* pack_file_name);

    const PerkInfo* FindPerkInfo(const PerkID& perk_id) const;
    const PerkInfo* FindPerkInfo(const SLOTCODE& item_code) const;
    const PerkGroup* FindPerkGroup(const PerkID& perk_id) const;
    const PerkTable& GetPerkTable() const;
    const GroupTable& GetPerkGroupTable() const;
public:
    // IParser interfaces
    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);
private:
    typedef STLX_MAP<int, int> SORTED_GROUP;
    //
    static PerkInfoParser* CreateInstance();
    bool _Load();
    bool _BuildReference(int max_number_of_perk_infos);
    bool _BuildGroupInfo(SORTED_GROUP& sorted_group);
    //
    PerkTable perk_table_;
    GroupTable group_table_;
    CashPerkTable cash_perk_table_;
    PerkInfo* perk_info_array_;
    //
    static PerkInfoParser* perk_info_parser_static_;
    __DISABLE_COPY(PerkInfoParser);
};

//==================================================================================================
// inline implements

// interfaces for util::Singleton<>
inline PerkInfoParser* PerkInfoParser::Instance()
{
    if (perk_info_parser_static_) {
        return perk_info_parser_static_;
    }
    return CreateInstance();
}

inline const PerkInfo* PerkInfoParser::FindPerkInfo(const PerkID& perk_id) const
{
    PerkTable::const_iterator found = perk_table_.find(perk_id.uid_);
    if (found == perk_table_.end()) {
        return NULL;
    }
    const PerkInfo* perk_info = found->second;
    return perk_info;
}

inline const PerkInfo* PerkInfoParser::FindPerkInfo(const SLOTCODE& item_code) const
{
    CashPerkTable::const_iterator found = cash_perk_table_.find(item_code);
    if (found == cash_perk_table_.end()) {
        return NULL;
    }
    const PerkInfo* perk_info = found->second;
    return perk_info;
}

inline const PerkGroup* PerkInfoParser::FindPerkGroup(const PerkID& perk_id) const
{
    GroupTable::const_iterator found = group_table_.find(perk_id.group_);
    if (found == group_table_.end()) {
        return NULL;
    }
    const PerkGroup* perk_group = &found->second;
    return perk_group;
}

inline const PerkInfoParser::PerkTable& PerkInfoParser::GetPerkTable() const
{
    return perk_table_;
}

inline const PerkInfoParser::GroupTable& PerkInfoParser::GetPerkGroupTable() const
{
    return group_table_;
}

#endif //PROGRAMCOMMON_PERKINFOPARSER_H