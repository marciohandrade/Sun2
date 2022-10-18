#include "SunClientPrecompiledHeader.h"
#include "EnduranceInfoParser.h"

#ifdef _NA_000000_20130114_RENEWER_UI

void EnduranceInfoParser::Release()
{
	Unload();
}

BOOL EnduranceInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch (ScriptCode)
	{
	case SCRIPT_ENDURANCE_INFO:
		return _Load(bReload);
	default:
		break;
	}

	return FALSE;
}
bool EnduranceInfoParser::FindTypeData( std::vector<EnduranceInfo*>* MissionHelpInfo,int type )
{
	return false;
}


bool EnduranceInfoParser::FindData( EnduranceInfo& MissionHelpInfo,int type,int itemIndex )
{
	for (int i = 0 ; i < (int)EnduranceInfo_info_.size() ; ++i)
	{
		if(EnduranceInfo_info_[i]->type_ == type && EnduranceInfo_info_[i]->item_index_ == itemIndex)
		{
			MissionHelpInfo = *EnduranceInfo_info_[i];
			return true;
		}
	}

	return false;
}


BOOL EnduranceInfoParser::_Load( BOOL bReload )
{
	if (bReload == TRUE)
	{
		Unload();
	}

	int nRowSize = GetRowSize();

	EnduranceInfo_info_.resize(nRowSize);

	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		EnduranceInfo* info = new EnduranceInfo();

		info->id_			= GetDataInt("ID",nRow);
		info->type_			= GetDataInt("TYPE",nRow);
		info->item_index_	= GetDataInt("ITEMIDX",nRow);
		info->x_position	= GetDataInt("X",nRow);
		info->y_position	= GetDataInt("Y",nRow);
		info->width_		= GetDataInt("W",nRow);
		info->height_		= GetDataInt("H",nRow);

		EnduranceInfo_info_[nRow] = info;
	}

	return true;
}

void EnduranceInfoParser::Unload()
{
	EnduranceInfo_list::const_iterator it, end( EnduranceInfo_info_.end() );
	for(it = EnduranceInfo_info_.begin() ; it != end ; ++it)
	{
		EnduranceInfo* info = (*it);
		SAFE_DELETE(info);
	}

	EnduranceInfo_info_.clear();
}

EnduranceInfoParser::EnduranceInfoParser()
{

}

EnduranceInfoParser::~EnduranceInfoParser()
{

}
#endif

