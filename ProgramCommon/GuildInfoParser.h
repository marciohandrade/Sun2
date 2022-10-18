#pragma once

#include <Singleton.h>
#include <ScriptCode.IParser.h>
#include <map>

struct GuildInfoData
{
    LEVELTYPE guild_create_level_;//길드 창설 레벨 : "NeedLevel"
    MONEY guild_create_heim_;//길드 창설 금액 : "NeedHeim"
    BYTE max_alliance_;//동맹 길드 최대수 : "MaxAlliance"
    BYTE max_hostile_;//적대 길드 최대수 : "MaxHostile"
    BYTE max_apply_hostile_;//동시 적대 신청 가능 수 : "ApplyHostile"
    DWORD time_of_hostile_latency_;//적대 대기 시간 : "HotileLatency"
    DWORD time_of_hostile_;//적대 유지 시간 : "HostileHour"
    CODETYPE hostile_item_code_;//적대 신청 아이템 코드 : "HostileItem"
    DWORD time_of_disbanded_latency_;//길드 해산 대기 시간 : "DisbandedLatency"
    DWORD time_of_expulsion_penalty_;//길드원 제명 패털티 시간 : "ExpulsionPenalty"
    DWORD time_of_withdrawal_penalty_;//길드 탈퇴 패털티 시간 : "WithdrawalPenalty"
    DWORD max_guild_coin_donation_;//1일 길드 코인 기부 가능 수 : "CoinDonation"  
};

class GuildInfoParser : public util::Singleton<GuildInfoParser>, public IParser
{
public:
    GuildInfoParser(void){};
    ~GuildInfoParser(void){};

public:

    const GuildInfoData& GetGuildInfo();

    virtual void Release();

    virtual	BOOL LoadScript(
        eSCRIPT_CODE script_code, 
        BOOL is_reload
        );

private:
    GuildInfoData guild_data_;
};

inline const GuildInfoData& 
GuildInfoParser::GetGuildInfo()
{
    return guild_data_;
}