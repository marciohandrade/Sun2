// wzflag.h

#ifndef _PROGRAMCOMMON_WZFLAG_H_
#define _PROGRAMCOMMON_WZFLAG_H_

#include "wztypedef.h"

//------------------------------------------------------------------------------
/**
    @class CWzFlag
*/
template <class T>
class CWzFlag
{
public:
    CWzFlag( void )
    : m_flags( 0 )
    {
        // empty
    }

    ~CWzFlag( void )
    {
        // empty
    }

    void Clear( void )
    {
        m_flags = 0;
    }

    void Init( T flag )
    {
        m_flags = flag;
    }

    void Set( T flag )
    {
        m_flags |= flag;
    }

    void Unset( T flag )
    {
        m_flags &= ~flag;
    }

    void SetB( T flag, bool b )
    {
        b ? Set( flag ) : Unset( flag );
    }

    void SetB( T flag, BOOL b )
    {
        ( b != FALSE ) ? Set( flag ) : Unset( flag );
    }

    bool Check( T flag ) const
    {
        return ( ( m_flags & flag ) == flag );
    }

private:
    T m_flags;
};

#endif // _PROGRAMCOMMON_WZFLAG_H_
