#pragma once
#include <Singleton.h>
#include <SolarFileReader.h>
#include <vector>

struct CREATION_INFO
{
    BYTE class_type;
    char class_name[MAX_CHARNAME_LENGTH + 1];
    DWORD class_description_code;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    char race_name[MAX_CHARNAME_LENGTH + 1];
    DWORD race_description_code;
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    BYTE base_gender_type;
    DWORD enable_select_class[eCHAR_TYPE_MAX - 1];
};

class CharacterCreationInfoParser : public Singleton<CharacterCreationInfoParser>, public IParser
{
    typedef std::vector<CREATION_INFO*> CREATION_INFO_LIST;
    typedef std::vector<CREATION_INFO*>::iterator CREATION_INFO_ITR;

public:
    CharacterCreationInfoParser();
    ~CharacterCreationInfoParser();

    void Init(DWORD info_pool_size);
    void Init(DWORD info_pool_size, char* pack_filename);
    void Release();

    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    //------------------------------------------------------------------------------
    //! Returns:   CREATION_INFO*
    //! Qualifier: ���� �������� ���� ������ ����
    //! Parameter: BYTE class_type : ��ũ��Ʈ:ChaIndex �ʵ� (���� �ε���)
    //------------------------------------------------------------------------------
    CREATION_INFO* GetCreationInfo(BYTE class_type);

    //------------------------------------------------------------------------------
    //! Returns:   int
    //! Qualifier: ������ �������� ���� ������ ����
    //! Parameter: eCHAR_TYPE class_type : ����
    //! Parameter: bool silhouette_type[] : ���� ������ ���� �迭
    //------------------------------------------------------------------------------
    int GetSkinClass(eCHAR_TYPE char_class, bool skin_class[/*eCHAR_TYPE_MAX*/]);

private:
    void LoadScript();

    void Unload();
    CREATION_INFO_LIST creation_info_list_;
};


