#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "FameListParser.h"
#include "ReputeListParser.h"

namespace HonorSystem
{
    struct FamePointInfo 
    {
        WORD fame_code;
        DWORD fame_point_value;
        FameListParser::FameInfo* fame_info;
    };

    struct ReputePointInfo 
    {
        WORD repute_code;
        DWORD repute_point_value;
        ReputeListParser::ReputeInfo* repute_info;
    };

class HonorSystemData
{
    typedef STLX_SET<WORD> TitleCodeContainer;
    typedef STLX_MAP<WORD, FamePointInfo> FamePointInfoContainer;
    typedef STLX_MAP<WORD, ReputePointInfo> ReputePointInfoContainer;

public:
    HonorSystemData();
    ~HonorSystemData();

    //- 칭호
    void AddTitleCode(WORD add_title_code);

    void RemoveTitleCode(WORD remove_title_code);

    void RecvTitleList(HonorTitleTable& title_table);

    bool IsHaveTitleCode(WORD remove_title_code);

    void ClearTitleCodeList();

    //- 명성/평판
    void InitializeHonorPointTable();

    void RecvFameReputePointTable(HonorPointTable& honor_point_table);
    
    void ChangeFamePoint(WORD fame_code, DWORD changed_fame_point);

    void ChangeReputePoint(WORD repute_code, DWORD changed_repute_point);

    void ClearFameReputePointTable();

    HonorSystem::FamePointInfo* GetFamePointInfo(WORD fame_code);

    HonorSystem::ReputePointInfo* GetReputePointInfo(WORD repute_code);

private:
    TitleCodeContainer& title_code_contaioner() { return title_code_contaioner_; }
    FamePointInfoContainer& fame_point_info_container() { return fame_point_info_container_; }
    ReputePointInfoContainer& repute_point_info_container() { return repute_point_info_container_; }

    TitleCodeContainer title_code_contaioner_;
    FamePointInfoContainer fame_point_info_container_;
    ReputePointInfoContainer repute_point_info_container_;

    friend void HonorSystem::HonorSystemFunction::RefreshTitleListMenu();
    friend void HonorSystem::HonorSystemFunction::RefreshHonorPointListMenu();
};

};//namespace HonorSystem

#endif //_NA_003027_20111013_HONOR_SYSTEM