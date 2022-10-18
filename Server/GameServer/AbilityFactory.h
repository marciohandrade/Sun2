#pragma once

#ifndef GAMESERVER_ABILITY_FACTORY_H_
#define GAMESERVER_ABILITY_FACTORY_H_

#include <boost/scoped_ptr.hpp>


class AbilityFactory
{
private:
    AbilityFactory();
    ~AbilityFactory();

public:
    static AbilityFactory* Instance()
    {
        static AbilityFactory instance;
        return &instance;
    }
    void Init(const DWORD pool_size);
    Ability* AllocAbility(const WORD ability_id, const eSKILL_TYPE skill_type);
    void FreeAbility(Ability* const ability);

private:
    class Allocator;
    boost::scoped_ptr<Allocator> allocator_;
};

#endif // GAMESERVER_ABILITY_FACTORY_H_