#include "SunClientPrecompiledHeader.h"
#ifndef _NA_20111213_SKILLTREE_VISUAL_RENEWAL2
#include "uiSkillTreeEffect.h"
#include "uiSkillTree.h"
#include "SkillInfoParser.h"
#include "InterfaceManager.h"
#include "SkillMain.h"
#include "Command.h"



enum eSkillTreeConst
{
    kMaxColumn = 6,
    kMaxRow = 6,
    kPointLevel = 5,        // ���ο� ������ �����µ� ���Ǵ� ����Ʈ ��
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


        // left.right(������ġ����) top.bottom(�̹���ũ�⺸��)
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
// ���� ����
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
static cTreeImage chain_on ={27.f, 83.f, 0.f, 465.f, 335.f, 47.f};
static cTreeImage left_gauge_bar ={27.f, 94.f, 404.f, 113.f, 13.f, 336.f};
static cTreeImage right_gauge_bar ={354.f, 94.f, 404.f, 113.f, 13.f, 336.f};
static cTreeImage move_effect ={133.f, 50.f, 0.f, 0.f, 128.f, 128.f};
static cTreeImage move_effect_loop ={133.f, 50.f, 0.f, 128.f, 128.f, 128.f};

static int tick_per_time_ = 36;

static float state_time_[OpenLevelEffect::kAniEnd] = 
{
    0.f,  // kAniNone
    0.f,  // kAniStart
    30.f, // kBlinkChain
    60.f, // kTornadoAppear
    56.f, // kChainMove
    20.f, // kChainMoveStop,
    62.f, // kBlockBlink
    20.f, // kBlockFadeOut
    20.f, // kBlockFadeOutStop  
    30.f, // kTornadoFadeOut
    20.f, // kBlinkChainFast
    20.f  // kChainFadeOut
};

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

    // left.right(������ġ����) top.bottom(�̹���ũ�⺸��)
    g_pSunRenderer->RenderTexture(texture,
                                  image.stx + rect.left, image.sty + rect.top,
                                  image.srcw, image.srch - rect.bottom,
                                  image.srcx, image.srcy, 
                                  image.srcw, image.srch - rect.bottom);

    g_pSunRenderer->SetColor(old_color);
    g_pSunRenderer->SetAlphaBlendMode(alpha_mode);
}

//------------------------------------------------------------------------------
/** ���� ��������Ʈ
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
    // �������� �������� �������϶�. ���� ��ġ������ ����� ��Ƶ� �Ŀ� �ִ�Ÿ�ӿ� ���� �����´�.

    RECT gauge_rect = dest_rect;
    gauge_rect.bottom = LONG(left_gauge_bar.srch - gauge_position);

    WzColor color = WzColor_RGBA(128, 128, 128, 255);
    DrawImage(texture, gauge_rect, left_gauge_bar, &color);
    DrawImage(texture, gauge_rect, right_gauge_bar, &color);

    gauge_rect.bottom = LONG(left_gauge_bar.srch - animation_time);
    DrawImage(texture, gauge_rect, left_gauge_bar);
    DrawImage(texture, gauge_rect, right_gauge_bar);

}

//------------------------------------------------------------------------------
static void DrawGauge(const RECT& dest_rect, HANDLE texture, float gauge_position )
{
    // �׳� �ش���ġ���� ��
    RECT gauge_rect = dest_rect;
    gauge_rect.bottom = LONG(left_gauge_bar.srch - gauge_position);

    DrawImage(texture, gauge_rect, left_gauge_bar);
    DrawImage(texture, gauge_rect, right_gauge_bar);
}


//------------------------------------------------------------------------------
static void DrawBlock( const RECT& dest_rect, HANDLE texture, int index, WzColor* color )
{
    RECT block_rect = dest_rect;
    block_rect.top = dest_rect.top + LONG(index*chain_back.srch);

    DrawImage( texture, block_rect, chain_back, color );
}



//------------------------------------------------------------------------------
// class uiSkillTreeEffect
//------------------------------------------------------------------------------
uiSkillTreeEffect::uiSkillTreeEffect()
{
    m_pAnimationChangedCommand = NULL;

    start_tab_ = 0;
    cur_tab_ = -1;

	skill_ui_ = INVALID_HANDLE_VALUE;
    skill_ui_2_ = INVALID_HANDLE_VALUE;
    skill_effect_ = INVALID_HANDLE_VALUE;
    arrow_texture_ = INVALID_HANDLE_VALUE;

    InitArrowResource();
}

uiSkillTreeEffect::~uiSkillTreeEffect()
{
    if( m_pAnimationChangedCommand != NULL )
    {
        delete m_pAnimationChangedCommand;
        m_pAnimationChangedCommand = NULL;
    }
}

void uiSkillTreeEffect::Init()
{
    open_effect().ani_state = OpenLevelEffect::kAniNone;
    open_effect().ani_time = 0;
    open_effect().Init_tornado = false;
    open_effect().tornado_time = 0;
    //open_effect().tornado_tick = 0;

    CManagerTexture* manager = g_pSunRenderer->x_pManagerTexture;

    skill_ui_ = manager->LoadTexture("Data\\Interface\\all_buttenline_new1.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    skill_ui_2_ = manager->LoadTexture("Data\\Interface\\all_character.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    skill_effect_ = manager->LoadTexture("Data\\Interface\\etc\\skill_eff.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    arrow_texture_ = manager->LoadTexture("Data\\Interface\\etc\\all_text.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
}

void uiSkillTreeEffect::Release()
{
    if (skill_ui_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture( skill_ui_ );
        skill_ui_ = INVALID_HANDLE_VALUE;
    }
    if (skill_ui_2_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture( skill_ui_2_ );
        skill_ui_2_ = INVALID_HANDLE_VALUE;
    }
    if (skill_effect_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture( skill_effect_ );
        skill_effect_ = INVALID_HANDLE_VALUE;
    }
    if (arrow_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture( arrow_texture_ );
        arrow_texture_ = INVALID_HANDLE_VALUE;
    }
}

void uiSkillTreeEffect::SetTotalSkillLevel( int total_level, bool data_clear )
{
	int cur_tab = total_level / kPointLevel;
	if( cur_tab >= kMaxRow )
		cur_tab = kMaxRow;

	if( cur_tab > 0 && 
		cur_tab != cur_tab_ &&
        ( cur_tab == kMaxRow && cur_tab_ == 5 ) == false &&     // 5�ܿ��� 6������ �Ѿ�� ���� ������ �н�
        cur_tab < kMaxRow + 1 &&
        data_clear == false )
	{
        start_tab_ = cur_tab_;

		cur_tab_ = cur_tab;
		cur_stat_ = total_level % kPointLevel;
        SetState( OpenLevelEffect::kAniStart );
	}
	else
	{
        start_tab_ = cur_tab;
        cur_tab_ = cur_tab;
        cur_stat_ = total_level % kPointLevel;

        if( data_clear == true )
        {
            SetState( OpenLevelEffect::kAniNone );
        }
	}
}

void uiSkillTreeEffect::UpdateGaugePosition()
{
    // ���� ��ų�ܰ� + ����� ����Ʈ ��ŭ ������ ������ ��ġ�� ������Ʈ

    int stat = cur_tab_ * kRowPointLevel + cur_stat_ * kPointLevel;
    int max_gauge_height = kRowPointLevel * kMaxRow;  // 150.f

    gauge_position_ = ( left_gauge_bar.srch * min(stat, max_gauge_height) ) / max_gauge_height;
}


void uiSkillTreeEffect::RenderOpenEffect( const RECT& dest_rect )
{
    if (skill_ui() == NULL)
    {
        return;
    }

    dest_rect_ = dest_rect;
    //next_tab_ = min( cur_tab_ + 1, kMaxRow);


    // 1. ������
    UpdateGaugePosition();

    if( GetOpenLevelEffectState() == OpenLevelEffect::kAniStart )
    {
        DWORD delta_time = GetCurrentTime() - animation_time_;
        int length = ( (DWORD)gauge_position_ * delta_time ) / animation_duration_;

        DrawGaugeAnimation( dest_rect_, skill_ui_, gauge_position_, length );
    }
    else
    {
        DrawGauge( dest_rect_, skill_ui_, gauge_position_ );
    }

    dest_rect_.bottom = 0; // �̰� ���ϴ°��� ������

    
    // 2. ��������
    DrawRemainStat( dest_rect_, xGetSkillData()->GetRemainUseablePoint() );

    RenderArrows();
    RenderLockImage();

    // 3. ���� ��
    RenderBlocks();

    // 4. ü��
    RenderChain();

    // 5. ����̵�
    RenderTornado();
    // 6. ����ó��
    ProcessOpenState();
}

void uiSkillTreeEffect::SetState( OpenLevelEffect::AnimationState state )
{
    if( state == OpenLevelEffect::kAniEnd )
    {
        state = OpenLevelEffect::kAniNone;
    }

    if( open_effect().ani_state == state )
    {
        return;
    }

    if( state == OpenLevelEffect::kTornadoAppear )
    {
        open_effect().Init_tornado = true;
        open_effect().tornado_time = GetCurrentTime();
        //open_effect().tornado_tick = 0;
    }

    render_color_ = WzColor_RGBA( 255, 255, 255, 255 );
    open_effect().ani_state = state;
    open_effect().ani_time = 0;

    animation_time_ = GetCurrentTime();
    animation_duration_ = (DWORD)(state_time_[state]) * tick_per_time_;

    if( state == OpenLevelEffect::kAniStart )
    {
        // �ִϸ��̼� �����Ҷ� �������� �����̵� �ð��� ���Ѵ�.
        int length = ((DWORD)gauge_position_ / (kRowPointLevel) ) * (kRowPointLevel);
        animation_duration_ = ( (DWORD)(length) * tick_per_time_ );
    }

    if( m_pAnimationChangedCommand != NULL )
    {
        m_pAnimationChangedCommand->Execute();
    }
}

int uiSkillTreeEffect::GetGaugeSlideHeight()
{
    return cur_tab_;
}

//------------------------------------------------------------------------------
/** �⺻���ΰ͵� �׸���
*/
void uiSkillTreeEffect::ProcessOpenState()
{
    OpenLevelEffect::AnimationState animation_state = GetOpenLevelEffectState();
    int animation_time = GetOpenLevelEffectAnimationTime();

    if ( animation_state == OpenLevelEffect::kAniNone)
    {
        return;
    }

    int delta_time = GetCurrentTime() - animation_time_;
    int ani_time = delta_time / tick_per_time_;

    // 0.06�ʸ� 1���������� ó��
    open_effect().ani_time = delta_time / tick_per_time_;

    // ����ó��
    switch(animation_state)
    {
    //case OpenLevelEffect::kChainMove:
    //    {
    //        if ((open_effect().tornado_tick%4) == 0)
    //        {
    //            return;
    //        }
    //    }
    //    break;
    case OpenLevelEffect::kBlockFadeOut:
    case OpenLevelEffect::kTornadoFadeOut:
    case OpenLevelEffect::kChainFadeOut:
        {
            float ratio = animation_time/state_time_[animation_state];
            float percent = 255.f - (255.f*ratio);
            render_color_ = WzColor_RGBA( 255, 255, 255, (int)percent );
        }
        break;
    }

    // �ִϸ��̼� �ð��� �˻��� ���¸� �����Ѵ�.
    if( animation_time_ + animation_duration_ < GetCurrentTime() )
    {
        SetState( (OpenLevelEffect::AnimationState)(open_effect().ani_state + 1) );
    }
}

//------------------------------------------------------------------------------
/** ��
    ��ġ�� 0 �϶� ù��° ������ ��ó���ϸ� �ȵȴ�. 
    �ִϸ��̼��� �����Ҷ� ��ġ�� 1�̵ǰ� �ִϸ��̼��� ������������ ���ؾ���
*/
void uiSkillTreeEffect::RenderBlocks()
{
    // ���� ��ġ�� �ش��ϴ� ���� ��Ȳ�� ���� �ٸ���.
    OpenLevelEffect::AnimationState animation_state = GetOpenLevelEffectState();
    int animation_time = GetOpenLevelEffectAnimationTime();


    HANDLE texture = skill_ui_;

    if( xGetSkillData()->IsSkillSimulated() == true )
    {
        WzColor color = WzColor_RGBA(255,255,255,128);

        int total_level = xGetSkillData()->GetSkillDataSet().GetTotalUsedPoint();
        int cur_tab = total_level / kPointLevel;
        if( cur_tab >= kMaxRow )
            cur_tab = kMaxRow;

        // ���� ��ġ���� ū ���� �׻� �׸���.
        for (int idx = 0; idx < uiSkillTree::kMaxRow; ++idx)
        {
            if (idx > cur_tab)
            {
                DrawBlock( dest_rect_, skill_ui_, idx, &color );
            }
        }
    }
    else
    {

        WzColor color = WzColor_RGBA(255,255,255,255);

        if( animation_state != OpenLevelEffect::kAniNone )
        {
            std::vector<int> array;
            int move_count = cur_tab_ - start_tab_;
            for( int i=0;i!=move_count;++i)
            {
                if( start_tab_ +i + 1 >= kMaxRow )
                    continue;
                array.push_back( start_tab_ +i + 1 );
            }


            switch ( animation_state )
            {
            case OpenLevelEffect::kTornadoAppear:
            case OpenLevelEffect::kAniStart:
            case OpenLevelEffect::kBlinkChain:
            case OpenLevelEffect::kChainMove:
            case OpenLevelEffect::kChainMoveStop:
                // �׳� ��
                for( size_t i=0;i!=array.size();++i)
                {
                    DrawBlock( dest_rect_, skill_ui_, array[i], &color );
                }
                break;

            case OpenLevelEffect::kBlockBlink:
                {

                    DWORD delta_time = GetCurrentTime() - animation_time_;
                    float time = (float)delta_time * WZ_PI * 2.0f / (3720.f/5.f);   // 3.720�ʿ� 5�� ����
                    float rate = cos( time ) * 0.5f + 0.5f;
                    color = WzColor_RGBA( 255, 255, 255, (BYTE)(rate*255) );

                    for( size_t i=0;i!=array.size();++i)
                    {
                        DrawBlock( dest_rect_, skill_ui_, array[i], &color );
                    }
                }
                break;

            case OpenLevelEffect::kBlockFadeOut:
                {
                    DWORD delta_time = GetCurrentTime() - animation_time_;
                    float rate = (float)delta_time/ (float)animation_duration_;
                    if( rate > 1.0f )
                        rate = 1.0f;
                    color = WzColor_RGBA( 255, 255, 255, (BYTE)((1.0f - rate)*255) );   // �������.

                    // ���̵� �ƿ��Ǹ� ������� ���
                    for( size_t i=0;i!=array.size();++i)
                    {
                        DrawBlock( dest_rect_, skill_ui_, array[i], &color );
                    }
                }
                break;
            }
        }

        color = WzColor_RGBA(255,255,255,255);

        // ���� ��ġ���� ū ���� �׻� �׸���.
        for (int idx = 0; idx < uiSkillTree::kMaxRow; ++idx)
        {
            if (idx > cur_tab_)
            {
                DrawBlock( dest_rect_, skill_ui_, idx, &color );
            }
        }
    }
}

//------------------------------------------------------------------------------
/** ����̵�
*/
void uiSkillTreeEffect::RenderTornado()
{
    if( xGetSkillData()->IsSkillSimulated() == true )
        return;

    OpenLevelEffect& effect = open_effect();
    RECT rect = dest_rect_;
    
    DWORD delta_time = GetCurrentTime() - animation_time_;

    WzColor old_color = g_pSunRenderer->GetDiffuseColor();

    // ����̵��� ����ġ
    int tornado_pos;

    if (effect.ani_state == OpenLevelEffect::kChainMove )
    {
        tornado_pos = min( start_tab_ + 1 , kMaxRow);

        int move_line_count = cur_tab_ - start_tab_;
        if( cur_tab_ == kMaxRow )          // ������ ���̸� �Ʒ��� �������� �ʴ´�.
            move_line_count = max( 0, move_line_count -1 );
        int move_height = (int)chain_back.srch * move_line_count;

        rect.top += (int)((float)move_height * (float)delta_time / (float)animation_duration_);

    }
    else if (effect.ani_state > OpenLevelEffect::kChainMove )
    {
        tornado_pos = min( cur_tab_ + 1, kMaxRow);
    }
    else
    {
        tornado_pos = min( start_tab_ + 1 , kMaxRow);
    }

    // ����̵� ���̵�ƿ��� ���� �÷� ����
    if (effect.ani_state == OpenLevelEffect::kTornadoFadeOut)
    {
        g_pSunRenderer->SetColor( render_color_ );
    }

    switch (effect.ani_state)
    {
    case OpenLevelEffect::kTornadoAppear:
    case OpenLevelEffect::kChainMove:
    case OpenLevelEffect::kBlockBlink:
    case OpenLevelEffect::kChainMoveStop:
    case OpenLevelEffect::kBlockFadeOutStop:
    case OpenLevelEffect::kBlockFadeOut:
    case OpenLevelEffect::kTornadoFadeOut:
        {
            rect.top += LONG(tornado_pos*chain_back.srch);

            // 
            DWORD tornado_tick = (GetCurrentTime() - open_effect().tornado_time) / 30;

			// ȸ���� ����Ʈ
			// �� 16���� ��������Ʈ�� �̿��ϸ�, ������ �� 4���� �ִϸ��̼� �ǰ�, ���� ������ 12���� ������ ���� �ִϸ��̼��� �ȴ�.

			DWORD animation_tick = 2;	// 2ƽ ���� �ִϸ��̼��� �Ѵ�.
			DWORD max_init_frame = 4;	// ���۵ɶ� ���Ǵ� �ִϸ��̼��� ������
			DWORD max_loop_frame = 12;	// ������ ���Ǵ� �ִϸ��̼��� ������


            // ���� �������� ����Ǿ����� �˻�
            if (effect.Init_tornado == true &&
                (tornado_tick >= animation_tick * max_init_frame ) )
            {
				// �������� �����Ѵ�. 
                effect.Init_tornado = false;
            }
			
            if( effect.Init_tornado == false )
            {
                tornado_tick -= 8;
            }


			// ���� ����
			ENUM_ALPHABLEND alpha_mode = g_pSunRenderer->GetAlphaBlendMode();
			g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);


			if ( effect.Init_tornado == true )
			{
				// ���� ������
				DWORD cur_frame =  (tornado_tick/animation_tick) % max_init_frame;

				// move_effect �� �̿��� ���
				g_pSunRenderer->RenderTexture(
					skill_effect(), 
					move_effect.stx + rect.left, 
					move_effect.sty + rect.top, 
					move_effect.srcw, 
					move_effect.srch,
					(cur_frame % 4) * move_effect.srcw + move_effect.srcx,
					(cur_frame / 4) * move_effect.srch + move_effect.srcy,
					move_effect.srcw, 
					move_effect.srch);
			}
			else
			{
				// ���� ������
				DWORD cur_frame =  (tornado_tick/animation_tick) % max_loop_frame;

				// move_effect_loop �� �̿��� ���
				g_pSunRenderer->RenderTexture(
					skill_effect(), 
					move_effect_loop.stx + rect.left, 
					move_effect_loop.sty + rect.top, 
					move_effect_loop.srcw, 
					move_effect_loop.srch,
					(cur_frame % 4) * move_effect_loop.srcw + move_effect_loop.srcx,
					(cur_frame / 4) * move_effect_loop.srch + move_effect_loop.srcy,
					move_effect_loop.srcw,
					move_effect_loop.srch);

			}

			// ���� ���� ����
			g_pSunRenderer->SetColor(old_color);
			g_pSunRenderer->SetAlphaBlendMode(alpha_mode);

        }
        break;
    }
}

//------------------------------------------------------------------------------
void uiSkillTreeEffect::RenderMaxLevel( const RECT& dest_rect )
{
    static float offset_x = -6.f;
    static float offset_y= -5.f;

    g_pSunRenderer->RenderTexture(
        skill_ui(),
        dest_rect.left + offset_x,
        dest_rect.top + offset_y,
        42.f,
        52.f,
        0.f, 279.f, 42.f, 52.f);
}

//------------------------------------------------------------------------------
/** ü��
*/
void uiSkillTreeEffect::RenderChain()
{
    RECT rect = dest_rect_;
    OpenLevelEffect& effect = open_effect();


    WzColor color = WzColor_RGBA(255, 255, 255, 255 );
    if( xGetSkillData()->IsSkillSimulated() == true )
    {
        color = WzColor_RGBA(255,255,255,128);
    }


    if( effect.ani_state == OpenLevelEffect::kAniNone )
    {
        // 
        int total_level = xGetSkillData()->GetSkillDataSet().GetTotalUsedPoint();
        int cur_tab = total_level / kPointLevel;
        if( cur_tab >= kMaxRow )
            cur_tab = kMaxRow;

        //
        int chain_tab = min( cur_tab + 1, kMaxRow);

        rect.top += LONG(chain_tab*chain_back.srch);
        DrawImage( skill_ui_, rect, chain_off, &color);
    }
    else
    {
        int chain_tab = min( start_tab_ + 1, kMaxRow );

        if (effect.ani_state == OpenLevelEffect::kChainMove)
        {
            DWORD delta_time = GetCurrentTime() - animation_time_;

            int move_line_count = cur_tab_ - start_tab_;
            if( cur_tab_ == kMaxRow )          // ������ ���̸� �Ʒ��� �������� �ʴ´�.
                move_line_count = max( 0, move_line_count -1 );
            int move_height = (int)chain_back.srch * move_line_count;

            rect.top += (int)((float)move_height * (float)delta_time / (float)animation_duration_);
        }
        else if (effect.ani_state > OpenLevelEffect::kChainMove )
        {
            // �̵���
            chain_tab = cur_tab_ + 1;
        }

        if( chain_tab > kMaxRow )
            chain_tab = kMaxRow;

        //pos
        rect.top += LONG(chain_tab*chain_back.srch);
        DrawImage( skill_ui_, rect, chain_off, &color);


        // �÷�
        if (effect.ani_state == OpenLevelEffect::kChainFadeOut) 
        {
            color = render_color_;
        }

        switch (effect.ani_state)
        {
        case OpenLevelEffect::kAniNone:
        case OpenLevelEffect::kAniStart:
            {
                return;
            }
        case OpenLevelEffect::kBlockBlink:
        case OpenLevelEffect::kBlockFadeOut:
        case OpenLevelEffect::kBlockFadeOutStop:
        case OpenLevelEffect::kTornadoFadeOut:
            {
                if ((open_effect().ani_time/16)%2 != 0)
                    return;
            }
            break;
        case OpenLevelEffect::kBlinkChainFast:
        case OpenLevelEffect::kBlinkChain:
            {
                if ((open_effect().ani_time/4)%2 != 0)
                {
                    return;
                }
            }
            break;
        }

        DrawImage( skill_ui_, rect, chain_on, &color);
    }
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
    local_image_array_.push_back( cRect( 0, 24, 17, 58 ));     // ���� ª�� ȭ�� 
    local_image_array_.push_back( cRect( 17, 24, 17, 58 ));    // ���� ª�� ȭ��
    local_image_array_.push_back( cRect( 94, 0, 17, 113 ));    // ���� �� ȭ��
    local_image_array_.push_back( cRect( 111, 0, 17, 113 ));   // ���� �� ȭ��
    local_image_array_.push_back( cRect( 0, 91, 53, 37 ));     // ���� ���� ȭ��
    local_image_array_.push_back( cRect( 0, 91, 53, 37 ));     // ���� ���� ȭ��

    local_image_array_.push_back( cRect( 35, 58, 32, 32 ));    // ���ǥ��


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
        // ȭ��ǥ ����� Ȱ��ȭ�Ǿ�� �� ȭ��ǥ�� �ִٸ� Ȱ��ȭ
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

        // ��ų ��� �������� �䱸 ��ų�� �ִ� ��� ��ų�� lock��ġ�� ���
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
        // ȭ��ǥ ����� Ȱ��ȭ�Ǿ�� �� ȭ��ǥ�� �ִٸ� Ȱ��ȭ
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

        // �⺻ ��ǥ�� ���� ���Ѵ�
        float x = (float)dest_rect_.left + local_position_array_.at(position).GetX();
        float y = (float)dest_rect_.top + local_position_array_.at(position).GetY();


        // 4, 5���� ���� ȭ��ǥ �̹����̸� ��ǥ�� ����
        if( arrow.GetArrowType() == 4 || arrow.GetArrowType() == 5 )
        {
            static int offset_x = -15;
            static int offset_y = 20;
            x += offset_x;
            y += offset_y;
        }

        WzColor color = WzColor_RGBA( 255, 255, 255, (BYTE)(rate*255) );

        // ���
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

    // ��� �̹����� ��Ȯ�� ��ġ�� �����ϱ� ���� offset
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

#endif //_NA_20111213_SKILLTREE_VISUAL_RENEWAL2
