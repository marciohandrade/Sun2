//WorldSystemMemo.h - 시스템 메모와 관련된 클레스

// implemented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION

class WorldSystemMemo
{
public:
    WorldSystemMemo(){}
    ~WorldSystemMemo(){}

    void SendSystemMemo(const BasicSystemMemoInfo* system_memo_info, DWORD memo_info_list_num,
                        eSYSTEM_MEMO_TYPE system_memo_type);

private:
    //-- DB로 시스템 메모를 보낸다.
    void SendSystemMemoOfLotto(const SystemMemoInfoOfLotto* memo_info_list, DWORD list_num);
    void SendSystemMemoToDBProxy(const BasicSystemMemoInfo* memo_info_list, DWORD list_num);
};
