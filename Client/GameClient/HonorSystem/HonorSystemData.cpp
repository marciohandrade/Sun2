#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "../ProgramCommon/HonorSystem/HonorSystemPacket.h"
#include "HonorSystem/HonorSystem.h"
#include "FameListParser.h"
#include "ReputeListParser.h"

namespace HonorSystem
{
    HonorSystemData::HonorSystemData()
    {

    }

    HonorSystemData::~HonorSystemData()
    {
        title_code_contaioner().clear();
        fame_point_info_container().clear();
        repute_point_info_container().clear();
    }

    void HonorSystemData::AddTitleCode(WORD add_title_code)
    {
        title_code_contaioner().insert(add_title_code);
    }

    void HonorSystemData::RemoveTitleCode(WORD remove_title_code)
    {
        title_code_contaioner().erase(remove_title_code);
    }

    void HonorSystemData::RecvTitleList(HonorTitleTable& title_table)
    {
        title_code_contaioner().clear();
        
        DWORD title_count = title_table.count_;
        WORD title_code;
        for (DWORD table_index = 0; table_index < title_count; ++table_index)
        {
            title_code = title_table.title_[table_index].title_index_;
            title_code_contaioner().insert(title_code);
        }
    }

    bool HonorSystemData::IsHaveTitleCode(WORD remove_title_code)
    {
        TitleCodeContainer::iterator found_itr = title_code_contaioner().find(remove_title_code);
        if (found_itr == title_code_contaioner().end())
        {
            return false;
        }

        return true;
    }

    void HonorSystemData::ClearTitleCodeList()
    {
        title_code_contaioner().clear();
    }

    void HonorSystemData::InitializeHonorPointTable()
    {
        // ¸í¼º
        ClearFameReputePointTable();

        FamePointInfo new_fame_point_info;
        FameListParser::FameInfoMap& fame_info_container = FameListParser::Instance()->GetFameInfoContainer();
        FameListParser::FameInfoMap::iterator fame_info_itr = fame_info_container.begin();
        FameListParser::FameInfoMap::iterator fame_info_end_itr = fame_info_container.end();
        for ( ; fame_info_itr != fame_info_end_itr; ++fame_info_itr)
        {
            FameListParser::FameInfo* fame_info = fame_info_itr->second;
            if (fame_info == NULL)
            {
                continue;
            }

            new_fame_point_info.fame_code = fame_info->fame_code;
            new_fame_point_info.fame_point_value = 0;
            new_fame_point_info.fame_info = fame_info;

            fame_point_info_container().insert(FamePointInfoContainer::value_type(fame_info->fame_code, new_fame_point_info));
        }

        ReputePointInfo new_repute_point_info;
        ReputeListParser::ReputeInfoTable& repute_info_table = ReputeListParser::Instance()->GetReputeInfoTable();
        ReputeListParser::ReputeInfoTable::iterator repute_info_itr = repute_info_table.begin();
        ReputeListParser::ReputeInfoTable::iterator repute_info_end_itr = repute_info_table.end();
        for ( ; repute_info_itr != repute_info_end_itr; ++repute_info_itr)
        {
            ReputeListParser::ReputeInfo* repute_info = (*repute_info_itr);
            if (repute_info == NULL)
            {
                continue;
            }

            new_repute_point_info.repute_code = repute_info->repute_code;
            new_repute_point_info.repute_point_value = 0;
            new_repute_point_info.repute_info = repute_info;

            repute_point_info_container().insert(ReputePointInfoContainer::value_type(repute_info->repute_code, new_repute_point_info));
        }
    }

    void HonorSystemData::RecvFameReputePointTable(HonorPointTable& honor_point_table)
    {
        WORD honor_point_count = honor_point_table.count_;
        for (int point_info_index = 0; point_info_index < honor_point_count; ++point_info_index)
        {
            HONOR_POINT_INFO& honor_point_info = honor_point_table.point_table_[point_info_index];

            if (honor_point_info.type == HONOR_POINT_INFO::FAME)
            {
                ChangeFamePoint(honor_point_info.honor_point_code_, honor_point_info.honor_point_);
            }
            else if (honor_point_info.type == HONOR_POINT_INFO::REPUTE)
            {
                ChangeReputePoint(honor_point_info.honor_point_code_, honor_point_info.honor_point_);
            }
        }
    }

    void HonorSystemData::ChangeFamePoint(WORD fame_code, DWORD changed_fame_point)
    {
        FamePointInfoContainer::iterator found_itr = fame_point_info_container().find(fame_code);
        if (found_itr == fame_point_info_container().end())
        {
            return;
        }

        FamePointInfo& fame_point_info = found_itr->second;
        fame_point_info.fame_point_value = changed_fame_point;
    }

    void HonorSystemData::ChangeReputePoint(WORD repute_code, DWORD changed_repute_point)
    {
        ReputePointInfoContainer::iterator found_itr = repute_point_info_container().find(repute_code);
        if (found_itr == repute_point_info_container().end())
        {
            return;
        }

        ReputePointInfo& repute_point_info = found_itr->second;
        repute_point_info.repute_point_value = changed_repute_point;
    }

    void HonorSystemData::ClearFameReputePointTable()
    {
        fame_point_info_container().clear();
        repute_point_info_container().clear();
    }

    HonorSystem::FamePointInfo* HonorSystemData::GetFamePointInfo(WORD fame_code)
    {
        FamePointInfoContainer::iterator found_itr = fame_point_info_container().find(fame_code);
        if (found_itr == fame_point_info_container().end())
        {
            return NULL;
        }

        return &(found_itr->second);
    }

    HonorSystem::ReputePointInfo* HonorSystemData::GetReputePointInfo(WORD repute_code)
    {
        ReputePointInfoContainer::iterator found_itr = repute_point_info_container().find(repute_code);
        if (found_itr == repute_point_info_container().end())
        {
            return NULL;
        }

        return &(found_itr->second);
    }

};//namespace HonorSystem

#endif //_NA_003027_20111013_HONOR_SYSTEM
