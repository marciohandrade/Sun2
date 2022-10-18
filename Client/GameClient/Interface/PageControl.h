#pragma once

//#define _YMS_TREECONTROL 
//#ifdef _YMS_TREECONTROL

//------------------------------------------------------------------------------ 
class PageControl
{
public:
    PageControl();
    virtual ~PageControl();

    bool Init(CCtrlStaticWZ* _text_control, 
                CCtrlButtonWZ* _left_button_control, 
                CCtrlButtonWZ* _left_list_control);
    bool IsInit() { return (text_control_ != NULL && left_button_control_ != NULL && right_button_control_ != NULL); }
    void Release();

    //------------------------------------------------------------------------------
    //! Qualifier: ���� �������� ��ü �������� �����Ѵ�.
    //!            �ּ� 1���������� ���õ� (0 �������� ����)
    //! Parameter: int _current_page : one-base
    //! Parameter: int _total_page : one-base
    //------------------------------------------------------------------------------
    void SetPage(int _current_page, int _total_page);
    void SetTotalPage( int _total_page );

    //------------------------------------------------------------------------------
    //! Returns:   bool : true : ó���Ǿ����� ���� �޼��� �����ص���, false : �޼��� ó�� ���� ����
    //! Qualifier: PageControl ���� ��Ʈ�� �޼��� ó��
    //! Parameter: SI_MESSAGE * msg
    //! Parameter: OUT bool& is_change : ���� ��ȭ�� �Ͼ� ������
    //------------------------------------------------------------------------------
    bool MessageProc(SI_MESSAGE* msg, OUT bool& is_change);

    int current_page() const { return current_page_; }
    int current_page_zero_base() const { return current_page_ - 1; }

    CCtrlStaticWZ* text_control() const { return text_control_; }
    CCtrlButtonWZ* left_button_control() { return left_button_control_; }
    CCtrlButtonWZ* right_button_control() { return right_button_control_; }

private:
    bool OnLButtonClick(SI_MESSAGE* msg, OUT bool& is_change);
    bool Next();
    bool Prev();
    void Apply();

    CCtrlStaticWZ* text_control_;
    CCtrlButtonWZ* left_button_control_;
    CCtrlButtonWZ* right_button_control_;

    int current_page_;
    int total_page_;
};


//#endif //#ifdef _YMS_TREECONTROL