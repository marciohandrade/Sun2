#pragma once

#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>
#include <map>


struct NEW_CHAR_ITEM_INFO
{
	BYTE		m_byClassID;
	WORD		m_wItemCode;
	int         m_ReserveValue; // (f100714.1L) change a type to support a multi-use purpose
	DWORD		m_dwCasheApplyTime;	
	WORD		m_wStyle;
};

typedef std::multimap< BYTE, NEW_CHAR_ITEM_INFO* >								NEW_CHAR_ITEM_INFO_MAP;
typedef NEW_CHAR_ITEM_INFO_MAP::iterator										NEW_CHAR_ITEM_INFO_MAP_ITER;
typedef std::pair< NEW_CHAR_ITEM_INFO_MAP_ITER, NEW_CHAR_ITEM_INFO_MAP_ITER >	NEW_CHAR_ITEM_INFO_MAP_RANGE;


class NewCharItemInfoParser : public util::Singleton<NewCharItemInfoParser>, public IParser
{
public:
	NewCharItemInfoParser(void);
	~NewCharItemInfoParser(void);

public:
	VOID					Init( char * pszPackFileName );
	VOID					Release();

public:
			BOOL			_Load( BOOL bReload );
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

public:
	const NEW_CHAR_ITEM_INFO_MAP_RANGE	FindItemInfoRange( BYTE byClassID );
	const NEW_CHAR_ITEM_INFO*			FindItemInfo( BYTE byClassID );
    // f101006.4L, add an interface related to preparation data building
    // on the data base proxy server
    const NEW_CHAR_ITEM_INFO_MAP& GetTable() const;
private:
	NEW_CHAR_ITEM_INFO_MAP	m_mapNewCharItemInfo;
};

// f101006.4L
inline const NEW_CHAR_ITEM_INFO_MAP& NewCharItemInfoParser::GetTable() const {
    return m_mapNewCharItemInfo;
}
