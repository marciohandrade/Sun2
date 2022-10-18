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
        WORD repute_code; // 평판코드
        WORD repute_group_code; // 평판그룹코드
        WORD repute_rating_point[kMaxReputeRating]; // 평판 단계별 필요점수
        WORD reward_fame_point[kMaxReputeRating]; // 평판 단계별 명성 보상점수
        WORD fame_area_code; // 연관 명성 코드
        DWORD repute_rating_accumulate_point[kMaxReputeRating]; // 평판 단계까지 필요 누적점수
#ifndef _SERVER //if'N'def
        DWORD repute_name_code; // 평판 제목
        DWORD repute_group_name_code; // 평판 그룹 제목
        DWORD repute_description_code; // 평판 설명
        DWORD repute_effectcode[kMaxReputeRating]; // 단계별 달성 이펙트
#endif //_SERVER
    };

    struct ReputeGroupInfo
    {
        WORD repute_group_code; // 평판그룹코드
        DWORD repute_group_name_code; // 평판 그룹 제목
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