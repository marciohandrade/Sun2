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

    BYTE index;                                          // �ε��� (����ܰ�)

    BYTE target_type;                                    // Ÿ�� Ÿ��
    WORD target_array[TARGET_SIZE];                      // Ÿ�� ����Ʈ
    WzID collect_id;                                     // ä����(���ι�)
    
    SLOTCODE reward_item_array[REWARD_ITEM_SIZE];        // �ܰ躰 ���������
    AttrOptionInfo reward_buff_option[REWARD_BUFF_SIZE]; // �ܰ躰 �������
    WORD reward_drop_heim_rate;                          // ���ӵ�� ����� ����

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