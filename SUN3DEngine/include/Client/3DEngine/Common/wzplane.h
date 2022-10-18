// wzplane.h

#ifndef _COMMON_WZPLANE_H_
#define _COMMON_WZPLANE_H_

#include "wzcommontypedef.h"

//------------------------------------------------------------------------------
/**
    @struct WzPlane
*/
struct WzPlane
{
    union
    {
        struct
        {
            WzVector m_wvNormal;
            float m_fD;
        };

        struct
        {
            // ax + by + cz + d = 0
            float a, b, c, d;
        };
    };

    WzPlane operator + ( const WzPlane& ) const;
    WzPlane operator - ( const WzPlane& ) const;
    WzPlane operator * ( float ) const;
};

//------------------------------------------------------------------------------
/**
*/
inline WzPlane WzPlane::operator + ( const WzPlane& rhs ) const
{
    WzPlane wpRet;

    wpRet.a = this->a + rhs.a;
    wpRet.b = this->b + rhs.b;
    wpRet.c = this->c + rhs.c;
    wpRet.d = this->d + rhs.d;

    return wpRet;
}

//------------------------------------------------------------------------------
/**
*/
inline WzPlane WzPlane::operator - ( const WzPlane& rhs ) const
{
    WzPlane wpRet;

    wpRet.a = this->a - rhs.a;
    wpRet.b = this->b - rhs.b;
    wpRet.c = this->c - rhs.c;
    wpRet.d = this->d - rhs.d;

    return wpRet;
}

//------------------------------------------------------------------------------
/**
*/
inline WzPlane WzPlane::operator * ( float val ) const
{
    WzPlane wpRet;

    wpRet.a = this->a * val;
    wpRet.b = this->b * val;
    wpRet.c = this->c * val;
    wpRet.d = this->d * val;

    return wpRet;
}

#endif // _COMMON_WZPLANE_H_