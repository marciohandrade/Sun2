#pragma  once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>
#include <hash_map>
#include <ReliableRatioRandomizer.h>
#include <ReliableRatioRandomizerEX.hpp>

#define MAX_LIMITED_ITEM_NAME_LENGTH		(64)

#define MAX_DURA_RATIO_COUNT				3
#define MAX_LIMIT_VALUE_RATIO_OPTION_COUNT	3
#define MAX_DAMAGE_RATIO_COUNT				3
#ifndef _NA_000251_20100727_SOCKET_SYSTEM
#define MAX_LIMITED_SOCKET_COUNT			4
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#define MAX_LIMITED_GAMBLE_COUNT			4
#define MAX_GAMBLE_OPTION_COUNT				5

struct sLIMITED_ITEM_INFO
{
	DWORD			m_dwIndex;
	char			m_pszName[MAX_LIMITED_ITEM_NAME_LENGTH];
	//ver1.1 : 아이템 네이밍에 고유색깔을 부여

	enum { MAX_COLOR_LENGTH = 7 };
	BYTE			m_bUse;							//<아이템 네이밍색깔 사용여부
	char			m_szColor[MAX_COLOR_LENGTH];	//<아이템 네이밍색깔 RGB

	eITEM_WAA_TYPE	m_eWAAType;
	WORD			m_wEquipImage;
	DWORD			m_dwInvenImage;
	
	DWORD			m_dwDuraRatio[MAX_DURA_RATIO_COUNT];
	INT				m_iDura_Min[MAX_DURA_RATIO_COUNT];
	INT				m_iDura_Max[MAX_DURA_RATIO_COUNT];
	DWORD			m_dwDuraRatioSum;

	DWORD			m_dwLimitValueRatio[MAX_LIMIT_VALUE_RATIO_OPTION_COUNT];
	INT				m_iLimitValue_Min[MAX_LIMIT_VALUE_RATIO_OPTION_COUNT];
	INT				m_iLimitValue_Max[MAX_LIMIT_VALUE_RATIO_OPTION_COUNT];
	DWORD			m_dwLimitValueRatioSum;

	DWORD			m_dwDamageRatio[MAX_DAMAGE_RATIO_COUNT];
	INT				m_iDamage_Min[MAX_DAMAGE_RATIO_COUNT];
	INT				m_iDamage_Max[MAX_DAMAGE_RATIO_COUNT];
    DWORD			m_dwDamageRatioSum;
	
	BYTE			m_byRepairLimite;

#ifndef _NA_000251_20100727_SOCKET_SYSTEM
    DWORD			m_dwSocketRatio[MAX_LIMITED_SOCKET_COUNT];
    DWORD			m_dwSocketRatioSum;
#endif // _NA_000251_20100727_SOCKET_SYSTEM
    
	DWORD			m_dwGambleRatio[MAX_LIMITED_GAMBLE_COUNT];    
	DWORD			m_dwGambleList[MAX_LIMITED_GAMBLE_COUNT][MAX_GAMBLE_OPTION_COUNT];
	DWORD			m_dwGambleRatioSum;
};

typedef stdext::hash_map<DWORD, sLIMITED_ITEM_INFO *>		LIMITED_ITEM_INFO_HASH;

#ifndef _NA_000251_20100727_SOCKET_SYSTEM
typedef stdext::hash_map<DWORD, ReliableRatioRandomizer<MAX_LIMITED_SOCKET_COUNT> *>		SOCKET_RATIO_HASH;
#endif // _NA_000251_20100727_SOCKET_SYSTEM
typedef stdext::hash_map<DWORD, ReliableRatioRandomizer<MAX_LIMITED_GAMBLE_COUNT> *>		GAMBLE_RATIO_HASH;

typedef stdext::hash_map<DWORD, ReliableRatioRandomizerEX<MAX_DURA_RATIO_COUNT> *>					DURA_RATIO_HASH;
typedef stdext::hash_map<DWORD, ReliableRatioRandomizerEX<MAX_LIMIT_VALUE_RATIO_OPTION_COUNT> *>	LIMIT_VALUE_RATIO_HASH;
typedef stdext::hash_map<DWORD, ReliableRatioRandomizerEX<MAX_DAMAGE_RATIO_COUNT> *>				DAMAGE_RATIO_HASH;

class LimitedItemInfoParser : public util::Singleton<LimitedItemInfoParser>, public IParser
{
public:		LimitedItemInfoParser(void);
	virtual	~LimitedItemInfoParser(void);

public:		VOID					Init( char * pszPackFileName );
			VOID					Release();


			VOID					Unload();

public:		sLIMITED_ITEM_INFO*		GetLimitedItemInfo( DWORD Index );
			
			INT						GetRandomkeyFromDuraRatio( DWORD Index );
			INT						GetRandomkeyFromLimitValueRatio( DWORD Index );
			INT						GetRandomkeyFromDamageRatio( DWORD Index );
#ifndef _NA_000251_20100727_SOCKET_SYSTEM
            INT						GetRandomkeyFromSocketRatio( DWORD Index );
#endif // _NA_000251_20100727_SOCKET_SYSTEM
            INT						GetRandomkeyFromGambleRatio( DWORD Index );

			VOID					SetFirst() { m_LimitedItemInfoIterator = m_HashLimitedItemInfo.begin();	}
			sLIMITED_ITEM_INFO*		GetNext();

protected:	WORD					ValidCheck( sLIMITED_ITEM_INFO* pInfo );

public:
	virtual	BOOL					LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload );
private:	BOOL					_Load( BOOL bReload );

private:	LIMITED_ITEM_INFO_HASH				m_HashLimitedItemInfo;
			LIMITED_ITEM_INFO_HASH::iterator	m_LimitedItemInfoIterator;

			DURA_RATIO_HASH						m_HashDuraRatioRandomizer;
			LIMIT_VALUE_RATIO_HASH				m_HashLimitValueRatioRandomizer;
			DAMAGE_RATIO_HASH					m_HashDamageRatioRandomizer;
#ifndef _NA_000251_20100727_SOCKET_SYSTEM
            SOCKET_RATIO_HASH					m_HashSocketRatioRandomizer;
#endif // _NA_000251_20100727_SOCKET_SYSTEM
            GAMBLE_RATIO_HASH					m_HashGambleRatioRandomizer;

			CHAR								m_pszFileName[MAX_PATH];
};
