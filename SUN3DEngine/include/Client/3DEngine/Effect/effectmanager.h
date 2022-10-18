// effectmanager.h

#ifndef _EFFECT_EFFECTMANAGER_H_
#define _EFFECT_EFFECTMANAGER_H_

// xxx: 외부에서 본 파일을 직접 include하는 경우가 있어서
// typedef.h를 include했다.
#include "typedef.h"

#include <programcommon/discaccess.h>
#include <programcommon/wzhandlemanager.h>
#include <programcommon/wzsingleton.h>
#include <programcommon/wzflag.h>

#include "createinfo/bodycreate.h"
#include "loadmanager.h"

#include <hash_map>

#ifndef LAST_RELEASE
#include "effectdebuginfo.h"
extern WZEFF_API St_EffDbgInfo g_effDbgInfo;
#endif

#define DIST_FROM_NEAR   56.0f

class CDrawBase;
class CWorldBase;
class CEffectBody;
class CMemoryManager;
class CSoundInfoManager;


//------------------------------------------------------------------------------
/**
    @class CEffectManager
*/
class CEffectManager : public CWzSingleton<CEffectManager>  
{
    struct sEffectTexture
    {
        WzID m_TextureID;
        std::string m_TexturePath;
        HANDLE m_TextureHandle;
    };

    std::vector<sEffectTexture*> m_TextureArray;


public:
    // 생성자/소멸자
    CEffectManager();
    virtual ~CEffectManager();

    // 생성/파괴
    WZEFF_API bool Create( CDrawBase* pDraw, CWorldBase* pWorld );
    WZEFF_API void Destroy();

    // 텍스처 파일 목록을 로드
    WZEFF_API bool LoadTextureFiles( const char* pszDefaultFolder,
                                     const char* pszFileName,
                                     EENUM_LOAD_TYPE eLoadType = EELT_ADD );

    // 로드된 이펙트 텍스쳐를 제거
    WZEFF_API void ReleaseEffectTexture();


    // 메시 파일 로딩
    WZEFF_API bool LoadMeshFiles( const char* pszDefaultFolder,
                                  const char* pszFileName, 
                                  EENUM_LOAD_TYPE eLoadType = EELT_ADD );
    
    // 이펙트 생성 정보들 등록 (목록 파일로부터)
    WZEFF_API bool LoadEffectFromFile( const char* pszListFilePath, 
                                       const char* pszEffFolderPath,
                                       EENUM_LOAD_TYPE eLoadType = EELT_ADD,
                                       int nLevel = 0 );

    // 해당 목록의 이펙트들 제거
    WZEFF_API void UnLoadEffectFromFile( const char* pszListFilePath );

    // 파일 로딩
    WZEFF_API St_BodyCreate* Load( DA_FILE hf, WzID wzId = 0 );
    WZEFF_API St_BodyCreate* Load( const char* pszFilePath, WzID wzID );

    // 파일 저장
    WZEFF_API bool Save( DA_FILE hf, WzID wzId );
    WZEFF_API bool Save( const char* pszFilePath, WzID wzId );

    // ele 정보 로드/저장/제거
    WZEFF_API St_Ele_Node* LoadEleInfo( DA_FILE hf );
    WZEFF_API void SaveEleInfo( DA_FILE hf, St_Ele_Node* pEleinfo );
    WZEFF_API void DeleteEleInfo( St_Ele_Node* pNodeinfo );

    // 이펙트 생성 정보 등록
    WZEFF_API void RegisterEffectInfo( const St_BodyCreate* pEffectBody );

    // 빈 이펙트 생성 정보 추가
    WZEFF_API bool AddEffectInfo( WzID wzID );
    // 이펙트 정보 얻기
    WZEFF_API St_BodyCreate* GetEffectInfo( WzID wzID );

    // 이펙트 생성 정보 제거
    WZEFF_API void ReleaseAllEffectInfo();
    WZEFF_API void ReleaseAtEffectInfo( WzID wzID );

    // 이펙트 생성
    WZEFF_API HANDLE CreateEffect( WzID wzEffID, 
                                   const WzVector& wvPos, 
                                   const WzVector& wvScale,
                                   const WzVector& wvRot,
                                   int iRate = 100 );

    WZEFF_API HANDLE CreateEffect( WzID wzEffID, const WzVector& wvPos, int iRate = 100 );
    WZEFF_API HANDLE CreateEffect( WzID wzEffID, const WzVector& wvPos, const WzVector& wvScale, int iRate = 100 );

    // 선택된 element만 생성(재생) - 툴에서 사용
    WZEFF_API HANDLE CreateSelectedElements( WzID wzEffID, const WzVector& wvPos, std::list<int>* pSelList );

    // 핸들로 EffectBody 포인터 얻기
    WZEFF_API CEffectBody* GetEffectPtrHandle( HANDLE hHandle );

    // 생성된 이펙트 파괴
    WZEFF_API void DestroyAllEffect();
    WZEFF_API void RemoveEffectPtr( HANDLE hHandle );

    // 이펙트 처리/랜더링 (프레임당 한번 호출)
    WZEFF_API void Move( DWORD dwTick );
    WZEFF_API void Render();

    // 메모리풀 재생성
    WZEFF_API void ShrinkMemoryPool();
    // 사운드 정보 초기화
    WZEFF_API void ClearSoundInfos();

    // 자동 생성 레벨 조절 파라미터 리스트 초기화
    WZEFF_API void ClearAutoLvlCtlParams();
    // 자동 생성 레벨 조절 파라미터 추가
    WZEFF_API void AddAutoLvlCtlParam( int iFrame, int iLvl );

    // 자동 생성 비율 조절 파라미터 리스트 초기화
    WZEFF_API void ClearAutoRateCtlParams();
    // 자동 생성 비율 조절 파라미터 추가
    WZEFF_API void AddAutoRateCtlParam( int iFrame, int iRate );

    // 이펙트 LOD 파라미터 리스트 초기화
    WZEFF_API void ClearEffectLODParams();
    // 이펙트 LOD 파라미터 추가
    WZEFF_API void AddEffectLODParam( float fDist, int iRate );

    // 이펙트 랜더 켜기/끄기
    WZEFF_API void SetRenderEffect(bool bRender);
    // 이펙트 랜더 상태 얻기
    WZEFF_API bool IsRenderEffect();


#ifdef _DEBUG
    // MILKY 디버그용
    WZEFF_API long GetTotalMemorySize();
    WZEFF_API long GetTotalFreeMemorySize();
    WZEFF_API long GetEffectCount();
#endif 


    // 텍스처 파일 목록 얻기
    const CLoadBaseManager& GetLoadTextureMG() const;
    // 메시 파일 목록 얻기
    const CLoadBaseManager& GetLoadMeshMG() const;

    // 메모리 매니저 얻기
    CMemoryManager* GetMemoryManager();
    // 사운드 정보 매니저 얻기
    CSoundInfoManager* GetSoundInfoManager();

    // CDiscAccess 설정/얻기
    void SetDiscAccess( CDiscAccess* pDiscAccess );
    CDiscAccess* GetDiscAccess();

    // 월드 설정/얻기
    void SetWorld( CWorldBase* pWorld );
    CWorldBase* GetWorld();

    // CDrawBase 얻기
    CDrawBase* GetDraw();

    // 이펙트 베이스 레벨 설정
    void SetBaseEffectLevel( int lvel );
    int GetBaseEffectLevel();

    // 생성된 이펙트 수
    int GetNumCreatedEffects() const;

    // 옵션 초기화
    void ClearOption();
    // 옵션 설정
    void SetOption( DWORD flag );
    // 옵션 제거
    void UnsetOption( DWORD flag );
    // 옵션 검사
    bool CheckOption( DWORD flag ) const;

    // 
    HANDLE GetEffectTexture( WzID EffectTextureID );

private:
    // 초기화
    void Initialize();

    // 로딩된 텍스처/메시 파일 제거
    void RemoveTextureFiles();
    void RemoveMeshFiles();

    // Body 생성 정보 로딩/저장
    void LoadBodyInfoOld( DA_FILE hf, St_BodyCreate* pInfo );
    void LoadBodyInfo( DA_FILE hf, St_BodyCreate* pInfo );
    void SaveBodyInfo( DA_FILE hf, St_BodyCreate* pInfo );

    // Element 생성 정보 로딩/저장
    void LoadElementInfo( DA_FILE hf, St_ElementCreate* pInfo );
    void SaveElementInfo( DA_FILE hf, St_ElementCreate* pInfo );

    // 생성 정보/저장
    void LoadCreateInfo( DA_FILE hf, St_Create_Info* pInfo );
    void SaveCreateInfo( DA_FILE hf, St_Create_Info* pInfo );

    // 이동 정보/저장
    void LoadMoveInfo( DA_FILE hf, St_Move_Info* pInfo );
    void SaveMoveInfo( DA_FILE hf, St_Move_Info* pInfo );

    // Change 정보/저장
    void LoadChangeInfo( DA_FILE hf, St_Change_Info* pInfo );
    void SaveChangeInfo( DA_FILE hf, St_Change_Info* pInfo );

    // Visual 정보/저장
    void LoadVisualInfo( DA_FILE hf, St_Visual_Info* pInfo );
    void SaveVisualInfo( DA_FILE hf, St_Visual_Info* pInfo );

    // element 생성 (실제 이펙트 생성)
    bool CreateElement( CEffectBody* pEffBody, const St_BodyCreate* pBodyCreate, int iRate = 100 );

    // 생성 이펙트 레벨 조절
    void AdjustCreateEffectLevel();
    // 이펙트 비율 조절
    void AdjustEffectRate();

public:
    typedef stdext::hash_map<WzID, St_BodyCreate*> stdBodyCreateMap;
    typedef stdext::hash_map<HANDLE, CEffectBody*> stdEffectBodyMap;
    typedef std::vector<St_AutoCtlParam> stdAutoCtlParams;
    typedef std::vector<St_EffLODParam> stdEffLODParams;

    CDrawBase* m_pDrawRef;
    CWorldBase* m_pWorldRef;

    CDiscAccess* m_pDiscAccess;

    CMemoryManager* m_pMemManager;
    CSoundInfoManager* m_pSoundInfoManager;

    CLoadBaseManager m_cLoadTextureMG;
    CLoadBaseManager m_cLoadMeshMG;

    CWzHandleManager<CEffectBody*> m_hmEffect;
    stdBodyCreateMap m_htEffSaveData;
    stdEffectBodyMap m_hmEffectMG;
    
    char m_szEffectVer[10];
    CWzFlag<DWORD> m_optFlag;

    stdAutoCtlParams m_autoLvlCtlParams;
    stdAutoCtlParams m_autoRateCtlParams;
    stdEffLODParams m_effLODParams;

    int m_nBaseEffectLvl;
    int m_nCreateEffectLvl;
    int m_nEffectRate;

    bool m_bRenderEffet;


public:
    WZEFF_API stdEffectBodyMap& GetEffectBodyMap() { return m_hmEffectMG; }
    const stdBodyCreateMap& GetEffSaveData() const { return m_htEffSaveData; }

#ifdef _USE_ENGINE_MEMORY_POOL_
    template<typename T>
        T* AllocNew();

    template<typename T>
        void AllocDelete(T* ptr);

    void CopyBody(St_BodyCreate* rhs, St_BodyCreate* lhs);
    void ReleaseBody(St_BodyCreate* pBody);
#endif //_USE_ENGINE_MEMORY_POOL_
};

//------------------------------------------------------------------------------
/**
*/
inline void CEffectManager::DeleteEleInfo( St_Ele_Node* pNodeinfo )
{
    SAFE_DELETE( pNodeinfo );
}

//------------------------------------------------------------------------------
/**
*/
inline HANDLE CEffectManager::CreateEffect( WzID wzEffID, const WzVector& wvPos, int iRate )
{
    static WzVector wvScale = { 1.0f, 1.0f, 1.0f };
    static WzVector wvRot = { 0.0f, 0.0f, 0.0f };

    return CreateEffect( wzEffID, wvPos, wvScale, wvRot, iRate );
}

//------------------------------------------------------------------------------
/**
*/
inline HANDLE CEffectManager::CreateEffect( WzID wzEffID, const WzVector& wvPos, const WzVector& wvScale, int iRate )
{
    static WzVector wvRot = { 0.0f, 0.0f, 0.0f };

    return CreateEffect( wzEffID, wvPos, wvScale, wvRot, iRate );
}

//------------------------------------------------------------------------------
/**
*/
inline const CLoadBaseManager& CEffectManager::GetLoadTextureMG() const
{
    return m_cLoadTextureMG;
}

//------------------------------------------------------------------------------
/**
*/
inline const CLoadBaseManager& CEffectManager::GetLoadMeshMG() const
{ 
    return m_cLoadMeshMG; 
}

//------------------------------------------------------------------------------
/**
*/
inline CMemoryManager* CEffectManager::GetMemoryManager()
{
    return m_pMemManager;
}

//------------------------------------------------------------------------------
/**
*/
inline CSoundInfoManager* CEffectManager::GetSoundInfoManager()
{
    return m_pSoundInfoManager;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectManager::SetDiscAccess( CDiscAccess* pDiscAccess )
{   
    WzAssert( pDiscAccess );

    m_pDiscAccess = pDiscAccess;    
}

//------------------------------------------------------------------------------
/**
*/
inline CDiscAccess* CEffectManager::GetDiscAccess()
{
    return m_pDiscAccess;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectManager::SetWorld( CWorldBase* pWorld )
{
    // 생성된 이펙트 모두 파괴
    DestroyAllEffect();
    
    m_pWorldRef = pWorld ;      
}

//------------------------------------------------------------------------------
/**
*/
inline CWorldBase* CEffectManager::GetWorld()
{
    return m_pWorldRef;
}

//------------------------------------------------------------------------------
/**
*/
inline CDrawBase* CEffectManager::GetDraw()
{
    return m_pDrawRef; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectManager::SetBaseEffectLevel( int lvl )
{
    m_nBaseEffectLvl = lvl;
}

//------------------------------------------------------------------------------
/**
*/
inline int CEffectManager::GetBaseEffectLevel()
{
    return m_nBaseEffectLvl;
}

//------------------------------------------------------------------------------
/**
*/
inline int CEffectManager::GetNumCreatedEffects() const
{
    return (int)m_hmEffectMG.size();
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectManager::ClearOption()
{
    m_optFlag.Clear();
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectManager::SetOption( DWORD flag )
{
    m_optFlag.Set( flag );
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectManager::UnsetOption( DWORD flag )
{
    m_optFlag.Unset( flag );
}

//------------------------------------------------------------------------------
/**
*/
inline bool CEffectManager::CheckOption( DWORD flag ) const
{
    return m_optFlag.Check( flag );
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectManager::SetRenderEffect(bool bRender)
{
    m_bRenderEffet = bRender;
}

//------------------------------------------------------------------------------
/**
*/
inline bool CEffectManager::IsRenderEffect()
{
     return m_bRenderEffet;
}

#endif // _EFFECT_EFFECTMANAGER_H_
