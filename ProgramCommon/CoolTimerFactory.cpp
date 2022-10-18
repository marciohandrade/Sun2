#include "stdafx.h"
#include "./CoolTimerFactory.h"

//==================================================================================================

CoolTimerFactory* CoolTimerFactory::instance_static_ = NULL;

CoolTimerFactory* CoolTimerFactory::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(CoolTimerFactory** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        CoolTimerFactory** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    CoolTimerFactory* provider = new CoolTimerFactory;
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

//==================================================================================================

CoolTimerFactory::CoolTimerFactory()
    : cool_timer_pool_(new util::CMemoryPoolFactory<util::Timer>)
{
    cool_timer_pool_->Initialize(100);
}

CoolTimerFactory::~CoolTimerFactory()
{
    SAFE_DELETE(cool_timer_pool_);
}

void CoolTimerFactory::Release()
{
    if (cool_timer_pool_) {
        cool_timer_pool_->Release();
    };
}

void CoolTimerFactory::Init(DWORD chunking_pool_size)
{
    // nothing to do
    __UNUSED(chunking_pool_size);
}

util::Timer* CoolTimerFactory::Alloc()
{
    return reinterpret_cast<util::Timer*>(cool_timer_pool_->Alloc());
}

void CoolTimerFactory::Free(util::Timer* timer)
{
    cool_timer_pool_->Free(timer);
}

void CoolTimerFactory::DisplayPoolInfo()
{
    SUNLOG(eCRITICAL_LOG, _T("[%4u,%4u][band:%u node:%u] CoolTimePool\n"),
        cool_timer_pool_->GetPoolBasicSize(), cool_timer_pool_->GetPoolExtendSize(), 
        cool_timer_pool_->GetNumberOfBands(), cool_timer_pool_->GetAvailableNumberOfTypes());
}
