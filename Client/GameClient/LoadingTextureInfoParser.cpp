#include "SunClientPrecompiledHeader.h"
#include "LoadingTextureInfoParser.h"

//------------------------------------------------------------------------------ 
LoadingTextureInfoParser::LoadingTextureInfoParser(void)
{
    loadingtexureinfo_vec().clear();
    loaded_texture_index_vector_.clear();
    shuffle_index_ = -1;
}
//------------------------------------------------------------------------------ 
LoadingTextureInfoParser::~LoadingTextureInfoParser(void)
{
}
//------------------------------------------------------------------------------ 
void LoadingTextureInfoParser::Release()
{
    loadingtexureinfo_vec().clear();
    loaded_texture_index_vector_.clear();
    shuffle_index_ = -1;
}
//------------------------------------------------------------------------------ 
BOOL LoadingTextureInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    if (bReload == TRUE)
    {
        loadingtexureinfo_vec().clear();
    }

    int nRowSize = GetRowSize();
    for (int nRow = 0; nRow < nRowSize; ++nRow)
    {
        BASE_LoadingTextureInfo info;

        //info.index = GetDataInt("index", nRow);
        info.file_name = GetDataString("file_name", nRow);

        loadingtexureinfo_vec().push_back(info);
    }
    shuffle_index_ = -1;

    return TRUE;
}
//------------------------------------------------------------------------------ 
BASE_LoadingTextureInfo* LoadingTextureInfoParser::GetLoadingTextureInfo(INOUT int& index)
{
    //! 마지막 이미지는 탑 로딩 이미지(최소 2장)
    //! 랜덤으로 돌릴때는 마지막것을 제외한것으로 한다.
    if (loadingtexureinfo_vec().size() <= 0)
    {
        return NULL;
    }
    else if (loadingtexureinfo_vec().size() <= 1)
    {
        index = 0;
    }
    else
    {
        if (index < 0)
        {
            // 이전에 로딩된 것이 없다면
            if (shuffle_index_ == -1)
            {
                loaded_texture_index_vector_.clear();
                for (unsigned int i = 0; i < loadingtexureinfo_vec().size() - 1; ++i)
                {
                    loaded_texture_index_vector_.push_back(i);
                }
                std::random_shuffle(loaded_texture_index_vector_.begin(), loaded_texture_index_vector_.end());
                shuffle_index_ = loaded_texture_index_vector_.size() - 1;
            }
            
            index = loaded_texture_index_vector_[shuffle_index_];
            --shuffle_index_;
        }
        else if (index >= (int)loadingtexureinfo_vec().size())
        {
            // 탑 이라고 생각하고 마지막 것을 리턴한다.
            //! 마지막것
            index = loadingtexureinfo_vec().size() - 1;
        }
    }

    return &(loadingtexureinfo_vec_[index]);
}