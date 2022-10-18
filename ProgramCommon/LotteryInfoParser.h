#pragma once
#ifndef PROGRAMCOMMON_LOTTERY_INFO_PARSER_H
#define PROGRAMCOMMON_LOTTERY_INFO_PARSER_H

//==================================================================================================
// @history
//  - 2008.04.28, aeoragie, create
//  - 2011.06.10, waverix, improvements

#include <SolarHashTable.h>
#include <ScriptCode.IParser.h>
#include <ItemOptionHeader.h>

//==================================================================================================

class MiniSlotInfo
{
public:
    MiniSlotInfo();
    bool operator==(const MiniSlotInfo& rhs) const;
    bool operator!=(const MiniSlotInfo& rhs) const;
    //
    BOOL Random;
    DWORD Index;
    DWORD ItemTypeIndex;
    //
    __DISABLE_COPY(MiniSlotInfo);
};

inline MiniSlotInfo::MiniSlotInfo() {
    ZeroMemory(this, sizeof(*this));
};

inline bool MiniSlotInfo::operator==(const MiniSlotInfo& rhs) const
{
    return ((Random == rhs.Random) &&
            (Index == rhs.Index) && 
            (ItemTypeIndex == rhs.ItemTypeIndex));
}

__forceinline bool MiniSlotInfo::operator!=(const MiniSlotInfo& rhs) const
{
    return !operator==(rhs);
}

//==================================================================================================

class LotteryIndexGroup
{
public:
    //LotteryIndexGroup(SLOTCODE group_index, int count);
    SLOTCODE GetGroupIndex() const;
    uint8_t GetMiniSlotCount() const;
    // CHANGES: f110610.1L, changes return value specification to acquire valid access pointer.
    const MiniSlotInfo* GetMiniSlotInfo(size_t pos) const;

private:
    bool AddMiniSlotInfo(const MiniSlotInfo* mini_slot);
    //
    SLOTCODE group_index_;
    uint8_t number_of_minislots_; // v <= MAX_PANDORA_SLOT_NUM
    const MiniSlotInfo* mini_slots_[MAX_PANDORA_SLOT_NUM];
    //
    friend class LotteryInfoParser;
    __DISABLE_COPY(LotteryIndexGroup);
};

inline SLOTCODE LotteryIndexGroup::GetGroupIndex() const {
    return group_index_;
}

inline uint8_t LotteryIndexGroup::GetMiniSlotCount() const {
    return number_of_minislots_;
}

// CHANGES: f110610.1L, changes return value specification to acquire valid access pointer.
inline const MiniSlotInfo* LotteryIndexGroup::GetMiniSlotInfo(size_t pos) const
{
    if (bool invalid = (number_of_minislots_ == 0) ||
                       (number_of_minislots_ < pos) ||
                       (number_of_minislots_ > MAX_PANDORA_SLOT_NUM))
    {
        return NULL;
    };
    return mini_slots_[pos];
}

//==================================================================================================

class LotteryInfoParser : public IParser
{
public:
    static LotteryInfoParser* Instance();
    static void DestroyInstance(); // old util::Singleton support interface
    // IParser interfaces...
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);
    virtual void Release();
    //
    //void Init(char * pszPackFileName);

    const LotteryIndexGroup* GetLotteryIndexGroup(SLOTCODE lottery_index) const;

private:
    typedef util::SolarHashTable<LotteryIndexGroup*, SLOTCODE> LOTTERY_TABLE;
    //
    LotteryInfoParser();
    virtual ~LotteryInfoParser();
    static LotteryInfoParser* CreateInstance();

    void Unload();
    bool ValidCheck(const LotteryIndexGroup& lottery_info,
                    MiniSlotInfo* const mini_slot_info);
    bool _Load(bool reloaded);
    //----------------------------------------------------------------------------------------------
    // fields...
    LOTTERY_TABLE lottery_table_;
    LotteryIndexGroup* lottery_group_array_;
    MiniSlotInfo* mini_slot_info_array_;
    // last order...
    static LotteryInfoParser* instance_static_;
    //
    __DISABLE_COPY(LotteryInfoParser);
    WAVERIX_TEST_BLOCK_IN_CLASS(LotteryInfoParser);
};

//==================================================================================================

inline LotteryInfoParser* LotteryInfoParser::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

inline const LotteryIndexGroup*
LotteryInfoParser::GetLotteryIndexGroup(SLOTCODE lottery_index) const
{
    const LotteryIndexGroup* lottery_info = lottery_table_.GetData(lottery_index);
    return lottery_info;
}

//==================================================================================================

#endif //PROGRAMCOMMON_LOTTERY_INFO_PARSER_H