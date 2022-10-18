// managerwzunitloader.h

#ifndef _3DRENDERER_MANAGERWZUNITLOADER_H_
#define _3DRENDERER_MANAGERWZUNITLOADER_H_

#include "managerbase.h"
#include <programcommon/wzhashtable.h>

// 텍스쳐 로드 스타일
#define ULS_DEFAULT                 (0x00000000)
#define ULS_HALFSIZE_TEXTURE        (0x00000001)    // 로드시 텍스처 크기를 반으로
#define ULS_INSTANCING              (0x00000002)    // 인스턴싱 가능
#define ULS_UNIT_LIGHTMAP           (0x00000004)    // 유닛 라이트맵 가능
#define ULS_CHECK_VALID_ANIMATION   (0x00000008)    // 사용 안함
#define ULS_NO_MULTITHREAD          (0x00000010)    // 멀티 스레드 로딩 안함

class CWzUnit;

struct St_WzUnitContainer
{
    CWzUnit* m_pUnit;
    short m_nReference; // 참조 카운트
};

//------------------------------------------------------------------------------
/**
    @class CManagerWzUnitLoader
*/
class CManagerWzUnitLoader : public CManagerBase
{
public:
    // 생성자/소멸자
    CManagerWzUnitLoader();
    virtual ~CManagerWzUnitLoader();

    // 생성/파괴
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();

    // 유닛 로딩
    virtual WzID LoadWzUnit( WzID widType, 
                             const char* lpszFilePath, 
                             BOOL bAutoLoadWzd = TRUE,  // 사용 안함 
                             DWORD dwLoadStyle = ULS_DEFAULT,
                             float fTextureRate = -1.0f );

    // 유닛 추가 (외부에서 직접 호출하는 경우)
    virtual WzID AddWzUnit( CWzUnit* pUnit,
                            BOOL bAutoLoadWzd = TRUE,   // 사용 안함 
                            DWORD dwLoadStyle = ULS_DEFAULT );

    // 유닛 파괴
    virtual void DestroyWzUnit( WzID widType );

    // 유닛 얻기
    CWzUnit* GetWzUnit( WzID widType );

    // wzm 경로 설정
    void SetFolderToLoadWzm( const char* lpszFolder );

protected:
    // 기존에 있으면 참조 카운터 증가
    BOOL AddRefCountIfExist( WzID widType );

    // 유닛 정보 추가
    WzID AddWzUnitInfo( CWzUnit* pUnit, DWORD dwLoadStyle );

    // 유닛 파괴
    void DestroyWzUnitPtr( CWzUnit* pUnit);
    void DestroyAllWzUnit();

    // 유닛 데이타 로딩/파괴
    BOOL LoadWzUnitData( CWzUnit* pUnit, DWORD dwLoadStyle );
    void DestroyWzUnitData( CWzUnit* pUnit );

    // 메시 데이타 로딩/파괴
    BOOL LoadMeshData( CWzUnit* pUnit, DWORD dwLoadStyle );
    void DestroyMeshData( CWzUnit* pUnit );

    // 맵핑 데이타 로딩/파괴
    BOOL LoadMapData( CWzUnit* pUnit, DWORD dwLoadStyle );
    void DestroyMapData( CWzUnit* pUnit );

    // 애니메이션 데이타 로딩/파괴
    BOOL LoadAnimData( CWzUnit* pUnit, DWORD dwLoadStyle );
    void DestroyAnimData( CWzUnit* pUnit );

    // 텍스처 로딩 옵션에 로딩 스타일 적용
    void ApplyLoadStyleToTexLoadOpt( DWORD& dwTexOpt, DWORD dwLoadStyle );

#ifdef _DEBUG
public:
    virtual long GetMemorySize();
#endif 

private:
    // 변수 초기화
    void InitVariables();
    // 해제
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
