#ifndef _TextureAnimationManager_h_
#define _TextureAnimationManager_h_

#pragma once

struct BASE_TextureListInfo;
struct BASE_TextureAnimationInfo;

enum TexAniGroup
{
    kTexAniGroup_Sword = 1,
    kTexAniGroup_SUN = 2,
    kTexAniGroup_EP2 = 3,
    kTexAniGroup_EP2_Effect = 4,

    kTexAniGroup_Max
};

//------------------------------------------------------------------------------ 
class TextureAnimationManager : public Singleton<TextureAnimationManager>
{
public:
    struct TexAniKey
    {
		DWORD					Group_index;
        HANDLE                  tex_handle;
        BASE_TextureListInfo*   tex_info;
        BASE_TextureAnimationInfo*  ani_info;
    };
    struct TexAniInterpolateKey
    {
        HANDLE          tex_handle;
        ENUM_ALPHABLEND blend_mode;
        float           alpha;
        RECT            scr_rect;
        RECT            dest_rect;
    };
    class TexAni
    {
    public:
        bool        GetValue(OUT TexAniInterpolateKey& key, DWORD tick);

    //private:
        TexAniKey*  GetLessThenValue(DWORD tick);
        TexAniKey*  GetGreatValue(DWORD tick);

        DWORD                   start_tick;
        DWORD                   group_index;
        std::vector<TexAniKey>  ani_key;
    };
public:
    TextureAnimationManager(void);
    ~TextureAnimationManager(void);

    bool Play(DWORD group_index);
    bool Load(DWORD group_index);
    void Clear(DWORD group_index);
    void Release();
    void Render();

	DWORD	getticktime()	{ return tick_time_animation_;}
    std::vector<TexAni>& ani_vec() { return ani_vec_; }
private:
    void DestoryTexture(TexAni* tex_ani);

    std::vector<TexAni> ani_vec_;

	DWORD	tick_time_animation_;
};



#endif // _TextureAnimationManager_h_
