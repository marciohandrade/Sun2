#pragma once

#include <Singleton.h>
#include <ScriptCode.IParser.h>
#include <map>

struct GuildInfoData
{
    LEVELTYPE guild_create_level_;//��� â�� ���� : "NeedLevel"
    MONEY guild_create_heim_;//��� â�� �ݾ� : "NeedHeim"
    BYTE max_alliance_;//���� ��� �ִ�� : "MaxAlliance"
    BYTE max_hostile_;//���� ��� �ִ�� : "MaxHostile"
    BYTE max_apply_hostile_;//���� ���� ��û ���� �� : "ApplyHostile"
    DWORD time_of_hostile_latency_;//���� ��� �ð� : "HotileLatency"
    DWORD time_of_hostile_;//���� ���� �ð� : "HostileHour"
    CODETYPE hostile_item_code_;//���� ��û ������ �ڵ� : "HostileItem"
    DWORD time_of_disbanded_latency_;//��� �ػ� ��� �ð� : "DisbandedLatency"
    DWORD time_of_expulsion_penalty_;//���� ���� ����Ƽ �ð� : "ExpulsionPenalty"
    DWORD time_of_withdrawal_penalty_;//��� Ż�� ����Ƽ �ð� : "WithdrawalPenalty"
    DWORD max_guild_coin_donation_;//1�� ��� ���� ��� ���� �� : "CoinDonation"  
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