// managervertexshader.h

#ifndef _3DRENDERER_MANAGERVERTEXSHADER_H_
#define _3DRENDERER_MANAGERVERTEXSHADER_H_

#include "managerbase.h"
#include "managerd3d.h"
#include <fvec.h>

enum ENUM_VERTEX_SHADER_TYPE
{
	VST_NONE = 0,

	// --- Unit 관련
	VST_NLINK_BONETRANSFORM_FIRST,
	VST_BONETRANSFORM_FIRST = VST_NLINK_BONETRANSFORM_FIRST,
	//{ -------------------------------------------------------------
	VST_BONETRANSFORM = VST_BONETRANSFORM_FIRST,		// 유닛
	VST_BONETRANSFORM_2L,	

	VST_BONETRANSFORM_UVNORMAL,			// Normal 을 참조해서 uv 적용
	VST_BONETRANSFORM_UV1DNORMAL,
	VST_BONETRANSFORM_MEMBRANE,			// Membrane
	VST_BONETRANSFORM_UVLIGHT,			// Light 방향 를 참조해서 uv 적용
	VST_BONETRANSFORM_UVCHAOS,			//
	VST_BONETRANSFORM_SPECULAR,			// 스페큘러 적용
	VST_BONETRANSFORM_SPECULAR_2L,
	VST_BONETRANSFORM_PSPECULAR,		// 픽셀당 스페큘러맵 적용
	VST_BONETRANSFORM_PSPECULAR_2L,
	VST_BONETRANSFORM_FOR_NORMALMAP,	// Normal map 사용
	VST_BONETRANSFORM_FOR_NORMALMAP_2L,	// Normal map 사용 ( Light 2 개 )
	VST_BONETRANSFORM_FOR_INORMALMAP,	// Normal map with Irradiance 사용
	VST_BONETRANSFORM_FOR_INORMALMAP_2L,// ( Light 2 개 )
	VST_BONETRANSFORM_PHONG,			// Phong shading 용
	VST_BONETRANSFORM_HAZE,
	VST_BONETRANSFORM_REFRACTION_MASK,
	VST_BONETRANSFORM_INFLATION,		
	VST_BONETRANSFORM_INFLATION_2L,	
	//} -------------------------------------------------------------
	VST_BONETRANSFORM_LAST = VST_BONETRANSFORM_INFLATION_2L,

	// --- Unit 2-Link 관련
	VST_2BONETRANSFORM,
	VST_2BONETRANSFORM_2L,	
	VST_2BONETRANSFORM_UVNORMAL,
	VST_2BONETRANSFORM_UV1DNORMAL,		
	VST_2BONETRANSFORM_MEMBRANE,
	VST_2BONETRANSFORM_UVLIGHT,
	VST_2BONETRANSFORM_UVCHAOS,			//
	VST_2BONETRANSFORM_SPECULAR,		// 스페큘러 적용
	VST_2BONETRANSFORM_SPECULAR_2L,	
	VST_2BONETRANSFORM_PSPECULAR,		// 픽셀당 스페큘러맵 적용
	VST_2BONETRANSFORM_PSPECULAR_2L,
	VST_2BONETRANSFORM_FOR_NORMALMAP,
	VST_2BONETRANSFORM_FOR_NORMALMAP_2L,
	VST_2BONETRANSFORM_FOR_INORMALMAP,	
	VST_2BONETRANSFORM_FOR_INORMALMAP_2L,
	VST_2BONETRANSFORM_PHONG,	// Phong shading 용
	VST_2BONETRANSFORM_HAZE,
	VST_2BONETRANSFORM_REFRACTION_MASK,
	VST_2BONETRANSFORM_INFLATION,		
	VST_2BONETRANSFORM_INFLATION_2L,

	// --- Unit 2-Link 관련
	VST_3BONETRANSFORM,
	VST_3BONETRANSFORM_2L,	
	VST_3BONETRANSFORM_UVNORMAL,
	VST_3BONETRANSFORM_UV1DNORMAL,		
	VST_3BONETRANSFORM_MEMBRANE,
	VST_3BONETRANSFORM_UVLIGHT,
	VST_3BONETRANSFORM_UVCHAOS,			//
	VST_3BONETRANSFORM_SPECULAR,		// 스페큘러 적용
	VST_3BONETRANSFORM_SPECULAR_2L,	
	VST_3BONETRANSFORM_PSPECULAR,		// 픽셀당 스페큘러맵 적용
	VST_3BONETRANSFORM_PSPECULAR_2L,
	VST_3BONETRANSFORM_FOR_NORMALMAP,
	VST_3BONETRANSFORM_FOR_NORMALMAP_2L,
	VST_3BONETRANSFORM_FOR_INORMALMAP,	
	VST_3BONETRANSFORM_FOR_INORMALMAP_2L,
	VST_3BONETRANSFORM_PHONG,	// Phong shading 용
	VST_3BONETRANSFORM_HAZE,
	VST_3BONETRANSFORM_REFRACTION_MASK,
	VST_3BONETRANSFORM_INFLATION,		
	VST_3BONETRANSFORM_INFLATION_2L,	
	VST_NLINK_BONETRANSFORM_LAST = VST_3BONETRANSFORM_INFLATION_2L,

	VST_BONETRANSFORM_LIGHTMAP,
	VST_BONETRANSFORM_LIGHTMAP_2L,

	VST_BONETRANSFORM_REFRACTION_GLASS,					// 굴절용 - 유리
	VST_BONETRANSFORM_REFRACTION_GLASS_2L,
	VST_BONETRANSFORM_REFRACTION_CHAOS,					// 굴절용 - 울렁울렁
	VST_BONETRANSFORM_REFRACTION_INVERSE,				// 굴절용 - 반전
	
	VST_BONETRANSFORM_FOR_NORMALMAP_REFRACTION_GLASS,	// 노말맵 굴절용(NoSunLight) - 유리
	VST_BONETRANSFORM_FOR_INORMALMAP_REFRACTION_GLASS,	
	VST_BONETRANSFORM_FOR_INORMALMAP_REFRACTION_GLASS_2L,	

	// --- 그림자 관련

	VST_SHADOW_MAP_FIRST,
	//{ -------------------------------------------------------------
	VST_BONETRANSFORM_SHADOW_MAP = VST_SHADOW_MAP_FIRST,		
	VST_BONETRANSFORM_SHADOW_MAP_DRAW,	
	VST_BONETRANSFORM_SHADOW_MAP_DRAW_H,
	//{ -------------------------------------------------------------
	VST_SHADOW_MAP_LAST,

	VST_2BONETRANSFORM_SHADOW_MAP = VST_SHADOW_MAP_LAST,		
	VST_2BONETRANSFORM_SHADOW_MAP_DRAW,	
	VST_2BONETRANSFORM_SHADOW_MAP_DRAW_H,	
	VST_3BONETRANSFORM_SHADOW_MAP,		
	VST_3BONETRANSFORM_SHADOW_MAP_DRAW,	
	VST_3BONETRANSFORM_SHADOW_MAP_DRAW_H,	


	VST_OCCLUSION_CULL,

	// --- Instancing 관련
	VST_INSTANCING,

	// --- Effect 관련

	VST_EFFECT_REFRACTION_GLASS,		// 효과(검기) 굴절

	// --- 2D 관련
	VST_2D_PCT,							// 2D Position/Color/Texture
	VST_2D_REFRACTION_PCT,				// 2D 굴절용 Position/Color/Texture

	// --- Terrain 관련
	VST_TERRAIN,		// 지형
	VST_TERRAIN_LIGHTMAP,	// 지형 + 라이트맵
	VST_TERRAIN_SHADOW_MAP_DRAW,
	VST_TERRAIN_SHADOW_MAP_DRAW_PROJECTED,
	VST_TERRAIN_WATER,
	VST_TERRAIN_WATER11,
	VST_TERRAIN_SPECULAR,
	VST_TERRAIN_DETAIL,
	VST_TERRAIN_SPECULAR_DETAIL,
	VST_TERRAIN_HAZE,
	VST_TERRAIN_GLOW,
	VST_TERRAIN_NORMALMAP,

	// --- Post Effect 관련
	// glow
	VST_POSTEFF_GLOW_COPY_FROM_BACKBUFFER,	// Back buffer 에서 얻어옴
	VST_POSTEFF_GLOW_MIXANDMIX,	// 두 버퍼를 통해 섞기
	VST_POSTEFF_GLOW_APPLY_TO_BACKBUFFER,	// 섞은 결과를 Back buffer 에 적용

	// motion blur
	VST_POSTEFF_MOTION_BLUR_COPY_TO_PREVBUFFER, // 이전 프레임버퍼에 현재 프레임 복사
	VST_POSTEFF_MOTION_BLUR_BLENDING,			// 이전프레임과 현재프레임의 블렌딩

	// gray scale
	VST_POSTEFF_GRAYSCALE_FINAL,				// 전 처리 없이 바로 Back buffer 에 적용
	VST_POSTEFF_UNIT_MOTIONBLUR_BLENDING,		// 유닛단위로 이전 프레임과 현재 프레임의 블렌딩
	VST_POSTEFF_DOF,							// vertex shader for DOF
    VST_SUN_MASKING,

	VST_STREE_BRANCH,
	VST_STREE_BRANCH_SHADOW,
	VST_STREE_LEAF,
	VST_STREE_LEAF_SHADOW,
	VST_STREE_BILLBOARD,
	VST_STREE_BRANCH_SHADOW_MAP_DRAW,
#ifdef _NA_007863_20141203_VOLUME_LIGHT_SCATTERING
    VST_POSTEFF_SUNLIGHT,                       // 볼륨라이트 Scattering
#endif // _NA_007863_20141203_VOLUME_LIGHT_SCATTERING
    
    NUM_VST
};

const int NUM_BONETRANSFORM = (VST_BONETRANSFORM_LAST - VST_BONETRANSFORM_FIRST + 1);
const int NUM_NLINK_BONETRANSFORM = (VST_NLINK_BONETRANSFORM_LAST - VST_NLINK_BONETRANSFORM_FIRST + 1);

#define IS_BONETRANSFORM( vst ) \
	( VST_BONETRANSFORM_FIRST <= (vst) && VST_BONETRANSFORM_LAST >= (vst) )

#define IS_NLINK_BONETRANSFORM( vst ) \
	( VST_NLINK_BONETRANSFORM_FIRST <= (vst) && VST_NLINK_BONETRANSFORM_LAST >= (vst) )


typedef struct
{
	LPDIRECT3DVERTEXSHADER9 m_lpVS;	// vertex shader
	LPDIRECT3DVERTEXDECLARATION9 m_lpVD;	// vertex declaration
	LPD3DXCONSTANTTABLE m_lpCT;	// constant table
} St_VertexShaderObject;


/// @name Default Vertex Shader Constants

#define DVSC_CLIPMATRIX			( 0x00000001)
#define DVSC_LIGHT				( 0x00000002)
#define DVSC_COLOR				( 0x00000004)
#define DVSC_CAMERA				( 0x00000008)
#define DVSC_FOG				( 0x00000010)
#define DVSC_OTHERS				( 0x00000020)
#define DVSC_ALL				( 0x000000FF)


class CManagerVertexShader : public CManagerBase, CManagerD3D
{
public:
	CManagerVertexShader(void);
	virtual ~CManagerVertexShader(void);
	virtual void Clear();

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	// 초기화
protected:
	DWORD m_dwVertexShaderVersion;
	BOOL m_bShaderVersion11;
	BOOL m_bShaderVersion20;
	DWORD m_dwNumConstant;
	BOOL m_bUseFog;
	BOOL m_bInstancing;

public:
	virtual BOOL CheckShaderVersion( DWORD dwMajor, DWORD dwMinor);
	//! 빠른체크 - 상수형 인자값에 대한 CheckShaderVersion() 함수 리턴값일 미리 저장해서 사용.
	virtual BOOL CheckShaderVersion11() const { return m_bShaderVersion11; } // 1.1 지원여부
	virtual BOOL CheckShaderVersion20() const { return m_bShaderVersion20; } // 2.0 지원여부

	virtual BOOL CheckBoneTransform( int iNumBone);
	virtual int GetMaxNumBoneToTransform();

	virtual BOOL CheckInstancing(){ return m_bInstancing;} // Instancing 지원 여부

	// Vertex Shader Object 관리
protected:
	St_VertexShaderObject m_vsoList[NUM_VST];
	int m_iVSVersionNumberToUse;	// 사용할 VS 버전 ( 0 - 1.1, 1 - 2.0)

	void CreateVos( char *lpszShaderPath);
	void DestroyVos();
	void CheckVSVersionNumberToUse();
	BOOL CatShaderFileName( char *lpszFilePath, int iIndex, BOOL bHLSL, int iTrial = 0);
	BOOL CreateVSFromAscii( int iIndex, char *lpszFileName, BOOL bHLSL);
	BOOL CreateVSFromBinary( int iIndex, char *lpszFileName, BOOL bHLSL);
public:
	virtual BOOL UseVos( int iVst, void* pParam = NULL)	{ return ( UseVos( ( ENUM_VERTEX_SHADER_TYPE)iVst, pParam)); }
	BOOL CheckVos( int iVst)	{ return ( CheckVos( ( ENUM_VERTEX_SHADER_TYPE)iVst)); }
	virtual BOOL CheckVos( ENUM_VERTEX_SHADER_TYPE eVst);
	void SetBoneMatrixOnConstant( WzMatrix *pwmTransform, int iNumBone, void* pvParam=NULL);
	void SetWorldMatrixOnConstantForInstancing( CWzArray<St_InstancingInfo*>* parInstancingInfo, int iArraySize, int iStartIndex );

	// 설정값
protected:
	WzVector m_wvFactorForUVNormal[2];
	WzUVVert m_wuvShift;
	ENUM_SHADING_TYPE m_eShadingType;
	WzColor m_wcDrawColor;
	WzColor m_wcAmbient;
	float	m_fSpecular[4];
	float	m_fRefractionRate;
	float	m_fInflationFactor;
	float	m_fDetailTextureScale;
	float	m_fTweeningFactor[2];
	float	m_fHazeDist;

public:
	void SetFactorForUVNormal( WzVector *pwvFactorForUVNormal);
	void SetUVShift( WzUVVert *pwuvShift = NULL);
	void SetShadingType( ENUM_SHADING_TYPE eShadingType)	{ m_eShadingType = eShadingType; }
	void SetDiffuseColor( WzColor wcDrawColor)	{ m_wcDrawColor = wcDrawColor; }
	void SetSpecularColor( WzColor wcColor, float fIntensity);
	void SetAmbientColor( WzColor wcColor)		{ m_wcAmbient = wcColor; }
	void SetRefractionRate( float fRefractionRate) { m_fRefractionRate = fRefractionRate; }
	void SetInflationFector( float fInflationFactor) { m_fInflationFactor = fInflationFactor; }
	void SetDetailTextureScale( float fScale ){ m_fDetailTextureScale = fScale; }
	void SetTweeningFactor( int index, float fWeight ) { m_fTweeningFactor[index] = fWeight; }
	void SetHazeDist( float fHazeDist){ m_fHazeDist = fHazeDist; }

public:
	enum { MAX_DEFAULT_CONSTANT = 20 };

	// 표준 Shader 처리
	virtual BOOL UseVos( ENUM_VERTEX_SHADER_TYPE eVst, void* pParam = NULL) { return UseDefaultVos(eVst); }

	// 표준 외 Shader 처리를 위한 함수
	BOOL SetFog( ENUM_VERTEX_SHADER_TYPE eVst); 
	BOOL SetShaderConstants( const float* pConstants, int iStartIndex, int iVec4Count);
	BOOL SetShaderConstantsMatrix( const WzMatrix* pwmConstants, int iStartIndex);
	BOOL SetShaderDeclaration( ENUM_VERTEX_SHADER_TYPE eVst);
	BOOL SetShader( ENUM_VERTEX_SHADER_TYPE eVst);

	const float* FillDefaultConstants( DWORD dwFlags = DVSC_ALL, BOOL bHlsl = TRUE);
	const float* GetDefaultConstants(int iIndex=0) const { return m_fDefaultConstant[iIndex]; }

	enum { MAX_2D_CONSTANTS = 256 };
	int GetNumberOf2DConstants() const { return m_iNum2DConstant; }
	float* Get2DConstants(int iIndex=0) { return &m_f2DConstant[iIndex][0]; }

protected:
	BOOL UseDefaultVos( ENUM_VERTEX_SHADER_TYPE eVst);

protected:

	union
	{
		__m128	m_fDefaultConstantA16[MAX_DEFAULT_CONSTANT];
		float m_fDefaultConstant[MAX_DEFAULT_CONSTANT][4];
	};

	float m_f2DConstant[MAX_2D_CONSTANTS][4];
	int m_iNum2DConstant;

protected:
	WzDeclare_Sync( VertexShader);
};

#endif // _3DRENDERER_MANAGERVERTEXSHADER_H_