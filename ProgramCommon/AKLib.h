/*-----------------------------------------------------------------------------
 * AKLib.h
 *-----------------------------------------------------------------------------
 * ArmorKit version 2 client ������ ���̺귯��	(�ܺ� ������)
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh Yong Hwan (fixbrain@gmail.com, unsorted@msn.com)
**---------------------------------------------------------------------------*/
#ifndef _AK_LIB_V2_
#define _AK_LIB_V2_


#include "AKLibConfig.h"
#include "AKHackReport.h"

// ������Ʈ ����/Config ����
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
// ArmorKit ���� ����
//-----------------------------------------------------------------------------
#define AKVER_INFO_LENGTH		64
#define	AKVERSION_MJ_HIGH		2
#define AKVERSION_MJ_LOW		0
#define AKVERSION_MN_HIGH		0
#define AKVERSION_MN_LOW		7

//-----------------------------------------------------------------------------
// Armorkit �� Ž���� Hack ���� ������ �����ϱ� ���� �ݹ� �Լ� ����
//-----------------------------------------------------------------------------
typedef int (*ARMOKIT_CALLBACK)(PHACK_DETECT_INFO pHDI);

//-----------------------------------------------------------------------------
// ArmorKit library �ν��Ͻ� ������ ���� factory method
//-----------------------------------------------------------------------------
IAKLib*	CreateIAKLibInstance(void);
void	DeleteIAKLibInstance(IAKLib* created_instance);




//-----------------------------------------------------------------------------
// ArmorKit version 2 ���̺귯�� �������̽� 
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