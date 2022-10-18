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
	int				m_iBlurCount;				//�� Ƚ��
	float			m_fMinLuminance;			//������ �ּ� �� 0.0~1.0
	float			m_fSelectHighLight;			//���õ� ���� HighLight�� 
	WzColor			m_wcSelectColor;			//������ ����
	float			m_fFinalColorBlend;			//���� �������� �󸶸�ŭ ���� �Ҳ���.
	float			m_fOriginalColorBlend;		//�����̹��� ���� ����
};

// ���� ����
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

enum ENUM_PE_STEP	// Post Effect ���� �ܰ�
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

	virtual BOOL Initiate();	// PostEffect ó���� ���� �غ� (���� ȣ���)
	virtual void Apply();		// PostEffect ���� (�ܺ� ȣ�� ����)
protected:
	virtual void Apply_Before();	// �� �ܰ�
	virtual void Apply_After();	// �� �ܰ�
	virtual void ApplyStepByStep();	// ���� PostEffect �� �ܰ躰 ����

	// ���� ����
protected:
	BOOL m_bHardwareSupport;	// Hardware �� �����ϴ��� ����
	BOOL m_bApplied;	// ����Ǿ����� ����
	BOOL m_bFogWasEnabled;	// PostEffect �ܺο��� Fog �� �����־����� ����
	ENUM_PE_STEP m_ePEStep;	// ���� ���� �ܰ�
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

	// ���� �Ҵ�
protected:
	St_PEFrameBufferList m_PEFrameBuffer[NUM_FBS];

	BOOL CreateFrameBufferList();
	void DestroyFrameBufferList();
	HANDLE AssignFreeFrameBuffer( ENUM_FB_SIZE eSize);
	BOOL ReleaseFreeFrameBuffer( ENUM_FB_SIZE eSize, HANDLE hHandle);

	// ���� ���۵� �� ����
protected:
	HANDLE m_hFrameBuffer;	// Frame Buffer
	HANDLE m_hOldMotionBlurBuffer;	// ���� Frame �� ������ Motion Blur ���
	HANDLE m_hCurrentUnitMotionBlurBuffer;	// UnitMotionBlur�϶� MotionBlur�Ǵ� Unit�� �̰��� ����
	HANDLE m_hTempBuffer;					// m_hCurrentUnitMotionBlurBuffer�� ����Ÿ���� �Ǳ� ���� ����Ÿ���� �����ϱ� ���� �ӽ� ����
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

	// --- �� ȿ���� Parameter
	
	// Glow
protected:
	St_PE_BLUR			m_BlurAttribute;

	// DOF Members
	float				m_fFocus;			// Camera ��Ŀ�� �Ÿ�
	float				m_fFocusRange;		// Camera ��Ŀ�� ����
	float				m_fFocusOutRange;	// Camera ��Ŀ�� �ƿ� ����
	float				m_fFocusInRange;	// Camera ��Ŀ�� �� ����
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
	BOOL		m_bFirstFrame;				// ��Ǻ��� ù ����������..
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


	// --- Shader ����
protected:
	virtual void SetVS( ENUM_VERTEX_SHADER_TYPE evst, float *pfData = NULL, int iStartIdx = 0, int iVec4Count = 0)	{}
	virtual void SetVSConst( float *pfData, int iStartIdx, int iVec4Count)	{}
	virtual void SetPS( ENUM_PIXEL_SHADER_TYPE ePst, float *pfData = NULL, int iStartIdx = 0, int iVec4Count = 0)	{}
	virtual void SetPSConst( float *pfData, int iStartIdx, int iVec4Count)	{}
	virtual void SetConstant4RenderTexture(float fWidth,float fHeight)	{}

	// --- �߰� �Լ�
protected:
	virtual void BlurStepN(int nthBlur, HANDLE hTarget, HANDLE hSource)	{}
	virtual void Effect_End( HANDLE hFrameBuffer);

	// --- �� ȿ����
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
	virtual BOOL BeginGeneratingRefrationMap();	// �����ӹ��� 1/2 ¥�� ���� ����
	virtual HANDLE EndGeneratingRefrationMap();	// �����ӹ��� 1/2 ¥�� ���� ����
	virtual void FreeRefrationMap( HANDLE hRefration) {}
	virtual void RenderWaterMask(){}
    virtual void RenderVolumeLight(){};	

	// --- �ܺ� ȣ�� �Լ�
public:
	virtual void ApplyNow();	// Post Effect �� ���� �����ؼ� BackBuffer �� ����Ǿ��ֵ��� ��
	virtual void FreeRestingBuffer();	// ��� �ȵǰ� �ִ� ���� ����
};

#endif // _3DRENDERER_MANAGERPOSTEFFECT_H_