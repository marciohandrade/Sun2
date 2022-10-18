// typedef.h

#ifndef _3DFILEACCESS_TYPEDEF_H_
#define _3DFILEACCESS_TYPEDEF_H_

#include <client/3dengine/common/wzcommontypedef.h>

#ifdef WZ3DFILEACCESS_EXPORTS
#define WZ3DA_API   __declspec( dllexport )
#else
#define WZ3DA_API   __declspec( dllimport )
#endif

#define VERSION_WZD_FILE        (120)
#define VERSION_WZUNIT_FILE     (138)

#define IDSTR_WZDANI            ("WZAD")
#define IDSTR_WZDMESH           ("WZMD")
#define IDSTR_WZUNIT            ("WZUT")
#define IDSTR_WZLIGHTMAPUV      ("WLMU")

#endif
