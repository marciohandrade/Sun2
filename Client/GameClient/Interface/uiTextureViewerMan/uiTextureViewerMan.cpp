#include "SunClientPrecompiledHeader.h"

#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER
#include "uiTextureviewerman\uitextureviewerman.h"
#include "uiTextureViewerSearchWindow/uiTextureViewerSearchWindow.h"
#include "uiTextureViewerTextureWindow/uiTextureViewerTextureWindow.h"
#include "interfacemanager.h"

const DWORD IM_TEXTUREVIEWER_MANAGER::TEXTUREVIEWER_SEARCH_WINDOW = g_IDGenerator.GenerateID();
const DWORD IM_TEXTUREVIEWER_MANAGER::TEXTUREVIEWER_TEXTURE_WINDOW = g_IDGenerator.GenerateID();


const char c_strDlgFname_TextureViewer_SearchWindow[] = ("Data\\Interface\\61_3_BattleZone_Create_room.iwz");
const char c_strDlgFname_Texture_Viewer_Texture_Window[] = ("Data\\Interface\\Texture_Viewer_View.iwz");

uiTextureViewerMan::uiTextureViewerMan(InterfaceManager* pUIMan):
uiBaseMan(pUIMan)
{
	ui_texture_viewer_search_window_ = NULL;
    ui_texture_viewer_texture_window_ = NULL;
}

uiTextureViewerMan::~uiTextureViewerMan(void)
{
}

void uiTextureViewerMan::OnInitialize()
{
	// Texture Viewer 시작
	ui_texture_viewer_search_window_ = CreateUIDialog<uiTextureViewerSearchWindow>
		(IM_TEXTUREVIEWER_MANAGER::TEXTUREVIEWER_SEARCH_WINDOW, 
        c_strDlgFname_TextureViewer_SearchWindow,
		this, TRUE, TRUE);
	assert(ui_texture_viewer_search_window_);

    ui_texture_viewer_texture_window_ = CreateUIDialog<uiTextureViewerTextureWindow>
        (IM_TEXTUREVIEWER_MANAGER::TEXTUREVIEWER_TEXTURE_WINDOW, 
        c_strDlgFname_Texture_Viewer_Texture_Window,
        this, TRUE, TRUE);
    assert(ui_texture_viewer_texture_window_);
}

uiBase* uiTextureViewerMan::GetTextureViewerWindow( DWORD texture_viewer_window_id )
{
	uiBase* ui_texture_viewer_window_ptr = NULL;
	switch(texture_viewer_window_id)
	{
	case UI_TEXTURE_VIEWER_SEARCH_WINDOW: // 기본적인 SEARCH 윈도우
        {
		//ui_texture_viewer_window_ptr = (uiBase*) ui_texture_viewer_search_window_;
        ui_texture_viewer_window_ptr = static_cast<uiBase*> (ui_texture_viewer_search_window_);
		break;
        }
    case UI_TEXTURE_VIEWER_TEXTURE_WINDOW: // Texture를 뿌려줄 Frame
        {
            //ui_texture_viewer_window_ptr = (uiBase*) ui_texture_viewer_texture_window_;
            ui_texture_viewer_window_ptr = static_cast<uiBase*> (ui_texture_viewer_texture_window_);
        }        
        break;
	default:
        {

        }
		break;
	}
	return ui_texture_viewer_window_ptr;
}

void uiTextureViewerMan::ToggleTextureViewerWindow( DWORD texture_viewer_window_id )
{
	uiBase* ui_texture_viewer_window_ptr = GetTextureViewerWindow(texture_viewer_window_id);	
	if (ui_texture_viewer_window_ptr)
	{
		BOOL is_show = ui_texture_viewer_window_ptr->IsVisible();
		ui_texture_viewer_window_ptr->ShowInterface((is_show != TRUE));
       //RECT rc = { 0, 0, 800, 600 };
       // ((uiTextureViewerTextureWindow*)ui_texture_viewer_window_ptr)->Render(rc);
	}	
}

uiBase* uiTextureViewerMan::get_texture_viewer_item( int type )
{
    uiBase* texture_viewer_item_ptr = NULL;

    switch (type)
    {
    case UI_TEXTURE_VIEWER_SEARCH_WINDOW:
        {
            texture_viewer_item_ptr = static_cast<uiBase*> (ui_texture_viewer_search_window_);
        }        
        break;
    case UI_TEXTURE_VIEWER_TEXTURE_WINDOW:
        {
            texture_viewer_item_ptr = static_cast<uiBase*> (ui_texture_viewer_texture_window_);
        }        
        break;
    default:
        {

        }
        break;
    }

    return texture_viewer_item_ptr;
}

void uiTextureViewerMan::textureviewer_search_start( void )
{
    uiTextureViewerSearchWindow* ptr = NULL;

    if (ui_texture_viewer_search_window_)
    {
        if (ui_texture_viewer_search_window_->get_Is_start() == false)
        {
            ui_texture_viewer_search_window_->StartTree();
        }
    }
}
#endif
#endif