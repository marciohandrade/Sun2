#pragma once
#include "uiBase.h"
#include "FTextRender.h"

class uiSystemMessage : public uiBase
{
    enum UIControlPos
    {
        kControl_Title_Text = 0,    // 제목
        kControl_Message_Text,      // 내용
        kControl_VScroll,           // 스크롤
        kControl_VScroll_Picture,   // 스크롤이미지
        kControl_Verify_Button,     // 확인버튼
        kControl_Confirm_Button,    // 예
        kControl_Rejection_Button,  // 아니오
        kControl_Close_Button,      // 닫기

        kControl_Size,
    };
public:
    enum MessageType
    {
        kVerify_Type = 0,
        kConfirm_Type,
        kVerify_And_Dont_Close_Type,  // 닫기버튼 없는 Verify타입 (esc로도 닫을 수 없음)
        kConfirm_And_Dont_Close_Type, // 닫기버튼 없는 Confirm타입 (esc로도 닫을 수 없음)

        kMax_Type,
    };

    enum ButtonIndex
    {
        kVerify_Button = 0,
        kConfirm_Button,
        kRejection_Button,
        kClose_Button,

        kMax_Button,
    };

    typedef void (*MessageCallback)(ButtonIndex button_index);
    typedef STLX_MAP<DWORD, DWORD> UserDataContainer;

public:
    uiSystemMessage(InterfaceManager* interface_manager);
    virtual ~uiSystemMessage();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

    void Clear();
    void SetMessageType(MessageType type);
    void SetTitle(const TCHAR* title);
    void SetMessage(const TCHAR* message);
    void SetTimer(DWORD life_time);
    void SetButtonText(ButtonIndex button_index, DWORD string_code);
    void SetButtonText(ButtonIndex button_index, const TCHAR* button_text);
    void SetModal(bool show_lock_effect);
    void SetUserData(DWORD user_data);

    void set_callback_function(MessageCallback val) { callback_function_ = val; }

    static void DoSystemMessage(DWORD l_param, DWORD r_param);
    static DWORD GetUserData();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnVScrollUp(SI_MESSAGE* message);
    bool OnVScrollDown(SI_MESSAGE* message);
    bool OnVScrollThumb(SI_MESSAGE* message);

    void UpdateMessage();

    MessageType message_type() const { return message_type_; }
    void set_message_type(MessageType val) { message_type_ = val; }
    FTextRender& message_text_renderer() { return message_text_renderer_; }
    DWORD expired_time() const { return expired_time_; }
    void set_expired_time(DWORD val) { expired_time_ = val; }
    bool is_modal() const { return is_modal_; }
    void set_modal(bool val) { is_modal_ = val; }
    bool is_show_lock_effect() const { return is_show_lock_effect_; }
    void set_show_lock_effect(bool val) { is_show_lock_effect_ = val; }
    MessageCallback callback_function() const { return callback_function_; }
    STLX_STRING& message_string() { return message_string_; }

    static UserDataContainer& GetUserDataContainer() { return user_data_container_; }
    static DWORD& CurrentDialogKey() { return current_dialog_key_; }

private:
    MessageType message_type_;
    FTextRender message_text_renderer_;   
    DWORD expired_time_;
    bool is_modal_;
    bool is_show_lock_effect_;
    MessageCallback callback_function_;
    STLX_STRING message_string_;
    
    static UserDataContainer user_data_container_;
    static DWORD current_dialog_key_;
    
};