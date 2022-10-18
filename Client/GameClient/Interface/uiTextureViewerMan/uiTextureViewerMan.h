#pragma once

#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER
#include "uiBaseMan.h"

struct IM_TEXTUREVIEWER_MANAGER
{
	static const DWORD TEXTUREVIEWER_SEARCH_WINDOW;
    static const DWORD TEXTUREVIEWER_TEXTURE_WINDOW;
};

enum
{
	UI_TEXTURE_VIEWER_SEARCH_WINDOW,
    UI_TEXTURE_VIEWER_TEXTURE_WINDOW,    
	UI_TEXTURE_VIEWER_WINDOW_MAX
};

class uiTextureViewerSearchWindow;
class uiTextureViewerTextureWindow;
class uiTextureViewerMan : public uiBaseMan
{
public:
	virtual void OnInitialize();

	uiBase* GetTextureViewerWindow(DWORD texture_viewer_window_id);
	void ToggleTextureViewerWindow(DWORD texture_viewer_window_id);
	
public:
	uiTextureViewerMan(InterfaceManager* pUIMan);
	virtual ~uiTextureViewerMan(void);

public:
    uiBase* get_texture_viewer_item(int type);
    void    textureviewer_search_start(void);

private:
	uiTextureViewerSearchWindow* ui_texture_viewer_search_window_;
    uiTextureViewerTextureWindow* ui_texture_viewer_texture_window_;
	
	
	
};
#endif
#endif