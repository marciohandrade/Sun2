#pragma once

#include "uiBase.h"
#include "VScrollDialog.h"


class uiOptionKeyBinding : public uiBase, public VScrollDialog
{
public:
    uiOptionKeyBinding(InterfaceManager* ui_man );
    virtual ~uiOptionKeyBinding();

protected:
    virtual void OnShowWindow( BOOL val );

private:
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();
    virtual void MessageProc( SI_MESSAGE * pMessage );


private:
    CCtrlVScrollWZ* vscroll_control() { return m_pScroll; } // implement VScrollDialog
    bool GetScrollArea(RECT& area); // implement VScrollDialog

    bool OnVScrollDown(SI_MESSAGE* msg); // override VScrollDialog
    bool OnVScrollUp(SI_MESSAGE* msg); // override VScrollDialog
    bool OnVScrollThumb(SI_MESSAGE* msg); // override VScrollDialog

private:

    class sKeyBindingControlItem
    {
    public:
        sKeyBindingControlItem( WzID Title, WzID Text, WzID Left, WzID Right ) : m_Title(Title), m_Text(Text), m_Left(Left), m_Right(Right) {}

        WzID m_Title;
        WzID m_Text;
        WzID m_Left;
        WzID m_Right;
    };

    std::vector<sKeyBindingControlItem*> m_ControlArray;


    BOOL m_Initialized;

    CCtrlVScrollWZ* m_pScroll;
    std::vector<CCtrlButtonCheckWZ*> m_CheckButtonArray;
    CCtrlButtonCheckWZ* m_CheckCharacterSetting;

    STLX_VECTOR<DWORD> m_IndexArray;



    struct InputKeyInfo 
    {
        DWORD function_index;
        int list_index;
        WORD hot_key_code;
        BYTE bind_pair_index;

        void Clear()
        {
            function_index = 0;
            list_index = -1;
            hot_key_code = 0;
            bind_pair_index = 0;
        }
    };
    InputKeyInfo m_InputKeyInfo;

    struct sInputKeyResultInfo
    {
        enum eResultState
        {
            RESULT_STATE_SETTED_MAINKEY,
            RESULT_STATE_SETTED_SUBKEY,
            RESULT_STATE_RELEASED_MAINKEY,
            RESULT_STATE_RELEASED_SUBKEY,
        };

        DWORD m_FunctionIndex;
        eResultState m_ResultState;

        bool IsSettedMainKey() { return m_ResultState == RESULT_STATE_SETTED_MAINKEY; }
        bool IsSettedSubKey() { return m_ResultState == RESULT_STATE_SETTED_SUBKEY; }
        bool IsReleasedMainKey() { return m_ResultState == RESULT_STATE_RELEASED_MAINKEY; }
        bool IsReleasedSubKey() { return m_ResultState == RESULT_STATE_RELEASED_SUBKEY; }
    };

    std::vector<sInputKeyResultInfo> m_ResultArray;

    void AddResultInfo( sInputKeyResultInfo::eResultState ResultState, DWORD FunctionIndex );


    void InitializeControl();
    void UpdateControlState();

    BOOL OnClickButton( SI_MESSAGE* pMessage );
    BOOL OnClickHotKeyButton( SI_MESSAGE* pMessage );


    void InitializeKeyBindInfo();
    void ApplyHotKey();
    void ReleaseKeyBindInfo(DWORD index, BYTE bind_pair_index);
    void RegisterKeyBindInfo(DWORD index, BYTE bind_pair_index, WORD hot_key_code);

public:
    void RecvUserKeySetting(WORD hot_key_code);

};