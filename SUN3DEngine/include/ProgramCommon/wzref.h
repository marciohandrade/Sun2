// wzref.h

#ifndef _PROGRAMCOMMON_WZREF_H_
#define _PROGRAMCOMMON_WZREF_H_

#include "wztypedef.h"

//------------------------------------------------------------------------------
/**
    @class CWzRef
*/
template<class T>
class CWzRef
{
public:
    CWzRef( void )
    {
        Init();
    }

    void Init( void )
    {
        m_ref = NULL;
        m_obj = NULL;
    }

    void Set( T* obj )
    {
        m_obj = obj;
        m_ref = NULL;
    }

    void SetRef( CWzRef* ref )
    {
        WzAssert( !m_obj );

        m_ref = ref;
        m_obj = NULL;
    }

    T* Get( void ) const
    {
        return ( m_ref ? m_ref->m_obj : m_obj );
    }

    T* operator -> ( void ) const
    {
#ifdef _DEBUG
        T* obj = Get();
        WzAssert( obj );

        return obj;
#else
        return Get();
#endif
    }

    CWzRef& operator = ( T* obj )
    {
        Set( obj );

        return *this;
    }

    operator T* ( void ) const
    {
        return m_obj;
    }

private:
    CWzRef* m_ref;
    T* m_obj;
};

#endif // _PROGRAMCOMMON_WZREF_H_
