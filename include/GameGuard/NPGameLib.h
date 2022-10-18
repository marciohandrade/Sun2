//=============================================================================
//    File : NPGameLib.h
//     Use : Handle Communication between Game and GameMon
//  Author : Kim Chan Ho (chkim@inca.co.kr)
//    Date : 2003/07/01 - 03
//=============================================================================

#ifndef __NPGAMELIB_H__
#define __NPGAMELIB_H__


/// Definitions
#ifdef NPGAMEDLL_EXPORTS
#define NPGAMEDLL_API __declspec(dllexport)
#else
#define NPGAMEDLL_API
#endif

// Callback Message
#define NPGAMEMON_UNDEFINED            1000        // Undefined message
#define NPGAMEMON_COMM_ERROR           1001        // Communication error
#define NPGAMEMON_COMM_CLOSE           1002        // Communication closing

#define NPGAMEMON_SPEEDHACK            1011        // SpeedHack detected
#define NPGAMEMON_GAMEHACK_KILLED      1012        // GameHack killed
#define NPGAMEMON_GAMEHACK_DETECT      1013        // GameHack detected
#define NPGAMEMON_INIT_ERROR           1014        // GameMon Init Error
#define NPGAMEMON_GAMEHACK_DOUBT       1015        // GameHack doubt
#define NPGAMEMON_CHECK_CSAUTH         1016        // CSAuth
#define NPGAMEMON_CHECK_CSAUTH2        1017        // CSAuth2
#define NPGAMEMON_GAMEHACK_REPORT      1018        // GameHack report
#define NPGAMEMON_CHECK_CSAUTH3        1019        // CSAuth3

// Error Code  110 - 300
#define NPGAMEMON_ERROR_EXIST          110         // GameMon Already Exist
#define NPGAMEMON_ERROR_CREATE         111         // GameGuard Directory Create Error
#define NPGAMEMON_ERROR_NPSCAN         112         // npscan.des Error
#define NPGAMEMON_ERROR_THREAD         113         // CreateThread Error
#define NPGAMEMON_ERROR_INIT           114         // GameMon Initialize Error
#define NPGAMEMON_ERROR_GAME_EXIST     115         // Game Instance Already Exist
#define NPGAMEMON_ERROR_AUTH_INI       120         // .ini Authentication Fail
#define NPGAMEMON_ERROR_AUTH_NPGMUP    121         // npgmup.des Authentication Fail
#define NPGAMEMON_ERROR_AUTH_GAMEMON   122         // GameMon.des Authentication Fail
#define NPGAMEMON_ERROR_AUTH_NEWUP     123         // npgmup.des.new Auth Fail
#define NPGAMEMON_ERROR_AUTH_GAMEGUARD 124         // GameGuard.des Authentication Fail
#define NPGAMEMON_ERROR_AUTH_DLL       125         // NPGameDLL.dll Authentication Fail
#define NPGAMEMON_ERROR_DECRYPT        130         // .ini File Decryption Fail
#define NPGAMEMON_ERROR_CORRUPT_INI    141         // Corrupt ini file Error
#define NPGAMEMON_ERROR_CORRUPT_INI2   142         // Not match GameName in ini file Error
#define NPGAMEMON_ERROR_NFOUND_INI     150         // ini File not Found
#define NPGAMEMON_ERROR_NFOUND_NPGMUP  151         // npgmup.des not found
#define NPGAMEMON_ERROR_NFOUND_NEWUP   152         // npgmup.des.new not found
#define NPGAMEMON_ERROR_NFOUND_GG      153         // GameGuard.des not found
#define NPGAMEMON_ERROR_NFOUND_GM      154         // GameMon.des not found
#define NPGAMEMON_ERROR_CRYPTOAPI      155         // rsabase.dll is corrupted
#define NPGAMEMON_ERROR_COMM           160         // Communication Init Error
#define NPGAMEMON_ERROR_EXECUTE        170         // GameMon Execute Error
#define NPGAMEMON_ERROR_EVENT          171         // GameMon Event Create Error
#define NPGAMEMON_ERROR_NPGMUP         180         // npgmup.dll Error
#define NPGAMEMON_ERROR_MOVE_INI       191         // Move ini Error
#define NPGAMEMON_ERROR_MOVE_NEWUP     192         // Move npgmup.des.new Error

#define NPGAMEMON_ERROR_ILLEGAL_PRG    200         // Detected a illegal program

#define NPGAMEMON_ERROR_GAMEMON        210         // GameMon Init Error
#define NPGAMEMON_ERROR_SPEEDCHECK     220         // SpeedCheck Init Error
#define NPGAMEMON_ERROR_GAMEGUARD      230         // GameGuard Init Error

// Error Code  310 - 400
// Update Error Code
#define NPGMUP_ERROR_PARAM             320         // Invalid .ini
#define NPGMUP_ERROR_INIT              330         // npgmup.des Initialize Error
#define NPGMUP_ERROR_DOWNCFG           340         // Download Error
#define NPGMUP_ERROR_ABORT             350         // Canceled by User
#define NPGMUP_ERROR_AUTH              360         // File Authentication Error
#define NPGMUP_ERROR_AUTH_INI          361         // .ini Authentication Error
#define NPGMUP_ERROR_DECRYPT           370         // .ini Decrypt Error
#define NPGMUP_ERROR_CONNECT           380         // Connect to update server Fail
#define NPGMUP_ERROR_INI               390         // .ini is corrupted

// Error Code  500
// Collision Error Code
#define NPGG_ERROR_COLLISION				500         // Detected a Collision Program

#define NPGAMEMON_SUCCESS              0x755       // GameMon Initialize Success


// USER_POLYBUFFER (NPGE L5 only)
#ifdef _X86_
#define USER_POLYBUFFER \
	__asm _emit 0xEB \
	__asm _emit 0x04 \
	__asm _emit 0xEB \
	__asm _emit 0x05 \
	__asm _emit 0x39 \
	__asm _emit 0x19

#else
#define USER_POLYBUFFER
#endif


// gameguard auth data
typedef struct _GG_AUTH_DATA
{
	DWORD dwIndex;
	DWORD dwValue1;
	DWORD dwValue2;
	DWORD dwValue3;
} GG_AUTH_DATA, *PGG_AUTH_DATA;

typedef struct _CSAuth3Data
{
	DWORD dwServerNumber;
	DWORD dwPacketSize;
	BYTE  bPacket[4096];

}CSAuth3Data, * PCSAuth3Data;

#if defined(_LIB) || !defined(__LITE_VERSION)

#ifdef __cplusplus
extern "C" {
#endif

	NPGAMEDLL_API DWORD __cdecl CheckNPGameMon();
	NPGAMEDLL_API BOOL  __cdecl CloseNPGameMon();
	NPGAMEDLL_API DWORD __cdecl InitNPGameMon();
	NPGAMEDLL_API void  __cdecl SetHwndToGameMon(HWND hWnd);
	NPGAMEDLL_API DWORD __cdecl PreInitNPGameMonA(LPCSTR szGameName);
	NPGAMEDLL_API DWORD __cdecl PreInitNPGameMonW(LPCWSTR szGameName);
	NPGAMEDLL_API BOOL  __cdecl SendUserIDToGameMonA(LPCSTR szUserID);
	NPGAMEDLL_API BOOL  __cdecl SendUserIDToGameMonW(LPCWSTR szUserID);
	NPGAMEDLL_API BOOL  __cdecl SendCSAuthToGameMon(DWORD dwAuth);
	NPGAMEDLL_API BOOL  __cdecl SendCSAuth2ToGameMon(PGG_AUTH_DATA pAuth);
	NPGAMEDLL_API BOOL  __cdecl SendCSAuth2ToGameMonWithSeperator(PGG_AUTH_DATA pAuth, DWORD dwSeperator);
	NPGAMEDLL_API BOOL  __cdecl SendCSAuth3ToGameMon(PBYTE pbPacket, DWORD dwPacketSize, DWORD dwServerNumber);
	NPGAMEDLL_API LPSTR __cdecl NPDect(LPSTR lpszString);
	NPGAMEDLL_API BOOL  __cdecl NPGuardData(LPVOID lpData, DWORD dwSize);
	NPGAMEDLL_API BOOL  __cdecl NPReleaseData(LPVOID lpData, DWORD dwSize);
	NPGAMEDLL_API LPCSTR __cdecl GetInfoFromGameMon();
	NPGAMEDLL_API LPBYTE __cdecl GetHackInfoFromGameMon(DWORD* dwSize);
	NPGAMEDLL_API BOOL  __cdecl ResourceAuthA(LPCSTR lpszFilename, bool bCheckFilename /* = true */ );
	NPGAMEDLL_API BOOL  __cdecl ResourceAuthW(LPCWSTR lpszFilename, bool bCheckFilename /* = true */ );
	NPGAMEDLL_API BOOL  __cdecl CheckD3DDevice(LPVOID lpD3DDevice, LPSTR lpszDll);
	NPGAMEDLL_API DWORD	__cdecl SetD3DDeviceInfo(DWORD dwD3DVer, LPVOID *lppD3DDevicePtr);	// 2011.02.09
	NPGAMEDLL_API DWORD __cdecl GGGetLastError();					// 2009.07.27
	NPGAMEDLL_API BOOL	__cdecl SetRunGGerror();					// 2009.10.12
	NPGAMEDLL_API void	__cdecl SetCommonDir(LPCSTR szCommon);      // 2010.04.21
	NPGAMEDLL_API DWORD __cdecl GetGameMonVersion(); // 2010.09.13
	NPGAMEDLL_API DWORD __cdecl InitPacketProtect(LPCSTR lpszUserKey);
	NPGAMEDLL_API DWORD __cdecl EncryptPacket(LPVOID lpData, DWORD dwLength);
	NPGAMEDLL_API DWORD __cdecl DecryptPacket(LPVOID lpData, DWORD dwLength);
	NPGAMEDLL_API DWORD __cdecl EncryptPeerPacket(LPVOID lpData, DWORD dwLength);
	NPGAMEDLL_API DWORD __cdecl DecryptPeerPacket(LPVOID lpData, DWORD dwLength);
	NPGAMEDLL_API DWORD __cdecl PPGetLastError();
	NPGAMEDLL_API BOOL	__cdecl FixVC80DEP();
	typedef BOOL (CALLBACK *NPGMCALLBACK)(DWORD dwMsg, DWORD dwArg);
	NPGAMEDLL_API void  __cdecl SetCallbackToGameMon(NPGMCALLBACK fnCallback);
	NPGAMEDLL_API DWORD __cdecl SetModulePathA(LPCSTR szModulePath);	// 2008.10.31
	NPGAMEDLL_API DWORD __cdecl SetModulePathW(LPCWSTR szModulePath); // 2008.10.31
	NPGAMEDLL_API BOOL  __cdecl IsAdminPrivilege();	// 2009.02.25
	//NPGAMEDLL_API BOOL	__cdecl GGDirect3DCreate9( UINT SDKVersion,  OUT LPVOID* pOriDirect3DCreate9Return ); // 2012.03.27
	NPGAMEDLL_API void	__cdecl ProtectWDDM(); // 2012.03.27
	NPGAMEDLL_API DWORD __cdecl SetUserIDA(LPCSTR szModulePath);	// 2013.03.04
	NPGAMEDLL_API DWORD __cdecl SetUserIDW(LPCWSTR szModulePath); // 2013.03.04

#ifdef __cplusplus
}
#endif


#ifdef _UNICODE
#define PreInitNPGameMon		PreInitNPGameMonW
#define SetModulePath			SetModulePathW
#define SendUserIDToGameMon		SendUserIDToGameMonW
#define ResourceAuth			ResourceAuthW
#define SetUserID				SetUserIDW
#else
#define SetModulePath			SetModulePathA
#define PreInitNPGameMon		PreInitNPGameMonA
#define SendUserIDToGameMon		SendUserIDToGameMonA
#define ResourceAuth			ResourceAuthA
#define SetUserID				SetUserIDA
#endif


#ifndef NPGAMELIB_C

/// CNPGameLib Wrapper Class ///
class CNPGameLib
{
public:
	explicit CNPGameLib(LPCTSTR lpszGameName)
	{
#ifdef NO_GAMEGUARD
		UNREFERENCED_PARAMETER(lpszGameName);
		return;
#else if
		PreInitNPGameMon(lpszGameName);
#endif
	}

	explicit CNPGameLib(LPCTSTR lpszGameName, LPCTSTR lpszModulePath)
	{
#ifdef NO_GAMEGUARD
		UNREFERENCED_PARAMETER(lpszGameName);
		UNREFERENCED_PARAMETER(lpszModulePath);
		return;
#else if
		SetModulePath(lpszModulePath);
		PreInitNPGameMon(lpszGameName);
#endif
	}

	explicit CNPGameLib(LPCTSTR lpszGameName, LPCTSTR lpszModulePath, LPCTSTR lpszUserID)
	{
#ifdef NO_GAMEGUARD
		UNREFERENCED_PARAMETER(lpszGameName);
		UNREFERENCED_PARAMETER(lpszModulePath);
		UNREFERENCED_PARAMETER(lpszUserID);
		return;
#else if
		SetModulePath(lpszModulePath);
		SetUserID(lpszUserID);
		PreInitNPGameMon(lpszGameName);
#endif
	}

	~CNPGameLib()
	{
#ifdef NO_GAMEGUARD
		return;
#else if
		CloseNPGameMon();
#endif
	}

public:
	DWORD Init()
	{
#ifdef NO_GAMEGUARD
		return NPGAMEMON_SUCCESS;
#else if
		return (InitNPGameMon());
#endif
	}

	void SetHwnd(HWND hWnd)
	{
#ifdef NO_GAMEGUARD
		UNREFERENCED_PARAMETER(hWnd);
		return;
#else if
		SetHwndToGameMon(hWnd);
#endif
	}

	DWORD Check()
	{
#ifdef NO_GAMEGUARD
		return NPGAMEMON_SUCCESS;
#else if
		return (CheckNPGameMon());
#endif
	}

	DWORD Send(LPCTSTR lpszUserId)
	{
#ifdef NO_GAMEGUARD
		UNREFERENCED_PARAMETER(lpszUserId);
		return NPGAMEMON_SUCCESS;
#else if
		return (SendUserIDToGameMon(lpszUserId));
#endif
	}

	LPCSTR GetInfo()
	{
#ifdef NO_GAMEGUARD
		return NULL;
#else if
		return (GetInfoFromGameMon());
#endif
	}
	LPBYTE GetHackInfo(DWORD *dwSize)
	{
#ifdef NO_GAMEGUARD
		UNREFERENCED_PARAMETER(dwSize);
		return NULL;
#else if
		return (GetHackInfoFromGameMon(dwSize));
#endif
	}
	DWORD Auth(DWORD dwAuth)
	{
#ifdef NO_GAMEGUARD
		UNREFERENCED_PARAMETER(dwAuth);
		return NPGAMEMON_SUCCESS;
#else if
		return (SendCSAuthToGameMon(dwAuth));
#endif
	}

	DWORD Auth2(PGG_AUTH_DATA pAuth)
	{
#ifdef NO_GAMEGUARD
		UNREFERENCED_PARAMETER(pAuth);
		return NPGAMEMON_SUCCESS;
#else if
		return (SendCSAuth2ToGameMon(pAuth));
#endif
	}

	DWORD Auth2(PGG_AUTH_DATA pAuth, DWORD dwSeperator)
	{
#ifdef NO_GAMEGUARD
		UNREFERENCED_PARAMETER(pAuth);
		UNREFERENCED_PARAMETER(dwSeperator);
		return NPGAMEMON_SUCCESS;
#else if
		return (SendCSAuth2ToGameMonWithSeperator(pAuth, dwSeperator));
#endif
	}

	DWORD Auth3(PBYTE pbPacket, DWORD dwPacketSize, DWORD dwServerNumber)
	{
#ifdef NO_GAMEGUARD
		UNREFERENCED_PARAMETER(pbPacket);
		UNREFERENCED_PARAMETER(dwPacketSize);
		UNREFERENCED_PARAMETER(dwServerNumber);
		return NPGAMEMON_SUCCESS;
#else if
		return (SendCSAuth3ToGameMon(pbPacket, dwPacketSize, dwServerNumber));
#endif
	}

	BOOL ResAuth(LPCTSTR lpszFilename, bool bCheckFilename = true)
	{
#ifdef NO_GAMEGUARD
		UNREFERENCED_PARAMETER(lpszFilename);
		UNREFERENCED_PARAMETER(bCheckFilename);
		return TRUE;
#else if
		return (ResourceAuth(lpszFilename, bCheckFilename));
#endif
	}

private:
	// No copies
	CNPGameLib(const CNPGameLib &rhs);
	CNPGameLib &operator=(const CNPGameLib &rhs);
};
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
///
/*
class CNPDword
{
public:
	CNPDword();
	//CNPDword(DWORD dwSize);
	~CNPDword();
	DWORD Get();
	void Set(DWORD dwValue);

private:
	void Guard();
	void Release();

public:
	LPDWORD m_pValue;
	DWORD m_dwSize;
};
*/

extern BOOL  __cdecl RegisterToMgr(LPVOID, DWORD dwOption);
extern void  __cdecl RemoveFromMgr(LPVOID);
extern DWORD __cdecl GetDataFromMgr(LPVOID);
extern void  __cdecl SetDataToMgr(LPVOID, DWORD);


class CNPDword {
public:
	CNPDword(DWORD dwOption) : m_dwData(0)
	{
		// �ɼ� ���� ��� �߰� (0 ����Ʈ)
		m_bFlag = RegisterToMgr(this, dwOption);
	}
	CNPDword() : m_dwData(0)
	{
		// ���� �ɼ� ������ ���ٸ� 100�� �⺻
		m_bFlag = RegisterToMgr(this, 100);
	}

	~CNPDword()
	{
		RemoveFromMgr(this);
	}

	DWORD Get()
	{
		if (m_bFlag)
			return (GetDataFromMgr(this) ^ (DWORD)this);
		else
			return m_dwData;
	}

	void Set(DWORD dwData)
	{
		if (m_bFlag)
			SetDataToMgr(this, dwData ^ (DWORD)this);
		else
			m_dwData = dwData;
	}

private:
	DWORD m_dwData;
	BOOL  m_bFlag;
};

#endif // NPGAMELIB_C

#else // defined(_LIB) || !defined(__LITE_VERSION)

#ifdef __cplusplus
extern "C" {
#endif

	NPGAMEDLL_API DWORD __cdecl StartGameGuard(LPCSTR szGameName);
	NPGAMEDLL_API BOOL  __cdecl EndGameGuard();

#ifdef __cplusplus
}
#endif

#endif // defined(_LIB) || !defined(__LITE_VERSION)

#endif // __NPGAMELIB_H__
