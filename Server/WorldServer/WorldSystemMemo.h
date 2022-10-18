//WorldSystemMemo.h - �ý��� �޸�� ���õ� Ŭ����

// implemented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION

class WorldSystemMemo
{
public:
    WorldSystemMemo(){}
    ~WorldSystemMemo(){}

    void SendSystemMemo(const BasicSystemMemoInfo* system_memo_info, DWORD memo_info_list_num,
                        eSYSTEM_MEMO_TYPE system_memo_type);

private:
    //-- DB�� �ý��� �޸� ������.
    void SendSystemMemoOfLotto(const SystemMemoInfoOfLotto* memo_info_list, DWORD list_num);
    void SendSystemMemoToDBProxy(const BasicSystemMemoInfo* memo_info_list, DWORD list_num);
};
