#include "stdafx.h"
#include <math.h>
#include "SinTable.h"


//-------------------------------------------------------------------------------------------------
//	전역 변수 선언
//-------------------------------------------------------------------------------------------------

SinTable g_SinTable;


SinTable::SinTable()
{	
	int i;

	m_pSinTable = new float[NUM_SINTABLE + NUM_SINTABLE / 4+1];
	m_pCosTable = m_pSinTable + NUM_SINTABLE / 4;

	for( i = 0 ; i < ( NUM_SINTABLE + NUM_SINTABLE / 4 + 1) ; ++i )
	{
		m_pSinTable[i] = float( sin( PAI * i * 2 / NUM_SINTABLE ) );
	}
	m_pCosTable[NUM_SINTABLE / 4] = 0;

	float fValue;
	m_pACosTable = new int[NUM_ACOSTABLE+1];		// 0, 1~180 에서 0값때문에 +1을 한다.

	// 0~1일때 Angle 값을 구한다.
	for( i = 0; i <= NUM_ACOSTABLE/2; ++i )
	{
		fValue = (float)i / 90;
		m_pACosTable[i] = (int)( acosf(fValue) * 180 / PAI );
	}
	// 0~(-1)일때 Angle 값을 구한다.
	for( i = NUM_ACOSTABLE/2+1; i <= NUM_ACOSTABLE; ++i )
	{
		fValue = 1 - (float)i / 90;
		m_pACosTable[i] = (int)( acosf(fValue) * 180 / PAI );
	}
}


SinTable::~SinTable()
{
	SAFE_DELETE( m_pSinTable );
	SAFE_DELETE( m_pACosTable );

	m_pCosTable = 0;
}

float SinTable::FSIN( int iAngle )
{
	if( iAngle < 0 )
	{
		DISPMSG( "[SinTable::FSIN] Invalid Angle[%d] \n", iAngle );
		iAngle = -iAngle;
		return ( -m_pSinTable[iAngle] );
	}
	else if( iAngle > 360 )
	{
		DISPMSG( "[SinTable::FSIN] Invalid Angle[%d] \n", iAngle );
		iAngle = iAngle % 360;
	}

	return m_pSinTable[iAngle];
}

float SinTable::FCOS( int iAngle )
{
	if( iAngle < 0 )
	{
		DISPMSG( "[SinTable::FSIN] Invalid Angle[%d] \n", iAngle );
		iAngle = -iAngle;
	}
	else if( iAngle > 360 )
	{
		DISPMSG( "[SinTable::FSIN] Invalid Angle[%d] \n", iAngle );
		iAngle = iAngle % 360;
	}

	return m_pCosTable[iAngle];
}

int SinTable::FACOS( float fValue )
{
	if( fValue > 1 || fValue < -1 )
	{
		DISPMSG( "[SinTable::FACOS] Invalid Value[%f] \n", fValue );
		return 0;
	}

	int iValue = 0;
	if( fValue > 0 )
	{
		iValue = (int)( 90 * fValue );
	}
	else
	{
		iValue = (int)( 90 - 90 * fValue );
	}

	return m_pACosTable[iValue];
}




















