#include "stdafx.h"
#include ".\mapinfoloader.h"

#if(0)

#include <SolarFileReader.h>
#include <Macro.h>
#include <ProgramCommon/WzDataType.h>
#include "FieldInfo.h"
#include "MapInfo.h"
#include <MapInfoParser.h>
#include "GameServerEx.h"

MapInfoLoader::MapInfoLoader()
{
	m_pMapInfoHash = new MAPINFO_HASH;
	m_pMapInfoHash->Initialize( MAX_MAPINFO_NUM );
	m_pFieldInfoHash = new SHAREDFIELDINFO_HASH;
	m_pFieldInfoHash->Initialize( MAX_SHAREDFIELDINFO_NUM );
}

MapInfoLoader::~MapInfoLoader()
{
	SAFE_DELETE( m_pMapInfoHash );
	SAFE_DELETE( m_pFieldInfoHash );
}

VOID MapInfoLoader::UnloadInfo()
{
	if( m_pMapInfoHash )
	{
		MapInfo * pInfo = NULL;
		MAPINFO_HASH_IT it, end(m_pMapInfoHash->end());
		for( it = m_pMapInfoHash->begin() ; it != end ; ++it )
		{
			pInfo = (*it);
			SAFE_DELETE( pInfo );
		}
		m_pMapInfoHash->clear();
	}

	if( m_pFieldInfoHash )
	{
		SharedFieldInfo * pInfo = NULL;
		SHAREDFIELDINFO_HASH_IT it, end(m_pFieldInfoHash->end());
		for( it = m_pFieldInfoHash->begin() ; it != end ; ++it )
		{
			
			pInfo = (*it);
			pInfo->Destroy();
			SAFE_DELETE( pInfo );
		}
		m_pFieldInfoHash->clear();
	}
}

MapInfo * MapInfoLoader::_CreateMapInfo( sMAPINFO * _pMAPINFO_, BOOL bUsingViewPort )
{
	/*
	MapInfo * pMapInfo = new MapInfo;
	pMapInfo->SetMapInfo( _pMAPINFO_ );

	for(int i=0;i<sMAPINFO::MAX_FIELD_NUM;++i)
	{
		if( 0 != _pMAPINFO_->FCode[i] )
		{
			SharedFieldInfo *pSharedField = _GetSharedFieldInfo( _pMAPINFO_->FCode[i] );

			// �� ������ ������ ������ �������� ��!!!
			if( !pSharedField )
			{
				SUNLOG( eCRITICAL_LOG, "[_CreateMapInfo] MapCode[%d] FieldCode[%d] �� ��� ���̸� �����ٶ�!!! -���-", _pMAPINFO_->MapCode, _pMAPINFO_->FCode[i] );
				continue;
			}

			FieldInfo * pFieldInfo = new FieldInfo( const_cast<SharedFieldInfo *>(pSharedField) );
			pFieldInfo->EstablishSectorInfo( (eZONETYPE)_pMAPINFO_->byMKind, bUsingViewPort );
			pMapInfo->AddFieldInfo( pFieldInfo );
		}
	}
	
	return pMapInfo;
	*/
	return NULL;
}

VOID MapInfoLoader::EstablishInfo()
{
	MapInfo * pMapInfo		= NULL;
	MapInfoParser * pParser = MapInfoParser::Instance();

	// ���� �ʵ� �ε� �� �߰� 
	sFIELDINFO_HASH & rFieldHash = pParser->GetFieldInfoHash();
	sFIELDINFO_HASH_IT itendFieldInfo = rFieldHash.end();
	for( sFIELDINFO_HASH_IT it = rFieldHash.begin() ; it != itendFieldInfo ; ++it )
	{
		sFIELDINFO * psFieldInfo = const_cast<sFIELDINFO *>(*it);

		// �ش� ����Ÿ�Կ� �´� �ʸ� �ε��Ѵ�.
		if( psFieldInfo->byServerType != sFIELDINFO::SERVER_TYPE_BOTH )
		{
			if( g_pGameServer->GetServerType() == FIELD_SERVER )
			{
//				if( psFieldInfo->byServerType != sFIELDINFO::SERVER_TYPE_FIELD )	continue;
			}
			else if( g_pGameServer->GetServerType() == BATTLE_SERVER )
			{
				if( psFieldInfo->byServerType != sFIELDINFO::SERVER_TYPE_BATTLE )	continue;
			}
		}

		SharedFieldInfo * pSharedField = new SharedFieldInfo;
		pSharedField->Create( psFieldInfo );
		_AddSharedFieldInfo( (MAPCODE)(*it)->FieldCode, pSharedField );
		//break;

	}

	// ����Ʈ ���� �ʰ� 
	// �������� �ʴ� ������ �����Ͽ� �߰�
	sMAPINFO_HASH & rHash = pParser->GetMapInfoHash();
	sMAPINFO_HASH_IT itendMapInfo = rHash.end();
	for( sMAPINFO_HASH_IT it = rHash.begin() ; it != itendMapInfo ; ++it )
	{
		sMAPINFO *psMapInfo = const_cast<sMAPINFO *>(*it);

		// �ش� ����Ÿ�Կ� �´� �ʸ� �ε��Ѵ�.(�ϴ� �ʵ弭���� ��� �ε��Ѵ�.)
		if( g_pGameServer->GetServerType() != FIELD_SERVER )
		{
			eZONETYPE ztZONE = (eZONETYPE)psMapInfo->GetZoneType4SharedCode();
			if( ztZONE == eZONETYPE_VILLAGE ||
                ztZONE == eZONETYPE_DOMINATION_FIELD ||
                ztZONE == eZONETYPE_FIELD )
			{
				if( g_pGameServer->GetServerType() != FIELD_SERVER )	continue;
			}
			else if( ztZONE == eZONETYPE_LOBBY || 
                     ztZONE == eZONETYPE_CHARSELECT )
			{
				continue;
			}
			else
			{
				if( g_pGameServer->GetServerType() != BATTLE_SERVER )	continue;
			}
		}

		pMapInfo = _CreateMapInfo( psMapInfo, FALSE );
		_AddMapInfo( pMapInfo->GetMapCode(), FALSE, pMapInfo );
		//break;

		pMapInfo = _CreateMapInfo( psMapInfo, TRUE );
		_AddMapInfo( pMapInfo->GetMapCode(), TRUE, pMapInfo );
	}
}









#endif



