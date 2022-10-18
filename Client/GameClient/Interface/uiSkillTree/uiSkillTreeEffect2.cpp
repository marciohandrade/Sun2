#include "SunClientPrecompiledHeader.h"
#ifdef _NA_20111213_SKILLTREE_VISUAL_RENEWAL2
#ifndef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
#include "uiSkillTreeEffect2.h"
#include "uiSkillTree.h"
#include "SkillInfoParser.h"
#include "InterfaceManager.h"
#include "SkillMain.h"
#include "Command.h"
#include "SoundEffect.h"



enum eSkillTreeConst
{
    kMaxColumn = 6,
    kMaxRow = 6,
    kPointLevel = 5,        // 새로운 라인이 열리는데 사용되는 포인트 수
    kRowPointLevel = 25,
    kMaxSlot = 36,
};


//////////////////////////////////////////////////////////////////////////////////
class cPosition
{
private:
    float x_;
    float y_;

public:
    cPosition( float x, float y ) : x_(x), y_(y) {}

    float GetX() const { return x_; }
    float GetY() const { return y_; }
};

//////////////////////////////////////////////////////////////////////////////////
class cRect
    : public cPosition
{
private:
    float w_;
    float h_;

public:
    cRect( float x, float y, float w, float h )
        : cPosition( x, y )
        , w_(w)
        , h_(h)
    {
    }

    float GetW() const { return w_; }
    float GetH() const { return h_; }
};

//////////////////////////////////////////////////////////////////////////////////
class cImageFacade
{
private:
    static bool begined_;
    static WzColor old_color_;
    static ENUM_ALPHABLEND old_alpha_mode_;

public:
    cImageFacade()
    {
        begined_ =  false;
    }

    static void BeginRender()
    {
        old_color_ = g_pSunRenderer->GetDiffuseColor();
        old_alpha_mode_ = g_pSunRenderer->GetAlphaBlendMode();
        g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);

        begined_ = true;
    }

    //static void DrawImage(HANDLE texture, RECT& rect, cTreeImage& image, WzColor* color = NULL)
    static void Render( HANDLE texture, float x, float y, float w, float h, const cRect& image, WzColor* color )
    {
        if (color)
        {
            g_pSunRenderer->SetColor(*color);
        }
        else
        {
            g_pSunRenderer->SetColor( 255, 255, 255, 255 );
        }


        // left.right(시작위치보정) top.bottom(이미지크기보정)
        g_pSunRenderer->RenderTexture( texture,
            x, y,
            w, h,
            image.GetX(), image.GetY(),
            image.GetW(), image.GetH() );
    }

    static void Render( HANDLE texture, float x, float y, const cRect& image, WzColor* color )
    {
        Render( texture, x, y, image.GetW(), image.GetH(), image, color );
    }

    static void EndRender()
    {
        if( begined_ == true )
        {
            g_pSunRenderer->SetColor(old_color_);
            g_pSunRenderer->SetAlphaBlendMode(old_alpha_mode_);

            begined_ = false;
        }
    }
};

bool cImageFacade::begined_ = false;
WzColor cImageFacade::old_color_ = 0xFFFFFFFF;
ENUM_ALPHABLEND cImageFacade::old_alpha_mode_ = AB_NONE;


//////////////////////////////////////////////////////////////////////////////////
// 지역 변수
std::vector<cRect> local_image_array_;
std::vector<cPosition> local_position_array_;


//////////////////////////////////////////////////////////////////////////////////
struct cTreeImage
{
    float stx,sty;
    float srcx,srcy;
    float srcw,srch;
};

static cTreeImage chain_back ={42.f, 93.f, 0.f, 362.f, 310.f, 56.f};
static cTreeImage chain_off ={27.f, 83.f, 0.f, 418.f, 335.f, 47.f};
static cTreeImage chain_flush = {27.f, 83.f, 0.f, 465.f, 335.f, 47.f};
static cTreeImage chain_on ={27.f, 83.f, 0.f, 465.f, 335.f, 47.f};
static cTreeImage left_gauge_bar ={27.f, 94.f, 404.f, 113.f, 13.f, 336.f};
static cTreeImage right_gauge_bar ={354.f, 94.f, 404.f, 113.f, 13.f, 336.f};
static cTreeImage move_effect ={133.f, 50.f, 0.f, 0.f, 128.f, 128.f};


int SKILLTREE_ANIMATION_TIME = 2000;    // 5초


//------------------------------------------------------------------------------
/**
*/
static void DrawImageClip(HANDLE texture, RECT& rect, cTreeImage& image, WzColor* color = NULL)
{
    WzColor old_color = g_pSunRenderer->GetDiffuseColor();
    ENUM_ALPHABLEND alpha_mode = g_pSunRenderer->GetAlphaBlendMode();
    g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);
    if (color)
    {
        g_pSunRenderer->SetColor(*color);
    }
    else
    {
        g_pSunRenderer->SetColor( 255, 255, 255, 255 );
    }

    float dest_width = (float)rect.right - (float)rect.left;
    float dest_height = (float)rect.bottom - (float)rect.top;
    float src_width = image.srcw;
    float src_height = image.srch;
    if( dest_width < src_width )
        src_width = dest_width;
    if( dest_height < src_height )
        src_height = dest_height;

    // left.right(시작위치보정) top.bottom(이미지크기보정)
    g_pSunRenderer->RenderTexture(texture,
        rect.left + image.stx, rect.top + image.sty,
        dest_width, dest_height,
        image.srcx, image.srcy, 
        src_width, src_height );

    g_pSunRenderer->SetColor(old_color);
    g_pSunRenderer->SetAlphaBlendMode(alpha_mode);
}
//------------------------------------------------------------------------------
/**
*/
static void DrawImage(HANDLE texture, RECT& rect, cTreeImage& image, WzColor* color = NULL)
{
    WzColor old_color = g_pSunRenderer->GetDiffuseColor();
    ENUM_ALPHABLEND alpha_mode = g_pSunRenderer->GetAlphaBlendMode();
    g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);
    if (color)
    {
        g_pSunRenderer->SetColor(*color);
    }
    else
    {
        g_pSunRenderer->SetColor( 255, 255, 255, 255 );
    }


    float dest_width = (float)rect.right - (float)rect.left;
    float dest_height = (float)rect.bottom - (float)rect.top;
    float src_width = image.srcw;
    float src_height = image.srch;
    if( dest_width < src_width )
        src_width = dest_width;
    if( dest_height < src_height )
        src_height = dest_height;

    // left.right(시작위치보정) top.bottom(이미지크기보정)
    g_pSunRenderer->RenderTexture(texture,
        rect.left + image.stx, rect.top + image.sty,
        dest_width, dest_height,
        image.srcx, image.srcy, 
        src_width, src_height );

    //// left.right(시작위치보정) top.bottom(이미지크기보정)
    //g_pSunRenderer->RenderTexture(texture,
    //    rect.left + image.stx, rect.top + image.sty,
    //    (float)rect.right - (float)rect.left, (float)rect.bottom - (float)rect.top,
    //    image.srcx, image.srcy, 
    //    image.srcw, image.srch);

    g_pSunRenderer->SetColor(old_color);
    g_pSunRenderer->SetAlphaBlendMode(alpha_mode);
}

//------------------------------------------------------------------------------
/** 남은 스텟포인트
*/
static void DrawRemainStat( const RECT& dest_rect, DWORD remain_stat )
{
    RECT rect = {180 + dest_rect.left, 70 + dest_rect.top, 
                      207 + dest_rect.left, 370 + dest_rect.top};

    g_InterfaceManager.RenderDrawNumber( rect, remain_stat, DT_RIGHT | DT_TOP, eIMAGE_NUMBER_LARGE_YELLOW );
}

//------------------------------------------------------------------------------
static void DrawGaugeAnimation(const RECT& dest_rect, HANDLE texture, float gauge_position, int animation_time )
{
    // 게이지가 내려오는 연출중일때. 현재 위치까지는 배경을 깔아둔 후에 애니타임에 따라 내려온다.

    RECT gauge_rect = dest_rect;
    gauge_rect.bottom = LONG(left_gauge_bar.srch - gauge_position);

    WzColor color = WzColor_RGBA(128, 128, 128, 255);
    DrawImage(texture, gauge_rect, left_gauge_bar, &color);
    DrawImage(texture, gauge_rect, right_gauge_bar, &color);

    gauge_rect.bottom = LONG(left_gauge_bar.srch - animation_time);
    DrawImage(texture, gauge_rect, left_gauge_bar);
    DrawImage(texture, gauge_rect, right_gauge_bar);

}

////------------------------------------------------------------------------------
//static void DrawChain(const RECT& dest_rect, HANDLE texture, int chain_position )
//{
//    RECT draw_rect;
//    draw_rect.top = dest_rect.top + (LONG)chain_position*(LONG)chain_back.srch;
//    draw_rect.bottom = draw_rect.top + (LONG)chain_off.srch;
//    draw_rect.left = dest_rect.left;
//    draw_rect.right = draw_rect.left + (LONG)chain_off.srcw;
//
//    DrawImage( skill_ui_, draw_rect, chain_off, &color);
//}
//
//void uiSkillTreeEffect::DrawGauge( const RECT& dest_rect, DWORD left_point, DWORD right_point )
//{
//    HANDLE texture_handle = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Common_bg01.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
//    g_pSunRenderer->x_pManagerTexture->DestroyTexture( texture_handle );
//
//    // 스킬트리 창에서의 offet rect
//    static int draw_x = 10;
//    static int draw_y = 100;
//    static int draw_w = 12;
//    static int draw_h = 20;
//
//    // 그냥 해당위치까지 쭉
//    RECT gauge_rect = dest_rect;
//
//    gauge_rect.right = gauge_rect.left + (LONG)left_gauge_bar.srcw;
//    gauge_rect.bottom = gauge_rect.top + gauge_position;
//    DrawImageClip(texture, gauge_rect, left_gauge_bar);
//
//    gauge_rect = dest_rect;
//    gauge_rect.right = gauge_rect.left + (LONG)left_gauge_bar.srcw;
//    gauge_rect.bottom = gauge_rect.top + gauge_position;
//    DrawImageClip(texture, gauge_rect, right_gauge_bar);
//
//}

//------------------------------------------------------------------------------
static void DrawGauge(const RECT& dest_rect, HANDLE texture, int gauge_position )
{
    // 그냥 해당위치까지 쭉
    RECT gauge_rect = dest_rect;

    gauge_rect.right = gauge_rect.left + (LONG)left_gauge_bar.srcw;
    gauge_rect.bottom = gauge_rect.top + gauge_position;
    DrawImageClip(texture, gauge_rect, left_gauge_bar);

    gauge_rect = dest_rect;
    gauge_rect.right = gauge_rect.left + (LONG)left_gauge_bar.srcw;
    gauge_rect.bottom = gauge_rect.top + gauge_position;
    DrawImageClip(texture, gauge_rect, right_gauge_bar);
}


//------------------------------------------------------------------------------
static void DrawBlock( const RECT& dest_rect, HANDLE texture, int index, WzColor* color )
{
    RECT block_rect = dest_rect;
    block_rect.right = block_rect.left + (LONG)chain_back.srcw;
    block_rect.top = dest_rect.top + (LONG)index*(LONG)chain_back.srch;
    block_rect.bottom = block_rect.top + (LONG)chain_back.srch;

    DrawImage( texture, block_rect, chain_back, color );
}



//------------------------------------------------------------------------------
// class uiSkillTreeEffect
//------------------------------------------------------------------------------
uiSkillTreeEffect::uiSkillTreeEffect()
{
    is_playing_animateion_ = false;

    start_tab_ = 0;
    cur_tab_ = -1;

    arrow_texture_ = INVALID_HANDLE_VALUE;
    sound_handle_ = 0;

    InitArrowResource();
}

uiSkillTreeEffect::~uiSkillTreeEffect()
{
}

void uiSkillTreeEffect::Init()
{
    TCHAR skill_ui[] = "Data\\Interface\\all_buttenline_new1.tga";
    TCHAR skill_ui2[] = "Data\\Interface\\all_character.tga";
    TCHAR skill_effect[] = "Data\\Interface\\etc\\skill_eff.tga";
    TCHAR arrow_texture[] = "Data\\Interface\\etc\\all_text.tga";

    m_DrawTextureManager.Initialize();
    m_DrawTextureManager.AddImage( "chain_back",        skill_ui,       0, 362, 310, 56 );
    m_DrawTextureManager.AddImage( "chain_off",         skill_ui,       0, 418, 335, 47 );
    m_DrawTextureManager.AddImage( "chain_flush",       skill_ui2,       0, 465, 335, 47 );
    m_DrawTextureManager.AddImage( "chain_on",          skill_ui,       0, 465, 335, 47 );
    m_DrawTextureManager.AddImage( "gauge_bar",         skill_ui,       404, 113, 13, 336 );
    m_DrawTextureManager.AddImage( "max_level",         skill_ui,       0, 279, 42, 52 );


    char temp[256];
    for( size_t i=0;i!=4;++i)
    {
        for( size_t j=0;j!=4;++j)
        {
            wsprintf(temp, "move_effect_%01d", i*4+j);
            m_DrawTextureManager.AddImage( temp, skill_effect,   j*128, i*128, 128, 128 );
        }
    }



    CManagerTexture* manager = g_pSunRenderer->x_pManagerTexture;
    arrow_texture_ = manager->LoadTexture("Data\\Interface\\etc\\all_text.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
}

void uiSkillTreeEffect::Release()
{
    m_DrawTextureManager.Release();

    if (arrow_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture( arrow_texture_ );
        arrow_texture_ = INVALID_HANDLE_VALUE;
    }
}

void uiSkillTreeEffect::SetPlayAnimation(bool value)
{
    is_playing_animateion_ = value; 
    if( value == false )
    {
        OnStopEffectAnimation();
    }
}

void uiSkillTreeEffect::SetTotalSkillLevel( int total_level, bool data_clear )
{
	int cur_tab = total_level / kPointLevel;
	if( cur_tab >= kMaxRow )
		cur_tab = kMaxRow;

	if( cur_tab > 0 && 
		cur_tab != cur_tab_ &&
        ( cur_tab == kMaxRow && cur_tab_ == 5 ) == false &&     // 5단에서 6단으로 넘어가는 경우는 연출을 패스
        cur_tab < kMaxRow + 1 &&
        data_clear == false )
	{
        start_tab_ = cur_tab_;

		cur_tab_ = cur_tab;
		cur_stat_ = total_level % kPointLevel;

        is_playing_animateion_ = true;
        started_animation_time_ = GetCurrentTime();

        OnStartEffectAnimation();
	}
	else
	{
        start_tab_ = cur_tab;
        cur_tab_ = cur_tab;
        cur_stat_ = total_level % kPointLevel;

        if( data_clear == true )
        {
            is_playing_animateion_ = false;
        }
	}
}

void uiSkillTreeEffect::RenderOpenEffect( const RECT& dest_rect )
{
    dest_rect_ = dest_rect;

    if( is_playing_animateion_ )
    {
        int delta_time = GetCurrentTime() - started_animation_time_;
        int max_time = SKILLTREE_ANIMATION_TIME;


        if( delta_time > max_time )
        {
            is_playing_animateion_ = false;
        }
        else
        {

            // DrawGauge
            {
                int stat = cur_tab_ * kRowPointLevel + cur_stat_ * kPointLevel;
                int max_gauge_height = kRowPointLevel * kMaxRow;  // 150.f

                int gauge_height = (int)(( left_gauge_bar.srch * min(stat, max_gauge_height) ) / max_gauge_height);

                RECT draw_rect = dest_rect;
                draw_rect.bottom = draw_rect.top + gauge_height;
                m_DrawTextureManager.Draw( "gauge_bar", draw_rect, 27, 94, 1.0f );
                m_DrawTextureManager.Draw( "gauge_bar", draw_rect, 354, 94, 1.0f );
            }

            // Ramain SkillPoint Number
            DrawRemainStat( dest_rect, xGetSkillData()->GetRemainUseablePoint() );


            RenderArrows();
            RenderLockImage();

            // 닫힌 블럭
            RenderBlocks( dest_rect, delta_time, max_time );
            RenderChain( dest_rect, delta_time, max_time );
            RenderTornado( dest_rect, delta_time, max_time );

        }
    }


    if( is_playing_animateion_ == false )
    {
        // DrawGauge
        {
            int stat = cur_tab_ * kRowPointLevel + cur_stat_ * kPointLevel;
            int max_gauge_height = kRowPointLevel * kMaxRow;  // 150.f

            int gauge_height = (int)(( left_gauge_bar.srch * min(stat, max_gauge_height) ) / max_gauge_height);

            RECT draw_rect = dest_rect;
            draw_rect.bottom = draw_rect.top + gauge_height;
            m_DrawTextureManager.Draw( "gauge_bar", draw_rect, 27, 94, 1.0f );
            m_DrawTextureManager.Draw( "gauge_bar", draw_rect, 354, 94, 1.0f );
        }

        // Ramain SkillPoint Number
        DrawRemainStat( dest_rect, xGetSkillData()->GetRemainUseablePoint() );


        RenderArrows();
        RenderLockImage();

        // 닫힌 블럭
        RenderBlocks( dest_rect, 0, 0 );
        RenderChain( dest_rect, 0, 0 );
        RenderTornado( dest_rect, 0, 0 );

    }
}

void uiSkillTreeEffect::RenderBlocks( RECT dest_rect, int delta_time, int max_time )
{
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
#else
    // 현재 위치에 해당하는 블럭은 상황에 따라 다르다.
    if( xGetSkillData()->IsSkillSimulated() == true )
    {
        int total_level = xGetSkillData()->GetSkillDataSet().GetTotalUsedPoint();
        int cur_tab = total_level / kPointLevel;
        if( cur_tab >= kMaxRow )
            cur_tab = kMaxRow;

        // 현재 위치보다 큰 블럭은 항상 그린다.
        for (int idx = 0; idx < uiSkillTree::GetMaxRowCount(); ++idx)
        {
            if (idx > cur_tab)
            {
                m_DrawTextureManager.Draw( "chain_back", dest_rect, 42, 93 + idx*56, 0.5f );
            }
        }
    }
    else if( is_playing_animateion_ == false )
    {
        // 현재 위치보다 큰 블럭은 항상 그린다.
        for (int idx = 0; idx < uiSkillTree::GetMaxRowCount(); ++idx)
        {
            if (idx > cur_tab_)
            {
                m_DrawTextureManager.Draw( "chain_back", dest_rect, 42, 93 + idx*56, 1.0f );
            }
        }
    }
    else
    {
        // 현재 위치보다 큰 블럭은 항상 그린다.
        for (int idx = 0; idx < uiSkillTree::GetMaxRowCount(); ++idx)
        {
            if (idx > cur_tab_)
            {
                m_DrawTextureManager.Draw( "chain_back", dest_rect, 42, 93 + idx*56, 1.0f );
            }
        }
    }
#endif
}

void uiSkillTreeEffect::RenderChain( RECT dest_rect, int delta_time, int max_time )
{
    if( xGetSkillData()->IsSkillSimulated() == true )
    {
        int total_level = xGetSkillData()->GetSkillDataSet().GetTotalUsedPoint();
        int chain_tab = min( total_level / kPointLevel, kMaxRow);

        while( chain_tab < kMaxRow -1 )
        {
            m_DrawTextureManager.Draw( "chain_off", dest_rect, 27, 83 + chain_tab*56+56, 0.5f );

            ++chain_tab;
        }
    }
    else if( is_playing_animateion_ == false )
    {
        int total_level = xGetSkillData()->GetSkillDataSet().GetTotalUsedPoint();
        int chain_tab = min( total_level / kPointLevel, kMaxRow);

        while( chain_tab < kMaxRow -1 )
        {
            m_DrawTextureManager.Draw( "chain_off", dest_rect, 27, 83 + chain_tab*56+56, 1.0f );

            ++chain_tab;
        }
    }
    else
    {
        int chain_tab = min( start_tab_ + 1, kMaxRow );

        while( chain_tab < kMaxRow )
        {
            // 연출대상
            if( chain_tab <= cur_tab_ )
            {

                if( delta_time + 550 > max_time && delta_time + 100 <= max_time )
                {
                    int remain_max = 450;
                    int remain_time = (max_time-100) - delta_time;

                    float color_rate = (float)remain_time / (float)remain_max;

                    m_DrawTextureManager.Draw( "chain_on", dest_rect, 27, 83 + chain_tab*56, color_rate );
                }
                else if( delta_time + 100 > max_time )
                {
                }
                else
                {
                    static float v0 = 0.001f;
                    static float v1 = 0.5f;
                    static float v2 = 0.5f;
                    static float v3 = 0.3f;
                    static float v4 = 0.7f;

                    float color_rate = cos(WZ_PI*(float)delta_time * v0 + WZ_PI*0.5f);
                    color_rate *= v1;
                    color_rate += v2;
                    color_rate *= v3;
                    color_rate += v4;

                    m_DrawTextureManager.Draw( "chain_on", dest_rect, 27, 83 + chain_tab*56, color_rate );
                }

                if( delta_time + 550 > max_time && delta_time + 350 <= max_time )
                {
                    int remain_max = 200;
                    int remain_time = (max_time-350) - delta_time;

                    float color_rate = 1.0f - (float)remain_time / (float)remain_max;
                    m_DrawTextureManager.Draw( "chain_flush", dest_rect, 27, 83 + chain_tab*56, color_rate );
                }
                else if( delta_time + 350 > max_time )
                {
                    int remain_max = 350;
                    int remain_time = max_time - delta_time;

                    float color_rate = (float)remain_time / (float)remain_max;
                    m_DrawTextureManager.Draw( "chain_flush", dest_rect, 27, 83 + chain_tab*56, color_rate );
                }
            }
            else
            {
                float color_rate = 0.5f;
                if( delta_time + 500 >= max_time )
                {
                    color_rate += (float)(delta_time - (max_time-500)) / 1000.f; // 0.0f ~ 0.5f)
                }

                m_DrawTextureManager.Draw( "chain_off", dest_rect, 27, 83 + chain_tab*56, color_rate );
            }

            ++chain_tab;
        }
    }
}

void uiSkillTreeEffect::RenderTornado( RECT dest_rect, int delta_time, int max_time )
{
    if( xGetSkillData()->IsSkillSimulated() == true )
    {
        return;
    }
    else if( is_playing_animateion_ == true )
    {
        if( delta_time + 100 > max_time )
            return;

        // 1. 프레임과 색상을 결정


        int time_per_frame = 60;   // 1프레임에 사용할 시간 0.06초 즉 초당 1.6666 프레임 (1000/60)

        // 회오리 이펙트
        // 총 16장의 스프라이트를 이용하며, 시작할 때 4개가 애니메이션 되고, 이후 나머지 12개가 루프를 돌며 애니메이션이 된다.
        int frame_index = 0;
        if( delta_time <= time_per_frame * 4 )
        {
            // 최초의 4프레임 (0,1,2,3 프레임)
            frame_index = delta_time / time_per_frame;
        }
        else
        {
            // 이후 12프레임의 루프(4~15)
            frame_index = (delta_time - time_per_frame*4 ) / time_per_frame;
            frame_index = frame_index % 12;
            frame_index += 4;
        }


        float color_rate = 1.0f;
        if( delta_time + 500 > max_time )
        {
            // 종료 1초직전 부터 감소
            color_rate = (float)((max_time - delta_time - 100)/400);
        }

        // 색설정
        WzColor color = WzColor_RGBA( 255, 255, 255, (BYTE)(color_rate*255) );


        // 2.출력
        int tornado_pos = min( start_tab_ + 1 , kMaxRow);
        while( tornado_pos <= cur_tab_ && tornado_pos < kMaxRow )
        {
            char temp[256];
            wsprintf(temp,"move_effect_%01d", frame_index );

            m_DrawTextureManager.Draw( temp, dest_rect, 133, 50 + tornado_pos*56, color_rate );

            ++tornado_pos;
        }
    }
}

//------------------------------------------------------------------------------
void uiSkillTreeEffect::RenderMaxLevel( const RECT& dest_rect )
{
    RECT draw_rect = dest_rect;
    draw_rect.right = draw_rect.left + 100;
    draw_rect.bottom = draw_rect.top + 100;
    m_DrawTextureManager.Draw( "max_level", draw_rect, -6, -5, 1.0f );
}

void uiSkillTreeEffect::AddArrowInfo( int arrow_position, int arrow_type, int skill_class_code, int skill_level )
{
    SkillTreeArrowInfo info;
    info.SetArrowPosition( arrow_position );
    info.SetArrowType( arrow_type );
    info.SetArrowSkillInfo( skill_class_code, skill_level );

    arrow_list_.push_back( info );
}

void uiSkillTreeEffect::InitArrowResource()
{
    local_image_array_.push_back( cRect( 0, 24, 17, 58 ));     // 좌측 짧은 화살 
    local_image_array_.push_back( cRect( 17, 24, 17, 58 ));    // 우측 짧은 화살
    local_image_array_.push_back( cRect( 94, 0, 17, 113 ));    // 좌측 긴 화살
    local_image_array_.push_back( cRect( 111, 0, 17, 113 ));   // 우측 긴 화살
    local_image_array_.push_back( cRect( 0, 91, 53, 37 ));     // 좌측 꺽인 화살
    local_image_array_.push_back( cRect( 0, 91, 53, 37 ));     // 우측 꺽인 화살

    local_image_array_.push_back( cRect( 35, 58, 32, 32 ));    // 잠김표시


    for( int i=0;i!=kMaxRow;++i)
    {
        for( int j=0;j!=kMaxColumn;++j)
        {
            if( j >= 3 )
            {
                local_position_array_.push_back( cPosition( 61.f + j*50.f + 6.f, 88.f + 57.f*i ) );
            }
            else
            {
                local_position_array_.push_back( cPosition( 61.f + j*50.f, 88.f + 57.f*i ) );
            }
        }
    }
}

void uiSkillTreeEffect::SetAnimationArrow( int skill_class_code, int skill_level )
{
    for( size_t i=0;i!=arrow_list_.size();++i)
    {
        // 화살표 목록중 활성화되어야 할 화살표가 있다면 활성화
        SkillTreeArrowInfo& arrow = arrow_list_[i];
        if( arrow.GetSkillClassCode() == skill_class_code && arrow.GetSkillLeve() <= skill_level )
        {
            arrow.StartAnimation();
        }
    }
}

void uiSkillTreeEffect::ResetLock()
{
    lock_info_array_.clear();

    SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for( size_t i=0;i!=skill_set.size();++i)
    {
        SkillData& skill_data = skill_set[i];

        // 스킬 등록 과정에서 요구 스킬이 있는 모든 스킬의 lock위치를 등록
        if( skill_data.GetRequiredSkillClassCode() > INVALID_SKILL_CLASS_CODE )
        {
            AddLockInfo( skill_data.GetSlotPosition(), skill_data.GetSkillClassCode());
        }
    }
}

void uiSkillTreeEffect::ResetArrow()
{
    for( size_t i=0;i!=arrow_list_.size();++i)
    {
        arrow_list_[i].SetHideState();
    }
}

void uiSkillTreeEffect::SetVisibleArrow( int skill_class_code, int skill_level )
{
    for( size_t i=0;i!=arrow_list_.size();++i)
    {
        // 화살표 목록중 활성화되어야 할 화살표가 있다면 활성화
        SkillTreeArrowInfo& arrow = arrow_list_[i];
        if( arrow.GetSkillClassCode() == skill_class_code && arrow.GetSkillLeve() <= skill_level )
        {
            arrow.SetVisibleState();
        }
    }
}

void uiSkillTreeEffect::RenderArrows()
{
    HANDLE texture = GetArrowTexture();
    if( texture == INVALID_HANDLE_VALUE )
        return;

    float rate;
    cImageFacade::BeginRender();

    for( size_t i=0;i!=arrow_list_.size();++i)
    {
        SkillTreeArrowInfo& arrow = arrow_list_[i];

        rate = arrow.GetImageRate();
        if( rate == 0.f )
            continue;

        int image_index = arrow.GetArrowType();
        int position = arrow.GetArrowPosition();

        if( image_index >= (int)local_image_array_.size() ||
            position >= (int)local_position_array_.size() )
            continue;

        // 기본 좌표를 먼저 구한다
        float x = (float)dest_rect_.left + local_position_array_.at(position).GetX();
        float y = (float)dest_rect_.top + local_position_array_.at(position).GetY();


        // 4, 5번의 꺽인 화살표 이미지이면 좌표를 수정
        if( arrow.GetArrowType() == 4 || arrow.GetArrowType() == 5 )
        {
            static int offset_x = -15;
            static int offset_y = 20;
            x += offset_x;
            y += offset_y;
        }

        WzColor color = WzColor_RGBA( 255, 255, 255, (BYTE)(rate*255) );

        // 출력
        cImageFacade::Render( texture, x, y, local_image_array_.at( image_index ), &color );

        //DrawImage( texture, dest_rect_, image, &color );
    }

    cImageFacade::EndRender();
}

void uiSkillTreeEffect::RenderLockImage()
{
    HANDLE texture = GetArrowTexture();
    if( texture == INVALID_HANDLE_VALUE )
        return;


    if( lock_info_array_.empty() == true )
    {
        return;
    }

    // 잠금 이미지의 정확한 위치를 설정하기 위한 offset
    static float offset_x = -8.f;
    static float offset_y = 11.f;

    cImageFacade::BeginRender();

    float x, y, rate;

    for( size_t i=0;i!=lock_info_array_.size();++i)
    {
        int position = lock_info_array_[i].GetPosition();

        x = (float)dest_rect_.left + local_position_array_.at(position).GetX();
        y = (float)dest_rect_.top + local_position_array_.at(position).GetY();

        rate = lock_info_array_[i].GetImageRate();

        if( rate == 0.f )
            continue;

        x += offset_x;
        y += offset_y;

        WzColor color = WzColor_RGBA( 255, 255, 255, (BYTE)(rate*255) );

        cImageFacade::Render( texture, x, y, local_image_array_.at( 6 ), &color );
    }

    cImageFacade::EndRender();
}

void uiSkillTreeEffect::OnStartEffectAnimation()
{
    if( sound_handle_ != 0 )
    {
        g_SoundEffect.StopUI( sound_handle_ );
    }
    sound_handle_ = g_SoundEffect.PlayUI( UI_SOUND_SKILLTREE_BLUEEFFECT );
}

void uiSkillTreeEffect::OnStopEffectAnimation()
{
    if( sound_handle_ != 0 )
    {
        g_SoundEffect.StopUI( sound_handle_ );
        sound_handle_ = 0;
    }
}

#endif
#endif //_NA_20111213_SKILLTREE_VISUAL_RENEWAL2

