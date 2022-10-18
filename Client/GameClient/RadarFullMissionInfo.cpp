#include "SunClientPrecompiledHeader.h"
#include "radarfullmissioninfo.h"
#include "IconMgr.h"

#ifdef _NA_000000_20120911_FULL_MISSION_HELPER
void RadarFullMissionInfoParser::Release()
{
	Unload();
}

BOOL RadarFullMissionInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch (ScriptCode)
	{
	case SCRIPT_MISSIONHELP_INFO:
		return _Load(bReload);
	}

	return FALSE;
}

BOOL RadarFullMissionInfoParser::_Load( BOOL bReload )
{
	if (bReload == TRUE)
	{
		Unload();
	}

	int nRowSize = GetRowSize();

	MissionHelper_info_.resize(nRowSize);

	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		MissionData* info = new MissionData();
		
		info->MapID_ = GetDataInt("MapCode",nRow);
		info->FieldID_ = GetDataInt("FieldCode",nRow);
		info->IconID_ = GetDataInt("IconIndex",nRow);
		info->IconPosX = GetDataFloat("UIPosX",nRow);
		info->IconPosY = GetDataFloat("UIPosY",nRow);
		info->IconPosZ = GetDataFloat("UIPosZ",nRow);
		info->StringID  = GetDataInt("Tooltip",nRow);
		DeleteStringToAddString(info->color_,GetDataString("TextColor",nRow),1,"0x");

		MissionHelper_info_[nRow] = info;
	}

	if (hRadarTexture_==INVALID_HANDLE_VALUE)
	{
		//너무 긴 라인 줄임
		hRadarTexture_ = g_pSunRenderer->x_pManagerTexture->\
			LoadTexture("Data\\Interface\\all_radarpoint.tga" 
			,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

		St_TextureContainer* pTextureInfo;

		pTextureInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(hRadarTexture_);

		RadarTextureSize_.x = 0;
		RadarTextureSize_.x = 0;

		//크레쉬 수정
		if(pTextureInfo)
		{
			RadarTextureSize_.x = pTextureInfo->m_nImageWidth;
			RadarTextureSize_.y = pTextureInfo->m_nImageHeight;
		}
		
	}

	return true;
}

void RadarFullMissionInfoParser::Unload()
{
	MissionHelper_list::const_iterator it, end( MissionHelper_info_.end() );
	for(it = MissionHelper_info_.begin() ; it != end ; ++it)
	{
		MissionData* info = (*it);
		SAFE_DELETE(info);
	}

	MissionHelper_info_.clear();

	if (hRadarTexture_ != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(hRadarTexture_);
		hRadarTexture_ = INVALID_HANDLE_VALUE;
	}
}

RadarFullMissionInfoParser::RadarFullMissionInfoParser()
{
	hRadarTexture_ = INVALID_HANDLE_VALUE;
	RadarTextureSize_.x = 0;
	RadarTextureSize_.y = 0;

	RadarBasePos_.top = 802;
	RadarBasePos_.left = 0;
	RadarBasePos_.right = 27;
	RadarBasePos_.bottom = 27;

	RadarOverPos_.top = 829;
	RadarOverPos_.left = 0;
	RadarOverPos_.right = 27;
	RadarOverPos_.bottom = 27;

}

RadarFullMissionInfoParser::~RadarFullMissionInfoParser()
{

}

bool RadarFullMissionInfoParser::FindData( std::vector<MissionData*>* MissionHelpInfo,int mapcode,int fieldcode )
{
	for (int i = 0 ; i < (int)MissionHelper_info_.size() ; ++i)
	{
		if(MissionHelper_info_[i]->FieldID_ == fieldcode && MissionHelper_info_[i]->MapID_ == mapcode)
		{
			(*MissionHelpInfo).push_back(MissionHelper_info_[i]);
		}
	}

	return true;
}

RECT RadarFullMissionInfoParser::GetInputReturnRealSize( int Number,bool isMouseOver )
{
	RECT isPos;

	if (Number == 0)
	{
		if(isMouseOver)
		{
			isPos.left = 384;
			isPos.right = 32;
			isPos.bottom = 32;
			isPos.top = 608;
		}
		else
		{
			isPos.left = 352;
			isPos.right = 32;
			isPos.bottom = 32;
			isPos.top = 608;
		}
	}
	else
	{
		if(isMouseOver)
		{
			isPos = RadarOverPos_;

			//번호는 1부터 시작함으로 최초 1을 빼야됨
			isPos.left = (27 * (Number-1));


		}
		else
		{
			isPos = RadarBasePos_;

			//번호는 1부터 시작함으로 최초 1을 빼야됨
			isPos.left = (27 * (Number-1));
		}
	}


	return isPos;
}

__forceinline void RadarFullMissionInfoParser::DeleteStringToAddString( TCHAR* OutText,const TCHAR* InputText , int OutCount,const TCHAR* AddString )
{
	TCHAR TempString[512];
	memset(TempString,0,sizeof(TCHAR)*512);

	for (int idx = OutCount, count = 0 ; idx < (int)strlen(InputText) + 1 ; ++idx,++count)
	{
		TempString[count] = InputText[idx];
	}

	sprintf(OutText,"%s%s",AddString,TempString);
}
#endif //#ifdef _NA_000000_20120911_FULL_MISSION_HELPER