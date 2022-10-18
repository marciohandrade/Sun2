// Singleton.h: interface for the Singleton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SINGLETON_H_
#define SINGLETON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <assert.h>
#include <windows.h>

namespace util {

// �̱��� ���� ���
struct SingletonRelease
{
    enum Value
    {
        kManual = 1, // ����
        kAuto = 2 // �ڵ�
    };
};

// ���� ���� ���� (�⺻��)
template <typename T, SingletonRelease::Value kRelease = SingletonRelease::kManual>
class Singleton
{
protected:
    Singleton()
    {		
    }
    ~Singleton()
    {
    }

public:
    static T* Instance()
    {
        if (instance_ == NULL)
        {
            instance_ = new T;
        }
        return instance_;
    }

    static void DestroyInstance()
    {
        if (instance_ != NULL) 
        {		
            delete instance_;
            instance_ = NULL;
        }
    }

private:
    static T* instance_;
};

// �ڵ� ���� ����
template <typename T>
class Singleton<T, SingletonRelease::kAuto>
{
protected:
    Singleton()
    {		
    }
    ~Singleton()
    {
    }

public:
    static T* Instance()
    {
        static T instance;
        return &instance;
    }
};

template<typename T, SingletonRelease::Value kRelease> 
T* Singleton< T, kRelease >::instance_ = NULL;

} // util

#endif // SINGLETON_H_