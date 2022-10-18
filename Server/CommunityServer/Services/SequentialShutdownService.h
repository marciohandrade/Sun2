#pragma once
#ifndef WORLDSERVER_SEQUENTIAL_SHUTDOWN_SERVICE_H
#define WORLDSERVER_SEQUENTIAL_SHUTDOWN_SERVICE_H

namespace ns_world {
;

class SequentialShutdownService
{
public:
    // FIFO
    static SequentialShutdownService* Register();
    // NOTE: side-effect, delete instance
    static void Shutdown(SequentialShutdownService* instance);
private:
    typedef STLX_VECTOR<ns_world::IShutdown*> ShutdownList;
    //
    SequentialShutdownService();
    ~SequentialShutdownService();
    //
    ShutdownList shutdown_list_;
    //
    __DISABLE_COPY(SequentialShutdownService);
};

}; //end of namespace

#endif //WORLDSERVER_SEQUENTIAL_SHUTDOWN_SERVICE_H