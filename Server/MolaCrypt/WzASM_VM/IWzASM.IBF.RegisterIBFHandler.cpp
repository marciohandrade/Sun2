#include "stdafx.h"
#include "IWzASM.h"
#include "IWzASM.IBF.h"

#if CODE_BACKUP

	VOID	sMETHOD_IBF_HANDLERS_REGISTER_CMD::RegisterHandlers()
	{
		M03_RegisterIBFHandler.Change( (WzPTR)FNPTR_IBF_HANDLER(_RegisterIBFHandler) );

		//		set<WORD>	findKEY;
		//		set<WORD>::iterator it;
		//#	undef DECL_IBF_HASH
		//#	define DECL_IBF_HASH(name, tag, val)					\
		//	M##val##_04 . Change( (HANDLER_WzASM_IBF_##name) );							\
		//	it = findKEY.find( M##val##_02.VALUE );										\
		//	if( it != findKEY.end() )		printf( "\nDUPLICATED KEY %02X\n", *it );	\
		//	else							findKEY.insert( M##val##_02.VALUE );

	#	undef DECL_IBF_HASH
	#	define DECL_IBF_HASH(name, tag, val)					\
		M##val##_04 . Change( (HANDLER_WzASM_IBF_##name) );

	#	include "IWzASM.IBFList.h"

	#ifndef _DEBUG
	}
	#else //_DEBUG
		Output();
	}

	VOID	sMETHOD_IBF_HANDLERS_REGISTER_CMD::Output()
	{
		BYTE* pOFF = (BYTE*)this;
		for( DWORD i=0 ; i<sizeof(*this) ; ++i, ++pOFF )
		{
			printf( "[%02X]", *pOFF );
		}
		puts("");

		WORD wSIZE;
		CommanStreamInfo( pOFF, wSIZE );
	}
	#endif //

#endif //CODE_BACKUP


