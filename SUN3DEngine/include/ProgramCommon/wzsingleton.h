// wzsingleton.h

#ifndef _PROGRAMCOMMON_WZSINGLETON_H_
#define _PROGRAMCOMMON_WZSINGLETON_H_

#include "wztypedef.h"
#include "wzassert.h"

//------------------------------------------------------------------------------
/**
    @class CWzSingleton
*/
template <class T>
class CWzSingleton  
{
public:
    // 생성자
    CWzSingleton( void )
    {
        WzAssert( !s_instance );

        // 다중상속의 경우 이 포인터값이 다를 수 있다. 그래서 이 값을 아래에서 더해준다.
        int offset = (int)(T*)1 - (int)(CWzSingleton<T>*)(T*)1;

        s_instance = (T*)( (int)this + offset );
    }

    // 소멸자
    virtual ~CWzSingleton( void )
    {
        s_instance = NULL;
    }

    // 인스턴스 얻기
    static T* GetSingleton( void )
    {
        return s_instance;
    }

private:
    static T* s_instance;
};

template <class T>
T* CWzSingleton<T>::s_instance = NULL;

#endif // _PROGRAMCOMMON_WZSINGLETON_H_