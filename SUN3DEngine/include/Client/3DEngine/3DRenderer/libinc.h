// libinc.h

#ifndef _3DRENDERER_LIBINC_H_
#define _3DRENDERER_LIBINC_H_

#if defined(DEBUG) || defined(_DEBUG)
    #pragma comment( lib, "d3dx9dt.lib" )
#else
    #pragma comment( lib, "d3dx9.lib" )
#endif

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

    #pragma comment( lib, "3DFileAccess_d.lib" )
    #pragma comment( lib, "Effect_d.lib" )

#else

    #ifdef LAST_RELEASE
        #pragma comment( lib, "ProgramCommonLibForEngine.lib" )
        #pragma comment( lib, "Common.lib" )

        #pragma comment( lib, "3DFileAccess.lib" )
        #pragma comment( lib, "Effect.lib" )
    #else
        #pragma comment( lib, "ProgramCommonLibForEngine_p.lib" )
        #pragma comment( lib, "Common_p.lib" )

        #pragma comment( lib, "3DFileAccess_p.lib" )
        #pragma comment( lib, "Effect_p.lib" )
    #endif
#endif

#endif // _3DRENDERER_LIBINC_H_