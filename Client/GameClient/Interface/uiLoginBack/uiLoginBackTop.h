#ifndef _uiLoginBackTop_h_
#define _uiLoginBackTop_h_

#include "uibase.h"

class uiLoginMan;

//------------------------------------------------------------------------------ 
class uiLoginBackTop : public uiBase
{
    enum UIControlPos
    {
        kControl_Text_S000 = 0,
        kControl_Text_S001,
        kControl_List_L000,
        kControl_Size,
    };

public:
    static DWORD kDialog_ID;

public:
    uiLoginBackTop(InterfaceManager* ui_man);
    virtual ~uiLoginBackTop(void);

    virtual void    Init(CDrawBase* drawbase);
    virtual void    Release(void);
    virtual void    Process(DWORD tick);
    virtual void	OnShowWindow(BOOL val);
    virtual void    OnRenderSolarDialog();
    virtual void    MessageProc(SI_MESSAGE* message);

    uiLoginMan* GetManager();

    //------------------------------------------------------------------------------
    void    SetSceneTitle(DWORD title_string_code);
    void    SetSceneTitleClassName(BYTE class_type, DWORD class_name_code);
    void    SetSceneTitleString(TCHAR* title_string);


private:
    bool OnLButtonClick(SI_MESSAGE* message);

    void RenderClassMark();

    BYTE class_type_;
};

#endif // _uiLoginBackTop_h_
