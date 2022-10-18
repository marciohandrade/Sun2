// wzuvvert.h

#ifndef _COMMON_WZUVVERT_H_
#define _COMMON_WZUVVERT_H_

#include "wzcommontypedef.h"

//------------------------------------------------------------------------------
/**
    @struct WzUVVert
*/
struct WzUVVert
{
    union
    {
        struct
        {
            float u, v;
        };

        float m_Element[2];
    };

    BOOL operator == ( const WzUVVert& ) const;
    BOOL operator != ( const WzUVVert& ) const;
};

//------------------------------------------------------------------------------
/**
*/
inline BOOL WzUVVert::operator == ( const WzUVVert& rhs ) const
{
    if( m_Element[0] != rhs.m_Element[0] ||
        m_Element[1] != rhs.m_Element[1] )
    {
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL WzUVVert::operator != ( const WzUVVert& rhs ) const
{
    return !( *this == rhs );
}

#endif // _COMMON_WZUVVERT_H_