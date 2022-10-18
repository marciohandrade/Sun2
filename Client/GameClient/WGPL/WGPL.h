#pragma once

//#ifdef _C9_USE_WGPL_

#include "WGPLShuffle.h"
#include "WGPLSecureContainer.h"

// ***********************************************************************
//						MEMORY SHUFFLE INTERFACE
// ***********************************************************************

// -----------------------------------------------------------------------
// RELEASE/DEBUG SHUFFLE MODE DEFINES
//
// [EXAMPLE]
// SHUFFLE(int) value = 100;
// -----------------------------------------------------------------------
#define SHUFFLE_SIMPLE(TYPE)		WGPL::Shuffle<TYPE, false, 1>		// �޸� ��ȣȭ�� ���� (üũ�� �˻� ����, ���� ����)
#define SHUFFLE_SECURE(TYPE)		WGPL::Shuffle<TYPE, true, 2>		// �޸� ��ȣȭ, üũ�� �˻�, ���� 1��
#define SHUFFLE_VERY_SECURE(TYPE)	WGPL::Shuffle<TYPE, true, 11>		// �޸� ��ȣȭ, üũ�� �˻�, ���� 10��
#define SHUFFLE(TYPE)				SHUFFLE_SIMPLE(TYPE)				// ����Ʈ ����

// -----------------------------------------------------------------------
// RELEASE/DEBUG SHUFFLE MODE DEFINES
// _tick : shuffle period in ms
//
// [EXAMPLE]
// SHUFFLE(int) value = 100;
// SHUFFLE_TIME(val, MINIMUM_ALLOC_TIME);
// -----------------------------------------------------------------------
#define SHUFFLE_TIME(shuffle_obj, _tick) shuffle_obj.SetShuffleTime(_tick);

// for interoperability to ngpl
#define CODE_GUARD

//
//#else
////
//#define SHUFFLE(TYPE) TYPE
//#define CODE_GUARD
////
////
//#endif
