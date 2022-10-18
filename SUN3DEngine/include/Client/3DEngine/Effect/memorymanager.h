// memorymanager.h

#ifndef _EFFECT_MEMORYMANAGER_H_
#define _EFFECT_MEMORYMANAGER_H_

#include "element/eelementsprite.h"
#include "element/eelementmesh.h"
#include "element/eelementchain.h"
#include "element/eelementflatchain.h"
#include "element/eelementdecal.h"
#include "element/eelementweather.h"
#include "element/effectbody.h"
#include <programcommon/wzmemorypool.h>

//------------------------------------------------------------------------------
/**
    @class CMemoryManager
*/
class CMemoryManager
{
public:
    // 생성자/소멸자
    CMemoryManager();
    ~CMemoryManager();

    // 생성
    BOOL Create();
    // 파괴
    void Destroy();

    // 축소
    void Shrink();

    // Sprite 할당/해제
    CEElementSprite* AllocateSprite();
    void FreeSprite( CEElementSprite* p );

    // Mesh 할당/해제
    CEElementMesh* AllocateMesh();
    void FreeMesh( CEElementMesh* p );

    // Chain 할당/해제
    CEElementChain* AllocateChain();
    void FreeChain( CEElementChain* p );

    // FlatChain 할당/해제
    CEElementFlatChain* AllocateFlatChain();
    void FreeFlatChain( CEElementFlatChain* p );

    // Decal 할당/해제
    CEElementDecal* AllocateDecal();
    void FreeDecal( CEElementDecal* p );

    // Weather 할당/해제
    CEElementWeather* AllocateWeather();
    void FreeWeather( CEElementWeather* p );

    // Body 할당/해제
    CEffectBody* AllocateEffectBody();
    void FreeEffectBody( CEffectBody* p );

#ifdef _DEBUG    
    long GetTotalMemorySize();
    long GetTotalFreeMemorySize();
#endif 

private:
    CWzMemoryPool<CEElementSprite>* m_wmpSprite;
    CWzMemoryPool<CEElementMesh>* m_wmpMesh;
    CWzMemoryPool<CEElementChain>* m_wmpChain;
    CWzMemoryPool<CEElementFlatChain>* m_wmpFlatChain;
    CWzMemoryPool<CEElementDecal>* m_wmpDecal;
    CWzMemoryPool<CEElementWeather>* m_wmpWeather;
    CWzMemoryPool<CEffectBody>* m_wmpBody;

#ifdef _DEBUG
    long m_SpriteAllocateCount;
    long m_SpriteFreeCount;
#endif 
};

//------------------------------------------------------------------------------
/**
*/
inline CEElementSprite* CMemoryManager::AllocateSprite()
{
    WzAssert( m_wmpSprite );

#ifdef _DEBUG
    ++m_SpriteAllocateCount;
#endif 

    return m_wmpSprite->Alloc();
}

//------------------------------------------------------------------------------
/**
*/
inline void CMemoryManager::FreeSprite( CEElementSprite* p )
{
    WzAssert( p );
    WzAssert( m_wmpSprite );

    if( m_wmpSprite && p )
    {
#ifdef _DEBUG
        ++m_SpriteFreeCount;
#endif 
        m_wmpSprite->Free( p );
    }
#ifdef _DEBUG
    else
    {
        m_SpriteFreeCount = m_SpriteFreeCount;
    }
#endif
}

//------------------------------------------------------------------------------
/**
*/
inline CEElementMesh* CMemoryManager::AllocateMesh()
{
    WzAssert( m_wmpMesh );

    return m_wmpMesh->Alloc();
}

//------------------------------------------------------------------------------
/**
*/
inline void CMemoryManager::FreeMesh( CEElementMesh* p )
{
    WzAssert( p );
    WzAssert( m_wmpMesh );

    if( m_wmpMesh && p )
    {
        m_wmpMesh->Free( p );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline CEElementChain* CMemoryManager::AllocateChain()
{
    WzAssert( m_wmpChain );

    return m_wmpChain->Alloc();
}

//------------------------------------------------------------------------------
/**
*/
inline void CMemoryManager::FreeChain(CEElementChain* p )
{
    WzAssert( p );
    WzAssert( m_wmpChain );

    if( m_wmpChain && p )
    {
        m_wmpChain->Free( p );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline CEElementFlatChain* CMemoryManager::AllocateFlatChain()
{
    WzAssert( m_wmpFlatChain );

    return m_wmpFlatChain->Alloc();
}

//------------------------------------------------------------------------------
/**
*/
inline void CMemoryManager::FreeFlatChain( CEElementFlatChain* p )
{
    WzAssert( p );
    WzAssert( m_wmpFlatChain );

    if( m_wmpFlatChain && p )
    {
        m_wmpFlatChain->Free( p );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline CEElementDecal* CMemoryManager::AllocateDecal()
{
    WzAssert( m_wmpDecal );

    return m_wmpDecal->Alloc();
}

//------------------------------------------------------------------------------
/**
*/
inline void CMemoryManager::FreeDecal( CEElementDecal* p )
{
    WzAssert( p );
    WzAssert( m_wmpDecal );

    if( m_wmpDecal && p )
    {
        m_wmpDecal->Free( p );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline CEElementWeather* CMemoryManager::AllocateWeather()
{
    WzAssert( m_wmpWeather );

    return m_wmpWeather->Alloc();
}

//------------------------------------------------------------------------------
/**
*/
inline void CMemoryManager::FreeWeather( CEElementWeather* p )
{
    WzAssert( p );
    WzAssert( m_wmpWeather );

    if( m_wmpWeather && p )
    {
        m_wmpWeather->Free( p );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline CEffectBody* CMemoryManager::AllocateEffectBody()
{
    WzAssert( m_wmpBody );

    return m_wmpBody->Alloc();
}

//------------------------------------------------------------------------------
/**
*/
inline void CMemoryManager::FreeEffectBody( CEffectBody* p )
{
    WzAssert( p );
    WzAssert( m_wmpBody );

    if( m_wmpBody && p )
    {
        m_wmpBody->Free( p );
    }
}

#endif // _EFFECT_MEMORYMANAGER_H_