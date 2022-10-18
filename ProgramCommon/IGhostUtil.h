/*-----------------------------------------------------------------------------
 * IGhostUtil.h
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh Yong Hwan (fixbrain@gmail.com, unsorted@msn.com)
**---------------------------------------------------------------------------*/
#ifndef _I_GHOST_UTIL_H_
#define _I_GHOST_UTIL_H_

#include "IGhost.h"

//-----------------------------------------------------
// IGhost 인스턴스 자동 소멸을 위한 자원관리 객체
//-----------------------------------------------------
class RSCM_IGhost		
{
public:
	RSCM_IGhost(IGhost* pGhost);    
	~RSCM_IGhost();

	IGhost* get(void){ return m_ghost; };
private:
	IGhost* m_ghost;
};



#endif//_I_GHOST_UTIL_H_