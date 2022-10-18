#pragma once


class Guild;
class GuildMember;

typedef util::SolarHashTable<GuildMember *, CHARGUID> GUILDMEMBER_HASH;

typedef util::SolarHashTable<Guild *> GUILD_HASH;

typedef STLX_HASH_MAP<std::string, GUILDGUID> GUILD_NAME_HASH;

//<�������� ���ڵ�, ������ ���� ����>
typedef STLX_MULTIMAP<MAPCODE, DOMINATION_INFO> AreaInfoMap;
//<���� ���� ���ڵ�, ������ ��� ���� �ε� üũ ����>
typedef STLX_HASH_MAP<MAPCODE, bool> AuctionInfoLoadingCheckMap;
//<���� ���� ���ڵ�, ������ �ɹ� ����Ʈ �ε� üũ ����>
typedef STLX_HASH_MAP<MAPCODE, bool> MemberLoadCheckMap;

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY 
    //<��� ����>
    typedef STLX_LIST<DominationAuctionInfo> DominationAuctionInfoList;
    //<���� �� �ڵ�, ������ ��� ����Ʈ>
    typedef STLX_HASH_MAP<MAPCODE, DominationAuctionInfoList> AllAreaAuctionInfoMap;
#else
    //<�����ݾ�(Ű��), ��� ����>
    typedef STLX_MULTIMAP<MONEY, DominationAuctionInfo> DominationAuctionInfoMap;
    //<���� ���� ���ڵ�, ������ ��� ���� ��>
    typedef STLX_HASH_MAP<MAPCODE, DominationAuctionInfoMap> AllAreaAuctionInfoMap;
#endif

typedef STLX_MULTIMAP<MAPCODE, DominationMemberInfo> MemberMap;

//--------------------------------------------------------------------------------
//s GUILDPOOL�� �޸� Ǯ ���� �ڵ�
//--------------------------------------------------------------------------------
#include <MemoryPoolFactory.h>

template<class _Ct>
class GUILDPOOL
{
public:
	GUILDPOOL(){ m_Pool.Initialize(20, 20, "GUILDPOOL"); }
	~GUILDPOOL(){ m_Pool.Release();	}
	_Ct * Alloc() { return (_Ct*)m_Pool.Alloc(); }
	BOOL Free( _Ct * p ) { return m_Pool.Free(p); }
private:
	util::CMemoryPoolFactory<_Ct> m_Pool;
};


#define __DECL_GUILDPOOL(_ClassType)									\
	private:															\
	static GUILDPOOL<_ClassType> s_Pool;								\
	public:																\
	static _ClassType *	ALLOC() { return s_Pool.Alloc(); }				\
	static VOID FREE( _ClassType * pClass ) { s_Pool.Free( pClass ); }

#define __IMPL_GUILDPOOL(_ClassType)									\
	GUILDPOOL<_ClassType> _ClassType::s_Pool;
//--------------------------------------------------------------------------------
//e GUILDPOOL�� �޸� Ǯ ���� �ڵ�
//--------------------------------------------------------------------------------