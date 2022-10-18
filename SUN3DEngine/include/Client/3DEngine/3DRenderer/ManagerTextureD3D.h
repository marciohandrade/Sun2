// managertextured3d.h

#ifndef _3DRENDERER_MANAGERTEXTURED3D_H_
#define _3DRENDERER_MANAGERTEXTURED3D_H_

// ���� �޸� ���� �������
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
    // ������/�Ҹ���
    CManagerTextureD3D();
    virtual ~CManagerTextureD3D();

    // ����/�ı�
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();

    // ��ġ �ս�/������
    virtual void OnLostDevice();
    virtual void OnResetDevice();

    // �ؽ�ó �ε�
    virtual HANDLE LoadTexture( const char* lpszFilePath, 
                                DWORD dwLoadStyle = 0, 
                                float fTextureRate = -1.0f );

    // ���� �ε�
    virtual BOOL LoadReally( HANDLE hHandle );

    // �ε��� ��� �ؽ�ó ������¡
    virtual BOOL ResizeAllTexture( BOOL bApplyUnitTextureRate = FALSE );

    // �ؽ�ó Lock/Unlock
    virtual void* LockTexture( HANDLE hTexture, long* plPitch, int* piNumComponents );
    virtual void UnlockTexture( HANDLE hTexture );

    // �ؽ�ó ���� ��� (xxx: Tool only)
    virtual BOOL GetTextureBuffer( HANDLE hHandle, 
                                   int* piOutBufferSize,
                                   BYTE** ppbyOutBuffer = NULL,
                                   int* piOutWidth = NULL,
                                   int* piOutHeight = NULL );

    // ������ Ÿ�� ����
    virtual BOOL SetRenderTarget( HANDLE hTexture = INVALID_HANDLE_VALUE,
                                  BOOL bAutoViewportRestore = FALSE,
                                  RECT* prtViewport = NULL );

    // ����/���Ľ� ���� ����
    virtual BOOL SetDepthStencilSurface( HANDLE hTexture = INVALID_HANDLE_VALUE );

    // ������ ���� ����
    virtual BOOL SetScissorRect( RECT* prt );
    virtual void EnableScissorRect( BOOL bEnable );

    // Į�� ���� ���� ����
    virtual void SetColorBlendingOp( UINT uiStage, ETEXTUREOP eTextureOp = ETOP_DEFAULT );
    virtual void SetColorArg1( UINT uiStage, ETEXTUREARG eTextureArg = ETARG_DEFAULT );
    virtual void SetColorArg2( UINT uiStage, ETEXTUREARG eTextureArg = ETARG_DEFAULT );

    // ���� ���� ���� ����
    virtual void SetAlphaBlendingOp( UINT uiStage, ETEXTUREOP eTextureOp = ETOP_DEFAULT );
    virtual void SetAlphaArg1( UINT uiStage, ETEXTUREARG eTextureArg = ETARG_DEFAULT );
    virtual void SetAlphaArg2( UINT uiStage, ETEXTUREARG eTextureArg = ETARG_DEFAULT );

    // �ؽ�ó ��� ����
    virtual void SetTFactor( WzColor wcColor );
    virtual void EnableTextureFactor( BOOL bEnable, int iTextureStage = 0, BOOL bUsingAlpha = FALSE );

    // �ؽ�ó ��ǥ �ε��� ����
    virtual void SetTexCoordIndex( DWORD dwValue, int iTextureStage = 0 );

    // �ؽ�ó ��ȯ ����
    virtual BOOL SetTextureTransform( ETTRANS eTexTrans, int iTextureStage = 0 );
    virtual BOOL SetTextureTransform( WzMatrix* pTexMat, int iTextureStage = 0 );

    // ���� ���� ��� ����
    virtual BOOL SetBumpMappingMatrix( EBMMTX eBumpMtx, float fValue, int iTextureStage = 0 );

    // �ؽ�ó ���� ���� ����/����
    virtual void StartTextureFilter( BOOL bColor = TRUE, BOOL bAlpha = TRUE );
    virtual void EndTextureFilter();

    virtual void StartTextureFilterUsingAlpha();
    virtual void EndTextureFilterUsingAlpha();

    virtual void StartTextureFilter2X();   
    virtual void EndTextureFilter2X();

    // �Ӹ� ���̾ ����
    virtual void SetMipmapBias( float fMipmapBias );

    // �޸� ���� ����
    virtual void UpdateMemoryInfo();

protected:
    // �ؽ�ó �ı�
    virtual void DestroyTextureInfoPtr( St_TextureContainer* ptc );

    // ���� ��� ����/�˻�
    BOOL ConvertFilePathAndCheck( const char* lpszFilePath, char* lpszConvPath );

    // �ش� ������ �����̳ʿ� �߰�
    HANDLE New_AddTexture( const char* lpszFilePath, 
                           DWORD dwLoadStyle,  
                           BOOL& bAlreadyExist,
                           float fTextureRate = -1.0f );

    // ���� �ؽ�ó ����
    BOOL New_LoadTexture( St_TextureContainer* ptc );

    // �̹��� ���۷κ��� �ؽ�ó ����
    BOOL New_CreateTexture( St_TextureContainer* ptc,
                            const BYTE* pbyBuffer,
                            int iBufferSize, 
                            int iWidth,
                            int iHeight,
                            int iNumComponents,
                            int iImageWidth,
                            int iImageHeight,
                            PixFormat pixFormat = PF_UNKNOWN );

    // �̹��� ���۷κ��� �ؽ�ó ����
    BOOL New_CreateTextureMT( St_TextureContainer* ptc,
                              const BYTE* pbyBuffer,
                              int iBufferSize, 
                              int iWidth,
                              int iHeight,
                              int iNumComponents,
                              int iImageWidth,
                              int iImageHeight,
                              PixFormat pixFormat = PF_UNKNOWN );

    // �ؽ�ó ����
    virtual HANDLE GenerateTexture( const BYTE* pbyBuffer, 
                                    int iWidth, 
                                    int iHeight, 
                                    int iNumComponents, 
                                    int iImageWidth, 
                                    int iImageHeight, 
                                    const char* lpszFileName, 
                                    DWORD dwLoadStyle = 0 );

    // ����Ÿ ����
    BOOL ConvertImageBlock( BYTE* pbyDstBuffer,
                            D3DFORMAT d3dfmtDst,
                            int iDstLPitch,
                            const BYTE* pbySrcBuffer,
                            D3DFORMAT d3dfmtSrc,
                            int iSrcLPitchWidth,
                            int iSrcWidth,
                            int iSrcHeight );

    BOOL ConvertBumpMap( St_TextureContainer* ptc );

    // ���ø� ��� �ʱ�ȭ
    void InitSamplerState();

    // �ؽ�ó ����
    virtual BOOL SetTextureInfo( St_TextureContainer* ptc, int iTextureStage = 0 );

    // D3D Format ���
    D3DFORMAT GetD3DFormatToTry( int iNumComponents );
    // D3D Format ������Ʈ �� ���
    int GetNumComponentOfD3DFormat( D3DFORMAT d3dfmt );

    // �޸� ���� �ʱ�ȭ
    void InitMemoryInfo();

private:
    // ���� �ʱ�ȭ
    void InitVariables();
    // ����
    void ReleaseContents();

protected:
    IDirect3DSurface9* m_pOldRenderTarget;
    IDirect3DSurface9* m_pOldDepthStencilSurface;

    DWORD m_dwMapping[MAX_TEXTURE_STAGE];   // �����ϴ� ��� (���� ����� ���� ���������� ���� ����)
    BOOL m_bAutoGenMipmap;

    LPDIRECTDRAW7 m_lpDD;                   // ���� �޸� ���� ���
};

#endif // _3DRENDERER_MANAGERTEXTURED3D_H_
