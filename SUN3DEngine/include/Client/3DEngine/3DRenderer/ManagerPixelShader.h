// managerpixelshader.h

#ifndef _3DRENDERER_MANAGERPIXELSHADER_H_
#define _3DRENDERER_MANAGERPIXELSHADER_H_

#include "managerbase.h"

enum ENUM_PIXEL_SHADER_TYPE
{
	PST_NONE = 0,
	PST_SPECULAR_SHADING, // Specular map ���
	PST_SPECULAR_SHADING_2L,
	PST_NORMALMAP_SHADING,
	PST_NORMALMAP_SPECULAR_SHADING, // Normal map�� Specular map ���
	PST_NORMALMAP_SHADING_2L,	// 2 Light
	PST_NORMALMAP_SPECULAR_SHADING_2L, // Normal map�� Specular map ��� ( 2 Light)
	// NormalMap with Irradiance
	PST_NORMALMAP_IRRADIANCE_SHADING,				// Normal map�� with Irradiance ���
	PST_NORMALMAP_IRRADIANCE_SHADING_2L,			// ( 2 Light)
	PST_NORMALMAP_SPECULAR_IRRADIANCE_SHADING,		// Normal map�� Specular map with Irradiance ���
	PST_NORMALMAP_SPECULAR_IRRADIANCE_SHADING_2L,	// ( 2 Light)

	PST_PHONG,	// Phong shading

	PST_REFRACTION_GLASS,			// ���� - ����
	PST_REFRACTION_CHAOS,			// ���� - �ﷷ�ﷷ
	PST_REFRACTION_INVERSE,			// ���� - ����
	PST_NORMALMAP_REFRACTION_GLASS,	// �븻�� ����(NoSunLight) - ����
	PST_NORMALMAP_IRRADIANCE_REFRACTION_GLASS,
	PST_NORMALMAP_IRRADIANCE_REFRACTION_GLASS_2L,
	PST_GLOW,						// Glow
	PST_HAZE,						// Haze
	PST_REFRACTION_MASK,			// Haze

	PST_SHADOW_MAP,					// ������ʿ�
	PST_SHADOW_MAP_TEXTURE,			// �ؽ��� ������ʿ�
	PST_SHADOW_MAP_DRAW,			// ������ �׸����
	PST_SHADOW_MAP_TEXTURE_DRAW,	// �ؽ��� ������ �׸����
	PST_SHADOW_MAP_DRAW_H,			// ������ �׸����(�����׽�Ʈ)
	PST_SHADOW_MAP_TEXTURE_DRAW_H,	// �ؽ��� ������ �׸����(�����׽�Ʈ)


	PST_STREE_SHADOW_MAP_DRAW,		//���ǵ� Ʈ����
	PST_STREE_SHADOW_MAP_TEXTURE_DRAW,


	PST_EFFECT_REFRACTION_GLASS,	// �˱� ����

	// --- Terrain ����
	PST_TERRAIN,
	PST_TERRAIN_SHADOW_MAP_DRAW,
	PST_TERRAIN_SHADOW_MAP_TEXTURE_DRAW,
	PST_TERRAIN_WATER_SPECULAR,
	PST_TERRAIN_WATER,
	PST_TERRAIN_SPECULAR,
	PST_TERRAIN_SPECULAR_PERVERTEX,
	PST_TERRAIN_DETAIL,
	PST_TERRAIN_SPECULAR_DETAIL,
	PST_TERRAIN_SPECULAR_DETAIL_PERVERTEX,
	PST_TERRAIN_HAZE,
	PST_TERRAIN_LIGHTMAP,
	PST_TERRAIN_GLOW,
	PST_TERRAIN_NORMALMAP,

	PST_P_EFFECT_BLUR_SAMPLING,
	PST_P_EFFECT_BLUR_SAMPLING_AND_UNITGLOWMAP,
	PST_P_EFFECT_BLUR_STEP,
	PST_P_EFFECT_BLUR_FINAL,

	PST_P_EFFECT_MOTION_BLUR_COPY_TO_PREVBUFFER,		// ���� �����ӹ��ۿ� ���� ������ ����
	PST_P_EFFECT_MOTION_BLUR_BLENDING,					// ���������Ӱ� ������������ ����

	PST_P_EFFECT_GRAYSCALE_FINAL,						// ���� �۾� ���� �ٷ� �׷��� ������ ����
	PST_P_EFFECT_UNIT_MOTIONBLUR_APPLY_TO_BACKBUFFER,	// �����ӹ��ۿ� ��Ǻ����� ��ħ
	PST_P_EFFECT_UNIT_MOTIONBLUR_BLENDING,				// ���� ��Ǻ����ۿ� ���� ��Ǻ����� ����
	PST_P_EFFECT_DOF,									// PixelShader for Depth Of Field Rendering!!
    p_P_EFFECT_SUNLIGHT,								// ��������Ʈ Scattering

	NUM_PST
};


#include "managerd3d.h"

typedef struct
{
	LPDIRECT3DPIXELSHADER9 m_lpPS;	// Pixel Shader
	//LPDIRECT3DVERTEXDECLARATION9 m_lpVD;
	LPD3DXCONSTANTTABLE m_lpCT;	// constant table
} St_PixelShaderObject;



class CManagerPixelShader :	public CManagerBase, CManagerD3D
{
public:
	CManagerPixelShader(void);
	virtual ~CManagerPixelShader(void);
	virtual void Clear();

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	// �ʱ�ȭ
protected:
	DWORD m_dwPixelShaderVersion;
	BOOL m_bShaderVersion11;
	BOOL m_bShaderVersion20;
	BOOL m_bShaderVersion30;
public:
	virtual BOOL CheckShaderVersion( DWORD dwMajor, DWORD dwMinor);
	//! ����üũ - ����� ���ڰ��� ���� CheckShaderVersion() �Լ� ���ϰ��� �̸� �����ؼ� ���.
	virtual BOOL CheckShaderVersion11() const { return m_bShaderVersion11; }
	virtual BOOL CheckShaderVersion20() const { return m_bShaderVersion20; }
	virtual BOOL CheckShaderVersion30() const { return m_bShaderVersion30; }

	// Pixel Shader Object ����
protected:
	St_PixelShaderObject m_psoList[NUM_PST];
	int m_iPSVersionNumberToUse;	// ����� PS ���� ( 0 - 1.1, 1 - 2.0)

	void CreatePos( char *lpszShaderPath);
	void DestroyPos();
	void CheckPSVersionNumberToUse();
	BOOL CatShaderFileName( char *lpszFilePath, int iIndex, BOOL bHLSL, int iTrial = 0);
	BOOL CreatePSFromAscii( int iIndex, char *lpszFileName, BOOL bHLSL);
	BOOL CreatePSFromBinary( int iIndex, char *lpszFileName, BOOL bHLSL);
public:
	virtual BOOL UsePos( int iPst, void* pParam = NULL)	{ return ( UsePos( ( ENUM_PIXEL_SHADER_TYPE)iPst, pParam)); }
	BOOL CheckPos( int iPst)	{ return ( CheckPos( ( ENUM_PIXEL_SHADER_TYPE)iPst)); }
	virtual BOOL CheckPos( ENUM_PIXEL_SHADER_TYPE ePst);
	
public:
	void SetSpecularColor( WzColor wcColor, float fIntensity);
	void SetAmbientColor( WzColor wcColor) { m_wcAmbient = wcColor; }

protected:
	// Unit Color
	float	m_fSpecular[4];
	WzColor m_wcAmbient;

public:
	enum { MAX_DEFAULT_CONSTANT = 8 };
	
	// ǥ�� Shader ó��
	virtual BOOL UsePos( ENUM_PIXEL_SHADER_TYPE ePst, void* pParam = NULL) { return UseDefaultPos(ePst); }

	// ǥ�� �� Shader ó���� ���� �Լ�
	BOOL SetShaderConstants( const float* pConstants, int iStartIndex, int iVec4Count);
	BOOL SetShader( ENUM_PIXEL_SHADER_TYPE ePst);

	const float* FillDefaultConstants();
	const float* GetDefaultConstants(int iIndex=0) const { return m_fDefaultConstant[iIndex]; }

protected:
	virtual BOOL UseDefaultPos( ENUM_PIXEL_SHADER_TYPE ePst);

protected:
	float m_fDefaultConstant[MAX_DEFAULT_CONSTANT][4];


	// ������
protected:
	float m_fRefractionAlpha;
	float m_fRefractionRate;
	float m_fDetailFunc[4];
	float m_fGlowMapFactor;
	float m_fHazeScale;
	float m_fTerrainGlowFactor;

	float	m_fTerrainHeightFactor;				// factor for Terrain Parallaxmap

public:
	void SetRefractionAlpha( float fRefractionAlpha) { m_fRefractionAlpha = fRefractionAlpha; }
	void SetRefractionRate( float fRefractionRate) { m_fRefractionRate = fRefractionRate; }
	void SetDetailFunc( int nIndex, float fFunc) { m_fDetailFunc[nIndex] = fFunc; }
	void SetGlowMapFactor( float fFactor){ m_fGlowMapFactor = fFactor; }
	void SetHazeScale( float fHazeScale){ m_fHazeScale = fHazeScale; }
	void SetTerrainGlowFactor( float fFactor){ m_fTerrainGlowFactor = fFactor; }

	void SetTerrainHeightFactor(float fHeightFactor)	{	m_fTerrainHeightFactor = fHeightFactor;	}
	float GetTerrainHeightFactor()						{	return m_fTerrainHeightFactor;	}
};

#endif // _3DRENDERER_MANAGERPIXELSHADER_H_