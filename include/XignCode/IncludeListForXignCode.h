#pragma once
#ifndef PROGRAMCOMMON_INCLUDELIST_XIGNCODE_H
#define PROGRAMCOMMON_INCLUDELIST_XIGNCODE_H

#ifdef _SERVER

// use re-directive path ex: "XignCode/##version_path##/zwave_sdk_linker.h"
//#include "./110331/zwave_sdk_linker.h"
//#include "./110331/zwave_sdk_helper.h"
//#include "../../../include/XignCode/IncludeListForXignCode.h"
#define XIGNCODE_INCLUDE_PATH_HEADER( v, h )	 <../../../include/XignCode/##v/##h>
#include XIGNCODE_INCLUDE_PATH_HEADER( XIGNCODE_VERSION, zwave_sdk_linker.h )
#include XIGNCODE_INCLUDE_PATH_HEADER( XIGNCODE_VERSION, zwave_sdk_helper.h )

#else

// use re-directive path ex: "XignCode/##version_path##/zwave_sdk_client.h"
#include "./110331/zwave_sdk_client.h"

#endif

#endif //PROGRAMCOMMON_INCLUDELIST_XIGNCODE_H
