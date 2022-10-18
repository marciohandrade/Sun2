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

// ���� ���� �� ����ɼ� ���� Ŭ����
// HERO �� ���� ���� ��°� Ÿ���� �������� ����� ���� �������� Ŭ������ �и�
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
        SONNENSCHEIN_CLEAR_STRINGCODE_START = 3518, // �ҳٻ��� <1�ܰ� ���� Żȯ> // 1~9�ܰ�
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
    // ����������+ �ҳٻ��ι��� ��ħ
    DominationInfoParser::SelectedRewardOptions  CombineRewadInfo(
        STLX_VECTOR<MAPCODE>& domination_state_info, const BYTE sonnenschein_level);

    void SetSonnenScheinRewardInfo(const BYTE sonnenschein_level);
    REWARD_OPTION sonnenschein_reward_option_;  // �ҳٻ��� �����ɼ� �����
    sDOMINATION_INFO sonnenschein_dummy_strcut_; // �ҳٻ��� �� ���� ����ü
#endif //_NA_008334_20150608_SONNENSCHEIN
};
