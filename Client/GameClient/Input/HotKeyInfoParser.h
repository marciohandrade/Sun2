#pragma once
#include "Singleton.h"
#include "HotKeyDefine.h"

class HotKeyInfoParser : public util::Singleton<HotKeyInfoParser>, public IParser
{
    struct BASE_HotKeyInfo 
    {
        DWORD function_index;
        DWORD description_code;
        KEY_BIND_PAIR key_pair;
        bool support_setting;
        WORD action_code;

        BASE_HotKeyInfo()
            :function_index(0), description_code(0), support_setting(false), action_code(0)
        {

        }
    };
    typedef STLX_MAP<DWORD, BASE_HotKeyInfo*> HotKeyInfoMap;

public:
    typedef STLX_MAP<WORD, DWORD> AllowKeyCodeMap;    

public:
    HotKeyInfoParser();
    ~HotKeyInfoParser();

    void Release();

    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    /// ����Ʈ Ű���� ���� ���
    bool GetDefalutKeyInfo(KEY_BIND_INFO& output_info);
    
    /// ��ɿ� ���� ���� ��Ʈ���ڵ�
    DWORD GetDescriptionCodeByIndex(HOT_KEY_FUNCTION_INDEX function_index);

    // Ű���ÿ� ���Ǵ� Ű�ڵ� ������ �Ǵ�
    bool IsAllowKeyCode(WORD key_code);

    // Ű�ڵ� ���� �ش�Ǵ� ��Ʈ���ڵ�
    int GetStringCodeByKeyCode(WORD key_code);

    // Ű�ڵ� ���� �ش�Ǵ� ��Ʈ���ڵ�(ţ���Կ�)
    int GetStringCodeForQuickSlot(WORD key_code);

    // Ű��Ʈ���� ���� Ű�ڵ尪
    WORD GetKeyCodeByString(const TCHAR* key_string);

    // �������� ��� �ε������ ���
    bool GetIndexList(STLX_VECTOR<DWORD>& output_list);

    // ����� �����׼ǽ�ų�ڵ�
    WORD GetUserActionCode(HOT_KEY_FUNCTION_INDEX function_index);

    HotKeyInfoParser::AllowKeyCodeMap& allow_key_code_map() { return allow_key_code_map_; }
    HotKeyInfoParser::AllowKeyCodeMap& quick_key_code_map() { return quick_key_code_map_; }

private:
    void Unload();
    void BuildAllowKeyCodeList();

    /// ��ɿ� ���� �������� ��������
    /// �������� ������ �ȵǸ� UI�� �׸��� ������ ������, ������ �������� ����
    bool SupportUserSetting(HOT_KEY_FUNCTION_INDEX function_index);

    KEY_BIND_INFO& defalut_Info() { return defalut_Info_; }
    HotKeyInfoParser::HotKeyInfoMap& hot_key_info_map() { return hot_key_info_map_; }    

private:
    KEY_BIND_INFO defalut_Info_; // �ش� �����ʹ� �ε� ���� ����Ǹ� �ȵ�. �ܺο� ������� �ʵ�����
    HotKeyInfoMap hot_key_info_map_; // ��ɿ� ���� �����ڵ�/����������������
    AllowKeyCodeMap allow_key_code_map_;// ��� Ű�ڵ�/��Ʈ��(+��Ʈ���ڵ�)
    AllowKeyCodeMap quick_key_code_map_;// �����Ͽ��� ���� ��Ʈ��
};

