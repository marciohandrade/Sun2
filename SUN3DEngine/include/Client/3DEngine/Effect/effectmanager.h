// effectmanager.h

#ifndef _EFFECT_EFFECTMANAGER_H_
#define _EFFECT_EFFECTMANAGER_H_

// xxx: �ܺο��� �� ������ ���� include�ϴ� ��찡 �־
// typedef.h�� include�ߴ�.
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
    // ������/�Ҹ���
    CEffectManager();
    virtual ~CEffectManager();

    // ����/�ı�
    WZEFF_API bool Create( CDrawBase* pDraw, CWorldBase* pWorld );
    WZEFF_API void Destroy();

    // �ؽ�ó ���� ����� �ε�
    WZEFF_API bool LoadTextureFiles( const char* pszDefaultFolder,
                                     const char* pszFileName,
                                     EENUM_LOAD_TYPE eLoadType = EELT_ADD );

    // �ε�� ����Ʈ �ؽ��ĸ� ����
    WZEFF_API void ReleaseEffectTexture();


    // �޽� ���� �ε�
    WZEFF_API bool LoadMeshFiles( const char* pszDefaultFolder,
                                  const char* pszFileName, 
                                  EENUM_LOAD_TYPE eLoadType = EELT_ADD );
    
    // ����Ʈ ���� ������ ��� (��� ���Ϸκ���)
    WZEFF_API bool LoadEffectFromFile( const char* pszListFilePath, 
                                       const char* pszEffFolderPath,
                                       EENUM_LOAD_TYPE eLoadType = EELT_ADD,
                                       int nLevel = 0 );

    // �ش� ����� ����Ʈ�� ����
    WZEFF_API void UnLoadEffectFromFile( const char* pszListFilePath );

    // ���� �ε�
    WZEFF_API St_BodyCreate* Load( DA_FILE hf, WzID wzId = 0 );
    WZEFF_API St_BodyCreate* Load( const char* pszFilePath, WzID wzID );

    // ���� ����
    WZEFF_API bool Save( DA_FILE hf, WzID wzId );
    WZEFF_API bool Save( const char* pszFilePath, WzID wzId );

    // ele ���� �ε�/����/����
    WZEFF_API St_Ele_Node* LoadEleInfo( DA_FILE hf );
    WZEFF_API void SaveEleInfo( DA_FILE hf, St_Ele_Node* pEleinfo );
    WZEFF_API void DeleteEleInfo( St_Ele_Node* pNodeinfo );

    // ����Ʈ ���� ���� ���
    WZEFF_API void RegisterEffectInfo( const St_BodyCreate* pEffectBody );

    // �� ����Ʈ ���� ���� �߰�
    WZEFF_API bool AddEffectInfo( WzID wzID );
    // ����Ʈ ���� ���
    WZEFF_API St_BodyCreate* GetEffectInfo( WzID wzID );

    // ����Ʈ ���� ���� ����
    WZEFF_API void ReleaseAllEffectInfo();
    WZEFF_API void ReleaseAtEffectInfo( WzID wzID );

    // ����Ʈ ����
    WZEFF_API HANDLE CreateEffect( WzID wzEffID, 
                                   const WzVector& wvPos, 
                                   const WzVector& wvScale,
                                   const WzVector& wvRot,
                                   int iRate = 100 );

    WZEFF_API HANDLE CreateEffect( WzID wzEffID, const WzVector& wvPos, int iRate = 100 );
    WZEFF_API HANDLE CreateEffect( WzID wzEffID, const WzVector& wvPos, const WzVector& wvScale, int iRate = 100 );

    // ���õ� element�� ����(���) - ������ ���
    WZEFF_API HANDLE CreateSelectedElements( WzID wzEffID, const WzVector& wvPos, std::list<int>* pSelList );

    // �ڵ�� EffectBody ������ ���
    WZEFF_API CEffectBody* GetEffectPtrHandle( HANDLE hHandle );

    // ������ ����Ʈ �ı�
    WZEFF_API void DestroyAllEffect();
    WZEFF_API void RemoveEffectPtr( HANDLE hHandle );

    // ����Ʈ ó��/������ (�����Ӵ� �ѹ� ȣ��)
    WZEFF_API void Move( DWORD dwTick );
    WZEFF_API void Render();

    // �޸�Ǯ �����
    WZEFF_API void ShrinkMemoryPool();
    // ���� ���� �ʱ�ȭ
    WZEFF_API void ClearSoundInfos();

    // �ڵ� ���� ���� ���� �Ķ���� ����Ʈ �ʱ�ȭ
    WZEFF_API void ClearAutoLvlCtlParams();
    // �ڵ� ���� ���� ���� �Ķ���� �߰�
    WZEFF_API void AddAutoLvlCtlParam( int iFrame, int iLvl );

    // �ڵ� ���� ���� ���� �Ķ���� ����Ʈ �ʱ�ȭ
    WZEFF_API void ClearAutoRateCtlParams();
    // �ڵ� ���� ���� ���� �Ķ���� �߰�
    WZEFF_API void AddAutoRateCtlParam( int iFrame, int iRate );

    // ����Ʈ LOD �Ķ���� ����Ʈ �ʱ�ȭ
    WZEFF_API void ClearEffectLODParams();
    // ����Ʈ LOD �Ķ���� �߰�
    WZEFF_API void AddEffectLODParam( float fDist, int iRate );

    // ����Ʈ ���� �ѱ�/����
    WZEFF_API void SetRenderEffect(bool bRender);
    // ����Ʈ ���� ���� ���
    WZEFF_API bool IsRenderEffect();


#ifdef _DEBUG
    // MILKY ����׿�
    WZEFF_API long GetTotalMemorySize();
    WZEFF_API long GetTotalFreeMemorySize();
    WZEFF_API long GetEffectCount();
#endif 


    // �ؽ�ó ���� ��� ���
    const CLoadBaseManager& GetLoadTextureMG() const;
    // �޽� ���� ��� ���
    const CLoadBaseManager& GetLoadMeshMG() const;

    // �޸� �Ŵ��� ���
    CMemoryManager* GetMemoryManager();
    // ���� ���� �Ŵ��� ���
    CSoundInfoManager* GetSoundInfoManager();

    // CDiscAccess ����/���
    void SetDiscAccess( CDiscAccess* pDiscAccess );
    CDiscAccess* GetDiscAccess();

    // ���� ����/���
    void SetWorld( CWorldBase* pWorld );
    CWorldBase* GetWorld();

    // CDrawBase ���
    CDrawBase* GetDraw();

    // ����Ʈ ���̽� ���� ����
    void SetBaseEffectLevel( int lvel );
    int GetBaseEffectLevel();

    // ������ ����Ʈ ��
    int GetNumCreatedEffects() const;

    // �ɼ� �ʱ�ȭ
    void ClearOption();
    // �ɼ� ����
    void SetOption( DWORD flag );
    // �ɼ� ����
    void UnsetOption( DWORD flag );
    // �ɼ� �˻�
    bool CheckOption( DWORD flag ) const;

    // 
    HANDLE GetEffectTexture( WzID EffectTextureID );

private:
    // �ʱ�ȭ
    void Initialize();

    // �ε��� �ؽ�ó/�޽� ���� ����
    void RemoveTextureFiles();
    void RemoveMeshFiles();

    // Body ���� ���� �ε�/����
    void LoadBodyInfoOld( DA_FILE hf, St_BodyCreate* pInfo );
    void LoadBodyInfo( DA_FILE hf, St_BodyCreate* pInfo );
    void SaveBodyInfo( DA_FILE hf, St_BodyCreate* pInfo );

    // Element ���� ���� �ε�/����
    void LoadElementInfo( DA_FILE hf, St_ElementCreate* pInfo );
    void SaveElementInfo( DA_FILE hf, St_ElementCreate* pInfo );

    // ���� ����/����
    void LoadCreateInfo( DA_FILE hf, St_Create_Info* pInfo );
    void SaveCreateInfo( DA_FILE hf, St_Create_Info* pInfo );

    // �̵� ����/����
    void LoadMoveInfo( DA_FILE hf, St_Move_Info* pInfo );
    void SaveMoveInfo( DA_FILE hf, St_Move_Info* pInfo );

    // Change ����/����
    void LoadChangeInfo( DA_FILE hf, St_Change_Info* pInfo );
    void SaveChangeInfo( DA_FILE hf, St_Change_Info* pInfo );

    // Visual ����/����
    void LoadVisualInfo( DA_FILE hf, St_Visual_Info* pInfo );
    void SaveVisualInfo( DA_FILE hf, St_Visual_Info* pInfo );

    // element ���� (���� ����Ʈ ����)
    bool CreateElement( CEffectBody* pEffBody, const St_BodyCreate* pBodyCreate, int iRate = 100 );

    // ���� ����Ʈ ���� ����
    void AdjustCreateEffectLevel();
    // ����Ʈ ���� ����
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
    // ������ ����Ʈ ��� �ı�
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
