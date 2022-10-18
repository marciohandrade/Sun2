#pragma once


#define		PAI						3.1415926535897932384f
#define		ANGLE_PER_RADIAN		57.29578f
#define		NUM_SINTABLE			360
#define		NUM_ACOSTABLE			180


class SinTable
{
public:
	SinTable();
	~SinTable();

	float		FSIN( int iAngle );			// iAngle : 0 ~ 360
	float		FCOS( int iAngle );
	int			FACOS( float fValue );		// fValue : -1 ~ 1

private:
	float *		m_pSinTable;
	float *		m_pCosTable;
	int *		m_pACosTable;
};

extern SinTable g_SinTable;

