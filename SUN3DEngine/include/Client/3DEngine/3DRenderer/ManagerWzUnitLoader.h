// managerwzunitloader.h

#ifndef _3DRENDERER_MANAGERWZUNITLOADER_H_
#define _3DRENDERER_MANAGERWZUNITLOADER_H_

#include "managerbase.h"
#include <programcommon/wzhashtable.h>

// �ؽ��� �ε� ��Ÿ��
#define ULS_DEFAULT                 (0x00000000)
#define ULS_HALFSIZE_TEXTURE        (0x00000001)    // �ε�� �ؽ�ó ũ�⸦ ������
#define ULS_INSTANCING              (0x00000002)    // �ν��Ͻ� ����
#define ULS_UNIT_LIGHTMAP           (0x00000004)    // ���� ����Ʈ�� ����
#define ULS_CHECK_VALID_ANIMATION   (0x00000008)    // ��� ����
#define ULS_NO_MULTITHREAD          (0x00000010)    // ��Ƽ ������ �ε� ����

class CWzUnit;

struct St_WzUnitContainer
{
    CWzUnit* m_pUnit;
    short m_nReference; // ���� ī��Ʈ
};

//------------------------------------------------------------------------------
/**
    @class CManagerWzUnitLoader
*/
class CManagerWzUnitLoader : public CManagerBase
{
public:
    // ������/�Ҹ���
    CManagerWzUnitLoader();
    virtual ~CManagerWzUnitLoader();

    // ����/�ı�
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();

    // ���� �ε�
    virtual WzID LoadWzUnit( WzID widType, 
                             const char* lpszFilePath, 
                             BOOL bAutoLoadWzd = TRUE,  // ��� ���� 
                             DWORD dwLoadStyle = ULS_DEFAULT,
                             float fTextureRate = -1.0f );

    // ���� �߰� (�ܺο��� ���� ȣ���ϴ� ���)
    virtual WzID AddWzUnit( CWzUnit* pUnit,
                            BOOL bAutoLoadWzd = TRUE,   // ��� ���� 
                            DWORD dwLoadStyle = ULS_DEFAULT );

    // ���� �ı�
    virtual void DestroyWzUnit( WzID widType );

    // ���� ���
    CWzUnit* GetWzUnit( WzID widType );

    // wzm ��� ����
    void SetFolderToLoadWzm( const char* lpszFolder );

protected:
    // ������ ������ ���� ī���� ����
    BOOL AddRefCountIfExist( WzID widType );

    // ���� ���� �߰�
    WzID AddWzUnitInfo( CWzUnit* pUnit, DWORD dwLoadStyle );

    // ���� �ı�
    void DestroyWzUnitPtr( CWzUnit* pUnit);
    void DestroyAllWzUnit();

    // ���� ����Ÿ �ε�/�ı�
    BOOL LoadWzUnitData( CWzUnit* pUnit, DWORD dwLoadStyle );
    void DestroyWzUnitData( CWzUnit* pUnit );

    // �޽� ����Ÿ �ε�/�ı�
    BOOL LoadMeshData( CWzUnit* pUnit, DWORD dwLoadStyle );
    void DestroyMeshData( CWzUnit* pUnit );

    // ���� ����Ÿ �ε�/�ı�
    BOOL LoadMapData( CWzUnit* pUnit, DWORD dwLoadStyle );
    void DestroyMapData( CWzUnit* pUnit );

    // �ִϸ��̼� ����Ÿ �ε�/�ı�
    BOOL LoadAnimData( CWzUnit* pUnit, DWORD dwLoadStyle );
    void DestroyAnimData( CWzUnit* pUnit );

    // �ؽ�ó �ε� �ɼǿ� �ε� ��Ÿ�� ����
    void ApplyLoadStyleToTexLoadOpt( DWORD& dwTexOpt, DWORD dwLoadStyle );

#ifdef _DEBUG
public:
    virtual long GetMemorySize();
#endif 

private:
    // ���� �ʱ�ȭ
    void InitVariables();
    // ����
    void ReleaseContents();

protected:
    CWzHashTable<St_WzUnitContainer*, WzID> m_htWzUnit;

    char m_szWzdPath[MAX_PATH];
    float m_fTextureRate;
};

//------------------------------------------------------------------------------
/**
*/
inline CWzUnit* CManagerWzUnitLoader::GetWzUnit( WzID widType )
{
    St_WzUnitContainer* pwuc = m_htWzUnit.Get( widType );
    if( pwuc )
    {
        return pwuc->m_pUnit;
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerWzUnitLoader::SetFolderToLoadWzm( const char* lpszFolder )
{
    WzAssert( lpszFolder );

    strcpy( m_szWzdPath, lpszFolder );
}

#endif // _3DRENDERER_MANAGERWZUNITLOADER_H_
