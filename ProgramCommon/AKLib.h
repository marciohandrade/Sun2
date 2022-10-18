/*-----------------------------------------------------------------------------
 * AKLib.h
 *-----------------------------------------------------------------------------
 * ArmorKit version 2 client 배포용 라이브러리	(외부 배포용)
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh Yong Hwan (fixbrain@gmail.com, unsorted@msn.com)
**---------------------------------------------------------------------------*/
#ifndef _AK_LIB_V2_
#define _AK_LIB_V2_


#include "AKLibConfig.h"
#include "AKHackReport.h"

// 업데이트 서버/Config 파일
#ifdef	AK_FOR_DEV
#include "servers_dev.inc"
#elif	defined( AK_FOR_HUXLEY )
#include "servers_huxley.inc"
#elif	defined( AK_FOR_SUN )
#include "servers_sun.inc"
#elif	defined( AK_FOR_PARFAIT )
#include "servers_parfait.inc"
#endif


class IAKLib;
class RSCM_IAKLib;

//-----------------------------------------------------------------------------
// ArmorKit 버전 정보
//-----------------------------------------------------------------------------
#define AKVER_INFO_LENGTH		64
#define	AKVERSION_MJ_HIGH		2
#define AKVERSION_MJ_LOW		0
#define AKVERSION_MN_HIGH		0
#define AKVERSION_MN_LOW		7

//-----------------------------------------------------------------------------
// Armorkit 이 탐지한 Hack 관련 정보를 통지하기 위한 콜백 함수 원형
//-----------------------------------------------------------------------------
typedef int (*ARMOKIT_CALLBACK)(PHACK_DETECT_INFO pHDI);

//-----------------------------------------------------------------------------
// ArmorKit library 인스턴스 생성을 위한 factory method
//-----------------------------------------------------------------------------
IAKLib*	CreateIAKLibInstance(void);
void	DeleteIAKLibInstance(IAKLib* created_instance);




//-----------------------------------------------------------------------------
// ArmorKit version 2 라이브러리 인터페이스 
//-----------------------------------------------------------------------------
class IAKLib
{
public:
	IAKLib();
    virtual ~IAKLib();
		
	// ready for ArmorKit game security service 
	virtual int preStart(DWORD svc_flag, ARMOKIT_CALLBACK CallBack, TCHAR *Version, DWORD svc_ip) = 0;

	// start ArmorKit game security service 	
	virtual int Start(void) = 0;

	// stop ArmorKit game security service
	virtual int Stop(void) = 0;

	//----------------------------------------------------------------------------
	// SUN only
	//----------------------------------------------------------------------------
#ifdef AK_FOR_SUN
	virtual int SUN_GenerateSID(int seed, char *sid, int length) = 0;
	virtual int SUN_SetFileHashList(TCHAR *pFileHashList, DWORD dwListSize) = 0;
#endif//AK_FOR_SUN

};




//-----------------------------------------------------------------------------
// ResSourCe Manager for IAKLib
//-----------------------------------------------------------------------------
class RSCM_IAKLib
{
public:
	RSCM_IAKLib();
    ~RSCM_IAKLib();

	IAKLib* get() {return m_instance;}
private:
	IAKLib* m_instance;
};


#endif // _AK_LIB_V2_