#pragma once

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>
#include <SolarHashTable.h>

struct SScheinRewardInfo
{
    enum eSONNENSCHIEIN_TARGET_TYPE{
        TARGET_TYPE_COLLECTION = 0,
        TARGET_TYPE_MONSTER,
        TARGET_TYPE_MAPOBJECT,
        TARGET_TYPE_MAX
    };
    enum {
        TARGET_SIZE = 8,
        REWARD_ITEM_SIZE = 5,
        REWARD_BUFF_SIZE = 5,
    };

    BYTE index;                                          // 인덱스 (보상단계)

    BYTE target_type;                                    // 타겟 타입
    WORD target_array[TARGET_SIZE];                      // 타겟 리스트
    WzID collect_id;                                     // 채집물(각인물)
    
    SLOTCODE reward_item_array[REWARD_ITEM_SIZE];        // 단계별 보상아이템
    AttrOptionInfo reward_buff_option[REWARD_BUFF_SIZE]; // 단계별 보상버프
    WORD reward_drop_heim_rate;                          // 하임드랍 상향률 보상

    SScheinRewardInfo()
    {
        ::ZeroMemory(this, sizeof(*this));
    }
};

class SonnenScheinRewardInfoParser : public util::Singleton<SonnenScheinRewardInfoParser>, public IParser
{
public:
    typedef STLX_HASH_MAP<BYTE, SScheinRewardInfo*> SScheinRewardHash;
    SonnenScheinRewardInfoParser() {}
    virtual ~SonnenScheinRewardInfoParser() {}

    VOID Init() {}
    VOID Init(char* file_name);

    virtual VOID Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    const SScheinRewardInfo* FindData(const BYTE reward_phase) const;

    SScheinRewardHash& GetSScheinRewardInfoHash() {return sschein_rewardinfo_hash_;}

    WORD GetDataSize() {return sschein_rewardinfo_hash_.size();}
private:
    SScheinRewardHash sschein_rewardinfo_hash_;

    BOOL _Load(BOOL is_reload);
    void _Unload();
};
#endif //_NA_008334_20150608_SONNENSCHEIN