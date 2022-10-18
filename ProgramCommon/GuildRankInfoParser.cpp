#include "StdAfx.h"
#include ".\guildrankinfoparser.h"

#ifdef _NA_003923_20120130_GUILD_RENEWAL
GuildRankInfo::GuildRankInfo()
: guild_grade_(0)
, max_member_count_(0)
, item_code_levelup_(0)
, need_guild_coin_to_rankup_(0)
, reward_gp_by_levelup_(0)
{
    ZeroMemory(&max_member_of_duty_, sizeof(max_member_of_duty_));

}

GuildRankInfoParser::GuildRankInfoParser(void)
{
    guild_rank_info_map_.clear();
    max_guild_grade_ = 0;
}

GuildRankInfoParser::~GuildRankInfoParser(void)
{

}

void 
GuildRankInfoParser::Release()
{
    GuildRankInfoMap::iterator begin_it = guild_rank_info_map_.begin(),
                               end_it = guild_rank_info_map_.end();

    for (; begin_it != end_it; ++begin_it)
    {
        GuildRankInfo* guild_rank_info = begin_it->second;
        if (guild_rank_info)	
        {
            delete guild_rank_info;
            guild_rank_info = NULL;
        }
    }

    guild_rank_info_map_.clear();
}

const GuildRankInfo*		
GuildRankInfoParser::FindGuildRankInfo(BYTE guild_grade) const
{
    GuildRankInfoMap::const_iterator find_it = guild_rank_info_map_.find(guild_grade);

    if (find_it == guild_rank_info_map_.end())
    {
        return NULL;
    }

    return find_it->second;
}

BYTE
GuildRankInfoParser::GetMaxMemberByGrade(const BYTE guild_grade, const eGUILD_DUTY guild_duty) const
{
    const GuildRankInfo* rank_info = FindGuildRankInfo(guild_grade);
    if (rank_info == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not found guild rank info : grade = %d",
            guild_grade
            );
        return 0;
    }

    return rank_info->max_member_count_;
}


BYTE GuildRankInfoParser::GetMatchedRankInfoByGuildCoin( INT guild_coin )
{
    BYTE found_level = 0;

    for(int level = max_guild_grade_; level >= 1 ; level--)
    {
        const GuildRankInfo* rank_info = FindGuildRankInfo(level);
        if (!rank_info)
            continue;

        if(rank_info->need_guild_coin_to_rankup_ <= guild_coin) {
            found_level = level;
            break;
        }
    }

    return found_level;
}

#pragma warning ( push )
#pragma warning ( disable : 4244)
BOOL GuildRankInfoParser::LoadScript( eSCRIPT_CODE script_code, BOOL is_reload )
{
    __UNUSED((script_code, is_reload));

    int row_size = GetRowSize();
    for (int script_row = 0; script_row < row_size; ++script_row)
    {
        GuildRankInfo* rank_info = new GuildRankInfo;

        rank_info->guild_grade_ = GetDataNumber("GuildGrade", script_row);
        rank_info->max_member_count_ = GetDataNumber("MaxMember", script_row);
        rank_info->need_guild_coin_to_rankup_ = GetDataNumber("NeedGuildCoin", script_row);
        rank_info->item_code_levelup_ = GetDataNumber("NeedItemCode", script_row);
        rank_info->reward_gp_by_levelup_ = GetDataNumber("RewardGP", script_row);
        rank_info->max_member_of_duty_[eGUILD_DUTY_MASTER] = GetDataNumber("MasterMember", script_row);
        rank_info->max_member_of_duty_[eGUILD_DUTY_SUBMASTER] = GetDataNumber("SubMasterMember", script_row);
        rank_info->max_member_of_duty_[eGUILD_DUTY_LEADER] = GetDataNumber("LeaderMember", script_row);

        if (FindGuildRankInfo(rank_info->guild_grade_) != NULL ||
            rank_info->guild_grade_ != script_row + 1)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                "Script data incorrect : file = %s, guild level = %d", 
                GetCurFileName(),
                rank_info->guild_grade_ 
                );
            FASSERT(0);
        }

        guild_rank_info_map_.insert(std::make_pair(rank_info->guild_grade_, rank_info));

        if (max_guild_grade_ < rank_info->guild_grade_)
            max_guild_grade_ = rank_info->guild_grade_;
    }

    return TRUE;
}
#pragma warning ( pop )
#else//_NA_003923_20120130_GUILD_RENEWAL

VOID						GuildRankInfo::Release()
{
	GUILD_DUTY_USE_MAP_ITER iter;

	// 길드 직책별 권한 사용 정보. 
	for( iter = m_mapGuildDutyUseInfo.begin(); iter != m_mapGuildDutyUseInfo.end(); ++iter )
	{
		GUILD_DUTY_USE_INFO* pRight = iter->second;
		if( pRight )
		{
			delete pRight;
			pRight = NULL;
		}
	}

	m_mapGuildDutyUseInfo.clear();
}

GUILD_DUTY_USE_INFO*		GuildRankInfo::Find( eGUILD_DUTY Duty )
{
	GUILD_DUTY_USE_MAP_ITER iter;
	if( m_mapGuildDutyUseInfo.size() == 0 )
		return NULL;

	iter = m_mapGuildDutyUseInfo.find( Duty );
	if( iter != m_mapGuildDutyUseInfo.end() )
	{
		GUILD_DUTY_USE_INFO* pRight = iter->second;
		if( pRight )
			return pRight;
	}

	return NULL;
}

//========================================================================================================================


GuildRankInfoParser::GuildRankInfoParser(void)
{
	m_mapGuildRankInfo.clear();
	m_byMaxGuildLevel = 0;
}

GuildRankInfoParser::~GuildRankInfoParser(void)
{
}

VOID					GuildRankInfoParser::Release()
{
	GUILD_RANK_INFO_MAP_ITER iter;
	for( iter = m_mapGuildRankInfo.begin(); iter != m_mapGuildRankInfo.end(); ++iter )
	{
		GuildRankInfo* pInfo = iter->second;
		if( pInfo )	
		{
			pInfo->Release();
			delete pInfo;
			pInfo = NULL;
		}
	}

	m_mapGuildRankInfo.clear();
}

GuildRankInfo*		GuildRankInfoParser::FindGuildRankInfo( BYTE Grade )
{
	GUILD_RANK_INFO_MAP_ITER iter = m_mapGuildRankInfo.find( Grade );
	if( iter != m_mapGuildRankInfo.end() )
	{
		GuildRankInfo* pInfo = iter->second;
		return pInfo;
	}

	return NULL;
}

GUILD_DUTY_USE_INFO * GuildRankInfoParser::GetGuildDutyUseInfo(BYTE Grade,eGUILD_DUTY eDuty)
{
	GuildRankInfo * pGuildRankInfo = FindGuildRankInfo(Grade);
	if( pGuildRankInfo )
	{
		GUILD_DUTY_USE_INFO * pDutyUseInfo = pGuildRankInfo->Find(eDuty);
		if( pDutyUseInfo )
		{
			return pDutyUseInfo;
		}
	}
	return NULL;
}

#pragma warning ( push )
#pragma warning ( disable : 4244)
BOOL GuildRankInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
    __UNUSED((ScriptCode, bReload));
	int nRowSize = GetRowSize();
	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		GuildRankInfo* pInfo = new GuildRankInfo;

		pInfo->guild_grade_			= GetDataNumber("GuildLevel", nRow);
		_tcsncpy( pInfo->m_szLevelName, GetDataString("GuildLevelName", nRow), MAX_PATH );
		pInfo->max_member_count_		= GetDataNumber("MaxMember", nRow);
		pInfo->m_dwHeim					= GetDataNumber("NeedHeim", nRow);
		pInfo->m_UP						= GetDataNumber("NeedUP", nRow);
		pInfo->m_GP		                = GetDataNumber("NeedGP", nRow);

		pInfo->item_code_levelup_	= GetDataNumber("NeedItemCode", nRow);
		pInfo->m_byCreateCopsCount	= GetDataNumber("CorpsEstablishNum", nRow);
		pInfo->m_dwLevelStringCode	= GetDataNumber("GuildRankNCode", nRow);

		char szDuty[eGUILD_DUTY_MAX][64] = {"empty","GuildMaster","SubMaster","Captain","Leader","Contingent","Regular","Temp"};
		char szTemp[64] = {0,};
		for( BYTE byDuty = 1; byDuty < eGUILD_DUTY_MAX; byDuty++ )
		{
			GUILD_DUTY_USE_INFO* pDutyRight = new GUILD_DUTY_USE_INFO;
			pDutyRight->m_Duty			= (eGUILD_DUTY)byDuty;
			pDutyRight->m_bUse			= (.0f != GetDataNumber(szDuty[byDuty], nRow));
			sprintf(szTemp,"%s%s", szDuty[byDuty], "NCode");
			pDutyRight->m_dwStringCode	= GetDataNumber(szTemp, nRow);
			if( !pInfo->m_mapGuildDutyUseInfo.insert( std::make_pair( pDutyRight->m_Duty, pDutyRight ) ).second )
			{
				delete pDutyRight;
				pDutyRight = NULL;
			}
		}

		if( !FindGuildRankInfo(pInfo->guild_grade_ ) )
		{
			m_mapGuildRankInfo.insert( std::make_pair(pInfo->guild_grade_, pInfo ) );
			if( m_byMaxGuildLevel < pInfo->guild_grade_ )
				m_byMaxGuildLevel = pInfo->guild_grade_;
		}
		else
		{
			SUNLOG( eCRITICAL_LOG, "Script Data Fail, File =  %s, GuildLevel = %d", GetCurFileName(), pInfo->guild_grade_ );
			FASSERT(0);
		}
	}

	return TRUE;
}
#pragma warning ( pop )
#endif//_NA_003923_20120130_GUILD_RENEWAL


