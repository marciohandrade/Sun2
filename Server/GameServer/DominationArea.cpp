#include "stdafx.h"
#include "DominationArea.h"
#include "DominationAuction.h"
#include "PacketStruct_GZ.h"

#ifdef _NA001385_20090924_DOMINATION
DominationArea::DominationArea(MAPCODE map_code) : 
    map_code_(map_code), 
    transaction_increase_deffense_wall_(false), 
    is_load_data_(false), 
    auction_(map_code)
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    , member_(map_code)
{
}

DominationArea::~DominationArea()
{
}

void
DominationArea::SetInfo(const DOMINATION_INFO& domination_info)
{
    domination_info_ = domination_info;
    SUNLOG(eFULL_LOG, __FUNCTION__":Set Area Info : %d", domination_info.m_FieldCode);

    _IsLoadData(true);
}

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
void DominationArea::SetInfo( const DOMINATION_INFO& domination_info, const eDOMINATION_EVENT_STATE domi_state )
{
    domination_info_ = domination_info;
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    //SetInfo가 호출되는 시점이 점령전이 초기화 되는 시점 이므로 이때 점령한 길드의 정보를 저장한다.
    m_PrevInGuildInfo = domination_info.m_inGuildInfo;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    SUNLOG(eFULL_LOG, __FUNCTION__":Set Area Info : %d", domination_info.m_FieldCode);

    switch (domi_state)
    {
    case DOMINATION_EVENT_WAIT:
        domination_info_.progress_state_ = DOMINATION_PROGRESS_STATE_READY;
        break;
    case DOMINATION_EVENT_START:
        domination_info_.progress_state_ = DOMINATION_PROGRESS_STATE_START;
        break;
    default:
        domination_info_.progress_state_ = DOMINATION_PROGRESS_STATE_NONE;
        break;
    }

    _IsLoadData(true);
}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

bool 
DominationArea::ExistGuild(GUILDGUID guild_guid)const
{
    const DOMINATION_INFO& domination_info = GetInfo();

    return (domination_info.m_inGuildInfo.m_GuildGuid == guild_guid ||
            domination_info.m_disGuildInfo.m_GuildGuid == guild_guid)? true : false;
}

DOMINATION_BASIC_GUILD_INFO*
DominationArea::_WhatGuild(GUILDGUID guild_guid)
{
    DOMINATION_INFO& domination_info = GetInfo();

    if (domination_info.m_inGuildInfo.m_GuildGuid == guild_guid)
    {
        //점령 길드
        return &(domination_info.m_inGuildInfo);
    }
    else if (domination_info.m_disGuildInfo.m_GuildGuid == guild_guid)
    {
        //공격 길드
        return &(domination_info.m_disGuildInfo);
    }

    SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not found guild : %d", guild_guid);
    return NULL;
}

void 
DominationArea::ChangeGuildMark(GUILDGUID guild_guid, 
                                GUILDMARKIDX pattern_index, GUILDMARKIDX background_index)
{
    DOMINATION_BASIC_GUILD_INFO* guild_info = _WhatGuild(guild_guid);
    if (guild_info == NULL)
    {
        return;
    }
    guild_info->m_PatternIndex = pattern_index;
    guild_info->m_BackGroundIndex = background_index;
}

void 
DominationArea::ChangeGuildName(GUILDGUID guild_guid, TCHAR* guild_name)
{
    DOMINATION_BASIC_GUILD_INFO* guild_info = _WhatGuild(guild_guid);
    if (guild_info == NULL)
    {
        return;
    }
    _tcsncpy(guild_info->m_tszGuildName, guild_name, _countof(guild_info->m_tszGuildName));
    guild_info->m_tszGuildName[_countof(guild_info->m_tszGuildName) - 1] = '\0';
}

void 
DominationArea::ChangeGuildMaster(GUILDGUID guild_guid, TCHAR* master_name)
{
    DOMINATION_BASIC_GUILD_INFO* guild_info = _WhatGuild(guild_guid);
    if (guild_info == NULL)
    {
        return;
    }
    _tcsncpy(guild_info->m_tszGuildMaster, master_name, _countof(guild_info->m_tszGuildMaster));
    guild_info->m_tszGuildMaster[_countof(guild_info->m_tszGuildMaster) - 1] = '\0';
}

bool
DominationArea::CanIncreaseDeffenseWall()const
{
    if (transaction_increase_deffense_wall_ == true)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not increase deffense wall point [transaction]");
        return false;
    }

    return true;
}

bool 
DominationArea::IsDeffenseGuild(GUILDGUID guild_guid)const
{
    if(domination_info_.m_inGuildInfo.m_GuildGuid == guild_guid)
    {
        return true;
    }
    return false;
}

bool 
DominationArea::IsAttackGuild(GUILDGUID guild_guid)const
{
    if(domination_info_.m_disGuildInfo.m_GuildGuid == guild_guid)
    {
        return true;
    }
    return false;
}

bool 
DominationArea::IsApplyGuild(GUILDGUID guild_guid)const
{
    if(domination_info_.m_disGuildInfo.m_GuildGuid == guild_guid ||
       domination_info_.m_inGuildInfo.m_GuildGuid == guild_guid)
    {
        return true;
    }
    return false;
}

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
bool 
DominationArea::IsPrevInGuild( GUILDGUID guild_guid ) const
{
    if (m_PrevInGuildInfo.m_GuildGuid == guild_guid)
    {
        return true;
    }
    return false;
}
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

void 
DominationArea::DataLoad()const
{
    if (is_load_data_ == false)
    {
        _RequestData();
    }
    
    if (auction_.IsLoaded() == false)
    {
        auction_.RequestData();
    }

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    if (member_.IsLoaded() == false)
    {
        member_.RequestData();
    }
}

void 
DominationArea::_RequestData()const
{
    MSG_GZ_DOMINATION_INFO_SYN send_msg;
    send_msg.m_MapCode = GetMapCode();
    g_pGameServer->SendToServer(GUILD_SERVER, &send_msg, sizeof(send_msg));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_domination",
            _T("|["__FUNCTION__"]|{mapcode}="), send_msg.m_MapCode)));
}

const RC::eDOMINATION_RESULT
DominationArea::CanJoinMember(const GUILDGUID guild_guid, const CHARGUID char_guid)const
{
    if (IsApplyGuild(guild_guid) == false)
    {
        return RC::DOMINATION_RESULT_NOT_JOIN_GUILD;
    }

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    bool guild_type = (IsAttackGuild(guild_guid)) ? true : false;
    if (GetMember()->MemberCount(guild_type, char_guid) > MAX_DOMINATION_MEMBER)
    {
        return RC::DOMINATION_RESULT_OVERFLOW_MEMBER;
    }
    
    return RC::DOMINATION_RESULT_SUCCESS;
}
#endif//_NA001385_20090924_DOMINATION

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
void DominationArea::ChangeAreaProgressState( const eDOMINATION_PROGRESS_STATE area_state )
{
    domination_info_.progress_state_ = area_state;
}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL