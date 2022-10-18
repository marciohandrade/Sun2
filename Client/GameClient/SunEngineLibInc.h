// SunEngineLibInc.h

#ifndef _SUN_SUNENGINELIBINC_H_
#define _SUN_SUNENGINELIBINC_H_

#ifndef ENGINE_LIB_PATH //#if'N'def
    #define ENGINE_LIB_PATH  "../../SUN3DENGINE/lib"
#endif

#pragma comment( lib, "../../Lib/libvisual2003_visual2008.lib")

#ifdef _DEBUG
	#pragma comment( lib, "../Lib/"VC_DIR_REDIRECT"/CrashReporter_ds.lib")
    #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/wzFilePacker_static_d.lib")
	#pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/ProgramCommonLibForEngine_d.lib")
	#pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/Common_d.lib")
	#pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/3DFrameWork_d.lib")

    #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/wzsound.lib")

#else
	#pragma comment( lib, "../Lib/"VC_DIR_REDIRECT"/CrashReporter_rs.lib")
    #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/wzFilePacker_static.lib")
	#ifdef LAST_RELEASE
		#pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/ProgramCommonLibForEngine.lib")
		#pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/Common.lib")
		#pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/3DFrameWork.lib")
	#else
		#pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/ProgramCommonLibForEngine_p.lib")
		#pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/Common_p.lib")
		#pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/3DFrameWork_p.lib")
	#endif

    #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/wzsound.lib")

#endif

// ���� 103���� �Ϲ���(implicit) ��ũ

#ifdef _DEBUG
    #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/3DFileAccess_d.lib" )
    #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/3DRendererD3D_d.lib" )
    #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/3DTerrain_d.lib" )
    #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/Effect_d.lib" )

    // xxx: �ٸ� dll�� �������� ���� ControlsDll.dll�� ��ũ�ϸ�
    // Win2000���� ���� ���ȴ�. xp������ ������.
    // ���� ��Ȯ�� ������ �𸣰ڰ� ���߿��� �˰� �Ǹ� ��������.

    // #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/ControlsDLL_d.lib" )
#else
    #ifdef LAST_RELEASE
        #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/3DFileAccess.lib" )
        #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/3DRendererD3D.lib" )
        #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/3DTerrain.lib" )
        #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/Effect.lib" )

        // #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/ControlsDLL.lib" )
    #else
        #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/3DFileAccess_p.lib" )
        #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/3DRendererD3D_p.lib" )
        #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/3DTerrain_p.lib" )
        #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/Effect_p.lib" )

        // #pragma comment( lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/ControlsDLL_p.lib" )
    #endif
#endif


#endif // _SUN_SUNENGINELIBINC_H_
