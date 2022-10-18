#include "SunClientPrecompiledHeader.h"

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
#include "uiSmartNpcDialogMan/uiSmartNpcDialog.h"
#include "InterfaceManager.h"
/*
// Control의 종류를 정의 한다.
#define CTRL_KIND_DIALOG        0x00000000  // 기본 바닥 Control
#define CTRL_KIND_CLIENT        0x00000001  // 기본 바닥 Control
#define CTRL_KIND_MAINBAR       0x00000002  // Window의 Title Bar
#define CTRL_KIND_MINI          0x00000004  // Mini button
#define CTRL_KIND_MAX           0x00000008  // Max  button
#define CTRL_KIND_EXIT          0x00000010  // Exit button
#define CTRL_KIND_CHECKBT       0x00000020  // Check Button Control
#define CTRL_KIND_BUTTON        0x00000040  // Button Control
#define CTRL_KIND_TEXT          0x00000080  // Text Control
#define CTRL_KIND_EDIT          0x00000100  // Edit Control
#define CTRL_KIND_LIST          0x00000200  // List Control
#define CTRL_KIND_HSCROLL       0x00000400  // Hscroll Control
#define CTRL_KIND_VSCROLL       0x00000800  // VScroll Control
#define CTRL_KIND_TAB           0x00001000  // Tab Control
#define CTRL_KIND_PICTURE       0x00002000  // Picture Control
#define CTRL_KIND_RECTANGLE     0x00004000  // BOX Control
#define CTRL_KIND_TEXTIMAGE     0x00008000  // TextImage Control
#define CTRL_KIND_WEB           0x00010000  // Web Browser Control
*/
uiSmartNpcDialog::uiSmartNpcDialog(InterfaceManager* ui_manager)
{
    dialog_type_ = SmartDialogType_Max;
    dialog_sub_type_ = 0;
    id_ = 0;
}

CCtrlStaticWZ* uiSmartNpcDialog::CreateContorlStatic(DWORD dialog_key, DWORD control_id)
{
    RECT rt;
    rt.left = rt.right = rt.top = rt.bottom = 0;
    rt.right = rt.bottom = 20;

    CDialogManager* dialog_manager = g_InterfaceManager.GetDialogManager();
    CControlWZ* control_ = dialog_manager->CreateControlWZ(dialog_key, control_id, 0x80, 5, &rt);  // 스타일 5 == 가운데 정렬
    if (control_ == NULL)
    {
        return NULL;
    }

    //control_->SetIsOutline(false);
    //control_->SetOutlineColor(RGBA(0,0,0,255));

    CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(control_);
    if (static_control == NULL)
    {
        return NULL;
    }

    static_control->SetTextColor(0xff79c9f0);
    static_control->SetFontID(858879092);
    
    return static_control;
}

CCtrlButtonWZ* uiSmartNpcDialog::CreateContorlButton(DWORD dialog_key, DWORD control_id)
{
    RECT rt;
    rt.left = rt.right = rt.top = rt.bottom = 0;
    rt.right = rt.bottom = 20;

    CDialogManager* dialog_manager = g_InterfaceManager.GetDialogManager();
    CControlWZ* control_ = dialog_manager->CreateControlWZ(dialog_key, control_id, 0x40, 5, &rt);
    if (control_ == NULL)
    {
        return NULL;
    }

    SI_TSUBINFO image_sub_info;
    image_sub_info.Initialize();
    image_sub_info.m_blIs = true;

    image_sub_info.m_sImageName = "all_butten4.tga";
    image_sub_info.m_blUseGradation = false;
    image_sub_info.m_fTileCornerX = 9;
    image_sub_info.m_fTileCornerY = 9;

    image_sub_info.m_nType = SI_TSUBINFO::DrawType_TileExpand;
    image_sub_info.m_sFRect.fTopX = 43;
    image_sub_info.m_sFRect.fTopY = 213;
    image_sub_info.m_sFRect.fWidth = 30;
    image_sub_info.m_sFRect.fHeight = 33;
    image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
    control_->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
    image_sub_info.m_color = 0x77ffffff;
    control_->SetCtrlDetailImageFormat(E_DISABLE, &image_sub_info);

    image_sub_info.m_sFRect.fTopX = 73;
    control_->SetCtrlDetailImageFormat(E_ON, &image_sub_info);

    image_sub_info.m_sFRect.fTopX = 103;
    control_->SetCtrlDetailImageFormat(E_CLICK, &image_sub_info);


    control_->SetIsOutline(false);
    control_->SetOutlineColor(RGBA(0,0,0,255));

    CCtrlButtonWZ* button_control = static_cast<CCtrlButtonWZ*>(control_);
    if (button_control == NULL)
    {
        return NULL;
    }

    button_control->SetTextColor(4286171632);  //0xff79c9f0
    button_control->SetTextMouseOnColor(4294967295);
    button_control->SetTextMouseDownColor(4284374622);
    button_control->SetFontID(825324659); //858879092
    //m_pDraw->FileRead(hHandle,&m_lnOffsetX,sizeof(long));			
    //m_pDraw->FileRead(hHandle,&m_lnOffsetY,sizeof(long));	
    return button_control;
}

CCtrlButtonWZ* uiSmartNpcDialog::CreateContorlCloseButton(DWORD dialog_key, DWORD control_id)
{
    RECT rt;
    rt.left = rt.right = rt.top = rt.bottom = 0;
    rt.right = rt.bottom = 20;

    CDialogManager* dialog_manager = g_InterfaceManager.GetDialogManager();
    CControlWZ* control_ = dialog_manager->CreateControlWZ(dialog_key, control_id, 0x40, 0, &rt);
    if (control_ == NULL)
    {
        return NULL;
    }

    SI_TSUBINFO image_sub_info;
    image_sub_info.Initialize();
    image_sub_info.m_blIs = true;

    image_sub_info.m_sImageName = "all_butten4.tga";
    image_sub_info.m_blUseGradation = false;
    image_sub_info.m_fTileCornerX = 0;
    image_sub_info.m_fTileCornerY = 0;

    image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
    image_sub_info.m_sFRect.fTopX = 42;
    image_sub_info.m_sFRect.fTopY = 247;
    image_sub_info.m_sFRect.fWidth = 28;
    image_sub_info.m_sFRect.fHeight = 30;
    image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
    control_->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
    image_sub_info.m_color = 0x77ffffff;
    control_->SetCtrlDetailImageFormat(E_DISABLE, &image_sub_info);

    image_sub_info.m_sFRect.fTopX = 70;
    control_->SetCtrlDetailImageFormat(E_ON, &image_sub_info);

    image_sub_info.m_sFRect.fTopX = 98;
    control_->SetCtrlDetailImageFormat(E_CLICK, &image_sub_info);


    control_->SetIsOutline(false);
    control_->SetOutlineColor(RGBA(0,0,0,255));

    CCtrlButtonWZ* button_control = static_cast<CCtrlButtonWZ*>(control_);
    if (button_control == NULL)
    {
        return NULL;
    }

    //button_control->SetTextColor(0xff79c9f0);
    //button_control->SetFontID(858879092);

    return button_control;
}

CCtrlListWZ* uiSmartNpcDialog::CreateContorlList(DWORD dialog_key, DWORD control_id)
{
    RECT rt;
    rt.left = rt.right = rt.top = rt.bottom = 0;
    rt.right = rt.bottom = 20;

    CDialogManager* dialog_manager = g_InterfaceManager.GetDialogManager();
    CControlWZ* control_ = dialog_manager->CreateControlWZ(dialog_key, control_id, 0x200, 0, &rt);
    if (control_ == NULL)
    {
        return NULL;
    }

    control_->SetIsOutline(false);
    control_->SetOutlineColor(RGBA(0,0,0,255));

    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(control_);
    if (list_control == NULL)
    {
        return NULL;
    }

    //list_control->SetTextColor(0xff79c9f0);
    //list_control->SetFontID(858879092);

    SI_TSUBINFO image_sub_info;
    image_sub_info.Initialize();
    image_sub_info.m_blIs = true;

    image_sub_info.m_sImageName = "";
    image_sub_info.m_blUseGradation = false;
    image_sub_info.m_fTileCornerX = 0;
    image_sub_info.m_fTileCornerY = 0;

    image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
    image_sub_info.m_sFRect.fTopX = 0;
    image_sub_info.m_sFRect.fTopY = 0;
    image_sub_info.m_sFRect.fWidth = 0;
    image_sub_info.m_sFRect.fHeight = 0;
    image_sub_info.m_color = 0x00ffffff; //RGBA(255,255,255,255);
    control_->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);

    return list_control;
}

CCtrlVScrollWZ* uiSmartNpcDialog::CreateContorlVScroll(DWORD dialog_key, DWORD control_id)
{
    RECT rt;
    rt.left = rt.right = rt.top = rt.bottom = 0;
    rt.right = rt.bottom = 20;

    CDialogManager* dialog_manager = g_InterfaceManager.GetDialogManager();
    CControlWZ* control_ = dialog_manager->CreateControlWZ(dialog_key, control_id, 0x800, 0, &rt);
    if (control_ == NULL)
    {
        return NULL;
    }

    control_->SetIsOutline(false);
    control_->SetOutlineColor(RGBA(0,0,0,255));

    SI_TSUBINFO image_sub_info;
    image_sub_info.Initialize();
    image_sub_info.m_blIs = true;

    image_sub_info.m_sImageName = "";
    image_sub_info.m_blUseGradation = false;
    image_sub_info.m_fTileCornerX = 0;
    image_sub_info.m_fTileCornerY = 0;

    image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
    image_sub_info.m_sFRect.fTopX = 0;
    image_sub_info.m_sFRect.fTopY = 0;
    image_sub_info.m_sFRect.fWidth = 0;
    image_sub_info.m_sFRect.fHeight = 0;
    image_sub_info.m_color = 0x00ffffff;

    control_->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);

    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(control_);
    if (scroll_control == NULL)
    {
        return NULL;
    }

    SI_TFORMAT format;
    for (int i = 0; i <= E_DISABLE; ++i)
    {
        format.m_sImageState[i].Initialize();
        format.m_sImageState[i].m_blIs = true;

        format.m_sImageState[i].m_sImageName = "all_butten4.tga";
        format.m_sImageState[i].m_blUseGradation = false;
        format.m_sImageState[i].m_fTileCornerX = 0;
        format.m_sImageState[i].m_fTileCornerY = 0;

        format.m_sImageState[i].m_nType = SI_TSUBINFO::DrawType_Whole;
        format.m_sImageState[i].m_sFRect.fTopX = 476.0f + (i * 12);
        format.m_sImageState[i].m_sFRect.fTopY = 0;
        format.m_sImageState[i].m_sFRect.fWidth = 12;
        format.m_sImageState[i].m_sFRect.fHeight = 19;
        format.m_sImageState[i].m_color = 0xffffffff;
    }
    format.m_sImageState[E_DISABLE].m_sFRect.fTopX = 476;
    format.m_sImageState[E_DISABLE].m_color = 0x77ffffff;
    format.m_sImageState[E_CHECK].Initialize();

    scroll_control->SetArrowUPBTImageFormat(&format);

    for (int i = 0; i < E_DISABLE; ++i)
    {
        format.m_sImageState[i].m_sFRect.fTopX = 440.0f + (i * 12);
    }
    format.m_sImageState[E_DISABLE].m_sFRect.fTopX = 440;
    
    scroll_control->SetArrowDownBTImageFormat(&format);

    for (int i = 0; i < E_NUM; ++i)
    {
        format.m_sImageState[i].Initialize();
    }
    format.m_sImageState[E_BASE].m_blIs = true;

    format.m_sImageState[E_BASE].m_sImageName = "all_butten4.tga";
    format.m_sImageState[E_BASE].m_blUseGradation = false;
    format.m_sImageState[E_BASE].m_fTileCornerX = 0;
    format.m_sImageState[E_BASE].m_fTileCornerY = 0;

    format.m_sImageState[E_BASE].m_nType = SI_TSUBINFO::DrawType_Whole;
    format.m_sImageState[E_BASE].m_sFRect.fTopX = 500;
    format.m_sImageState[E_BASE].m_sFRect.fTopY = 58;
    format.m_sImageState[E_BASE].m_sFRect.fWidth = 12;
    format.m_sImageState[E_BASE].m_sFRect.fHeight = 20;
    format.m_sImageState[E_BASE].m_color = 0xffffffff;

    scroll_control->SetArrowThumbBTImageFormat(&format);

    rt.left = rt.top = 0;
    rt.right = 10;
    rt.bottom = 19;
    scroll_control->SetArrowUPSize(rt);
    scroll_control->SetArrowThumbSize(rt);
    scroll_control->SetArrowDownSize(rt);

    return scroll_control;
}

CCtrlPictureWZ* uiSmartNpcDialog::CreateContorlPicture(DWORD dialog_key, DWORD control_id)
{
    RECT rt;
    rt.left = rt.right = rt.top = rt.bottom = 0;
    rt.right = rt.bottom = 20;

    CDialogManager* dialog_manager = g_InterfaceManager.GetDialogManager();
    CControlWZ* control_ = dialog_manager->CreateControlWZ(dialog_key, control_id, 0x2000, 0, &rt);
    if (control_ == NULL)
    {
        return NULL;
    }

    control_->SetIsOutline(false);

    CCtrlPictureWZ* picture_control = static_cast<CCtrlPictureWZ*>(control_);
    if (picture_control == NULL)
    {
        return NULL;
    }

    return picture_control;
}

CCtrlMultilineEditWZ* uiSmartNpcDialog::CreateContorlEdit(DWORD dialog_key, DWORD control_id)
{
    RECT rt;
    rt.left = rt.right = rt.top = rt.bottom = 0;
    rt.right = rt.bottom = 20;

    CDialogManager* dialog_manager = g_InterfaceManager.GetDialogManager();
    CControlWZ* control_ = dialog_manager->CreateControlWZ(dialog_key, control_id, 0x100, CTRL_ES_LEFT | CTRL_ES_MULTILINE, &rt);
    if (control_ == NULL)
    {
        return NULL;
    }

    control_->SetIsOutline(false);

    SI_TSUBINFO image_sub_info;
    image_sub_info.Initialize();
    image_sub_info.m_blIs = true;

    image_sub_info.m_sImageName = "";
    image_sub_info.m_blUseGradation = false;
    image_sub_info.m_fTileCornerX = 0;
    image_sub_info.m_fTileCornerY = 0;

    image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
    image_sub_info.m_sFRect.fTopX = 0;
    image_sub_info.m_sFRect.fTopY = 0;
    image_sub_info.m_sFRect.fWidth = 0;
    image_sub_info.m_sFRect.fHeight = 0;
    image_sub_info.m_color = 0x00ffffff;

    control_->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);

    CCtrlMultilineEditWZ* edit_control = static_cast<CCtrlMultilineEditWZ*>(control_);
    if (edit_control == NULL)
    {
        return NULL;
    }
    //
    //. 색상 관련 함수.
    edit_control->SetColorCaret(4294967295);			//. 캐럿 색을 설정한다.
    edit_control->SetColorText(4294967295);			//. 문자 색을 설정한다.

    edit_control->SetColorSelBack(4282927144);
    edit_control->SetColorSelText(4294967295);

    edit_control->SetColorCompText(4294953160);
    edit_control->SetColorCompBack(3321888768);
    edit_control->SetColorCompCaret(4294967295);
    edit_control->SetColorIndicatorIme(4294967295);
    edit_control->SetColorIndicatorEng(4278190080);
    edit_control->SetColorIndicatorBK(4286611584);

    edit_control->SetFontID(842100333);

    edit_control->SetLimitLine(5);
    edit_control->SetHeight(15);
    edit_control->SetUseInputEnterKey(false);
    edit_control->SetLimitText(256);
    //
    edit_control->SetVScrollBaseIsOutline(false);
    edit_control->SetVScrollArrowUPBTIsOutline(false);
    edit_control->SetVScrollArrowDownBTIsOutline(false);
    edit_control->SetVScrollArrowThumbBTIsOutline(false);

    // 스크롤
    SI_TFORMAT format;
    for (int i = 0; i < E_NUM; ++i)
    {
        format.m_sImageState[i].Initialize();
        format.m_sImageState[i].m_blIs = true;

        format.m_sImageState[i].m_sImageName = "";
        format.m_sImageState[i].m_blUseGradation = false;
        format.m_sImageState[i].m_fTileCornerX = 0;
        format.m_sImageState[i].m_fTileCornerY = 0;

        format.m_sImageState[i].m_nType = SI_TSUBINFO::DrawType_Whole;
        format.m_sImageState[i].m_sFRect.fTopX = 0;
        format.m_sImageState[i].m_sFRect.fTopY = 0;
        format.m_sImageState[i].m_sFRect.fWidth = 0;
        format.m_sImageState[i].m_sFRect.fHeight = 0;
        format.m_sImageState[i].m_color = 0;
    }

    edit_control->SetVScrollBaseImageFormat(&format);
    edit_control->SetVScrollArrowUPBTImageFormat(&format);
    edit_control->SetVScrollArrowDownBTImageFormat(&format);
    edit_control->SetVScrollArrowThumbBTImageFormat(&format);

    return edit_control;
}
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
