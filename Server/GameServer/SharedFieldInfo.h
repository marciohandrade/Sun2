//=============================================================================================================================
/// SharedFieldInfo class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2006. 2. 24
	@purpose
		- *.map의 정보를 로드하여 게임 전체에 공유되는 필드 정보 클래스
*/
//=============================================================================================================================

#pragma once

#include <Struct.h>
#include <vector>
#include <ProgramCommon/WzArchive.h>
#include <3DEngine/Common/BoundingVolume.h>
#include <hash_map>
#include <TriggerInfoManager.h>

typedef STLX_HASH_MAP<DWORD, MAPOBJECT_INFO *>			MAPOBJECT_INFO_HASH;
typedef MAPOBJECT_INFO_HASH::iterator					MAPOBJECT_INFO_HASH_IT;
typedef std::pair<DWORD, MAPOBJECT_INFO *>				MAPOBJECT_INFO_PAIR;

class SharedFieldInfo
{
	enum { _MAX_MAPOBJECT_INFO_NUM = 100 };
public:		SharedFieldInfo(void);
			~SharedFieldInfo(void);

			BOOL				Create(
									FIELDCODE fieldcode
									, const char* field_path
									, FIELDCODE wmocode
									, BOOL bNewPathEngine
									, BOOL bWarCastle );
			VOID				Destroy();

	inline	DWORD				GetRandomStartingTileIndex( BYTE byClass );
	inline	BOOL				CheckMapVersion( DWORD dwCheckSum );
	inline	TriggerGroupInfo*	GetTriggerGroupInfo( const TCHAR * pszGroupName );
	inline	CWorldBase*			GetWorldBase() { return m_pWorldBase; }

	inline	MAPOBJECT_INFO_HASH&	GetMapObjectInfoHashTable() { return m_MapObjectInfoHash; }
	inline	MAPOBJECT_INFO*		FindMapObjectInfo( DWORD dwObjectID )
			{
				MAPOBJECT_INFO_HASH_IT it = m_MapObjectInfoHash.find(dwObjectID);
				if( it != m_MapObjectInfoHash.end() ) return it->second;
				return NULL;
			}

	inline	BOOL				IsNewPathEngine()	{ return m_bNewPathEngine; }
	inline	BOOL				IsWarCastle()		{ return m_bWarCastle; }
    // (f100623.1L) cache for checksums
    const EngineFileChecksum& GetFileChecksum() const { return checksums_; }

protected:	BOOL				_Load( const char * field_path );
			BOOL				_Serialize_WMO( CWzArchive & archive );
			BOOL				_LoadMapObjectInfo( const char * pszFilePath );
			BOOL				_LoadPathEngineCollision( char * pszFileName );
			VOID				_SerializeMapObjectAll( CWzArchive & archive );
			VOID				_SetStartingTileIndex();
			VOID				_SetCRStartingTileIndex();
			VOID				_UnloadMapObjectInfo();
			VOID				_CheckVersionBetweenMapWmo( DWORD MapVersion, DWORD WmoVersion );

private:	__PROPERTY( FIELDCODE, Code );
			__PROPERTY( FIELDCODE, WMOCode );
			__PROPERTY( WORD, Version );
			CWorldBase*				m_pWorldBase;
			MAPOBJECT_INFO_HASH		m_MapObjectInfoHash;
			STLX_VECTOR<DWORD>		m_StartTiles;
			STLX_VECTOR<DWORD>		m_CR1StartTiles;
			STLX_VECTOR<DWORD>		m_CR2StartTiles;

			INT						m_LastUsedRoomIndex;
			friend class nsAI::MovingPlanner;
			BOOL					m_bNewPathEngine;
			BOOL					m_bWarCastle;
            // (f100623.1L) cache for checksums
            EngineFileChecksum checksums_;
			TriggerInfoManager		m_TriggerInfoManager;
			TCHAR					m_tszMapPath[MAX_PATH];
			TCHAR					m_tszWmoPath[MAX_PATH];
};


// <NOTE>
//   byClass == 0 이면 모든 캐릭터 Village
inline DWORD	SharedFieldInfo::GetRandomStartingTileIndex( BYTE byClass )
{
	const DWORD dwCR1 = (1<<eCHAR_BERSERKER | 1<<eCHAR_ELEMENTALIST);
	const DWORD dwCR2 = (1<<eCHAR_DRAGON | 1<<eCHAR_VALKYRIE | 1<<eCHAR_SHADOW);

	if( eCHAR_TYPE_MAX > byClass )
	{
        STLX_VECTOR<DWORD>* pStartTiles = NULL;
		DWORD dwBitField = (1<<byClass);
		if( dwBitField == 1 )
			pStartTiles = &m_StartTiles;
		else if( dwBitField & dwCR1 )
			pStartTiles = &m_CR1StartTiles;
		else if( dwBitField & dwCR2 )
			pStartTiles = &m_CR2StartTiles;

		if( !pStartTiles )
		{
			// 무슨 이런 뭐같은 경우가 있나?
#ifdef _DEBUG
			ASSERT( !"[GetRandomStartingTileIndex]들어 올 수 없는 클래스" );
#else
			SUNLOG( eCRITICAL_LOG, "[GetRandomStartingTileIndex] Unexpected Class Code %u", byClass );
#endif
			return 0;
		}

		if( pStartTiles->empty() )
		{
			SUNLOG( eCRITICAL_LOG, "Warning:[FieldCode:%d][Class:%d]m_StartTiles == 0\n", m_Code, byClass );
			return 0;
		}

		random_shuffle( pStartTiles->begin(), pStartTiles->end() );
		return *(pStartTiles->begin());
	}
	return 0;
}

inline TriggerGroupInfo*	SharedFieldInfo::GetTriggerGroupInfo( const TCHAR * pszGroupName )
{
	return m_TriggerInfoManager.FindTriggerGruopInfo( pszGroupName );
}

inline BOOL		SharedFieldInfo::CheckMapVersion( DWORD dwCheckSum )
{
	if( m_pWorldBase->GetCheckSum() != dwCheckSum )
	{
		return FALSE;
	}
	else
	{
		if( 0 == dwCheckSum ) //< 0인 경우는 구버전 맵!!
			return FALSE;
	}
	return TRUE;
}

