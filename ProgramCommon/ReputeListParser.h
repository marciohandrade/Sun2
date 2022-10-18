#pragma once

#ifndef PROGRAMCOMMON_HONORSYSTEM_REPUTELISTPARSER_H_
#define PROGRAMCOMMON_HONORSYSTEM_REPUTELISTPARSER_H_

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include <Singleton.h>
#include <SolarHashTable.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

class ReputeListParser : public util::Singleton<ReputeListParser>, public IParser
{
public:
    enum
    {
        kMaxReputeRating = 3,
    };

    struct ReputeInfo
    {
        WORD repute_code; // �����ڵ�
        WORD repute_group_code; // ���Ǳ׷��ڵ�
        WORD repute_rating_point[kMaxReputeRating]; // ���� �ܰ躰 �ʿ�����
        WORD reward_fame_point[kMaxReputeRating]; // ���� �ܰ躰 �� ��������
        WORD fame_area_code; // ���� �� �ڵ�
        DWORD repute_rating_accumulate_point[kMaxReputeRating]; // ���� �ܰ���� �ʿ� ��������
#ifndef _SERVER //if'N'def
        DWORD repute_name_code; // ���� ����
        DWORD repute_group_name_code; // ���� �׷� ����
        DWORD repute_description_code; // ���� ����
        DWORD repute_effectcode[kMaxReputeRating]; // �ܰ躰 �޼� ����Ʈ
#endif //_SERVER
    };

    struct ReputeGroupInfo
    {
        WORD repute_group_code; // ���Ǳ׷��ڵ�
        DWORD repute_group_name_code; // ���� �׷� ����
        STLX_SET<WORD> repute_code_set;
    };

    typedef util::SolarHashTable<ReputeInfo*> ReputeInfoTable;
    typedef STLX_MAP<WORD, ReputeGroupInfo*> ReputeGroupInfoMap;

public:
    ReputeListParser(void);
    virtual	~ReputeListParser(void);

    void Init();
    void Init(char* pack_filename);
    void Release();

    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    ReputeInfo* GetReputeInfo(WORD repute_code);

    ReputeInfoTable& GetReputeInfoTable() { return repute_info_table_; }
    ReputeGroupInfoMap& GetReputeGroupInfoMap() { return repute_group_info_map_; };

    static int GetReputeRating(ReputeInfo* repute_info, DWORD current_repute_point);

private:
    BOOL _Load(BOOL is_reload);

    ReputeInfoTable repute_info_table_;
    ReputeGroupInfoMap repute_group_info_map_;
};

#endif //_NA_003027_20111013_HONOR_SYSTEM

#endif //PROGRAMCOMMON_HONORSYSTEM_REPUTELISTPARSER_H_