#include "stdafx.h"
#include "./QueryPoolFactory.h"

//==================================================================================================

const int nsQuery::QueryPoolFactory::kAlarmCheckInterval = 60 * 1000;

nsQuery::QueryPoolFactory* nsQuery::QueryPoolFactory::instance_reference_ = NULL;

nsQuery::QueryPoolFactory::QueryPoolFactory()
{
    util::LList::Init(&root_);
    //
    alarm_check_timer_.SetTimer(kAlarmCheckInterval);
}

nsQuery::QueryPoolFactory::~QueryPoolFactory()
{
    for (AlarmNode* it = root_.next; it != &root_;)
    {
        AlarmNode* node = it;
        it = util::LList::Delete(it);
        TAllocDelete(AlarmNode, node);
    };
}

bool nsQuery::QueryPoolFactory::RegisterInternal(
    util::SafeMemoryPoolFactoryShared* pool_base,
    const char* type_name_string)
{
    static QueryPoolFactory factory_instance;
    instance_reference_ = &factory_instance;

    pool_base->Initialize(20);
    pool_base->SetName(type_name_string);

    AlarmNode* alloc_node = TAllocNew(AlarmNode);
    util::LList::Init(alloc_node);
    alloc_node->pool_base = pool_base;
    util::LList::AddNext(alloc_node, &instance_reference_->root_);

    return true;
}

bool nsQuery::QueryPoolFactory::CheckPoolAlarm()
{
    nsQuery::QueryPoolFactory* const this_ = instance_reference_;
    if (this_->alarm_check_timer_.IsExpired() == false) {
        return true;
    };
    //
    for (AlarmNode* it = this_->root_.next; it != &this_->root_; it = it->next)
    {
        util::SafeMemoryPoolFactoryShared* const pool_base = it->pool_base;
        ulong number_of_nodes = pool_base->GetNumberOfBands();
        if (bool no_check_ranges = (number_of_nodes < 1000)) {
            continue;
        };
        ulong number_of_free = pool_base->GetAvailableNumberOfTypes();
        int percentage = (number_of_free * 100) / number_of_nodes;
        if (percentage < 25) {
            SUNLOG(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|")
                   _T("Msg=warnings, too many chunked pool and, too many allocated|PoolName=%s|"),
                   pool_base->GetName());
        };
    };
    return true;
}