#pragma once

//#define _YMS_TREECONTROL 
//
//#ifdef _YMS_TREECONTROL

//------------------------------------------------------------------------------ 
class TabCheckButtonControl
{
public:
    TabCheckButtonControl();
    virtual ~TabCheckButtonControl();

    void Release();
    void Push(CCtrlButtonCheckWZ* check_button, DWORD user_data);

    //------------------------------------------------------------------------------
    //! Returns:   bool : true : ó���Ǿ����� ���� �޼��� �����ص���, false : �޼��� ó�� ���� ����
    //! Qualifier: dropdown ���� ��Ʈ�� �޼��� ó��
    //! Parameter: SI_MESSAGE * msg
    //------------------------------------------------------------------------------
    bool MessageProc(SI_MESSAGE* msg);
    bool Click(CCtrlButtonCheckWZ* check_button, bool is_ignore_drawstate = true);
    bool Click(DWORD user_data, bool is_ignore_drawstate = true);

    CCtrlButtonCheckWZ* Current(OUT DWORD& user_data);

    //! return 0xffffffff : ���õ� �޴��� �������
    DWORD CurrentUserData();


    CCtrlButtonCheckWZ* GetButtonControl(IN DWORD user_data);
    bool IsVisible();
    void SetVisible(bool is_visible);
    void SetVisible(DWORD user_data, bool is_visible);
    void SetEnable(bool is_enable);
    void SetEnable(DWORD user_data, bool is_enable);

    std::list<CCtrlButtonCheckWZ*>& button_list() { return button_list_; }
    bool is_discard_message() const { return is_discard_message_; }
    void set_discard_message(bool val);

private:
    bool OnLButtonClick(SI_MESSAGE* msg);

    std::list<CCtrlButtonCheckWZ*>  button_list_;
    bool is_discard_message_;
};


//#endif //#ifdef _YMS_TREECONTROL