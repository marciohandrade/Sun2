#pragma once
#include "uiBase.h"

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "FTextRender.h"

class CreateCharClassDialog : public uiBase
{
    enum UIControlPos
    {
        kControl_Text_S000 = 0,
        kControl_Text_S001,
        kControl_Text_S003,
        kControl_VScroll_V000,
        kControl_Picture_P002,
        kControl_Picture_P001,
        kControl_Picture_P000,
        kControl_Picture_P003,
		kControl_Picture_P004,
        kControl_Size,
    };

public:
    CreateCharClassDialog(InterfaceManager* ui_manager);
    virtual ~CreateCharClassDialog();

    virtual void Init(CDrawBase* drawbase);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    
    void SetClassDescription(BYTE select_race);    
    virtual void		OnUpdateSolarDialog();
    virtual void		OnRenderSolarDialog();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void RegisterControl();
    bool AddFormatText(const TCHAR* message);
    void UpdateMouseWheel();

    HANDLE handle_texture_;
    St_TextureContainer* texture_information_;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    HANDLE handle_texture2_;
    St_TextureContainer* texture_information2_;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    BYTE select_class_;

    TCHAR complete_string_[INTERFACE_STRING_LENGTH];
    CCtrlVScrollWZ* vertical_scroll_;
    FTextRender	ftext_render_;
    FTextSentence ftext_sentence_;
    FTextParser ftext_parser_;
    FTextParam ftext_param_;
};
#endif//_NA_005003_20120612_NEW_CHARACTER_MYST;lIC_FOR_CLIENT