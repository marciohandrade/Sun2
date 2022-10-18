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

    /// 디폴트 키세팅 정보 얻기
    bool GetDefalutKeyInfo(KEY_BIND_INFO& output_info);
    
    /// 기능에 대한 설명 스트링코드
    DWORD GetDescriptionCodeByIndex(HOT_KEY_FUNCTION_INDEX function_index);

    // 키세팅에 허용되는 키코드 값인지 판단
    bool IsAllowKeyCode(WORD key_code);

    // 키코드 값에 해당되는 스트링코드
    int GetStringCodeByKeyCode(WORD key_code);

    // 키코드 값에 해당되는 스트링코드(큇슬롯용)
    int GetStringCodeForQuickSlot(WORD key_code);

    // 키스트링에 대한 키코드값
    WORD GetKeyCodeByString(const TCHAR* key_string);

    // 설정가능 기능 인덱스목록 얻기
    bool GetIndexList(STLX_VECTOR<DWORD>& output_list);

    // 연결된 유저액션스킬코드
    WORD GetUserActionCode(HOT_KEY_FUNCTION_INDEX function_index);

    HotKeyInfoParser::AllowKeyCodeMap& allow_key_code_map() { return allow_key_code_map_; }
    HotKeyInfoParser::AllowKeyCodeMap& quick_key_code_map() { return quick_key_code_map_; }

private:
    void Unload();
    void BuildAllowKeyCodeList();

    /// 기능에 대한 유저세팅 지원여부
    /// 유저세팅 지원이 안되면 UI에 항목이 나오지 않으며, 서버에 저장하지 않음
    bool SupportUserSetting(HOT_KEY_FUNCTION_INDEX function_index);

    KEY_BIND_INFO& defalut_Info() { return defalut_Info_; }
    HotKeyInfoParser::HotKeyInfoMap& hot_key_info_map() { return hot_key_info_map_; }    

private:
    KEY_BIND_INFO defalut_Info_; // 해당 데이터는 로드 이후 변경되면 안됨. 외부에 노출되지 않도록함
    HotKeyInfoMap hot_key_info_map_; // 기능에 대한 설명코드/유저세팅지원여부
    AllowKeyCodeMap allow_key_code_map_;// 허용 키코드/스트링(+스트링코드)
    AllowKeyCodeMap quick_key_code_map_;// 퀵슬록에서 사용될 스트링
};

