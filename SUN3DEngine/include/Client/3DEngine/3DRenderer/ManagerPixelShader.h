// managerpixelshader.h

#ifndef _3DRENDERER_MANAGERPIXELSHADER_H_
#define _3DRENDERER_MANAGERPIXELSHADER_H_

#include "managerbase.h"

enum ENUM_PIXEL_SHADER_TYPE
{
	PST_NONE = 0,
	PST_SPECULAR_SHADING, // Specular map 사용
	PST_SPECULAR_SHADING_2L,
	PST_NORMALMAP_SHADING,
	PST_NORMALMAP_SPECULAR_SHADING, // Normal map과 Specular map 사용
	PST_NORMALMAP_SHADING_2L,	// 2 Light
	PST_NORMALMAP_SPECULAR_SHADING_2L, // Normal map과 Specular map 사용 ( 2 Light)
	// NormalMap with Irradiance
	PST_NORMALMAP_IRRADIANCE_SHADING,				// Normal map과 with Irradiance 사용
	PST_NORMALMAP_IRRADIANCE_SHADING_2L,			// ( 2 Light)
	PST_NORMALMAP_SPECULAR_IRRADIANCE_SHADING,		// Normal map과 Specular map with Irradiance 사용
	PST_NORMALMAP_SPECULAR_IRRADIANCE_SHADING_2L,	// ( 2 Light)

	PST_PHONG,	// Phong shading

	PST_REFRACTION_GLASS,			// 굴절 - 유리
	PST_REFRACTION_CHAOS,			// 굴절 - 울렁울렁
	PST_REFRACTION_INVERSE,			// 굴절 - 반전
	PST_NORMALMAP_REFRACTION_GLASS,	// 노말맵 굴절(NoSunLight) - 유리
	PST_NORMALMAP_IRRADIANCE_REFRACTION_GLASS,
	PST_NORMALMAP_IRRADIANCE_REFRACTION_GLASS_2L,
	PST_GLOW,						// Glow
	PST_HAZE,						// Haze
	PST_REFRACTION_MASK,			// Haze

	PST_SHADOW_MAP,					// 쉐더우맵용
	PST_SHADOW_MAP_TEXTURE,			// 텍스쳐 쉐더우맵용
	PST_SHADOW_MAP_DRAW,			// 쉐더우 그리기용
	PST_SHADOW_MAP_TEXTURE_DRAW,	// 텍스쳐 쉐더우 그리기용
	PST_SHADOW_MAP_DRAW_H,			// 쉐더우 그리기용(알파테스트)
	PST_SHADOW_MAP_TEXTURE_DRAW_H,	// 텍스쳐 쉐더우 그리기용(알파테스트)


	PST_STREE_SHADOW_MAP_DRAW,		//스피드 트리용
	PST_STREE_SHADOW_MAP_TEXTURE_DRAW,


	PST_EFFECT_REFRACTION_GLASS,	// 검기 굴절

	// --- Terrain 관련
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

	PST_P_EFFECT_MOTION_BLUR_COPY_TO_PREVBUFFER,		// 이전 프레임버퍼에 현재 프레임 복사
	PST_P_EFFECT_MOTION_BLUR_BLENDING,					// 이전프레임과 현재프레임의 블렌딩

	PST_P_EFFECT_GRAYSCALE_FINAL,						// 이전 작업 없이 바로 그레이 스케일 적용
	PST_P_EFFECT_UNIT_MOTIONBLUR_APPLY_TO_BACKBUFFER,	// 프레임버퍼와 모션블러버퍼 합침
	PST_P_EFFECT_UNIT_MOTIONBLUR_BLENDING,				// 이전 모션블러버퍼와 현재 모션블러버퍼 블렌딩
	PST_P_EFFECT_DOF,									// PixelShader for Depth Of Field Rendering!!
    p_P_EFFECT_SUNLIGHT,								// 볼륨라이트 Scattering

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

	// 초기화
protected:
	DWORD m_dwPixelShaderVersion;
	BOOL m_bShaderVersion11;
	BOOL m_bShaderVersion20;
	BOOL m_bShaderVersion30;
public:
	virtual BOOL CheckShaderVersion( DWORD dwMajor, DWORD dwMinor);
	//! 빠른체크 - 상수형 인자값에 대한 CheckShaderVersion() 함수 리턴값일 미리 저장해서 사용.
	virtual BOOL CheckShaderVersion11() const { return m_bShaderVersion11; }
	virtual BOOL CheckShaderVersion20() const { return m_bShaderVersion20; }
	virtual BOOL CheckShaderVersion30() const { return m_bShaderVersion30; }

	// Pixel Shader Object 관리
protected:
	St_PixelShaderObject m_psoList[NUM_PST];
	int m_iPSVersionNumberToUse;	// 사용할 PS 버전 ( 0 - 1.1, 1 - 2.0)

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
	
	// 표준 Shader 처리
	virtual BOOL UsePos( ENUM_PIXEL_SHADER_TYPE ePst, void* pParam = NULL) { return UseDefaultPos(ePst); }

	// 표준 외 Shader 처리를 위한 함수
	BOOL SetShaderConstants( const float* pConstants, int iStartIndex, int iVec4Count);
	BOOL SetShader( ENUM_PIXEL_SHADER_TYPE ePst);

	const float* FillDefaultConstants();
	const float* GetDefaultConstants(int iIndex=0) const { return m_fDefaultConstant[iIndex]; }

protected:
	virtual BOOL UseDefaultPos( ENUM_PIXEL_SHADER_TYPE ePst);

protected:
	float m_fDefaultConstant[MAX_DEFAULT_CONSTANT][4];


	// 설정값
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