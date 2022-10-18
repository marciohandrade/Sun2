#pragma once
#ifndef WORLDSERVER_WORLD_GUILD_COMMON_H
#define WORLDSERVER_WORLD_GUILD_COMMON_H

//--------------------------------------------------------------------------------
// GUILDPOOL의 메모리 풀 관련 코드
//--------------------------------------------------------------------------------

namespace ns_world {
;

template<class _Ct>
class GUILDPOOL
{
public:
    GUILDPOOL() { m_Pool.Initialize(20, 20, "GUILDPOOL"); }
    ~GUILDPOOL() { m_Pool.Release(); }
    _Ct* Alloc() { return (_Ct*)m_Pool.Alloc(); }
    BOOL Free(_Ct* p) { return m_Pool.Free(p); }
private:
    util::CMemoryPoolFactory<_Ct> m_Pool;
};

}; //end of namespace

//==================================================================================================

#define __DECL_GUILDPOOL(_ClassType) \
    private: \
        static ns_world::GUILDPOOL<_ClassType> s_Pool; \
    public: \
        static _ClassType* ALLOC() { return s_Pool.Alloc(); } \
        static void FREE(_ClassType* pClass) { s_Pool.Free(pClass); }

#define __IMPL_GUILDPOOL(_ClassType) \
    ns_world::GUILDPOOL<_ClassType> _ClassType::s_Pool;
//--------------------------------------------------------------------------------
//e GUILDPOOL의 메모리 풀 관련 코드
//--------------------------------------------------------------------------------

//==================================================================================================

#endif //WORLDSERVER_WORLD_GUILD_COMMON_H