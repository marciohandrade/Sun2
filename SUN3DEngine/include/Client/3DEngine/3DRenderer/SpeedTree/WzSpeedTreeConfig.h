///////////////////////////////////////////////////////////////////////  
//  SpeedTreeRT runtime configuration #defines
//
//  (c) 2003 IDV, Inc.
//
//  *** INTERACTIVE DATA VISUALIZATION (IDV) PROPRIETARY INFORMATION ***
//
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Interactive Data Visualization and may
//  not be copied or disclosed except in accordance with the terms of
//  that agreement.
//
//      Copyright (c) 2001-2003 IDV, Inc.
//      All Rights Reserved.
//
//      IDV, Inc.
//      1233 Washington St. Suite 610
//      Columbia, SC 29201
//      Voice: (803) 799-1699
//      Fax:   (803) 931-0320
//      Web:   http://www.idvinc.com

#ifndef _3DRENDERER_SPEEDTREECONFIG_H_
#define _3DRENDERER_SPEEDTREECONFIG_H_

#include "speedtreert.h"

const int       c_nNumWindMatrices = 4;

const float c_fNearLodFactor=100.f;
const float c_fFarLodFactor=1000.0f;

//임시 포그값 
const float     c_fFogStart = 100.0f;
const float     c_fFogEnd = 1000.0f;
const float     c_fFogLinearScale = (1.0f / (c_fFogEnd - c_fFogStart));

// vertex shader constant locations
const int c_nVertexShader_LeafLightingAdjustment = 70;
const int c_nVertexShader_Light = 71;
const int c_nVertexShader_Material = 74;
const int c_nVertexShader_TreePos = 32;
const int c_nVertexShader_WindMatrices = 16;
const int c_nVertexShader_LeafTables = 36;



// setup lighting (enable ONE of the two below)
//#define WRAPPER_USE_STATIC_LIGHTING
#define WRAPPER_USE_DYNAMIC_LIGHTING //무조건 다이나믹

#if defined WRAPPER_USE_STATIC_LIGHTING && defined WRAPPER_USE_DYNAMIC_LIGHTING
    #error Please define exactly one lighting mode
#endif

// setup wind (enable ONE of the three below)
#define WRAPPER_USE_GPU_WIND
//#define WRAPPER_USE_CPU_WIND
//#define WRAPPER_USE_NO_WIND

#if defined WRAPPER_USE_GPU_WIND && defined WRAPPER_USE_CPU_WIND
    #error Please define exactly one lighting mode
#elif defined WRAPPER_USE_GPU_WIND && defined WRAPPER_USE_NO_WIND
    #error Please define exactly one lighting mode
#elif defined WRAPPER_USE_CPU_WIND && defined WRAPPER_USE_NO_WIND
    #error Please define exactly one lighting mode
#endif


// leaf placement algorithm (enable ONE of the two below)
#define WRAPPER_USE_GPU_LEAF_PLACEMENT
//#define WRAPPER_USE_CPU_LEAF_PLACEMENT

#if defined WRAPPER_USE_GPU_LEAF_PLACEMENT && defined WRAPPER_USE_CPU_LEAF_PLACEMENT
    #error Please define exactly one leaf placement algorithm
#endif


// texture coordinates (enable this define for DirectX-based engines)
#define WRAPPER_FLIP_T_TEXCOORD


// up vector //작동 안됨(정식버전 에서만 사용가능)
#define WRAPPER_UP_POS_Y
//#define WRAPPER_UP_POS_Z

#if defined WRAPPER_UP_POS_Y && defined WRAPPER_UP_POS_Z
    #error Please define exactly one up vector
#endif


//?? 아직 모름
// loading from STF or clones/instances? (enable ONE of the two below)
//#define WRAPPER_FOREST_FROM_STF
#define WRAPPER_FOREST_FROM_INSTANCES

#if defined WRAPPER_FOREST_FROM_STF && defined WRAPPER_FOREST_FROM_INSTANCES
    #error Please define exactly one loading mechanism
#endif


// billboard modes 
#define WRAPPER_BILLBOARD_MODE
#define WRAPPER_RENDER_HORIZONTAL_BILLBOARD


// render self-shadows
#define WRAPPER_RENDER_SELF_SHADOWS

#endif // _3DRENDERER_SPEEDTREECONFIG_H_