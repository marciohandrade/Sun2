#include "SunClientPrecompiledHeader.h"
#include "DrawTexture.h"
#include "GlobalData.h"


namespace nDrawTexture
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DrawTextureManager::Texture* DrawTextureManager::Texture::Create(LPCTSTR name, LPCTSTR texture_path, int x, int y, int width, int height)
    {
        CManagerTexture* manager = g_pSunRenderer->x_pManagerTexture;

        HANDLE texture = manager->LoadTexture( texture_path, TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD );
        if( texture != INVALID_HANDLE_VALUE )
        {
            Texture* instance = new Texture;
            _tcscpy(instance->name_, name);
            instance->texture_ = texture;
            instance->x_ = x;
            instance->y_ = y;
            instance->width_ = width;
            instance->height_ = height;

            return instance;
        }

        return NULL;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DrawTextureManager::Texture::Texture()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DrawTextureManager::Texture::~Texture()
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture( texture_ );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void DrawTextureManager::Texture::Draw( const RECT& dest_rect, WzColor* color )
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

        float dest_width = (float)dest_rect.right - (float)dest_rect.left;
        float dest_height = (float)dest_rect.bottom - (float)dest_rect.top;
        float src_width = (float)width_;
        float src_height = (float)height_;
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
#else
        if( dest_width < src_width )
            src_width = dest_width;
        if( dest_height < src_height )
            src_height = dest_height;

        if( dest_width > src_width )
            dest_width = src_width;
        if( dest_height > src_height )
            dest_height = src_height;
#endif
        // left.right(시작위치보정) top.bottom(이미지크기보정)
        g_pSunRenderer->RenderTexture(texture_,
            (float)dest_rect.left, (float)dest_rect.top,
            dest_width, dest_height,
            (float)x_, (float)y_, 
            src_width, src_height );

        g_pSunRenderer->SetColor(old_color);
        g_pSunRenderer->SetAlphaBlendMode(alpha_mode);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // DrawTextureManager
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DrawTextureManager::DrawTextureManager()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void DrawTextureManager::Initialize()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void DrawTextureManager::Release()
    {
        for( size_t i=0;i!=image_array_.size();++i)
        {
            delete image_array_[i];
        }
        image_array_.clear();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void DrawTextureManager::AddImage( LPCTSTR name, LPCTSTR texture_path, int x, int y, int width, int height )
    {
        if( Texture* image = Texture::Create( name, texture_path, x, y, width, height ) )
        {
            image_array_.push_back( image );
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void DrawTextureManager::Draw( LPCTSTR name, const RECT& dest_rect, float color_rate )
    {
        for( size_t i=0;i!=image_array_.size();++i)
        {
            Texture* image = image_array_[i];
            if( _tcsicmp( image->GetName(), name ) == 0 )
            {
                WzColor color = WzColor_RGBA(255,255,255,(BYTE)(color_rate*255));
                image->Draw( dest_rect, &color );
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void DrawTextureManager::Draw( LPCTSTR name, const RECT& dest_rect, int offset_x, int offset_y, float color_rate )
    {
        RECT offset_rect = dest_rect;
        offset_rect.left += offset_x;
        offset_rect.right += offset_x;
        offset_rect.top += offset_y;
        offset_rect.bottom += offset_y;
        Draw( name, offset_rect, color_rate );
    }

}