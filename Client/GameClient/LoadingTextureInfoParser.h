#ifndef _LoadingTextureInfoParser_h_
#define _LoadingTextureInfoParser_h_

#pragma once

#include "UtilityCommon.h"

//------------------------------------------------------------------------------ 
struct BASE_LoadingTextureInfo
{
    //int index;
    std::string file_name;
};

//------------------------------------------------------------------------------ 
class LoadingTextureInfoParser	: public Singleton<LoadingTextureInfoParser>, public IParser
{
    typedef std::vector<BASE_LoadingTextureInfo> LoadingTexureInfo_vector;

public:
    LoadingTextureInfoParser(void);
    ~LoadingTextureInfoParser(void);

    virtual void Release();
    virtual	BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    //------------------------------------------------------------------------------
    //! Returns:   BASE_LoadingTextureInfo*
    //! Qualifier: �ε�ȭ�鿡 ������ �̹��� ����
    //! Parameter: int index -1: �̸� ����, ������ ������ �����Ѵ�.
    //------------------------------------------------------------------------------
    BASE_LoadingTextureInfo* GetLoadingTextureInfo(INOUT int& index);

    LoadingTexureInfo_vector& loadingtexureinfo_vec() { return loadingtexureinfo_vec_; }

private:
    LoadingTexureInfo_vector loadingtexureinfo_vec_;
    std::vector<int>         loaded_texture_index_vector_;
    int                      shuffle_index_;
};



#endif // _LoadingTextureInfoParser_h_
