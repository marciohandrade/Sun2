// libinc.h

#ifndef _3DTERRAIN_LIBINC_H_
#define _3DTERRAIN_LIBINC_H_

#ifdef _DEBUG
    #ifdef _DLL
        #pragma comment( lib, "wzFilePacker_MD_static_d.lib" )
    #else
        #pragma comment( lib, "wzFilePacker_static_d.lib" )
    #endif
#else
    #ifdef _DLL
        #pragma comment( lib, "wzFilePacker_MD_static.lib" )
    #else
        #pragma comment( lib, "wzFilePacker_static.lib" )
    #endif
#endif

#ifdef _DEBUG
    #pragma comment( lib, "ProgramCommonLibForEngine_d.lib" )
    #pragma comment( lib, "Common_d.lib" )

    #ifndef _SERVER
        #pragma comment( lib, "3DFileAccess_d.lib" )
    #endif

#else
    #ifdef LAST_RELEASE
        #pragma comment( lib, "ProgramCommonLibForEngine.lib" )
        #pragma comment( lib, "Common.lib" )

        #ifndef _SERVER
            #pragma comment( lib, "3DFileAccess.lib" )
        #endif
    #else
        #pragma comment( lib, "ProgramCommonLibForEngine_p.lib" )
        #pragma comment( lib, "Common_p.lib" )

        #ifndef _SERVER
            #pragma comment( lib, "3DFileAccess_p.lib" )
        #endif
    #endif
#endif


#endif // _3DTERRAIN_LIBINC_H_