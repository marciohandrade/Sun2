// managermultiloader.h

#ifndef _3DRENDERER_MANAGERMULTILOADER_H_
#define _3DRENDERER_MANAGERMULTILOADER_H_

#include "managerbase.h"
#include <programcommon/wzqueue.h>
#include <programcommon/wzarray.h>

class CThreadObject;

// 멀티쓰레드 로딩 스타일
#define MLS_TEXTURE         (0x00000001)
#define MLS_WZD             (0x00000002)
#define MLS_DEFAULT         (MLS_TEXTURE | MLS_WZD)    // 전부 사용    

// 로딩 객체 타입
enum ENUM_LOADOBJTYPE
{
    LOT_TEXTURE = 0,
    LOT_WZD
};

// 로딩 큐 타입
enum ENUM_LOAD_QUEUE_TYPE
{
    QUEUE_TYPE_QUICK = 0,       
    QUEUE_TYPE_NORMAL,
    NUM_QUEUE_TYPE
};

// 로딩 객체
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
    // 생성자/소멸자
    CManagerMultiLoader();
    virtual ~CManagerMultiLoader();

    // 생성/파괴
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();
    
    // 큐에 객체 추가
    virtual void AddObjectToLoad( ENUM_LOADOBJTYPE eLoadObjType, 
                                  DWORD dwObject,
                                  ENUM_LOAD_QUEUE_TYPE eyTpe = QUEUE_TYPE_NORMAL );

    // 로딩
    virtual void New_Load();

    // 멀티 쓰레드 로딩 켜기/끄기
    void TurnOn_MultiThreadedLoading( BOOL bOn );

    // 멀티 쓰레드 로딩 여부
    BOOL IsTurnedOn_MultiThreadedLoading( DWORD dwStyle ) const;

    // 로딩 스타일 설정/얻기
    void SetMultiThreadedLoadingStyle( DWORD dwStyle );
    DWORD GetMultiThreadedLoadingStyle() const;

    // 로딩할 객체 수 얻기
    DWORD GetNumberOfLoadObject( ENUM_LOAD_QUEUE_TYPE eType );

    // 초당 로딩 수 설정 (xxx: 사용 안함)
    void SetLoadCountPerSecond( int LoadPerSecond );
    
    // 쓰레드 멈춤 (xxx: 사용 안함)
    void ExitMultiThreadedLoading();

protected:
    // 쓰레드 객체 생성/파괴
    BOOL CreateThreadObject();
    void DestroyThreadObject();

private:
    // 변수 초기화
    void InitVariables();
    // 해제
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
    // 사용 안함
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerMultiLoader::ExitMultiThreadedLoading()
{
    // 사용 안함
}

#endif // _3DRENDERER_MANAGERMULTILOADER_H_