/**
 *	@file		GlobalMacro.h
 *	@brief		이곳에 각종 메크로들을 올려서 사용합시다.
 *  @author		Unknown
 *	@version	Unknown
 */

////----------------------------------------------------------------------------
///** global macro header
//	@author : Yu, Jae Young (y2jinc@webzen.co.kr)
//	@desc : 이곳에 각종 메크로들을 올려서 사용합시다.
//	@remark 
//*/

#ifndef GLOBALMACRO_H
#define GLOBALMACRO_H

#pragma once

#define DoNothing()		// 아무것도 실행될 필요가 없음을 분명히 해 주는 매크로

#define SET_ARRAY_VECTOR(fOutArrVec, x, y, z)		{ fOutArrVec[0] = x; fOutArrVec[1] = y; fOutArrVec[2] = z; }

//---------------------------------------------------------------------------------------
//		스트링 관련
//---------------------------------------------------------------------------------------
#ifdef Sprintf
#undef Sprintf
#endif

#define CONVERT_LENGTH 2048
__forceinline void ConvertToASCII(IN wchar_t * pInStr , OUT char * pOutStr)
{
	WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, pInStr, -1 , pOutStr, CONVERT_LENGTH , NULL, NULL );
}

__forceinline void ConvertToUNICODE(IN char * pInStr , OUT wchar_t * pOutStr )
{
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pInStr, -1, pOutStr, CONVERT_LENGTH );
}

namespace GLOBALMACRO
{
	// strcpy
	__forceinline void StrCopy( char * pSrc , const char * pDest )
	{
		strcpy( pSrc , pDest );
	}
	__forceinline void StrCopy( char * pSrc , const wchar_t * pDest)
	{
		WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, pDest, -1 , pSrc, CONVERT_LENGTH , NULL, NULL );
	}
	__forceinline void StrCopy( wchar_t * pSrc , const wchar_t * pDest)
	{
		wcscpy( pSrc , pDest);
	}
	__forceinline void StrCopy ( wchar_t * pSrc , const char * pDest)
	{
		MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pDest, -1, pSrc, CONVERT_LENGTH );
	}

	// strncopy
	__forceinline void StrnCopy( char * pSrc ,	const char * pDest ,size_t Length )
	{
		strncpy( pSrc , pDest , Length);
	}
	__forceinline void StrnCopy( char * pSrc ,	const wchar_t * pDest, size_t Length)
	{
		WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, pDest, -1 , pSrc, CONVERT_LENGTH , NULL, NULL );
	}
	__forceinline void StrnCopy( wchar_t * pSrc , const wchar_t * pDest, size_t Length)
	{
		wcsncpy( pSrc , pDest , Length );
	}
	__forceinline void StrnCopy ( wchar_t * pSrc , const char * pDest, size_t Length)
	{
		if( strlen(pDest) > 0)
			MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pDest, -1, pSrc, CONVERT_LENGTH );
	}

	// sprintf 
	__forceinline void Sprintf( char * pSrc , char * pDest , ...)
	{
		char	szOutputMsg[512] = {0,};

		va_list args; 
		va_start(args, pDest); 
		_vsnprintf( szOutputMsg, 512, pDest, args ); 
		va_end(args); 

		sprintf( pSrc , szOutputMsg );

	}
	__forceinline void Sprintf( wchar_t * pSrc , wchar_t * pDest , ...)
	{
		wchar_t	szOutputMsg[512] = {0,};

		va_list args; 
		va_start(args, pDest); 
		_vsnwprintf( szOutputMsg, 512, pDest, args ); 
		va_end(args);

		swprintf( pSrc , szOutputMsg );
	}

	//snprintf
	__forceinline void Snprintf( char * pSrc , size_t Length , char * pDest , ...)
	{
		char	szOutputMsg[512] = {0,};

		va_list args; 
		va_start(args, pDest); 
		_vsnprintf( szOutputMsg, 512, pDest, args ); 
		va_end(args); 

		_snprintf( pSrc , Length , szOutputMsg );

	}
	__forceinline void Snprintf( wchar_t * pSrc , size_t Length , wchar_t * pDest , ...)
	{
		wchar_t	szOutputMsg[512] = {0,};

		va_list args; 
		va_start(args, pDest); 
		_vsnwprintf( szOutputMsg, 512, pDest, args ); 
		va_end(args);

		_snwprintf( pSrc , Length , szOutputMsg );
	}

	// Strcmp
	__forceinline int Strcmp( const char * pStr1 , const char * pStr2)
	{
		return strcmp( pStr1 , pStr2 );	
	}
	__forceinline int Strcmp( const wchar_t * pStr1 , const wchar_t * pStr2 )
	{
		return wcscmp( pStr1 , pStr2 );
	}
	__forceinline int Strcmp( const char * pStr1 , const wchar_t * pStr2)
	{
		char szMessage[2048 ]= {0,};
		WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, pStr2, -1 , szMessage, CONVERT_LENGTH , NULL, NULL );

		return strcmp( szMessage , pStr1 );	
	}
	__forceinline int Strcmp( const wchar_t * pStr1 , const char * pStr2 )
	{
		char szMessage[2048 ]= {0,};
		WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, pStr1, -1 , szMessage, CONVERT_LENGTH , NULL, NULL );
		return strcmp( szMessage , pStr2 );
	}

	/// Strncmp
	__forceinline int StrnCmp( const char * pStr1 , const char * pStr2 , size_t Length)
	{
		return strncmp( pStr1 , pStr2 , Length );
	}
	__forceinline int StrnCmp( const wchar_t * pStr1 , const wchar_t * pStr2 , size_t Length )
	{
		return wcsncmp( pStr1 , pStr2 , Length );
	}
	__forceinline int StrnCmp( const char * pStr1 , const wchar_t * pStr2 , size_t Length)
	{
		char szMessage[2048 ]= {0,};
		WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, pStr2, -1 , szMessage, CONVERT_LENGTH , NULL, NULL );

		return strncmp( szMessage , pStr1 , Length);	
	}
	__forceinline int StrnCmp( const wchar_t * pStr1 , const char * pStr2 , size_t Length)
	{
		char szMessage[2048 ]= {0,};
		WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, pStr1, -1 , szMessage, CONVERT_LENGTH , NULL, NULL );
		return strncmp( szMessage , pStr2 , Length);
	}
}

#define		StrnCopy				_tcsncpy
#define		StrCopy					_tcscpy
#define		Strcmp					_tcscmp
#define		Stricmp					_tcsicmp
#define		Sprintf					_stprintf
#define		Snprintf				_sntprintf
#define		StrnCmp					GLOBALMACRO::StrnCmp

//---------------------------------------------------------------------------------------
//		UI 컨트롤 관련
//---------------------------------------------------------------------------------------
#define UI_CONTROL_STATIC_PROPERTY(Name, WzID)					\
	public:														\
		inline CCtrlStaticWZ * GetCtrl##Name()					\
		{														\
			return (CCtrlStaticWZ *) GetControlWZ(WzID);		\
		}														\
		inline DWORD GetWzID##Name()							\
		{														\
			return GetControlWZ(WzID)->GetCtrlID();				\
		}														\
		inline void SetCaption##Name(TCHAR * pszData)			\
		{														\
			GlobalFunc::SetCaption((CCtrlStaticWZ *)GetControlWZ(WzID), pszData); \
		}														\
		inline void SetCaption##Name(DWORD dwStrID)				\
		{														\
			TCHAR szBuf[INTERFACE_STRING_LENGTH];				\
			g_InterfaceManager.GetInterfaceString(dwStrID, szBuf, INTERFACE_STRING_LENGTH); \
			GlobalFunc::SetCaption((CCtrlStaticWZ *)GetControlWZ(WzID), szBuf); \
		}														\
		inline void Show##Name(BOOL bFlag)						\
		{														\
			ENUM_STATEWZ es = bFlag? WZ_SHOW:WZ_HIDE;			\
			GetControlWZ(WzID)->ShowWindowWZ(es);				\
		}														\
		inline BOOL IsShow##Name()								\
		{														\
			return (BOOL)WZ_SHOW == GetControlWZ(WzID)->GetState(); \
		}

#define UI_CONTROL_VSCROLL_PROPERTY(Name, WzID)					\
	public:														\
		inline CCtrlVScrollWZ * GetCtrl##Name()					\
		{														\
		return (CCtrlVScrollWZ *) GetControlWZ(WzID);			\
		}														\
		inline DWORD GetWzID##Name()							\
		{														\
			return GetControlWZ(WzID)->GetCtrlID();				\
		}														\
		inline void Show##Name(BOOL bFlag)						\
		{														\
			ENUM_STATEWZ es = bFlag? WZ_SHOW:WZ_HIDE;			\
			GetControlWZ(WzID)->ShowWindowWZ(es);				\
		}														\
		inline BOOL IsShow##Name()								\
		{														\
			return (BOOL)WZ_SHOW == GetControlWZ(WzID)->GetState(); \
		}

#define UI_CONTROL_HSCROLL_PROPERTY(Name, WzID)					\
	public:														\
		inline CCtrlHScrollWZ * GetCtrl##Name()					\
		{														\
			return (CCtrlHScrollWZ *) GetControlWZ(WzID);		\
		}														\
		inline DWORD GetWzID##Name()							\
		{														\
			return GetControlWZ(WzID)->GetCtrlID();				\
		}														\														\
		inline void Show##Name(BOOL bFlag)						\
		{														\
			ENUM_STATEWZ es = bFlag? WZ_SHOW:WZ_HIDE;			\
			GetControlWZ(WzID)->ShowWindowWZ(es);				\
		}														\
		inline BOOL IsShow##Name()								\
		{														\
			return (BOOL)WZ_SHOW == GetControlWZ(WzID)->GetState(); \
		}

#define UI_CONTROL_TI_PROPERTY(Name, WzID)						\
	public:														\
		inline CCtrlImageStaticWZ * GetCtrl##Name()				\
		{														\
			return (CCtrlImageStaticWZ *) GetControlWZ(WzID);	\
		}														\
		inline DWORD GetWzID##Name()							\
		{														\
			return GetControlWZ(WzID)->GetCtrlID();				\
		}														\
		inline void SetCaption##Name(TCHAR * pszData)			\
		{														\
			GlobalFunc::SetCaption((CCtrlStaticWZ *)GetControlWZ(WzID), pszData); \
		}														\
		inline void SetCaption##Name(DWORD dwStrID)	\
		{														\
			TCHAR szBuf[INTERFACE_STRING_LENGTH];									\
			g_InterfaceManager.GetInterfaceString(dwStrID, szBuf, INTERFACE_STRING_LENGTH); \
			GlobalFunc::SetCaption((CCtrlStaticWZ *)GetControlWZ(WzID), szBuf); \
		}														\
		inline void Show##Name(BOOL bFlag)						\
		{														\
			ENUM_STATEWZ es = bFlag? WZ_SHOW:WZ_HIDE;			\
			GetControlWZ(WzID)->ShowWindowWZ(es);		\
		}														\
		inline BOOL IsShow##Name()								\
		{														\
			return (BOOL)WZ_SHOW == GetControlWZ(WzID)->GetState(); \
		}	

#define UI_CONTROL_BUTTON_PROPERTY(Name, WzID)					\
	public:														\
		inline CCtrlButtonWZ * GetCtrl##Name()					\
		{														\
			return (CCtrlButtonWZ *) GetControlWZ(WzID);		\
		}														\
		inline DWORD GetWzID##Name()							\
		{														\
			return GetControlWZ(WzID)->GetCtrlID();				\
		}														\
		inline void SetCaption##Name(TCHAR * pszData)			\
		{														\
			GlobalFunc::SetCaption((CCtrlButtonWZ *)GetControlWZ(WzID), pszData); \
		}														\
		inline void SetCaption##Name(DWORD dwStrID)	\
		{														\
			TCHAR szBuf[INTERFACE_STRING_LENGTH];									\
			g_InterfaceManager.GetInterfaceString(dwStrID, szBuf, INTERFACE_STRING_LENGTH); \
			GlobalFunc::SetCaption((CCtrlButtonWZ *)GetControlWZ(WzID), szBuf); \
		}														\
		inline void Show##Name(BOOL bFlag)						\
		{														\
			ENUM_STATEWZ es = bFlag? WZ_SHOW:WZ_HIDE;			\
			GetControlWZ(WzID)->ShowWindowWZ(es);		\
		}														\
		inline BOOL IsShow##Name()								\
		{														\
			return (BOOL)WZ_SHOW == GetControlWZ(WzID)->GetState(); \
		}	

#define UI_CONTROL_CHECK_BUTTON_PROPERTY(Name, WzID)			\
	public:														\
		inline CCtrlButtonCheckWZ * GetCtrl##Name()				\
		{														\
			return (CCtrlButtonCheckWZ *) GetControlWZ(WzID);	\
		}														\
		inline DWORD GetWzID##Name()							\
		{														\
			return GetControlWZ(WzID)->GetCtrlID();				\
		}														\
		inline void SetCaption##Name(TCHAR * pszData)			\
		{														\
			GlobalFunc::SetCaption((CCtrlButtonWZ *)GetControlWZ(WzID), pszData); \
		}														\
		inline void SetCaption##Name(DWORD dwStrID)	\
		{														\
			TCHAR szBuf[INTERFACE_STRING_LENGTH];									\
			g_InterfaceManager.GetInterfaceString(dwStrID, szBuf, INTERFACE_STRING_LENGTH); \
			GlobalFunc::SetCaption((CCtrlButtonWZ *)GetControlWZ(WzID), szBuf); \
		}														\
		inline void SetCheck##Name(BOOL bFlag)					\
		{														\
			CCtrlButtonCheckWZ * pCtrl = (CCtrlButtonCheckWZ *)GetControlWZ(WzID);	\
			if (pCtrl)											\
			{													\
				pCtrl->SetCheckState(FALSE != bFlag);			\
			}													\
		}\
		inline BOOL GetCheck##Name()							\
		{														\
			CCtrlButtonCheckWZ * pCtrl = (CCtrlButtonCheckWZ *)GetControlWZ(WzID);\
			if (pCtrl)											\
			{													\
				return pCtrl->GetCheckState();					\
			}													\
		}\
		inline void Show##Name(BOOL bFlag)						\
		{														\
			ENUM_STATEWZ es = bFlag? WZ_SHOW:WZ_HIDE;			\
			GetControlWZ(WzID)->ShowWindowWZ(es);		\
		}														\
		inline BOOL IsShow##Name()								\
		{														\
			return (BOOL)WZ_SHOW == GetControlWZ(WzID)->GetState(); \
		}														
	

#define UI_CONTROL_LIST_PROPERTY(Name, WzID)					\
	public:														\
		inline CCtrlListWZ * GetCtrl##Name()					\
		{														\
			return (CCtrlListWZ *) GetControlWZ(WzID);			\
		}														\
		DWORD GetWzID##Name()									\
		{														\
			return GetControlWZ(WzID)->GetCtrlID();				\
		}														\
		inline void Show##Name(BOOL bFlag)						\
		{														\
			ENUM_STATEWZ es = bFlag? WZ_SHOW:WZ_HIDE;			\
			GetControlWZ(WzID)->ShowWindowWZ(es);				\
		}														\
		inline BOOL IsShow##Name()								\
		{														\
			return (BOOL)WZ_SHOW == GetControlWZ(WzID)->GetState(); \
		}

//-----------------------------------------------------------
#define TYPEDEF_MAP(Name, KEY_TYPE, DATA_TYPE)			\
	typedef std::map<KEY_TYPE, DATA_TYPE>	Name##_MAP;	\
	typedef Name##_MAP::iterator			Name##_ITR;	\
	typedef std::pair<KEY_TYPE, DATA_TYPE>		Name##_PAIR; 

#define WzIdToConst(l1,l2,l3,l4) ((l1) * 0x01000000 + (l2) * 0x00010000 + (l3) * 0x00000100 + (l4) * 0x00000001)

#define GET_USER_INTERFACE( type, id )	((type*)g_InterfaceManager.GetUserInterface( id ))
#define	GET_DIALOG( id )				((uiBase*)g_InterfaceManager.GetDialog( id ))
#define GET_CAST_DIALOG( type, id )		((type*)g_InterfaceManager.GetDialog( id ) )
#define GET_DIALOG_MANAGER( type, id )	((type*)g_InterfaceManager.GetUserInterfaceManager( id ))
#define RGBATOARGB(r,g,b,a)        ((COLORREF)((((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16))|(((DWORD)(BYTE)(a))<<24)))	//. ARGB 

#define IS_NUMBERIC(t)		((t) == VALUE_TYPE_VALUE)
#define IS_PERCENTAGE(t)	((t) != VALUE_TYPE_VALUE)

#endif

