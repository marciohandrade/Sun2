#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "ObjectManager.h"
#include "Hero.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "InterfaceManager.h"
#include "GuildSystem/uiGuildJoinRequestList.h"
#include "GuildFacilityImageParser.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "GlobalFunc.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

namespace GuildSystem{

    GuildSystemData guild_system_data;
    GuildSystemData& GetGuildSystemData()
    {
        return guild_system_data;
    }

    // 길드원 정렬 함수
    template <class T, T GUILD_MEMBER_INFO::*p>
        bool sortGuildMemberByTypeHigh(GUILD_MEMBER_INFO & lhw, GUILD_MEMBER_INFO& rhw)
    {
        return lhw.*p < rhw.*p;
    }

    template <class T, T GUILD_MEMBER_INFO::*p>
        bool sortGuildMemberByTypeLow(GUILD_MEMBER_INFO & lhw, GUILD_MEMBER_INFO& rhw)
    {
        return lhw.*p > rhw.*p;
    }

    //! vs2008 에서는
    //! 기본 클래스의 멤버 포인터에서 파생 클래스의 멤버 포인터로의 표준 변환은 템플릿 인수에 적용되지 않습니다.
    template <class T, T BASE_GUILD_MEMBER_INFO::*p>
        bool sortGuildMemberByTypeHigh_Base(GUILD_MEMBER_INFO & lhw, GUILD_MEMBER_INFO& rhw)
    {
        return lhw.*p < rhw.*p;
    }

    template <class T, T BASE_GUILD_MEMBER_INFO::*p>
        bool sortGuildMemberByTypeLow_Base(GUILD_MEMBER_INFO & lhw, GUILD_MEMBER_INFO& rhw)
    {
        return lhw.*p > rhw.*p;
    }

    //스트링 StrCmp이용한 정렬
    bool sortGuildMemberByNameHigh(GUILD_MEMBER_INFO & lhw, GUILD_MEMBER_INFO & rhw)
    {
        int returnValue =  StrnCmp(lhw.m_tszCharName , rhw.m_tszCharName, MAX_CHARNAME_LENGTH);
        if( returnValue < 0 )
            return true;
        else
            return false;
    }

    bool sortGuildMemberByNameLow(GUILD_MEMBER_INFO & lhw, GUILD_MEMBER_INFO & rhw)
    {
        int returnValue =  StrnCmp(lhw.m_tszCharName , rhw.m_tszCharName, MAX_CHARNAME_LENGTH);
        if( returnValue > 0 )
            return true;
        else
            return false;
    }

    GuildSystemData::GuildSystemData()
    {
        
    }

    GuildSystemData::~GuildSystemData()
    {

    }

    void GuildSystemData::Clear()
    {
        set_recv_guild_info(false);
        set_recv_total_info(false);
        
        ZeroMemory(&GetGuildInfo(), sizeof(GetGuildInfo()));
        guild_member_container().clear();
        SetGuildName("");
        SetMasterName("");

        if (g_pHero != NULL)
        {
            g_pHero->ClearGuildRenderInfo();
        }

        ClearGuildRelation();
        wareshoue_deposit_log().clear();
        wareshoue_withdraw_log().clear();
        guild_log_info_container().clear();

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        guild_join_request_info_container().clear();
        guild_facility_container().clear();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        verify_dialog_key_ = 0;
    }

    // 기본 길드 정보
    void GuildSystemData::SetGuildInfo(GUILD_INFO& guild_info)
    {
        set_recv_guild_info(true);
        GetGuildInfo() = guild_info;
        SetGuildName(guild_info.m_tszGuildName);

        if (g_pHero != NULL)
        {
            g_pHero->GetGuildRenderInfo().m_BackGroundIndex = guild_info.m_BackGroundIdx;
            g_pHero->GetGuildRenderInfo().m_PatternIndex = guild_info.m_PatternIdx;
            g_pHero->GetGuildRenderInfo().m_GuildGuid = guild_info.m_GuildGuid;
            g_pHero->GetGuildRenderInfo().m_GuildGrade = guild_info.m_GuildGrade;
        }
    }

    // 길드원 정보
    void GuildSystemData::SetGuildMemberInfo(GUILD_MEMBER_TOTAL_INFO& guild_total_member_info)
    {
        set_recv_total_info(true);

        guild_member_container().clear();

        for (int index = 0; index < guild_total_member_info.m_byMemberCount; ++index)
        {
            GUILD_MEMBER_INFO& guild_member_info = guild_total_member_info.m_MemberInfo[index];

            guild_member_container().push_back(guild_member_info);
            if (guild_member_info.m_eGuildDuty == eGUILD_DUTY_MASTER)
            {
                SetMasterName(guild_member_info.m_tszCharName);
            }

            Player* guild_member = 
                static_cast<Player*>(g_ObjectManager.GetObject(guild_member_info.m_tszCharName));
            if (guild_member != NULL)
            {
                guild_member->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_ALLIENCE;
                if (guild_member == g_pHero)
                {
                    guild_member->GetGuildRenderInfo().m_GuildDuty = 
                        guild_member_info.m_eGuildDuty;
                }
            }
        }

        // 길드원 기본 정렬
        sort(guild_member_container().begin(), 
            guild_member_container().end(), 
            sortGuildMemberByNameHigh);
        sort(guild_member_container().begin(), 
            guild_member_container().end(), 
            sortGuildMemberByTypeLow<LEVELTYPE , &GUILD_MEMBER_INFO::m_Level >);
        sort(guild_member_container().begin(), 
            guild_member_container().end(), 
            sortGuildMemberByTypeLow<DWORD , &GUILD_MEMBER_INFO::guild_coin_ >);
    }

    void GuildSystemData::AddGuildMemberInfo( GUILD_MEMBER_INFO& add_member_info )
    {
        guild_member_container().push_back(add_member_info);

        Player* add_member = 
            static_cast<Player*>(g_ObjectManager.GetObject(add_member_info.m_tszCharName));
        if (add_member != NULL)
        {
            add_member->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_ALLIENCE;
        }
    }

    void GuildSystemData::DeleteGuildMemberInfo(TCHAR* member_name)
    {
        if (member_name == NULL)
        {
            return;
        }

        GuildMemberContainer::iterator found_itr = guild_member_container().begin();
        GuildMemberContainer::iterator end_itr = guild_member_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            if(StrnCmp(member_name, (*found_itr).m_tszCharName, MAX_CHARNAME_LENGTH) == 0)
            {
                guild_member_container().erase(found_itr);
                break;
            }
        }
    }

    GUILD_MEMBER_INFO* GuildSystemData::GetGuildMemberInfo(const TCHAR* member_name)
    {
        GUILD_MEMBER_INFO* member_info = NULL;

        GuildMemberContainer::iterator found_itr = guild_member_container().begin();
        GuildMemberContainer::iterator end_itr = guild_member_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            if (StrnCmp(member_name, (*found_itr).m_tszCharName, MAX_CHARNAME_LENGTH) == 0)
            {
                member_info = &(*found_itr);
                break;
            }
        }

        return member_info;
    }

    GUILD_MEMBER_INFO* GuildSystemData::GetGuildMemberInfo( CHARGUID member_guid )
    {
        GUILD_MEMBER_INFO* member_info = NULL;

        GuildMemberContainer::iterator found_itr = guild_member_container().begin();
        GuildMemberContainer::iterator end_itr = guild_member_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            if (member_guid == (*found_itr).m_CharGuid)
            {
                member_info = &(*found_itr);
                break;
            }
        }

        return member_info;
    }

    void GuildSystemData::SortGuildMember( GuildMemberSortType sort_type )
    {
        static bool high_case = true; // default 오름 차순
        static bool low_case = false; // default 내림 차순


        // 다른 소팅 탭 눌렀을 때, 기본 소팅 방식으로 셋팅되도록 함.
        static GuildMemberSortType prev_sort_type = kMaxType;
        if( prev_sort_type != sort_type )
        {
            high_case = true;
            low_case = false;
        }
        prev_sort_type = sort_type;

        switch (sort_type)
        {
        case kSortByName:
            {
                if(high_case )
                {
                    sort(guild_member_container().begin(),guild_member_container().end(), sortGuildMemberByNameHigh);
                }
                else
                {
                    sort(guild_member_container().begin(),guild_member_container().end(), sortGuildMemberByNameLow);
                }
                high_case ^= 1;
            }
            break;

        case kSortByLevel:
            {
                if( low_case )
                {
                    sort(guild_member_container().begin(),
                        guild_member_container().end(), 
                        sortGuildMemberByTypeHigh<LEVELTYPE , &GUILD_MEMBER_INFO::m_Level >);
                }
                else
                {
                    sort(guild_member_container().begin(),
                        guild_member_container().end(), 
                        sortGuildMemberByTypeLow<LEVELTYPE , &GUILD_MEMBER_INFO::m_Level >);
                }

                low_case ^= 1;
            }
            break;

        case kSortByClass:
            {
                if( high_case )
                {
                    sort(guild_member_container().begin(), 
                        guild_member_container().end(), 
                        sortGuildMemberByTypeHigh<BYTE, &GUILD_MEMBER_INFO::m_byClass>);	
                }
                else
                {
                    sort(guild_member_container().begin(), 
                        guild_member_container().end(), 
                        sortGuildMemberByTypeLow<BYTE, &GUILD_MEMBER_INFO::m_byClass>);	
                }
                high_case ^= 1;
            }
            break;

        case kSortByDuty:
            {
                if( high_case )
                {
                    sort(guild_member_container().begin(), 
                        guild_member_container().end(), 
                        sortGuildMemberByTypeHigh_Base<eGUILD_DUTY, &GUILD_MEMBER_INFO::m_eGuildDuty>);
                }
                else
                {
                    sort(guild_member_container().begin(), 
                        guild_member_container().end(), 
                        sortGuildMemberByTypeLow_Base<eGUILD_DUTY, &GUILD_MEMBER_INFO::m_eGuildDuty>);
                }
                high_case ^= 1;	
            }
            break;

        case kSortByGuildCoin:
            {
                if( low_case )
                {
                    sort(guild_member_container().begin(),
                        guild_member_container().end(), 
                        sortGuildMemberByTypeHigh<DWORD , &GUILD_MEMBER_INFO::guild_coin_ >);
                }
                else
                {
                    sort(guild_member_container().begin(),
                        guild_member_container().end(), 
                        sortGuildMemberByTypeLow<DWORD , &GUILD_MEMBER_INFO::guild_coin_ >);
                }

                high_case ^= 1;
            }
            break;

        case kSortByLogin:
            {
                if( low_case )
                {
                    sort( guild_member_container().begin(), guild_member_container().end(), sortGuildMemberByTypeHigh<BOOL, &GUILD_MEMBER_INFO::m_bLogin>);
                }
                else
                {
                    sort( guild_member_container().begin(), guild_member_container().end(), sortGuildMemberByTypeLow<BOOL, &GUILD_MEMBER_INFO::m_bLogin>);
                }
                low_case ^= 1;	
            }
            break;
        }
    }

    // 길드명
    void GuildSystemData::SetGuildName(const char* guild_name)
    {
        guild_name_ = guild_name;
        if (g_pHero != NULL)
        {
            g_pHero->SetGuildName(const_cast<char*>(guild_name), MAX_GUILDNAME_LENGTH);
        }
        StrnCopy(GetGuildInfo().m_tszGuildName, guild_name, MAX_GUILDNAME_LENGTH);
        GetGuildInfo().m_tszGuildName[_countof(GetGuildInfo().m_tszGuildName) - 1] = '\0';
    }

    void GuildSystemData::SetMasterName( const char* master_name )
    {
        master_name_ = master_name;
    }

    // 길드마크 텍스쳐
    void GuildSystemData::LoadGuildMarkTexture()
    {
        guild_mark_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(
            "Data\\Interface\\all_GuildMark.tga", 
            TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    }

    HANDLE GuildSystemData::GetGuildMarkTexture()
    {
        return guild_mark_texture_;
    }

    // 길드 관계
    void GuildSystemData::SetGuildRelationInfo( BASE_GUILD_ADD_INFO& guild_add_info )
    {
        int total_count = guild_add_info.GetTotalSize();
        for (int info_index = 0; info_index < total_count; ++info_index)
        {
            GUILD_RELATION& guild_relation = guild_add_info.m_stRelationGuild[info_index];
            GuildSystem::Function::SetPlayerGuildRelation(
                guild_relation.m_GuildGuid, 
                guild_relation.m_eRelationType);

            switch (guild_relation.m_eRelationType)
            {
            case eGUILD_RELATION_ALLIENCE:
                {
                    alliance_container().push_back(guild_relation);
                }
                break;
            case eGUILD_RELATION_BOTH_HOSTILITY:
                {
                    hostility_container().push_back(guild_relation);
                }
                break;
            case eGUILD_RELATION_PROCLAIM_HOSTILITY:
            case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
                {
                    wait_hostility_container().push_back(guild_relation);
                }
                break;
            }
        }
    }

    void GuildSystemData::AddGuildRelation( GUILD_RELATION& guild_relation )
    {
        GuildSystem::Function::SetPlayerGuildRelation(guild_relation.m_GuildGuid, 
            guild_relation.m_eRelationType);

        switch (guild_relation.m_eRelationType)
        {
        case eGUILD_RELATION_ALLIENCE:
            {
                alliance_container().push_back(guild_relation);
            }
            break;
        case eGUILD_RELATION_BOTH_HOSTILITY:
            {
                hostility_container().push_back(guild_relation);
            }
            break;
        case eGUILD_RELATION_PROCLAIM_HOSTILITY:
        case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
            {
                wait_hostility_container().push_back(guild_relation);
            }
            break;
        }
    }

    void GuildSystemData::DeleteGuildRelation( GUILD_RELATION& guild_relation )
    {
        DeleteGuildRelation(guild_relation.m_GuildGuid, guild_relation.m_eRelationType);
    }

    void GuildSystemData::DeleteGuildRelation( GUILDGUID guild_guid, eGUILD_RELATION guild_relation )
    {
        GuildRelationContainer* relation_container = NULL;
        switch (guild_relation)
        {
        case eGUILD_RELATION_ALLIENCE:
            {
                relation_container = &alliance_container();
            }
            break;
        case eGUILD_RELATION_PROCLAIM_HOSTILITY:
        case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
            {
                relation_container = &wait_hostility_container();
            }
            break;
        case eGUILD_RELATION_BOTH_HOSTILITY:
        case eGUILD_RELATION_CANCEL_BOTH_HOSTILITY:
            {
                relation_container = &hostility_container();
            }
            break;
        }

        if (relation_container == NULL)
        {
            return;
        }

        GuildRelationContainer::iterator found_itr = relation_container->begin();
        GuildRelationContainer::iterator end_itr = relation_container->end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (guild_relation.m_GuildGuid == guild_guid)
            {
                relation_container->erase(found_itr);
                break;
            }
        }

        GuildSystem::Function::SetPlayerGuildRelation(guild_guid, eGUILD_RELATION_NONE);
    }

    GUILD_RELATION* GuildSystemData::GetGuildRelation( GUILDGUID guild_guid )
    {
        GuildRelationContainer::iterator found_itr = alliance_container().begin();
        GuildRelationContainer::iterator end_itr = alliance_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (guild_relation.m_GuildGuid == guild_guid)
            {
                return &guild_relation;
            }
        }

        found_itr = hostility_container().begin();
        end_itr = hostility_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (guild_relation.m_GuildGuid == guild_guid)
            {
                return &guild_relation;
            }
        }

        found_itr = wait_hostility_container().begin();
        end_itr = wait_hostility_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (guild_relation.m_GuildGuid == guild_guid)
            {
                return &guild_relation;
            }
        }

        return NULL;
    }

    GUILD_RELATION* GuildSystemData::GetGuildRelation( const TCHAR* guild_name )
    {
        GuildRelationContainer::iterator found_itr = alliance_container().begin();
        GuildRelationContainer::iterator end_itr = alliance_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (StrnCmp(guild_name, guild_relation.m_GuildName, MAX_CHARNAME_LENGTH) == 0)
            {
                return &guild_relation;
            }
        }

        found_itr = hostility_container().begin();
        end_itr = hostility_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (StrnCmp(guild_name, guild_relation.m_GuildName, MAX_CHARNAME_LENGTH) == 0)
            {
                return &guild_relation;
            }
        }

        found_itr = wait_hostility_container().begin();
        end_itr = wait_hostility_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (StrnCmp(guild_name, guild_relation.m_GuildName, MAX_CHARNAME_LENGTH) == 0)
            {
                return &guild_relation;
            }
        }

        return NULL;
    }

    void GuildSystemData::GetRelationGuildName( GUILDGUID guild_guid, TCHAR* OUT guild_name )
    {
        GuildRelationContainer::iterator found_itr = alliance_container().begin();
        GuildRelationContainer::iterator end_itr = alliance_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (guild_relation.m_GuildGuid == guild_guid)
            {
                StrnCopy(guild_name, guild_relation.m_GuildName, MAX_CHARNAME_LENGTH);
                guild_name[_countof(guild_relation.m_GuildName) - 1] = '\0';
                return;
            }
        }

        found_itr = hostility_container().begin();
        end_itr = hostility_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (guild_relation.m_GuildGuid == guild_guid)
            {
                StrnCopy(guild_name, guild_relation.m_GuildName, MAX_CHARNAME_LENGTH);
                guild_name[_countof(guild_relation.m_GuildName) - 1] = '\0';
                return;
            }
        }

        found_itr = wait_hostility_container().begin();
        end_itr = wait_hostility_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (guild_relation.m_GuildGuid == guild_guid)
            {
                StrnCopy(guild_name, guild_relation.m_GuildName, MAX_CHARNAME_LENGTH);
                guild_name[_countof(guild_relation.m_GuildName) - 1] = '\0';
                return;
            }
        }
    }

    void GuildSystemData::ClearGuildRelation()
    {
        alliance_container().clear();
        hostility_container().clear();
        wait_hostility_container().clear();
    }

    GUILDGUID GuildSystemData::GetGuildGUID( const TCHAR* guild_name )
    {
        GuildRelationContainer::iterator found_itr = alliance_container().begin();
        GuildRelationContainer::iterator end_itr = alliance_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (StrnCmp(guild_name, guild_relation.m_GuildName, MAX_CHARNAME_LENGTH) == 0)
            {
                return guild_relation.m_GuildGuid;
            }
        }

        found_itr = hostility_container().begin();
        end_itr = hostility_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (StrnCmp(guild_name, guild_relation.m_GuildName, MAX_CHARNAME_LENGTH) == 0)
            {
                return guild_relation.m_GuildGuid;
            }
        }

        found_itr = wait_hostility_container().begin();
        end_itr = wait_hostility_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (StrnCmp(guild_name, guild_relation.m_GuildName, MAX_CHARNAME_LENGTH) == 0)
            {
                return guild_relation.m_GuildGuid;
            }
        }

        return -1;
    }

    bool GuildSystemData::IsHaveGuildRelation()
    {
        if ((alliance_container().size() > 0) || 
            (hostility_container().size() > 0) || 
            (wait_hostility_container().size() > 0))
        {
            return true;
        }

        return false;
    }


    int GuildSystemData::GetRelationProclaimCount()
    {
        int proclaim_count = 0;
        GuildRelationContainer::iterator found_itr = hostility_container().begin();
        GuildRelationContainer::iterator end_itr = hostility_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if ((guild_relation.m_eRelationType == eGUILD_RELATION_BOTH_HOSTILITY) &&
                (guild_relation.m_eProClaim == PROCLAIM_GUILD))
            {
                ++proclaim_count;
            }
        }

        found_itr = wait_hostility_container().begin();
        end_itr = wait_hostility_container().end();
        for ( ; found_itr != end_itr; ++found_itr)
        {
            GUILD_RELATION& guild_relation = (*found_itr);
            if (guild_relation.m_eRelationType == eGUILD_RELATION_PROCLAIM_HOSTILITY)
            {
                ++proclaim_count;
            }
        }

        return proclaim_count;
    }

    void GuildSystemData::ClearWareHouseLog()
    {
        wareshoue_deposit_log().clear();
        wareshoue_withdraw_log().clear();
    }

    void GuildSystemData::AddWareHouseLog(STRUCT_GUILDWAREHOUSE_LOGINFO* warehouse_log_info, 
                                          DWORD num_of_log)
    {
        const DWORD kMaxLogNumber = (MAX_GUILDWAREHOUSE_LOGLIST_NUM / 2);

        for (DWORD log_index = 0; log_index < num_of_log; ++log_index)
        {
            STRUCT_GUILDWAREHOUSE_LOGINFO& log_info = warehouse_log_info[log_index];
            if ((log_info.m_LogType == MONEY_IN) || 
                (log_info.m_LogType == ITEM_IN))
            {
                wareshoue_deposit_log().push_back(log_info);

                if (wareshoue_deposit_log().size() > kMaxLogNumber)
                {
                    wareshoue_deposit_log().pop_front();
                }
            }
            else if ((log_info.m_LogType == MONEY_OUT) || 
                (log_info.m_LogType == ITEM_OUT))
            {
                wareshoue_withdraw_log().push_back(log_info);
                if (wareshoue_withdraw_log().size() > kMaxLogNumber)
                {
                    wareshoue_withdraw_log().pop_front();
                }
            }
        }
    }

    void GuildSystemData::ClearGuildLog()
    {
        guild_log_info_container().clear();
    }

    void GuildSystemData::AddGuildLog( DWORD guild_log_category, GuildInGameLogData* guild_log_data, DWORD num_of_log )
    {
        GuildLogInfo guild_log_info;
        guild_log_info.guild_log_category = guild_log_category;
        for (DWORD log_index = 0; log_index < num_of_log; ++log_index)
        {
            guild_log_info.guild_log_data = *(guild_log_data + log_index);
            guild_log_info_container().insert(
                GuildLogInfoContainer::value_type(guild_log_info.guild_log_data.log_time_, guild_log_info));
        }
    }

    void GuildSystemData::AddGuildRankingInfo( GUILD_RANKING_INFO* guild_ranking_info, int num_of_info )
    {
        guild_ranking_info_container().clear();

        for (int info_index = 0; info_index < num_of_info; ++info_index)
        {
            GUILD_RANKING_INFO& ranking_info = *(guild_ranking_info + info_index);
            guild_ranking_info_container().push_back(ranking_info);
        }

        //if (num_of_info > 0)
        //{
        //    GuildSystem::Function::ShowGuildRanking();
        //}
    }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void GuildSystemData::set_guild_join_request_total_info_( GUILD_JOIN_REQUEST_TOTAL_INFO* guild_join_request_total_info )
    {
        if (!guild_join_request_total_info)
            return;

        guild_join_request_info_container().clear();

        for (int i = 0; i < MAX_GUILD_JOIN_REQUEST_NUM; ++i)
        {
            GUILD_JOIN_REQUEST_INFO info = guild_join_request_total_info->m_GuildJoinRequestInfo[i];
            if (info.m_char_guid > 0)
            {
                guild_join_request_info_container().push_back(info);
            }
        }
    }

    bool GuildSystemData::get_guildname_for_guild_join_request( const GUILDGUID& guild_guid, OUT TCHAR* guild_name )
    {        
        GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer::iterator found_itr;
        
        GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer& guild_ranking_guild_info_container = 
            GuildSystem::GetGuildSystemData().guild_ranking_guild_info_container();
        found_itr = guild_ranking_guild_info_container.find(guild_guid);
        if (found_itr == guild_ranking_guild_info_container.end())
            return false;

        GuildSystem::GuildSystemData::GuildInfoForClient& guild_info = found_itr->second;
        Snprintf(guild_name, MAX_GUILDNAME_LENGTH+1, guild_info.game_guild_info_.m_tszGuildName);

        return true;
    }

    void GuildSystemData::set_guild_facility_container(const GUILD_FACILITY_TOTAL_INFO& facility_total_info)
    {
        guild_facility_container().clear();
        init_guild_facility_container();
        GuildFacilityContainer::iterator found_itr;
        for (int i = 0; i < facility_total_info.m_count; ++i)
        {
            found_itr = guild_facility_container().find(facility_total_info.m_GuildFacilityInfo[i].code);
            if (found_itr == guild_facility_container().end())
            {
                GuildFacilityInfoForClient facility_data;
                facility_data.facility_code_ = facility_total_info.m_GuildFacilityInfo[i].code;
                facility_data.facility_level_ = facility_total_info.m_GuildFacilityInfo[i].level;
                guild_facility_container().insert(
                    GuildFacilityContainer::value_type(facility_data.facility_code_, facility_data));
            }
            else
            {
                found_itr->second.facility_code_ = facility_total_info.m_GuildFacilityInfo[i].code;
                found_itr->second.facility_level_ = facility_total_info.m_GuildFacilityInfo[i].level;
            }
        }
        if (g_pHero)
        {
            g_pHero->UpdateSkillAttr();
        }        
    }

    bool GuildSystemData::get_guild_facility_info( const FCODE& facility_code, OUT GuildFacilityInfoForClient& facility_info )
    {
        int container_size = GuildSystem::GetGuildSystemData().guild_facility_container().size();
        if (container_size <= 0)
        {
            init_guild_facility_container();
        }

        GuildSystem::GuildSystemData::GuildFacilityContainer::iterator found_itr;

        GuildSystem::GuildSystemData::GuildFacilityContainer& guild_facility_container = 
            GuildSystem::GetGuildSystemData().guild_facility_container();
        found_itr = guild_facility_container.find(facility_code);
        if (found_itr == guild_facility_container.end())
            return false;

        facility_info.facility_code_ = (found_itr->second).facility_code_;
        facility_info.facility_level_ = (found_itr->second).facility_level_;
        facility_info.facility_active_ = (found_itr->second).facility_active_;

        return true;
    }

    void GuildSystemData::init_guild_facility_container()
    {
        guild_facility_container().clear();
        HASH_TABLE_GUILD_FACILITY_IMAGE_INFO* data_hash_table = 
            GuildFacilityImageParser::Instance()->GetHashTable();
        if (data_hash_table == NULL)
            return;

        BASE_GuildFacilityImageInfo* data_ptr = NULL;
        data_hash_table->SetFirst();
        while (data_ptr = data_hash_table->GetNext())
        {
            GuildFacilityInfoForClient data;
            data.facility_code_ = data_ptr->facility_order_;
            guild_facility_container().insert(
                GuildFacilityContainer::value_type(data.facility_code_, data));
        }
    }

    void GuildSystemData::set_element_guild_facility( const GUILD_FACILITY_INFO& facility_info )
    {
        GuildSystem::GuildSystemData::GuildFacilityContainer::iterator found_itr;
        found_itr = guild_facility_container().find(facility_info.code);
        if (found_itr == guild_facility_container().end())
            return;

        found_itr->second.facility_code_ = facility_info.code;
        found_itr->second.facility_level_ = facility_info.level;
        GuildSystem::Function::UpdateAllGuildInstitution();
    }

    void GuildSystemData::set_element_guild_facility_active( const FCODE& facility_code, const bool& is_active )
    {
        GuildSystem::GuildSystemData::GuildFacilityContainer::iterator found_itr;
        found_itr = guild_facility_container().find(facility_code);
        if (found_itr == guild_facility_container().end())
            return;

        found_itr->second.facility_active_ = is_active;
        GuildSystem::Function::UpdateAllGuildInstitution();
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION


#ifdef _NA_008334_20150608_SONNENSCHEIN
    BYTE GuildSystemData::get_sonnenschein_phase()
    {
        if (GlobalFunc::HideSonnenScheinnBuffScene())
        {
            return 0;
        }
        return GetGuildInfo().sonnenschein_phase_;
    }
#endif //_NA_008334_20150608_SONNENSCHEIN

}; //namespace GuildSystem

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL