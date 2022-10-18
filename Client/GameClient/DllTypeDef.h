// dlltypedef.h

#ifndef _SUN_DLLTYPEDEF_H_
#define _SUN_DLLTYPEDEF_H_

// 3dfileaccess
#ifndef FUNC_ContactWzd
typedef void* (*FUNC_ContactWzd)( ENUM_CONTACT_WZD, void* );
#endif

// terrain
#ifndef FUNC_ContactTerrain
typedef void* (*FUNC_ContactTerrain)( ENUM_CONTACT_TERRAIN, void* );
#endif

// effect
#ifndef FUNC_ContactEffect
typedef void* (*FUNC_ContactEffect)( ENUM_CONTACT_EFFECT, void* );
#endif

// interface
#ifndef FUNC_DialogManagerWZ
typedef void* (*FUNC_DialogManagerWZ)( ENUM_USE_TYPE, void* );
#endif

#endif // _SUN_DLLTYPEDEF_H_