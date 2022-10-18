#include "StdAfx.h"
#include ".\CollectionParser.h"

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
#include "../../Achievement/CollectTreasureBoxInfo.h"
#endif 

CollectionParser::CollectionParser(void)
{
	m_CollectionInfoHash.Initialize( MAX_COLLECTION_LIST_NUM );
}

CollectionParser::~CollectionParser(void)
{
	Unload();
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    CollectTreasureBoxInfo::Instance()->DestroyInstance();
#endif 
}


VOID CollectionParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );
}

VOID CollectionParser::Reload()
{
//	Load( m_tszFileName, TRUE );
}

VOID CollectionParser::Unload()
{
    COLLECTION_INFO_HASH::iterator it = m_CollectionInfoHash.begin(),
                                   end = m_CollectionInfoHash.end();
    for (  ; it != end; ++it)
    {
        const CollectionInfo* info = *it;
        delete info;
    }
    m_CollectionInfoHash.RemoveAll();
}

BOOL CollectionParser::_Validating( const CollectionInfo& Collection ) const
{
	if( Collection.m_CollectionID == 0 || 
		Collection.m_CollectionID == StrToWzID("NULL") )
	{
		SUNLOG( eCRITICAL_LOG, "[CollectionParser] UserID[%d]는 0이나 NULL이 될 수 없다.", Collection.m_CollectionID );
		ASSERT(FALSE);
	}
	
	return TRUE;
}

BOOL CollectionParser::_Load( BOOL bReload )
{
    __UNUSED(bReload);
	char szTemp[5] = {0,};
	CollectionInfo Collection;

	int rowSize = GetRowSize();
	for( int row = 0; row < rowSize; ++row )
	{
		memset(&Collection,0,sizeof(CollectionInfo));

		strncpy(szTemp, GetDataString("CollectionID",row), 4);
		Collection.m_CollectionID = StrToWzID(szTemp);

		Collection.m_dwRespawnTime	  = GetDataDWORD( "RespawnTime", row );
		Collection.m_dwCollectingTime = GetDataDWORD( "CollectingTime", row );


		strncpy(szTemp, GetDataString("AnimationCode_Start", row), 4);
		Collection.m_wzidAnimation[CollectionInfo::START] = StrToWzID(szTemp);
		strncpy(szTemp, GetDataString("AnimationCode_Loop", row), 4);
		Collection.m_wzidAnimation[CollectionInfo::LOOP] = StrToWzID(szTemp);
		strncpy(szTemp, GetDataString("AnimationCode_End", row), 4);
		Collection.m_wzidAnimation[CollectionInfo::END] = StrToWzID(szTemp);

		strncpy(szTemp, GetDataString("Collectable_EffectCode", row), 4);
		Collection.m_wzidEffect[CollectionInfo::COLLECTABLE] = StrToWzID(szTemp);
        //_NA001385_20090924_DOMINATION_STAMP
        strncpy(szTemp, GetDataString("Start_EffectCode", row), 4);
        Collection.m_wzidEffect[CollectionInfo::COLLECTING] = StrToWzID(szTemp);
        //
		strncpy(szTemp, GetDataString("Success_EffectCode", row), 4);
		Collection.m_wzidEffect[CollectionInfo::SUCCESS] = StrToWzID(szTemp);


		Collection.m_byScriptType = GetDataBYTE( "ScriptType", row );
		Collection.m_ItemCode	  = GetDataWORD( "ItemCode", row );
		Collection.m_IGType		  = GetDataInt( "IGType", row );

		Collection.m_Condition[CollectionInfo::QUEST]    = GetDataDWORD( "Condision_Quest", row );
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
        Collection.m_Condition[CollectionInfo::AC] = GetDataDWORD( "AC_LocationCode", row );
#ifdef _SERVER
        CollectTreasureBoxInfo::Instance()->AddCollectCondition(
            static_cast<WORD>(Collection.m_Condition[CollectionInfo::AC]),
            GetDataWORD( "AC_ConnectionCode1", row ),
            GetDataWORD( "AC_ConnectionCode2", row ));
#endif
#else
		Collection.m_Condition[CollectionInfo::AC]	     = GetDataDWORD( "Condision_AC", row );
#endif
        Collection.m_Condition[CollectionInfo::MAPOBJID] = GetDataDWORD( "Condision_ObjectID", row );
        Collection.m_Condition[CollectionInfo::STAMP]    = GetDataDWORD( "Condision_Stamp", row );
		Collection.m_Condition[CollectionInfo::COUNT]    = 1;	// 채집물은 1번만 채집 가능하다. (1회성 컨텐츠임. 1이상일시 와우의 채광방식) 
#ifdef _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
        Collection.m_Condition[CollectionInfo::TRIGGER]    = GetDataDWORD( "Condision_Trigger", row );
#endif //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
        
		//.//

		_Validating(Collection);

		//.//

		if( NULL == FindCollectionInfo(Collection.m_CollectionID) ) 
			m_CollectionInfoHash.Add(new const CollectionInfo(Collection), Collection.m_CollectionID);
	}

	return TRUE;
}


BOOL	CollectionParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_COLLECT:
		return _Load( bReload );
		break;
	}

	return FALSE;
}
