#ifndef __QUERY_POOL_FACTORY_H__
#define __QUERY_POOL_FACTORY_H__

#pragma once



#include "QueryPoolFactoryDefine.h"

//=======================================================================================================================
/// ����Ǯ�� �����ϴ� ���丮 Ŭ����
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 10. 20
	@remarks
			- �������� Ǯ�� ���� �����Ҵ��� ���ְ� ������ ������.
			- ���ϰ� �߰��ϱ� ���� ��ũ��Ʈ�� �ۼ��ߴ�.
			- Thread-Safe�� Alloc()�� Free()
	@note
			- �߰��Ҷ��� ������ ���� ������ �߰��Ѵ�.
			  1. I_POOL_ENTRY( ���� Ŭ����, �⺻ Ǯ ����, Ȯ�� Ǯ ���� )
			  2. R_POOL_ENTITY( ���� Ŭ���� )
			  3. IMPLEMENT_POOL_ENTITY( ���� Ŭ���� )
		    - ����Ҷ��� 
			  1. �Ҵ� : QUERYALLOC( ���� Ŭ���� )
			  2. ���� : QUERYFREE( ���� Ŭ����, Ŭ���� ��ü )
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
