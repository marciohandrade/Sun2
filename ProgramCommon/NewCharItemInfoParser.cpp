#include "StdAfx.h"
#include ".\newchariteminfoparser.h"

NewCharItemInfoParser::NewCharItemInfoParser(void)
{
}

NewCharItemInfoParser::~NewCharItemInfoParser(void)
{
}



VOID		NewCharItemInfoParser::Init( char * pszPackFileName )
{
    __UNUSED(pszPackFileName);
}

VOID		NewCharItemInfoParser::Release()
{
	NEW_CHAR_ITEM_INFO_MAP_ITER	iter;
	for( iter = m_mapNewCharItemInfo.begin(); iter != m_mapNewCharItemInfo.end(); ++iter )
	{
		NEW_CHAR_ITEM_INFO* pInfo = iter->second;
		if( pInfo )
			delete pInfo;
	}

	m_mapNewCharItemInfo.clear();
}

BOOL		NewCharItemInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch(ScriptCode)
	{
	case SCRIPT_NEW_CHARACTER_ITEM_INFO:
		return _Load( bReload );
	}

	return FALSE; 
}

BOOL		NewCharItemInfoParser::_Load( BOOL bReload )
{
    __UNUSED(bReload);
	int rowSize = GetRowSize();

	for( int row = 0; row < rowSize; ++row )
	{
		NEW_CHAR_ITEM_INFO* pInfo = new NEW_CHAR_ITEM_INFO;
		pInfo->m_byClassID			= GetDataBYTE( "ClassID", row );
		pInfo->m_wItemCode			= GetDataWORD( "ItemCode", row );
        // (f100714.1L) change a type to support a multi-use purpose
		pInfo->m_ReserveValue = GetDataInt( "ItemSupplyNum", row );
		pInfo->m_dwCasheApplyTime	= GetDataDWORD( "ItemUseTime", row );
		pInfo->m_wStyle				= GetDataWORD( "StyleSkillID", row );
		//자료구조가 멀티맵 이다. 따라서 중복이여도 상관없다.
		m_mapNewCharItemInfo.insert( std::make_pair( pInfo->m_byClassID, pInfo ) );
	}

	return TRUE;
}


const NEW_CHAR_ITEM_INFO_MAP_RANGE	NewCharItemInfoParser::FindItemInfoRange( BYTE byClassID )
{
	return m_mapNewCharItemInfo.equal_range( byClassID );
}

const NEW_CHAR_ITEM_INFO*			NewCharItemInfoParser::FindItemInfo( BYTE byClassID )
{
	NEW_CHAR_ITEM_INFO_MAP_ITER iter = m_mapNewCharItemInfo.find( byClassID );
	if( iter != m_mapNewCharItemInfo.end() )
		return iter->second;

	return NULL;
}


