//=============================================================================================================================
/// ResrcManager class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 8. 2
	@note
		- Load된 맵관련 리소스를 관리하는 클래스
	@history
		
*/
//=============================================================================================================================

#pragma once

#include <hash_map>
#include <MemoryPoolFactory.h>

class FieldInfo;
class UnitRegenInfoManager;
class UnitGroupInfoManager;
class SharedFieldInfo;

class ResrcManager
{
	typedef STLX_HASH_MAP<DWORD, FieldInfo *>				_FIELD_HASH;
	typedef STLX_HASH_MAP<DWORD, UnitRegenInfoManager *>	_REGEN_HASH;
	typedef STLX_HASH_MAP<DWORD, UnitGroupInfoManager *>	_GROUP_HASH;
	typedef STLX_HASH_MAP<DWORD, SharedFieldInfo*>			_SHAREDFIELD_HASH;

public:
	ResrcManager(void);
	~ResrcManager(void);

	static void SetPoolSize( DWORD field_size, DWORD sector_size );

	void Destroy();
	void Establish();

	__inline FieldInfo * FindFieldInfo( FIELDCODE fieldcode, WORD wmocode )
	{
		DWORD code = MAKEDWORD(fieldcode, wmocode);
		_FIELD_HASH::iterator it = m_FieldInfoHash.find(code);
		if( it == m_FieldInfoHash.end() ) return NULL;
		return it->second;
	}

	__inline UnitRegenInfoManager * FindRegenInfoMgr( FIELDCODE fieldcode, WORD wutcode )
	{
		DWORD code = MAKEDWORD(fieldcode, wutcode);
		_REGEN_HASH::iterator it = m_UnitRegenInfoMgrHash.find(code);
		if( it == m_UnitRegenInfoMgrHash.end() ) return NULL;
		return it->second;
	}
	__inline UnitGroupInfoManager * FindGroupInfoMgr( FIELDCODE fieldcode, WORD wutcode )
	{
		DWORD code = MAKEDWORD(fieldcode, wutcode);
		_GROUP_HASH::iterator it = m_UnitGroupInfoMgrHash.find(code);
		if( it == m_UnitGroupInfoMgrHash.end() ) return NULL;
		return it->second;
	}

	__inline SharedFieldInfo * FindSharedFieldInfo( FIELDCODE fieldcode, WORD wmocode )
	{
		DWORD code = MAKEDWORD(fieldcode, wmocode);
		_SHAREDFIELD_HASH::iterator it = m_SharedFieldInfoHash.find(code);
		if( it == m_SharedFieldInfoHash.end() ) return NULL;
		return it->second;
	}

private:
	void Establish( sMAPINFO * mapinfo );

	__inline void InsertFieldInfo( FIELDCODE fieldcode, WORD wmocode, FieldInfo * p ) 
	{ 
		DWORD code = MAKEDWORD(fieldcode, wmocode);
		std::pair<_FIELD_HASH::iterator,bool> ret = m_FieldInfoHash.insert( _FIELD_HASH::value_type(code, p) ); 
		ASSERT(ret.second);
	}
	__inline void InsertRegenInfoMgr( FIELDCODE fieldcode, WORD wutcode, UnitRegenInfoManager * p ) 
	{ 
		DWORD code = MAKEDWORD(fieldcode, wutcode);
		std::pair<_REGEN_HASH::iterator,bool> ret = m_UnitRegenInfoMgrHash.insert( _REGEN_HASH::value_type(code, p) ); 
		ASSERT(ret.second);	
	}
	__inline void InsertGroupInfoMgr( FIELDCODE fieldcode, WORD wutcode, UnitGroupInfoManager * p ) 
	{ 
		DWORD code = MAKEDWORD(fieldcode, wutcode);
		std::pair<_GROUP_HASH::iterator,bool> ret = m_UnitGroupInfoMgrHash.insert( _GROUP_HASH::value_type(code, p) );
		ASSERT(ret.second);
	}
	__inline void InsertSharedFieldInfo( FIELDCODE fieldcode, WORD wmocode, SharedFieldInfo * p ) 
	{ 
		DWORD code = MAKEDWORD(fieldcode, wmocode);
		std::pair<_SHAREDFIELD_HASH::iterator,bool> ret = m_SharedFieldInfoHash.insert( _SHAREDFIELD_HASH::value_type(code, p) ); 
		ASSERT(ret.second);
	}

	_FIELD_HASH m_FieldInfoHash;
	_REGEN_HASH m_UnitRegenInfoMgrHash;
	_GROUP_HASH m_UnitGroupInfoMgrHash;
	static util::CMemoryPoolFactory<GameField> m_GameFieldPool;
	static util::CMemoryPoolFactory<Sector>	m_SectorPool;
	_SHAREDFIELD_HASH	m_SharedFieldInfoHash;

};

extern ResrcManager g_ResrcMgr;
