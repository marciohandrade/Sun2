// managermultiloader.h

#ifndef _3DRENDERER_MANAGERMULTILOADER_H_
#define _3DRENDERER_MANAGERMULTILOADER_H_

#include "managerbase.h"
#include <programcommon/wzqueue.h>
#include <programcommon/wzarray.h>

class CThreadObject;

// ��Ƽ������ �ε� ��Ÿ��
#define MLS_TEXTURE         (0x00000001)
#define MLS_WZD             (0x00000002)
#define MLS_DEFAULT         (MLS_TEXTURE | MLS_WZD)    // ���� ���    

// �ε� ��ü Ÿ��
enum ENUM_LOADOBJTYPE
{
    LOT_TEXTURE = 0,
    LOT_WZD
};

// �ε� ť Ÿ��
enum ENUM_LOAD_QUEUE_TYPE
{
    QUEUE_TYPE_QUICK = 0,       
    QUEUE_TYPE_NORMAL,
    NUM_QUEUE_TYPE
};

// �ε� ��ü
struct St_LoadObject
{
    ENUM_LOADOBJTYPE m_eLoadObjType;
    DWORD m_dwObject;
};

// Loader function
void ThreadFunc_Loader( void* pParam );

//------------------------------------------------------------------------------
/**
    @class CManagerMultiLoader
*/
class CManagerMultiLoader : public CManagerBase
{
public:
    // ������/�Ҹ���
    CManagerMultiLoader();
    virtual ~CManagerMultiLoader();

    // ����/�ı�
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();
    
    // ť�� ��ü �߰�
    virtual void AddObjectToLoad( ENUM_LOADOBJTYPE eLoadObjType, 
                                  DWORD dwObject,
                                  ENUM_LOAD_QUEUE_TYPE eyTpe = QUEUE_TYPE_NORMAL );

    // �ε�
    virtual void New_Load();

    // ��Ƽ ������ �ε� �ѱ�/����
    void TurnOn_MultiThreadedLoading( BOOL bOn );

    // ��Ƽ ������ �ε� ����
    BOOL IsTurnedOn_MultiThreadedLoading( DWORD dwStyle ) const;

    // �ε� ��Ÿ�� ����/���
    void SetMultiThreadedLoadingStyle( DWORD dwStyle );
    DWORD GetMultiThreadedLoadingStyle() const;

    // �ε��� ��ü �� ���
    DWORD GetNumberOfLoadObject( ENUM_LOAD_QUEUE_TYPE eType );

    // �ʴ� �ε� �� ���� (xxx: ��� ����)
    void SetLoadCountPerSecond( int LoadPerSecond );
    
    // ������ ���� (xxx: ��� ����)
    void ExitMultiThreadedLoading();

protected:
    // ������ ��ü ����/�ı�
    BOOL CreateThreadObject();
    void DestroyThreadObject();

private:
    // ���� �ʱ�ȭ
    void InitVariables();
    // ����
    void ReleaseContents();

protected:
    CWzQueue<St_LoadObject> m_loadQ[NUM_QUEUE_TYPE];
    
    CThreadObject* m_pLoaderThread;

    DWORD m_dwLoadingStyle;
    BOOL m_bOn;

    WzDeclare_Sync( LoadObject );
};

//------------------------------------------------------------------------------
/**
*/
inline void CManagerMultiLoader::TurnOn_MultiThreadedLoading( BOOL bOn )
{
    m_bOn = bOn;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerMultiLoader::IsTurnedOn_MultiThreadedLoading( DWORD dwStyle ) const
{
    return( m_bOn && ((m_dwLoadingStyle & dwStyle) == dwStyle) );
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerMultiLoader::SetMultiThreadedLoadingStyle( DWORD dwStyle )
{
    m_dwLoadingStyle = dwStyle;
}

//------------------------------------------------------------------------------
/**
*/
inline DWORD CManagerMultiLoader::GetMultiThreadedLoadingStyle() const
{
    return m_dwLoadingStyle;
}

//------------------------------------------------------------------------------
/**
*/
inline DWORD CManagerMultiLoader::GetNumberOfLoadObject( ENUM_LOAD_QUEUE_TYPE eType )
{
    WzSync_This( LoadObject );

    WzAssert( eType >= 0 && eType < NUM_QUEUE_TYPE );

    return m_loadQ[eType].GetCount();
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerMultiLoader::SetLoadCountPerSecond( int )
{
    // ��� ����
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerMultiLoader::ExitMultiThreadedLoading()
{
    // ��� ����
}

#endif // _3DRENDERER_MANAGERMULTILOADER_H_