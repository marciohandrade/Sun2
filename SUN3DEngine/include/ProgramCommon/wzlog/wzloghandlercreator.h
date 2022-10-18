// wzloghandlercreator.h

#ifndef _PROGRAMCOMMON_WZLOGHANDLERCREATOR_H_
#define _PROGRAMCOMMON_WZLOGHANDLERCREATOR_H_

#include "wzlogtype.h"

/*
    ��ǻ� �� ������ �״��� �ʿ��ϰų� �� �־�� �ϴ� ������ �ƴϴ�.
    �� DLL�� �α� �ڵ鷯�� ��ġ�� �� ���� �������� �ڵ尡 �ʿ��߱� ������
    �� ���Ͽ��� �� �������� �κ��� ó���ϰ��� ���������.
    �̴� ������ �Ǽ� ���� ���������� ������ �� �ִ�.
*/

bool NewDefaultWzLogHandler( const char* name );

bool NewWzLogHandler( const WzLogCreateInfo& wlci );
void DeleteWzLogHandler( void );

#endif // _PROGRAMCOMMON_WZLOGHANDLERCREATOR_H_