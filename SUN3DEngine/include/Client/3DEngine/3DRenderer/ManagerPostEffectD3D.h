// managerposteffectd3d.h

#ifndef _3DRENDERER_MANAGERPOSTEFFETD3D_H_
#define _3DRENDERER_MANAGERPOSTEFFETD3D_H_

#include "managerposteffect.h"
#include "managerd3d.h"


class CManagerPostEffectD3D : public CManagerPostEffect, CManagerD3D
{
public:
	CManagerPostEffectD3D(void);
	virtual ~CManagerPostEffectD3D(void);

	virtual void Clear();

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();

protected:
	virtual void Apply_Before();	// �� �ܰ�
	virtual void Apply_After();	// �� �ܰ�

	// --- Shader ����
protected:
	virtual void SetVS( ENUM_VERTEX_SHADER_TYPE evst, float *pfData = NULL, int iStartIdx = 0, int iVec4Count = 0);
	virtual void SetVSConst( float *pfData, int iStartIdx, int iVec4Count);
	virtual void SetPS( ENUM_PIXEL_SHADER_TYPE ePst, float *pfData = NULL, int iStartIdx = 0, int iVec4Count = 0);
	virtual void SetPSConst( float *pfData, int iStartIdx, int iVec4Count);
	virtual void SetConstant4RenderTexture(float fWidth,float fHeight);

	// --- �߰� �Լ�
protected:
	virtual void BlurStepN( int nthBlur, HANDLE hTarget, HANDLE hSource);
	virtual void Effect_End( HANDLE hFrameBuffer);

	// --- �� ȿ����
protected:
	virtual void Glow_GetDownSampleFromOriginalScene( HANDLE hOriginalScene, HANDLE hGlowBuffer);
	virtual void Glow_End( HANDLE hDestBuffer, HANDLE hFrameBuffer, HANDLE hGlowBuffer);

	virtual void MotionBlur_GetBlurredSampleFromOriginalScene( HANDLE hNewBlur, HANDLE hOldBlur, HANDLE hOriginalScene);

	virtual void GrayScale_ConvertToGrayImage( HANDLE hTo, HANDLE hFrom);

	virtual void RenderDepthOfField( HANDLE hOriginalScene);

	virtual HANDLE UnitMotionBlur_Accumulate( HANDLE hCurrent, HANDLE hPrev, float fBlurRatio );
	virtual void UnitMotionBlur_BlendOriginalAndPrevScene( HANDLE hNewBlur, HANDLE hOldBlur, HANDLE hOriginalScene);
public:
	virtual BOOL BeginGeneratingRefrationMap();	// �����ӹ��� 1/2 ¥�� ���� ����
	virtual HANDLE EndGeneratingRefrationMap();	// �����ӹ��� 1/2 ¥�� ���� ����
    virtual void RenderVolumeLight();	
    void RestoreVolumeLight();
	BOOL IsEnableVoluemeLight();

	virtual void FreeRefrationMap( HANDLE hRefration);
	virtual void RenderWaterMask();

	void SaveTextureForRenderTarget(TCHAR* Path,void* Texture);
};

#endif // _3DRENDERER_MANAGERPOSTEFFETD3D_H_