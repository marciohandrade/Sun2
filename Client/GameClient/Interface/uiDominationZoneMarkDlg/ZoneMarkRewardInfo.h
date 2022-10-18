#pragma once

#include "uibase.h"
#include "DominationInfoParser.h"

struct REWARD_OPTION
{
    MAPCODE MapCode;
    DWORD Count;
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    DWORD OptionKind[sDOMINATION_INFO::REWARD_OPTION_MAX];
    DWORD OptionType[sDOMINATION_INFO::REWARD_OPTION_MAX];
    int OptionValue[sDOMINATION_INFO::REWARD_OPTION_MAX];
    DWORD OptionNC[sDOMINATION_INFO::REWARD_OPTION_MAX];
#else
    DWORD OptionKind[sDOMINATION_INFO::LIMIT_MAX];
    DWORD OptionType[sDOMINATION_INFO::LIMIT_MAX];
    int OptionValue[sDOMINATION_INFO::LIMIT_MAX];
    DWORD OptionNC[sDOMINATION_INFO::LIMIT_MAX];
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

    REWARD_OPTION()
    {
        MapCode = 0;
        Count = 0;			
    }
};

// 인장 정보 및 보상옵션 정보 클래스
// HERO 의 인장 정보 출력과 타겟의 인장정보 출력을 위해 정보관련 클래스를 분리
class ZoneMarkRewardInfo
{
public:
    typedef std::vector<REWARD_OPTION>				REWARD_OPTION_VECTOR;
    typedef std::vector<REWARD_OPTION>::iterator	REWARD_OPTION_VECTOR_ITR;

#ifdef _NA_008334_20150608_SONNENSCHEIN
    enum
    {
        SONNENSCHEIN_MAPCODE = 73,
        SONNENSCHEIN_INDEX_KEY = 9999,
        SONNENSCHEIN_CLEAR_STRINGCODE_START = 3518, // 소넨샤인 <1단계 구역 탈환> // 1~9단계
    };
#endif //_NA_008334_20150608_SONNENSCHEIN

public:
    ZoneMarkRewardInfo();
    ~ZoneMarkRewardInfo();

    void Clear();

    void SetZoneMarkInfo(STLX_VECTOR<MAPCODE>& domination_state_info
#ifdef _NA_008334_20150608_SONNENSCHEIN
        , BYTE sonnenschein_level = 0
#endif //_NA_008334_20150608_SONNENSCHEIN
        );
    void SetZoneMarkInfo(MAPCODE mapcode);

    void RenderTooltip(DWORD reward_index, bool force_enable, DWORD DialogKey );
    REWARD_OPTION_VECTOR& GetRewardOptionInfo()   {   return domination_reward_option_; }
    DWORD GetRewardCount() {   return domination_reward_option_.size(); }
    
    const DominationInfoParser::SelectedRewardOptions& GetSelectedRewardOptionInfo() 
    {
        return domination_selected_reward_option_;
    }

private:
    REWARD_OPTION_VECTOR domination_reward_option_;
    DominationInfoParser::SelectedRewardOptions domination_selected_reward_option_;

#ifdef _NA_008334_20150608_SONNENSCHEIN
    // 점령전버프+ 소넨샤인버프 합침
    DominationInfoParser::SelectedRewardOptions  CombineRewadInfo(
        STLX_VECTOR<MAPCODE>& domination_state_info, const BYTE sonnenschein_level);

    void SetSonnenScheinRewardInfo(const BYTE sonnenschein_level);
    REWARD_OPTION sonnenschein_reward_option_;  // 소넨샤인 버프옵션 저장용
    sDOMINATION_INFO sonnenschein_dummy_strcut_; // 소넨샤인 용 더미 구조체
#endif //_NA_008334_20150608_SONNENSCHEIN
};
