// managerwzdloader.h

#ifndef _3DRENDERER_MANAGERWZDLOADER_H_
#define _3DRENDERER_MANAGERWZDLOADER_H_

#include "managerbase.h"
#include <programcommon/wzhashtable.h>

// Wzd 정보
struct St_WzdContainer
{
    // Wzd 로딩 상태
    enum ENUM_LOAD_STATE
    {
        ELS_NONE = 0,
        ELS_LOADING,
        ELS_LOADED
    };

    CWzdBase* m_pWzd;       // Wzd 포인터
    char* m_lpszFileName;   // 파일명
    short m_nReference;     // 참조 횟수
    DWORD m_dwLoadOption;   // 로딩 옵션
    DWORD m_dwDisc;         // Disc 번호
    DWORD m_dwParam1;       // Param1
    DWORD m_dwParam2;       // Param2
    int m_iType;            // WZD 타입(메시, 애니메이션 등)

    ENUM_LOAD_STATE m_eLoadState;
};

//------------------------------------------------------------------------------
/**
    @class CManagerWzdLoader
*/
class CManagerWzdLoader : public CManagerBase, 
                          protected CWzHandleManager<St_WzdContainer*>  
{
public:
    // 생성자/소멸자
    CManagerWzdLoader();
    virtual ~CManagerWzdLoader();

    // 생성/파괴
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();

    // 장치 소실/복구시
    virtual void OnLostDevice();
    virtual void OnResetDevice();

    // Wzd 로딩
    virtual HANDLE LoadWzd( const char* lpszFilePath, 
                            int iType, 
                            DWORD dwLoadOption = 0, 
                            float fTextureRate = 1.0f,
                            BOOL bQuick = FALSE,
                            DWORD dwParam1 = 0,
                            DWORD dwParam2 = 0 );

    // 실제 로딩
    virtual BOOL LoadReally( HANDLE hHandle );

    // Wzd 파괴
    virtual void DestroyWzd( HANDLE hHandle );

    // Wzd 정보 핸들 찾기
    HANDLE FindWzd( const char* lpszFilePath );
    // Wzd 정보 얻기
    St_WzdContainer* GetWzdInfo( HANDLE hHandle );

    // Wzd 데이타 얻기
    CWzdBase* GetWzd( HANDLE hHandle );

protected:
    // Wzd 정보 파괴
    void DestroyWzdInfoPtr( St_WzdContainer* pwc );
    // Wzd 전부 파괴
    void DestroyAllWzd();

    // 실제 로딩
    BOOL LoadReally( St_WzdContainer* pwc );

    // 메시 텍스처 로딩/파괴
    void LoadMeshTexture( CWzdBase* pWzd );
    void DestroyMeshTexture( CWzdBase* pWzd );

    // 모든 Wzd 버텍스 버퍼 복구
    void RestoreAllWzdVertexBuffers();

private:
    // 변수 초기화
    void InitVariables();
    // 해제
    void ReleaseContents();

protected:
    CWzHashTableStr<HANDLE> m_htsWzd;   // Wzd 정보 핸들 테이블, 파일명으로 빠르게 검색

    float m_fTextureRate;   // 텍스처 크기 비율

    WzDeclare_Sync( Wzd );
    WzDeclare_Sync( WzdLoading );
};

//------------------------------------------------------------------------------
/**
*/
inline HANDLE CManagerWzdLoader::FindWzd( const char* lpszFilePath )
{
    WzSync_This( Wzd );

    WzAssert( lpszFilePath );

    HANDLE hFind = m_htsWzd.GetC( lpszFilePath );

    return ( hFind == 0 ? INVALID_HANDLE_VALUE : hFind );
}

//------------------------------------------------------------------------------
/**
*/
inline St_WzdContainer* CManagerWzdLoader::GetWzdInfo( HANDLE hHandle )
{
    WzSync_This( Wzd );

    St_WzdContainer* pwc = GetData( hHandle );
    if( pwc && pwc->m_eLoadState == St_WzdContainer::ELS_LOADED )
    {
        return pwc;
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline CWzdBase* CManagerWzdLoader::GetWzd( HANDLE hHandle )
{
    WzSync_This( Wzd );

    St_WzdContainer* pwc = GetData( hHandle );
    if( pwc && pwc->m_pWzd && pwc->m_pWzd->CheckFlag( WZDF_LOADED ) )
    {
        return pwc->m_pWzd;
    }

    return NULL;
}

#endif // _3DRENDERER_MANAGERWZDLOADER_H_
