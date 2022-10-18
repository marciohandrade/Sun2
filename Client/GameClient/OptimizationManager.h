#ifndef	__OPTIMIZATION_MANAGER_H__
#define	__OPTIMIZATION_MANAGER_H__
//------------------------------------------------------------------------------
/**
*/

#pragma once

#include "Narray.h"

class ObjectBase;


typedef struct _ARMOR_PARTS_INFO
{
	eCHAR_TYPE	charType;
    DWORD		dwItemType;
	DWORD		dwCode;
} ARMOR_PARTS_INFO;


typedef struct _ALL_ARMOR_CODE
{
	DWORD dwArmorCode;
	DWORD dwProtectorCode;
	DWORD dwHelmetCode;
	DWORD dwPantsCode;
	DWORD dwBootsCode;
	DWORD dwGloveCode;
	DWORD dwBeltCode;
	DWORD dwShirtsCode;
	DWORD dwWeapon;
	DWORD dwStyle;
} ALL_ARMOR_CODE;


class OptimizationManager
{
public:
	OptimizationManager(void);
	~OptimizationManager(void);

	void		Init();
	void		Destory();
	BOOL		AddPartsInfo(BASE_ITEMINFO *pInfo);
	BOOL		GetAllPartInfo(eCHAR_TYPE charType,ALL_ARMOR_CODE &getInfo);
	BOOL		ChangePerfMode(BOOL bHero);
	void        SetOriginalMode();
	BOOL		Process( DWORD dwTick );					
	
	inline      void SetPerformanceMode(BOOL bFlag){m_bPerformanceMode=bFlag;}
	inline      BOOL IsPerformanceMode(){return m_bPerformanceMode;}
	float       GetTextureRate(ObjectBase *pObj);


protected:
	nArray<ARMOR_PARTS_INFO> m_partsInfoArray;
	BOOL        m_bPerformanceMode;
};

extern OptimizationManager g_OptimizationManager;
#endif 
