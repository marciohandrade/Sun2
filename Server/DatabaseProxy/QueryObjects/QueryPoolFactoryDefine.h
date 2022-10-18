#ifndef __QUERY_POOL_FACTORY_DEFINE_H__
#define __QUERY_POOL_FACTORY_DEFINE_H__

//==================================================================================================
// NOTE: the below section is used to correctly fit a query reordering sequence binding
#define _REORDERING_PARAM(field_name, _PARAMETER_TYPE_) \
    _BINDING_PARAM(_REORDERING_PARAM_NO_##field_name, field_name, _PARAMETER_TYPE_)

#define _REORDERING_PARAM_PTR(field_name, _PARAMETER_TYPE_) \
    _BINDING_PARAM_PTR(_REORDERING_PARAM_NO_##field_name, field_name, _PARAMETER_TYPE_)

// temporary
#define _REORDERING_PARAM_DUMMY(field_name, _PARAMETER_TYPE_) \
    _BINDING_PARAM(_REORDERING_PARAM_NO_##field_name, _dummy_field_, _PARAMETER_TYPE_)

#define _MAKE_REORDERING_PARAM_NO_START_(field_name, number) \
    _REORDERING_PARAM_NO_##field_name = (number)

#define _MAKE_REORDERING_PARAM_NO_(field_name) \
    _REORDERING_PARAM_NO_##field_name

#define _MAKE_REORDERING_PARAM_NO_PTR_(field_name) \
    _REORDERING_PARAM_NO_##field_name

#define _MAKE_REORDERING_PARAM_NO_COUNTS_() \
    _REORDERING_PARAM_NO_MAX_COUNTS

//--------------------------------------------------------------------------------------------------

#define _REORDERING_BINDING_COLUMN(field_name) \
    _BINDING_COLUMN(_REORDERING_BINDING_NO_##field_name, field_name)

#define _REORDERING_BINDING_COLUMN_PTR(field_name) \
    _BINDING_COLUMN_PTR(_REORDERING_BINDING_NO_##field_name, field_name)

// temporary
#define _REORDERING_BINDING_COLUMN_DUMMY(field_name) \
    _BINDING_COLUMN(_REORDERING_BINDING_NO_##field_name, _dummy_field_)

#define _MAKE_REORDERING_BINDING_NO_START_(field_name, number) \
    _REORDERING_BINDING_NO_##field_name = (number)

#define _MAKE_REORDERING_BINDING_NO_(field_name) \
    _REORDERING_BINDING_NO_##field_name
// to compatible
#define _MAKE_REORDERING_BINDING_NO_PTR_(field_name) \
    _REORDERING_BINDING_NO_##field_name

#define _MAKE_REORDERING_BINDING_NO_COUNTS_() \
    _REORDERING_BINDING_NO_MAX_COUNTS

//--------------------------------------------------------------------------------------------------
/*
// samples
{
    ...;
private:
    enum {
        _MAKE_REORDERING_PARAM_NO_START_(param_name#1, [start_no]),
    #ifdef _CONTENT_PRE_PROCESSOR_
        _MAKE_REORDERING_PARAM_NO_(param_name#2),
    #endif
        ...,
        _MAKE_REORDERING_PARAM_NO_COUNTS_() // end of records. it equal to number of records
    };
    enum {
        _MAKE_REORDERING_BINDING_NO_START_(field_name#1, [start_no]),
        _MAKE_REORDERING_BINDING_NO_(field_name#2),
    #ifdef _CONTENT_PRE_PROCESSOR_
        _MAKE_REORDERING_BINDING_NO_(field_name#3),
    #endif
        ...,
        _MAKE_REORDERING_BINDING_NO_COUNTS_() // end of records. it equal to number of records
    };
public:
    enum {
        ...,
        PARAM_COL_NUM  = _MAKE_REORDERING_PARAM_NO_COUNTS_(),
        ...,
        RESULT_COL_NUM = _MAKE_REORDERING_BINDING_NO_COUNTS_(),
    };

    _BEGIN_PARAM_DATA(...)
        _REORDERING_PARAM(param_name#1, 0, SQL_PARAM_OUTPUT)
        _REORDERING_PARAM(param_name#2, 0, SQL_PARAM_INPUT)
        _REORDERING_PARAM(param_name#3, 0, SQL_PARAM_INPUT)
        ...
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(...)
        _REORDERING_BINDING_COLUMN(field_name#1) -or-
        _REORDERING_BINDING_COLUMN_PTR(field_name#2)
        ...
    _END_BINDING_DATA();
};
*/

//==================================================================================================

namespace nsQuery {};

#define __DECL_SAFECUSTOMPOOL_PTR(_CLASS_NAME_) \
    private: \
        typedef util::SafeMemoryPoolFactory<_CLASS_NAME_ >* query_pool_type; \
        static query_pool_type query_pool_static; \
        static bool pool_register_static; \
    public: \
        static _CLASS_NAME_* ALLOC(); \
        static void FREE(_CLASS_NAME_* query_ptr); \
    public:

#define __IMPL_SAFECUSTOMPOOL_PTR(_CLASS_NAME_) \
    _CLASS_NAME_::query_pool_type _CLASS_NAME_::query_pool_static; \
    bool _CLASS_NAME_::pool_register_static = ::nsQuery::QueryPoolFactory::RegisterPool(\
        &_CLASS_NAME_::query_pool_static, #_CLASS_NAME_); \
    _CLASS_NAME_* _CLASS_NAME_::ALLOC() { return query_pool_static->Alloc(); } \
    void _CLASS_NAME_::FREE(_CLASS_NAME_* query_ptr) { query_pool_static->Free(query_ptr); }

//==================================================================================================


#endif // __QUERY_POOL_FACTORY_DEFINE_H__