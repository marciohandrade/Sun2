#ifndef _TextureAnimationInfoParser_h_
#define _TextureAnimationInfoParser_h_

#pragma once

enum HORIZONTAL_ARRANGE;
enum VERTICAL_ARRANGE;

//------------------------------------------------------------------------------ 
struct BASE_TextureAnimationInfo
{
    DWORD           group_index;
    DWORD           tick_millisecond;       
    float           alpha;
    ENUM_ALPHABLEND blend_mode;

    //! 좌표 정렬
    HORIZONTAL_ARRANGE  align_x;
    VERTICAL_ARRANGE    align_y;

    //! x, y, width, height
    int     dest_x;
    int     dest_y;
    int     dest_wide_x;
    int     dest_wide_y;
    int     dest_width;
    int     dest_height;

    int     dest_half_wide_x;
    int     dest_half_wide_y;

    //! BASE_TextureListInfo::texture_index
    DWORD   texture_index;
};

//------------------------------------------------------------------------------ 
class TextureAnimationInfoParser : public Singleton<TextureAnimationInfoParser>, public IParser
{
    typedef std::map< DWORD /* group_index */, std::vector<BASE_TextureAnimationInfo> > AnimationInfo_map;

public:
    TextureAnimationInfoParser(void);
    ~TextureAnimationInfoParser(void);

    virtual void Release();
    virtual	BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    bool GetDestRect(OUT RECT& out_rect, BASE_TextureAnimationInfo* info);

    //------------------------------------------------------------------------------
    //! Returns: int : 이미지 크기까지 감안된 위치
    //! Qualifier: 이미지를 그릴 위치
    //------------------------------------------------------------------------------
    int GetX(BASE_TextureAnimationInfo* info, RECT& window_rect);
    int GetY(BASE_TextureAnimationInfo* info, RECT& window_rect);

    std::vector<BASE_TextureAnimationInfo>* animation_key(DWORD group_index);

    AnimationInfo_map& animationinfo_map() { return animationinfo_map_; }

private:
    AnimationInfo_map animationinfo_map_;
};



#endif // _TextureAnimationInfoParser_h_
