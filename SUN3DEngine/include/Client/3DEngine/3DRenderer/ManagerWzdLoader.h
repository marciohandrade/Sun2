// managerwzdloader.h

#ifndef _3DRENDERER_MANAGERWZDLOADER_H_
#define _3DRENDERER_MANAGERWZDLOADER_H_

#include "managerbase.h"
#include <programcommon/wzhashtable.h>

// Wzd ����
struct St_WzdContainer
{
    // Wzd �ε� ����
    enum ENUM_LOAD_STATE
    {
        ELS_NONE = 0,
        ELS_LOADING,
        ELS_LOADED
    };

    CWzdBase* m_pWzd;       // Wzd ������
    char* m_lpszFileName;   // ���ϸ�
    short m_nReference;     // ���� Ƚ��
    DWORD m_dwLoadOption;   // �ε� �ɼ�
    DWORD m_dwDisc;         // Disc ��ȣ
    DWORD m_dwParam1;       // Param1
    DWORD m_dwParam2;       // Param2
    int m_iType;            // WZD Ÿ��(�޽�, �ִϸ��̼� ��)

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
    // ������/�Ҹ���
    CManagerWzdLoader();
    virtual ~CManagerWzdLoader();

    // ����/�ı�
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();

    // ��ġ �ҽ�/������
    virtual void OnLostDevice();
    virtual void OnResetDevice();

    // Wzd �ε�
    virtual HANDLE LoadWzd( const char* lpszFilePath, 
                            int iType, 
                            DWORD dwLoadOption = 0, 
                            float fTextureRate = 1.0f,
                            BOOL bQuick = FALSE,
                            DWORD dwParam1 = 0,
                            DWORD dwParam2 = 0 );

    // ���� �ε�
    virtual BOOL LoadReally( HANDLE hHandle );

    // Wzd �ı�
    virtual void DestroyWzd( HANDLE hHandle );

    // Wzd ���� �ڵ� ã��
    HANDLE FindWzd( const char* lpszFilePath );
    // Wzd ���� ���
    St_WzdContainer* GetWzdInfo( HANDLE hHandle );

    // Wzd ����Ÿ ���
    CWzdBase* GetWzd( HANDLE hHandle );

protected:
    // Wzd ���� �ı�
    void DestroyWzdInfoPtr( St_WzdContainer* pwc );
    // Wzd ���� �ı�
    void DestroyAllWzd();

    // ���� �ε�
    BOOL LoadReally( St_WzdContainer* pwc );

    // �޽� �ؽ�ó �ε�/�ı�
    void LoadMeshTexture( CWzdBase* pWzd );
    void DestroyMeshTexture( CWzdBase* pWzd );

    // ��� Wzd ���ؽ� ���� ����
    void RestoreAllWzdVertexBuffers();

private:
    // ���� �ʱ�ȭ
    void InitVariables();
    // ����
    void ReleaseContents();

protected:
    CWzHashTableStr<HANDLE> m_htsWzd;   // Wzd ���� �ڵ� ���̺�, ���ϸ����� ������ �˻�

    float m_fTextureRate;   // �ؽ�ó ũ�� ����

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
