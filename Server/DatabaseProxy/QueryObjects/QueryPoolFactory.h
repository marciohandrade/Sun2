#ifndef __QUERY_POOL_FACTORY_H__
#define __QUERY_POOL_FACTORY_H__

#pragma once



#include "QueryPoolFactoryDefine.h"

//=======================================================================================================================
/// 쿼리풀을 관리하는 팩토리 클래스
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 10. 20
	@remarks
			- 쿼리들을 풀로 만들어서 동적할당을 없애고 성능을 높였다.
			- 편리하게 추가하기 위해 스크립트로 작성했다.
			- Thread-Safe한 Alloc()과 Free()
	@note
			- 추가할때는 다음과 같은 순으로 추가한다.
			  1. I_POOL_ENTRY( 쿼리 클래스, 기본 풀 개수, 확장 풀 개수 )
			  2. R_POOL_ENTITY( 쿼리 클래스 )
			  3. IMPLEMENT_POOL_ENTITY( 쿼리 클래스 )
		    - 사용할때는 
			  1. 할당 : QUERYALLOC( 쿼리 클래스 )
			  2. 해제 : QUERYFREE( 쿼리 클래스, 클래스 객체 )
    @history
        - ~2011.04.29, custom version
        - 2011.04.29, waverix, changes to more simple registering methodology
*/
//==================================================================================================

#include <ITimeout.h>
#include <SafeMemoryPoolFactory.h>

//==================================================================================================
//
namespace nsQuery {
;

class QueryPoolFactory
{
public:
    template<class QueryPoolType>
    static bool RegisterPool(
        QueryPoolType** query_type_holder, const char* type_name_string);
    static bool CheckPoolAlarm();
private:
    static const int kAlarmCheckInterval;
    struct AlarmNode : public util::__LList<AlarmNode>
    {
        util::SafeMemoryPoolFactoryShared* pool_base;
    };
    //
    QueryPoolFactory();
    ~QueryPoolFactory();
    //
    static bool RegisterInternal(util::SafeMemoryPoolFactoryShared* pool_base,
        const char* type_name_string);
    //
    util::__LList<AlarmNode> root_;
    util::ITimerBase alarm_check_timer_;
    static QueryPoolFactory* instance_reference_;
};

//==================================================================================================
}; //end of namespace
//==================================================================================================

template<class QueryPoolType>
inline bool nsQuery::QueryPoolFactory::RegisterPool(
    QueryPoolType** query_type_holder, const char* type_name_string)
{
    static QueryPoolType query_pool_type;
    *query_type_holder = &query_pool_type;
    return RegisterInternal(&query_pool_type, type_name_string);
}

//==================================================================================================


#endif // __QUERY_POOL_FACTORY_H__
