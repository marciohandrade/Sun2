#include "stdafx.h"
#include "IWzASM.h"
#include "IWzASM.IBF.h"

#if CODE_BACKUP
	#undef DECL_GRES_HASH
	#define DECL_GRES_HASH(name, val, val2)							\
		do {														\
		IWzASM_PROC& rHANDLER = m_HASHED_HANDLER[GResHASH::name];	\
		rHANDLER.DATASIZE = 0;										\
		rHANDLER.RESFUNC = CALL_IGRes##name;						\
		} while(0);
	
	VOID		GResHASH::RegisterIGRes()
	{
	#	include "IWzASM.ResHashList.h"
	}
#endif //CODE_BACKUP


#undef DECL_GRES_HASH
#define DECL_GRES_HASH(name, val, val2)						\
	uREG_NODE	CALL_IGRes##name( uREG_NODE* pARGs )		\
	{														\
		uREG_NODE uRET = { 0, };							\
		DEBUGGING_OUTPUT();									\
		uRET = CALL_GRES_OBJECT_HANDLER( name, pARGs );		\
		return uRET;										\
	}

//

#	include "IWzASM.ResHashList.h"

#undef DECL_GRES_HASH


