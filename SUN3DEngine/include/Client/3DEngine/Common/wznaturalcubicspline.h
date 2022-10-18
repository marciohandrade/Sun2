// wznaturalcubicspline.h

#ifndef _COMMON_WZNATURALCUBICSPLINE_H_
#define _COMMON_WZNATURALCUBICSPLINE_H_

#include "wzcommontypedef.h"

//------------------------------------------------------------------------------
/**
    @class WzNaturalCubicSpline
*/
class WzNaturalCubicSpline
{
public:
    enum
    {
        TYPE_DEF = -1,
        TYPE_SPLINE = 0,
        TYPE_LINE = 1
    };

    WzNaturalCubicSpline();
    ~WzNaturalCubicSpline();

    bool Create( int type = TYPE_SPLINE );
    void Destroy();

    void Add( float t, float y );
    int Calculate( float* out, float t );

    int GetCount();
    float GetValue( int idx );

private:
    enum
    {
        MAX_PTS = 32,
        MAX_FNS = 16
    };

    int _type;              // Ÿ��(-1 : �⺻, 0 : �, 1 : ����)

    int _n;                 // �Է� �� ��
    float _t[MAX_PTS];      // �Է� �� �ð�
    float _y[MAX_PTS];      // �Է� �� ��(t�ð������� ��)

    float _a[MAX_FNS - 1];  // ai = ti+1 - ti
    float _b[MAX_FNS - 2];  // bi = 2(ai + ai+1)
    float _r[MAX_FNS - 2];  // ri = 6((yi+2 - yi+1) / ai+2) - (yi+1 - yi) / ai)
    float _x[MAX_FNS];
};

//------------------------------------------------------------------------------
/**
*/
inline int WzNaturalCubicSpline::GetCount()
{
    return _n;
}

//------------------------------------------------------------------------------
/**
*/
inline float WzNaturalCubicSpline::GetValue( int idx )
{
    if( idx >= 0 && idx < _n )
    {
        return _y[idx];
    }

    return 0.0f;
}

#endif // _COMMON_WZNATURALCUBICSPLINE_H_
