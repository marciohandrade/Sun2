#include "stdafx.h"
#include "./GameShop.BitBlockContainer.h"


#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL



#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING



namespace GameShop
{


//===============================================================================================
//
// class BitBlockContainer::MemoryAllocator
//
//===============================================================================================
BitBlockContainer::MemoryAllocator::MemoryAllocator()
    : total_instanced_(0)
{

}

BitBlockContainer::MemoryAllocator::~MemoryAllocator()
{
}



void BitBlockContainer::MemoryAllocator::Initialize(uint bit_block_count, uint bit_count)
{
    __UNUSED((bit_block_count, bit_count));
    //uint base_count = sizeof(uchar) * 8; // 1byte = 8bit, 1 bit 가 1개의 flag 이다.
    //uint need_byte = (bit_count / base_count)  + ((bit_count % base_count) ? 1 : 0);
    //allocate_size_ = bit_block_count * need_byte;
    //block_size_ = need_byte;
}

void BitBlockContainer::MemoryAllocator::Release()
{
    UsedBitsBlockQueue::const_iterator it = block_memory_pool_.begin(),
                                       end = block_memory_pool_.end();
    for (; it != end; ++it)
    {
        UsedBitsBlock* used_bits_block = *it;
        TAllocDelete(UsedBitsBlock, used_bits_block);
    };

    block_memory_pool_.clear();
}

uint8_t* BitBlockContainer::MemoryAllocator::new_memory()
{
    UsedBitsBlock* used_bits_block = NULL;
    if (block_memory_pool_.empty()) {
        used_bits_block = TAllocNew(UsedBitsBlock);
    }
    else {
        used_bits_block = block_memory_pool_.back();
        block_memory_pool_.pop_back();
    };
    ZeroMemory(used_bits_block, sizeof(*used_bits_block));
    return used_bits_block->stream;
}

void BitBlockContainer::MemoryAllocator::delete_memory(uint8_t* pointer)
{
    UsedBitsBlock* used_bits_block = reinterpret_cast<UsedBitsBlock*>(pointer);
    block_memory_pool_.push_back(used_bits_block);
}


//===============================================================================================
//
// class BitBlockContainer::BitBlock
//
//===============================================================================================
BitBlockContainer::BitBlock::BitBlock()
{

}

BitBlockContainer::BitBlock::~BitBlock()
{

}

void BitBlockContainer::BitBlock::Initialize(BitBlockContainer::MemoryAllocator* memory_allocator)
{
    memory_allocator_ = memory_allocator;
    memory_point_hashmap_.clear();
}

void BitBlockContainer::BitBlock::Release()
{
    STLX_HASH_MAP<uint, uchar*>::iterator iter = memory_point_hashmap_.begin();
    for ( ; iter != memory_point_hashmap_.end() ; ++iter)
        memory_allocator_->delete_memory(iter->second);
    memory_point_hashmap_.clear();
}

const BitBlockContainer::BitBlock::Extraction& BitBlockContainer::BitBlock::extract(uint index)
{
    static Extraction result;

    uint count = sizeof(uchar) * 8;
    uint start_key = index / count;
    uchar* pointer = NULL;
    STLX_HASH_MAP<uint, uchar*>::iterator iter = memory_point_hashmap_.find(start_key);
    if (iter != memory_point_hashmap_.end())
    {
        pointer = iter->second;
    }
    else
    {
        pointer = memory_allocator_->new_memory();
        memory_point_hashmap_[start_key] = pointer;
    }

    result.pointer = pointer;
    result.position = (uchar)(((count - 1) - (index % count)));
    return result;
}




//===============================================================================================
//
// class BitBlockContainer
//
//===============================================================================================
BitBlockContainer::BitBlockContainer()
{
}

BitBlockContainer::~BitBlockContainer()
{
}

void BitBlockContainer::Initialize(uint bit_block_count, uint bit_count)
{
    memory_allocator_.Initialize(bit_block_count, bit_count);
    bit_block_pool_.Initialize(bit_block_count, bit_count/2);
    bitblock_hashmap_.clear();
}

void BitBlockContainer::Release()
{
    bit_block_pool_.Release();
    memory_allocator_.Release();
    bitblock_hashmap_.clear();
}

BitBlockContainer::BitBlock* BitBlockContainer::new_bit_block(uint key)
{
    BitBlockHashMap::iterator iter = bitblock_hashmap_.find(key);
    if (iter != bitblock_hashmap_.end())
    {
        BitBlock* bit_block = iter->second;
        bit_block->Release();
        return bit_block;
    }

    BitBlock* bit_block = (BitBlock*)bit_block_pool_.Alloc();
    bit_block->Initialize(&memory_allocator_);
    bitblock_hashmap_[key] = bit_block;
    return bit_block;
}

void BitBlockContainer::delete_bit_block(uint key)
{
    BitBlockHashMap::iterator iter = bitblock_hashmap_.find(key);
    if (iter != bitblock_hashmap_.end())
    {
        BitBlock* bit_block = iter->second;
        bit_block->Release();
        bit_block_pool_.Free(bit_block);
        bitblock_hashmap_.erase(iter);
    }
}

BitBlockContainer::BitBlock* BitBlockContainer::find_bit_block(uint key)
{
    BitBlockHashMap::iterator iter = bitblock_hashmap_.find(key);
    if (iter != bitblock_hashmap_.end())
    {
        return iter->second;
    }
    return NULL;
}


} // namespace GameShop

#endif // _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
