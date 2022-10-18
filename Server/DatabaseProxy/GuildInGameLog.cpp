#include "StdAfx.h"
#include "GuildInGameLog.h"

#ifdef _NA_003923_20120130_GUILD_RENEWAL

#include "PacketStruct_DZ.h"
#include "PacketStruct_GZ.h"

#include "PacketHandler/Handler_Guild.GuildToDbp.h"
#include "QueryObjects/DBHandler.h"

GuildInGameLog::GuildInGameLog(GUILDGUID guild_guid)
{
    for (int log_type = GuildGrowthLog; log_type < GuildMaxLog; ++log_type)
    {
        log_list_[log_type] = new InGameLogList;
    }

    _RequestLogListToDB(guild_guid);
}

GuildInGameLog::~GuildInGameLog()
{
    for (int log_type = GuildGrowthLog; log_type < GuildMaxLog; ++log_type)
    {
        SAFE_DELETE(log_list_[log_type]);
    }
}

void 
GuildInGameLog::WriteGuildLog(const BYTE log_type,//eLOG_GUILD
                              const GUILDGUID request_guild_guid,
                              const TCHAR* string_param,
                              const DWORD param1,
                              const DWORD param2,
                              const DWORD param3)
{   
    GuildInGameLogData log_data;
    log_data.log_type_ = log_type;
    log_data.param1_ = param1;
    log_data.param2_ = param2;
    log_data.param3_ = param3;

    if (string_param)
    {
        _tcsncpy(log_data.string_param_, string_param, _countof(log_data.string_param_));
        log_data.string_param_[_countof(log_data.string_param_) - 1] = '\0';
    }
    else
    {
        log_data.string_param_[0] = '\0';
    }

    _WriteGuildLogToDB(request_guild_guid, log_data);
}

void 
GuildInGameLog::InsertLogToLIst(const GuildInGameLogData& log_data)
{
    GuildInGameLogCategory log_category = 
        _GetLogCategory(static_cast<GuildInGameLogType>(log_data.log_type_));
    if (log_category >= GuildMaxLog)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Guild logtype is incorrect : type = %d",
            log_data.log_type_
            );
        return;
    }

    InGameLogList* ingame_log_list = log_list_[log_category];

    ingame_log_list->push_front(log_data);

    if (ingame_log_list->size() >= MAX_IN_GAME_GUILD_LOG) {
        ingame_log_list->pop_back();
    }
}

void
GuildInGameLog::InitializeGuildLog(const GuildInGameLogData* input_data, const WORD log_count)
{
    const WORD total_max_log_count = MAX_IN_GAME_GUILD_LOG * GuildMaxLog;

    if (log_count > total_max_log_count)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Guild log count is over : count = %d",
            log_count
            );
        return;
    }

    for (int log_index = 0; log_index < log_count; ++log_index)
    {
        const GuildInGameLogData& init_data = input_data[log_index];
        GuildInGameLogCategory log_category = 
            _GetLogCategory(static_cast<GuildInGameLogType>(init_data.log_type_));
        if (log_category >= GuildMaxLog)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : Guild logtype is incorrect : type = %d",
                log_category
                );
            continue;
        }

        InGameLogList* log_list = log_list_[log_category];
        if (log_list->size() < MAX_IN_GAME_GUILD_LOG-1) {
            log_list->push_back(init_data); // push_back ?? 디비에서 읽어 올때는 최신로그부터 들어오는건가?
        }
    }
}

void 
GuildInGameLog::SendInGameLogToPlayer(CHARGUID char_guid)
{
    for (BYTE index = 0; index < GuildMaxLog; ++index)
    {
        MSG_GZ_GUILD_IN_GAME_GUILD_LOG_LIST_ANS send_msg;
        send_msg.log_category_ = index;
        send_msg.request_char_guid_ = char_guid;
        send_msg.log_count_ = 
            _SerializeGuildLog(
            send_msg.log_data_, 
            static_cast<GuildInGameLogCategory>(index)
            );
        g_DBProxyServer.SendToSessionType(GAME_SERVER, &send_msg, sizeof(send_msg));
    }
}

BYTE
GuildInGameLog::_SerializeGuildLog(GuildInGameLogData* OUT output_data, 
                                   const GuildInGameLogCategory log_category)const
{
    if (log_category >= GuildMaxLog)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Guild logtype is incorrect : type = %d",
            log_category
            );
        return 0;
    }

    const InGameLogList* log_list = log_list_[log_category];

    InGameLogList::const_iterator begin_it = log_list->begin();
    InGameLogList::const_iterator end_it = log_list->end();

    if (log_list->size() >= MAX_IN_GAME_GUILD_LOG)
    {
        //MAX_IN_GAME_GUILD_LOG 사이즈를 넘는 갯수의 로그 카운터는 비정상적이다.
        //하지만 만약의 경우 넘더라도 해당 컨텐츠 서비스는 이루어져야 하므로 감시 로그를 삽입.
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Log count is over : count = %d",
            log_list->size()
            );
    }

    BYTE log_count = 0;
    for (; begin_it != end_it; ++begin_it)
    {
        const GuildInGameLogData& input_log = *(begin_it);
        output_data[log_count++] = input_log;

        if (log_count >= MAX_IN_GAME_GUILD_LOG)
            break;
    }

    return log_count;
}

void
GuildInGameLog::_WriteGuildLogToDB(const GUILDGUID guild_guid, const GuildInGameLogData& log_data)
{
    MSG_DZ_GUILD_WRITE_IN_GAME_LOG_SYN send_msg;
    send_msg.log_category_ = _GetLogCategory(static_cast<GuildInGameLogType>(log_data.log_type_));
    send_msg.guild_guid_ = guild_guid;
    send_msg.log_data_ = log_data;

    Handler_Guild_GuildToDbp::OnDZ_GUILD_WRITE_IN_GAME_LOG_SYN(
        g_DBHandler.GetSharedSession(), &send_msg, sizeof(send_msg));
}

void 
GuildInGameLog::_RequestLogListToDB(GUILDGUID guild_guid)const
{
    MSG_DZ_GUILD_IN_GAME_LOG_LIST_REQ send_msg;
    send_msg.guild_guid_ = guild_guid;

    Handler_Guild_GuildToDbp::OnDZ_GUILD_IN_GAME_LOG_LIST_REQ(
        g_DBHandler.GetSharedSession(), &send_msg, sizeof(send_msg));
}

GuildInGameLogCategory
GuildInGameLog::_GetLogCategory(const GuildInGameLogType log_type)const
{
    switch(log_type)
    {
    case GuildCreate:		     
    case GuildLevelUp:	         
    case GuildLevelDown:         
    case GuildEquipmentApply:    
    case GuildEquipmentLevelup:  
    case GuildEquipmentLevelDown:
    case GuildGivePoint:         
    case GuildChangeName:        
    case GuildRegisterMark:      
    case GuildRegisterAnnounce:  
    case GuildRegisterIntroduce: 
        {
            return GuildGrowthLog;
        }

    case GuildMemberJoin:       
    case GuildMemberKick:	    
    case GuildMemberOut:	    
    case GuildDutyChange:	    
    case GuildChangeMaster:	
        {
            return GuildMemberLog;
        }

    case GuildRelationAllienceSet:
    case GuildRelationAllienceCancle:	
    case GuildSendHostilitySet: 
    case GuildRecvHostilitySet: 
    case GuildBothHostilitySet: 
    case GuildHostilityCancel:  
        {
            return GuildRelationLog;
        }
    case GuildSelectedDomiAttack:
    case GuildNotifyDomiAttack:  
    case GuildSuccessDomiAttack: 
    case GuildSuccessDomiDefense:
    case GuildAuctionBatting:    
    case GuildFailAuction:
    case GuildPenaltyTime: //_NA_000000_20130429_DOMINATION_PENALTY
        {
            return GuildDominationLog;
        }
    }

    return GuildMaxLog;
}

#endif//_NA_003923_20120130_GUILD_RENEWAL