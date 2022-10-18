
#include <MemoryPoolFactory.h>

struct sPASSWORD_BLOCK_IN_DBPROXY
{
	enum { NONE = 0, WAREHOUSE = 1, INVENTORY = 2 };
	BYTE	byType;
	CHAR	pszPassword[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH+1];

	inline VOID		Init() { byType = NONE; memset(pszPassword, 0, sizeof(pszPassword)); }
	inline BOOL		IsEmpty() { return (BOOL)(pszPassword[0] == '\0'); }

	inline BYTE&	Type() { return byType; }

	inline CHAR*	Get() { return pszPassword; }
	inline VOID		Set( CHAR psPassword[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH] )
	{
		strncpy( pszPassword, psPassword, sizeof(pszPassword) );
		pszPassword[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH] = '\0';
	}
};

typedef util::CMemoryPoolFactory<sPASSWORD_BLOCK_IN_DBPROXY>		TEMP_PWD_POOL;
