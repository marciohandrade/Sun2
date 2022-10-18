#include "stdafx.h"
#include "GameShop.IndexGenerator.h"


#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL



#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


//===============================================================================================
//
// class GameShop.IndexGenerator
//
//===============================================================================================
namespace GameShop
{


IndexGenerator::IndexGenerator()
: next_index_(0)
{
    destroyed_index_.clear();
}

IndexGenerator::~IndexGenerator()
{

}

void IndexGenerator::clear()
{
    next_index_ = 0;
    destroyed_index_.clear();
}

uint16_t IndexGenerator::new_index()
{
    uint16_t result = ~(0);
    if (!destroyed_index_.empty())
    {
        result = destroyed_index_.front();
        destroyed_index_.pop_front();
    }
    else
    {
        if ((next_index_+1) < result)
        {
            result = next_index_;
            ++next_index_;
        }
    }
    return result;
}

void IndexGenerator::delete_index(uint16_t index)
{
    ASSERT(index < next_index_);
    if (index < next_index_)
    {
        destroyed_index_.push_back(index);
    }
}

} //namespace GameShop



#endif // _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
