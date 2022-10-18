#pragma once

class uiBase;

class TabCheckButtonControllManager
{
    struct TabDialogData
    {
        float dialog_position_x;
        float dialog_position_y;
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		DWORD prev_tab_index;
#endif // _NA_008472_20150903_IMPROVED_BALANCE
        DWORD current_tab_index;
        DWORD key_queue_type;
    };

    typedef STLX_VECTOR<uiBase*> TabDialogContainer;

public:
    TabCheckButtonControllManager();
    ~TabCheckButtonControllManager();

    // 초기화
    bool Initialize(DWORD tab_num, 
                    DWORD key_queue_type, 
                    POINT* dialog_position, 
                    DWORD init_tab_index = 0);

    // 탭 다이얼로그 넣기 (0번 인덱스부터 (tab_num - 1)까지 넣어야함)
    void PushTab(DWORD tab_index, uiBase* tab_dialog);

    // 다이얼로그 열기
    void ShowDialog(DWORD tab_index, bool is_show);

    // 다이얼로그 열기
    void ShowCurrentDialog(bool is_show);

    // 탭이 클릭됨
    void OnClickTab(DWORD tab_index);

    // 다이얼로그 얻기
    uiBase* GetTabDialog(DWORD tab_index);
    uiBase* GetCurrentTabDialog();

    // 다이얼로그 포커스 업데이트
    void UpdateDialogFocus(uiBase* main_dialog);

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	// 현재 선택된 탭 인덱스 얻기
	DWORD GetCurrentTabIndex() { return tab_dialog_data_.current_tab_index; }
	DWORD GetPrevTabIndex()    { return tab_dialog_data_.prev_tab_index; }

    void SetCurrentTabIndex(DWORD tap_index) { tab_dialog_data_.current_tab_index = tap_index; }
#endif // _NA_008472_20150903_IMPROVED_BALANCE

private:
    TabDialogContainer& tab_dialog_container() { return tab_dialog_container_; }
    TabDialogData& tab_dialog_data() { return tab_dialog_data_; }

    void RegisterKeyQueueType(uiBase* tab_dialog);
    void ReleaseKeyQueueType();
    void SaveDialogPosition(uiBase* tab_dialog);

    bool CheckValidTabIndex(DWORD tab_index);

private:
    TabDialogContainer tab_dialog_container_;
    TabDialogData tab_dialog_data_;
};
