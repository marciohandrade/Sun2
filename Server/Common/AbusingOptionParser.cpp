#include "stdafx.h"
#include "AbusingOptionParser.h"
#include <iostream>

AbusingOptionParser::AbusingOptionParser()
{
}

AbusingOptionParser::~AbusingOptionParser()
{
}

template <typename MAP>
void ClearMap(MAP& container)
{
    MAP::iterator pos = container.begin();
    const MAP::iterator& end_pos = container.end();
    for (; pos != end_pos; ++pos)
    {
        MAP::mapped_type& pointer_value = pos->second;
        SAFE_DELETE(pointer_value);
    }
    container.clear();
}

VOID AbusingOptionParser::Release()
{
    ClearMap(abusing_option_table_);
}

VOID AbusingOptionParser::Init(DWORD pool_size)
{
    __UNUSED(pool_size);
    ClearMap(abusing_option_table_);
}

ABUSING_OPTION_INFO* AbusingOptionParser::GetAbusingOptionInfo(BYTE abuse_sub_type) const
{
	ABUSING_OPTION_TABLE::const_iterator position = abusing_option_table_.find(abuse_sub_type);
    if (position == abusing_option_table_.end())
    {
        return NULL;
    }

	ABUSING_OPTION_INFO* abusing_option_info = position->second;
	return abusing_option_info;
}

//#pragma warning ( push )
//#pragma warning ( disable : 4244)

BOOL AbusingOptionParser::LoadOptionInfo(BOOL is_reload) 
{
	const int row_size = GetRowSize();
	for (int row = 0; row < row_size; ++row)
	{
        // (CHANGES)(20100311)(hukim) AbuseType, AbuseSubType, Disconnect, Level, MilestoneCount, 
        // IncAbusingPoint, MaxAbusingPoint 불필요한 캐스팅을 막기 위해 원래 타입에 맞는 메서드를
        // 사용하여 읽어오도록 변경

		const BYTE abuse_type = GetDataBYTE("AbuseType", row);
		const BYTE abuse_sub_type = GetDataBYTE("AbuseSubType", row);

		ABUSING_OPTION_INFO* abusing_option_info = GetAbusingOptionInfo(abuse_sub_type);
		if (abusing_option_info == NULL)
		{
			abusing_option_info = new ABUSING_OPTION_INFO;
			abusing_option_table_.insert(std::make_pair(abuse_sub_type, abusing_option_info));
		}
		else
		{
			if (is_reload == FALSE)
			{
				SUNLOG(eCRITICAL_LOG, 
                       _T("Script File Data Error, File = %s, abuse_type = %d, abuse_sub_type = %d"), 
                       GetCurFileName(), abuse_type, abuse_sub_type);
				ASSERT(!"데이터에 오류가 있습니다.");
			}
		}

		abusing_option_info->m_AbuseType = abuse_type;
		abusing_option_info->m_AbuseSubType = abuse_sub_type;

		strncpy(abusing_option_info->m_szCategoryName, GetDataString("szCategoryName", row), 64);
		strncpy(abusing_option_info->m_szViolationName, GetDataString("szViolationName", row), 64);

		abusing_option_info->m_bDisconnect = GetDataBYTE("bDisconnect", row);
		abusing_option_info->m_Level = GetDataBYTE("Level", row);
		abusing_option_info->m_MilestoneCount = GetDataWORD("MilestoneCount", row);
		abusing_option_info->m_IncAbusingPoint = GetDataWORD("IncAbusingPoint", row);
		abusing_option_info->m_MaxAbusingPoint = GetDataWORD("MaxAbusingPoint", row);

		if (abusing_option_info->m_MilestoneCount == 0)
		{
			SUNLOG(eCRITICAL_LOG, 
                   _T("[AbusingOptionParser] abuse_type[%u]abuse_sub_type[%u] m_MilestoneCount is 0!"), 
                   abuse_type, abuse_sub_type);
			return FALSE;
		}
	}

	return TRUE;
}

//#pragma warning ( pop )

BOOL AbusingOptionParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
	switch(script_code)
	{
	case SCRIPT_ABUSING_OPTION:
	case SCRIPT_ABUSING_OPTION_TXT:
		return LoadOptionInfo(is_reload);
	};
	return FALSE;
}
