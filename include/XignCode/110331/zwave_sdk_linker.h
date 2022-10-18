/*--
	Copyright (C) 2007-2010 Wellbia.com Co., Ltd.

	Module Name:
		zwave_sdk_linker.cpp

	Abstract:
		XIGNCODE Linker for Server

	Author:
		2010-10-18 HyunOh Kim <zextor@wellbia.com>

	Environment:
		win32

	Library:
		win32, stdc

--*/

#ifndef ZWAVE_SDK_LINKER
#define ZWAVE_SDK_LINKER

#include "xtypes.h"
#include "zwave_sdk_errors.h"

#define ZWAVE_STATE_STOP		0x0001
#define ZWAVE_STATE_RUN		0x0002

#ifdef _M_X64
#define LINK_DLL_NAME L"zwave_sdk_linker_x64.dll"
#else                 
#define LINK_DLL_NAME L"zwave_sdk_linker.dll"
#endif

enum Z_SCAN_TYPE_LIST
{
	// ġƮ���� Ž�� ��ȸ�� �����ڵ�
	Z_SCAN_TYPE_ANTI_CHEAT_ENGINE=1,
	// ��Ŭ���̾�Ʈ Ž�� ��ȸ�� �����ڵ�
	Z_SCAN_TYPE_ANTI_NONECLIENTBOT=2,
	// Ŭ���̾�Ʈ ��Ƽ�׸�Ƽ ���� �ڵ�
	Z_SCAN_TYPE_XIGN_VALID_CHECK=3
};


// �����
#ifndef Z_RETURN
#define Z_RETURN
enum Z_RETURN
{
	// �߸��� ��Ŷ
	Z_RTN_ERROR = -1,
	// �������� Ŭ���̾�Ʈ
	Z_RTN_NONE, 
	// ��Ŭ���̾�Ʈ �� Ŭ���̾�Ʈ
	Z_RTN_NONCLIENT,
	// ġƮ���� Ž��
	Z_RTN_BLACK_CODE,
	// ġƮ���� ���� Ž��
	Z_RTN_SUSPICIOUS,
	// ��ũ�� Ž��
	Z_RTN_USERDEFINED,
	// ����� ����
	Z_RTN_RESEND
	// ��Ŷ ������
};
#endif

#ifndef _CryptType
typedef enum _CryptType 
{ 
	Blowfish
	, Rijndael
	, Rc4 
} CryptType;
#endif

#ifdef _UNICODE
#define ZSWAVE_Initialize		ZSWAVE_InitializeW
#define ZSWAVE_GetLibraryPath	ZSWAVE_GetLibraryPathW
#define ZSWAVE_VerifyEx			ZSWAVE_VerifyExW
#define ZSWAVE_Link				ZSWAVE_LinkW
#else
#define ZSWAVE_Initialize		ZSWAVE_InitializeA
#define ZSWAVE_GetLibraryPath	ZSWAVE_GetLibraryPathA
#define ZSWAVE_VerifyEx			ZSWAVE_VerifyExA
#define ZSWAVE_Link				ZSWAVE_LinkA
#endif

typedef xpch PACKETDATA;
typedef xpcch CPACKETDATA;

xbool	XCALL ZSWAVE_InitializeW(xcwstr path);
xbool	XCALL ZSWAVE_InitializeA(xpcch path);
xbool	XCALL ZSWAVE_Uninitialize();
xulong	XCALL ZSWAVE_GetDBConnectionCount();
xbool	XCALL ZSWAVE_GetLibraryPathW(xwstr path, xulong size);
xbool	XCALL ZSWAVE_GetLibraryPathA(xpch path, xulong size);
xhandle XCALL ZSWAVE_OpenAt(int gi, int di, xulong bs);
xhandle XCALL ZSWAVE_Open(xulong bs, xulong type);
xbool	XCALL ZSWAVE_Close(xhandle h);
xbool	XCALL ZSWAVE_GetDataAt(xhandle h, int no, xpch data);
xbool	XCALL ZSWAVE_Verify(xhandle h, xpcch data, xulong size);
int		XCALL ZSWAVE_VerifyExA(xhandle h, xpcch data, xulong size,  xstr msg, xulong msgSize);
int		XCALL ZSWAVE_VerifyExW(xhandle h, xpcch data, xulong size, xwstr msg, xulong msgSize);
int		XCALL ZSWAVE_GetDataCount(xhandle s);
xbool	XCALL ZSWAVE_CheckUpdates();

xbool	XCALL ZSWAVE_RsaFreeBuffer(xhandle H, xpuch Buffer);
xhandle	XCALL ZSWAVE_RsaCreate();
xbool	XCALL ZSWAVE_RsaClose(xhandle H);
xbool	XCALL ZSWAVE_RsaPublicDecrypt(xhandle H, xpcuch Input, xsize InputSize, xpuch *Output, xsize *OutputSize);
xbool	XCALL ZSWAVE_RsaPublicEncrypt(xhandle H, xpcuch Input, xsize InputSize, xpuch *Output, xsize *OutputSize);
xbool	XCALL ZSWAVE_RsaPrivateDecrypt(xhandle H, xpcuch Input, xsize InputSize, xpuch *Output, xsize *OutputSize);
xbool	XCALL ZSWAVE_RsaPrivateEncrypt(xhandle H, xpcuch Input, xsize InputSize, xpuch *Output, xsize *OutputSize);
xbool	XCALL ZSWAVE_RsaSetPublicKey(xhandle H, xpcuch Key, xsize KeySize);
xbool	XCALL ZSWAVE_RsaSetPrivateKey(xhandle H, xpcuch Key, xsize KeySize);
xbool	XCALL ZSWAVE_RsaGenerateKey(xpuch PublicKey, xsize PublicKeySize, xpuch PrivateKey, xsize PrivateKeySize, int Nbits, int Exponent);
xbool	XCALL ZSWAVE_Decrypt(xpcuch Input, xsize InputSize, xpcuch Key, xsize KeySize, xpuch Output, xsize OutputSize, xsize *Required, CryptType Type);
xbool	XCALL ZSWAVE_Encrypt(xpcuch Input, xsize InputSize, xpcuch Key, xsize KeySize, xpuch Output, xsize OutputSize, xsize *Required, CryptType Type);


#endif