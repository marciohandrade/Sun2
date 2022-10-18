#include "SunClientPrecompiledHeader.h"
#include "TextureAnimationManager.h"
#include "TextureAnimationInfoParser.h"
#include "TextureListInfoParser.h"
#include "GameFunc.h"

//------------------------------------------------------------------------------ 
bool TextureAnimationManager::TexAni::GetValue(OUT TexAniInterpolateKey& key, DWORD tick)
{
    TexAniKey* less_key = GetLessThenValue(tick);
    TexAniKey* great_key = GetGreatValue(tick);
    if (less_key == NULL)
    {
        return false;
    }

    if (great_key == NULL)
    {
        key.scr_rect = less_key->tex_info->tex_size;
        key.tex_handle = less_key->tex_handle;
        key.blend_mode = less_key->ani_info->blend_mode;
        key.alpha = less_key->ani_info->alpha;

        TextureAnimationInfoParser::Instance()->GetDestRect(key.dest_rect, less_key->ani_info);
        return true;
    }

    //!보간
    int gab = great_key->ani_info->tick_millisecond - less_key->ani_info->tick_millisecond;
    if (gab <= 0)
    {
        return false;
    }

    RECT less_rect, great_rect;

    TextureAnimationInfoParser::Instance()->GetDestRect(less_rect, less_key->ani_info);
    TextureAnimationInfoParser::Instance()->GetDestRect(great_rect, great_key->ani_info);

    float ratio = float(tick - less_key->ani_info->tick_millisecond) / float(gab);

    key.tex_handle  = (ratio > 0.5f) ? great_key->tex_handle : less_key->tex_handle;
    key.scr_rect    = (ratio > 0.5f) ? great_key->tex_info->tex_size : less_key->tex_info->tex_size;
    key.blend_mode  = (ratio > 0.5f) ? great_key->ani_info->blend_mode : less_key->ani_info->blend_mode;

    key.alpha = GameFunc::GetInterpolationValue(less_key->ani_info->alpha, great_key->ani_info->alpha, ratio);
    key.dest_rect = GameFunc::GetInterpolationValue(less_rect, great_rect, ratio);

    return true;
}
//------------------------------------------------------------------------------ 
TextureAnimationManager::TexAniKey* TextureAnimationManager::TexAni::GetLessThenValue(DWORD tick)
{
    std::vector<TexAniKey>::reverse_iterator rbegin = ani_key.rbegin();
    std::vector<TexAniKey>::reverse_iterator rend = ani_key.rend();

    for (; rbegin != rend; ++rbegin)
    {
        if (rbegin->ani_info->tick_millisecond <= tick)
        {
            return &(*rbegin);
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
TextureAnimationManager::TexAniKey* TextureAnimationManager::TexAni::GetGreatValue(DWORD tick)
{
    std::vector<TexAniKey>::iterator begin = ani_key.begin();
    std::vector<TexAniKey>::iterator end = ani_key.end();

    for (; begin != end; ++begin)
    {
        if (begin->ani_info->tick_millisecond > tick)
        {
            return &(*begin);
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------ 
//! TextureAnimationManager
//------------------------------------------------------------------------------ 
TextureAnimationManager::TextureAnimationManager(void)
{
    ani_vec_.clear();
	tick_time_animation_ = 0;
}
//------------------------------------------------------------------------------ 
TextureAnimationManager::~TextureAnimationManager(void)
{
    Release();
}
//------------------------------------------------------------------------------ 
bool TextureAnimationManager::Play(DWORD group_index)
{
    std::vector<TexAni>::iterator ani_begin = ani_vec().begin();
    std::vector<TexAni>::iterator ani_end = ani_vec().end();

    for (; ani_begin != ani_end; ++ani_begin)
    {
        TexAni* tex_ani = &(*ani_begin);

        if (tex_ani->group_index == group_index)
        {
            tex_ani->start_tick = clock_function::GetTickCount();
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------ 
bool TextureAnimationManager::Load(DWORD group_index)
{
    std::vector<BASE_TextureAnimationInfo>* ani_key = TextureAnimationInfoParser::Instance()->animation_key(group_index);
    if ((ani_key == NULL) || (ani_key->size() <= 0))
    {
        return false;
    }

    TexAni tex_ani;

    std::vector<BASE_TextureAnimationInfo>::iterator ani_key_begin = ani_key->begin();
    std::vector<BASE_TextureAnimationInfo>::iterator ani_key_end = ani_key->end();
    for (; ani_key_begin != ani_key_end; ++ani_key_begin)
    {
        BASE_TextureAnimationInfo* ani_info = &(*ani_key_begin);

        HANDLE tex_handle = NULL;
        BASE_TextureListInfo* tex_info = TextureListInfoParser::Instance()->GetTextureInfo(ani_info->texture_index);
        if (tex_info != NULL)
        {
            tex_handle = g_pSunRenderer->x_pManagerTexture->LoadTexture(tex_info->file_name.c_str(), 
                                            TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
        }

        TexAniKey key;
		key.Group_index = group_index;
        key.tex_handle = tex_handle;
        key.ani_info = ani_info;
        key.tex_info = tex_info;

        tex_ani.ani_key.push_back(key);
    }

    tex_ani.group_index = group_index;
    tex_ani.start_tick = 0;

    ani_vec().push_back(tex_ani);

    return true;
}

//------------------------------------------------------------------------------ 
void TextureAnimationManager::Clear(DWORD group_index)
{
    std::vector<TexAni>::iterator ani_begin = ani_vec().begin();
    std::vector<TexAni>::iterator ani_end = ani_vec().end();

    for (; ani_begin != ani_end; ++ani_begin)
    {
        TexAni* tex_ani = &(*ani_begin);

        if (tex_ani->group_index == group_index)
        {
            DestoryTexture(tex_ani);

            ani_vec().erase(ani_begin);
            return;
        }
    }
}
//------------------------------------------------------------------------------ 
void TextureAnimationManager::Release()
{
    std::vector<TexAni>::iterator ani_begin = ani_vec().begin();
    std::vector<TexAni>::iterator ani_end = ani_vec().end();

    for (; ani_begin != ani_end; ++ani_begin)
    {
        DestoryTexture( &(*ani_begin));
    }
    ani_vec().clear();
}
//------------------------------------------------------------------------------ 
void TextureAnimationManager::DestoryTexture(TexAni* tex_ani)
{
    std::vector<TexAniKey>::iterator key_begin = tex_ani->ani_key.begin();
    std::vector<TexAniKey>::iterator key_end = tex_ani->ani_key.end();

    for (; key_begin != key_end; ++key_begin)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(key_begin->tex_handle);
        key_begin->tex_handle = NULL;
    }
}
//------------------------------------------------------------------------------ 
void TextureAnimationManager::Render()
{
    ENUM_ALPHABLEND backup_blend_mode = g_pSunRenderer->GetAlphaBlendMode();

    std::vector<TexAni>::iterator ani_begin = ani_vec().begin();
    std::vector<TexAni>::iterator ani_end = ani_vec().end();

	DWORD NowGroupIdx;
    for (; ani_begin != ani_end; ++ani_begin)
    {
        TexAni* tex_ani = &(*ani_begin);

		NowGroupIdx = tex_ani->group_index;
        if (tex_ani->start_tick <= 0)
        {
            //! 아직 시작 안했음.
            continue;
        }

        DWORD elapse = clock_function::GetTickCount() - tex_ani->start_tick;
		tick_time_animation_ = elapse;

        TexAniInterpolateKey key;
        if (tex_ani->GetValue(key, elapse) == true)
        {
            g_pSunRenderer->SetAlphaBlendMode(key.blend_mode);

            WzColor color = WzColor_RGBA(255, 255, 255, int(key.alpha * 255.0f));
            WzColor vex_color[4] = {color, color, color, color};
            
            g_pSunRenderer->RenderTexture(key.tex_handle, 
                            static_cast<float>(key.dest_rect.left), 
                            static_cast<float>(key.dest_rect.top),      // dest x,y
                            static_cast<float>(key.dest_rect.right), 
                            static_cast<float>(key.dest_rect.bottom),  // dest w,h
                            static_cast<float>(key.scr_rect.left), 
                            static_cast<float>(key.scr_rect.top),        // src x,y
                            static_cast<float>(key.scr_rect.right), 
                            static_cast<float>(key.scr_rect.bottom),   // src w,h
                            vex_color);
        }
    }

    g_pSunRenderer->SetAlphaBlendMode(backup_blend_mode);
}
