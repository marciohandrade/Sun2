// wzloghandlercreator.h

#ifndef _PROGRAMCOMMON_WZLOGHANDLERCREATOR_H_
#define _PROGRAMCOMMON_WZLOGHANDLERCREATOR_H_

#include "wzlogtype.h"

/*
    사실상 본 파일은 그다지 필요하거나 꼭 있어야 하는 파일은 아니다.
    각 DLL에 로그 핸들러를 설치할 때 거의 공통적인 코드가 필요했기 때문에
    본 파일에서 그 공통적인 부분을 처리하고자 만들어졌다.
    이는 에러나 실수 방지 차원에서도 괜찮을 수 있다.
*/

bool NewDefaultWzLogHandler( const char* name );

bool NewWzLogHandler( const WzLogCreateInfo& wlci );
void DeleteWzLogHandler( void );

#endif // _PROGRAMCOMMON_WZLOGHANDLERCREATOR_H_