// managertextured3d.h

#ifndef _3DRENDERER_MANAGERTEXTURED3D_H_
#define _3DRENDERER_MANAGERTEXTURED3D_H_

// 비디오 메모리 정보 얻기위함
#include <ddraw.h>

#include "managertexture.h"
#include "managerd3d.h"
#include <client/3dengine/common/wzimagetype.h>

//------------------------------------------------------------------------------
/**
    @class CManagerTextureD3D
*/
class CManagerTextureD3D : public CManagerTexture, 
                           public CManagerD3D  
{
public:
    // 생성자/소멸자
    CManagerTextureD3D();
    virtual ~CManagerTextureD3D();

    // 생성/파괴
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();

    // 장치 손실/복구시
    virtual void OnLostDevice();
    virtual void OnResetDevice();

    // 텍스처 로딩
    virtual HANDLE LoadTexture( const char* lpszFilePath, 
                                DWORD dwLoadStyle = 0, 
                                float fTextureRate = -1.0f );

    // 실제 로딩
    virtual BOOL LoadReally( HANDLE hHandle );

    // 로딩된 모든 텍스처 리사이징
    virtual BOOL ResizeAllTexture( BOOL bApplyUnitTextureRate = FALSE );

    // 텍스처 Lock/Unlock
    virtual void* LockTexture( HANDLE hTexture, long* plPitch, int* piNumComponents );
    virtual void UnlockTexture( HANDLE hTexture );

    // 텍스처 버퍼 얻기 (xxx: Tool only)
    virtual BOOL GetTextureBuffer( HANDLE hHandle, 
                                   int* piOutBufferSize,
                                   BYTE** ppbyOutBuffer = NULL,
                                   int* piOutWidth = NULL,
                                   int* piOutHeight = NULL );

    // 렌더링 타겟 설정
    virtual BOOL SetRenderTarget( HANDLE hTexture = INVALID_HANDLE_VALUE,
                                  BOOL bAutoViewportRestore = FALSE,
                                  RECT* prtViewport = NULL );

    // 깊이/스탠실 버퍼 설정
    virtual BOOL SetDepthStencilSurface( HANDLE hTexture = INVALID_HANDLE_VALUE );

    // 렌더링 영역 설정
    virtual BOOL SetScissorRect( RECT* prt );
    virtual void EnableScissorRect( BOOL bEnable );

    // 칼라 블랜딩 정보 설정
    virtual void SetColorBlendingOp( UINT uiStage, ETEXTUREOP eTextureOp = ETOP_DEFAULT );
    virtual void SetColorArg1( UINT uiStage, ETEXTUREARG eTextureArg = ETARG_DEFAULT );
    virtual void SetColorArg2( UINT uiStage, ETEXTUREARG eTextureArg = ETARG_DEFAULT );

    // 알파 블랜딩 정보 설정
    virtual void SetAlphaBlendingOp( UINT uiStage, ETEXTUREOP eTextureOp = ETOP_DEFAULT );
    virtual void SetAlphaArg1( UINT uiStage, ETEXTUREARG eTextureArg = ETARG_DEFAULT );
    virtual void SetAlphaArg2( UINT uiStage, ETEXTUREARG eTextureArg = ETARG_DEFAULT );

    // 텍스처 요소 설정
    virtual void SetTFactor( WzColor wcColor );
    virtual void EnableTextureFactor( BOOL bEnable, int iTextureStage = 0, BOOL bUsingAlpha = FALSE );

    // 텍스처 좌표 인덱스 설정
    virtual void SetTexCoordIndex( DWORD dwValue, int iTextureStage = 0 );

    // 텍스처 변환 설정
    virtual BOOL SetTextureTransform( ETTRANS eTexTrans, int iTextureStage = 0 );
    virtual BOOL SetTextureTransform( WzMatrix* pTexMat, int iTextureStage = 0 );

    // 범프 맵핑 행렬 설정
    virtual BOOL SetBumpMappingMatrix( EBMMTX eBumpMtx, float fValue, int iTextureStage = 0 );

    // 텍스처 필터 적용 시작/종료
    virtual void StartTextureFilter( BOOL bColor = TRUE, BOOL bAlpha = TRUE );
    virtual void EndTextureFilter();

    virtual void StartTextureFilterUsingAlpha();
    virtual void EndTextureFilterUsingAlpha();

    virtual void StartTextureFilter2X();   
    virtual void EndTextureFilter2X();

    // 밉맵 바이어스 설정
    virtual void SetMipmapBias( float fMipmapBias );

    // 메모리 정보 갱신
    virtual void UpdateMemoryInfo();

protected:
    // 텍스처 파괴
    virtual void DestroyTextureInfoPtr( St_TextureContainer* ptc );

    // 파일 경로 수정/검사
    BOOL ConvertFilePathAndCheck( const char* lpszFilePath, char* lpszConvPath );

    // 해당 정보를 컨테이너에 추가
    HANDLE New_AddTexture( const char* lpszFilePath, 
                           DWORD dwLoadStyle,  
                           BOOL& bAlreadyExist,
                           float fTextureRate = -1.0f );

    // 실제 텍스처 생성
    BOOL New_LoadTexture( St_TextureContainer* ptc );

    // 이미지 버퍼로부터 텍스처 생성
    BOOL New_CreateTexture( St_TextureContainer* ptc,
                            const BYTE* pbyBuffer,
                            int iBufferSize, 
                            int iWidth,
                            int iHeight,
                            int iNumComponents,
                            int iImageWidth,
                            int iImageHeight,
                            PixFormat pixFormat = PF_UNKNOWN );

    // 이미지 버퍼로부터 텍스처 생성
    BOOL New_CreateTextureMT( St_TextureContainer* ptc,
                              const BYTE* pbyBuffer,
                              int iBufferSize, 
                              int iWidth,
                              int iHeight,
                              int iNumComponents,
                              int iImageWidth,
                              int iImageHeight,
                              PixFormat pixFormat = PF_UNKNOWN );

    // 텍스처 생성
    virtual HANDLE GenerateTexture( const BYTE* pbyBuffer, 
                                    int iWidth, 
                                    int iHeight, 
                                    int iNumComponents, 
                                    int iImageWidth, 
                                    int iImageHeight, 
                                    const char* lpszFileName, 
                                    DWORD dwLoadStyle = 0 );

    // 데이타 복사
    BOOL ConvertImageBlock( BYTE* pbyDstBuffer,
                            D3DFORMAT d3dfmtDst,
                            int iDstLPitch,
                            const BYTE* pbySrcBuffer,
                            D3DFORMAT d3dfmtSrc,
                            int iSrcLPitchWidth,
                            int iSrcWidth,
                            int iSrcHeight );

    BOOL ConvertBumpMap( St_TextureContainer* ptc );

    // 샘플링 방식 초기화
    void InitSamplerState();

    // 텍스처 설정
    virtual BOOL SetTextureInfo( St_TextureContainer* ptc, int iTextureStage = 0 );

    // D3D Format 얻기
    D3DFORMAT GetD3DFormatToTry( int iNumComponents );
    // D3D Format 컴포넌트 수 얻기
    int GetNumComponentOfD3DFormat( D3DFORMAT d3dfmt );

    // 메모리 정보 초기화
    void InitMemoryInfo();

private:
    // 변수 초기화
    void InitVariables();
    // 해제
    void ReleaseContents();

protected:
    IDirect3DSurface9* m_pOldRenderTarget;
    IDirect3DSurface9* m_pOldDepthStencilSurface;

    DWORD m_dwMapping[MAX_TEXTURE_STAGE];   // 맵핑하는 방식 (현재 적용된 것이 무엇인지에 대한 정보)
    BOOL m_bAutoGenMipmap;

    LPDIRECTDRAW7 m_lpDD;                   // 비디오 메모리 정보 얻기
};

#endif // _3DRENDERER_MANAGERTEXTURED3D_H_
