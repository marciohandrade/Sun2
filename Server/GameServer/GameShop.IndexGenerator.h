#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL



#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#ifndef SOLAR_SHOP_INDEXGENERATOR_H_
#define SOLAR_SHOP_INDEXGENERATOR_H_


#if _MSC_VER > 1000
#pragma once
#endif


//===============================================================================================
//
// class GameShop.IndexGenerator
//
//===============================================================================================
namespace GameShop
{

class IndexGenerator
{
public:
    IndexGenerator();
    ~IndexGenerator();

    uint16_t new_index();
    void delete_index(uint16_t index);
    void clear();

private:
    STLX_DEQUE<uint16_t> destroyed_index_;
    uint16_t next_index_;
};

} //namespace GameShop


#endif // SOLAR_SHOP_INDEXGENERATOR_H_

#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
