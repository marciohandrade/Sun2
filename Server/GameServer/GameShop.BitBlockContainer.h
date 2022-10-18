#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL


#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#ifndef SUN_GAMESERVER_GAMESHOP_BITBLOCKCONTAINER_H_
#define SUN_GAMESERVER_GAMESHOP_BITBLOCKCONTAINER_H_


#if _MSC_VER > 1000
#pragma once
#endif



//===============================================================================================
//
// class ShopCacheSystem
//
//===============================================================================================
namespace GameShop
{
class BitBlockContainer
{
private:
    class MemoryAllocator
    {
    public:
        MemoryAllocator();
        ~MemoryAllocator();
        void Initialize(uint bit_block_count, uint bit_count);
        void Release();

        uint8_t* new_memory();
        void delete_memory(uint8_t* pointer);

    private:
        // NOTE: namka199, 1 bit 가 1개의 flag 이다
        static const size_t kMaxNumberOfUsedBits = 300;
        static const size_t kUsedBytes = (kMaxNumberOfUsedBits / 8) +
                                         ((kMaxNumberOfUsedBits % 8) ? 1 : 0);
        struct UsedBitsBlock
        {
            uint8_t stream[kUsedBytes];
        };
        typedef STLX_DEQUE<UsedBitsBlock*> UsedBitsBlockQueue;
        //
        size_t total_instanced_;
        UsedBitsBlockQueue block_memory_pool_;
    };

public:
    class BitBlock
    {
    public:
        BitBlock();
        ~BitBlock();

        void Initialize(MemoryAllocator* memory_allocator);
        void Release();

        bool is_on(uint index)
        {
            const Extraction& result =  extract(index);
            return (*(result.pointer) & (1 << result.position)) ? true : false;
        }
        void flag_on(uint index)
        {
            const Extraction& result =  extract(index);
            *(result.pointer) |= (1 << result.position);
        }
        void flag_off(uint index)
        {
            const Extraction& result =  extract(index);
            *(result.pointer) &= ~(1 << result.position);
        }

    private:
        struct Extraction
        {
            uchar* pointer;
            uchar position;
        };

        const Extraction& extract(uint index);
        STLX_HASH_MAP<uint, uchar*> memory_point_hashmap_;
        MemoryAllocator* memory_allocator_;
    };

public:
    BitBlockContainer();
    ~BitBlockContainer();

    void Initialize(uint bit_block_count, uint bit_count);
    void Release();

    BitBlock* new_bit_block(uint key);
    void delete_bit_block(uint key);
    BitBlock* find_bit_block(uint key);

private:
    MemoryAllocator memory_allocator_;
    util::CMemoryPoolFactory<BitBlock> bit_block_pool_;

    typedef STLX_HASH_MAP<uint, BitBlock*> BitBlockHashMap;
    BitBlockHashMap bitblock_hashmap_;
};




}; //namespace GameShop



#endif // SUN_GAMESERVER_GAMESHOP_BITBLOCKCONTAINER_H_

#endif // _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
