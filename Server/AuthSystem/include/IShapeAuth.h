/*-----------------------------------------------------------------------------
 * IShapeAuth.h
 *-----------------------------------------------------------------------------
 * 외부 배포용 인터페이스 파일
 *-----------------------------------------------------------------------------
 * All rights reserved by Rho Yong Hwan (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * Revision History:
 * Date					Who					What
 * ----------------		----------------	----------------
 * 12.09.2007			Rho Yong Hwan		birth
**---------------------------------------------------------------------------*/
#ifndef _IShapeAuth_h_
#define _IShapeAuth_h_

#include "SATypeDef.h"

//
//  IShapeAuth interface class
//
class IShapeAuth
{
public:
	IShapeAuth(){};
	virtual ~IShapeAuth(){};
	
	virtual int InitSA(void) = 0;
	virtual int TerminateSA(void) = 0;
	virtual int ReloadSA(LPCTSTR lpCfgPathName) = 0;

	virtual int GetSAData(PSA_AUTH_DATA pAuthData) = 0;
	virtual int FreeSAData(PSA_AUTH_DATA pAuthData) = 0;
};

//
// factory method
//
IShapeAuth* CreateSA(void);
void DeleteSA(IShapeAuth *created_instance);

//
// Resource Manager for Shape Auth
//
class RMSA
{
public:
	RMSA();
    ~RMSA()
	{
		DeleteSA(m_instance);
	}

	IShapeAuth* get() {return m_instance;}
private:
	IShapeAuth* m_instance;
};


#endif//_IShapeAuth_h_