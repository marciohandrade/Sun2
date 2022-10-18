#ifndef __UTIL_PACKFILE_H__
#define __UTIL_PACKFILE_H__
// util_packfile.h

#ifdef USE_PHYSICSFS

	#include "packFile/physfs.h"
	#include "packFile/pgfilearchive.h"
	extern PG_FileArchive packFile;

#else

	#include "wzFilePacker.h"
	extern CPackFile packFile;

#endif

extern HANDLE hVFS;

#endif //__UTIL_PACKFILE_H__