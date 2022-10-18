#include "WorldServerPch.hxx"
#include "./SequentialShutdownService.h"

#include "Objects/WorldUserManager.h"
#include "Channel/WorldChannelManager.h"

//==================================================================================================

ns_world::SequentialShutdownService::SequentialShutdownService()
{
}

ns_world::SequentialShutdownService::~SequentialShutdownService()
{
}

ns_world::SequentialShutdownService* ns_world::SequentialShutdownService::Register()
{
    SequentialShutdownService* instance = TAllocNew(ns_world::SequentialShutdownService);

    ShutdownList& shutdown_list = instance->shutdown_list_;
    shutdown_list.reserve(10);

    shutdown_list.push_back(ns_world::WorldUserManager::Instance());
    shutdown_list.push_back(ns_world::WorldChannelManager::Instance());

    return instance;
}

void ns_world::SequentialShutdownService::Shutdown(
    SequentialShutdownService* instance)
{
    ShutdownList& shutdown_list = instance->shutdown_list_;

    ShutdownList::const_iterator it = shutdown_list.begin();
    const ShutdownList::const_iterator end = shutdown_list.end();
    for (; it != end; ++it)
    {
        ns_world::IShutdown* shutdown_instance = *it;
        shutdown_instance->Shutdown();
    };

    TAllocDelete(ns_world::SequentialShutdownService, instance);
}


