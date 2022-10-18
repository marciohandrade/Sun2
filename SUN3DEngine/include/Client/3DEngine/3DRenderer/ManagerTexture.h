// managertexture.h

#ifndef _3DRENDERER_MANAGERTEXTURE_H_
#define _3DRENDERER_MANAGERTEXTURE_H_

#include "managerbase.h"
#include <programcommon/wzhashtable.h>

// 텍스쳐 로드 스타일
#define TLS_NEAREST                 (0x00000001)    // linear 가 아닌 nearest (격자형태) - 디폴트
#define TLS_LINEAR                  (0x00000002)    // linear (smoothing)
#define TLS_REPEAT                  (0x00000004)    // 반복 패턴 - 디폴트
#define TLS_CLAMP                   (0x00000008)    // Clamp 형태
#define TLS_KEEPBUFFER              (0x00000010)    // 사용 안함
#define TLS_NO_MIPMAP               (0x00000020)    // Mipmap 없이 로드
#define TLS_AS_RENDERTARGET         (0x00000040)    // RenderTarget 으로 쓰임
#define TLS_AS_DEPTHSTENCIL         (0x00000080)    // DepthStencil으로 쓰임
#define TLS_R32F                    (0x00000100)    // 32bit floating-point 포맷 for the Red 채널.
#define TLS_AS_DEPTHSTENCILSURFACE  (0x00000200)    // DepthStencilSurface로 쓰임(SetTexture를 할수 없음)
#define TLS_NEED_UPDATE_TFS         (0x00000400)    // SetTexture하기전에 Surface로부터 Update가 필요할 경우
#define TLS_SPECULARMAP             (0x00001000)    // 스페큘러 맵
#define TLS_NORMALMAP               (0x00002000)    // 노말맵 텍스처
#define TLS_ORIGINAL_32BIT          (0x00004000)    // 무조건 이미지 포맷을 유지하도록
#define TLS_ORIGINAL_SIZE           (0x00008000)    // 텍스처를 축소시키지 않도록             
#define TLS_EMPTY_TEXTURE           (0x00010000)    // 파일로 생성하지 않을 경우
#define TLS_HALF_SIZE               (0x00020000)    // 텍스처 크기를 반으로 로드하도록
#define TLS_EXTERNAL                (0x00040000)
#define TLS_NO_MULTITHREAD          (0x00080000)    // 멀티쓰레드로 로딩 안하도록
#define TLS_BUMPMAP                 (0x00100000)    // 범프맵
#define TLS_QUICK_LOAD              (0x00200000)    // 멀티쓰레드시 퀵로드

/// 외부 사용자용 RenderTarget 으로 쓰임
#define TLS_AS_EXTERNAL_RENDERTARGET    (TLS_EXTERNAL | TLS_AS_RENDERTARGET)

// 로딩 후 세팅되는 값
#define TLS_WITHALPHA               (0x01000000)    // 알파 채널이 있다.
#define TLS_WITHALPHA_HOLE          (0x02000000)    // 뚫리는 알파 이미지
#define TLS_WITHZWRITE_DISABLE      (0x04000000)    // 렌더링시 ZWrite를 Disable

#define TLS_UPDATE_TEXTURE_FROM_SURFACE (0x40000000)

// 최대 텍스처 스테이지 수
#define MAX_TEXTURE_STAGE           (8)

// Texture Option
#define TO_SAVE_TO_FULLPATHNAME     (0x00000001)    // LoadTexture()로부터 FullPathName의 이름으로 저장

// Texture Coord Index
#define TSS_TCI_PASSTHRU                    (0x00000000L)
#define TSS_TCI_CAMERASPACENORMAL           (0x00010000L)
#define TSS_TCI_CAMERASPACEPOSITION         (0x00020000L)
#define TSS_TCI_CAMERASPACEREFLECTIONVECTOR (0x00030000L)
#define TSS_TCI_SPHEREMAP                   (0x00040000L) 

// 텍스처 정보
struct St_TextureContainer
{
    // 로딩 상태
    enum ENUM_LOAD_STATE
    {
        ELS_NONE = 0,
        ELS_LOADING,
        ELS_LOADED
    };

    char m_lpszFileName[256];
    void* m_pTexture;
    void* m_pTextureSurface;
    short m_nWidth;         // 텍스처 가로 크기(2^n)
    short m_nHeight;        // 텍스처 세로 크기(2^n)
    short m_nImageWidth;    // 이미지 가로 크기
    short m_nImageHeight;   // 이미지 세로 크기
    short m_nNumComponents;
    short m_nReference;     // 참조 카운트
    DWORD m_dwLoadStyle;
    DWORD m_dwDisc;

    float m_fTextureRate;
    BOOL m_bApplyUnitTextureRate;            

    ENUM_LOAD_STATE m_eLoadState;
};

// 메모리 정보
struct St_DbgTexMemInfo
{
    UINT m_uiInitAvailableTextureMem;
    UINT m_uiAvailableTextureMem;

    UINT m_uiInitAvailableVidMem;
    UINT m_uiAvailableVidMem;

    UINT m_uiLocalVidMem;
};

//------------------------------------------------------------------------------
/**
    @class CManagerTexture
*/
class CManagerTexture : public CManagerBase, 
                        protected CWzHandleManager<St_TextureContainer*>  
{
public:
    enum ETTRANS
    {
        TTRNS_DISABLE = 0,
        TTRNS_COUNT1,
        TTRNS_COUNT2,
        TTRNS_COUNT3,
        TTRNS_COUNT4,
        TTRNS_PROJECTED = 256,
        TTRNS_PROJECTEDORCOUNT4 = 256 | 4,
    };

    enum EBMMTX
    {
        EBMMTX_BUMPENVMAT00 = 0,
        EBMMTX_BUMPENVMAT01,
        EBMMTX_BUMPENVMAT10,
        EBMMTX_BUMPENVMAT11,
    };

    enum ETEXTUREOP
    {
        ETOP_DISABLE = 1,
        ETOP_SELECTARG1 = 2,        // arg1
        ETOP_SELECTARG2 = 3,        // arg2
        ETOP_MODULATE = 4,          // arg1 * arg2
        ETOP_MODULATE2X = 5,        // (arg1 * arg2) << 1
        ETOP_ADD = 7,               // arg1 + arg2
        ETOP_ADDSIGNED = 8,         // arg1 + arg2 - 0.5
        ETOP_ADDSIGNED2X = 9,       // (arg1 + arg2 - 0.5) << 1
        ETOP_SUBTRACT = 10,         // arg1 - arg2
        ETOP_ADDSMOOTH = 11,        // arg1 + arg2 * (1 - arg1)
        ETOP_DEFAULT = 0x7fffffff,
    };

    enum ETEXTUREARG
    {
        ETARG_DIFFUSE = 0,
        ETARG_CURRENT = 1,  
        ETARG_TEXTURE = 2,
        ETARG_TFACTOR = 3,
        ETARG_DEFAULT = 0x7fffffff,
    };

public:
    // 생성자/소멸자
    CManagerTexture();
    virtual ~CManagerTexture();

    // 생성/파괴
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();

    // 장치 소실/복구시
    virtual void OnLostDevice();
    virtual void OnResetDevice();

    // 텍스처 로딩
    virtual HANDLE LoadTexture( const char* lpszFilePath, 
                                DWORD dwLoadStyle = 0, 
                                float fTextureRate = -1.0f ) = 0;

    // 빈 텍스처 생성
    virtual HANDLE GenerateEmptyTexture( int iWidth, 
                                         int iHeight, 
                                         int iNumComponents,
                                         DWORD dwLoadStyle = 0 );

    // 실제 로딩
    virtual BOOL LoadReally( HANDLE hHandle ) = 0;

    // 텍스처 파괴
    virtual void DestroyTexture( HANDLE hHandle );

    // 텍스처 정보 얻기
    St_TextureContainer* GetTextureInfo( HANDLE hHandle );

    // 로딩된 모든 텍스처 리사이징
    virtual BOOL ResizeAllTexture( BOOL bApplyUnitTextureRate = FALSE ) = 0;

    // 텍스처 Lock/Unlock
    virtual void* LockTexture( HANDLE hTexture, long* plPitch, int* piNumComponents ) = 0;
    virtual void UnlockTexture( HANDLE hTexture ) = 0;

    // 텍스처 버퍼 얻기 (xxx: Tool only)
    virtual BOOL GetTextureBuffer( HANDLE hHandle, 
                                   int* piOutBufferSize,
                                   BYTE** ppbyOutBuffer = NULL,
                                   int* piOutWidth = NULL,
                                   int* piOutHeight = NULL ) = 0;

    // 렌더링 타겟 설정/얻기
    virtual BOOL SetRenderTarget( HANDLE hTexture = INVALID_HANDLE_VALUE, 
                                  BOOL bAutoViewportRestore = FALSE,
                                  RECT* prtViewport = NULL ) = 0;

    HANDLE GetRenderTarget( RECT* prtViewport );

    // 깊이/스텐실 버퍼 설정/얻기
    virtual BOOL SetDepthStencilSurface( HANDLE hTexture = INVALID_HANDLE_VALUE ) = 0;
    HANDLE GetDepthStencilSurface();

    // 렌더링 영역 설정
    virtual BOOL SetScissorRect( RECT* prt ) = 0;

    virtual void EnableScissorRect( BOOL bEnable );
    BOOL IsEnableScissorRect() const;

    // 텍스처 설정
    virtual BOOL SetTexture( HANDLE hHandle, int iTextureStage = 0 );

    // 칼라 블랜딩 정보 설정
    virtual void SetColorBlendingOp( UINT uiStage, ETEXTUREOP eTextureOp = ETOP_DEFAULT ) = 0;
    virtual void SetColorArg1( UINT uiStage, ETEXTUREARG eTextureArg = ETARG_DEFAULT ) = 0;
    virtual void SetColorArg2( UINT uiStage, ETEXTUREARG eTextureArg = ETARG_DEFAULT ) = 0;

    // 알파 블랜딩 정보 설정
    virtual void SetAlphaBlendingOp( UINT uiStage, ETEXTUREOP eTextureOp = ETOP_DEFAULT ) = 0;
    virtual void SetAlphaArg1( UINT uiStage, ETEXTUREARG eTextureArg = ETARG_DEFAULT ) = 0;
    virtual void SetAlphaArg2( UINT uiStage, ETEXTUREARG eTextureArg = ETARG_DEFAULT ) = 0;

    // 텍스처 요소 설정
    void SetTextureFactor( WzColor wcColor );
    virtual void SetTFactor( WzColor wcColor ) = 0;

    virtual void EnableTextureFactor( BOOL bEnable, int iTextureStage = 0, BOOL bUsingAlpha = FALSE ) = 0;

    // 텍스처 좌표 인덱스 설정
    virtual void SetTexCoordIndex( DWORD dwValue, int iTextureStage = 0 ) = 0;

    // 텍스처 변환 설정
    virtual BOOL SetTextureTransform( ETTRANS eTexTrans, int iTextureStage = 0 ) = 0;
    virtual BOOL SetTextureTransform( WzMatrix* pTexMat, int iTextureStage = 0 ) = 0;

    // 범프 맵핑 행렬 설정
    virtual BOOL SetBumpMappingMatrix( EBMMTX eBumpMtx, float fValue, int iTextureStage = 0 ) = 0;

    // 텍스처 필터 적용 시작/종료
    virtual void StartTextureFilter( BOOL bColor = TRUE, BOOL bAlpha = TRUE ) = 0;
    virtual void EndTextureFilter() = 0;

    virtual void StartTextureFilterUsingAlpha() = 0;
    virtual void EndTextureFilterUsingAlpha() = 0;

    virtual void StartTextureFilter2X() = 0;
    virtual void EndTextureFilter2X() = 0;

    // 텍스처 필터 적용 여부
    BOOL IsTextureFilterUsed() const;
    BOOL IsAlphaTextureFilterUsed() const;
    BOOL IsTextureFilter2XUsed() const;

    // 밉맵 바이어스 설정/얻기
    virtual void SetMipmapBias( float fMipmapBias );
    float GetMipmapBias() const;

    // 특별 텍스처 추가/얻기
    virtual void AddSpecialTexture( WzID wid, HANDLE hHandle );
    HANDLE GetSpecialTexture( WzID wid );

    // 특별 텍스처 제거
    virtual void RemoveSpecialTexture( WzID wid );
    virtual void ClearSpecialTexture();

    // 전체 경로 적용
    void EnableSaveToFullPathName( BOOL bEnable );

    // 텍스처 옵션 설정/제거/검사
    void SetTextureOption( DWORD dwFlag );
    void UnsetTextureOption( DWORD dwFlag );
    BOOL CheckTextureOption( DWORD dwFlag ) const;

    // 메모리 정보 갱신
    virtual void UpdateMemoryInfo() = 0;

    // 메모리 정보 얻기
    UINT GetUsedTextureMem() const;
    UINT GetAvailableTextureMem() const;

    UINT GetUsedVidMem() const;
    UINT GetAvailableVidMem() const;

    UINT GetLocalVidMem() const;
    
#ifdef _DEBUG
    virtual int GetTextureCount() { return Idx_GetCount(); }
    virtual St_TextureContainer* GetTextureByIndex(int index) { return Idx_GetData(index); }
#endif 

protected:
    // 텍스처 파괴
    virtual void DestroyTextureInfoPtr( St_TextureContainer* ptc );
    // 텍스처 전부 파괴
    void DestroyAllTexture();

    // 텍스처 정보 추가
    HANDLE AddTextureInfo( St_TextureContainer* ptc );

    // 텍스처 정보 핸들 찾기
    HANDLE FindTextureHandle( const char* lpszFilePath );

    // 텍스처 생성
    virtual HANDLE GenerateTexture( const BYTE* pbyBuffer, 
                                    int iWidth, 
                                    int iHeight,
                                    int iNumComponents,
                                    int iImageWidth,
                                    int iImageHeight,
                                    const char* lpszFileName,
                                    DWORD dwLoadStyle = 0 ) = 0;


    // 렌더링 옵션에 따른 텍스처 크기 얻기
    void GetTextureSizeByRenderOption( int iWidth, 
                                       int iHeight, 
                                       BOOL bHalfSize,
                                       float fTextureRate,
                                       int* piOutWidth, 
                                       int* piOutHeight );
    // 텍스처 설정
    virtual BOOL SetTextureInfo( St_TextureContainer* ptc, int iTextureStage = 0 ) = 0;

private:
    // 변수 초기화
    void InitVariables();
    // 해제
    void ReleaseContents();

protected:
    CWzHashTableStr<HANDLE> m_htsTexture;
    CWzHashTable<HANDLE, WzID> m_htSpecialTexture;

    HANDLE m_hLatestTexture[MAX_TEXTURE_STAGE];

    HANDLE m_hRenderTarget;
    HANDLE m_hDepthStencilSurface;

    RECT m_rtRenderTarget;

    BOOL m_bScissorRect;
    RECT m_rtScissorRect;

    float m_fMipmapBias;    // mipmap bias
    WzColor m_wcTextureFactor;

    DWORD m_dwTextureOption;

    BOOL m_bTextureFilterUsed;
    BOOL m_bAlphaTextureFilterUsed;
    BOOL m_bTextureFilter2XUsed;

    St_DbgTexMemInfo m_dbgMemInfo;

    WzDeclare_Sync( Texture );
    WzDeclare_Sync( TextureLoading );
};

//------------------------------------------------------------------------------
/**
*/
inline St_TextureContainer* CManagerTexture::GetTextureInfo( HANDLE hHandle )
{
    WzSync_This( Texture );

    St_TextureContainer* ptc = GetData( hHandle );
    if( ptc && ptc->m_eLoadState == St_TextureContainer::ELS_LOADED )
    {
        return ptc;
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline HANDLE CManagerTexture::FindTextureHandle( const char* lpszFilePath )
{
    WzSync_This( Texture );

    WzAssert( lpszFilePath );

    // 파일명으로 테이블내 검색
    HANDLE hFind = m_htsTexture.GetC( lpszFilePath );

    return ( hFind == 0 ? INVALID_HANDLE_VALUE : hFind );
}

//------------------------------------------------------------------------------
/**
*/
inline HANDLE CManagerTexture::GetRenderTarget( RECT* prtViewport )
{
    if( prtViewport && m_hRenderTarget != INVALID_HANDLE_VALUE )
    {
        *prtViewport = m_rtRenderTarget;
    }

    return m_hRenderTarget;
}

//------------------------------------------------------------------------------
/**
*/
inline HANDLE CManagerTexture::GetDepthStencilSurface()
{
    return m_hDepthStencilSurface;
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerTexture::EnableScissorRect( BOOL bEnable )
{
    m_bScissorRect = bEnable;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerTexture::IsEnableScissorRect() const
{
    return m_bScissorRect; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerTexture::SetTextureFactor( WzColor wcColor )
{
    m_wcTextureFactor = wcColor; 
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerTexture::IsTextureFilterUsed() const 
{
    return m_bTextureFilterUsed; 
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerTexture::IsAlphaTextureFilterUsed() const
{
    return m_bAlphaTextureFilterUsed; 
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerTexture::IsTextureFilter2XUsed() const
{
    return m_bTextureFilter2XUsed; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerTexture::SetMipmapBias( float fMipmapBias )
{
    m_fMipmapBias = fMipmapBias;
}

//------------------------------------------------------------------------------
/**
*/
inline float CManagerTexture::GetMipmapBias() const
{ 
    return m_fMipmapBias; 
}

//------------------------------------------------------------------------------
/**
*/
inline HANDLE CManagerTexture::GetSpecialTexture( WzID wid )
{
    HANDLE hResult = m_htSpecialTexture.Get( wid );

    return ( hResult == 0 ? INVALID_HANDLE_VALUE : hResult );
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerTexture::EnableSaveToFullPathName( BOOL bEnable )
{
    bEnable ? SetTextureOption( TO_SAVE_TO_FULLPATHNAME ) :
              UnsetTextureOption( TO_SAVE_TO_FULLPATHNAME );
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerTexture::SetTextureOption( DWORD dwFlag )
{
    m_dwTextureOption |= dwFlag; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerTexture::UnsetTextureOption( DWORD dwFlag )
{
    m_dwTextureOption &= ~dwFlag; 
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerTexture::CheckTextureOption( DWORD dwFlag ) const
{
    return ( (m_dwTextureOption & dwFlag) == dwFlag ); 
}

//------------------------------------------------------------------------------
/**
*/
inline UINT CManagerTexture::GetUsedTextureMem() const
{
    return ( m_dbgMemInfo.m_uiInitAvailableTextureMem - 
             m_dbgMemInfo.m_uiAvailableTextureMem );
}

//------------------------------------------------------------------------------
/**
*/
inline UINT CManagerTexture::GetAvailableTextureMem() const
{
    return m_dbgMemInfo.m_uiAvailableTextureMem;
}

//------------------------------------------------------------------------------
/**
*/
inline UINT CManagerTexture::GetUsedVidMem() const
{
    return ( m_dbgMemInfo.m_uiInitAvailableVidMem - 
             m_dbgMemInfo.m_uiAvailableVidMem );
}

//------------------------------------------------------------------------------
/**
*/
inline UINT CManagerTexture::GetAvailableVidMem() const
{
    return m_dbgMemInfo.m_uiAvailableVidMem;
}

//------------------------------------------------------------------------------
/**
*/
inline UINT CManagerTexture::GetLocalVidMem() const
{
    return m_dbgMemInfo.m_uiLocalVidMem;
}

#endif // _3DRENDERER_MANAGERTEXTURE_H_
