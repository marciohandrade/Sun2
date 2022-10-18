#pragma once

#include <../ProgramCommon/SCPlayer.h>

class cPlayerData
    : public IPlayerAttributeLinker
{
private:
    BASE_PLAYERINFO* m_pPlayerInfo;
    DWORD m_WeaponKind;

public:
    cPlayerData();
    ~cPlayerData();

    void Initialize();
    void Release();

    void SetWeaponKind( DWORD WeaponKind ) { m_WeaponKind = WeaponKind; }

public:
    // IPlayerAttributeLinker interface
    virtual BASE_PLAYERINFO* GetCharacterInfo() const { return m_pPlayerInfo; }
    virtual eCHAR_TYPE GetCharType() const { return (eCHAR_TYPE)m_pPlayerInfo->m_byClassCode; }
    virtual LEVELTYPE GetLevel() const { return m_pPlayerInfo->m_LV; }
    virtual DWORD GetWeaponKind() const { return m_WeaponKind; }
    virtual void SetCharInfo(const BASE_PLAYERINFO* char_info) { memcpy(m_pPlayerInfo, char_info, sizeof(BASE_PLAYERINFO) ); }

};