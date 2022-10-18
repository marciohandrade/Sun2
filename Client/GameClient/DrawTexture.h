#pragma once


namespace nDrawTexture
{
    class DrawTextureManager
    {
    private:
        class Texture
        {
            TCHAR name_[256];
            HANDLE texture_;
            int x_;
            int y_;
            int width_;
            int height_;

        private:
            Texture();

        public:
            ~Texture();

            LPCTSTR GetName() { return name_; }
            void Draw( const RECT& dest_rect, WzColor* color = NULL );

            static Texture* Texture::Create(LPCTSTR name, LPCTSTR texture_path, int x, int y, int width, int height);
        };
        std::vector<Texture*> image_array_;

    public:
        DrawTextureManager();

        void Initialize();
        void Release();

        void AddImage( LPCTSTR key, LPCTSTR texture_path, int x, int y, int width, int height );

        void Draw( LPCTSTR key, const RECT& dest_rect, float color_rate );
        void Draw( LPCTSTR key, const RECT& dest_rect, int offset_x, int offset_y, float color_rate );
    };
}
