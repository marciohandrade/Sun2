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

    // �ʱ�ȭ
    bool Initialize(DWORD tab_num, 
                    DWORD key_queue_type, 
                    POINT* dialog_position, 
                    DWORD init_tab_index = 0);

    // �� ���̾�α� �ֱ� (0�� �ε������� (tab_num - 1)���� �־����)
    void PushTab(DWORD tab_index, uiBase* tab_dialog);

    // ���̾�α� ����
    void ShowDialog(DWORD tab_index, bool is_show);

    // ���̾�α� ����
    void ShowCurrentDialog(bool is_show);

    // ���� Ŭ����
    void OnClickTab(DWORD tab_index);

    // ���̾�α� ���
    uiBase* GetTabDialog(DWORD tab_index);
    uiBase* GetCurrentTabDialog();

    // ���̾�α� ��Ŀ�� ������Ʈ
    void UpdateDialogFocus(uiBase* main_dialog);

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	// ���� ���õ� �� �ε��� ���
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
