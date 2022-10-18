#pragma  once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>
#include <hash_map>

#define MAX_BULLET_OPTION_COUNT		3

struct tagEtherBulletOption
{
	WORD				m_wOptionKind[MAX_BULLET_OPTION_COUNT];	
	BYTE				m_byValueType[MAX_BULLET_OPTION_COUNT];
	INT					m_iValue[MAX_BULLET_OPTION_COUNT];
};

struct tagEtherBulletInfo
{
	enum
	{	
		eETHER_DEVICE_COMBO_NORMAL	= 0,
		eETHER_DEVICE_COMBO_1		= 1,
		eETHER_DEVICE_COMBO_2		= 2,
		eETHER_DEVICE_COMBO_3		= 3,
		eETHER_DEVICE_COMBO_4		= 4,
		eETHER_DEVICE_COMBO_5		= 5,
		eETHER_DEVICE_COMBO_MAX		= 5,
	};
	WORD					m_wBulletID;
	SLOTCODE				m_ItemCode;

	tagEtherBulletOption	m_EtherBulletOption[eETHER_DEVICE_COMBO_MAX];
	INT 					m_iAttackSpeed[eETHER_DEVICE_COMBO_MAX - 1];
	BYTE					m_byEffectCode;

	DWORD				m_dwEffect;	

    //_NA_008456_20150819_ETHER_BULLET_PVP_CHECK
    BOOL apply_pvp_damage_;     //PvP에 적용되는 탄환인가
};

typedef stdext::hash_map<DWORD, tagEtherBulletInfo *>		ETHER_BULLET_INFO_HASH;


class EtherBulletInfoParser : public util::Singleton<EtherBulletInfoParser>, public IParser
{
public:
	EtherBulletInfoParser(void);
	virtual ~EtherBulletInfoParser(void);

	VOID							Init( char * pszPackFileName );
	VOID							Release();

	VOID							Unload();
	VOID							Reload();

	virtual	BOOL					LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
	tagEtherBulletInfo *			GetEtherBulletInfoByIndex( DWORD Index );
	tagEtherBulletInfo *			GetEtherBulletInfoByItemCode( DWORD ItemCode );

	VOID							SetFirst() { m_HashEtherBulletInfoIterator = m_EtherBulletInfoIndex.begin(); }
	tagEtherBulletInfo*				GetNext();

protected:
	WORD							ValidCheck(tagEtherBulletInfo *pInfo);
private:
	BOOL							_Load( BOOL bReload );
	ETHER_BULLET_INFO_HASH				m_EtherBulletInfoIndex;
	ETHER_BULLET_INFO_HASH				m_EtherBulletInfoItemCode;
	ETHER_BULLET_INFO_HASH::iterator	m_HashEtherBulletInfoIterator;

	char								m_pszFileName[MAX_PATH];
};
