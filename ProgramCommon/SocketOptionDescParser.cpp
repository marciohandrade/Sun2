#include "StdAfx.h"
#include "SocketOptionDescParser.h"

__EMPTY_FILE_TAG();

#if SUN_CODE_BACKUP // NOTE: f110609.10L, don't use anymore
#include <Struct.h>


SocketOptionDescParser::SocketOptionDescParser():	m_pDataTable ( NULL )
{
}

SocketOptionDescParser::~SocketOptionDescParser()
{
	FASSERT( m_pDataTable == NULL);
}

VOID SocketOptionDescParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

VOID SocketOptionDescParser::Init(DWORD dwPoolSize)
{
	FASSERT( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_SocketOptionDesc*>;
	m_pDataTable->Initialize( dwPoolSize );
}

VOID SocketOptionDescParser::Init(DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

void SocketOptionDescParser::Unload()
{
    if (m_pDataTable == NULL) {
        return;
    };
    m_pDataTable->SetFirst();
    while (BASE_SocketOptionDesc* info = m_pDataTable->GetNext()) {
        delete info;
    }
    m_pDataTable->RemoveAll();
}

BASE_SocketOptionDesc* SocketOptionDescParser::GetSocketOptionDesc(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL	SocketOptionDescParser::_LoadEx( eSCRIPT_CODE ScriptCode, util::SolarFileReader& sr, BOOL bReload )
{
	FASSERT( m_pDataTable != NULL );
	// ���������� ���ε� ����� �����Ƿ� ���� Load()���� Unload�� ȣ������ �� ��!!
	// => Unload()ȣ���� Release()�� ���ְ� ����
	// Unload();
	while( TRUE )
	{
		eTOKEN_TYPE ettType = sr.GetNextTokenType();
		if ( TOKEN_END == ettType )
		{
			break;
		}

		BASE_SocketOptionDesc * pInfo = new BASE_SocketOptionDesc;
		pInfo->m_dwCode = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_STRING); strncpy( pInfo->m_szName, sr.GetTokenString(), MAX_SOCKET_DESC );
		sr.GetNextTokenType(TOKEN_NUMBER); pInfo->m_dwNCode = sr.GetTokenNumber();
		FASSERT( NULL == m_pDataTable->GetData( pInfo->m_dwCode ) );
		m_pDataTable->Add( pInfo, pInfo->m_dwCode );
	}

	return TRUE;
}

#pragma warning ( pop )
#endif //SUN_CODE_BACKUP