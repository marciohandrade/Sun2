#pragma once
#if SUN_CODE_BACKUP // NOTE: f110609.10L, don't use anymore
#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

#define MAX_SOCKET_DESC	0xFF


struct BASE_SocketOptionDesc {

     DWORD		m_dwCode;
     char		m_szName[MAX_SOCKET_DESC];
     DWORD		m_dwNCode;

};

class SocketOptionDescParser	: public util::Singleton<SocketOptionDescParser>, public IParser
{
public:
	SocketOptionDescParser();
	~SocketOptionDescParser();


	VOID           Init(DWORD dwPoolSize);
	VOID		   Init(DWORD dwPoolSize, char * pszPackFileName );
	VOID           Release();

	BASE_SocketOptionDesc*            GetSocketOptionDesc(DWORD dwKey);
	virtual	BOOL			_LoadEx( eSCRIPT_CODE ScriptCode, util::SolarFileReader& sr, BOOL bReload );


private:
	VOID		   Unload();
	util::SolarHashTable<BASE_SocketOptionDesc *>		*m_pDataTable;
};
#endif //SUN_CODE_BACKUP