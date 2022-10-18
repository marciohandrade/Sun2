#include "SunClientPrecompiledHeader.h"
#include "SkillEventEffectInfoParser.h"



cSkillEventEffectInfoParser::cSkillEventEffectInfoParser()
: m_pDataTable(NULL)
{

}

cSkillEventEffectInfoParser::~cSkillEventEffectInfoParser()
{

}

VOID cSkillEventEffectInfoParser::Init(DWORD dwPoolSize, char * pszPackFileName)
{
    Init( dwPoolSize );
    SetPackFileName( pszPackFileName );
}

VOID cSkillEventEffectInfoParser::Init(DWORD dwPoolSize)
{
    assert( m_pDataTable == NULL );
    m_pDataTable = new util::SolarHashTable<cSkillEventEffectInfo*>;
    m_pDataTable->Initialize( dwPoolSize );
}

VOID cSkillEventEffectInfoParser::Release()
{
    Unload();
    if (m_pDataTable)
        delete m_pDataTable;
    m_pDataTable = NULL;

}


VOID cSkillEventEffectInfoParser::Unload()
{
    cSkillEventEffectInfo * pInfo = NULL;
    if( m_pDataTable == NULL )
    {
        return;
    }
    m_pDataTable->SetFirst();
    while( pInfo = m_pDataTable->GetNext() )
    {
        delete pInfo;
    }
    m_pDataTable->RemoveAll();
}


cSkillEventEffectInfo* cSkillEventEffectInfoParser::GetInfo(DWORD SkillClassCode)
{
    return m_pDataTable->GetData( SkillClassCode );
}


BOOL cSkillEventEffectInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    assert(ScriptCode == SCRIPT_SKILLEVENTEFFECT_INFO);

    int rowSize = GetRowSize();

    for (int row = 0; row < rowSize; ++row)
    {
        const char* EventType = GetDataString("EventType", row );
        const char* DisplayType = GetDataString("PositionType", row );
        const char* EffectCode = GetDataString("EffectCode", row );
        DWORD SkillClassCode = (DWORD)GetDataNumber("SkillClassCode", row );
        DWORD LifeTime = (DWORD)GetDataNumber("LifeTime", row );
        BOOL ChaseTarget = (BOOL)GetDataNumber("ChaseTarget", row );
        BOOL StaticRotate = (BOOL)GetDataNumber("StaticRotate", row );
        DWORD ActiveDelayTime = (DWORD)GetDataNumber("ActiveDelayTime", row );

        if( LifeTime == 0 )
            LifeTime = 1000;

        cSkillEventEffectInfo* pInfo = GetInfo( SkillClassCode );
        if( pInfo == NULL )
        {
            pInfo = new cSkillEventEffectInfo();
            pInfo->SetSkillClassCode( SkillClassCode );
            m_pDataTable->Add( pInfo, SkillClassCode );
        }

        cSkillEventEffectInfoData* pData = pInfo->AddData( EventType, DisplayType, "", EffectCode );
        pData->SetLifeTime( LifeTime );
        pData->SetDelayTime( ActiveDelayTime );
        pData->SetStaticRotate( StaticRotate );
        pData->SetChaseTarget( ChaseTarget );
    }

    return TRUE;
}
