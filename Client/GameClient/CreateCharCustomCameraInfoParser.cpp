#include "SunClientPrecompiledHeader.h"
#include "CreateCharCustomCameraInfoParser.h"

//------------------------------------------------------------------------------ 
CreateCharCustomCameraInfoParser::CreateCharCustomCameraInfoParser(void)
{
    camerainfo_map().clear();
}
//------------------------------------------------------------------------------ 
CreateCharCustomCameraInfoParser::~CreateCharCustomCameraInfoParser(void)
{
}
//------------------------------------------------------------------------------ 
void CreateCharCustomCameraInfoParser::Release()
{
    camerainfo_map().clear();
}
//------------------------------------------------------------------------------ 
BOOL CreateCharCustomCameraInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    if (bReload == TRUE)
    {
        camerainfo_map().clear();
    }

    int nRowSize = GetRowSize();
    for (int nRow = 0; nRow < nRowSize; ++nRow)
    {
        BASE_CreateCharCustomCameraInfo info;

        info.char_type = static_cast<eCHAR_TYPE>(GetDataDWORD("Index", nRow));
        info.target_offset.x = GetDataFloat("Target_Offset_X", nRow);
        info.target_offset.y = GetDataFloat("Target_Offset_Y", nRow);
        info.target_offset.z = GetDataFloat("Target_Offset_Z", nRow);

        info.eye_direction.x = GetDataFloat("Eye_Direction_X", nRow);
        info.eye_direction.y = GetDataFloat("Eye_Direction_Y", nRow);
        info.eye_direction.z = GetDataFloat("Eye_Direction_Z", nRow);

        info.eye_offset.x = GetDataFloat("Eye_Offset_X", nRow);
        info.eye_offset.y = GetDataFloat("Eye_Offset_Y", nRow);
        info.eye_offset.z = GetDataFloat("Eye_Offset_Z", nRow);

        info.char_rot = GetDataFloat("Char_Rot", nRow);
        info.dist_min = GetDataFloat("Dist_Min", nRow);
        info.dist_max = GetDataFloat("Dist_Max", nRow);

        camerainfo_map().insert(std::make_pair(info.char_type, info));
    }

    return TRUE;
}
//------------------------------------------------------------------------------ 
BASE_CreateCharCustomCameraInfo* CreateCharCustomCameraInfoParser::GetCustomCameraInfo(eCHAR_TYPE char_type)
{
    CameraInfo_map::iterator iter = camerainfo_map().find(char_type);
    if (iter != camerainfo_map().end())
    {
        return &(iter->second);
    }
    return NULL;
}