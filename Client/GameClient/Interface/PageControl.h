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
    //! Qualifier: 현재 페이지와 전체 페이지를 셋팅한다.
    //!            최소 1페이지부터 셋팅됨 (0 페이지는 없음)
    //! Parameter: int _current_page : one-base
    //! Parameter: int _total_page : one-base
    //------------------------------------------------------------------------------
    void SetPage(int _current_page, int _total_page);
    void SetTotalPage( int _total_page );

    //------------------------------------------------------------------------------
    //! Returns:   bool : true : 처리되었을때 이후 메세지 무시해도됨, false : 메세지 처리 하지 않음
    //! Qualifier: PageControl 관련 컨트롤 메세지 처리
    //! Parameter: SI_MESSAGE * msg
    //! Parameter: OUT bool& is_change : 값이 변화가 일어 났는지
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