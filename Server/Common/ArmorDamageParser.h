#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <ServerStruct.h>
#include <SolarFileReader.h>

typedef util::SolarHashTable< ARMOR_DAMAGE_INFO* > ARMOR_DAMAGEINFO_HASH;
typedef ARMOR_DAMAGEINFO_HASH::iterator ARMOR_DAMAGEINFO_ITER;

class ArmorDamageParser : public util::Singleton<ArmorDamageParser>, public IParser
{
public:     
    ArmorDamageParser();
    ~ArmorDamageParser();

public:
    // (CHANGES)(20100309)(hukim) Init(), Release() �������̵� �޼��� ȥ���� ���� ���� virtual Ű���� �߰�
    // (CHANGES)(20100309)(hukim) FindDamageRatio() ���� ������ ����Ű�� �ʱ� ������ const �߰�

    virtual VOID Init(DWORD pool_size);
    virtual VOID Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    DAMAGETYPE FindDamageRatio(BYTE arrmor_type, BYTE attack_type) const;
    
private:
    // (CHANGES)(20100309)(hukim) �̱��� ��ü�̹Ƿ� ���簡 �Ͼ�� �ȵǱ� ������ __DISABLE_COPY() �߰�
    // (CHANGES)(20100309)(hukim) unload(), make_key() �ܺο��� ������ �ʱ� ������ private�� ����
    // (CHANGES)(20100309)(hukim) make_key() ���� ������ ����Ű�� �ʱ� ������ const �߰�
    // (CHANGES)(20100309)(hukim) m_szFileName�� ���Ǵ� ���� ���� ������ �ּ� ó��

    __DISABLE_COPY(ArmorDamageParser);
    
    BOOL load(BOOL is_reload);
    VOID unload();
    WORD make_key(BYTE arrmor_type, BYTE attack_type) const;

    ARMOR_DAMAGEINFO_HASH* armor_damage_table_;
    
    //TCHAR m_szFileName[MAX_PATH];
};
