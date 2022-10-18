#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//=============================================================================================================================
// Global header files
//	- 공용으로 사용 할 수 있는 전역 함수들의 모임
//=============================================================================================================================


#include "Const.h"
#include "Macro.h"

#include <string>
struct WzVector;

eCLASS_TYPE CharTypeToClassType(const eCHAR_TYPE char_type);

std::string ToString(const WzVector& vec);

template< typename Vector >
inline VOID RandomRangeVector( Vector * pVec, INT range )
{
	++range;
	pVec->x = pVec->x + (random(0,1)==0?-1.0f:1.0f)*(float)dRandom( 0, range );
	pVec->y = pVec->y + (random(0,1)==1?-1.0f:1.0f)*(float)dRandom( 0, range );
}

template<class Type>
inline Type _abs( Type value )
{
	return ( (value) < 0 ? -(value) : (value) );
}

template<class Type>
inline const Type & _max( Type & x, Type & y )
{
	return ( (x) < (y) ? (y) : (x) );
}

template<class Type>
inline const Type & _min( Type & x, Type & y )
{
	return ( (x) > (y) ? (y) : (x) );
}

template<class Type>
inline void _swap( Type & s1, Type & s2 )
{
	s1 ^= s2 ^= s1 ^= s2;
}
template<class Type>
inline Type _round( Type x )
{
	return ((x>0) ? floor(x+0.5f) : ceil(x-0.5f));
}

#endif // __GLOBAL_H__