#ifndef _CreateCharCustomCameraInfoParser_h_
#define _CreateCharCustomCameraInfoParser_h_

#pragma once

//------------------------------------------------------------------------------ 
struct BASE_CreateCharCustomCameraInfo
{
    eCHAR_TYPE char_type;
    WzVector target_offset;
    WzVector eye_direction;
    WzVector eye_offset;

    float char_rot;
    float dist_min;
    float dist_max;
};

//------------------------------------------------------------------------------ 
class CreateCharCustomCameraInfoParser	: public Singleton<CreateCharCustomCameraInfoParser>, public IParser
{
    typedef std::map<eCHAR_TYPE, BASE_CreateCharCustomCameraInfo> CameraInfo_map;

public:
    CreateCharCustomCameraInfoParser(void);
    ~CreateCharCustomCameraInfoParser(void);

    virtual void Release();
    virtual	BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    BASE_CreateCharCustomCameraInfo* GetCustomCameraInfo(eCHAR_TYPE char_type);

    CameraInfo_map& camerainfo_map() { return camerainfo_map_; }

private:
    CameraInfo_map camerainfo_map_;
};



#endif // _CreateCharCustomCameraInfoParser_h_
