// definekeyboard.h

#ifndef _PROGRAMCOMMON_WZDEFINEKEYBOARD_H_
#define _PROGRAMCOMMON_WZDEFINEKEYBOARD_H_

#include "wztypedef.h"

// WM_KEYDOWN 등의 메시지에서 lParam 으로부터 KSC 코드 얻기
#define lParamToKsc( lParam )   ( ( (LONG)(BYTE)( lParam >> 16 ) ) | ( lParam & 0x1000 ) )

// 키보드 스캔코드 define
#define KSC_ESC             (0x0001)
#define KSC_F1              (0x003b)
#define KSC_F2              (0x003c)
#define KSC_F3              (0x003d)
#define KSC_F4              (0x003e)
#define KSC_F5              (0x003f)
#define KSC_F6              (0x0040)
#define KSC_F7              (0x0041)
#define KSC_F8              (0x0042)
#define KSC_F9              (0x0043)
#define KSC_F10             (0x0044)
#define KSC_F11             (0x0057)
#define KSC_F12             (0x0058)

#define KSC_BACKQUOTE       (0x0029)        // `
#define KSC_1               (0x0002)
#define KSC_2               (0x0003)
#define KSC_3               (0x0004)
#define KSC_4               (0x0005)
#define KSC_5               (0x0006)
#define KSC_6               (0x0007)
#define KSC_7               (0x0008)
#define KSC_8               (0x0009)
#define KSC_9               (0x000a)
#define KSC_0               (0x000b)
#define KSC_SUBTRACT        (0x000c)        // -
#define KSC_EQUALS          (0x000d)        // =
#define KSC_BACKSPACE       (0x000e)        // <-

#define KSC_TAB             (0x000f)
#define KSC_Q               (0x0010)
#define KSC_W               (0x0011)
#define KSC_E               (0x0012)
#define KSC_R               (0x0013)
#define KSC_T               (0x0014)
#define KSC_Y               (0x0015)
#define KSC_U               (0x0016)
#define KSC_I               (0x0017)
#define KSC_O               (0x0018)
#define KSC_P               (0x0019)
#define KSC_OPENBRACKET     (0x001a)        // [
#define KSC_CLOSEBRACKET    (0x001b)        // ]
#define KSC_ENTER           (0x001c)

#define KSC_CAPSLOCK        (0x003a)
#define KSC_A               (0x001e)
#define KSC_S               (0x001f)
#define KSC_D               (0x0020)
#define KSC_F               (0x0021)
#define KSC_G               (0x0022)
#define KSC_H               (0x0023)
#define KSC_J               (0x0024)
#define KSC_K               (0x0025)
#define KSC_L               (0x0026)
#define KSC_SEMICOLON       (0x0027)        // ;
#define KSC_QUOTE           (0x0028)        // '

#define KSC_LEFTSHIFT       (0x002a)
#define KSC_Z               (0x002c)
#define KSC_X               (0x002d)
#define KSC_C               (0x002e)
#define KSC_V               (0x002f)
#define KSC_B               (0x0030)
#define KSC_N               (0x0031)
#define KSC_M               (0x0032)
#define KSC_COMMA           (0x0033)        // ,
#define KSC_PERIOD          (0x0034)        // .
#define KSC_SLASH           (0x0035)        // /
#define KSC_RIGHTSHIFT      (0x0036)
#define KSC_BACKSLASH       (0x002b)        // "\\"

#define KSC_LEFTCTRL        (0x001d)
#define KSC_LEFTALT         (0x0038)
#define KSC_SPACE           (0x0039)
#define KSC_RIGHTALT        (0x1038)
#define KSC_RIGHTCTRL       (0x101d)

#define KSC_PRINTSCREEN     (0x1037)
#define KSC_SCROLLLOCK      (0x0046)
#define KSC_PAUSE           (0x0045)
#define KSC_INSERT          (0x1052)
#define KSC_HOME            (0x1047)
#define KSC_PAGEUP          (0x1049)
#define KSC_DELETE          (0x1053)
#define KSC_END             (0x104f)
#define KSC_UP              (0x1048)
#define KSC_LEFT            (0x104b)
#define KSC_DOWN            (0x1050)
#define KSC_RIGHT           (0x104d)

#define KSC_NUMLOCK         (0x1045)
#define KSC_NUM_SLASH       (0x1035)        // /
#define KSC_NUM_MULTIPLY    (0x0037)        // *
#define KSC_NUM_SUBTRACT    (0x004a)        // -
#define KSC_NUM_7           (0x0047)
#define KSC_NUM_8           (0x0048)
#define KSC_NUM_9           (0x0049)
#define KSC_NUM_PLUS        (0x004e)        // +
#define KSC_NUM_4           (0x004b)
#define KSC_NUM_5           (0x004c)
#define KSC_NUM_6           (0x004d)
#define KSC_NUM_1           (0x004f)
#define KSC_NUM_2           (0x0050)
#define KSC_NUM_3           (0x0051)
#define KSC_NUM_ENTER       (0x101c)
#define KSC_NUM_0           (0x0052)
#define KSC_NUM_PERIOD      (0x0053)        // .

#endif // _PROGRAMCOMMON_WZDEFINEKEYBOARD_H_