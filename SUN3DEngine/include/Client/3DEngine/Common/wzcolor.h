// wzcolor.h

#ifndef _COMMON_WZCOLOR_H_
#define _COMMON_WZCOLOR_H_

#include "wzcommontypedef.h"

typedef DWORD WzColor;

// WzColor���� ARGB ��
#define WzColor_RGBA( r, g, b, a )      ( (WzColor)( (((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff) ) )
#define WzColor_RGB( r, g, b )          WzColor_RGBA( r, g, b, 0xff )

// Į�󰪿��� �ش� ��(ARGB ��)�� ����
#define Alpha_WzColor( wc )             ( (BYTE)(((wc) >> 24) & 0xff) )
#define Red_WzColor( wc )               ( (BYTE)(((wc) >> 16) & 0xff) )
#define Green_WzColor( wc )             ( (BYTE)(((wc) >> 8) & 0xff) )
#define Blue_WzColor( wc )              ( (BYTE)((wc) & 0xff) )

#define WzColorToD3DCOLOR( wc )         (wc)

// ARGB�� �ش� ���� ����
#define WzColor_ChangeAlpha( wc, a )    ( ((wc) & 0x00ffffff) | (((a) & 0xff) << 24) )
#define WzColor_ChangeRed( wc, r )      ( ((wc) & 0xff00ffff) | (((r) & 0xff) << 16) )
#define WzColor_ChangeGreen( wc, g )    ( ((wc) & 0xffff00ff) | (((g) & 0xff) << 8) )
#define WzColor_ChangeBlue( wc, b )     ( ((wc) & 0xffffff00) | (((b) & 0xff)) )

// Į�󰪸� ��Į���(���� ����)
#define WzColor_MultiplyFloat( wc, fMul )\
    WzColor_RGBA( (BYTE)( (float)Red_WzColor(wc) * fMul ),\
                  (BYTE)( (float)Green_WzColor(wc) * fMul ),\
                  (BYTE)( (float)Blue_WzColor(wc) * fMul ),\
                  Alpha_WzColor(wc) )

// Į�� ������ ��
#define WzColor_Multiply( wc1, wc2 )\
    WzColor_RGBA( (BYTE)( (float)Red_WzColor(wc1) * (float)Red_WzColor(wc2) / 255.0f ),\
                  (BYTE)( (float)Green_WzColor(wc1) * (float)Green_WzColor(wc2) / 255.0f ),\
                  (BYTE)( (float)Blue_WzColor(wc1) * (float)Blue_WzColor(wc2) / 255.0f),\
                  (BYTE)( (float)Alpha_WzColor(wc1) * (float)Alpha_WzColor(wc2) / 255.0f ) )

#endif // _COMMON_WZCOLOR_H_