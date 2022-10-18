// wzmatrix.h

#ifndef _COMMON_WZMATRIX_H_
#define _COMMON_WZMATRIX_H_

#include "wzcommontypedef.h"

//------------------------------------------------------------------------------
/**
    @struct WzMatrix
*/
struct WzMatrix
{
    union
    {
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };

        float m_Element[4][4];
    };

    BOOL operator == ( const WzMatrix& ) const;
    BOOL operator != ( const WzMatrix& ) const;
    WzMatrix operator + () const;
    WzMatrix operator - () const;
    WzMatrix& operator += ( const WzMatrix& );
    WzMatrix& operator -= ( const WzMatrix& );
    WzMatrix operator + ( const WzMatrix& ) const;
    WzMatrix operator - ( const WzMatrix& ) const;
    WzMatrix operator * ( float ) const;
};

//------------------------------------------------------------------------------
/**
*/
inline BOOL WzMatrix::operator == ( const WzMatrix& rhs ) const
{
    for( int r = 0; r < 4; ++r )
    {
        for( int c = 0; c < 4; ++c )
        {
            if( m_Element[r][c] != rhs.m_Element[r][c] )
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL WzMatrix::operator != ( const WzMatrix& rhs ) const
{
    return !( *this == rhs );
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix WzMatrix::operator + () const
{
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix WzMatrix::operator - () const
{
    WzMatrix wmRet;

    for( int r = 0; r < 4; ++r )
    {
        for( int c = 0; c < 4; ++c )
        {
            wmRet.m_Element[r][c] = -m_Element[r][c];
        }
    }

    return wmRet;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix& WzMatrix::operator += ( const WzMatrix& rhs )
{
    for( int r = 0; r < 4; ++r )
    {
        for( int c = 0; c < 4; ++c )
        {
            m_Element[r][c] += rhs.m_Element[r][c];
        }
    }

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix& WzMatrix::operator -= ( const WzMatrix& rhs )
{
    for( int r = 0; r < 4; ++r )
    {
        for( int c = 0; c < 4; ++c )
        {
            m_Element[r][c] -= rhs.m_Element[r][c];
        }
    }

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix WzMatrix::operator + ( const WzMatrix& rhs ) const
{
    WzMatrix wmRet;

    for( int r = 0; r < 4; ++r )
    {
        for( int c = 0; c < 4; ++c )
        {
            wmRet.m_Element[r][c] = m_Element[r][c] + rhs.m_Element[r][c];
        }
    }

    return wmRet;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix WzMatrix::operator - ( const WzMatrix& rhs ) const
{
    WzMatrix wmRet;

    for( int r = 0; r < 4; ++r )
    {
        for( int c = 0; c < 4; ++c )
        {
            wmRet.m_Element[r][c] = m_Element[r][c] - rhs.m_Element[r][c];
        }
    }

    return wmRet;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix WzMatrix::operator * ( float val ) const
{
    WzMatrix wmRet;

    for( int r = 0; r < 4; ++r )
    {
        for( int c = 0; c < 4; ++c )
        {
            wmRet.m_Element[r][c] = m_Element[r][c] * val;
        }
    }

    return wmRet;
}

#endif // _COMMON_WZMATRIX_H_