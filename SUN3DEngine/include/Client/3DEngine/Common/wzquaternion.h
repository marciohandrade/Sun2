// wzquaternion.h

#ifndef _COMMON_WZQUATERNION_H_
#define _COMMON_WZQUATERNION_H_

#include "wzcommontypedef.h"

struct WzQuaternion;

WzQuaternion* QuaternionMultiply( WzQuaternion* pwqOut, const WzQuaternion* pwq1, const WzQuaternion* pwq2 );

//------------------------------------------------------------------------------
/**
    @struct WzQuaternion
*/
struct WzQuaternion
{
    union
    {
        struct
        {
            float x, y, z, w;
        };

        float m_Element[4];
    };

    BOOL operator == ( const WzQuaternion& ) const;
    BOOL operator != ( const WzQuaternion& ) const;
    WzQuaternion operator * ( const WzQuaternion& ) const;
};

//------------------------------------------------------------------------------
/**
*/
inline BOOL WzQuaternion::operator == ( const WzQuaternion& rhs ) const
{
    if( m_Element[0] != rhs.m_Element[0] ||
        m_Element[1] != rhs.m_Element[1] ||
        m_Element[2] != rhs.m_Element[2] ||
        m_Element[3] != rhs.m_Element[3] )
    {
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL WzQuaternion::operator != ( const WzQuaternion& rhs ) const
{
    return !( *this == rhs );
}

//------------------------------------------------------------------------------
/**
*/
inline WzQuaternion WzQuaternion::operator * ( const WzQuaternion& rhs ) const
{
    WzQuaternion wqRet;

    QuaternionMultiply( &wqRet, this, &rhs );

    return wqRet;
}

#endif // _COMMON_WZQUATERNION_H_