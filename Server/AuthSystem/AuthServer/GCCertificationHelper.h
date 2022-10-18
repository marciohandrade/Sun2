#pragma once
#include "time.h"
#include <Singleton.h>

//////////////////////////////////////////////////////
// Gamechu BaseUserinfo sturct

#define GAMECHU_SERVER_KEY L"@GameOn@%#SUN#$%%2010@"

struct gcBaseUserInfo
{
	wchar_t		szUserSerialNumber[64];
	wchar_t		szNickName[64];
	__time64_t  ttTimeStamp;
	int			nPCBang;
	int			nSex;
	int			nAge;
    int         nPCBangServiceType;
	int			nLocation;
	int			nBirthYear;
};

typedef int (*TF_CheckCertification)( const wchar_t*,
									  const wchar_t*, 
									  const wchar_t*, 
									  gcBaseUserInfo* , 
									  DWORD, 
									  BOOL);


class GCCertificaltionHelper : public util::Singleton<GCCertificaltionHelper>
{
public:    
    enum eGCCModuleResult
    {
        GCC_SUCCESS         = 0,
        GCC_ERROR_MD5       = 1,
        GCC_ERROR_TIMESTAMP = 10,
        GCC_ERROR_PARSING   = 100,
        GCC_ERROR_USERINFO  = 101,
        GCC_ERROR_INPUTPARAM= 102,
        GCC_ERROR_UNKNOWN   = 1000,
    };
public:
	GCCertificaltionHelper() 
	{ 
		m_hInstGCLauncherDLL   = NULL; 
		m_pfCheckCertification = NULL;
	}
	
	~GCCertificaltionHelper()
	{ 
		if(m_hInstGCLauncherDLL)
		{
			::FreeLibrary(m_hInstGCLauncherDLL); 

			m_hInstGCLauncherDLL   = NULL;
			m_pfCheckCertification = NULL;
		}
	}

protected:
	HINSTANCE			  m_hInstGCLauncherDLL;
	TF_CheckCertification m_pfCheckCertification;
	

public:
	BOOL Load_GamechuDLL() /* Load DLL and Set function */ 
	{
#ifdef _DEBUG
		m_hInstGCLauncherDLL = LoadLibrary("GCCertificationD.dll");
#else
        m_hInstGCLauncherDLL = LoadLibrary("GCCertification.dll");
#endif
		if( m_hInstGCLauncherDLL == NULL ) 
			return FALSE;		

		m_pfCheckCertification = (TF_CheckCertification)GetProcAddress(m_hInstGCLauncherDLL, 
                                 "CheckCertification" );

		return TRUE;
	}

	//////////////////////////////////////////////////////
	// GetUserCertification
	// --------------------
	// szString    :
	// szServerKey : using MD5 key
	// pUserInfo   :
	//
	// return value --------------------------------------
	// return 0: success Certification
	// return other value : Error Certification 
	//          1 : md5 value error
	//         10 : TimeStamp over
	//        100 : Parsing Error
	//        101 : UserINfo Error
	//        102 : InputParam Error
	//       1000 : Unknown Error
	// ---------------------------------------------------
	int  CheckCertification( const wchar_t*  wzString, 
							 const wchar_t*  wzStatIndex,
							 const wchar_t*  wzServerKey,
							 gcBaseUserInfo* pUserInfo,
							 DWORD dTimeoutInterval)
	{
		if ( m_pfCheckCertification != NULL )
			return m_pfCheckCertification(wzString, wzStatIndex, wzServerKey, pUserInfo, 
                                          dTimeoutInterval, FALSE);

		return GCC_ERROR_UNKNOWN;
	}

    int  CheckCertification( const wchar_t*  wzString, 
        const wchar_t*  wzStatIndex,        
        gcBaseUserInfo* pUserInfo)
    {
        if ( m_pfCheckCertification != NULL )
        {
            DWORD start_tickcount = GetTickCount();

            int return_value =  m_pfCheckCertification(wzString, wzStatIndex, GAMECHU_SERVER_KEY, 
                                                       pUserInfo, 300, FALSE);

            DWORD end_tickcount = GetTickCount();

            if(end_tickcount - start_tickcount > 3000)
            {
                SUNLOG(eCRITICAL_LOG, "GameChu Certification Module Delay Time %u", 
                       end_tickcount - start_tickcount);
            }

            return return_value;
        }

        return GCC_ERROR_UNKNOWN;
    }
};
