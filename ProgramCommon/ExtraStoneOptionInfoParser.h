#pragma once


#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

//==================================================================================================

#pragma warning( push )
#pragma warning( disable : 4201 4510 4512 4610 )

struct sEXTRASTONE_OPT_INFO
{
	sEXTRASTONE_OPT_INFO():m_HashKey(0){}
	enum { MAX_VALUE_NUM = 4 };
	DWORD				m_dwIndex;
	union
	{
		struct {
			WORD		m_wItemType;
			LEVELTYPE	m_ItemLV;
		};
		const DWORD		m_HashKey;
	};
	WORD				m_wOptionIndex;
	DWORD				m_dwOptionNCode;
	BYTE				m_byValueType;
	DWORD				m_dwValue[MAX_VALUE_NUM];
};

#pragma warning( pop )

//==================================================================================================

class ExtraStoneOptionInfoParser : public util::Singleton<ExtraStoneOptionInfoParser>, public IParser
{
	typedef util::SolarHashTable<sEXTRASTONE_OPT_INFO *> _EXTRASTONE_OPT_INFO_HASH;
public:
	ExtraStoneOptionInfoParser(void);
	virtual ~ExtraStoneOptionInfoParser(void);

	VOID				Init( DWORD dwOptionInfoSize );
	VOID				Init( DWORD dwOptionInfoSize, char * pszPackFileName );
	VOID				Release();
	VOID				Reload();

	/*const sEXTRASTONE_OPT_INFO * GetExtraStoneOptionInfo( DWORD key ) 
	{ return m_pExtraStoneOptionInfoHash->GetData( key ); }
	const sEXTRASTONE_OPT_INFO * GetExtraStoneOptionInfo( WORD ItemType, LEVELTYPE ItemLV ) 
	{ return m_pExtraStoneOptionInfoHash->GetData( ((ItemType<<16)|ItemLV) ); }*/
	const sEXTRASTONE_OPT_INFO * GetExtraStoneOptionInfo( WORD ItemType )
	{ return m_pExtraStoneOptionInfoHash->GetData(ItemType); }

public:
	BOOL					_Load( BOOL bReload );
	BOOL					_LoadExtra( BOOL bReload );
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

protected:
	VOID				Unload();
private:
	_EXTRASTONE_OPT_INFO_HASH*	m_pExtraStoneOptionInfoHash;
	char						m_pszFileName[MAX_PATH];
};
