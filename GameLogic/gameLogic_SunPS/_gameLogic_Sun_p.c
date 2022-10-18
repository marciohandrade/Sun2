

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Thu Apr 20 13:28:19 2006
 */
/* Compiler settings for _gameLogic_Sun.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "_gameLogic_Sun.h"

#define TYPE_FORMAT_STRING_SIZE   97                                
#define PROC_FORMAT_STRING_SIZE   631                               
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   1            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ICSunOnline_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ICSunOnline_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need a Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure decodeSkill */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10 */	NdrFcShort( 0xd8 ),	/* 216 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 16 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 18 */	NdrFcShort( 0x1 ),	/* 1 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter buff */

/* 24 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter bstrXml */

/* 30 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Return value */

/* 36 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 38 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 40 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure decodeMission */

/* 42 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 44 */	NdrFcLong( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x8 ),	/* 8 */
/* 50 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 52 */	NdrFcShort( 0x90 ),	/* 144 */
/* 54 */	NdrFcShort( 0x8 ),	/* 8 */
/* 56 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 58 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 60 */	NdrFcShort( 0x1 ),	/* 1 */
/* 62 */	NdrFcShort( 0x0 ),	/* 0 */
/* 64 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter buff */

/* 66 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 68 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 70 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Parameter bstrXml */

/* 72 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 74 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 76 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Return value */

/* 78 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 80 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure decodeInventoryItem */

/* 84 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x9 ),	/* 9 */
/* 92 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 94 */	NdrFcShort( 0x5a1 ),	/* 1441 */
/* 96 */	NdrFcShort( 0x8 ),	/* 8 */
/* 98 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 100 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 102 */	NdrFcShort( 0x1 ),	/* 1 */
/* 104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 106 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter buff */

/* 108 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 110 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 112 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Parameter bstrXml */

/* 114 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 116 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 118 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Return value */

/* 120 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 122 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure decodeTempInventoryItem */

/* 126 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 128 */	NdrFcLong( 0x0 ),	/* 0 */
/* 132 */	NdrFcShort( 0xa ),	/* 10 */
/* 134 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 136 */	NdrFcShort( 0x18c ),	/* 396 */
/* 138 */	NdrFcShort( 0x8 ),	/* 8 */
/* 140 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 142 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 144 */	NdrFcShort( 0x1 ),	/* 1 */
/* 146 */	NdrFcShort( 0x0 ),	/* 0 */
/* 148 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter buff */

/* 150 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 152 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 154 */	NdrFcShort( 0x3c ),	/* Type Offset=60 */

	/* Parameter bstrXml */

/* 156 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 158 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 160 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Return value */

/* 162 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 164 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 166 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure decodeEquipItem */

/* 168 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 170 */	NdrFcLong( 0x0 ),	/* 0 */
/* 174 */	NdrFcShort( 0xb ),	/* 11 */
/* 176 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 178 */	NdrFcShort( 0x153 ),	/* 339 */
/* 180 */	NdrFcShort( 0x8 ),	/* 8 */
/* 182 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 184 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 186 */	NdrFcShort( 0x1 ),	/* 1 */
/* 188 */	NdrFcShort( 0x0 ),	/* 0 */
/* 190 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter buff */

/* 192 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 194 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 196 */	NdrFcShort( 0x42 ),	/* Type Offset=66 */

	/* Parameter bstrXml */

/* 198 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 200 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 202 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Return value */

/* 204 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 206 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure decodeWarehouseItem */

/* 210 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 212 */	NdrFcLong( 0x0 ),	/* 0 */
/* 216 */	NdrFcShort( 0xc ),	/* 12 */
/* 218 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 220 */	NdrFcShort( 0x77c ),	/* 1916 */
/* 222 */	NdrFcShort( 0x8 ),	/* 8 */
/* 224 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 226 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 228 */	NdrFcShort( 0x1 ),	/* 1 */
/* 230 */	NdrFcShort( 0x0 ),	/* 0 */
/* 232 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter buff */

/* 234 */	NdrFcShort( 0xa ),	/* Flags:  must free, in, */
/* 236 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 238 */	NdrFcShort( 0x48 ),	/* Type Offset=72 */

	/* Parameter bstrXml */

/* 240 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 242 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 244 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Return value */

/* 246 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 248 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure encodeSkill */

/* 252 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0xd ),	/* 13 */
/* 260 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 264 */	NdrFcShort( 0xfc ),	/* 252 */
/* 266 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 268 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 272 */	NdrFcShort( 0x1 ),	/* 1 */
/* 274 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrXml */

/* 276 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 278 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 280 */	NdrFcShort( 0x52 ),	/* Type Offset=82 */

	/* Parameter buff */

/* 282 */	NdrFcShort( 0x12 ),	/* Flags:  must free, out, */
/* 284 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 286 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter nResult */

/* 288 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 290 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 292 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 294 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 296 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 298 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure encodeMission */

/* 300 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 302 */	NdrFcLong( 0x0 ),	/* 0 */
/* 306 */	NdrFcShort( 0xe ),	/* 14 */
/* 308 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 310 */	NdrFcShort( 0x0 ),	/* 0 */
/* 312 */	NdrFcShort( 0xb4 ),	/* 180 */
/* 314 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 316 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 318 */	NdrFcShort( 0x0 ),	/* 0 */
/* 320 */	NdrFcShort( 0x1 ),	/* 1 */
/* 322 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrXml */

/* 324 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 326 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 328 */	NdrFcShort( 0x52 ),	/* Type Offset=82 */

	/* Parameter buff */

/* 330 */	NdrFcShort( 0x12 ),	/* Flags:  must free, out, */
/* 332 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 334 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Parameter nResult */

/* 336 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 338 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 340 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 342 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 344 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 346 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure encodeInventoryItem */

/* 348 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 350 */	NdrFcLong( 0x0 ),	/* 0 */
/* 354 */	NdrFcShort( 0xf ),	/* 15 */
/* 356 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 358 */	NdrFcShort( 0x0 ),	/* 0 */
/* 360 */	NdrFcShort( 0x5c5 ),	/* 1477 */
/* 362 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 364 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 366 */	NdrFcShort( 0x0 ),	/* 0 */
/* 368 */	NdrFcShort( 0x1 ),	/* 1 */
/* 370 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrXml */

/* 372 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 374 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 376 */	NdrFcShort( 0x52 ),	/* Type Offset=82 */

	/* Parameter buff */

/* 378 */	NdrFcShort( 0x12 ),	/* Flags:  must free, out, */
/* 380 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 382 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Parameter nResult */

/* 384 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 386 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 388 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 390 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 392 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure encodeTempInventoryItem */

/* 396 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 398 */	NdrFcLong( 0x0 ),	/* 0 */
/* 402 */	NdrFcShort( 0x10 ),	/* 16 */
/* 404 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0x1b0 ),	/* 432 */
/* 410 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 412 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 414 */	NdrFcShort( 0x0 ),	/* 0 */
/* 416 */	NdrFcShort( 0x1 ),	/* 1 */
/* 418 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrXml */

/* 420 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 422 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 424 */	NdrFcShort( 0x52 ),	/* Type Offset=82 */

	/* Parameter buff */

/* 426 */	NdrFcShort( 0x12 ),	/* Flags:  must free, out, */
/* 428 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 430 */	NdrFcShort( 0x3c ),	/* Type Offset=60 */

	/* Parameter nResult */

/* 432 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 434 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 436 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 438 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 440 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure encodeEquipItem */

/* 444 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 446 */	NdrFcLong( 0x0 ),	/* 0 */
/* 450 */	NdrFcShort( 0x11 ),	/* 17 */
/* 452 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */
/* 456 */	NdrFcShort( 0x177 ),	/* 375 */
/* 458 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 460 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 464 */	NdrFcShort( 0x1 ),	/* 1 */
/* 466 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrXml */

/* 468 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 470 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 472 */	NdrFcShort( 0x52 ),	/* Type Offset=82 */

	/* Parameter buff */

/* 474 */	NdrFcShort( 0x12 ),	/* Flags:  must free, out, */
/* 476 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 478 */	NdrFcShort( 0x42 ),	/* Type Offset=66 */

	/* Parameter nResult */

/* 480 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 482 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 484 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 486 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 488 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 490 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure encodeWarehouseItem */

/* 492 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 494 */	NdrFcLong( 0x0 ),	/* 0 */
/* 498 */	NdrFcShort( 0x12 ),	/* 18 */
/* 500 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 502 */	NdrFcShort( 0x0 ),	/* 0 */
/* 504 */	NdrFcShort( 0x7a0 ),	/* 1952 */
/* 506 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 508 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 512 */	NdrFcShort( 0x1 ),	/* 1 */
/* 514 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrXml */

/* 516 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 518 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 520 */	NdrFcShort( 0x52 ),	/* Type Offset=82 */

	/* Parameter buff */

/* 522 */	NdrFcShort( 0x12 ),	/* Flags:  must free, out, */
/* 524 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 526 */	NdrFcShort( 0x48 ),	/* Type Offset=72 */

	/* Parameter nResult */

/* 528 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 530 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 532 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 534 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 536 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 538 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure init */

/* 540 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 542 */	NdrFcLong( 0x0 ),	/* 0 */
/* 546 */	NdrFcShort( 0x13 ),	/* 19 */
/* 548 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 550 */	NdrFcShort( 0x0 ),	/* 0 */
/* 552 */	NdrFcShort( 0x8 ),	/* 8 */
/* 554 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 556 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 562 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 564 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 566 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 568 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure release */

/* 570 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 572 */	NdrFcLong( 0x0 ),	/* 0 */
/* 576 */	NdrFcShort( 0x14 ),	/* 20 */
/* 578 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 580 */	NdrFcShort( 0x0 ),	/* 0 */
/* 582 */	NdrFcShort( 0x8 ),	/* 8 */
/* 584 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 586 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 588 */	NdrFcShort( 0x0 ),	/* 0 */
/* 590 */	NdrFcShort( 0x0 ),	/* 0 */
/* 592 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 594 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 596 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 598 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure createItem */

/* 600 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 602 */	NdrFcLong( 0x0 ),	/* 0 */
/* 606 */	NdrFcShort( 0x15 ),	/* 21 */
/* 608 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 612 */	NdrFcShort( 0x8 ),	/* 8 */
/* 614 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 616 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 622 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 624 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 626 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 628 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/*  4 */	NdrFcShort( 0xc8 ),	/* 200 */
/*  6 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/*  8 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 10 */	NdrFcShort( 0x1c ),	/* Offset= 28 (38) */
/* 12 */	
			0x13, 0x0,	/* FC_OP */
/* 14 */	NdrFcShort( 0xe ),	/* Offset= 14 (28) */
/* 16 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 18 */	NdrFcShort( 0x2 ),	/* 2 */
/* 20 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 22 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 24 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 26 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 28 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 30 */	NdrFcShort( 0x8 ),	/* 8 */
/* 32 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (16) */
/* 34 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 36 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 38 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 40 */	NdrFcShort( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x4 ),	/* 4 */
/* 44 */	NdrFcShort( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0xffde ),	/* Offset= -34 (12) */
/* 48 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 50 */	NdrFcShort( 0x80 ),	/* 128 */
/* 52 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 54 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 56 */	NdrFcShort( 0x591 ),	/* 1425 */
/* 58 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 60 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 62 */	NdrFcShort( 0x17c ),	/* 380 */
/* 64 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 66 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 68 */	NdrFcShort( 0x143 ),	/* 323 */
/* 70 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 72 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 74 */	NdrFcShort( 0x76c ),	/* 1900 */
/* 76 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 78 */	
			0x12, 0x0,	/* FC_UP */
/* 80 */	NdrFcShort( 0xffcc ),	/* Offset= -52 (28) */
/* 82 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 84 */	NdrFcShort( 0x0 ),	/* 0 */
/* 86 */	NdrFcShort( 0x4 ),	/* 4 */
/* 88 */	NdrFcShort( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (78) */
/* 92 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 94 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ICSunOnline, ver. 0.0,
   GUID={0x14E845B9,0x0298,0x4CA1,{0x97,0xAC,0x6E,0x95,0x36,0x86,0x50,0x51}} */

#pragma code_seg(".orpc")
static const unsigned short ICSunOnline_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    42,
    84,
    126,
    168,
    210,
    252,
    300,
    348,
    396,
    444,
    492,
    540,
    570,
    600
    };

static const MIDL_STUBLESS_PROXY_INFO ICSunOnline_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ICSunOnline_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ICSunOnline_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ICSunOnline_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(22) _ICSunOnlineProxyVtbl = 
{
    &ICSunOnline_ProxyInfo,
    &IID_ICSunOnline,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::decodeSkill */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::decodeMission */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::decodeInventoryItem */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::decodeTempInventoryItem */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::decodeEquipItem */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::decodeWarehouseItem */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::encodeSkill */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::encodeMission */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::encodeInventoryItem */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::encodeTempInventoryItem */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::encodeEquipItem */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::encodeWarehouseItem */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::init */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::release */ ,
    (void *) (INT_PTR) -1 /* ICSunOnline::createItem */
};


static const PRPC_STUB_FUNCTION ICSunOnline_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _ICSunOnlineStubVtbl =
{
    &IID_ICSunOnline,
    &ICSunOnline_ServerInfo,
    22,
    &ICSunOnline_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x6000169, /* MIDL Version 6.0.361 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * __gameLogic_Sun_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ICSunOnlineProxyVtbl,
    0
};

const CInterfaceStubVtbl * __gameLogic_Sun_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ICSunOnlineStubVtbl,
    0
};

PCInterfaceName const __gameLogic_Sun_InterfaceNamesList[] = 
{
    "ICSunOnline",
    0
};

const IID *  __gameLogic_Sun_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define __gameLogic_Sun_CHECK_IID(n)	IID_GENERIC_CHECK_IID( __gameLogic_Sun, pIID, n)

int __stdcall __gameLogic_Sun_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!__gameLogic_Sun_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo _gameLogic_Sun_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & __gameLogic_Sun_ProxyVtblList,
    (PCInterfaceStubVtblList *) & __gameLogic_Sun_StubVtblList,
    (const PCInterfaceName * ) & __gameLogic_Sun_InterfaceNamesList,
    (const IID ** ) & __gameLogic_Sun_BaseIIDList,
    & __gameLogic_Sun_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

