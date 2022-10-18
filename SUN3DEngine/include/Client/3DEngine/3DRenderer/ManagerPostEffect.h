// managerposteffect.h

#ifndef _3DRENDERER_MANAGERPOSTEFFECT_H_
#define _3DRENDERER_MANAGERPOSTEFFECT_H_

#include "managerbase.h"
#include "managervertexshader.h"
#include "managerpixelshader.h"

// Post Effect Type
#define PET_GLOW			( 0x00000001 )
#define PET_MOTION_BLUR		( 0x00000002 )
#define PET_GRAYSCALE		( 0x00000004 )
#define PET_REFRACTION		( 0x00000008 )
#define PET_UNIT_MOTIONBLUR	( 0x00000010 )
#define PET_DEPTH_OF_FIELD	( 0x00000020 )
#define PET_VOLUME_LIGHT	( 0x00000040 )

struct St_PE_BLUR
{
	int				m_iBlurCount;				//블러 횟수
	float			m_fMinLuminance;			//적용할 최소 희도 0.0~1.0
	float			m_fSelectHighLight;			//선택된 색깔 HighLight량 
	WzColor			m_wcSelectColor;			//적용할 색상
	float			m_fFinalColorBlend;			//연산 마직막에 얼마만큼 적용 할껀지.
	float			m_fOriginalColorBlend;		//원본이미지 적용 비율
};

// 버퍼 정보
typedef struct St_PEFrameBufferList
{
	int m_iNumBuffer;
	int m_iCurrent;
	HANDLE *m_phBuffer;
	WORD *m_pwBufferStatus;
} St_PEFrameBufferList;

enum ENUM_FB_SIZE
{
	FBS_FULL = 0,
	FBS_HALF,
	NUM_FBS
};

enum ENUM_PE_STEP	// Post Effect 진행 단계
{
	PS_AFTER_INITIATE = 0,
	PS_AFTER_APPLY_BEFORE,
	PS_TO_MAKE_LAST_RESULT,
	PS_AFTER_APPLY_STEP_BY_STEP,
	PS_AFTER_APPLY_AFTER,
};


class CManagerPostEffect : public CManagerBase
{
public:
	CManagerPostEffect(void);
	virtual ~CManagerPostEffect(void);

	virtual void Clear();

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	virtual BOOL Initiate();	// PostEffect 처리를 위한 준비 (내부 호출됨)
	virtual void Apply();		// PostEffect 적용 (외부 호출 가능)
protected:
	virtual void Apply_Before();	// 전 단계
	virtual void Apply_After();	// 후 단계
	virtual void ApplyStepByStep();	// 여러 PostEffect 를 단계별 적용

	// 내부 상태
protected:
	BOOL m_bHardwareSupport;	// Hardware 가 적용하는지 여부
	BOOL m_bApplied;	// 적용되었는지 여부
	BOOL m_bFogWasEnabled;	// PostEffect 외부에서 Fog 가 켜져있었는지 여부
	ENUM_PE_STEP m_ePEStep;	// 현재 진행 단계
public:
	ENUM_PE_STEP GetPEStep()	{ return ( m_ePEStep); }

	// Post Effect Mode
protected:
	DWORD m_dwPostEffectMode, m_dwPostEffectModeToBe;
public:
	virtual void TurnOnPostEffect( DWORD dwPostEffectMode);
	virtual void TurnOffPostEffect( DWORD dwPostEffectMode);
	virtual BOOL IsPostEffectTurnedOn( DWORD dwPostEffectMode)	{ return ( dwPostEffectMode == ( dwPostEffectMode & m_dwPostEffectMode)); }
	virtual BOOL IsPostEffectGoingToBeTurnedOn( DWORD dwPostEffectMode)	{ return ( dwPostEffectMode == ( dwPostEffectMode & m_dwPostEffectModeToBe)); }
	virtual DWORD GetPostEffectMode()	{ return ( m_dwPostEffectMode); }

	// 버퍼 할당
protected:
	St_PEFrameBufferList m_PEFrameBuffer[NUM_FBS];

	BOOL CreateFrameBufferList();
	void DestroyFrameBufferList();
	HANDLE AssignFreeFrameBuffer( ENUM_FB_SIZE eSize);
	BOOL ReleaseFreeFrameBuffer( ENUM_FB_SIZE eSize, HANDLE hHandle);

	// 내부 버퍼들 및 인자
protected:
	HANDLE m_hFrameBuffer;	// Frame Buffer
	HANDLE m_hOldMotionBlurBuffer;	// 이전 Frame 에 생성된 Motion Blur 결과
	HANDLE m_hCurrentUnitMotionBlurBuffer;	// UnitMotionBlur일때 MotionBlur되는 Unit만 이곳에 렌더
	HANDLE m_hTempBuffer;					// m_hCurrentUnitMotionBlurBuffer가 렌더타겟이 되기 전에 렌더타겟을 저장하기 위한 임시 변수
	float m_fDownFrameEpsilonHeight, m_fDownFrameEpsilonWidth, m_fFrameEpsilonHeight, m_fFrameEpsilonWidth;
	float m_fDownSampleWidth, m_fDownSampleHeight, m_fFrameWidth, m_fFrameHeight;
	long m_lLatestMotionBlurFrame;

	void CheckHardwareSupport();

	HANDLE m_hTempRefractionMap;
	ENUM_CULL_FACE m_eOldCullFace;
	BOOL m_bOldTexture;
	ENUM_MESH_DRAW m_eOldMeshDraw;
	BOOL m_bZWrite;
	DWORD m_dwOldColorMask;
	HANDLE m_hOldRenderTarget;

	// --- 각 효과별 Parameter
	
	// Glow
protected:
	St_PE_BLUR			m_BlurAttribute;

	// DOF Members
	float				m_fFocus;			// Camera 포커싱 거리
	float				m_fFocusRange;		// Camera 포커싱 범위
	float				m_fFocusOutRange;	// Camera 포커싱 아웃 범위
	float				m_fFocusInRange;	// Camera 포커싱 인 범위
public:
	virtual void SetTechBlur(St_PE_BLUR* pnewBlur){m_BlurAttribute=*pnewBlur;}
	virtual St_PE_BLUR GetTechBlur()	{return m_BlurAttribute;}

	void	SetFocus(float fFocus)					{	m_fFocus = fFocus;					}
	float	GetFocus()								{	return m_fFocus;					}
	void	SetFocusRange(float fFocusRange)		{	m_fFocusRange = fFocusRange;		}
	float	GetFocusRange()							{	return m_fFocusRange;				}
	void	SetFocusOutRange(float fFocusOutRange)	{	m_fFocusOutRange = fFocusOutRange;	}
	float	GetFocusOutRange()						{	return m_fFocusOutRange;			}
	void	SetFocusInRange(float fFocusInRange)	{	m_fFocusInRange = fFocusInRange;	}
	float	GetFocusInRange()						{	return m_fFocusInRange;				}

	// Motion Blur
protected:
	float		m_fMotionBlurRatio;

public:
	virtual void SetMotionBlurRatio( float fRatio ){ m_fMotionBlurRatio = max( min( fRatio, 1.0f ), 0.0f ); }
	virtual float GetMotionBlurRatio(){ return m_fMotionBlurRatio; }

	// Gray Scale
protected:
	WzColor m_wcGrayScaleColor;
public:
	virtual void SetGrayScaleColor( WzColor wcColor)	{ m_wcGrayScaleColor = wcColor; }
	virtual WzColor GetGrayScaleColor()	{ return ( m_wcGrayScaleColor); }

	// Unit Motion Blur
protected:
	float		m_fUnitMotionBlurRatio;
	BOOL		m_bUnitMotionBlurCalled;
	St_Camera	m_stPrevCameraInfo;
	BOOL		m_bFirstFrame;				// 모션블러의 첫 프레임인지..
public:
	virtual void SetUnitMotionBlurRatio( float fRatio ){ m_fUnitMotionBlurRatio = max( min( fRatio, 1.0f ), 0.0f ); }
	virtual float GetUnitMotionBlurRatio(){ return m_fUnitMotionBlurRatio; }
	virtual void StartUnitMotionBlur();
	virtual void EndUnitMotionBlur();

	// Unit Glow
protected:
	HANDLE		m_hUnitGlowRT;
public:
	virtual HANDLE GetUnitGlowRT();
	virtual void FreeUnitGlowRT();


	// --- Shader 세팅
protected:
	virtual void SetVS( ENUM_VERTEX_SHADER_TYPE evst, float *pfData = NULL, int iStartIdx = 0, int iVec4Count = 0)	{}
	virtual void SetVSConst( float *pfData, int iStartIdx, int iVec4Count)	{}
	virtual void SetPS( ENUM_PIXEL_SHADER_TYPE ePst, float *pfData = NULL, int iStartIdx = 0, int iVec4Count = 0)	{}
	virtual void SetPSConst( float *pfData, int iStartIdx, int iVec4Count)	{}
	virtual void SetConstant4RenderTexture(float fWidth,float fHeight)	{}

	// --- 중간 함수
protected:
	virtual void BlurStepN(int nthBlur, HANDLE hTarget, HANDLE hSource)	{}
	virtual void Effect_End( HANDLE hFrameBuffer);

	// --- 각 효과별
protected:
	virtual void Glow_GetDownSampleFromOriginalScene( HANDLE hOriginalScene, HANDLE hGlowBuffer);
	virtual HANDLE Glow_MixAndMix( HANDLE hGlowBuffer);
	virtual void Glow_End( HANDLE hDestBuffer, HANDLE hFrameBuffer, HANDLE hGlowBuffer);

	virtual void MotionBlur_GetBlurredSampleFromOriginalScene( HANDLE hNewBlur, HANDLE hOldBlur, HANDLE hOriginalScene);
	virtual HANDLE MotionBlur_UpdatePrevImage( HANDLE hNewBlur, HANDLE hOldBlur);

	virtual void GrayScale_ConvertToGrayImage( HANDLE hTo, HANDLE hFrom);

	virtual void RenderDepthOfField( HANDLE hOriginalScene);

	virtual HANDLE UnitMotionBlur_Accumulate( HANDLE hCurrent, HANDLE hPrev, float fBlurRatio );
	virtual void UnitMotionBlur_BlendOriginalAndPrevScene( HANDLE hNewBlur, HANDLE hOldBlur, HANDLE hOriginalScene);
public:
	virtual BOOL BeginGeneratingRefrationMap();	// 프레임버퍼 1/2 짜리 버퍼 생성
	virtual HANDLE EndGeneratingRefrationMap();	// 프레임버퍼 1/2 짜리 버퍼 생성
	virtual void FreeRefrationMap( HANDLE hRefration) {}
	virtual void RenderWaterMask(){}
    virtual void RenderVolumeLight(){};	

	// --- 외부 호출 함수
public:
	virtual void ApplyNow();	// Post Effect 를 지금 적용해서 BackBuffer 에 적용되어있도록 함
	virtual void FreeRestingBuffer();	// 사용 안되고 있는 버퍼 해제
};

#endif // _3DRENDERER_MANAGERPOSTEFFECT_H_