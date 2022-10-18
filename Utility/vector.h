#ifndef N_VECTOR_H
#define N_VECTOR_H
//------------------------------------------------------------------------------
/**
    Implement 2, 3 and 4-dimensional vector classes
    @author
    - RadonLabs GmbH 
    @since
    - 2005.7.06
    @remarks
    - 지완 추가 
*/
#include "../ProgramCommon/Define.h"

#ifdef __USE_SSE__

// sse vector classes
#include "_vector2.h"
#include "_vector3_sse.h"
#include "_vector4_sse.h"

typedef _vector2 vector2; 
typedef _vector3_sse vector3;
typedef _vector4_sse vector4;

#else

// generic vector classes
#include "_vector2.h"
#include "_vector3.h"
#include "_vector4.h"

typedef _vector2 vector2;
typedef _vector3 vector3;
typedef _vector4 vector4;

#endif

//------------------------------------------------------------------------------
#endif
