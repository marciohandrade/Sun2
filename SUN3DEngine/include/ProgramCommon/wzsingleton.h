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
    // ������
    CWzSingleton( void )
    {
        WzAssert( !s_instance );

        // ���߻���� ��� �� �����Ͱ��� �ٸ� �� �ִ�. �׷��� �� ���� �Ʒ����� �����ش�.
        int offset = (int)(T*)1 - (int)(CWzSingleton<T>*)(T*)1;

        s_instance = (T*)( (int)this + offset );
    }

    // �Ҹ���
    virtual ~CWzSingleton( void )
    {
        s_instance = NULL;
    }

    // �ν��Ͻ� ���
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