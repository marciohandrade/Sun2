#pragma once

#ifndef OBJECTFACTORY_H_
#define OBJECTFACTORY_H_

#include <boost/scoped_ptr.hpp>
#include "Singleton.h"

class IObjectFactoryImpl;

class ObjectFactory : public util::Singleton<ObjectFactory>
{
public:
    enum ChunkMode
    {
        kVariableChunk = 1, 
        kFixedChunk = 2
    };
//#ifdef USE_FIXED_SIZE_MEMORY_POOL
    enum { kDefaultChunkMode = kFixedChunk };
//#else
//    enum { kDefaultChunkMode = kVariableChunk };
//#endif

public:
    ObjectFactory();
    ~ObjectFactory();
    
public:
    void CreateImpl(const int impl_type);
    void Init(
        const size_t player_pool_size,
        const size_t monster_pool_size,
        const size_t item_pool_size,
        const size_t map_object_Pool_size, 
        const size_t map_npc_pool_size,
        const size_t merchant_pool_size,
        const size_t lucky_monster_pool_size
    );
    void Release();
    Object*	AllocObject(const eOBJECT_TYPE object_type);
    bool FreeObject(Object* const object);

private:
    typedef boost::scoped_ptr<IObjectFactoryImpl> Impl;
    Impl impl_;
};

#endif // OBJECTFACTORY_H_