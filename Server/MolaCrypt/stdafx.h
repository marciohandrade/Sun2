//	stdafx.h : (??????)
//	MolaVersion.h : (071117)
//	author : waverix
//
#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _LIB
#   define _MOLA_LIB
#endif

#ifdef _USRDLL
#   define _MOLA_EXPORTS
#endif

#ifdef _SERVER
#   define SERVER_CODE          (1)
#   define CLIENT_CODE          (!SERVER_CODE)
#else //
#   define SERVER_CODE          (0)
#   define CLIENT_CODE          (!SERVER_CODE)
#endif //

//==================================================================================================
//
#include "../../include/Global.Define.h"
#include <tchar.h>
#include <windows.h>
#include <Define.h>
#include <Const.h>
#include <UtilityCommon.h>

#include <Struct.h>
#include <Protocol.h>
#include <Protocol_CG.h>
#include <PacketStruct_CG.h>

#include <MemoryPoolFactory.h>
#include <ITimeout.h>

//==================================================================================================
//	<VERSION CONTROL & MILESTONE UPDATE>
#include "MolaVersion.h"

//==================================================================================================
//
#include <Macros/RandomGeneratorPrototypes.h>
#include <Macros/MOLA_PROTOCOL_HASH.h>
#include <Macros/ConstVariables.h>
#include <Macros/ConstStructure.h>

//==================================================================================================
//
#ifdef _MOLA_LIB
//
#   include <BitOperation.h>
#   include <CryptHSEL.h>
#   include <JSCode.h>
#   include <CryptManager.h>
#   include <RandomSNKey.Gender.h>
#   include <Macros/MOLA_CS_Struct.h>
#   include <CryptManager.SecurePacket.h>

#   include <VMOp/MOLA.chunk_stl.h>
#   include <CryptManager.ISecureWorkNode.h>
#   include <IWzASM.h>
#   include <VMOp/VMOp.Units.h>
#   include <VMOp/VMOpCPU.h>
#   include <Cryptmanager.ISecureWorkNode.CmdStream.h>

#else //_MOLA_LIB

#endif //_MOLA_LIB
//

//==================================================================================================

#define MODULE_HANDLER_DECL(name)   \
    extern BOOL MODULE_##name( uREG_NODE* pNODE )

#define MODULE_HANDLER_IMPL(name)   \
    BOOL        MODULE_##name( uREG_NODE* pNODE )

#define MODULE_HANDLER_CALL(name, args) \
    MODULE_##name( args )

//==================================================================================================
//
MODULE_HANDLER_DECL(UNIT_TEST);
MODULE_HANDLER_DECL(REGISTER_QUERY_MOLA_CMD);

//==================================================================================================
typedef SharedMemoryPool MOLASharedMemoryPool;

#define MolaTAllocMalloc(_size_) TAllocMalloc(_size_)
#define MolaTAllocFree(_ptr_) TAllocFree(_ptr_)

#define MolaTAllocNew(_type_) TAllocNew(_type_)
#define MolaTAllocDelete(_type_, _instance_) TAllocDelete(_type_, _instance_)
