#pragma once


#include <Singleton.h>
#include <ScriptCode.IParser.h>
#include <ServerStruct.h>
#include <map>

typedef STLX_MAP<MAPCODE, MAP_PARTIAL_LOADING>		MAP_PARTIAL_LOADING_MAP;
typedef std::pair<MAPCODE, MAP_PARTIAL_LOADING>		MAP_PARTIAL_LOADING_PAIR;
typedef MAP_PARTIAL_LOADING_MAP::iterator			MAP_PARTIAL_LOADING_MAP_ITER;


class MapPartialLoadingParser : public util::Singleton<MapPartialLoadingParser>, public IParser
{
public:		MapPartialLoadingParser();
			~MapPartialLoadingParser();
public:
	virtual	VOID						Release() {}

public:		VOID						Reload();
			MAP_PARTIAL_LOADING*		FindMapInfo( MAPCODE MapKind );

public:
	virtual	BOOL						LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload );
private:	BOOL						_Load( BOOL bReload );
			MAP_PARTIAL_LOADING_MAP		m_mapPartialLoadingInfo;
			CHAR						m_pszFileName[MAX_PATH];
};







































