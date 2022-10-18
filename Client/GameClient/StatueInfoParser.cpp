#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "StatueInfoParser.h"

StatueInfoParser::StatueInfoParser(void)
{
}

StatueInfoParser::~StatueInfoParser(void)
{
}

VOID StatueInfoParser::Release()
{
	Unload();
}

VOID StatueInfoParser::Init( DWORD pool_size )
{

}

VOID StatueInfoParser::Init( DWORD pool_size, char* pack_file_name )
{
	SetPackFileName( pack_file_name );
}

#pragma warning ( push )
#pragma warning (disable : 4244 )
BOOL StatueInfoParser::LoadScript( eSCRIPT_CODE script_code, BOOL bReload )
{
	int row_size = GetRowSize();
	for ( int row_count = 0; row_count < row_size; ++row_count )
	{
		for ( int statue_count = 0; statue_count <= 1; ++statue_count )
		{
			TCHAR attribute_string[INTERFACE_STRING_LENGTH] = {0, };
			Snprintf(attribute_string, INTERFACE_STRING_LENGTH-1, _T("statue%dcreate"), statue_count);

			if ( GetDataNumber(attribute_string, row_count) == 1 )
			{
				BASE_StatueInfo* statue_info_ptr = new BASE_StatueInfo;

				// 积己
				Snprintf(attribute_string, INTERFACE_STRING_LENGTH-1, _T("statue%dcreate"), statue_count);
				statue_info_ptr->is_create_ = GetDataInt(attribute_string, row_count);
				// position
				Snprintf(attribute_string, INTERFACE_STRING_LENGTH-1, _T("statue%dX"), statue_count);
				statue_info_ptr->position_.x = GetDataFloat(attribute_string, row_count);
				Snprintf(attribute_string, INTERFACE_STRING_LENGTH-1, _T("statue%dY"), statue_count);
				statue_info_ptr->position_.y = GetDataFloat(attribute_string, row_count);
				Snprintf(attribute_string, INTERFACE_STRING_LENGTH-1, _T("statue%dZ"), statue_count);
				statue_info_ptr->position_.z = GetDataFloat(attribute_string, row_count);

				// direction
				Snprintf(attribute_string, INTERFACE_STRING_LENGTH-1, _T("statue%ddirectionX"), statue_count);
				statue_info_ptr->direction_.x = GetDataFloat(attribute_string, row_count);
				Snprintf(attribute_string, INTERFACE_STRING_LENGTH-1, _T("statue%ddirectionY"), statue_count);
				statue_info_ptr->direction_.y = GetDataFloat(attribute_string, row_count);
				Snprintf(attribute_string, INTERFACE_STRING_LENGTH-1, _T("statue%ddirectionZ"), statue_count);
				statue_info_ptr->direction_.z = GetDataFloat(attribute_string, row_count);

				// scale
				Snprintf(attribute_string, INTERFACE_STRING_LENGTH-1, _T("statue%dscale"), statue_count);
				statue_info_ptr->scale_value_ = GetDataFloat(attribute_string, row_count);

				// 局聪皋捞记 内靛
				Snprintf(attribute_string, INTERFACE_STRING_LENGTH-1, _T("statue%danimation"), statue_count);
				statue_info_ptr->animation_code_ = GetDataString(attribute_string, row_count);

				std::map<MAPCODE, std::list<BASE_StatueInfo*> >::iterator iter;
				MAPCODE map_code = GetDataNumber("mapcode", row_count);

				iter = statue_info_map_.find(map_code);
				if ( iter == statue_info_map_.end() )
				{
					std::list<BASE_StatueInfo*> statue_info_list;
					statue_info_list.push_back(statue_info_ptr);
					statue_info_map_.insert(make_pair<MAPCODE, std::list<BASE_StatueInfo*> >(map_code, statue_info_list));
				}
				else
				{
					(iter->second).push_back(statue_info_ptr);
				}
			}
		}
	}
	return TRUE;
}

VOID StatueInfoParser::Unload()
{
	std::map<MAPCODE, std::list<BASE_StatueInfo*> >::iterator iter = statue_info_map_.begin();
	std::list<BASE_StatueInfo*>::iterator list_iter;
	for ( ; iter != statue_info_map_.end(); ++iter)
	{
		if ((iter->second).size() > 0)
		{
			for ( list_iter = (iter->second).begin(); list_iter != (iter->second).end(); )
			{
				SAFE_DELETE((*list_iter));

				list_iter = (iter->second).erase(list_iter);
			}
		}
	}
}

bool StatueInfoParser::get_statue_info_list( MAPCODE map_code, OUT std::list<BASE_StatueInfo*>* statue_info_list )
{
	if (map_code == 0)
	{
		return false;
	}

	std::map<MAPCODE, std::list<BASE_StatueInfo*> >::iterator iter;
	iter = statue_info_map_.find(map_code);
	if ( iter == statue_info_map_.end() )
	{
		return false;
	}

	std::list<BASE_StatueInfo*>::iterator list_iter;
	for ( list_iter = (iter->second).begin(); list_iter != (iter->second).end(); ++list_iter)
	{
		statue_info_list->push_back((*list_iter));
	}

	return true;
}

#pragma warning ( pop )
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
