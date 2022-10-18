#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
#include "uiBase.h"
#include "FTextRender.h"

class uiCharacterRenameInputDialog : public uiBase
{
    enum RenameFlowState
    {
        State_None,
        State_Check_Name,
        State_Confirm_Use,
    };

    enum UIControlPos
    {
        kControl_Text_T001 = 0,     // 메시지텍스트
        kControl_Edit_E000,         // 에디트컨트롤
        kControl_TextImage_TI00,    // 중복확인/변경하기
        kControl_TextImage_TI01,    // 취소
        kControl_Button_BT00,       // X버튼
        kControl_Size,
    };
public:
    uiCharacterRenameInputDialog(InterfaceManager* ui_manager);
    virtual ~uiCharacterRenameInputDialog();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void MessageProc(SI_MESSAGE * message);

    void SetRenameData(int index, std::string& current_name);
            
protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();

private:
    void RegisterControl();
    bool OnLButtonClick(SI_MESSAGE* message);

    void ProcessRenameFlow();

    void InitInputDialog();
    void CheckCharacterRename(char* rename);
    void NoticeCannotUseName();
    void ConfirmUseName();

    void SendCharacterNameChangePacket();
    void ProcessSuccessRename(MSG_BASE* recv_packet);
    void ProcessErrorCode(MSG_BASE* recv_packet);

    void SetNoticeText(TCHAR* notice_text);

private:
    std::string current_name_;
    std::string rename_;
    int character_slot_index_;

    RenameFlowState rename_flow_state_;

    FTextRender notice_text_;
};

inline void uiCharacterRenameInputDialog::SetRenameData(int index, std::string& current_name)
{
    character_slot_index_ = index;
    current_name_ = current_name;
    rename_ = current_name;
    
    InitInputDialog();
}

#endif //_JP_20110222_INGAME_CHARNAME_CHANGE