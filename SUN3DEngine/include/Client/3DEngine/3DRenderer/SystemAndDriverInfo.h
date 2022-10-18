// systemanddriverinfo.h

#ifndef _3DRENDERER_SYSTEMANDDRIVERINFO_H_
#define _3DRENDERER_SYSTEMANDDRIVERINFO_H_

enum ENUM_PROCESSOR_MODEL
{
	EPM_Unknown = 0,

	// Intel Processor
	EPM_Intel_486DX = 1000,
	EPM_Intel_486SX,
	EPM_Intel_486DX2,
	EPM_Intel_486SL,
	EPM_Intel_486SX2,
	EPM_Intel_486DX2E,
	EPM_Intel_486DX4,
	EPM_Intel_Pentium,
	EPM_Intel_Pentium_MMX,
	EPM_Intel_Pentium_Pro,
	EPM_Intel_Pentium_II,
	EPM_Intel_Celeron,
	EPM_Intel_Pentium_III,
	EPM_Intel_Pentium_IV,

	// 차후 추가할 것
	// EPM_Intel_Pentium_D
	// EPM_Intel_Celeron_D


	// AMD Processor
	EPM_AMD_DX2 = 2000,
	EPM_AMD_DX4,
	EPM_AMD_AM5X86,
	EPM_AMD_K5,
	EPM_AMD_K6,
	EPM_AMD_K6_2,
	EPM_AMD_K6_3,
	EPM_AMD_Athlon,
	EPM_AMD_Duron,
	EPM_AMD_Athlon_64,
	EPM_AMD_Athlon_64_FX,


	EPM_UMC = 3000,			// United Microelectronics Corp. Processor
	EPM_Cyrix = 4000,		// Cyrix Corp., VIA Inc. Processor
	EPM_NexGen = 5000,		// NexGen Inc. (now AMD) Processor
	EPM_Centaur = 6000,		// IDT/Centaur (now VIA) Processor
	EPM_Rise = 7000,		// Rise Processor
	EPM_Transmeta = 8000,	// Transmeta Processor
	EPM_NSC = 9000,			// National Semiconductor Processor
};


struct St_SystemAndDriverInfo
{
	//{ 시스템 정보  }

	// DirectX 9.0c일 경우, 9,0,"c"
	unsigned long m_dwDirectXVersionMajor;		
	unsigned long m_dwDirectXVersionMinor;	
	char m_szDirectXVersionLetter[2];
	// RAM 1GB일 경우, 약 1024x1024x1024
	unsigned __int64 m_ullPhysicalMemory;
	// XP일 경우, "Windows XP Professional"
	char m_szOSEnglish[100];	
	// 인텔 CPU P4 3.0 HT일 경우, "Intel(R) Pentium(R) 4 CPU 3.00GHz (2 CPUs)"
	char m_szProcessorEnglish[200];
	
	// NumberOfCPUs

	// CPU 정보 
	int m_iProcessorFamilyID;
	int m_iProcessorExtendedFamilyID;
	int m_iProcessorModelID;
	int m_iProcessorSteppingCode;
	char m_szProcessorVendorName[13];
	int m_iProcessorSpeedInMHz;
	ENUM_PROCESSOR_MODEL m_eProcessorModel;

	// RAM 1GB일 경우, "1024MB RAM"
	char m_szPhysicalMemoryEnglish[100];
	// DirectX 9.0c일 경우, "DirectX 9.0c (4.09.0000.0904)"
	char m_szDirectXVersionLongEnglish[100];


	//{ 디스플레이 정보 }

	// 지포스 FX 5200일 경우, "NVIDIA GeForce FX 5200", "NVIDIA", "GeForce FX 5200"
	char m_szDescription[200];
	char m_szManufacturer[200];
	char m_szChipType[100];
	// 그래픽카드 RAM 128MB, "128.0 MB"
	char m_szDisplayMemoryEnglish[100];
	// 지포스 FX 5200일 경우, "nv4_disp.dll"
	char m_szDriverName[100];
	// 그래픽카드 드라이버 버전, 예) "6.14.0010.7801"
	char m_szDriverVersion[100];
	// 그래픽카드 드라이버 날짜, 예) "8/2/2005 16:35:00"
	char m_szDriverDateEnglish[100];
};	



inline bool GetSystemAndDriverInfo( const char *lpszDll, St_SystemAndDriverInfo* pInfo)
{
	HINSTANCE hDll = LoadLibraryA( lpszDll);
	if( !hDll)
	{
		return FALSE;
	}

	typedef bool ( *FUNC_GetSystemAndDriverInfo)( St_SystemAndDriverInfo*);
	FUNC_GetSystemAndDriverInfo pFnInfo;
	bool bRet = false;

	pFnInfo = (FUNC_GetSystemAndDriverInfo)GetProcAddress( hDll, "GetSystemAndDriverInfo");
	if( pFnInfo)
	{
		bRet = pFnInfo( pInfo);
	}

	FreeLibrary( hDll);
	return bRet;
}

#endif // _3DRENDERER_SYSTEMANDDRIVERINFO_H_