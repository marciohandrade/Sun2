// SunEngine.h

#ifndef _SUN_SUNENGINE_H_
#define _SUN_SUNENGINE_H_

#include <programcommon/wztypedef.h>

#include <client/3dengine/common/wzcommontypedef.h>
#include <client/3dengine/common/mathassistant.h>

#include <client/3dengine/3dfileaccess/typedef.h>
#include <client/3dengine/3dfileaccess/3dfileaccess.h>

#include <client/3dengine/3drenderer/typedef.h>
#include <client/3dengine/3drenderer/drawbase.h>
#include <client/3dengine/3drenderer/SystemAndDriverInfo.h>

#include <client/3dengine/3dterrain/typedef.h>
#include <client/3dengine/3dterrain/3dterrain.h>
#include <client/3dengine/3dterrain/worldbase.h>
#include <client/3dengine/3dterrain/pathexplorer.h>
#include <client/3dengine/3dterrain/autocameraplay.h>

#include <client/3dengine/effect/typedef.h>
#include <client/3dengine/effect/effect.h>

#include <client/3dengine/effect/element/effectbody.h>
#include <client/3dengine/effect/element/eelementflatchain.h>

#include <client/3dengine/effect/effectmanager.h>
#include <client/3dengine/effect/soundinfomanager.h>

#include <client/3dengine/interface/typedef.h>
#include <client/3dengine/interface/dllaccess.h>
#include <client/3dengine/interface/dialogwz.h>

#if WZENGINEVER >= 630
#include <client/3dengine/interface/CtrlStaticWZ.h>
#include <client/3dengine/interface/CtrlButtonWZ.h>
#include <client/3dengine/interface/CtrlButtonCheckWZ.h>
#include <client/3dengine/interface/CtrlEditWZ.h>
#include <client/3dengine/interface/CtrlListWZ.h>
#include <client/3dengine/interface/CtrlPictureWZ.h>
#include <client/3dengine/interface/CtrlStaticWZ.h>
#include <client/3dengine/interface/CtrlRectangleWZ.h>
#include <client/3dengine/interface/CtrlClientWZ.h>
#include <client/3dengine/interface/CtrlTabWZ.h>
#include <client/3dengine/interface/CtrlHScrollWZ.h>
#include <client/3dengine/interface/CtrlDxEdit.h>
#include <client/3dengine/interface/CtrlWebBrowserWZ.h>
#else
#include <client/3dengine/interface/dlgaccess.h>
#endif // WZENGINEVER >= 630

#include <client/3dengine/interface/dialogmanager.h>
#include <client/3dengine/interface/ctrldxedit.h>
#include <client/3dengine/interface/BaseManager.h>
#include <client/3dengine/interface/DialogManager.h>

#include <client/3dengine/3dframework/typedef.h>
#include <client/3dengine/3dframework/applast.h>

#include <client/wzSound/wzSound.h>

#endif // _SUN_SUNENGINE_H_