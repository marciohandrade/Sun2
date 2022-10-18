#pragma once


class Guild;
class GuildMember;

typedef util::SolarHashTable<GuildMember *, CHARGUID> GUILDMEMBER_HASH;

typedef util::SolarHashTable<Guild *> GUILD_HASH;

typedef STLX_HASH_MAP<std::string, GUILDGUID> GUILD_NAME_HASH;

//<지역점령 맵코드, 점령전 지역 정보>
typedef STLX_MULTIMAP<MAPCODE, DOMINATION_INFO> AreaInfoMap;
//<지역 점령 맵코드, 지역별 경매 정보 로드 체크 유무>
typedef STLX_HASH_MAP<MAPCODE, bool> AuctionInfoLoadingCheckMap;
//<지역 점령 맵코드, 지역별 맴버 리스트 로드 체크 유무>
typedef STLX_HASH_MAP<MAPCODE, bool> MemberLoadCheckMap;

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY 
    //<경매 정보>
    typedef STLX_LIST<DominationAuctionInfo> DominationAuctionInfoList;
    //<점령 맵 코드, 지역별 경매 리스트>
    typedef STLX_HASH_MAP<MAPCODE, DominationAuctionInfoList> AllAreaAuctionInfoMap;
#else
    //<입찰금액(키값), 경매 정보>
    typedef STLX_MULTIMAP<MONEY, DominationAuctionInfo> DominationAuctionInfoMap;
    //<지역 점령 맵코드, 지역별 경매 정보 맵>
    typedef STLX_HASH_MAP<MAPCODE, DominationAuctionInfoMap> AllAreaAuctionInfoMap;
#endif

typedef STLX_MULTIMAP<MAPCODE, DominationMemberInfo> MemberMap;

//--------------------------------------------------------------------------------
//s GUILDPOOL의 메모리 풀 관련 코드
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
//e GUILDPOOL의 메모리 풀 관련 코드
//--------------------------------------------------------------------------------