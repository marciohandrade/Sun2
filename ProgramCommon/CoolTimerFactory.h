#pragma once
#ifndef PROGRAMCOMMON_COOL_TIMER_FACTORY_H
#define PROGRAMCOMMON_COOL_TIMER_FACTORY_H

//==================================================================================================
// NOTE: f110513.4L, this factory can't collect all tokens after releasing process for that reason,
// waverix is writer decided to maintain the factory containing token list.
class CoolTimerFactory
{
public:
    static CoolTimerFactory* Instance();

    util::Timer* Alloc();
    void Free(util::Timer* timer);

    void Init(DWORD chunking_pool_size);
    void Release();

    void DisplayPoolInfo();
private:
    CoolTimerFactory();
    virtual ~CoolTimerFactory();
    static CoolTimerFactory* CreateInstance();
    //
    util::CMemoryPoolFactory<util::Timer>* cool_timer_pool_;
    //
    static CoolTimerFactory* instance_static_;
};

inline CoolTimerFactory* CoolTimerFactory::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

#endif //PROGRAMCOMMON_COOL_TIMER_FACTORY_H