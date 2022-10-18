#include "SunClientPrecompiledHeader.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "OptionListParser.h"
#include "ZoneMarkRewardInfo.h"
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "ChaosZoneSystem/SonnenScheinRewardInfoParser.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

ZoneMarkRewardInfo::ZoneMarkRewardInfo()
{
#ifdef _NA_008334_20150608_SONNENSCHEIN
    sonnenschein_dummy_strcut_.m_DominationMapCode = SONNENSCHEIN_MAPCODE;
    sonnenschein_dummy_strcut_.m_LinkMapCode = SONNENSCHEIN_MAPCODE;
    sonnenschein_dummy_strcut_.m_SubMapCode = SONNENSCHEIN_MAPCODE;
#endif //_NA_008334_20150608_SONNENSCHEIN
}

ZoneMarkRewardInfo::~ZoneMarkRewardInfo()
{

}

void ZoneMarkRewardInfo::Clear()
{
    domination_reward_option_.clear();
    domination_selected_reward_option_.clear();
}

void ZoneMarkRewardInfo::SetZoneMarkInfo(STLX_VECTOR<MAPCODE>& domination_state_info
#ifdef _NA_008334_20150608_SONNENSCHEIN
                                         , BYTE sonnenschein_level
#endif //_NA_008334_20150608_SONNENSCHEIN
                                         )
{
    Clear();

    DWORD size_of_state_info = domination_state_info.size();
    MAPCODE map_code;

    for (DWORD index = 0; index < size_of_state_info; ++index)
    {
        map_code = domination_state_info[index];
        SetZoneMarkInfo(map_code);
    }

#ifdef _NA_008334_20150608_SONNENSCHEIN
    // 소넨샤인 버프 저장
    SetSonnenScheinRewardInfo(sonnenschein_level);

    // 점령전 버프 + 소넨샤인 버프 취합
    domination_selected_reward_option_ = CombineRewadInfo(domination_state_info, sonnenschein_level);
#else
    domination_selected_reward_option_ = 
        DominationInfoParser::Instance()->GetRewardOption(domination_state_info);
#endif //_NA_008334_20150608_SONNENSCHEIN
}

void ZoneMarkRewardInfo::SetZoneMarkInfo( MAPCODE mapcode )
{
    BASE_OptionList* option_list = NULL;

    const sDOMINATION_INFO* domination_info = DominationInfoParser::Instance()->GetDominationInfo(mapcode);
    if (domination_info)
    {
        REWARD_OPTION reward_option;

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        for (DWORD reward_index = 0; reward_index < sDOMINATION_INFO::REWARD_OPTION_MAX; ++reward_index)
#else
        for (DWORD reward_index = 0; reward_index < sDOMINATION_INFO::LIMIT_MAX; ++reward_index)
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
        {
            if (domination_info->m_RewardOption[reward_index].m_wOptionKind)
            {
                option_list = OptionListParser::Instance()->GetOptionList(domination_info->m_RewardOption[reward_index].m_wOptionKind);
                if (option_list)
                {
                    reward_option.OptionNC[reward_option.Count] = option_list->mdwNameCode;
                    reward_option.OptionKind[reward_option.Count] = domination_info->m_RewardOption[reward_index].m_wOptionKind;
                    reward_option.OptionType[reward_option.Count] = domination_info->m_RewardOption[reward_index].m_byOptionType;
                    reward_option.OptionValue[reward_option.Count] = domination_info->m_RewardOption[reward_index].m_iOptionValue;                    
                    reward_option.Count++;
                }
            }				
        }

        if (reward_option.Count)
        {
            reward_option.MapCode = mapcode;
            domination_reward_option_.push_back(reward_option);				
        }
    }
}

void ZoneMarkRewardInfo::RenderTooltip(DWORD reward_index, bool force_enable, DWORD DialogKey )
{
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tooltip_manager)
    {
        if (reward_index >= domination_reward_option_.size()
#ifdef _NA_008334_20150608_SONNENSCHEIN
            && (reward_index != SONNENSCHEIN_INDEX_KEY)
#endif //_NA_008334_20150608_SONNENSCHEIN
        )
        {
            return;
        }

#ifdef _NA_008334_20150608_SONNENSCHEIN
        REWARD_OPTION* reward_option = NULL;
        if (reward_index != SONNENSCHEIN_INDEX_KEY)
        {
            reward_option = &domination_reward_option_[reward_index];
        }
        else
        {
            reward_option = &sonnenschein_reward_option_;
        }
#else
        REWARD_OPTION* reward_option = &domination_reward_option_[reward_index];
#endif //_NA_008334_20150608_SONNENSCHEIN
        TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR option_name[INTERFACE_STRING_LENGTH] = {0, };


        // 지배의 인장 출력
        g_InterfaceManager.GetInterfaceString(3335, tooltip_string, INTERFACE_STRING_LENGTH);
        tooltip_manager->RegisterTooltip(tooltip_string, -1, 20, StrToWzID("m212"), WzColor_RGBA(209,39,40,255));
		tooltip_manager->SetTooltipDialogID( DialogKey );

        // 필드명출력
        const sMAPINFO* pFieldInfo = MapInfoParser::Instance()->FindMapInfo(reward_option->MapCode);
        if (pFieldInfo)
        {
            g_InterfaceManager.GetInterfaceString(pFieldInfo->dwNCode, tooltip_string, INTERFACE_STRING_LENGTH);
            tooltip_manager->RegisterTooltip(tooltip_string, -1, 20, StrToWzID("m212"), RGBA(76,240,4,255));
        }

        //옵션출력
        DWORD option_text_color = WzColor_RGBA(255, 255, 255, 255);
        for (DWORD OptionIndex = 0; OptionIndex < reward_option->Count; ++OptionIndex)
        {
            // 선택된 옵션 정보에 따라 텍스트 컬러 결정
            int option_kind = reward_option->OptionKind[OptionIndex];
            bool is_selected_option = false;

            DominationInfoParser::SelectedRewardOptions::iterator found_itr = 
                domination_selected_reward_option_.find(option_kind);
            if (found_itr != domination_selected_reward_option_.end())
            {
                DominationInfoParser::SelectedRewardOption& selected_option = found_itr->second;
                if ((selected_option.domination_info->m_DominationMapCode == reward_option->MapCode) || 
                    (selected_option.domination_info->m_LinkMapCode == reward_option->MapCode) || 
                    (selected_option.domination_info->m_SubMapCode == reward_option->MapCode))
                {
                    if (selected_option.option_index == static_cast<int>(OptionIndex))
                    {
                        is_selected_option = true;
                    }
                }
            }
            else if (force_enable == false)
            {
                continue;
            }

            if ((is_selected_option == true) || 
                (force_enable == true))
            {
                option_text_color = WzColor_RGBA(255, 255, 255, 255);
            }
            else
            {
                option_text_color = WzColor_RGBA(150, 150, 150, 255);
            }

            TCHAR tooltip_format_value[] = _T("%s : %+d");
            TCHAR tooltip_format_ratio[] = _T("%s : %+.1f");

            g_InterfaceManager.GetInterfaceString(reward_option->OptionNC[OptionIndex], option_name, INTERFACE_STRING_LENGTH);
            if (reward_option->OptionType[OptionIndex] == VALUE_TYPE_VALUE)
            {
                Snprintf(tooltip_string, INTERFACE_STRING_LENGTH, tooltip_format_value, option_name, reward_option->OptionValue[OptionIndex]);
            }
            else
            {
                Snprintf(tooltip_string, INTERFACE_STRING_LENGTH, tooltip_format_ratio, option_name, reward_option->OptionValue[OptionIndex]/10.0f);
            }

            BASE_OptionList* option_list = OptionListParser::Instance()->GetOptionList(reward_option->OptionKind[OptionIndex]);
            BYTE option_type = static_cast<BYTE>(reward_option->OptionType[OptionIndex]);
            GlobalFunc::AddOptionSuffix(option_type, option_list, tooltip_string);

            tooltip_manager->RegisterTooltip(tooltip_string, -1, 20, StrToWzID("m212"), option_text_color);
        }
    }
}

#ifdef _NA_008334_20150608_SONNENSCHEIN
void ZoneMarkRewardInfo::SetSonnenScheinRewardInfo(const BYTE sonnenschein_level)
{
    const SScheinRewardInfo* find_info =
        SonnenScheinRewardInfoParser::Instance()->FindData(sonnenschein_level);

    if (find_info)
    {
        ZeroMemory(&sonnenschein_reward_option_, sizeof(REWARD_OPTION));

        BASE_OptionList* option_list = NULL;

        sonnenschein_reward_option_.MapCode = SONNENSCHEIN_MAPCODE;

        for (int option_index = 0; 
            option_index < _countof(find_info->reward_buff_option); 
            ++option_index)
        {
            option_list = OptionListParser::Instance()->GetOptionList(find_info->reward_buff_option[option_index].m_wOptionKind);
            if (option_list)
            {
                sonnenschein_reward_option_.OptionNC[sonnenschein_reward_option_.Count] =
                    option_list->mdwNameCode;
                sonnenschein_reward_option_.OptionKind[sonnenschein_reward_option_.Count] =
                    find_info->reward_buff_option[option_index].m_wOptionKind;
                sonnenschein_reward_option_.OptionType[sonnenschein_reward_option_.Count] =
                    find_info->reward_buff_option[option_index].m_byOptionType;
                sonnenschein_reward_option_.OptionValue[sonnenschein_reward_option_.Count] =
                    find_info->reward_buff_option[option_index].m_iOptionValue;                    
                sonnenschein_reward_option_.Count++;
            }
        }
    }
}

// 점령전버프+ 소넨샤인버프 합침
DominationInfoParser::SelectedRewardOptions
ZoneMarkRewardInfo::CombineRewadInfo(STLX_VECTOR<MAPCODE>& domination_state_info, const BYTE sonnenschein_level)
{
    DominationInfoParser::SelectedRewardOptions selelcted_options;

    // 점령전버프
    for (STLX_VECTOR<MAPCODE>::const_iterator map_it = domination_state_info.begin(); map_it < domination_state_info.end(); ++map_it)
    {
        const MAPCODE taken_map_code = *map_it;
        const sDOMINATION_INFO* const domination_info = DominationInfoParser::Instance()->GetDominationInfo(taken_map_code);
        if (domination_info == NULL)
        {
            continue;
        }
        for (int option_index = 0; 
            option_index < _countof(domination_info->m_RewardOption); 
            ++option_index)
        {
            const AttrOptionInfo& new_option_info = domination_info->m_RewardOption[option_index];
            const int option_kind = new_option_info.m_wOptionKind;
            if (option_kind == 0)
            {
                break;
            }
            DominationInfoParser::SelectedRewardOptions::const_iterator option_it = selelcted_options.find(option_kind);
            if (option_it != selelcted_options.end())
            {
                // 같은 종류의 보상 옵션이 존재하면 큰값만 남긴다.
                const DominationInfoParser::SelectedRewardOption& prev_option = option_it->second;
                const AttrOptionInfo* const prev_option_info = prev_option.option_info;
                if (new_option_info.m_iOptionValue > prev_option_info->m_iOptionValue)
                {
                    selelcted_options[option_kind] = 
                        DominationInfoParser::SelectedRewardOption(domination_info, option_index, &new_option_info);
                }
            }
            else
            {
                selelcted_options[option_kind] = 
                    DominationInfoParser::SelectedRewardOption(domination_info, option_index, &new_option_info);
            }
        }
    }

    // 소넨샤인버프
    if (sonnenschein_level != 0)
    {
        const SScheinRewardInfo* sonnenschein_reward_info =
            SonnenScheinRewardInfoParser::Instance()->FindData(sonnenschein_level);
        if (sonnenschein_reward_info != NULL)
        {
            for (int option_index = 0; 
                option_index < _countof(sonnenschein_reward_info->reward_buff_option); 
                ++option_index)
            {
                const AttrOptionInfo& new_option_info = sonnenschein_reward_info->reward_buff_option[option_index];
                const int option_kind = new_option_info.m_wOptionKind;
                if (option_kind == 0)
                {
                    break;
                }
                DominationInfoParser::SelectedRewardOptions::const_iterator option_it = selelcted_options.find(option_kind);
                if (option_it != selelcted_options.end())
                {
                    // 같은 종류의 보상 옵션이 존재하면 큰값만 남긴다.
                    const DominationInfoParser::SelectedRewardOption& prev_option = option_it->second;
                    const AttrOptionInfo* const prev_option_info = prev_option.option_info;
                    if (new_option_info.m_iOptionValue > prev_option_info->m_iOptionValue)
                    {
                        // 소넨샤인 맵코드로 넣어줌. 툴팁에 활성화된 옵션인지 체크할때 사용됨
                        selelcted_options[option_kind] = 
                            DominationInfoParser::SelectedRewardOption(&sonnenschein_dummy_strcut_, option_index, &new_option_info);
                    }
                }
                else
                {
                    selelcted_options[option_kind] = 
                        DominationInfoParser::SelectedRewardOption(&sonnenschein_dummy_strcut_, option_index, &new_option_info);
                }
            }
        }
    }

    return selelcted_options;
}
#endif //_NA_008334_20150608_SONNENSCHEIN