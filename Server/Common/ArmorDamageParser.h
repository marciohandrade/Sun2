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
    // (CHANGES)(20100309)(hukim) Init(), Release() 오버라이딩 메서드 혼동을 막기 위해 virtual 키워드 추가
    // (CHANGES)(20100309)(hukim) FindDamageRatio() 내부 변경을 일으키지 않기 때문에 const 추가

    virtual VOID Init(DWORD pool_size);
    virtual VOID Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    DAMAGETYPE FindDamageRatio(BYTE arrmor_type, BYTE attack_type) const;
    
private:
    // (CHANGES)(20100309)(hukim) 싱글톤 객체이므로 복사가 일어나선 안되기 때문에 __DISABLE_COPY() 추가
    // (CHANGES)(20100309)(hukim) unload(), make_key() 외부에서 사용되지 않기 때문에 private로 변경
    // (CHANGES)(20100309)(hukim) make_key() 내부 변경을 일으키지 않기 때문에 const 추가
    // (CHANGES)(20100309)(hukim) m_szFileName는 사용되는 곳이 없기 때문에 주석 처리

    __DISABLE_COPY(ArmorDamageParser);
    
    BOOL load(BOOL is_reload);
    VOID unload();
    WORD make_key(BYTE arrmor_type, BYTE attack_type) const;

    ARMOR_DAMAGEINFO_HASH* armor_damage_table_;
    
    //TCHAR m_szFileName[MAX_PATH];
};
