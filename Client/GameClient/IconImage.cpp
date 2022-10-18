#include "SunClientPrecompiledHeader.h"
#include "iconimage.h"
#include "iconmgr.h"
#include "InterfaceManager.h"

//const float ICON_IMAGE_SIZE = 38.f;
const float STATUS_ICON_IMAGE_SIZE = 14.f;


IconImage::IconImage() : m_dwKey(0), m_hTexture(INVALID_HANDLE_VALUE)
{
}

IconImage::~IconImage()
{

}

void IconImage::Create(HANDLE hTexture, int iSrcWidth, int iSrcHeight)
{
	ICONMGR()->AddIconCount();

	m_hTexture = hTexture;
	m_iSrcWidth  = iSrcWidth;
	m_iSrcHeight = iSrcHeight;

    active_render = ACTIVE_EFFECT_END;
    active_start_tick = 0;
}

void IconImage::Destroy()
{
	ICONMGR()->RemoveIconCount();
	g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_hTexture);
}

int	IconImage::GetSrcWidth() const
{
	return m_iSrcWidth;
}

int	IconImage::GetSrcHeight() const
{
	return m_iSrcHeight;
}

void IconImage::Render(RECT * pDest, RECT * pSrc /*= NULL*/)
{
	RECT rcSrc;
	rcSrc.left = 0;
	rcSrc.top = 0;
	rcSrc.right = (LONG)ICON_IMAGE_SIZE;
	rcSrc.bottom = (LONG)ICON_IMAGE_SIZE;

	if ( pSrc )
		rcSrc = *pSrc;

	g_pSunRenderer->RenderTexture(
		m_hTexture, 
		(float)pDest->left, 
		(float)pDest->top, 
		(float)pDest->right-pDest->left,
		(float)pDest->bottom-pDest->top,
		(float)rcSrc.left,
		(float)rcSrc.top,
		(float)rcSrc.right,
		(float)rcSrc.bottom
		);
}

void IconImage::Render( RECT * pDest,WzColor* Color /*= NULL*/, RECT * pSrc /*= NULL*/ )
{
	RECT rcSrc;
	rcSrc.left = 0;
	rcSrc.top = 0;
	rcSrc.right = (LONG)ICON_IMAGE_SIZE;
	rcSrc.bottom = (LONG)ICON_IMAGE_SIZE;

	if ( pSrc )
		rcSrc = *pSrc;

	g_pSunRenderer->RenderTexture(
		m_hTexture, 
		(float)pDest->left, 
		(float)pDest->top, 
		(float)pDest->right,
		(float)pDest->bottom,
		(float)rcSrc.left,
		(float)rcSrc.top,
		(float)rcSrc.right,
		(float)rcSrc.bottom,
		Color
		);
}
//--------------------------------------------------------------------------------------------------
/**  코드중복 줄이기위한함수
*/
void IconImage::RenderCoverImage(RECT* rect, WzColor color)
{
    WzColor vertex_color[4];

    for (int idx = 0; idx < 4; ++idx)
    {
        vertex_color[idx] = color;
    }

    g_pSunRenderer->RenderTexture(
        m_hTexture, 
        (float)rect->left, 
        (float)rect->top, 
        (float)rect->right-rect->left,
        (float)rect->bottom-rect->top,
        0.0f,
        0.0f,
        ICON_IMAGE_SIZE,
        ICON_IMAGE_SIZE,
        vertex_color
        );
}

//--------------------------------------------------------------------------------------------------
/**  쿨타임 슬롯그리기
*/
void IconImage::RenderSpinCooltime(RECT* rect, float rate)
{
    if (rate < 0.0f) 
    {
        rate = 0.0f;
    }

    if (rate > 100.0f) 
    {
        rate = 100.0f;
    }

    WzColor cover_color[10];

    for (int a = 0; a < 10; ++a)
    {
       cover_color[a] = WzColor_RGBA(220,220,220,255);
    }

    if (rate == 100.0f)
    {
        g_pSunRenderer->RenderTexture4SpinGage(m_hTexture,
            (float)rect->left - 0.5f, 
            (float)rect->top - 0.5f, 
            (float)rect->right-rect->left,
            (float)rect->bottom-rect->top,
            0.0f,
            0.0f,
            ICON_IMAGE_SIZE,
            ICON_IMAGE_SIZE,
            rate);


        if (active_render == ACTIVE_EFFECT_READY)
        {
            // 2011.06.17 송찬종
            //쿨타임이 완료됬을때 GetTickCount()를 한번만 호출하기 위해 ACTIVE_EFFECT_STATE 을 추가함
            //이런 계산에 관련한 로직은 Render()함수가 아닌 다른 곳에서 처리하는게 맞지만, 
            //아이템,스킬등의 쿨타임이 공통적으로 종료되는 시점을 찾기가 힘들어서 이곳에서 처리했습니다.
            active_start_tick = GetTickCount(); 
            active_render = ACTIVE_EFFECT_START;
        }

        if (active_render == ACTIVE_EFFECT_START)
        {
            //쿨타임 활성화 애니메이션
            const int ACTIVE_EFFECT_TICK_PER_FRAME = 20;
            const int ACTIVE_EFFECT_TOTAL_ANIMATION_FRAME = 34;
            const int ACTIVE_EFFECT_TOTAL_ANIMATION_TICK = ACTIVE_EFFECT_TICK_PER_FRAME * ACTIVE_EFFECT_TOTAL_ANIMATION_FRAME;
            const int ACTIVE_EFFECT_TOTAL_ANIMATION_TICK_HALF = static_cast<int>(ACTIVE_EFFECT_TOTAL_ANIMATION_TICK * 0.5);

            DWORD progress_tick = GetTickCount() - active_start_tick;
            int animation_index = (progress_tick / ACTIVE_EFFECT_TICK_PER_FRAME) % ACTIVE_EFFECT_TOTAL_ANIMATION_FRAME;

            if (progress_tick < ACTIVE_EFFECT_TOTAL_ANIMATION_TICK+ACTIVE_EFFECT_TOTAL_ANIMATION_TICK_HALF) //1.5바퀴 동안
            {
                const float active_effect_increase_alpha = 0.6f;
                const float active_effect_decrease_alpha = 0.3f;

                int active_color_fill_alpha = 0;

                if (progress_tick < ACTIVE_EFFECT_TOTAL_ANIMATION_TICK)
                {
                    //한바퀴 동안 아이콘 활성화 애니메이션
                    int width = rect->right - rect->left;
                    int height = rect->bottom - rect->top;
                    g_InterfaceManager.RenderActivateMarkIcon(rect->left - (width / 2), 
                                                                rect->top - (height / 2), 
                                                                width * 2, 
                                                                height * 2,
                                                                animation_index);
                }

                if (progress_tick < ACTIVE_EFFECT_TOTAL_ANIMATION_TICK_HALF)
                {
                    //반바퀴 동안 페이드인
                    active_color_fill_alpha = static_cast<int>(active_effect_increase_alpha * progress_tick);
                    if (active_color_fill_alpha > 255)
                    {
                        active_color_fill_alpha = min(active_color_fill_alpha,255);
                    }
                }
                else
                {
                    //반바퀴 후 페이드아웃
                    active_color_fill_alpha = static_cast<int>((active_effect_increase_alpha * ACTIVE_EFFECT_TOTAL_ANIMATION_TICK_HALF) -
                        active_effect_decrease_alpha * (progress_tick - ACTIVE_EFFECT_TOTAL_ANIMATION_TICK_HALF));
                    if (active_color_fill_alpha < 0)
                    {
                        active_color_fill_alpha = max(active_color_fill_alpha,0);
                    }
                }

                g_pSunRenderer->ColorFill(WzColor_RGBA(255,255,255,active_color_fill_alpha), 
                    (float)rect->left, 
                    (float)rect->top,
                    (float)rect->right-rect->left,
                    (float)rect->bottom-rect->top);
            }            
            else
            {
                active_render = ACTIVE_EFFECT_END;
            }
        }
    }
    else
    {
        if (active_render != ACTIVE_EFFECT_READY)
        {
            active_render = ACTIVE_EFFECT_READY;
        }

        RenderCoverImage(rect, WzColor_RGBA(70, 70, 70, 255));

        g_pSunRenderer->RenderTexture4SpinGage(m_hTexture,
            (float)rect->left - 0.5f, 
            (float)rect->top - 0.5f, 
            (float)rect->right-rect->left,
            (float)rect->bottom-rect->top,
            0.0f,
            0.0f,
            ICON_IMAGE_SIZE,
            ICON_IMAGE_SIZE,
            rate,
            cover_color);
    }

}

void IconImage::RenderWarningSpinCooltime(RECT* rect, float rate)
{
    if (rate < 0.0f) 
    {
        rate = 0.0f;
    }

    if (rate > 100.0f) 
    {
        rate = 100.0f;
    }

    float fBrightSize = rate;
    float fDarkSize = 100.0f - rate;

    WzColor Redcolor[10];

    for (int a = 0; a < 10; ++a)
    {
        Redcolor[a] = WzColor_RGBA(255,0,0,156);
    }

    WzColor darkcolor[4];

    for (int a = 0; a < 4; ++a)
    {
        darkcolor[a] = WzColor_RGBA(100,100,100,125);
    }

    if (rate < 100.0f)
    {
        g_pSunRenderer->RenderTexture(m_hTexture, (float)rect->left, 
            (float)rect->top, 
            (float)rect->right-rect->left,
            (float)rect->bottom-rect->top,
            0.0f,
            0.0f,
            ICON_IMAGE_SIZE,
            ICON_IMAGE_SIZE,
            darkcolor);
    }

    if (rate > 0.0f) 
    {
        g_pSunRenderer->RenderTexture4SpinGage(
            m_hTexture, 
            (float)rect->left - 0.5f, 
            (float)rect->top - 0.5f, 
            (float)rect->right-rect->left,
            (float)rect->bottom-rect->top,
            0.0f,
            0.0f,
            ICON_IMAGE_SIZE,
            ICON_IMAGE_SIZE,
            fBrightSize,
            Redcolor);
    }
}

void IconImage::RenderDeactivated(RECT *pDest)
{
    //static WzColor color = WzColor_RGBA(80,80,80,156);
    static WzColor color = 0xFF666666;
    RenderCoverImage(pDest, color);
}

void IconImage::RenderRewardList(RECT * pDest)
{
    static WzColor color = WzColor_RGBA(255,255,255,110);
    RenderCoverImage(pDest, color);
}

void IconImage::RenderBlocked(RECT * pDest)
{
    static WzColor color = WzColor_RGBA(255,0,0,156);
    RenderCoverImage(pDest, color);
}

void IconImage::RenderFreezing(RECT* pDest)
{
    static WzColor color = WzColor_RGBA(64,64,255,255);
    RenderCoverImage(pDest, color);
}

void IconImage::RenderWarning(RECT *pDest, float fRate)
{
	if (fRate < 0.0f) 
	{
		fRate = 0.0f;
	}

	if (fRate > 1.0f) 
	{
		fRate = 1.0f;
	}

	float fBrightSize = (pDest->bottom-pDest->top) * fRate;
	float fDarkSize = (pDest->bottom-pDest->top) - fBrightSize;

	St_TextureContainer *pInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(m_hTexture);

	if (pInfo) 
	{
		WzColor Redcolor[4];

		for (int a = 0; a < 4; ++a)
		{
			Redcolor[a] = WzColor_RGBA(255,0,0,156);

		}

		WzColor darkcolor[4];

		for (int a = 0; a < 4; ++a)
		{
			darkcolor[a] = WzColor_RGBA(30,30,30,156);

		}


		if (fRate > 0.0f) 
		{
			g_pSunRenderer->RenderTexture(
				m_hTexture, 
				(float)pDest->left, 
				(float)pDest->top, 
				(float)pDest->right - pDest->left,
				(float)fBrightSize,
				0.0f,
				0.0f,
				ICON_IMAGE_SIZE,
				ICON_IMAGE_SIZE * fRate,
				darkcolor
				);
		}

		if (fRate < 0.5f)
		{
			__asm nop;
		}


		if (fRate < 1.0f) 
		{
			g_pSunRenderer->RenderTexture(
				m_hTexture, 
				(float)pDest->left, 
				(float)pDest->top + fBrightSize, 
				(float)pDest->right - pDest->left,
				(float)fDarkSize,
				0.0f,
				ICON_IMAGE_SIZE * fRate,
				ICON_IMAGE_SIZE,
				ICON_IMAGE_SIZE * (1 - fRate)		
				,Redcolor);
		}
	}

}

void IconImage::RenderCooldown(RECT *pDest,float fRate)
{
	if (fRate < 0.0f) 
	{
		fRate = 0.0f;
	}

	if (fRate > 1.0f) 
	{
		fRate = 1.0f;
	}

	float fBrightSize = (pDest->bottom-pDest->top) * fRate;
	float fDarkSize = (pDest->bottom-pDest->top) - fBrightSize;

	St_TextureContainer *pInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(m_hTexture);

	if (pInfo) 
	{
		WzColor darkcolor[4];

		for (int a = 0; a < 4; ++a)
		{
			darkcolor[a] = WzColor_RGBA(30,30,30,156);

		}

		if (fRate > 0.0f) 
		{
			g_pSunRenderer->RenderTexture(
				m_hTexture, 
				(float)pDest->left, 
				(float)pDest->top, 
				(float)pDest->right - pDest->left,
				(float)fBrightSize,
				0.0f,
				0.0f,
				ICON_IMAGE_SIZE,
				ICON_IMAGE_SIZE * fRate,
				darkcolor
				);
		}

		if (fRate < 0.5f)
		{
			__asm nop;
		}


		if (fRate < 1.0f) 
		{
			g_pSunRenderer->RenderTexture(
				m_hTexture, 
				(float)pDest->left, 
				(float)pDest->top + fBrightSize, 
				(float)pDest->right - pDest->left,
				(float)fDarkSize,
				0.0f,
				ICON_IMAGE_SIZE * fRate,
				ICON_IMAGE_SIZE,
				ICON_IMAGE_SIZE * (1 - fRate)		
				);
		}
	}

}


//----------------------------------------------------------------------------
/**
*/
void 
IconImage::RenderWarning(RECT *pDest)
{
    static WzColor color = WzColor_RGBA(255,100,100,200);
    RenderCoverImage(pDest, color);
}

