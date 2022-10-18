// wznaturalcubicsplinedyn.h

#ifndef _COMMON_WZNATURALCUBICSPLINEDYN_H_
#define _COMMON_WZNATURALCUBICSPLINEDYN_H_

#include "wzcommontypedef.h"
#include <vector>

//------------------------------------------------------------------------------
/**
    @class WzNaturalCubicSplineDyn
*/
class WzNaturalCubicSplineDyn
{
public:
    enum
    {
        TYPE_DEF = -1,
        TYPE_SPLINE = 0,
        TYPE_LINE = 1
    };

    WzNaturalCubicSplineDyn();
    ~WzNaturalCubicSplineDyn();

    bool Create( int type = TYPE_SPLINE );
    void Destroy();

    void Add( float t, float y );
    int Calculate( float* out, float t );

    int GetCount();
    float GetValue( int idx );

private:
    struct _WZNCSPT
    {
        float t;
        float y;
    };

    int _type;      // 타입(-1 : 기본, 0 : 곡선, 1 : 직선)

    std::vector<_WZNCSPT> _pts;

    float* _a;      // ai = ti+1 - ti
    float* _b;      // bi = 2(ai + ai+1)
    float* _r;      // ri = 6((yi+2 - yi+1) / ai+2) - (yi+1 - yi) / ai)
    float* _x;
};

//------------------------------------------------------------------------------
/**
*/
inline int WzNaturalCubicSplineDyn::GetCount()
{
    return (int)_pts.size();
}

//------------------------------------------------------------------------------
/**
*/
inline float WzNaturalCubicSplineDyn::GetValue( int idx )
{
    if( idx >= 0 && idx < (int)_pts.size() )
    {
        return _pts[idx].y;
    }

    return 0.0f;
}

#endif // _COMMON_WZNATURALCUBICSPLINEDYN_H_
