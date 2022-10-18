#pragma once


#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>



enum 
{
	ETHER_EFFECT_METAL_BULLET,
	ETHER_EFFECT_ETHER_BULLET,
	ETHER_EFFECT_MAGIC_BULLET,
	ETHER_EFFECT_CHARGEING,
	ETHER_EFFECT_SHOT,


	MAX_ETHER_EFFECT,
};

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
enum
{
    ETHER_HAND_FIRST  = 1,
    ETHER_HAND_SECOND = 2, // 양손 무기가 다른 경우 SECOND 사용 할 것.
};
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

struct BASE_EtherWeaponEffect {

	BYTE	byWeaponType;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    BYTE    weapon_sub_type_;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	DWORD	dwTargetBone;
	DWORD	dwEffectCode[MAX_ETHER_EFFECT];

	WzVector wvOffsetPos;
	WzVector wvOffsetRot;
	WzVector wvScale;
};




class EtherWeaponEffectInfoParser	: public Singleton<EtherWeaponEffectInfoParser>, public IParser
{
public:
	EtherWeaponEffectInfoParser();
	~EtherWeaponEffectInfoParser();


	VOID                    Init(DWORD dwPoolSize);
	VOID		            Init(DWORD dwPoolSize, char * pszPackFileName);
	VOID                    Release();

	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

	BASE_EtherWeaponEffect* GetEtherWeponEffectInfo(DWORD dwKey);

private:
	BOOL					_Load( BOOL bReload );
	VOID		            Unload();
	util::SolarHashTable<BASE_EtherWeaponEffect *>		*m_pDataTable;
};
