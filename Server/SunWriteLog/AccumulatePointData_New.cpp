#include "stdafx.h"
#include ".\AccumulatePointData.h"

AccumulatePointData::AccumulatePointData(VOID)
{
    SetCategory( LOG_ACCUMULATEPOINT );
}

AccumulatePointData::~AccumulatePointData(VOID)
{
}

VOID AccumulatePointData::Init()
{
    BaseLogData::Init();

    m_AccumulateYear = 0;
    m_AccumulateTime = 0;
    m_OldAccumulatePoint = 0;
    m_NewAccumulatePoint = 0;
    m_RewardItemCode = 0;
}

VOID AccumulatePointData::GetBodyDataText( TCHAR* pszBodyData, int nSize )
{
    if( !pszBodyData )	return;

    _sntprintf( pszBodyData, nSize-1, ",%d,%d,%d,%d,%d" 
        , m_AccumulateYear
        , m_AccumulateTime
        , m_OldAccumulatePoint
        , m_NewAccumulatePoint
        , m_RewardItemCode
        );

    pszBodyData[nSize] = '\0';
}

VOID AccumulatePointData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
    BYTE cSize = 0;

    BaseLogData::Serialize(rArchive, bAddText);

    rArchive << (WORD)EXPDATA_LAST_VERSION;
    //바이너리
    if(EXPDATA_LAST_VERSION >= 1)
    {
        _CHECK_LOG_SIZE_AND_WRITE( cSize, m_AccumulateYear, rArchive);
        _CHECK_LOG_SIZE_AND_WRITE( cSize, m_AccumulateTime, rArchive);
        _CHECK_LOG_SIZE_AND_WRITE( cSize, m_OldAccumulatePoint, rArchive);
        _CHECK_LOG_SIZE_AND_WRITE( cSize, m_NewAccumulatePoint, rArchive);
        _CHECK_LOG_SIZE_AND_WRITE( cSize, m_RewardItemCode, rArchive);
    }

    //텍스트 추가. 스냅샷은 택스트 로그 안 남긴다.
    if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
    {		
        GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );
        rArchive.WriteText(m_szBodyText, (MAX_TEXT_SIZE + 1) * sizeof(TCHAR) );
    }	
}

VOID AccumulatePointData::Read(CLogArchive& rArchive)
{
    BYTE cSize = 0;

    BaseLogData::Read(rArchive);

    rArchive >> m_ReadVersion;
    //바이너리
    if(m_ReadVersion >= 1)
    {
        LOG_READ( cSize, m_AccumulateYear, rArchive );
        LOG_READ( cSize, m_AccumulateTime, rArchive );
        LOG_READ( cSize, m_OldAccumulatePoint, rArchive );
        LOG_READ( cSize, m_NewAccumulatePoint, rArchive );
        LOG_READ( cSize, m_RewardItemCode, rArchive );
    }
}

