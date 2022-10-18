#include "SunClientPrecompiledHeader.h"
#include "idgenerator.h"

const DWORD kRangeGeneratorID = 1000;
const char* StartGeneratorID = "9999";

//------------------------------------------------------------------------------
/**
*/
IDGenerator::IDGenerator( void )
{
	Initialize();
}

//------------------------------------------------------------------------------
/**
*/
IDGenerator::~IDGenerator( void )
{
	m_idSet.clear();
}

//------------------------------------------------------------------------------
/**
*/
void IDGenerator::Initialize( void )
{
    TCHAR buf[5] = {'\0', };
    DWORD generator_id = StrToWzID(StartGeneratorID);
    ++generator_id;

	for (int count = 0; count < kRangeGeneratorID; ++count)
	{
        ++generator_id;
        m_idSet.push_back(generator_id);
	}
}	

//------------------------------------------------------------------------------
/**
*/
DWORD IDGenerator::GenerateID( void )
{
	if( m_idSet.empty() )
	{
		assert( !"[IDGenerator] 할당할 수 있는 ID가 없습니다." );

		return 0;
	}

    DWORD newId = m_idSet.front();

    m_idSet.pop_front();

	return newId;
}

//------------------------------------------------------------------------------
/**
*/
void IDGenerator::DestroyID( DWORD dwID )
{
    IdSet::iterator it = find( m_idSet.begin(), m_idSet.end(), dwID );

    if( it == m_idSet.end() )
    {
        m_idSet.push_back( dwID );
    }
    else
    {
        assert( !"[IDGenerator] 할당되지 않은 ID 반납 시도" );
    }
}
