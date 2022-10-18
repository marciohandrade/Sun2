#include "StdAfx.h"
#include ".\guildrightinfoparser.h"
#include "GuildRankInfoParser.h"

#ifdef _NA_003923_20120130_GUILD_RENEWAL

/*
GuildRightInfo Interface
*/
GuildRightInfo::GuildRightInfo()
: guild_right_(eGUILD_RIGHTS_MAX)
, use_guild_grade_(0xff)
, guild_right_string_code_(0)
{
    ZeroMemory(guild_right_name_, sizeof(guild_right_name_));
    ZeroMemory(guild_right_of_duty_, sizeof(guild_right_of_duty_));
}

GuildRightInfo::~GuildRightInfo()
{
}

bool 
GuildRightInfo::CanUseToThisRightByDuty(const eGUILD_DUTY guild_duty) const
{
    return guild_right_of_duty_[guild_duty];
}

bool 
GuildRightInfo::CanUseToThisRightByGrade(const BYTE guild_grade) const
{
    return (use_guild_grade_ <= guild_grade) ? true : false;
}

/*
GuildRightInfo Interface
*/
bool	
GuildRightInfoParser::CheckDutyForRight(const eGUILD_RIGHTS guild_right, 
                                        const eGUILD_DUTY guild_duty) const
{
    const GuildRightInfo* guild_right_info = Find(guild_right);
    if (guild_right_info == NULL)
    {
        return false;
    }

    return guild_right_info->CanUseToThisRightByDuty(guild_duty);
}

bool	
GuildRightInfoParser::CheckGradeForRight(const eGUILD_RIGHTS guild_right, 
                                         const BYTE guild_grade) const
{
    const GuildRightInfo* guild_right_info = Find(guild_right);

    if(guild_right_info == NULL)
    {
        return FALSE;
    }

    return guild_right_info->CanUseToThisRightByGrade(guild_grade);
}

void
GuildRightInfoParser::Release()
{
    GuildRightInfoMap::iterator	begin_it = guild_right_info_map_.begin(),
                                end_it = guild_right_info_map_.end();

    for ( ;begin_it != end_it; ++begin_it)
    {
        GuildRightInfo* free_info = begin_it->second;
        if (free_info)
        {
            delete free_info;
            free_info = NULL;
        }
    }

    guild_right_info_map_.clear();
}

const GuildRightInfo*			
GuildRightInfoParser::Find(const eGUILD_RIGHTS guild_right) const
{
    GuildRightInfoMap::const_iterator find_it = guild_right_info_map_.find(guild_right);
    if (find_it == guild_right_info_map_.end())
    {
        return NULL;
    }

    return find_it->second;
}

BOOL	
GuildRightInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    __UNUSED((script_code, is_reload));

    int row_size = GetRowSize();
    for (int rows = 0; rows < row_size; ++rows)
    {
        GuildRightInfo* guild_right_info = new GuildRightInfo;

        guild_right_info->guild_right_ = static_cast<eGUILD_RIGHTS>(GetDataBYTE("Index", rows));

        strncpy( 
            guild_right_info->guild_right_name_, 
            GetDataString("GuildInfoName", rows), 
            _countof(guild_right_info->guild_right_name_)
            );
        guild_right_info->guild_right_name_[_countof(guild_right_info->guild_right_name_)-1] = '\0';

        guild_right_info->use_guild_grade_ = GetDataBYTE("UseGuildGrade", rows);

        //직책 추가시에 스크립트, eGUILD_DUTY내용에 해당 직책 추가 이후에 아래 내용에 더해준다.
        bool* right_of_duty = guild_right_info->guild_right_of_duty_;
        right_of_duty[eGUILD_DUTY_MASTER] = GetDataBOOL("Master", rows);
        right_of_duty[eGUILD_DUTY_SUBMASTER] = GetDataBOOL("SubMaster", rows);
        right_of_duty[eGUILD_DUTY_LEADER] = GetDataBOOL("Leader", rows);
        right_of_duty[eGUILD_DUTY_REGULAR_SOLDIER] = GetDataBOOL("Soldier", rows);

        guild_right_info->guild_right_string_code_ = GetDataDWORD("GuildInfoNameCode", rows);

        if (Find(guild_right_info->guild_right_))
        {
            SUNLOG( 
                eCRITICAL_LOG, 
                __FUNCTION__" : Guild right is duplicate : right = %d", 
                guild_right_info->guild_right_
                );

            delete guild_right_info;

            continue;
        }

        guild_right_info_map_.insert(
            std::make_pair(guild_right_info->guild_right_, guild_right_info) 
            );
    }

    return true;
}

#else//_NA_003923_20120130_GUILD_RENEWAL

VOID GuildRightInfo::Release()
{
	GUILD_GRADE_RIGHT_USE_MAP_ITER iterGrade;
	for( iterGrade = m_mapGuildGradeRight.begin(); iterGrade != m_mapGuildGradeRight.end(); ++iterGrade )
	{
		GUILD_GRADE_RIGHT_USE* pRight = iterGrade->second;
		if( pRight )
		{
			delete pRight;
			pRight = NULL;
		}
	}

	m_mapGuildGradeRight.clear();


	GUILD_DUTY_RIGHT_USE_MAP_ITER  iterDuty;
	for( iterDuty = m_mapGuildDutyRight.begin(); iterDuty != m_mapGuildDutyRight.end(); ++iterDuty )
	{
		GUILD_DUTY_RIGHT_USE* pRight = iterDuty->second;
		if( pRight )
		{
			delete pRight;
			pRight = NULL;
		}
	}

	m_mapGuildDutyRight.clear();
}

GUILD_GRADE_RIGHT_USE*			GuildRightInfo::FindGuildGradeRight( BYTE Grade )
{
	GUILD_GRADE_RIGHT_USE_MAP_ITER iter;

	iter = m_mapGuildGradeRight.find( Grade );
	if( iter != m_mapGuildGradeRight.end() )
	{
		GUILD_GRADE_RIGHT_USE* pRight = iter->second;
		if( pRight )
			return pRight;
	}

	return NULL;
}

GUILD_DUTY_RIGHT_USE*			GuildRightInfo::FindGuildDutyRight( eGUILD_DUTY Duty )
{
	GUILD_DUTY_RIGHT_USE_MAP_ITER iter;

	iter = m_mapGuildDutyRight.find( Duty );
	if( iter != m_mapGuildDutyRight.end() )
	{
		GUILD_DUTY_RIGHT_USE* pRight = iter->second;
		if( pRight )
			return pRight;
	}

	return NULL;
}


GuildRightInfoParser::GuildRightInfoParser(void)
{
}

GuildRightInfoParser::~GuildRightInfoParser(void)
{
}

VOID	GuildRightInfoParser::Release()
{
	GUILD_RIGHT_INFO_MAP_ITER	iter;
	for( iter = m_mapGuildRightInfo.begin(); iter != m_mapGuildRightInfo.end(); ++iter )
	{
		GuildRightInfo* pInfo = iter->second;
		if( pInfo )
		{
			pInfo->Release();
			delete pInfo;
			pInfo = NULL;
		}
	}

	m_mapGuildRightInfo.clear();
}

GuildRightInfo*			GuildRightInfoParser::Find( eGUILD_RIGHTS	Right )
{
	GUILD_RIGHT_INFO_MAP_ITER	iter;
	iter = m_mapGuildRightInfo.find( Right );
	if( iter != m_mapGuildRightInfo.end() )
	{
		GuildRightInfo* pInfo = iter->second;
		if( pInfo )
			return pInfo;
	}

	return NULL;
}

BOOL	GuildRightInfoParser::CheckGradeForRight( eGUILD_RIGHTS Right, BYTE Grade )
{
	GuildRightInfo* pGuildRightInfo = Find( Right );
	if( !pGuildRightInfo )		return FALSE;

	GUILD_GRADE_RIGHT_USE* pGradeRight = pGuildRightInfo->FindGuildGradeRight( Grade );
	if( !pGradeRight )				return FALSE;
	if( ( pGradeRight->m_Grade != Grade ) || ( FALSE == pGradeRight->m_bRightUse ) )		return FALSE;

	return TRUE;
}

BOOL	GuildRightInfoParser::CheckDutyForRight( eGUILD_RIGHTS Right, eGUILD_DUTY Duty )
{
	GuildRightInfo* pGuildRightInfo = Find( Right );
	if( !pGuildRightInfo )		return FALSE;

	GUILD_DUTY_RIGHT_USE* pGradeRight = pGuildRightInfo->FindGuildDutyRight( Duty );
	if( !pGradeRight )				return FALSE;
	if( ( pGradeRight->m_Duty != Duty ) || ( FALSE == pGradeRight->m_bRightUse ) )		return FALSE;

	return TRUE;
}

//길드 등급별 위임된 권한 체크.
BOOL	GuildRightInfoParser::CheckGradeDelegateRight(  eGUILD_RIGHTS Right, BYTE Grade )
{
    GuildRightInfo* pGuildRightInfo = Find( Right );
    if( !pGuildRightInfo )		return FALSE;

    if( !pGuildRightInfo->m_bRightDelegation )		return FALSE;

    return CheckGradeForRight( Right, Grade );
}

VOID	GuildRightInfoParser::GetDelegateRight( OUT eGUILD_RIGHTS* pGuildRight, OUT BYTE& byCount )
{
    GUILD_RIGHT_INFO_MAP_ITER	iter;
    byCount = 0;

    for( iter = m_mapGuildRightInfo.begin(); iter != m_mapGuildRightInfo.end(); ++iter )
    {
        GuildRightInfo* pInfo = iter->second;
        if( !pInfo )	continue;

        if( TRUE == pInfo->m_bRightDelegation )
        {
            pGuildRight[byCount] = pInfo->m_GuildRight;
            byCount++;
            ASSERT( byCount < eGUILD_RIGHTS_MAX );
        }
    }
}

VOID	GuildRightInfoParser::GetGradeDelegateRight( BYTE Grade, OUT GuildRightInfo** ppGuildRightInfo, OUT BYTE& byCount )
{
    GUILD_RIGHT_INFO_MAP_ITER	iter;
    byCount = 0;

    for( iter = m_mapGuildRightInfo.begin(); iter != m_mapGuildRightInfo.end(); ++iter )
    {
        GuildRightInfo* pInfo = iter->second;
        if( !pInfo )	continue;

        if( TRUE == pInfo->m_bRightDelegation )
        {
            if( !CheckGradeForRight( pInfo->m_GuildRight, Grade ) )
                continue;

            ppGuildRightInfo[byCount] = pInfo;
            byCount++;
            ASSERT( byCount < eGUILD_RIGHTS_MAX );
        }
    }
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL	GuildRightInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
    __UNUSED((ScriptCode, bReload));
	int nRowSize = GetRowSize();
	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		BYTE RightCode = GetDataNumber("Index", nRow);
		GuildRightInfo* pGuildRightInfo = NULL;
		pGuildRightInfo = Find( (eGUILD_RIGHTS)RightCode);
		if( pGuildRightInfo )
		{
			//중복된 권한 
			SUNLOG( eCRITICAL_LOG, "[GuildRightInfoParser::_Load] 길드 권한이 중복되었습니다. [%d]", RightCode );
			continue;
		}

		pGuildRightInfo = new GuildRightInfo;
		pGuildRightInfo->m_GuildRight = (eGUILD_RIGHTS)RightCode;

		strncpy( pGuildRightInfo->m_szRightName, GetDataString("GuildInfoName", nRow), MAX_SKILLNAME_LENGTH );
		pGuildRightInfo->m_szRightName[MAX_SKILLNAME_LENGTH-1] = '\0';


		//__UNUSED(BYTE byRight = 0;);
		char szTemp[64] = {0,};
		// 길드 등급별 권한 
		// gamelulu's note : Guild Grade 정보를 삭제 하면서 하드 코딩 된 내용 
		for( int i = 1; i < 8; i++ )
		{	
			GUILD_GRADE_RIGHT_USE*	pGuildRight = new GUILD_GRADE_RIGHT_USE;
			pGuildRight->m_Grade = i;
			sprintf(szTemp, "GuildRight%d", i);
			pGuildRight->m_bRightUse = (.0f != GetDataNumber(szTemp, nRow));
			if( !pGuildRightInfo->m_mapGuildGradeRight.insert( std::make_pair( pGuildRight->m_Grade, pGuildRight ) ).second )
			{
				SUNLOG( eCRITICAL_LOG, "[GuildRightInfoParser::_Load] 길드 등급별 권한이 중복되었습니다. [%d]", pGuildRight->m_Grade );
				delete pGuildRight;
			}
		}

		// 길드 직책별 권한.
		for( int i = 1; i < eGUILD_DUTY_MAX; i++ )
		{
			GUILD_DUTY_RIGHT_USE*	pGuildRight = new GUILD_DUTY_RIGHT_USE;
			pGuildRight->m_Duty = (eGUILD_DUTY)i;
			sprintf(szTemp, "GuildDuty%d", i);
			pGuildRight->m_bRightUse = (.0f != GetDataNumber(szTemp, nRow));
			if( !pGuildRightInfo->m_mapGuildDutyRight.insert( std::make_pair( pGuildRight->m_Duty, pGuildRight ) ).second )
			{
				SUNLOG( eCRITICAL_LOG, "[GuildRightInfoParser::_Load] 길드 직책별 권한이 중복되었습니다. [%d]", pGuildRight->m_Duty );
				delete pGuildRight;
			}
		}

		// 위임가능한 권한인가?
		pGuildRightInfo->m_bRightDelegation = GetDataNumber("GmasterRightDelegation", nRow);
		pGuildRightInfo->m_dwStringCode = GetDataNumber("GuildInfoNameCode", nRow);
		m_mapGuildRightInfo.insert( std::make_pair( pGuildRightInfo->m_GuildRight, pGuildRightInfo ) );
	}

	return TRUE;
}
#pragma warning ( pop )

#endif//_NA_003923_20120130_GUILD_RENEWAL