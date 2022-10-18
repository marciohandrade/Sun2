#include "stdafx.h"
#include "ObjectFactory.h"

#include <boost/pool/object_pool.hpp>
#include "Object.h"
#include "Player.h"
#include "Item.h"
#include "MapObject.h"
#include "Monster.h"
#include "Summoned.h"
#include "MapNPC.h"
#include "Merchant.h"
#include "LuckyMonster.h"
#include "TotemNpc.h"
#include "SSQMonster.h"
#include "Collection.h"
#include "CrystalWarp.h"
#include "FriendMonster.h"
#include "SyncMerchant.h"
#include "ObjKeyGenerator.h"
#include "MemoryPoolFactory.h"
#include "MemoryPoolFactoryV.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : IObjectFactoryImpl
////////////////////////////////////////////////////////////////////////////////////////////////////

class IObjectFactoryImpl
{
public:
    IObjectFactoryImpl() {}
    virtual ~IObjectFactoryImpl() {}

public:
    virtual void Init(
        const size_t player_pool_size,
        const size_t monster_pool_size,
        const size_t item_pool_size,
        const size_t map_object_Pool_size, 
        const size_t map_npc_pool_size,
        const size_t merchant_pool_size,
        const size_t lucky_monster_pool_size
    ) {
        __UNUSED(player_pool_size);
        __UNUSED(monster_pool_size);
        __UNUSED(item_pool_size);
        __UNUSED(map_object_Pool_size);
        __UNUSED(map_npc_pool_size);
        __UNUSED(merchant_pool_size);
        __UNUSED(lucky_monster_pool_size);
    }
    virtual void Release() {}
    virtual Object*	AllocObject(const eOBJECT_TYPE object_type) = 0;
    virtual bool FreeObject(Object* const object) = 0;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : FixedChunkImpl
////////////////////////////////////////////////////////////////////////////////////////////////////

class FixedChunkImpl : public IObjectFactoryImpl
{
public:
    FixedChunkImpl() : 
        monster_keygen_(MONSTER_OBJECT_KEY+1, NPC_OBJECT_KEY), 
        item_keygen_(ITEM_OBJECT_KEY+1, PET_OBJECT_KEY), 
        monster_pool_(monster_keygen_), 
        summoned_pool_(monster_keygen_), 
        item_pool_(item_keygen_), 
        money_pool_(item_keygen_), 
        map_npc_pool_(monster_keygen_), 
        merchant_pool_(monster_keygen_), 
        lucky_monster_pool_(monster_keygen_), 
        totem_npc_pool_(item_keygen_), 
        ssq_monster_pool_(monster_keygen_), 
        crystal_warp_pool_(monster_keygen_), 
        friend_monster_pool_(monster_keygen_), 
        sync_merchant_pool_(monster_keygen_)
    {
        pools_[player_pool_.kType] = &player_pool_;
        pools_[monster_pool_.kType] = &monster_pool_;
        pools_[summoned_pool_.kType] = &summoned_pool_;
        pools_[item_pool_.kType] = &item_pool_;
        pools_[money_pool_.kType] = &money_pool_;
        pools_[map_object_pool_.kType] = &map_object_pool_;
        pools_[map_npc_pool_.kType] = &map_npc_pool_;
        pools_[merchant_pool_.kType] = &merchant_pool_;
        pools_[lucky_monster_pool_.kType] = &lucky_monster_pool_;
        pools_[totem_npc_pool_.kType] = &totem_npc_pool_;
        pools_[ssq_monster_pool_.kType] = &ssq_monster_pool_;
        pools_[collection_pool_.kType] = &collection_pool_;
        pools_[crystal_warp_pool_.kType] = &crystal_warp_pool_;
        pools_[friend_monster_pool_.kType] = &friend_monster_pool_;
        pools_[sync_merchant_pool_.kType] = &sync_merchant_pool_;
    }
    ~FixedChunkImpl()
    {
    }

public:
    virtual Object*	AllocObject(const eOBJECT_TYPE object_type)
    {
        Pools::iterator it = pools_.find(object_type);
        if (it == pools_.end())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Invalid object type.|ObjectType = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                object_type
            );
            assert(false);
            return NULL;
        }
        IObjectPool* const object_pool = it->second;
        return object_pool->Alloc();
    }
    virtual bool FreeObject(Object* const object)
    {
        if (object == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Object is null.|"), 
                __FUNCTION__, 
                __LINE__
            );
            assert(false);
            return false;
        }
        if (object->IsDeleted())
        {
            const Object::Key key = object->GetObjectKey();
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                key
            );
            assert(false);
            return false;
        }
        const eOBJECT_TYPE object_type = object->GetObjectType();
        Pools::iterator it = pools_.find(object_type);
        if (it == pools_.end())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Invalid object type.|ObjectType = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                object_type
            );
            assert(false);
            return false;
        }
        IObjectPool* const object_pool = it->second;
        return object_pool->Free(object);
    }

private:
    // 키 생성기
    class ObjectKeyGenerator
    {
    public:
        enum { kInvalidKey = 0 };
    public:
        ObjectKeyGenerator(const Object::Key min_key, const Object::Key max_key) : 
            min_key_(min_key), max_key_(max_key), key_(min_key_)
        {
            assert(min_key_ < max_key_);
        }
        ~ObjectKeyGenerator() 
        {
        }
    public:
        Object::Key Alloc()
        {
            if (return_keys_.empty() == false)
            {
                const Object::Key key = return_keys_.front();
                return_keys_.pop_front();
                free_keys_.erase(key);
                return key;
            }
            if (key_ > max_key_)
            {
                assert(false);
                return kInvalidKey;
            }
            return key_++;
        }
        bool Free(const Object::Key key)
        {
            if (free_keys_.find(key) != free_keys_.end())
            {
                assert(false);
                return false;
            }
            return_keys_.push_back(key);
            free_keys_.insert(key);
            return true;
        }
    private:
        typedef STLX_DEQUE<Object::Key> ReturnKeys;
        typedef STLX_HASH_SET<Object::Key> FreeKeys;
        const Object::Key min_key_;
        const Object::Key max_key_;
        Object::Key key_;
        ReturnKeys return_keys_;
        FreeKeys free_keys_;
    };

    // 메모리 풀 인터페이스
    class IObjectPool
    {
    public:
        IObjectPool() {}
        virtual ~IObjectPool() {}
        virtual Object* Alloc() = 0;
        virtual bool Free(Object* const object) = 0;
    };

    // 메모리 풀 (키젠 사용)
    template <typename OBJECT_T, eOBJECT_TYPE kObjectType, size_t kPoolSize, bool kUseKeygen = true>
    class ObjectPool : public IObjectPool
    {
    public:
        enum { kType = kObjectType };
        ObjectPool(ObjectKeyGenerator& keygen) : pool_impl_(kPoolSize), keygen_(keygen)
        {
        }
    public:
        virtual Object* Alloc()
        {
            const Object::Key key = keygen_.Alloc();
            if (key == ObjectKeyGenerator::kInvalidKey)
            {
                return NULL;
            }
            Object* object = pool_impl_.construct();
            if (object == NULL) {
                keygen_.Free(key);
                return NULL;
            }
            object->SetObjectKey(key);
            object->SetObjectType(kObjectType);
            return object;
        }
        virtual bool Free(Object* const object)
        {
            OBJECT_T* dest_obj = (OBJECT_T*)object;
            const Object::Key key = dest_obj->GetObjectKey();
            if (keygen_.Free(key) == false)
            {
                return false;
            }
            dest_obj->Reuse();
            pool_impl_.destroy(dest_obj);
            return true;
        }
    private:
        typedef boost::object_pool<OBJECT_T> PoolImpl;
        PoolImpl pool_impl_;
        ObjectKeyGenerator& keygen_;
    };

    // 메모리 풀 (키젠 사용 안함)
    template <typename OBJECT_T, eOBJECT_TYPE kObjectType, size_t kPoolSize>
    class ObjectPool<OBJECT_T, kObjectType, kPoolSize, false> : public IObjectPool
    {
    public:
        enum { kType = kObjectType };
        ObjectPool() : pool_impl_(kPoolSize)
        {
        }
    public:
        virtual Object* Alloc()
        {
            Object* object = pool_impl_.construct();
            object->SetObjectKey(1);
            object->SetObjectType(kObjectType);
            return object;
        }
        virtual bool Free(Object* const object)
        {
            OBJECT_T* dest_obj = (OBJECT_T*)object;
            dest_obj->Reuse();
            pool_impl_.destroy(dest_obj);
            return true;
        }
    private:
        typedef boost::object_pool<OBJECT_T> PoolImpl;
        PoolImpl pool_impl_;
    };

    struct PoolSize 
    {
        enum { kPlayer = 10 };
        enum { kMonster = 1000 }; 
        enum { kSummoned = 10 };
        enum { kItem = 1000 };
        enum { kMoney = 1000 };
        enum { kMapObject = 100 }; 
        enum { kMapNpc = 10 };
        enum { kMerchant = 10 }; 
        enum { kLuckyMonster = 10 }; 
        enum { kTotemNpc = 10 };
        enum { kSSQMonster = 20 };
        enum { kCollection = 20 };
        enum { kCrystalWarp = 10 };
        enum { kFriendMonster = 20 };
        enum { kSyncMerchant = 20 };
    };
    typedef ObjectPool<Player, PLAYER_OBJECT, PoolSize::kPlayer, false> PlayerPool;
    typedef ObjectPool<Monster, MONSTER_OBJECT, PoolSize::kMonster> MonsterPool;
    typedef ObjectPool<Summoned, SUMMON_OBJECT, PoolSize::kSummoned> SummonedPool;
    typedef ObjectPool<Item, ITEM_OBJECT, PoolSize::kItem> ItemPool;
    typedef ObjectPool<Item, MONEY_OBJECT, PoolSize::kMoney> MoneyPool;
    typedef ObjectPool<MapObject, MAP_OBJECT, PoolSize::kMapObject, false> MapObjectPool;
    typedef ObjectPool<MapNpc, MAPNPC_OBJECT, PoolSize::kMapNpc> MapNpcPool;
    typedef ObjectPool<Merchant, MERCHANT_OBJECT, PoolSize::kMerchant> MerchantPool;
    typedef ObjectPool<LuckyMonster, LUCKYMON_OBJECT, PoolSize::kLuckyMonster> LuckyMonsterPool;
    typedef ObjectPool<TotemNpc, TOTEMNPC_OBJECT, PoolSize::kTotemNpc> TotemNpcPool;
    typedef ObjectPool<SSQMonster, SSQMONSTER_OBJECT, PoolSize::kSSQMonster> SSQMonsterPool;
    typedef ObjectPool<Collection, COLLECTION_OBJECT, PoolSize::kCollection, false> CollectionPool;
    typedef ObjectPool<CrystalWarp, CRYSTALWARP_OBJECT, PoolSize::kCrystalWarp> CrystalWarpPool;
    typedef ObjectPool<FriendMonster, FRIEND_MONSTER_OBJECT, PoolSize::kFriendMonster> FriendMonsterPool;
    typedef ObjectPool<SyncMerchant, SYNC_MERCHANT_OBJECT, PoolSize::kSyncMerchant> SyncMerchantPool;
    typedef STLX_HASH_MAP<int, IObjectPool*> Pools;

private:
    ObjectKeyGenerator monster_keygen_;
    ObjectKeyGenerator item_keygen_;

    PlayerPool player_pool_;
    MonsterPool monster_pool_;
    SummonedPool summoned_pool_;
    ItemPool item_pool_;
    MoneyPool money_pool_;
    MapObjectPool map_object_pool_;
    MapNpcPool map_npc_pool_;
    MerchantPool merchant_pool_;
    LuckyMonsterPool lucky_monster_pool_;
    TotemNpcPool totem_npc_pool_;
    SSQMonsterPool ssq_monster_pool_;
    CollectionPool collection_pool_;
    CrystalWarpPool crystal_warp_pool_;
    FriendMonsterPool friend_monster_pool_;
    SyncMerchantPool sync_merchant_pool_;
    
    Pools pools_;
}; // FixedChunkImpl


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : VariableChunkImpl
////////////////////////////////////////////////////////////////////////////////////////////////////

class VariableChunkImpl : public IObjectFactoryImpl
{
public:
    VariableChunkImpl()
    {
        typedef ObjectPoolDefault<PLAYER_OBJECT, Player> PlayerPool;
        typedef ObjectPoolDefault<MONSTER_OBJECT, Monster> MonsterPool;
        typedef ObjectPoolDefault<SUMMON_OBJECT, Summoned> SummonedPool;
        typedef ObjectPoolDefault<ITEM_OBJECT, Item> ItemPool;
        typedef ObjectPoolDefault<MAP_OBJECT, MapObject> MapObjectPool;
        typedef ObjectPoolDefault<MAPNPC_OBJECT, MapNpc> MapNpcPool;
        typedef ObjectPoolDefault<MERCHANT_OBJECT, Merchant> MerchantPool;
        typedef ObjectPoolDefault<LUCKYMON_OBJECT, LuckyMonster> LuckyMonsterPool;
        typedef ObjectPoolDefault<TOTEMNPC_OBJECT, TotemNpc> TotemNpcPool;
        typedef ObjectPoolDefault<SSQMONSTER_OBJECT, SSQMonster> SSQMonsterPool;
        typedef ObjectPoolDefault<COLLECTION_OBJECT, Collection> CollectionPool;
        typedef ObjectPoolDefault<CRYSTALWARP_OBJECT, CrystalWarp> CrystalWarpPool;
        typedef ObjectPoolDefault<FRIEND_MONSTER_OBJECT, FriendMonster> FriendMonsterPool;
        typedef ObjectPoolDefault<SYNC_MERCHANT_OBJECT, SyncMerchant> SyncMerchantPool;

        router_.RegisterPool<PlayerPool>(PoolIndex::kPlayer);
        router_.RegisterPool<MonsterPool>(PoolIndex::kMonster);
        router_.RegisterPool<SummonedPool>(PoolIndex::kSummoned);
        router_.RegisterPool<ItemPool>(PoolIndex::kItem);
        router_.RegisterPool<MapObjectPool>(PoolIndex::kMapObject);
        router_.RegisterPool<MapNpcPool>(PoolIndex::kMapNpc);
        router_.RegisterPool<MerchantPool>(PoolIndex::kMerchant);
        router_.RegisterPool<LuckyMonsterPool>(PoolIndex::kLuckyMonster);
        router_.RegisterPool<TotemNpcPool>(PoolIndex::kTotemNpc);
        router_.RegisterPool<SSQMonsterPool>(PoolIndex::kSSQMonster);
        router_.RegisterPool<CollectionPool>(PoolIndex::kCollection);
        router_.RegisterPool<CrystalWarpPool>(PoolIndex::kCrystalWarp);
        router_.RegisterPool<FriendMonsterPool>(PoolIndex::kFriendMonster);
        router_.RegisterPool<SyncMerchantPool>(PoolIndex::kSyncMerchant);

        KeyGenerator* keygens[PoolIndex::kMax] =
        {
            NULL, // PoolIndex::kPlayer
            &monster_keygen_, // PoolIndex::kMonster
            &monster_keygen_, // PoolIndex::kSummoned	
            &field_item_keygen_, // PoolIndex::kItem		
            NULL, // PoolIndex::kMapObject
            &monster_keygen_, // PoolIndex::kMapNpc	
            &monster_keygen_, // PoolIndex::kMerchant
            &monster_keygen_, // PoolIndex::kLuckyMonster
            &field_item_keygen_, // PoolIndex::kTotemNpc
            &monster_keygen_, // PoolIndex::kSSQMonster
            NULL, // PoolIndex::kCollection
            &monster_keygen_, // PoolIndex::kCrystalWarp
            &monster_keygen_, // PoolIndex::kFriendMonster
            &monster_keygen_ // PoolIndex::kSyncMerchant
        };

        for (int i = PoolIndex::kPlayer; i < PoolIndex::kMax; ++i)
        {
            IObjectAllocator* const alloc = dynamic_cast<IObjectAllocator*>(router_.GetPool(i));
            alloc->RegisterKeyGenerator(keygens[i]);
        }
    }
    ~VariableChunkImpl()
    {
    }

public:
    virtual void Init(
        const size_t player_pool_size,
        const size_t monster_pool_size,
        const size_t item_pool_size,
        const size_t map_object_Pool_size, 
        const size_t map_npc_pool_size,
        const size_t merchant_pool_size,
        const size_t lucky_monster_pool_size
    ) {
        __UNUSED(player_pool_size);
        __UNUSED(monster_pool_size);
        __UNUSED(item_pool_size);
        __UNUSED(map_object_Pool_size);
        __UNUSED(map_npc_pool_size);
        __UNUSED(merchant_pool_size);
        __UNUSED(lucky_monster_pool_size);

        if (router_.Initialize(PoolIndex::kPlayer, 10, 20, "Player") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kPlayer
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kMonster, 100, 100, "Monster") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kMonster
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kSummoned, 10, 20, "Summoned") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kSummoned
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kItem, 100, 100, "Item") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kItem
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kMapObject, 100, 100, "MapObject") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kMapObject
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kMapNpc, 10, 20, "MapNpc") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kMapNpc
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kMerchant, 10, 20, "Merchant") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kMerchant
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kLuckyMonster, 10, 10, "LuckyMonster") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kLuckyMonster
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kTotemNpc, 10, 20, "TotemNpc") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kTotemNpc
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kSSQMonster, 20, 20, "SSQMonster") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kSSQMonster
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kCollection, 20, 20, "Collection") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kCollection
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kCrystalWarp, 10, 10, "CrystalWarp") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kCrystalWarp
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kFriendMonster, 20, 20, "FriendMonster") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kFriendMonster
            );
            assert(false);
        }
        if (router_.Initialize(PoolIndex::kSyncMerchant, 20, 20, "SyncMerchant") == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Failed to initialize memory pool.|PoolIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                PoolIndex::kSyncMerchant
            );
            assert(false);
        }
        monster_keygen_.Create(MONSTER_OBJECT_KEY+1, NPC_OBJECT_KEY);
        field_item_keygen_.Create(ITEM_OBJECT_KEY+1, PET_OBJECT_KEY);
    }
    virtual void Release() 
    {
        router_.Release();
    }
    virtual Object*	AllocObject(const eOBJECT_TYPE object_type)
    {
        const PoolIndex::Value pool_index = GetPoolIndex(object_type);
        if (pool_index >= PoolIndex::kMax)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Invalid object type.|ObjectType = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                object_type
            );
            assert(false);
            return NULL;
        }
        Object* const object = (Object*)router_.Alloc(pool_index);
        if (object == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Fail to create object.|ObjectType = %d, PoolIndex = %d|"), 
                __FUNCTION__,
                __LINE__, 
                object_type, 
                pool_index
            );
            assert(false);
            return NULL;
        }
        if (object_type == ITEM_OBJECT || object_type == MONEY_OBJECT)
        {
            object->SetObjectType(object_type);
        }
        return object;
    }
    virtual bool FreeObject(Object* const object)
    {
        if (object == NULL)
        {
            return false;
        }
        const eOBJECT_TYPE object_type = object->GetObjectType();
        const PoolIndex::Value pool_index = GetPoolIndex(object_type);
        if (pool_index >= PoolIndex::kMax)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Invalid object type.|ObjectType = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                object_type
            );
            assert(false);
            return false;
        }
        return !!router_.Free(pool_index, object);
    }

private:
    struct PoolIndex
    {
        enum Value
        {
            kPlayer = 0,
            kMonster,
            kSummoned,
            kItem,
            kMapObject,
            kMapNpc,
            kMerchant,
            kLuckyMonster,
            kTotemNpc,
            kSSQMonster,
            kCollection,
            kCrystalWarp,
            kFriendMonster,
            kSyncMerchant,
            kMax
        };
    };    
    typedef util::CObjKeyGenerator<> KeyGenerator;

    // IObjectAllocator
    class IObjectAllocator
    {
    public:		
        IObjectAllocator() : keygen_(NULL) 
        {
        }
        virtual void RegisterKeyGenerator(KeyGenerator* const keygen)
        {
            keygen_ = keygen;
        }
        virtual void ActionPostAlloc(Object* const object, const eOBJECT_TYPE object_type)
        {
            if (keygen_ != NULL)
            {
                const DWORD key = keygen_->GetKey();
                object->SetObjectKey(key);
            }
            else
            {
                object->SetObjectKey(1);
            }
            object->SetObjectType(object_type);
        }
        virtual void ActionPrevFree(Object* const object)
        {
            if (keygen_ != NULL)
            {
                const DWORD key = object->GetObjectKey();
                keygen_->RestoreKey(key);
            }
            object->Reuse();
        }
    private:
        KeyGenerator* keygen_;
    };

    // ObjectPoolDefault
    template <eOBJECT_TYPE kObjectType, typename OBJECT_T>
    class ObjectPoolDefault : public util::CMemoryPoolFactory<OBJECT_T>, public IObjectAllocator
    {
    public:
        ObjectPoolDefault() 
        {
        }
        virtual void* Alloc()
        {
            Object* const object = (Object*)Base::Alloc();
            IObjectAllocator::ActionPostAlloc(object, object->GetObjectType());
            return object;
        }
        virtual BOOL Free(void* node)
        {
            Object* const object = (Object*)node;
            IObjectAllocator::ActionPrevFree(object);
            return Base::Free(object);
        }
    private:
        typedef util::CMemoryPoolFactory<OBJECT_T> Base;
    };

private:
    PoolIndex::Value GetPoolIndex(const eOBJECT_TYPE object_type) const
    {
        switch (object_type)
        {	
        case PLAYER_OBJECT:		
            return PoolIndex::kPlayer;		
        case MONSTER_OBJECT:	
            return PoolIndex::kMonster;		
        case SUMMON_OBJECT:		
            return PoolIndex::kSummoned;		
        case ITEM_OBJECT:
        case MONEY_OBJECT:		
            return PoolIndex::kItem;		
        case MAP_OBJECT:		
            return PoolIndex::kMapObject;	
        case MAPNPC_OBJECT:		
            return PoolIndex::kMapNpc;		
        case MERCHANT_OBJECT:	
            return PoolIndex::kMerchant;	
        case LUCKYMON_OBJECT:	
            return PoolIndex::kLuckyMonster;
        case TOTEMNPC_OBJECT:	
            return PoolIndex::kTotemNpc;	
        case SSQMONSTER_OBJECT:	
            return PoolIndex::kSSQMonster;	
        case COLLECTION_OBJECT:	
            return PoolIndex::kCollection;	
        case CRYSTALWARP_OBJECT: 
            return PoolIndex::kCrystalWarp; 
        case FRIEND_MONSTER_OBJECT: 
            return PoolIndex::kFriendMonster; 
        case SYNC_MERCHANT_OBJECT: 
            return PoolIndex::kSyncMerchant; 
        }
        return PoolIndex::kMax;
    }

private:
    util::PoolRouter<PoolIndex::kMax> router_;
    KeyGenerator monster_keygen_;
    KeyGenerator field_item_keygen_;

}; // VariableChunkImpl


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : ObjectFactory
////////////////////////////////////////////////////////////////////////////////////////////////////

ObjectFactory::ObjectFactory()
{
}

ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::CreateImpl(const int chunk_mode)
{
    SUNLOG(
        eCRITICAL_LOG, 
        _T("|%s|%d|Creating object factory implement...|ChunkMode = %d|"), 
        __FUNCTION__, 
        __LINE__, 
        chunk_mode
    );
//#ifdef USE_FIXED_SIZE_MEMORY_POOL
    typedef FixedChunkImpl DefaultImpl;
//#else
//    typedef VariableChunkImpl DefaultImpl;
//#endif
    switch (chunk_mode)
    {
    case kVariableChunk:
        impl_.reset(new VariableChunkImpl());
        return;
    case kFixedChunk:
        impl_.reset(new FixedChunkImpl());
        return;
    default:
        impl_.reset(new DefaultImpl());
        return;
    }
}

void ObjectFactory::Init( 
    const size_t player_pool_size, 
    const size_t monster_pool_size, 
    const size_t item_pool_size, 
    const size_t map_object_Pool_size, 
    const size_t map_npc_pool_size, 
    const size_t merchant_pool_size, 
    const size_t lucky_monster_pool_size 
) {
    int chunk_mode = kDefaultChunkMode;
    {
        char path_buf[MAX_PATH] = { 0 };
        const size_t written = ::GetModuleFileName(NULL, path_buf, _countof(path_buf));
        if (written > 0)
        {
            std::string ini_path(path_buf, written);
            const size_t deli_pos = ini_path.find_last_of('\\');
            if (deli_pos > 0)
            {
                ini_path = ini_path.substr(0, deli_pos);
                ini_path += "\\MemoryPool.ini";
                chunk_mode = ::GetPrivateProfileInt(
                    "OBJECT_FACTORY", "CHUNK_MODE", kDefaultChunkMode, ini_path.c_str());
            }
        }
    }
    CreateImpl(chunk_mode);    
    return impl_->Init(
        player_pool_size, 
        monster_pool_size, 
        item_pool_size, 
        map_object_Pool_size, 
        map_npc_pool_size, 
        merchant_pool_size, 
        lucky_monster_pool_size);
}

void ObjectFactory::Release()
{
    return impl_->Release();
}

Object* ObjectFactory::AllocObject(const eOBJECT_TYPE object_type)
{
    return impl_->AllocObject(object_type);
}

bool ObjectFactory::FreeObject(Object* const object)
{
    return impl_->FreeObject(object);
}